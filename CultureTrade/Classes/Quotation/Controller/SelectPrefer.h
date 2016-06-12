//
//  SelectPrefer.h
//  CultureTrade
//
//  Created by SZFT1 on 16/1/18.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SelectPrefer : NSObject

+ (void)checkLocalFile;

+ (void)writeToFileForKey:(NSString *)key value:(NSString *)value;

+ (NSDictionary *)readFromFile;
// 根据商品ID获取值
+ (NSString *)readFromFileForKey:(NSString *)key;
@end
