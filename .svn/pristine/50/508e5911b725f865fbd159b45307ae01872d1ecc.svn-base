//
//  Debug.h
//  iVMS
//
//  Created by XiongXiong on 11-5-30.
//  Copyright 2011 Helecomm Daniel. All rights reserved.
//

#import <Foundation/Foundation.h>

//#define __AMRRecorder__

#ifdef __AMRRecorder__
#import "vmsSendMessageAdapter.h"
#endif



#define __VMS_DEBUG__		///
#define _SIMULATOR_


#define __NET_STATUS__

#ifdef __VMS_DEBUG__
#define __DEBUG__
#endif

//#ifdef __VMS_DEBUG__

@interface NSData (NSDataExtensions)

- (BOOL) appendToFile: (NSString *) path atomically: (BOOL) flag;

@end

#ifdef __VMS_DEBUG__

extern void vms_log_prompt(NSString* format,...);

//#define vmslog vms_log_prompt

#define vmslog LOG

#else

#define vms_log_prompt(...) {}
#define vmslog(...)	{}

#endif

#ifdef __DEBUG__

#define LOG_FILE_LINE(fmt, ...) do {                                            \
NSString* file = [[NSString alloc] initWithFormat:@"%s", __FILE__]; \
NSLog((@"%@(%d) " fmt), [file lastPathComponent], __LINE__, ##__VA_ARGS__); \
[file release];                                                 \
} while(0)

#define LOG(fmt,...) do { \
NSString* prefix = [[NSString alloc] initWithFormat:@"%s",__func__]; \
NSLog((@"%@:" fmt), prefix,##__VA_ARGS__); \
[prefix release]; \
} while(0)

#define LOG_METHOD NSLog(@"%s", __func__)
#define LOG_CMETHOD NSLog(@"%@/%@", NSStringFromClass([self class]), NSStringFromSelector(_cmd))
#define COUNT(p) NSLog(@"%s(%d): count = %d\n", __func__, __LINE__, [p retainCount]);
#define LOG_TRACE(x) do {printf x; putchar('\n'); fflush(stdout);} while (0)


#else

#define LOG(...)
#define LOG_METHOD
#define LOG_CMETHOD
#define COUNT(p)
#define LOG_TRACE(x)

#endif
/*
#ifndef __OPTIMIZE__
# define NSLog(...) NSLog(__VA_ARGS__)
#else
# define NSLog(...) {}
#endif
*/
