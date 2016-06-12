//
//  TimeChartPoint.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/6.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "TimeChartPoint.h"
#import "GlobalModel.h"

#define kPointCount 242 //一天交易时间4个小时，240分钟

@implementation TimeChartPoint
- (instancetype)init
{
    self = [super init];
    [self initalization];
    return self;
}

- (void)initalization
{
    self.kLineWidth = 10.0f;
    self.intervalSpace = 1.0f;
}

- (NSMutableArray *)pointArrayTranslateByRealArray:(NSArray *)realArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame
{
    if (realArray.count == 0) {
        return [self noRealMinsToFillData:realArray innerMainFrame:mainFrame bottomFrame:bottomFrame];
    }else{
        return [self realMinsToFillData:realArray innerMainFrame:mainFrame bottomFrame:bottomFrame];
    }
}

- (NSMutableArray *)noRealMinsToFillData:(NSArray *)realArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame
{
    NSMutableArray *pointArray = [NSMutableArray arrayWithCapacity:kPointCount];
    NSMutableArray *allReal = [NSMutableArray array];
    float preClose = GLOBAL.sortUnit.m_uiPreClose;
    int newTime = (int)GLOBAL.nowData.m_usTime;
    
    if (newTime <= 570) return NULL;
    
    newTime -= 570;
    for (int i = 0; i<newTime+1; i++) {
        if (i > 120 && i < 210) continue;
        RealMinsUnitNS *real = [[RealMinsUnitNS alloc] init];
        real.m_usTime = i;
        real.m_uiNewPrice = preClose*1000;
        real.m_uiAverage = preClose*1000;
        [allReal addObject:real];
    }
    
    float maxPrice = preClose*1.1;
    float minPrice = preClose*0.9;
    
    self.maxPrice = maxPrice;
    self.minPrice = minPrice;
    self.maxVolume = 0;
    
    CGFloat mainHeight = mainFrame.size.height;
    CGFloat mainWidth = mainFrame.size.width;
    CGFloat bottomHeight = bottomFrame.size.height;
    
    for (int i  = 0; i < allReal.count ; i ++) { // 坐标正向算，最新的显示在最右边
        RealMinsUnitNS *real = allReal[i];
        CGFloat x = mainWidth / kPointCount * i;
        TimeChartPoint *timeChartPoint = [[TimeChartPoint alloc] init];
        timeChartPoint.realMin = real;
        
        if (maxPrice - minPrice == 0) {   // 连昨收价都没有取到
            timeChartPoint.newPricepoint = CGPointMake(x, 0);
            timeChartPoint.averagePricepoint = CGPointMake(x, 0);
            timeChartPoint.volumePoint = CGPointMake(x, bottomHeight);
        }else{
            timeChartPoint.newPricepoint = CGPointMake(x, (1-(real.m_uiNewPrice*0.001 - minPrice)/(maxPrice - minPrice))*mainHeight);
            timeChartPoint.averagePricepoint = CGPointMake(x, (1-(real.m_uiAverage*0.001 - minPrice)/(maxPrice - minPrice))*mainHeight);
            timeChartPoint.volumePoint = CGPointMake(x, bottomHeight);
            [pointArray addObject:timeChartPoint];
        }
        
    }
    return pointArray;
}

- (NSMutableArray *)realMinsToFillData:(NSArray *)realArray innerMainFrame:(CGRect)mainFrame bottomFrame:(CGRect)bottomFrame
{
    NSMutableArray *pointArray = [NSMutableArray arrayWithCapacity:kPointCount];
    
    if (realArray.count < 1) return NULL;
    NSMutableArray *todayTimeArray = [NSMutableArray array];
    todayTimeArray = [NSMutableArray arrayWithArray:realArray];
    
    RealMinsUnitNS *yesLastReal = [[RealMinsUnitNS alloc] init];
    yesLastReal.m_usTime = 0;
    yesLastReal.m_uiNewPrice = GLOBAL.sortUnit.m_uiPreClose*1000;
    yesLastReal.m_uiAverage = GLOBAL.sortUnit.m_uiPreClose*1000;
    for (int i = 0; i < realArray.count-1; i++) {
        // 获得最大值和最小值
        RealMinsUnitNS *real1 = realArray[i];
        RealMinsUnitNS *real2 = realArray[i+1];
        
        if (real1.m_usTime > real2.m_usTime)
        {// 剪除昨日大数据
            todayTimeArray = [NSMutableArray arrayWithArray:[realArray subarrayWithRange:NSMakeRange(i+1, realArray.count - i-1)]];
            //            yesLastReal = realArray[i];
        }
    }
    
    // 没有第0分钟数据，取昨天最后一个数据填充
    RealMinsUnitNS *real = [todayTimeArray firstObject];
    if (real.m_usTime != 0) { // 没有第0个
        RealMinsUnitNS *yesLast = [[RealMinsUnitNS alloc] init];
        yesLast.m_usTime = 0;
        yesLast.m_uiNewPrice = yesLastReal.m_uiNewPrice;
        yesLast.m_uiAverage = yesLastReal.m_uiAverage;
        [todayTimeArray insertObject:yesLast atIndex:0];
    }
    
    for (RealMinsUnitNS *real in todayTimeArray) {
        MYLog(@"初步整合:%hd,%f,%f",real.m_usTime,real.m_uiNewPrice,real.m_uiAverage);
    }
    
    NSMutableArray *allReal = [NSMutableArray array];
    float yesClosePrice = GLOBAL.sortUnit.m_uiPreClose;
    float maxPrice = GLOBAL.nowData.m_uiMaxPrice;// = yesClosePrice*1.1;
    float minPrice = GLOBAL.nowData.m_uiMinPrice;// = yesClosePrice*0.9;
    float maxVolume = 0;
    if (yesClosePrice != 0) {
        minPrice = minPrice < yesClosePrice ? yesClosePrice : minPrice;  // 取小
    }
    // 填充中间空白跳跃的stock数据
    for (int i = 0; i<todayTimeArray.count; i++) {
        //        time++;
        RealMinsUnitNS *real = todayTimeArray[i];
        if (real.m_usTime > 120 && real.m_usTime < 210) continue;
        if (real.m_usTime!=i) {
            RealMinsUnitNS *preReal = todayTimeArray[i-1];
            for (int j = preReal.m_usTime + 1; j < real.m_usTime; j++) {
                if (j > 120 && j < 210) continue;
                RealMinsUnitNS *newReal = [[RealMinsUnitNS alloc] init];
                newReal.m_usTime = j;
                newReal.m_uiNewPrice = preReal.m_uiNewPrice;
                newReal.m_uiAverage = preReal.m_uiAverage;
                [allReal addObject:newReal];
            }
        }
        [allReal addObject:real];
        maxPrice = MAX(MAX(real.m_uiNewPrice*0.001, real.m_uiAverage*0.001),maxPrice);
        minPrice = MIN(MIN(real.m_uiNewPrice*0.001, real.m_uiAverage*0.001),minPrice);
        maxVolume = MAX(real.m_uiVolume, maxVolume);
    }
    
    
    float interValue = maxPrice - minPrice;
    
    if (interValue == 0) {  // maxPrice == minPrice
        interValue = maxPrice;
    }
    maxPrice += interValue*0.1;
    minPrice -= interValue*0.1;
    
    self.maxPrice = maxPrice;
    self.minPrice = minPrice;
    self.maxVolume = maxVolume;
    
    
    //    for (RealMinsUnitNS *r in allReal) {
    //        MYLog(@"FIN 填充分时:%d,%f,%f",r.m_usTime,r.m_uiNewPrice,r.m_uiAverage);
    //    }
    
    // 填充坐标系,因为line是添加到mainBox中，y轴起始点以0计算,否则要加上(+ frame.origin.y - frame.size.height)
    CGFloat mainHeight = mainFrame.size.height;
    CGFloat mainWidth = mainFrame.size.width;
    CGFloat bottomHeight = bottomFrame.size.height;
    
    
    for (int i  = 0; i < allReal.count ; i ++) { // 坐标正向算，最新的显示在最右边
        RealMinsUnitNS *real = allReal[i];
        CGFloat x = mainWidth / kPointCount * i;
        TimeChartPoint *timeChartPoint = [[TimeChartPoint alloc] init];
        timeChartPoint.realMin = real;
        timeChartPoint.newPricepoint = CGPointMake(x, (1-(real.m_uiNewPrice*0.001 - minPrice)/(maxPrice - minPrice))*mainHeight);
        timeChartPoint.averagePricepoint = CGPointMake(x, (1-(real.m_uiAverage*0.001 - minPrice)/(maxPrice - minPrice))*mainHeight);
        timeChartPoint.volumePoint = CGPointMake(x, (1-real.m_uiVolume/maxVolume)*bottomHeight);
        [pointArray addObject:timeChartPoint];
        
    }
    return pointArray;
}

@end
