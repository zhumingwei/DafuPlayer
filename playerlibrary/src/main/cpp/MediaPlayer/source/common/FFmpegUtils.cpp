//
// Created by zhumingwei on 2020/7/2.
//


#include "FFmpegUtils.h"

/**
 * 过滤解码器属性
 * @param opts
 * @param codec_id
 * @param s
 * @param st
 * @param codec
 * @return
 */
AVDictionary *
filterCodecOptions(AVDictionary *opts, enum AVCodecID codec_id, AVFormatContext *s, AVStream *st,
                   AVCodec *codec) {
    AVDictionary *ret = NULL;
    AVDictionaryEntry *t = NULL;
    int flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
            :AV_OPT_FLAG_DECODING_PARAM;
    char prefix = 0;
    const AVClass *cc = avcodec_get_class();

    if(!codec) {
        codec = s->oformat ? avcodec_find_decoder(codec_id)
                :avcodec_find_decoder(codec_id);
    }
    switch (st->codecpar->codec_type){
        case AVMEDIA_TYPE_VIDEO: {
            prefix = 'v';
            flags |= AV_OPT_FLAG_VIDEO_PARAM;
            break;
        }
        case AVMEDIA_TYPE_AUDIO: {
            prefix = 'a';
            flags |= AV_OPT_FLAG_AUDIO_PARAM;
            break;
        }
        case AVMEDIA_TYPE_SUBTITLE: {
            prefix = 's';
            flags |= AV_OPT_FLAG_SUBTITLE_PARAM;
            break;
        }
    }

    while(t = av_dict_get(opts, "",t,AV_DICT_IGNORE_SUFFIX)){
        char *p = strchr(t->key, ':');

        /*
         * check stream specification in opt name
         */
        if (p){

        }
    }

    return ret;
}

int checkStreamSpecifier(AVFormatContext *s, AVStream *st, const char *spec) {
    return 0;
}

AVDictionary **setupStreamInfoOptions(AVFormatContext *s, AVDictionary *codec_opts) {
    return nullptr;
}

void printError(const char *filename, int err) {

}

double getRotation(AVStream *st) {
    return 0;
}

int isRealTime(AVFormatContext *s) {
    return 0;
}
