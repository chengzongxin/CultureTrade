//
//  VolumeChart.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/27.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "VolumeChart.h"
#import "ChartPoint.h"

@implementation VolumeChart

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initalization];
    return self;
}

- (void)initalization
{
    self.kLineWidth = 5.0f;
}

- (void)setPointArray:(NSArray *)pointArray
{
    _pointArray = pointArray;
    [self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect {
    if (self.pointArray.count) { // point数组为空时跳过
        [self drawVolumeChart];
        [self drawAverageLineMA5VOL:5];
        [self drawAverageLineMA10VOL:10];
    }
    
}

- (void)drawVolumeChart
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    for (NSUInteger i = 0; i <self.pointArray.count; i++) {
        ChartPoint *chartPoint = self.pointArray[i];
        
        CGFloat x = chartPoint.volumepoint.x - self.kLineWidth / 2;
        CGFloat y = self.frame.size.height - chartPoint.volumepoint.y;
        CGFloat width = self.kLineWidth;
        CGFloat height = chartPoint.volumepoint.y;
        
        
        CGRect rect = CGRectMake(x, y, width, height);
        CGContextAddRect(context, rect);
        if (chartPoint.openPricepoint.y >= chartPoint.closePricepoint.y) { // 绿色
            // 1.先设置填充颜色，再填充颜色，否则是把当前设置填充颜色填到下一个矩形，导致右偏移bug
//            CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);
            // 2.填充上下文指定的rect
//            CGContextFillRect(context, rect);
            [[UIColor redColor] set];
            UIRectFrame(rect);
        }else{
            CGContextSetFillColorWithColor(context, [UIColor greenColor].CGColor);
            CGContextFillRect(context, rect);
        }
        CGContextDrawPath(context, kCGPathStroke);
        
    }

}

- (void)drawAverageLineMA5VOL:(int)skip
{
    if (self.pointArray.count < skip ) {
        return;
    }
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef pathMA = CGPathCreateMutable();
    //    CGContextRestoreGState(context);
    ChartPoint *firstPoint = self.pointArray[skip-1];
    CGPathMoveToPoint(pathMA, NULL, firstPoint.MAVOL5point.x, firstPoint.MAVOL5point.y);
    
    for (int i = skip; i < self.pointArray.count ; i++) {
        ChartPoint *point = self.pointArray[i];
        CGPathAddLineToPoint(pathMA, NULL, point.MAVOL5point.x, point.MAVOL5point.y);
    }
    CGContextSetLineWidth(context, 0.5);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 255/255.0f, 1);//white
    CGContextAddPath(context, pathMA);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(pathMA);
}

- (void)drawAverageLineMA10VOL:(int)skip
{
    if (self.pointArray.count < skip ) {
        return;
    }
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef pathMA = CGPathCreateMutable();
    //    CGContextRestoreGState(context);
    ChartPoint *firstPoint = self.pointArray[skip-1];
    CGPathMoveToPoint(pathMA, NULL, firstPoint.MAVOL10point.x, firstPoint.MAVOL10point.y);
    
    for (int i = skip; i < self.pointArray.count ; i++) {
        ChartPoint *point = self.pointArray[i];
        CGPathAddLineToPoint(pathMA, NULL, point.MAVOL10point.x, point.MAVOL10point.y);
    }
    CGContextSetLineWidth(context, 0.5);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 0/255.0f, 1);//red
    CGContextAddPath(context, pathMA);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(pathMA);
}



@end
