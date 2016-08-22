//
//  Stock.h
//  CultureTrade
//
//  Created by SZFT1 on 15/11/26.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Stock : NSObject
// 数值系
@property (nonatomic,retain) NSString *date;
@property (nonatomic,assign) float highestPrice;
@property (nonatomic,assign) float lowestPrice;
@property (nonatomic,assign) float openPrice;
@property (nonatomic,assign) float closePrice;
@property (nonatomic,assign) float maxPrice;
@property (nonatomic,assign) float minPrice;
@property (nonatomic,assign) int volume;
@property (nonatomic,assign) float maxVolume;
@property (nonatomic,assign) float minVolume;
@property (nonatomic,assign) float MA5;
@property (nonatomic,assign) float MA10;
@property (nonatomic,assign) float MA20;
@property (nonatomic,assign) float MAVOL5;
@property (nonatomic,assign) float MAVOL10;


@end
