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

- (void)quote_ui_hisKDataCurDate_rsp:(NSString *)data count:(int)count
{
    MYLog(@"%@,%d",data,count);
    MYLog(@"123123123123");
}

- (void)quote_ui_hisKDataFirst_rsp:(NSString *)data count:(int)count
{
    MYLog(@"%@,%d",data,count);
    MYLog(@"123123123123");
}


@end


