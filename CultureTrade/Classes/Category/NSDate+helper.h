//
//  NSDate+helper.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/21.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDate (helper)
// 几天以前
+ (NSString *)fewDaysAgo:(int)number;

+ (NSDate *)dateFromString:(NSString *)string;

+ (NSString *)stringFromDate:(NSDate *)date;
@end
