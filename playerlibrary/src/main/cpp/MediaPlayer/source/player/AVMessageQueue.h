//
// Created by zhumingwei on 2020/7/4.
//

#ifndef DAFUPLAYER_AVMESSAGEQUEUE_H
#define DAFUPLAYER_AVMESSAGEQUEUE_H


extern "C" {
#include <cstring>
#include <libavutil/mem.h>
};


#include <Mutex.h>
#include <Condition.h>
#include "PlayerMessage.h"

typedef struct AVMessage {
    int what;
    int arg1;
    int arg2;
    void *obj;

    void (*free)(void *obj);

    struct AVMessage *next;
} AVMessage;

inline static void message_init(AVMessage *msg) {
    memset(msg, 0, sizeof(AVMessage));
}

inline static void message_free(void *obj) {
    av_free(obj);
}

class AVMessageQueue {
public:
    AVMessageQueue();
    virtual ~AVMessageQueue();

    void start();
    void stop();
    void flush();
    void release();
    void postMessage(int what);
    void postMessage(int what, int arg1);
    void postMessage(int what, int arg1, int arg2);
    void postMessage(int what, int arg1, int arg2, void *obj, int len);
    int getMessage(AVMessage *msg);
    int getMessage(AVMessage *msg,int block);
    void removeMessage(int what);

private:
    int putMessage(AVMessage *msg);

private:
    Mutex mMutex;
    Condition mCondition;
    AVMessage *mFirstMsg, *mLastMsg;
    bool abortRequest;
    int mSize;

};


#endif //DAFUPLAYER_AVMESSAGEQUEUE_H
