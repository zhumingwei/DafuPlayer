//
// Created by zhumingwei on 2020/7/7.
//

#ifndef DAFUPLAYER_SLESDEVICE_H
#define DAFUPLAYER_SLESDEVICE_H


#include <device/AudioDevice.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SLESDevice : public AudioDevice{
public:
    SLESDevice();
    virtual ~SLESDevice();
    int open(const AudioDeviceSpec *desired, AudioDeviceSpec *obtained) override;
    void start() override;
    void stop() override;
    void pause() override;
    void resume() override;
    void flush() override;
    void setStereoVolume(float left_volume, float right_volume) override;
    virtual void run();

private:
    // 转换成SL采样率
    SLuint32 getSlSampleRate(int sampleRate);
    //获取SLES音量
    SLmillibel getAmplificationLevel(float volumeLevel);

private:
    //引擎接口
    SLObjectItf slObject;
    SLEngineItf slEngine;

    //混音器
    SLObjectItf slOutputMixObject;

    // 播放器对象
    SLObjectItf slPlayerObject;
    SLPlayItf slPlayItf;
    SLVolumeItf slVolumeItf;

    // 缓冲队列接口
    SLAndroidSimpleBufferQueueItf slBufferQueueItf;

    AudioDeviceSpec audioDeviceSpec;    //音频设备参数
    int bytes_per_frame;                //一帧占多少字节
    int milli_per_buffer;               //一个缓冲区时长占多少
    int frames_per_buffer;              //一个缓冲区有多少帧
    int bytes_per_buffer;               //一个缓冲区的大小
    uint8_t *buffer;                    //缓冲区
    size_t buffer_capacity;             //缓冲区总大小

    Mutex mMutex;
    Condition mCondition;
    Thread *audioThread;                //音频播放线程
    int abortRequest;                   //终止标志
    int pauseRequest;                   //暂停标志
    int flushRequest;                   //刷新标志

    bool updateVolume;                  //更新音量
    float leftVolume;                   //左音量
    float rightVolume;                  //右音量
};


#endif //DAFUPLAYER_SLESDEVICE_H
