//
//  GlobalModel.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/14.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "GlobalModel.h"

@interface GlobalModel () <NSTradeEngineDelegate>

@end

@implementation GlobalModel 
//singleton_implementation(GlobalModel)
static GlobalModel *_instance;
+ (GlobalModel *)shareInstance
{
if (_instance == nil) {
    _instance = [[self alloc] init];
//    [NSTradeEngine sharedInstance].delegate = self;  // 初始化业务
}
return _instance;
}

+ (GlobalModel *)allocWithZone:(struct _NSZone *)zone
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super allocWithZone:zone];
    });
    return _instance;
}

- (NSString *)getRememberAccount
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kAccountKey];
}

- (NSString *)getRememberPassword
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kPasswordKey];
}

- (NSString *)getRememberServerIP
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kServerIP];
}


- (void)rememberBalanceIP:(NSString *)ip Port:(int)port
{
    [[NSUserDefaults standardUserDefaults] setObject:ip forKey:kBalanceIP];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    [[NSUserDefaults standardUserDefaults] setObject:@(port) forKey:kBalancePort];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (NSString *)getRememberBalanceIP
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kBalanceIP];
}

- (int)getRememberBalancePort
{
    NSNumber *number = [[NSUserDefaults standardUserDefaults] objectForKey:kBalancePort];
    return [number intValue];
}

@end


