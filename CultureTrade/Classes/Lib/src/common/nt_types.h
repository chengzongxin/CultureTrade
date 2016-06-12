#ifndef __NT_TYPES_H__
#define __NT_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

//以下是windows下的标准位宽类型定义,请尽量使用标准类型,便于进行32位操作系统与64位进行移植
#ifdef __NT_WIN__

/*
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
*/

typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else
typedef unsigned char* LPBYTE;
typedef unsigned long int HANDLE;
typedef unsigned char BYTE; 
typedef BYTE * PBYTE;
typedef int SOCKET;
typedef void* LPVOID;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef const char* LPCSTR;

//typedef char bool;

typedef struct sockaddr_in  SOCKADDR_IN;
typedef struct hostent* LPHOSTENT;
typedef struct in_addr* LPIN_ADDR;


typedef int WSAEVENT;

#define SOCKET_ERROR	-1
#define INVALID_SOCKET -1

#define FD_READ	0x01
#define FD_WRITE	0x02

#define FD_CLOSE	0x03
#define FD_CONNECT	0x04

#endif // __NT_WIN__


#ifdef __cplusplus
}
#endif

#endif // __NT_TYPES_H__

// end of file
