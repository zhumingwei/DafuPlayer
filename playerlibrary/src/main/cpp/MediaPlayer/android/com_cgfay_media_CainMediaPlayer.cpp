//
// Created by zhumingwei on 2020/7/15.
//
//
#if defined(__ANDROID__)

#include <jni.h>
#include <Mutex.h>
#include <Condition.h>
#include <CainMediaPlayer.h>
#include "JNIHelp.h"

extern "C" {
#include <libavcodec/jni.h>
}

const char *CLASS_NAME = "com/zhumingwei/playerlibrary/CainMediaPlayer";
const char *string_arg_exception = "java/lang/IllegalStateArgumentException";
// ----------------------------------------------------------------------------------------------------------------------
struct fields_t {
    jfieldID        context;
    jmethodID       post_event;
};

static fields_t fields;

static JavaVM *javaVM = NULL;

static JNIEnv *getJNIEnv() {
    JNIEnv *env;
    assert(javaVM != NULL);
    if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return NULL;
    }
    return env;
}
// ----------------------------------------------------------------------------------------------------------------------
class JNIMediaPlayerListener : public MediaPlayerListener {
public:
    JNIMediaPlayerListener(JNIEnv* env, jobject thiz, jobject weak_thiz);
    ~JNIMediaPlayerListener();
    void notify(int msg, int ext1, int ext2, void *obj) override;

private:
    JNIMediaPlayerListener();
    jclass mClass;
    jobject mObject;
};

JNIMediaPlayerListener::JNIMediaPlayerListener(JNIEnv *env, jobject thiz, jobject weak_thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) {
        ALOGE("Can't find com/cgfay/media/CainMediaPlayer");
        jniThrowException(env, "java/lang/Exception");
        return;
    }
    mClass = (jclass) env->NewGlobalRef(clazz);
    mObject = env->NewGlobalRef(weak_thiz);
}

JNIMediaPlayerListener::~JNIMediaPlayerListener() {
    JNIEnv *env = getJNIEnv();
    env->DeleteGlobalRef(mObject);
    env->DeleteGlobalRef(mClass);
}

void JNIMediaPlayerListener::notify(int msg, int ext1, int ext2, void *obj) {
    JNIEnv *env = getJNIEnv();
    bool status = (javaVM->AttachCurrentThread(&env, NULL) >= 0);
    env->CallStaticVoidMethod(mClass, fields.post_event, mObject, msg, ext1,ext2, obj);
    if (env->ExceptionCheck()){
        ALOGW("An exception occurred while notifying an event.");
        env->ExceptionClear();
    }
    if (status) {
        javaVM->DetachCurrentThread();
    }
}

// -------------------------------------------------------------------------------------------------

static CainMediaPlayer *getMediaPlayer(JNIEnv *env, jobject thiz){
    CainMediaPlayer * const mp = reinterpret_cast<CainMediaPlayer *const>(env->GetLongField(thiz,
                                                                                            fields.context));
    return mp;
}

static CainMediaPlayer *setMediaPlayer(JNIEnv *env, jobject thiz, long mediaPlayer){
    CainMediaPlayer *old = reinterpret_cast<CainMediaPlayer *>(env->GetLongField(thiz,
                                                                                 fields.context));
    env->SetLongField(thiz, fields.context, mediaPlayer);
    return old;
}
/*
 * If exception is NULL and opStatus is not OK, this method sends an error
 * event to the client application; otherwise , if exception is not NULL and
 * opStatus is not OK, this method throws the given exception to the client application.
 */
static void process_media_player_call(JNIEnv *env,jobject thiz, int opStatus,
        const char* exception, const char *message){
    if (exception == NULL){ //Don't throw exception. Instead, send and evnet.
        if (opStatus != (int)OK){
            CainMediaPlayer* mp = getMediaPlayer(env, thiz);
            if (mp!=0) mp->notify(MEDIA_ERROR, opStatus, 0);
        }
    } else {
        if (opStatus == (int)INVALID_OPERATION) {
            jniThrowException(env, "java/lang/IllegalStateException");
        } else if (opStatus == (int)PERMISSION_DENIED){
            jniThrowException(env, "java/lang/SecurityException");
        } else if (opStatus == (int)OK){
            if (strlen(message) > 230) {
                jniThrowException(env,exception, message);
            } else {
                char msg[256];
                sprintf(msg, "%s:status=0x%X", message, opStatus);
                jniThrowException(env, exception, msg);
            }
        }
    }
}

void CainMediaPlayer_setDataSourceAndHeaders(JNIEnv *env, jobject thiz, jstring path_,
        jobjectArray keys, jobjectArray values){
    CainMediaPlayer *mp = getMediaPlayer(env, thiz);
    if (mp == NULL) {
        jniThrowException(env, "java/lang/IllegalStateException");
        return;
    }
    if (path_ == NULL) {
        jniThrowException(env, string_arg_exception);
        return;
    }
    const char *path = env->GetStringUTFChars(path_, 0);
    if (path == NULL) {
        return;
    }
    const char *restrict = strstr(path, "mms://");
    char *restrict_to = restrict ? strdup(restrict) : NULL;
    if (restrict_to != NULL) {
        strncpy(restrict_to, "mmsh://", 6);
        puts(path);
    }

    char *headers = NULL;
    if (keys && values != NULL) {
        int keysCount = env->GetArrayLength(keys);
        int valuesCount = env->GetArrayLength(values);
        if (keysCount != valuesCount) {
            ALOGE("keys and values arrays have different length");
            jniThrowException(env, string_arg_exception);
            return;
        }
        int i = 0;
        const char *rawString = NULL;
        char hdrs[2048];
        //todo
    }
}

void CainMediaPlayer_setDataSource(JNIEnv *env, jobject thiz, jstring path_){
    //todo
}

static const JNINativeMethod gMethods[] ={
        {"_setDataSource", "(Ljava/lang/String;)V", (void *)CainMediaPlayer_setDataSource},
        {
         "_setDataSource",
                           "(Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V",
                                                    (void *)CainMediaPlayer_setDataSourceAndHeaders
        },
        {"_setDataSource", "(Ljava/io/FileDescriptor;JJ)V", (void *)CainMediaPlayer_setDataSourceFD},
        {"_setVideoSurface", "(Landroid/view/Surface;)V", (void *) CainMediaPlayer_setVideoSurface},
        {"_prepare", "()V", (void *) CainMediaPlayer_prepare},
        {"_prepareAsync", "()V", (void *) CainMediaPlayer_prepareAsync},
        {"_start", "()V", (void *) CainMediaPlayer_start},
        {"_stop", "()V", (void *) CainMediaPlayer_stop},
        {"_resume", "()V", (void *) CainMediaPlayer_resume},
        {"_getRotate", "()I", (void *) CainMediaPlayer_getRotate},
        {"_getVideoWidth", "()I", (void *) CainMediaPlayer_getVideoWidth},
        {"_getVideoHeight", "()I", (void *) CainMediaPlayer_getVideoHeight},
        {"_seekTo", "(F)V", (void *) CainMediaPlayer_seekTo},
        {"_pause", "()V", (void *) CainMediaPlayer_pause},
        {"_isPlaying", "()Z", (void *) CainMediaPlayer_isPlaying},
        {"_getCurrentPosition", "()J", (void *) CainMediaPlayer_getCurrentPosition},
        {"_getDuration", "()J", (void *) CainMediaPlayer_getDuration},
        {"_release", "()V", (void *) CainMediaPlayer_release},
        {"_reset", "()V", (void *) CainMediaPlayer_reset},
        {"_setLooping", "(Z)V", (void *) CainMediaPlayer_setLooping},
        {"_isLooping", "()Z", (void *) CainMediaPlayer_isLooping},
        {"_setVolume", "(FF)V", (void *) CainMediaPlayer_setVolume},
        {"_setMute", "(Z)V", (void *) CainMediaPlayer_setMute},
        {"_setRate", "(F)V", (void *) CainMediaPlayer_setRate},
        {"_setPitch", "(F)V", (void *) CainMediaPlayer_setPitch},
        {"native_init", "()V", (void *)CainMediaPlayer_init},
        {"native_setup", "(Ljava/lang/Object;)V", (void *) CainMediaPlayer_setup},
        {"native_finalize", "()V", (void *) CainMediaPlayer_finalize},
        {"_changeFilter", "(ILjava/lang/String;)V", (void *)CainMediaPlayer_changeFilter},
        {"_changeFilter", "(II)V", (void *)CainMediaPlayer_changeFilterById},
        {"_setOption", "(ILjava/lang/String;Ljava/lang/String;)V", (void *)CainMediaPlayer_setOption},
        {"_setOption", "(ILjava/lang/String;J)V", (void *)CainMediaPlayer_setOptionLong}
};
// 注册CainMediaPlayer的Native方法
static int register_com_cgfay_media_CainMediaPlayer(JNIEnv *env){
    int numMethods = (sizeof(gMethods) / sizeof(gMethods[0]));
    jclass clazz = env->FindClass(CLASS_NAME);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", CLASS_NAME);
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0){
        ALOGE("Native registration unable to find class '%s'", CLASS_NAME);
        return JNI_ERR;
    }
    env->DeleteLocalRef(clazz);
    return JNI_OK;
}
extern "C"
JNIEXPORT jint  JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    av_jni_set_java_vm(vm, NULL);
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    if (register_com_cgfay_media_CainMediaPlayer(env) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}
#endif