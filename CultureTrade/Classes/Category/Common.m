//
//  Common.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/5.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "Common.h"

@implementation Common
#pragma mark 字符串转换为日期时间对象
+(NSDate*)dateFromString:(NSString*)str{
    // 创建一个时间格式化对象
    NSDateFormatter *datef = [[NSDateFormatter alloc] init];
    // 设定时间的格式
    [datef setDateFormat:@"yyyy-MM-dd"];
    // 将字符串转换为时间对象
    NSDate *tempDate = [datef dateFromString:str];
    return tempDate;
}

+(NSObject *) getUserDefaults:(NSString *) name{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    return [userDefaults objectForKey:name];
}

+(void) setUserDefaults:(NSObject *) defaults forKey:(NSString *) key{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults setObject:defaults forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

@end
