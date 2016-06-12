
#include <stdio.h>
#include <stdarg.h>
#include "time.h"

//#ifdef __ANDROID__
//    #include <android/log.h>
//    #include <string.h>
//    #include <jni.h>
//#else
//
//#endif

#include "jxt_log.h"


void printBusLogEx(const char* format, ...)
{
#ifdef __ANDROID__
    #ifdef __JZ_ANDROID_DEBUG__
    LOGE("====================================START=======================================");
    int len = 10240;
    char s[10240] = {0};
    va_list argList;
    va_start(argList, format);
    //vsnprintf(s+strlen(s), len-strlen(s), format.c_str(), argList);
    vsnprintf(s, len, format, argList);
    va_end(argList);
    LOGE(s);
    LOGE("=====================================END=========================================\n");
    #endif
#else
    #ifdef __JZ_IOS_DEBUG__
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
    #endif
#endif
}

//#ifdef __ANDROID__
//
//void myPrintf(const char* format, ...)
//{
////    int len = 51200;
////	char s[51200] = {0};
////    va_list argList;
////    va_start(argList, format);
////	vsnprintf(s+strlen(s), len-strlen(s), format, argList);
////    va_end(argList);
////    LOGD("MyPrint",s);
//}
//
//#else
//void myPrintf(const char* format, ...)
//{
////    time_t lt =time(NULL);
////    printf("%s", ctime(&lt));
//    
//    va_list argList;
//    va_start(argList, format);
//    vprintf(format, argList);
//    va_end(argList);
//}
//#endif

//#include "interf_enc.h"
//int isLog = 1;


/*
void log_d(const char * TAG, const char * str)
{
	if (isLog == 0) return;
	char result[512];
	sprintf(result,"JNI [--%s--] ",TAG);
	strcat(result,str);
#ifdef __ANDROID__
	__android_log_write(ANDROID_LOG_DEBUG,"Jxt",result);
#endif
}

void log_w(const char * TAG, const char * str)
{
	if (isLog == 0) return;
	char result[512];
	sprintf(result,"JNI [--%s--] ",TAG);
	strcat(result,str);
#ifdef __ANDROID__
	__android_log_write(ANDROID_LOG_WARN,"Jxt",result);
#endif
}

void log_dv(const char * TAG,const char * str, void* param)
{
	if (isLog == 0) return;
	char result[512];
	sprintf(result,str,param);
	log_d(TAG,result);
}

void log_f(const char * TAG,char * format,  ...)
{
	if (isLog == 0) return;
	if (format == NULL)
	{
        return;
    }

	int len = 512;
	char s[512] = {0};
    va_list argList;
    va_start(argList, format);
	vsnprintf(s+strlen(s), len-strlen(s), format, argList);
    va_end(argList);
    log_d(TAG,s);
}*/
