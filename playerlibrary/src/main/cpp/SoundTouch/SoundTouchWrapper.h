//
// Created by zhumingwei on 2020/7/2.
//

#ifndef DAFUPLAYER_SOUNDTOUCHWRAPPER_H
#define DAFUPLAYER_SOUNDTOUCHWRAPPER_H

#include <stdint.h>
#include "include/SoundTouch.h"

using namespace std;
using namespace soundtouch;

class SoundTouchWrapper {
public:
    SoundTouchWrapper();
    virtual ~SoundTouchWrapper();
    //初始化
    void create();
    //转化
    int translate(
            short* data,
            float speed,
            float pitch,
            int len,
            int bytes_per_sample,
            int n_channel,
            int n_sampleRate
            );
    //清空
    void flush();
    //销毁
    void destroy();
    //获取SoundTouch对象
    SoundTouch * getSoundTouch();

private:
    SoundTouch *mSoundTouch;
};


#endif //DAFUPLAYER_SOUNDTOUCHWRAPPER_H
