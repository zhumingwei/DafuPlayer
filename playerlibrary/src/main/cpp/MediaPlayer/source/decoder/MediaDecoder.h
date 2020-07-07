//
// Created by zhumingwei on 2020/7/7.
//

#ifndef DAFUPLAYER_MEDIADECODER_H
#define DAFUPLAYER_MEDIADECODER_H


#include <Thread.h>
#include <AndroidLog.h>
#include <player/PlayerState.h>
#include <queue/PacketQueue.h>
#include <queue/FrameQueue.h>

class MediaDecoder : public Runnable {
public:
    MediaDecoder(AVCodecContext *avctx, AVStream *stream, int streamIndex, PlayerState *playerState);

    virtual ~MediaDecoder();

    virtual void start();

    virtual void stop();

    virtual void flush();

    int pushPacket(AVPacket *pkt);

    int getPacketSize();

    int getStreamIndex();

    AVStream *getStream();

    AVCodecContext *getCodecContext();

    int getMemorySize();

    int hasEnoughPackets();

    virtual void run();

protected:
    Mutex mMutex;
    Condition mCondition;
    bool abortRequest;
    PlayerState *playerState;
    PacketQueue *packetQueue;   //数据包队列
    AVCodecContext *pCodecCtx;
    AVStream *pStream;
    int streamIndex;
};


#endif //DAFUPLAYER_MEDIADECODER_H
