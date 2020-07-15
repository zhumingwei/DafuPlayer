//
// Created by zhumingwei on 2020/7/6.
//

#ifndef DAFUPLAYER_PLAYERMESSAGE_H
#define DAFUPLAYER_PLAYERMESSAGE_H


//Message what
#define MSG_FLUSH               0x00    //默认
#define MSG_ERROR               0x10    //出错回调
#define MSG_PREPARED            0x20    //准备完成回调
#define MSG_STARTED             0x30    //已经开始
#define MSG_COMPLETED           0x40    //播放完成回调

#define MSG_OPEN_INPUT          0x50    //打开文件
#define MSG_FIND_STREAM_INFO    0x51    //查找媒体流信息
#define MSG_PREPARE_DECODER     0X52    //准备解码器
#define MSG_VIDEO_SIZE_CHANGED   0X53    //视频大小变化
#define MSG_SAR_CHANGED          0X54    //长宽比变化
#define MSG_AUDIO_START         0X55    //开始音频解码
#define MSG_AUDIO_RENDERING_START 0X56  //音频渲染开始（播放开始）
#define MSG_VIDEO_START         0X57    //开始视频解码
#define MSG_VIDEO_RENDERING_START   0X58    //视频渲染开始
#define MSG_VIDEO_ROTATION_CHANGED   0X59    //旋转角度变化

#define MSG_BUFFERING_START         0X60    //缓冲开始
#define MSG_BUFFERING_END           0X61    //缓冲完成
#define MSG_BUFFERING_UPDATE        0X62    //缓冲更新
#define MSG_BUFFERING_TIME_UPDATE   0X63    //缓冲时间更新

#define MSG_SEEK_COMPLETE          0X70     //定位完成
#define MSG_PLAYBACK_STATE_CHANGE  0X80     //播放状态变更
#define MSG_TIMED_TXET               0X90    //字幕

#define MSG_REQUEST_PREPARE         0X200   //异步请求准备
#define MSG_REQUEST_START           0X201   //异步请求开始
#define MSG_REQUEST_PAUSE           0X202   //请求暂停
#define MSG_REQUEST_SEEK            0X203   //请求定位

#define MSG_CURRENT_POSITION        0X300   //当前时钟

#endif //DAFUPLAYER_PLAYERMESSAGE_H
