//
//  UncaughtExceptionHandler.m
//  demo
//
//  Created by SZFT1 on 14-8-20.
//  Copyright (c) 2014年 SZFT1. All rights reserved.
//

#import "UncaughtExceptionHandler.h"
#include <libkern/OSAtomic.h>
#include <execinfo.h>
#import "Global.h"

//http://www.cocoachina.com/newbie/tutorial/2012/0829/4672.html
NSString * const UncaughtExceptionHandlerSignalExceptionName = @"UncaughtExceptionHandlerSignalExceptionName";
NSString * const UncaughtExceptionHandlerSignalKey = @"UncaughtExceptionHandlerSignalKey";
NSString * const UncaughtExceptionHandlerAddressesKey = @"UncaughtExceptionHandlerAddressesKey";

volatile int32_t UncaughtExceptionCount = 0;
const int32_t UncaughtExceptionMaximum = 10;

const NSInteger UncaughtExceptionHandlerSkipAddressCount = 4;
const NSInteger UncaughtExceptionHandlerReportAddressCount = 5;

@implementation UncaughtExceptionHandler

+ (NSArray *)backtrace
{
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    
    int i;
    NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
    for (
         i = UncaughtExceptionHandlerSkipAddressCount;
         i < UncaughtExceptionHandlerSkipAddressCount +
         UncaughtExceptionHandlerReportAddressCount;
         i++)
    {
        @try {
            [backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
        }
        @catch (NSException *exception) {
        }	 	
    }
    free(strs);
    
    return backtrace;
}

- (void)alertView:(UIAlertView *)anAlertView clickedButtonAtIndex:(NSInteger)anIndex
{
	if (anIndex == 0){
		dismissed = YES;
	}else if (anIndex==1) {
        NSLog(@"ssssssss");
    }
}

- (void)validateAndSaveCriticalApplicationData{
	
}

- (void)handleException:(NSException *)exception {
	[self validateAndSaveCriticalApplicationData];
//    NSString * msg = [NSString stringWithFormat:NSLocalizedString(@"异常原因如下:\n获取设备的名称=%@\n获取系统名称=%@\n版本号=%@\n手机型号=%@\n%@\n%@", nil),
//                      [[UIDevice currentDevice] name],
//                      [[UIDevice currentDevice] systemName],
//                      [[UIDevice currentDevice] systemVersion],
//                      [[UIDevice currentDevice] model],
//                      [exception reason],
//                      [[exception userInfo] objectForKey:UncaughtExceptionHandlerAddressesKey]];

//    手机序列号
//    NSString* identifierNumber = [[UIDevice currentDevice] uniqueIdentifier];
//    NSLog(@"手机序列号: %@",identifierNumber);

    NSString* msg = @"异常原因如下:\n";
    
    //手机别名： 用户定义的名称
    NSString* userPhoneName = [[UIDevice currentDevice] name];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"手机别名: %@\n", userPhoneName ]];
    
    //设备名称
    NSString* deviceName = [[UIDevice currentDevice] systemName];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"设备名称: %@\n", deviceName]];
    
    //手机系统版本
    NSString* phoneVersion = [[UIDevice currentDevice] systemVersion];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"手机系统版本: %@\n", phoneVersion]];
    
    //手机型号
    NSString* phoneModel = [[UIDevice currentDevice] model];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"手机型号: %@\n", phoneModel]];
    
    //地方型号  （国际化区域名称）
    NSString* localPhoneModel = [[UIDevice currentDevice] localizedModel];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"国际化区域名称: %@\n",localPhoneModel]];

    // 当前应用名称
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    NSString *appCurName = [infoDictionary objectForKey:@"CFBundleDisplayName"];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"当前应用名称：%@\n",appCurName]];
    
    // 当前应用软件版本
    NSString *appCurVersion = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"当前应用软件版本:%@\n",appCurVersion]];
    
    // 当前公共版本
    NSString *appNativeVersion = [[NSTradeEngine setup] getNativeVersion];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"当前公共版本：%@\n",appNativeVersion]];
    
    // 当前应用版本号码   int类型
    NSString *appCurVersionNum = [infoDictionary objectForKey:@"CFBundleVersion"];
    msg = [msg stringByAppendingString:[NSString stringWithFormat:@"当前应用版本号码：%@\n",appCurVersionNum]];

    NSString* err = [NSString stringWithFormat:NSLocalizedString(@"%@\n%@\n", nil),
                     [exception reason],
                     [[exception userInfo] objectForKey:UncaughtExceptionHandlerAddressesKey]];
    msg = [msg stringByAppendingString:err];
    
    saveErrLog(msg);
    NSLog(@"%@", msg);
//	UIAlertView *alert =
//    [[UIAlertView alloc]
//      initWithTitle:NSLocalizedString(@"抱歉，程序出现了异常！", nil)
//      message: nil
//      delegate: self
//      cancelButtonTitle: NSLocalizedString(@"退出程序", nil)
//      otherButtonTitles: nil, nil];
//	[alert show];
    
//	CFRunLoopRef runLoop = CFRunLoopGetCurrent();
//	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
	
//	while (!dismissed){
//		for (NSString *mode in (__bridge NSArray *)allModes){
//			CFRunLoopRunInMode((__bridge CFStringRef)mode, 0.001, false);
//		}
//	}
    
//	CFRelease(allModes);
    
//	signal(SIGABRT, SignalHandler);
//	signal(SIGILL, SignalHandler);
//	signal(SIGSEGV, SignalHandler);
//	signal(SIGFPE, SignalHandler);
//	signal(SIGBUS, SignalHandler);
//	signal(SIGPIPE, SignalHandler);
//    signal(SIGKILL, SignalHandler);
//    signal(SIGTERM, SignalHandler);
//    NSSetUncaughtExceptionHandler(&HandleException);
//	
//	if ([[exception name] isEqual:UncaughtExceptionHandlerSignalExceptionName]){
//		kill(getpid(), [[[exception userInfo] objectForKey:UncaughtExceptionHandlerSignalKey] intValue]);
//	} else {
//		[exception raise];
//	}
}

@end

void HandleException(NSException *exception){
	int32_t exceptionCount = OSAtomicIncrement32(&UncaughtExceptionCount);
	if (exceptionCount > UncaughtExceptionMaximum) return;
	
	NSArray *callStack = [UncaughtExceptionHandler backtrace];
	NSMutableDictionary *userInfo =
    [NSMutableDictionary dictionaryWithDictionary:[exception userInfo]];
	[userInfo setObject:callStack forKey:UncaughtExceptionHandlerAddressesKey];
	
	[[[UncaughtExceptionHandler alloc] init]
            performSelectorOnMainThread:@selector(handleException:)
            withObject: [NSException exceptionWithName:[exception name]
                                                reason:[exception reason]
                                                userInfo:userInfo]
            waitUntilDone:YES];
}


void SignalHandler(int signal){
	int32_t exceptionCount = OSAtomicIncrement32(&UncaughtExceptionCount);
	if (exceptionCount > UncaughtExceptionMaximum) return;
    
	NSMutableDictionary *userInfo = [NSMutableDictionary dictionaryWithObject:[NSNumber numberWithInt:signal] forKey:UncaughtExceptionHandlerSignalKey];
    
	NSArray *callStack = [UncaughtExceptionHandler backtrace];
	[userInfo setObject:callStack forKey:UncaughtExceptionHandlerAddressesKey];
	
	[[[UncaughtExceptionHandler alloc] init] performSelectorOnMainThread: @selector(handleException:)
           withObject: [NSException exceptionWithName: UncaughtExceptionHandlerSignalExceptionName
                                            reason: [NSString stringWithFormat: NSLocalizedString(@"Signal %d was raised.", nil), signal]
                                            userInfo:[NSDictionary dictionaryWithObject: [NSNumber numberWithInt:signal]
                                            forKey:UncaughtExceptionHandlerSignalKey]]
           waitUntilDone:YES];
}


void InstallUncaughtExceptionHandler(void)
{
//    signal(SIGABRT, SignalHandler);
//    signal(SIGILL,  SignalHandler);
//    signal(SIGSEGV, SignalHandler);
//    signal(SIGFPE,  SignalHandler);
//    signal(SIGBUS,  SignalHandler);
//    signal(SIGPIPE, SignalHandler);
//    signal(SIGKILL, SignalHandler);
//    signal(SIGTERM, SignalHandler);
//    NSSetUncaughtExceptionHandler(&HandleException);
}






