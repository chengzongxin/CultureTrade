//
//  NSDate+helper.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/21.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "NSDate+helper.h"

@implementation NSDate (helper)
+ (NSString *)fewDaysAgo:(int)number
{
    NSDate * mydate = [NSDate date];
    
    NSDateFormatter * dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
//    NSString *currentDateStr = [dateFormatter stringFromDate:[NSDate date]];
//    DebugLog(@"---当前的时间的字符串 =%@",currentDateStr);
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateComponents *comps = nil;
    
    comps = [calendar components:NSCalendarUnitYear|NSCalendarUnitMonth|NSCalendarUnitMonth fromDate:mydate];
    
    NSDateComponents *adcomps = [[NSDateComponents alloc] init];
    
    [adcomps setYear:0];
    
    [adcomps setMonth:0];
    number = number * -1;
    [adcomps setDay:number];
    NSDate *newdate = [calendar dateByAddingComponents:adcomps toDate:mydate options:0];
    NSString *beforDate = [dateFormatter stringFromDate:newdate];
//    DebugLog(@"---前两个月 =%@",beforDate);
    return beforDate;
}

+ (NSDate *)dateFromString:(NSString *)string
{
    NSDateFormatter * dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    return [dateFormatter dateFromString:string];
}

+ (NSString *)stringFromDate:(NSDate *)date
{
    NSDateFormatter * dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    return [dateFormatter stringFromDate:date];
}

- (BOOL)dateIsBetweenDate:(NSDate *)beginDate andDate:(NSDate *)endDate
{
    if ([self compare:beginDate] == NSOrderedAscending) {
        return NO;
    }
    
    if ([self compare:endDate] == NSOrderedDescending) {
        return NO;
    }
    
    return YES;
}

+ (NSDate *)dateWithLocalDate
{
    NSDate *date = [NSDate date];
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate:date];
    NSDate *localDate = [date dateByAddingTimeInterval:interval];
    
    return localDate;
}

@end
