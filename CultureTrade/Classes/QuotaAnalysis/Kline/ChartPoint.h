//
//  ChartPoint.h
//  CultureTrade
//
//  Created by SZFT1 on 15/11/26.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Stock.h"
@interface ChartPoint : NSObject

@property (nonatomic,retain) Stock *stock;
// 坐标系
@property (nonatomic,assign) CGFloat kLineWidth;
@property (nonatomic,assign) CGFloat intervalSpace;

@property (nonatomic,assign) CGPoint highestPricepoint;
@property (nonatomic,assign) CGPoint lowestPricepoint;
@property (nonatomic,assign) CGPoint openPricepoint;
@property (nonatomic,assign) CGPoint closePricepoint;
@property (nonatomic,assign) CGPoint volumepoint;
@property (nonatomic,assign) CGPoint MA5point;
@property (nonatomic,assign) CGPoint MA10point;
@property (nonatomic,assign) CGPoint MA20point;
@property (nonatomic,assign) CGPoint MAVOL5point;
@property (nonatomic,assign) CGPoint MAVOL10point;

// stock个数
- (int)numberOfStockInFrame:(CGRect)frame;
// 根据传入的一组stock，返回一组坐标，传入待显示在frame的尺寸
// 返回3个均线坐标:MA5、MA10、MA20
- (NSArray *)pointArrayTranslateByStockArray:(NSArray *)stockArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame;
// K线坐标

// volume成交量坐标

@end
