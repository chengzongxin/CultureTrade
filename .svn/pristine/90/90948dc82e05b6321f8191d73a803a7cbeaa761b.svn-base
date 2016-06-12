/*
 *  common.h
 *  SpeakHere
 *
 *  Created by zengxw on 11-7-29.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>
//#include <time.h>
//#include <stdint.h>
//#include "typedef.h"
#include <stdarg.h>
#include <ctype.h>

#ifdef WIN32
#include <windows.h>
#include "stdint.h"
#include <io.h>
#else
#include <sys/time.h>
#include <stdint.h>
#endif

//#define IPHONE
#if defined(IPHONE)
#include <unistd.h>
#endif




//Memory
void* ty_malloc(int size);
void ty_mfree(void** p);


//File
int ty_fexist(const char * filename);
void * ty_freadopen( const char * path);
void * ty_fwriteopen( const char * path);
int ty_fread(void * data, int size, int len, void *file);
int ty_fwrite(void * data, int size, int len, void *fiel);
int ty_fseek(void * file, long offset, int flag);
long ty_ftell(void * file);
int ty_fclose(void * file);
int ty_fsave(const char * filepath, void * data, int size);
int ty_fdelete(const char * filepath);
long getfilesize(void *stream);

// binary write
// write value to data stream
void ty_writechar(char* stream, int* index, char v);
void ty_writeshort(char* stream, int* index, short v);
void ty_writeint(char* stream, int* index, int v);
void ty_writestring(char* stream, int* index, const char* str);
void ty_writestream(char* stream, int* index, const char* str, int strlen);

char ty_readchar(const char* stream, int *index);
short ty_readshort(const char* stream, int *index);
int ty_readint(const char* stream, int *index);
char* ty_readstring(const char* stream, int *index);
typedef struct _tag_ty_string
{
	char* str;
	int len;
}ty_string;
ty_string* ty_readtystring(const char* stream, int *index);

//time
double getsymtemtime();
int getsystemmintime();

//ts_hx_vms_date_time get_ts_hx_vms_date_time(double date);
//double get_double_date_time(ts_hx_vms_date_time vms_date);
#endif
