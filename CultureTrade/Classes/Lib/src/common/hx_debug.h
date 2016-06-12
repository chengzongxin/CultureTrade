#ifndef __HX_DEBUG_H__
#define __HX_DEBUG_H__

#define HX_LOG(fmt,...)	printf("%s(%d):" fmt "\n",__func__,__LINE__,##__VA_ARGS__)
#define HX_LOG_CMD(fmt,...)NULL//	printf("%s(%d):RSP," fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)

#endif

