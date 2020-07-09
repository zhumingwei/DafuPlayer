//
// Created by zhumingwei on 2020/7/9.
//

#include <device/android/SLESDevice.h>
#include "MediaPlayer.h"

/**
 * FFmpeg操作锁管理回调  //fixme
 * @param mtx
 * @param op
 * @return
 */
static int lockmgrCallback(void **mtx, enum AVLockOp op){
    switch (op) {
        case AV_LOCK_CREATE: {
            *mtx = new Mutex();
            if (!*mtx){
                av_log(NULL, AV_LOG_FATAL, "failed to create mutex.\n");
                return 1;
            }
            return 0;
        }

        case AV_LOCK_OBTAIN: {
            if (!*mtx){
                return 1;
            }
            return ((Mutex *)(*mtx))->lock() != 0;
        }

        case AV_LOCK_RELEASE:{
            if (!*mtx) {
                return 1;
            }
            return ((Mutex *)(*mtx))->unlock() != 0;
        }

        case AV_LOCK_DESTROY: {
            if(! *mtx){
                delete(*mtx);
                *mtx = NULL;
            }
            return 0;
        }
    }
    return 1;
}

MediaPlayer::MediaPlayer() {
    av_register_all();
    avformat_network_init();
    playerState = new PlayerState();
    mDuration = -1;
    audioDecoder = NULL;
    videoDecoder = NULL;
    pFormatCtx = NULL;
    lastPaused = -1;
    attachmentRequest = 0;

#if defined(__ANDROID__)
    audioDevice = new SLESDevice();
#else
    audioDevice = new AudioDevice();
#endif

    mediaSync = new MediaSync(playerState);
    audioResampler = NULL;
    readThread = NULL;
    mExit = true;

    //注册一个多线程锁管理回调，主要是解决多个视频源时保持avcodec_open/close 的原子操作
    if (av_lockmgr_register(lockmgrCallback)){
        av_log(NULL, AV_LOG_FATAL, "Could not initialize lock manager!\n");
    }

}

MediaPlayer::~MediaPlayer(){
    avformat_network_deinit();
    av_lockmgr_register(NULL);
}

status_t MediaPlayer::reset() {
    stop();
    if (mediaSync){
        mediaSync->reset();
        delete mediaSync;
        mediaSync = NULL;
    }
    if (audioDecoder){
        audioDecoder->stop();
        delete audioDecoder;
        audioDecoder = NULL;
    }
    if (videoDecoder){
        videoDecoder->stop();
        delete videoDecoder;
        videoDecoder = NULL;
    }
    if (audioDevice){
        audioDevice->stop();
        delete audioDevice;
        audioDevice = NULL;
    }

    if (audioResampler){
        delete audioResampler;
        audioResampler = NULL;
    }
    if (pFormatCtx != NULL){
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = NULL;
    }
    if (playerState) {
        delete playerState;
        playerState = NULL;
    }
    return NO_ERROR;
}

void MediaPlayer::setDataSource(const char *url, int64_t offset, const char *headers) {
    Mutex::Autolock lock(mMutex);
    playerState->url = av_strdup(url);// fixme diff from common copy
    playerState->offset = offset;
    if (headers){
        playerState->headers = av_strdup(headers);
    }
}

void MediaPlayer::setVideoDevice(VideoDevice *videoDevice) {
    Mutex::Autolock lock(mMutex);
    mediaSync->setVideoDevice(videoDevice);
}

status_t MediaPlayer::prepare() {
    Mutex::Autolock lock(mMutex);
    if (!playerState->url){
        return BAD_VALUE;
    }
    playerState->abortRequest = 0;
    if (!readThread){
        readThread = new Thread(this);
        readThread->start();
    }
    return NO_ERROR;
}

status_t MediaPlayer::prepareAsync() {
    Mutex::Autolock lock(mMutex);
    if (!playerState->url){
        return BAD_VALUE;
    }
    //发送消息请求准备
    if (playerState->messageQueue) {
        playerState->messageQueue->postMessage(MSG_REQUEST_PREPARE);
    }
    return NO_ERROR;
}

void MediaPlayer::start() {
    Mutex::Autolock lock(mMutex);
    playerState->abortRequest = 0;
    playerState->pauseRequest = 0;
    mExit = false;
    mCondition.signal();
}

void MediaPlayer::pause() {
    Mutex::Autolock autolock(mMutex);
    playerState->pauseRequest = 1;
    mCondition.signal();
}

void MediaPlayer::stop() {
    mMutex.lock();
    playerState->abortRequest = 1;
    mCondition.signal();
    mMutex.unlock();
    mMutex.lock();
    while (!mExit){
        mCondition.wait(mMutex);
    }
    mMutex.unlock();
    if (readThread != NULL){
        readThread->join();
        delete(readThread);
        readThread = NULL;
    }
}

void MediaPlayer::seekTo(float timeMs) {
    //when is a live media stream, duration si -1
    if (!playerState->realTime && mDuration < 0){
        return;
    }
    //等待上一次操作完成
    mMutex.lock();
    while (playerState -> seekRequest){
        mCondition.wait(mMutex);
    }
    mMutex.unlock();

    if (!playerState->seekRequest) {
        int64_t start_time = 0;
        int64_t seek_pos = av_rescale(timeMs, AV_TIME_BASE, 1000);
        start_time = pFormatCtx ? pFormatCtx->start_time : 0;
        if (start_time > 0 && start_time != AV_NOPTS_VALUE){
            seek_pos += start_time;
        }
        playerState->seekPos = seek_pos;
        playerState->seekRel = 0;
        playerState->seekFlags &= ~AVSEEK_FLAG_BYTE;
        mCondition.signal();
    }
}

void MediaPlayer::setLooping(int looping) {
    mMutex.lock();
    playerState->loop = looping;
    mCondition.signal();
    mMutex.unlock();
}

void MediaPlayer::setVolume(float leftVolume, float rightVolume) {
    if (audioDevice){
        audioDevice->setStereoVolume(leftVolume, rightVolume);
    }
}

void MediaPlayer::setMute(int mute) {
    mMutex.lock();
    playerState->mute = mute;
    mCondition.signal();
    mMutex.unlock();
}

void MediaPlayer::setRate(float rate) {
    mMutex.lock();
    playerState->playbackRate = rate;
    mCondition.signal();
    mMutex.unlock();
}

void MediaPlayer::setPitch(float pitch) {
    mMutex.lock();
    playerState->playbackPitch = pitch;
    mCondition.signal();
    mMutex.unlock();
}

int MediaPlayer::getRotate() {
    Mutex::Autolock lock(mMutex);
    if (videoDecoder){
        return videoDecoder->getRotate();
    }
    return 0;
}

int MediaPlayer::getVideoWidth() {
    Mutex::Autolock lock(mMutex);
    if (videoDecoder){
        return videoDecoder->getCodecContext()->width;
    }
    return 0;
}

int MediaPlayer::getVideoHeight() {
    Mutex::Autolock lock(mMutex);
    if (videoDecoder){
        return videoDecoder->getCodecContext()->height;
    }
    return 0;
}

//TODO
long MediaPlayer::getCurrentPosition() {
    Mutex::Autolock lock(mMutex);
    int64_t currentPosition = 0;
    //处于定位
    if (playerState->seekRequest){
        currentPosition = playerState->seekPos;
    } else {
        //起始延时
        int64_t start_time = pFormatCtx->start_time;
        int64_t start_diff = 0;
    }
}