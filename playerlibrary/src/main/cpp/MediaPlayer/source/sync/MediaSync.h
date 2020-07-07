//
// Created by zhumingwei on 2020/7/7.
//

#ifndef DAFUPLAYER_MEDIASYNC_H
#define DAFUPLAYER_MEDIASYNC_H

#include <Thread.h>
#include <player/PlayerState.h>

/**
 * 视频同步器
 */
class MediaSync : public Runnable {
public:
    MediaSync(PlayerState * playerState);
    virtual ~MediaSync();
    void reset();
    //todo
    void start(Video);
};


#endif //DAFUPLAYER_MEDIASYNC_H
