//#define ___DEBUG_SEND___
#ifdef __ANDROID__

    //#define __JZ_ANDROID_DEBUG__ //jni日志文件

    #include <android/log.h>
    #include <string.h>
    #include <jni.h>

    #define  LOG_TAG  "JZ_JNI_DEBUG"

    #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG,__VA_ARGS__)
    #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
    #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
    #define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
    #define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)

#endif

#ifndef __JXT_LOG_H
#define __JXT_LOG_H

#if __cplusplus
extern "C"
{
#endif

//extern void log_d(const char * TAG, const char * str);
//extern void log_w(const char * TAG, const char * str);
//extern void log_dv(const char * TAG,const char * str, void* param);
//extern void log_f(const char * TAG,char * format,  ...);
    
//extern void myPrintf(const char * format, ...);
extern void printBusLogEx(const char* format, ...);

#if __cplusplus
};
#endif

#endif


