//
//  Global.h
//  CultureTrade
//
//  Created by SZFT1 on 16/2/23.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Global : NSObject
+(void)saveErrLog:(NSString*)err;
+(NSString*)getDocumentDirectory;
@end
