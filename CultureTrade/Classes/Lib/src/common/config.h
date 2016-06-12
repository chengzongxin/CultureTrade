#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif


//是否使用异步io,在windows下使用完成端口,在linux下使用epoll,如果不定义,则windows和linux下都使用select模型
#define __NT_ASIO__
#define __NT_STATISTICS__ //做流量等统计

#ifdef NDEBUG // _DEBUG
#define __NT_NDEBUG__
#endif // NDEBUG

//使用新宏的意义在于更方便控制
#ifdef _WIN32

#define __NT_WIN__

#define _CRT_SECURE_NO_WARNINGS

#define __NT_LOCK_BUF__ //使用完成端口时是否预先锁定内存

//完成端口多线程下必须加锁
#ifdef __NT_ASIO__
#define __NT_MULTI_THREAD__
#endif // __NT_ASIO__

#pragma warning(disable: 4200)
#pragma warning(disable: 4996)

//#define __USE_FS__ //是否使用文件做后备

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <shlwapi.h>
#include <io.h>
#include <share.h>
#include <process.h>
#include <direct.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#else

#define __NT_UNIX__

//#define __NT_MULTI_THREAD__
#define __POSIX_SHM__ //是否默认使用posix方式的共享内存
#define __USE_TMPFS__ //是否使用临时文件系统

#include <stdint.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/mount.h>
//#include <sys/vfs.h>
//#include <sys/sysinfo.h>
//#include <sys/shm.h>
//#include <sys/mman.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
//#include <sys/epoll.h>
//#include <linux/stddef.h>
//#include <sys/resource.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sched.h>
#include <netinet/tcp.h>
#include <sys/ipc.h>
//#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>

#define __stdcall 

#endif // _WIN32


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <math.h>
#include "hx_debug.h"

#ifdef __cplusplus
}
#endif


#endif // __CONFIG_H__

// end of file
