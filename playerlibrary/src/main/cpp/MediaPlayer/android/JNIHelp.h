//
// Created by zhumingwei on 2020/7/12.
//

#ifndef DAFUPLAYER_JNIHELP_H
#define DAFUPLAYER_JNIHELP_H

#include "jni.h"
#include <errno.h>
#include <unistd.h>

#ifndef NELEM
# define NELEM(x) ((int)(sizeof(x) / sizeof((x)[0])));
#endif

#ifdef __cplusplus
extern "C" {
#endif


int jniRegisterNativeMethods(
        C_JNIEnv *env,
        const char *className,
        const JNINativeMethod *gMethods,
        int numMethods
);

int jniThrowException(
        C_JNIEnv *env,
        const char *className,
        const char *msg
);

int jniThrowNullPointerException(
        C_JNIEnv *env,
        const char *msg
);

int jniThrowRuntimeException(
        C_JNIEnv *env,
        const char* msg
        );

int jniThrowIOException(
        C_JNIEnv *env,
        int errnum
);

int jniGetFDFromFileDescriptor(
        C_JNIEnv *env,
        jobject fileDescriptor
);

const char *jniStrError(
        int errnum,
        char *buf,
        size_t buflen
);

jstring jniCreateString(
        C_JNIEnv *env,
        const jchar *unicodeChars,
        jsize len
);

void jniLogException(
        C_JNIEnv *env,
        int poriority,
        const char *tag,
        jthrowable exception
);


#ifdef __cplusplus
}
#endif

#if defined(__cplusplus)

inline int jniRegisterNativeMethods(
        JNIEnv *env,
        const char *className,
        const JNINativeMethod *gMethods,
        int numMethods
) {
    return jniRegisterNativeMethods(
            &env->functions,
            className,
            gMethods,
            numMethods
    );
}

inline int jniThrowException(
        JNIEnv* env,
        const char* className,
        const char* msg = NULL
        ){
    return jniThrowException(&env->functions, className, msg);
}

extern "C"
int jniThrowExceptionFmt(
        C_JNIEnv* env,
        const char* className,
        const char* fmt,
        va_list args
        );

inline int jniThrowExceptionFmt(
        JNIEnv* env,
        const char* className,
        const char* fmt,
        ...
        ){
    va_list args;
    va_start(args, fmt);
    return jniThrowExceptionFmt(&env->functions, className, fmt, args);
    va_end(args);
}

inline int jniThrowNullPointerException(
    JNIEnv* env,
    const char* msg
        ){
    return jniThrowNullPointerException(&(env->functions),
            msg);
}
inline int jniThrowRuntimeException(
        JNIEnv* env,
        const char* msg
        ){
    return jniThrowRuntimeException(&env->functions, msg);
}

inline int jniThrowIOException(
        JNIEnv* env,
        int errnum
        ){
    return jniThrowIOException(&env->functions, errnum);
}

inline int jniGetFDFromFileDescriptor(JNIEnv * env, jobject fileDescriptor) {
    return jniGetFDFromFileDescriptor(&env->functions, fileDescriptor);
}

inline jstring jniCreateString(JNIEnv* env, const jchar* unicodeChars, jsize len) {
    return jniCreateString(&env->functions, unicodeChars, len);
}

#if __cplusplus >= 201103L
inline jstring jniCreateString(JNIEnv* env, const char16_t* unicodeChars, jsize len) {
    return jniCreateString(&env->functions, reinterpret_cast<const jchar*>(unicodeChars), len);
}
#endif  // __cplusplus >= 201103L

inline void jniLogException(JNIEnv* env, int priority, const char* tag, jthrowable exception = NULL) {
    jniLogException(&env->functions, priority, tag, exception);
}


#if !defined(DISALLOW_COPY_AND_ASSIGN)
// DISALLOW_COPY_AND_ASSIGN disallows the copy and operator= functions. It goes in the private:
// declarations in a class.
#if __cplusplus >= 201103L
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;  \
  void operator=(const TypeName&) = delete
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);  \
  void operator=(const TypeName&)
#endif  // __has_feature(cxx_deleted_functions)
#endif  // !defined(DISALLOW_COPY_AND_ASSIGN)

#endif

/*
 * TEMP_FAILURE_RETRY is defined by some, but not all, versions of
 * <unistd.h>. (Alas, it is not as standard as we'd hoped!) So, if it's
 * not already defined, then define it here.
 */
#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({         \
    typeof (exp) _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif


#endif //DAFUPLAYER_JNIHELP_H


























