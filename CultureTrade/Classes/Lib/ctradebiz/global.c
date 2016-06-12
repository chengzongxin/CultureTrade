

#include "global.h"
#include "common.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

#define USER_FILE_PATH_NAME_LEN 1024
char User_LOG_FILE_PATH[USER_FILE_PATH_NAME_LEN];

//#define __LOG__
//#define __JXTDEBUG__

//日志文件存放路径
void setUserLogFileDirectoryPath(const char * logpath)
{
    memset((void *)User_LOG_FILE_PATH, 0, USER_FILE_PATH_NAME_LEN);
    strcpy(User_LOG_FILE_PATH, logpath);
}
const char * getUserLogFileDirectoryPath()
{
    return User_LOG_FILE_PATH;
}

#ifdef __JXTDEBUG__
void printBusLog(char * format,  ...)
{
	if (format == NULL)
	{
        return;
    }

	char s[1024] = {0};
	int len = 1024;

    va_list argList;
    va_start(argList, format);
	vsnprintf(s+strlen(s), len-strlen(s), format, argList);
    va_end(argList);

	Log(s);
}
#endif


#ifdef __ANDROID__
#include <android/log.h>
#endif

void Log(const char* msg)
{
#ifdef __LOG__
	time_t now;
	struct tm *nowTime;
	char tmp[1024] = {0};
	char path[1024]= {0};
	FILE *fp = NULL;

	time(&now);
	nowTime = localtime(&now);

	sprintf(tmp, "\r\n%04d-%02d-%02d %02d:%02d:%02d> ", nowTime->tm_year + 1900, nowTime->tm_mon + 1,
			nowTime->tm_mday, nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec);

//#if defined(IPHONE)
	strcat(path, User_LOG_FILE_PATH);
    strcat(path, "cbiz.log");
	fp = fopen(path, "a+");
//#else
//	fp = fopen(LOG_FIELNAME, "a+");
//#endif
	if (NULL == fp)
	{
		return;
	}

	ty_fseek(fp, 0, SEEK_END);
	ty_fwrite(tmp, sizeof(char), strlen(tmp), fp);
	ty_fwrite((char*)msg, sizeof(char), strlen(msg), fp);

	ty_fclose(fp);

    printf("%s",tmp);
#else

#ifdef __ANDROID__
	__android_log_write(ANDROID_LOG_DEBUG,"vmsbus",msg);
#else
	time_t now;
	struct tm *nowTime;

	time(&now);
	nowTime = localtime(&now);

	printf("[%d-%02d-%02d %02d:%02d:%02d]%s\r\n", nowTime->tm_year + 1900, nowTime->tm_mon + 1, nowTime->tm_mday,
												nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec, msg);
#endif

#endif // __LOG__
}



int utf8_to_unicode(char *in, unsigned short **out, int *outsize)
{
    unsigned char *p = (unsigned char *)in;
    unsigned short *result = NULL;
    int resultsize = 0;
    unsigned char *tmp = NULL;
    result = (unsigned short *)malloc(strlen(in) * 2 + 2); /* should be enough */
    memset(result, 0, strlen(in) * 2 + 2);
    tmp = (unsigned char *)result;
    while(*p)
    {
        if ( *p <= 0x7f)
        {
            *tmp = *p;
            tmp++;
            *tmp = '\0';
            resultsize += 2;
        }
        else if ((*p & (0xff << 5))== 0xc0)
        {
            //short t = 0;
            unsigned char t1 = 0;
            unsigned char t2 = 0;
            t1 = *p & (0xff >> 3);
            p++;
            t2 = *p & (0xff >> 2);
            *tmp = t2 | ((t1 & (0xff >> 6)) << 6);//t1 >> 2;
            tmp++;
            *tmp = t1 >> 2;//t2 | ((t1 & (0xff >> 6)) << 6);
            tmp++;
            resultsize += 2;
        }
        else if ((*p & (0xff << 4))== 0xe0)
        {
            //short t = 0;
            unsigned char t1 = 0;
            unsigned char t2 = 0;
            unsigned char t3 = 0;
            t1 = *p & (0xff >> 3);
            p++;
            t2 = *p & (0xff >> 2);
            p++;
            t3 = *p & (0xff >> 2);
            //Little Endian
            *tmp = ((t2 & (0xff >> 6)) << 6) | t3;//(t1 << 4) | (t2 >> 2);
            tmp++;
            *tmp = (t1 << 4) | (t2 >> 2);//((t2 & (0xff >> 6)) << 6) | t3;
            tmp++;
            resultsize += 2;
        }
        p++;
    }
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
    resultsize += 2;
    *out = result;
    *outsize = resultsize;
    return 0;
}





int charscontainedsub(char * src, char * sub)
{
	if (strstr(src, sub) != NULL)
	{
		return 1;
	}
	else {
		return 0;
	}
}

int getJPEGImageSize(unsigned char * data, int length)
{
	int i =0;
	short width = 0;
	short height = 0;
	unsigned char sw[2];
	for (i = 0; i < length; i++)
	{
		if (0xFF == data[i])
		{
			if ((i + 9) >= length) {
				return 0;
			}
			if (0xC0 == data[i+1])
			{
				memset(sw, 0, 2);
				memcpy(sw, &data[i+6], 1);
				memcpy( (unsigned char*) ((unsigned long)sw + 1), &data[i+5], 1);
				width = *(short*)&sw;
				memset(sw, 0, 2);
				memcpy(sw, &data[i+8], 1);
				memcpy( (unsigned char*) ((unsigned long)sw + 1), &data[i+7], 1);
				height = *(short*)&sw;
				return width * 10000 + height;
			}
		}
	}
	return 0;
}

cJSON * getJsonHear(int nSeq)
{
    cJSON * header;
    char sSeq[32];
    memset(sSeq, 0, 32);
    sprintf(sSeq, "%d", nSeq);
    header = cJSON_CreateObject();
    cJSON_AddStringToObject(header, "bf", "");
    cJSON_AddStringToObject(header, "sn", sSeq);
    return header;
}

void ByteToString(char * dest, unsigned char * src, int srclen)
{
    int j = 0;
    for( j = 0; j < srclen; j++ )
    {
        sprintf( (char*)((unsigned long)dest + j * 2), "%02x", src[j]);
    }
}

void getCurTimeStr(char * dest)
{
    time_t now;
    struct tm *nowTime;

	time(&now);
	nowTime = localtime(&now);

	sprintf(dest, "%04d-%02d-%02d %02d:%02d:%02d", nowTime->tm_year + 1900, nowTime->tm_mon + 1,
			nowTime->tm_mday, nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec);
}
//string DataHelper::GetCurTime()
//{
//	time_t t = time(0);
//	char tmp[64];
//	strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) );
//	string sTime = tmp;
//	return sTime;
//}
