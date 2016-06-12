//
//  Global.m
//  CultureTrade
//
//  Created by SZFT1 on 16/2/23.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "Global.h"

@implementation Global

+(NSString*)getDocumentDirectory{
    NSArray *directoryPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return [directoryPaths objectAtIndex:0];
}

+(void)saveErrLog:(NSString *)err
{
    NSString* dir = [[Global getDocumentDirectory] stringByAppendingString: @"/err/"];
    BOOL flag = true;
    flag = [[NSFileManager defaultManager] fileExistsAtPath:dir isDirectory:&flag];
    if(!flag){
        flag = [[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil];
    }
    NSString* stime = [NSString stringWithFormat:@"%.f", [[NSDate date] timeIntervalSince1970] ];
    NSString *fileFullName = [[dir stringByAppendingString:stime] stringByAppendingString:@".log"];
    NSData * data = [err dataUsingEncoding:NSUTF8StringEncoding];
    [data writeToFile:fileFullName atomically:NO];
}
@end
