//
//  KlineChart.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/27.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//


#import "KlineChart.h"
#import "UIColor+helper.h"
#import "Stock.h"
#import "ChartPoint.h"

@implementation KlineChart

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

- (void)addGesture
{
    
}

- (void)drawRect:(CGRect)rect
{
    [self drawInteralLine];
    
    if (self.pointArray.count) { // point数组为空时跳过
        [self drawAverageLine];
        [self drawKline];
    }
}

- (void)setPointArray:(NSArray *)pointArray
{
    // 重写set方法，重绘视图
    _pointArray = pointArray;
    [self setNeedsDisplay];
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



- (void)drawAverageLine
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef pathMA5 = CGPathCreateMutable();
    CGMutablePathRef pathMA10 = CGPathCreateMutable();
    CGMutablePathRef pathMA20 = CGPathCreateMutable();
    
    // MA5/MA10/MA20
    ChartPoint *oldestPoint = self.pointArray.firstObject;
    CGPathMoveToPoint(pathMA5, NULL, oldestPoint.MA5point.x, oldestPoint.MA5point.y);
    CGPathMoveToPoint(pathMA10, NULL, oldestPoint.MA10point.x, oldestPoint.MA10point.y);
    CGPathMoveToPoint(pathMA20, NULL, oldestPoint.MA20point.x, oldestPoint.MA20point.y);
    
    for (int i = 0; i < self.pointArray.count ; i++) {
        ChartPoint *point = self.pointArray[i];
        CGPathAddLineToPoint(pathMA5, NULL, point.MA5point.x, point.MA5point.y);
        CGPathAddLineToPoint(pathMA10, NULL, point.MA10point.x, point.MA10point.y);
        CGPathAddLineToPoint(pathMA20, NULL, point.MA20point.x, point.MA20point.y);
    }
    CGContextSetLineWidth(context, 0.5);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 255/255.0f, 1);
    CGContextAddPath(context, pathMA5);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(pathMA5);
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 0/255.0f, 0/255.0f, 1);
    CGContextAddPath(context, pathMA10);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(pathMA10);
    
    CGContextSetRGBStrokeColor(context, 255/255.0f, 255/255.0f, 0/255.0f, 1);
    CGContextAddPath(context, pathMA20);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(pathMA20);
    
}


- (void)addLineFrom:(CGPoint)srcPoint toPoint:(CGPoint)desPoint color:(CGFloat)R :(CGFloat)G :(CGFloat)B
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    CGMutablePathRef path = CGPathCreateMutable();
    CGContextSetRGBStrokeColor(context, R/255.0f, G/255.0f, B/255.0f, 1);
    CGPathMoveToPoint(path, NULL, srcPoint.x, srcPoint.y);
    CGPathAddLineToPoint(path, NULL, desPoint.x, desPoint.y);
    
    CGContextSetLineWidth(context, 1);
    CGContextSetShouldAntialias(context, YES);  // 是否抗锯齿
    
    CGContextAddPath(context, path);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(path);
    
}

- (void)drawKline
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetShouldAntialias(context, NO);
    
    // 往右边移动一格
    for (NSUInteger i = 0; i <self.pointArray.count; i++) {
        ChartPoint *chartPoint = self.pointArray[i];
        
        if (chartPoint.openPricepoint.y >= chartPoint.closePricepoint.y) { // 红色
            [self addLineFrom:chartPoint.highestPricepoint toPoint:chartPoint.lowestPricepoint color:255 :0 :0];
            
        }else{                                                              // 绿色
            [self addLineFrom:chartPoint.highestPricepoint toPoint:chartPoint.lowestPricepoint color:0 :255 :0];
        }
        
        if (chartPoint.openPricepoint.y >= chartPoint.closePricepoint.y) { // 红色
            
            CGFloat x = chartPoint.closePricepoint.x - self.kLineWidth / 2;
            CGFloat y = chartPoint.closePricepoint.y;
            CGFloat width = self.kLineWidth;
            CGFloat height = chartPoint.openPricepoint.y - chartPoint.closePricepoint.y;
            if (height == 0) height = 1;
            CGRect rect = CGRectMake(x, y, width, height);
            
            CGContextAddRect(context, rect);
            CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);
            CGContextFillRect(context, rect);
            
        }else{
            
            CGFloat x = chartPoint.openPricepoint.x - self.kLineWidth / 2;
            CGFloat y = chartPoint.openPricepoint.y;
            CGFloat width = self.kLineWidth;
            CGFloat height = chartPoint.closePricepoint.y - chartPoint.openPricepoint.y;
            if (height == 0) height = 1;
            CGRect rect = CGRectMake(x, y, width, height);
            CGContextAddRect(context, rect);
            // 1.先设置填充颜色，再填充颜色，否则是把当前设置填充颜色填到下一个矩形，导致右偏移bug
            CGContextSetFillColorWithColor(context, [UIColor greenColor].CGColor);
            // 2.填充上下文指定的rect
            CGContextFillRect(context, rect);
            
        }
        
        CGContextDrawPath(context, kCGPathStroke);
        
        }
}



@end