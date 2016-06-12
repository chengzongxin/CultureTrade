//
//  TimeLineChart.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/5.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "TimeLineChart.h"
#import "TimeChartPoint.h"

@implementation TimeLineChart

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initalization];
    return self;
}

- (void)initalization
{
    self.kLineWidth = 10.0f;
}

- (void)setPointArray:(NSArray *)pointArray
{
    // 重写set方法，重绘视图
    _pointArray = pointArray;
    [self setNeedsDisplay];
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
    [self drawInteralLine];
    
    [self drawTimeLine];
}

- (void)drawInteralLine
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef path = CGPathCreateMutable();
    
    CGFloat interalValue = self.frame.size.height / 6;
    for (int i = 1; i < 6; i++) {
        CGFloat y = interalValue * i;
        CGPathMoveToPoint(path, NULL, 0, y);
        CGPathAddLineToPoint(path, NULL, self.frame.size.width, y);
    }
    CGContextAddPath(context, path);
    
    CGContextSetLineWidth(context, 0.5);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    CGContextSetRGBStrokeColor(context, 150/255.0f, 150/255.0f, 150/255.0f, 0.5);
    
    
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(path);
}

- (void)drawTimeLine
{
    CGContextRef context = UIGraphicsGetCurrentContext();
//    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef newPath = CGPathCreateMutable();
    CGMutablePathRef avePath = CGPathCreateMutable();
    
    TimeChartPoint *firstTimePoint = self.pointArray.firstObject;
    CGPathMoveToPoint(newPath, NULL, firstTimePoint.newPricepoint.x, firstTimePoint.newPricepoint.y);
    CGPathMoveToPoint(avePath, NULL, firstTimePoint.averagePricepoint.x, firstTimePoint.averagePricepoint.y);
    
    for (int i = 1; i < self.pointArray.count ; i++) {
        TimeChartPoint *timePoint = self.pointArray[i];
//        MYLog(@"draw time line new= %@",NSStringFromCGPoint(timePoint.newPricepoint));
//        MYLog(@"draw time line ave= %@",NSStringFromCGPoint(timePoint.averagePricepoint));
        CGPathAddLineToPoint(newPath, NULL, timePoint.newPricepoint.x, timePoint.newPricepoint.y);
        CGPathAddLineToPoint(avePath, NULL, timePoint.averagePricepoint.x, timePoint.averagePricepoint.y);
    }
    CGContextSetLineWidth(context, 0.5);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 255/255.0f, 1);
    CGContextAddPath(context, newPath);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(newPath);
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 100/255.0f, 1);
    CGContextAddPath(context, avePath);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(avePath);
}

@end
