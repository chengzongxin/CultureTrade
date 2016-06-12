/*
 *  common.c
 *  SpeakHere
 *
 *  Created by zengxw on 11-7-29.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "common.h"
#include <string.h>
//
#ifdef WIN32
#define F_OK 0
#endif


void* ty_malloc(int size)
{
	void * tmp = malloc(size);
	memset(tmp, 0, size);
	return tmp;
}

void ty_mfree(void** p)
{
	if (*p != NULL) {
		free(*p);
		*p = NULL;
	}
}


//return 0 or -1
int ty_fexist(const char * filename)
{
	return 0;//access(filename, 0 );
}

void * ty_freadopen( const char * path)
{
	return fopen(path, "rb");
}

void * ty_fwriteopen( const char * path)
{
	return fopen(path, "wb");
}


int ty_fread(void * data, int size, int len, void *file)
{
	return fread(data, size, len, (FILE *) file );
}

int ty_fwrite(void * data, int size, int len, void *file)
{
	return fwrite(data, size, len, (FILE*)file);
}

//SEEK_SET
int ty_fseek(void * file, long offset, int flag)
{
	return fseek((FILE *)file, offset, flag);
}

long ty_ftell(void * file)
{
	return ftell((FILE *) file);
}
int ty_fclose(void * file)
{
	return fclose((FILE *) file);
}
int ty_fsave(const char * filepath, void * data, int size)
{
	void * pfile = ty_fwriteopen(filepath);
	ty_fwrite(data, 1, size, pfile);
	ty_fclose(pfile);
	return 1;
}
int ty_fdelete(const char * filepath)
{
	return remove(filepath);
}

long getfilesize(void *stream)
{
	long curpos, length;
	curpos = ty_ftell(stream);
	ty_fseek(stream, 0L, 2);
	length = ty_ftell(stream);
	ty_fseek(stream, curpos, 0);
	return length;
}

void ty_writechar(char* stream, int* index, char v)
{
	int idx = *index;

	if (NULL == stream) return;
	
	stream[idx] = v;

	idx += sizeof(char);
	
	*index = idx;
}

void ty_writeshort(char* stream, int *index, short v)
{
	int idx = *index;

	if (NULL == stream) return;
	
	memcpy(stream + idx, (char*)(&v), sizeof(short));
	
	idx += sizeof(short);

	*index = idx;
}

void ty_writeint(char* stream, int *index, int v)
{
	int idx = *index;

	if (NULL == stream) return;
	
	memcpy(stream + idx, (char*)(&v), sizeof(int));
	
	idx += sizeof(int);

	*index = idx;
}

void ty_writestring(char* stream, int *index, const char* str)
{
	int len = 0;
	int idx = *index;
	
	if (NULL == stream) return;
	
	len = strlen(str);
	
	ty_writeint(stream, &idx, len);

	*index = idx;
	if (NULL == str) return;
	
	memcpy(stream + idx, str, strlen(str));
	
	idx += strlen(str);

	*index = idx;
}

void ty_writestream(char* stream, int *index, const char* str, int strlen)
{
	int idx = *index;
	if (NULL == stream) return;
	
	ty_writeint(stream, &idx, strlen);

	*index = idx;

	if (NULL == str) return;
	
	memcpy(stream + idx, str, strlen);
	
	idx += strlen;

	*index = idx;
}


char ty_readchar(const char* stream, int *index)
{
	int idx = *index;

	if (NULL == stream) return 0;

	*index = idx + sizeof(char);

	return stream[idx];
}

short ty_readshort(const char* stream, int *index)
{
	short v = 0;
	int idx = *index;
	
	if (NULL == stream) return 0;
	
	memcpy((char*)(&v), stream + idx, sizeof(short));
	
	idx += sizeof(short);
	*index = idx;
	
	return v;
}

int ty_readint(const char* stream, int *index)
{
	int v = 0;
	int idx = *index;
	
	if (NULL == stream) return 0;
	
	memcpy((char*)(&v), stream + idx, sizeof(int));
	
	idx += sizeof(int);
	*index = idx;
	
	return v;
}

char* ty_readstring(const char* stream, int *index)
{
	char* ret = NULL;
	int len = 0;
	int idx = *index;
	
	if (NULL == stream) return NULL;
	
	len = ty_readint(stream, &idx);
	
	ret = (char*)malloc(len + 1);
	memset(ret, '\0', len + 1);
	memcpy(ret, stream + idx, len);
	idx += len;
	*index = idx;
	
	return ret;
}

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
}
#endif /* WIN32 */

//time
double getsymtemtime()
{
	time_t now;
	time(&now);
	return (double)now;
}

int getsystemmintime()
{
	struct timeval t_start;
	gettimeofday(&t_start, NULL);
	return (int)(t_start.tv_usec / 1000);
}
