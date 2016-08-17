//
//  ChartPoint.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/26.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "ChartPoint.h"
#import "Stock.h"
#import "GlobalModel.h"

@implementation ChartPoint
- (instancetype)init
{
    self = [super init];
    [self initalization];
    return self;
}

- (void)initalization
{
    self.kLineWidth = 3.0f;
    self.intervalSpace = 1.0f;
}

- (NSArray *)pointArrayTranslateByStockArray:(NSArray *)stockArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame
{
    
    // mainBox中待显示的个数
    int maxCount = mainFrame.size.width / (self.kLineWidth + self.intervalSpace);
//     多算一个，以免左边空隙不好看
//    maxCount ++;
    CGFloat mainHeight = mainFrame.size.height;
    CGFloat bottomHeight = bottomFrame.size.height;
    
    NSMutableArray *pointArray = [NSMutableArray arrayWithCapacity:maxCount];
    float maxPriceValue = 0.0,minPriceValue = MAXFLOAT,maxVolume = 10.0,minVolume = MAXFLOAT;
//    maxPriceValue = GLOBAL.stockInfo.m_uiPrevClose*1.1;
//    minPriceValue = GLOBAL.stockInfo.m_uiPrevClose*0.9;
    int stockCount = (int)MIN(stockArray.count, maxCount);
    for (int i = 0; i < stockCount; i++) {
        // 获得最大值和最小值
        Stock *stock = stockArray[i];
        
        maxPriceValue = MAX(maxPriceValue,stock.highestPrice);
        minPriceValue = MIN(minPriceValue,stock.lowestPrice);
        
        maxVolume = MAX(stock.volume, maxVolume);
        minVolume = 0.0;
    }

    // 填充坐标系,因为line是添加到mainBox中，y轴起始点以0计算,否则要加上(+ frame.origin.y - frame.size.height)
    
    for (int i  = 0; i < stockCount ; i ++) { // 坐标正向算，最新的显示在最右边
        Stock *stock = stockArray[i];
        
        CGFloat x = self.kLineWidth/2 + (self.kLineWidth + self.intervalSpace)*i;
        ChartPoint *chartPoint = [[ChartPoint alloc] init];
        
        float scaleMaxPriceValue = maxPriceValue*1.02; // 多划分一点区域，以免碰到底部
        float scaleMinPriceValue = minPriceValue*0.98; // 多划分一点区域，以免碰到底部
        
        chartPoint.stock = stock;
        chartPoint.stock.maxPrice = scaleMaxPriceValue;  // 影响左边坐标
        chartPoint.stock.minPrice = scaleMinPriceValue;
        chartPoint.stock.maxVolume = maxVolume;
        chartPoint.stock.minVolume = minVolume;
        // 穿过来的是0时，绘制error
        chartPoint.highestPricepoint = CGPointMake(x,(1-(stock.highestPrice - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight);
        chartPoint.lowestPricepoint = CGPointMake(x,(1-(stock.lowestPrice - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight);
        chartPoint.openPricepoint = CGPointMake(x,(1-(stock.openPrice - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight);
        chartPoint.closePricepoint = CGPointMake(x,(1-(stock.closePrice - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight);
        chartPoint.volumepoint = CGPointMake(x,(stock.volume - minVolume)/(maxVolume - minVolume)*bottomHeight); // 写死
//        chartPoint.MA5point = CGPointMake(x,(1-(stock.MA5 - minPriceValue)/(maxPriceValue - minPriceValue))*mainHeight);
//        chartPoint.MA10point = CGPointMake(x,(1-(stock.MA10 - minPriceValue)/(maxPriceValue - minPriceValue))*mainHeight);
//        chartPoint.MA20point = CGPointMake(x,(1-(stock.MA20 - minPriceValue)/(maxPriceValue - minPriceValue))*mainHeight);
        // 前面的数据为0的均线不画
        chartPoint.MA5point = (stock.MA5 > 0) ? CGPointMake(x,(1-(stock.MA5 - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight) : CGPointMake(x, 0);
        chartPoint.MA10point = (stock.MA10 > 0) ? CGPointMake(x,(1-(stock.MA10 - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight) : CGPointMake(x, 0);
        chartPoint.MA20point = (stock.MA20 > 0) ? CGPointMake(x,(1-(stock.MA20 - scaleMinPriceValue)/(scaleMaxPriceValue - scaleMinPriceValue))*mainHeight) : CGPointMake(x, 0);
        [pointArray addObject:chartPoint];
    }
    return pointArray;
}

- (int)numberOfStockInFrame:(CGRect)frame
{
    // mainBox中待显示的个数
    int count = frame.size.width / (self.kLineWidth + self.intervalSpace);
    // 多算一个，以免左边空隙不好看
//    count ++;
    return count;
}

@end
