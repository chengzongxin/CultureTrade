//
//  UncaughtExceptionHandler.h
//  demo
//
//  Created by SZFT1 on 14-8-20.
//  Copyright (c) 2014年 SZFT1. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UncaughtExceptionHandler : NSObject {
	BOOL dismissed;
}
void HandleException(NSException *exception);
void SignalHandler(int signal);

void InstallUncaughtExceptionHandler(void);
@end
