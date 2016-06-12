//
//  NSTradeBizUtils.h
//  FT Trader
//
//  Created by SZFT1 on 15/1/7.
//  Copyright (c) 2015年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString* PATTER;
@interface NSTradeBizUtils : NSObject

+(NSTradeBizUtils*) sharedInstance;

-(char*)toCString:(NSString*)s;

-(NSString*)toNSString:(const char *)c;

-(double) round:(double)f dec:(int)dec;

-(BOOL) isPhone:(NSString*)str;
-(BOOL) isShenFenzheng:(NSString*)str;
-(NSString*)format:(int)number decimal:(int)dot;
-(NSString*)formatTime:(int)time ptt:(NSString*)ptt;
- (NSString *)localIPAddress;
@end
