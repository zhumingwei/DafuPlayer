//
// Created by zhumingwei on 2020/7/12.
//

#include "CainMediaPlayer.h"

CainMediaPlayer::CainMediaPlayer() {
    msgThread = nullptr;
    abortRequest = true;
    videoDevice = nullptr;
    mediaPlayer = nullptr;
    mPrepareSync = false;
    mPrepareStatus = NO_ERROR;
    mAudioSessionId = 0;
    mSeeking = false;
    mSeekingPosition = 0;
}

CainMediaPlayer::~CainMediaPlayer() {

}

void CainMediaPlayer::init() {
    mMutex.lock();
    abortRequest = false;
    mCondition.signal();
    mMutex.unlock();

    mMutex.lock();
    if (videoDevice == nullptr) {
        videoDevice = new GLESDevice();
    }
    if (msgThread == nullptr) {
        msgThread = new Thread(this);
        msgThread->start();
    }
    mMutex.unlock();
}

void CainMediaPlayer::disconnect() {
    mMutex.lock();
    abortRequest = true;
    mCondition.signal();
    mMutex.unlock();

    reset();
    if (msgThread != nullptr) {
        msgThread->join();
        delete msgThread;
        msgThread = nullptr;
    }

    if (videoDevice != nullptr) {
        delete videoDevice;
        videoDevice = nullptr;
    }
    if (mListener != nullptr) {
        delete mListener;
        mListener = nullptr;
    }
}

status_t CainMediaPlayer::setDataSource(const char *url, int64_t offset, const char *headers) {
    if (url == nullptr) {
        return BAD_VALUE;
    }
    if (mediaPlayer == nullptr) {
        mediaPlayer = new MediaPlayer();
    }
    mediaPlayer->setDataSource(url, offset, headers);
    mediaPlayer->setVideoDevice(videoDevice);
    return NO_ERROR;
}

status_t CainMediaPlayer::setMetadataFilter(char **allow, char **block) {
    return NO_ERROR;
}

status_t
CainMediaPlayer::getMetadata(bool update_only, bool apply_filter, AVDictionary **metadata) {
    if (mediaPlayer != nullptr) {
        return mediaPlayer->getMetadata(metadata);
    }
    return NO_ERROR;
}

status_t CainMediaPlayer::setVideoSurface(ANativeWindow *native_window) {
    if (mediaPlayer == nullptr) {
        return NO_INIT;
    }
    if (native_window != nullptr) {
        videoDevice->surfaceCreated(native_window);
        return NO_ERROR;
    }
    return NO_ERROR;
}

status_t CainMediaPlayer::setListener(MediaPlayerListener *listener) {
    if (mListener != nullptr) {
        delete mListener;
        mListener = nullptr;
    }
    mListener = listener;
    return NO_ERROR;
}

status_t CainMediaPlayer::prepare() {
    if (mediaPlayer == nullptr) {
        return NO_INIT;
    }
    if (mPrepareSync) {
        return -EALREADY;
    }
    mPrepareSync = true;
    status_t ret = mediaPlayer->prepare();
    if (ret != NO_ERROR) {
        return ret;
    }
    if (mPrepareSync) {
        mPrepareSync = false;
    }
    return mPrepareStatus;
}

status_t CainMediaPlayer::prepareAsync() {
    if (mediaPlayer != nullptr) {
        mPrepareSync = true;
        return mediaPlayer->prepareAsync();
    }
    return INVALID_OPERATION;
}

void CainMediaPlayer::start() {
    if (mediaPlayer != nullptr) {
        mediaPlayer->start();
    }
}

void CainMediaPlayer::stop() {
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
}

void CainMediaPlayer::pause() {
    if (mediaPlayer) {
        mediaPlayer->stop();
    }
}

void CainMediaPlayer::resume() {
    if (mediaPlayer) {
        mediaPlayer->resume();
    }
}

bool CainMediaPlayer::isPlaying() {
    if (mediaPlayer) {
        return (mediaPlayer->isPlaying() != 0);
    }
    return false;
}

int CainMediaPlayer::getRotate() {
    if (mediaPlayer != nullptr) {
        return mediaPlayer->getRotate();
    }
    return 0;
}

int CainMediaPlayer::getVideoWidth() {
    if (mediaPlayer != nullptr) {
        return mediaPlayer->getVideoWidth();
    }
    return 0;
}

int CainMediaPlayer::getVideoHeight() {
    if (mediaPlayer != nullptr) {
        return mediaPlayer->getVideoHeight();
    }
    return 0;
}

status_t CainMediaPlayer::seekTo(float msec) {
    if (mediaPlayer != nullptr) {
        if (mSeeking) {
            mediaPlayer->getMessageQueue()->postMessage(MSG_REQUEST_SEEK, msec);
        } else {
            mediaPlayer->seekTo(msec);
            mSeekingPosition = (long) msec;
            mSeeking = true;
        }
    }
    return NO_ERROR;
}

long CainMediaPlayer::getCurrentPosition() {
    if (mediaPlayer != nullptr){
        if (mSeeking){
            return mSeekingPosition;
        }
        return mediaPlayer->getCurrentPosition();
    }
    return 0;
}

long CainMediaPlayer::getDuration() {
    if (mediaPlayer != nullptr){
        return mediaPlayer->getDuration();
    }
    return -1;
}

status_t CainMediaPlayer::reset() {
    mPrepareSync = false;
    if (mediaPlayer != nullptr){
        mediaPlayer -> reset();
        delete mediaPlayer;
        mediaPlayer = nullptr;
    }
    return NO_ERROR;
}

status_t CainMediaPlayer::setAudioStreamType(int type) {
    return NO_ERROR;
}

status_t CainMediaPlayer::setLooping(bool looping) {
    if (mediaPlayer != nullptr) {
        mediaPlayer->setLooping(looping);
    }
    return NO_ERROR;
}

bool CainMediaPlayer::isLooping() {
    if (mediaPlayer != nullptr){
        return (mediaPlayer->isLooping() != 0);
    }
    return false;
}

status_t CainMediaPlayer::setVolume(float leftVolume, float rightVolume) {
    if (mediaPlayer != nullptr){
        mediaPlayer->setVolume(leftVolume, rightVolume);
    }
    return NO_ERROR;
}

void CainMediaPlayer::setMute(bool mute) {
    if (mediaPlayer != nullptr) {
        mediaPlayer->setMute(mute);
    }
}

void CainMediaPlayer::setRate(float speed) {
    if (mediaPlayer != nullptr) {
        mediaPlayer->setRate(speed);
    }
}

void CainMediaPlayer::setPitch(float pitch) {
    if (mediaPlayer != nullptr){
        mediaPlayer->setPitch(pitch);
    }
}

status_t CainMediaPlayer::setAudioSessionId(int sessionId) {
    if (sessionId < 0){
        return BAD_VALUE;
    }
    mAudioSessionId = sessionId;
    return NO_ERROR;
}

int CainMediaPlayer::getAudioSessionId() {
    return mAudioSessionId;
}

void CainMediaPlayer::changeFilter(int type, const char *name) {
    if (videoDevice != nullptr){
        videoDevice->changeFilter((RenderNodeType) type, name);
    }
}

void CainMediaPlayer::changeFilter(int type, const int id) {
    if (videoDevice != nullptr) {
        videoDevice->changeFilter((RenderNodeType)type, id);
    }
}

void CainMediaPlayer::setOption(int category, const char *type, const char *option) {
        if (mediaPlayer != nullptr){
            mediaPlayer->getPlayerState()->setOption(category,type,option);
        }
}

void CainMediaPlayer::setOption(int category, const char *type, int64_t option) {
    if (mediaPlayer != nullptr){
        mediaPlayer->getPlayerState()->setOptionLong(category, type, option);
    }
}

void CainMediaPlayer::notify(int msg, int ext1, int ext2, void *obj, int len) {
    if (mediaPlayer != nullptr){
        mediaPlayer->getMessageQueue()->postMessage(msg, ext1, ext2, obj, len);
    }
}

void CainMediaPlayer::postEvent(int what, int arg1, int arg2, void *obj) {
    if(mListener != nullptr){
        mListener->notify(what, arg1, arg2, obj);
    }
}

void CainMediaPlayer::run() {
    int retval;
    while (true){
        if (abortRequest){
            break;
        }

        // 如果此时播放器还没有准备好，则睡眠10毫秒，等待播放器初始化
        if (!mediaPlayer || !mediaPlayer->getMessageQueue()){
            av_usleep(10 * 1000);
            continue;
        }

        AVMessage msg;
        retval = mediaPlayer->getMessageQueue()->getMessage(&msg);
        if (retval < 0){
            ALOGE("getMessage error");
            break;
        }
        assert(retval > 0);
        switch (msg.what){
            case MSG_FLUSH: {
                ALOGD("CainMediaPlayer is flushing.\n");
                postEvent(MEDIA_NOP, 0 ,0);
                break;
            }

            case MSG_ERROR: {
                ALOGD("CainMediaPlayer occurs error: %d\n", msg.arg1);
                if (mPrepareSync){
                    mPrepareSync = false;
                    mPrepareStatus = msg.arg1;
                }
                postEvent(MEDIA_ERROR, msg.arg1, 0);
                break;
            }

            //todo
        }

    }
}
























