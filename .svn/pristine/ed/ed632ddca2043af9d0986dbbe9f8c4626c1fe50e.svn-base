//
//  SelectPrefer.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/18.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "SelectPrefer.h"

//#define kLocalDictKey @"PreferStock"

@implementation SelectPrefer
//检查本地文件
+ (void)checkLocalFile
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSString *fileName = LocalConfigDirectory;
    if([fm fileExistsAtPath:fileName] == YES)
    {
        NSLog(@"File is exist!");
        MYLog(@"%@",LocalPreferFile);
    }else{
        NSError *error;
        BOOL success = [fm createDirectoryAtPath:fileName withIntermediateDirectories:YES attributes:nil error:&error];  // 如果没有中间文件夹直接创建
        if (!success) {
            NSLog(@"%@",error);
        }else{
            NSLog(@"File is create! %@",[fm contentsOfDirectoryAtPath:LocalConfigDirectory error:nil]);
        }
    }
}
// 保存变量
+ (void)writeToFileForKey:(NSString *)key value:(NSString *)value
{
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithContentsOfFile:LocalPreferFile];
    if (!dict) {
        dict = [NSMutableDictionary dictionary];
    }
    [dict setObject:value forKey:key];
    if ([dict writeToFile:LocalPreferFile atomically:YES]) { //更新版本到本地
        NSLog(@"writeToFile seccess!");
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:LocalPreferFile];
        NSLog(@"%@",dict);
    }
}
// 获取文件，返回一个字典
+ (NSDictionary *)readFromFile
{
    return [NSDictionary dictionaryWithContentsOfFile:LocalPreferFile];
}

// 根据商品ID获取值
+ (NSString *)readFromFileForKey:(NSString *)key
{
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:LocalPreferFile];
    return [dict objectForKey:key];
}

@end
