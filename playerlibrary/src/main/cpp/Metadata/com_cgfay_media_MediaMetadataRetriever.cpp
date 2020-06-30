//
// Created by zhumingwei on 2020/6/29.
//

#include <jni.h>
#include <AndroidLog.h>

#include "MediaMetadataRetriever.h"

extern "C" {
#include <libavcodec/jni.h>
}

struct retriever_fields_t {
    jfieldID context;
};

static retriever_fields_t fields;
static Mutex sLock;

static const char *const RETRIEVER_CLASS_NAME = "com/zhumingwei/playerlibrary/CainMediaMetadataRetriever";

static MediaMetadataRetriever *getRetriever(JNIEnv *env, jobject thiz){
    MediaMetadataRetriever *retriever =(MediaMetadataRetriever *) env->GetLongField(thiz,fields.context);
    return retriever;
}
// -------------------------------------------------------------------------------------------------

//todo

static void  CainMediaMetadataRetriever_setDataSourceAndHeaders(JNIEnv *env, jobject thiz,jstring path_,jobjectArray keys, jobjectArray values){
    ALOGV("setDataSource");
    MediaMetadataRetriever *retriever = getRetriever(env,thiz);
}

static void  CainMediaMetadataRetriever_setDataSource(JNIEnv *env, jobject thiz, jstring path_){
    CainMediaMetadataRetriever_setDataSourceAndHeaders(env,thiz,path_,NULL,NULL);
}

static void CainMediaMetadataRetriever_setDataSourceFD(JNIEnv *env, jobject thiz,
            jobject fileDescriptor, jlong offset, jlong length){

}

static jbyteArray CainMediaMetadataRetriever_getFrameAtTime(JNIEnv *env, jobject thiz, jlong timeUs, jint option){

}

static jbyteArray
CainMediaMetadataRetriever_getScaleFrameAtTime(JNIEnv *env, jobject thiz, jlong timeUs, jint option,
                                               jint width, jint height) {

}

static jbyteArray
CainMediaMetadataRetriever_getEmbeddedPicture(JNIEnv *env, jobject thiz, jint pictureType) {

}

static jstring
CainMediaMetadataRetriever_extractMetadata(JNIEnv *env, jobject thiz, jstring key_) {

}

static jstring
CainMediaMetadataRetriever_extractMetadataFromChapter(JNIEnv *env, jobject thiz, jstring key_,
                                                      jint chapter){

}

static jobject
CainMediaMetadataRetriever_getAllMetadata(JNIEnv *env, jobject thiz) {

}

static void
CainMediaMetadataRetriever_release(JNIEnv *env, jobject thiz) {

}

static void
CainMediaMetadataRetriever_setup(JNIEnv *env, jobject thiz) {

}

static void
CainMediaMetadataRetriever_native_init(JNIEnv *env) {

}

static void
CainMediaMetadataRetriever_native_finalize(JNIEnv *env, jobject thiz) {

}


// -------------------------------------------------------------------------------------------------
static JNINativeMethod nativeMethods[] = {
        {"setDataSource",              "(Ljava/lang/String;)V",                                       (void *) CainMediaMetadataRetriever_setDataSource},
        {"_setDataSource",             "(Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V", (void *) CainMediaMetadataRetriever_setDataSourceAndHeaders},
        {"setDataSource",              "(Ljava/io/FileDescriptor;JJ)V",                               (void *) CainMediaMetadataRetriever_setDataSourceFD},
        {"_getFrameAtTime",            "(JI)[B",                                                      (void *) CainMediaMetadataRetriever_getFrameAtTime},
        {"_getScaledFrameAtTime",      "(JIII)[B",                                                    (void *) CainMediaMetadataRetriever_getScaleFrameAtTime},
        {"getEmbeddedPicture",         "(I)[B",                                                       (void *) CainMediaMetadataRetriever_getEmbeddedPicture},
        {"extractMetadata",            "(Ljava/lang/String;)Ljava/lang/String;",                      (void *) CainMediaMetadataRetriever_extractMetadata},
        {"extractMetadataFromChapter", "(Ljava/lang/String;I)Ljava/lang/String;",                     (void *) CainMediaMetadataRetriever_extractMetadataFromChapter},
        {"_getAllMetadata",            "()Ljava/util/HashMap;",                                       (void *) CainMediaMetadataRetriever_getAllMetadata},
        {"release",                    "()V",                                                         (void *) CainMediaMetadataRetriever_release},
        {"native_setup",               "()V",                                                         (void *) CainMediaMetadataRetriever_setup},
        {"native_init",                "()V",                                                         (void *) CainMediaMetadataRetriever_native_init},
        {"native_finalize",            "()V",                                                         (void *) CainMediaMetadataRetriever_native_finalize}
};

//注册CainMediaMetadataRetriever的Native方法
static int register_com_cgfay_media_CainMediaMetadataRetriever(JNIEnv *env) {
    int numMethods = sizeof(nativeMethods)/ sizeof(nativeMethods[0]);
    jclass clazz = env->FindClass(RETRIEVER_CLASS_NAME);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", RETRIEVER_CLASS_NAME);
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, nativeMethods, numMethods) < 0) {
        ALOGE("Native registration unable to find class '%s'", RETRIEVER_CLASS_NAME);
        return JNI_ERR;
    }
    env->DeleteLocalRef(clazz);
    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    av_jni_set_java_vm(vm, NULL);
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    if (register_com_cgfay_media_CainMediaMetadataRetriever(env) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}