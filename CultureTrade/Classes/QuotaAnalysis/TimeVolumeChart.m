//
//  TimeVolumeChart.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/7.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "TimeVolumeChart.h"
#import "TimeChartPoint.h"

@interface TimeVolumeChart ()
@end

@implementation TimeVolumeChart

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initalization];
    return self;
}

- (void)initalization
{
    self.kLineWidth = 1.0f;
}

- (void)setPointArray:(NSArray *)pointArray
{
    _pointArray = pointArray;
    [self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect {
    if (self.pointArray.count) { // point数组为空时跳过
        [self drawVolumeChart];
    }
    
}

- (void)drawVolumeChart
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    float previousDealPrice = 0.0f;
    for (NSUInteger i = 0; i <self.pointArray.count; i++) {
        TimeChartPoint *timeChartPoint = self.pointArray[i];
        if (i != 0) {
            TimeChartPoint *previousTimeChartPoint = self.pointArray[i - 1];
            previousDealPrice = previousTimeChartPoint.realMin.m_uiNewPrice;
        }
        CGFloat x = timeChartPoint.volumePoint.x;
        CGFloat y = timeChartPoint.volumePoint.y;
        CGFloat width = self.kLineWidth;
        CGFloat height = self.frame.size.height - timeChartPoint.volumePoint.y;
        
        
        CGRect rect = CGRectMake(x, y, width, height);
        CGContextAddRect(context, rect);
        // 1.先设置填充颜色，再填充颜色，否则是把当前设置填充颜色填到下一个矩形，导致右偏移bug
        if (timeChartPoint.realMin.m_uiNewPrice >= previousDealPrice) {
            CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);
        }else{
            CGContextSetFillColorWithColor(context, [UIColor greenColor].CGColor);
        }
        // 2.填充上下文指定的rect
        CGContextFillRect(context, rect);
        CGContextDrawPath(context, kCGPathStroke);
    }
}

@end
