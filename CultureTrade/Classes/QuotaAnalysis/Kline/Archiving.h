//
//  Archiving.h
//  CultureTrade
//
//  Created by SZFT1 on 16/2/2.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
// stockArray
@interface Archiving : NSObject <NSCopying>
@property (nonatomic,retain) NSString *date;
@property (nonatomic,assign) float highestPrice;
@property (nonatomic,assign) float lowestPrice;
@property (nonatomic,assign) float openPrice;
@property (nonatomic,assign) float closePrice;
@property (nonatomic,assign) float maxPrice;
@property (nonatomic,assign) float minPrice;
@property (nonatomic,assign) float volume;
@property (nonatomic,assign) float maxVolume;
@property (nonatomic,assign) float minVolume;
@property (nonatomic,assign) float MA5;
@property (nonatomic,assign) float MA10;
@property (nonatomic,assign) float MA20;

- (void)writeToFile:(NSString *)path saveArray:(NSArray *)array;
- (NSArray *)readFromFile:(NSString *)path;
@end
