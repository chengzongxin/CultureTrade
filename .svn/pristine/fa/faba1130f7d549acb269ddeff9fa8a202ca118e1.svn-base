//
//  NSString+helper.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

#define toCString(ns) [[NSString alloc] toCString: ns];
#define toNString(cs) [NSString toNSString: cs];


@interface NSString (helper)

+ (NSString *)LocalizedStringByInt:(int)key;
- (NSString *)fileAppend:(NSString *)append;
+ (NSString *)toNSString:(const char *) c;
+ (NSString *)stringFromDate:(NSDate *)date;
// 数值变化
+ (NSString*)changePrice:(int)price;
+ (NSString*)changePriceByFloat:(float)price;
//+ (char *)toCString:(NSString*)s;
+ (NSString *)int2string:(int)intValue;
// 把完整的时间转换成只显示小时：分：秒
+ (NSString *)stringFormatFromFullDate:(NSString *)fullDate;
- (char*)toCString:(NSString*)s;
// 判断字符串是否为纯数字
- (BOOL)isPureNumandCharacters:(NSString *)string;
+ (NSString *)stringFromDateDay:(NSDate *)date;
@end
