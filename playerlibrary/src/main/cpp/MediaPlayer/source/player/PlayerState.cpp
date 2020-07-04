//
// Created by zhumingwei on 2020/7/4.
//

#include <AndroidLog.h>
#include "PlayerState.h"

PlayerState::PlayerState(){
    init();
    reset();
}

PlayerState::~PlayerState() {
    reset();
    if (messageQueue) {
        messageQueue->release();
        delete messageQueue;
        messageQueue = nullptr;
    }
}

void PlayerState::init() {
    sws_dict = static_cast<AVDictionary *>(malloc(sizeof(AVDictionary)));
    memset(sws_dict, 0, sizeof(AVDictionary));
    swr_opts = static_cast<AVDictionary *>(malloc(sizeof(AVDictionary)));
    memset(swr_opts, 0, sizeof(AVDictionary));
    format_opts = static_cast<AVDictionary *>(malloc(sizeof(AVDictionary)));
    memset(format_opts, 0, sizeof(AVDictionary));

    iformat = NULL;
    url = NULL;
    headers = NULL;

    audioCodecName = NULL;
    videoCodecName = NULL;
    messageQueue = new AVMessageQueue();
}

void PlayerState::reset() {
    //todo
}