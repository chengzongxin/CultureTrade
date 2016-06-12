

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

//#define __LOG__



#include "common.h"
#include "cJSON.h"
#define JXT_SUCCESS 1
#define JXT_FAILED  0

#define TIMESTRLEN 64
#define USERACCLEN 64

//#define __JXTDEBUG__

//#ifdef __JXTDEBUG__
//void printBusLog(char * format,  ...);
//#else
//#define printBusLog(...) NULL
//#endif


void Log(const char* msg);


    void setUserLogFileDirectoryPath(const char * logpath);

    const char * getUserLogFileDirectoryPath();


void vms_strcopy(char ** dest, char * src);
void vms_datacopy( void ** dest, void * src, int srcsize);
int charscontainedsub(char * src, char * sub);
int getJPEGImageSize(unsigned char * data, int length);
void appendData(void *des, int index, void * srcdata, int size);


cJSON * getJsonHear(int nSeq);
void ByteToString(char * dest, unsigned char * src, int srclen);
void getCurTimeStr(char * dest);

#ifdef __cplusplus
}
#endif

#endif // __GLOBAL_H__
