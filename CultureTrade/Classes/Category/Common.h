//
//  Common.h
//  CultureTrade
//
//  Created by SZFT1 on 15/11/5.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Common : NSObject
+(NSObject *) getUserDefaults:(NSString *) name;
+(void) setUserDefaults:(NSObject *) defaults forKey:(NSString *) key;
@end
