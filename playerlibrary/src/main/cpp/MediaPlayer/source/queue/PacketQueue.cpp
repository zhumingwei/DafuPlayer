//
// Created by zhumingwei on 2020/7/3.
//

#include "PacketQueue.h"

PacketQueue::PacketQueue(){
    abort_request = 0;
    first_pkt = NULL;
    last_pkt = NULL;
    nb_packets = 0;
    size = 0;
    duration = 0;
}

PacketQueue::~PacketQueue() {
    abort();
    flush();
}

/**
 * 入队数据包
 * @param pkt
 * @return
 */
int PacketQueue::put(AVPacket *pkt) {
    PacketList *pkt1;

    if (abort_request){
        return -1;
    }
    pkt1 = static_cast<PacketList *>(av_malloc(sizeof(PacketList)));
    if (!pkt1){
        return -1;
    }
    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    if (!last_pkt){
        first_pkt = pkt1;
    } else {
        last_pkt->next = pkt1;
    }

    last_pkt = pkt1;
    nb_packets++;
    size += pkt1->pkt.size + sizeof(*pkt1);
    duration += pkt1->pkt.duration;
    return 0;
}

/**
 * 入队数据包
 * @param pkt
 * @return
 */
int PacketQueue::pushPacket(AVPacket *pkt) {
    int ret;
    mMutex.lock();
}

//todo
int PacketQueue::pushNullPacket(int stream_index) {}