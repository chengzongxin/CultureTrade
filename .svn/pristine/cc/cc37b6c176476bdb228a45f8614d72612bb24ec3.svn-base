//
//  TimeChartPoint.h
//  CultureTrade
//
//  Created by SZFT1 on 16/1/6.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DataModel.h"
#import "DataModel.h"

@interface TimeChartPoint : NSObject
@property (nonatomic,retain) RealMinsUnitNS *realMin;

@property (nonatomic,assign) CGFloat kLineWidth;
@property (nonatomic,assign) CGFloat intervalSpace;

@property (nonatomic,assign) CGPoint newPricepoint;
@property (nonatomic,assign) CGPoint volumePoint;
@property (nonatomic,assign) CGPoint averagePricepoint;

@property (nonatomic,assign) float maxPrice;
@property (nonatomic,assign) float minPrice;
@property (nonatomic,assign) float maxVolume;

- (NSMutableArray *)pointArrayTranslateByRealArray:(NSArray *)realArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame;
//- (int)numberOfStockInFrame:(CGRect)frame;
@end
