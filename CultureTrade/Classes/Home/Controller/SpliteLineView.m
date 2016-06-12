//
//  SpliteLineView.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/24.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "SpliteLineView.h"
@implementation SpliteLineView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    return self;
}
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
    //1.获取绘图的上下文
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    //线性渐变
    [self drawLinarGradient:context];
    
    //径向渐变
//    [self drawRadialGradient:context];
}


-(void)drawLinarGradient:(CGContextRef) context
{
    
    //2.创建一个渐变
    //2.1 创建一个颜色空间
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    //2.2 设置颜色
    //设置开始颜色
    UIColor *startColor = COLOR_SPLITELINE_TAIL;
    const CGFloat *startColorComponents = CGColorGetComponents([startColor CGColor]);
    
    //设置中间颜色
    UIColor *midColor = COLOR_SPLITELINE;
    const CGFloat *midColorComponents = CGColorGetComponents([midColor CGColor]);
    
    //设置结束颜色
    UIColor *endColor = COLOR_SPLITELINE_TAIL;
    const  CGFloat *endColorComponents =CGColorGetComponents([endColor CGColor]);
    
    //颜色分量的强度值的数组
    CGFloat components[12] = {startColorComponents[0],startColorComponents[1],
        startColorComponents[2],startColorComponents[3],
        
        midColorComponents[0],midColorComponents[1],
        midColorComponents[2],midColorComponents[3],
        
        endColorComponents[0],endColorComponents[1],
        endColorComponents[2],endColorComponents[3]
    };
    //渐变系数(程度)
    CGFloat locations[3] = {0.0,0.5,1.0};
    
    
    //创建渐变对象(参数：颜色空间、颜色分量强度值数组、渐变系数数组、设置的渐变系数个数)
    CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, components, locations, 3);
    
    
    //3.在上下文中画渐变(参数：上下文、颜色空间、开始点、结束点、渐变模式,默认为0)
    CGContextDrawLinearGradient(context, gradient, CGPointZero, CGPointMake(self.frame.size.width, self.frame.size.height), kCGGradientDrawsAfterEndLocation);
    
    
    //渐变模式：
    //kCGGradientDrawsAfterEndLocation      //向外发散
    //kCGGradientDrawsBeforeStartLocation   //向里发散
    
    
    //4.对creat创建的对象必须清理(避免内存泄露)
    CGColorSpaceRelease(colorSpace);
    CGGradientRelease(gradient);
}

-(void)drawRadialGradient:(CGContextRef) context
{
    
    //2.创建一个渐变
    //2.1 创建一个颜色空间
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    //2.2 设置开始颜色和结束颜色
    UIColor *startColor = COLOR_GOLD_THEME;
    const CGFloat *startColorComponents = CGColorGetComponents([startColor CGColor]);
    
    UIColor *endColor = COLOR_VC_BG;
    const  CGFloat *endColorComponents =CGColorGetComponents([endColor CGColor]);
    
    //颜色分量的强度值数组
    CGFloat components[8] = {startColorComponents[0],startColorComponents[1],
        startColorComponents[2],startColorComponents[3],
        endColorComponents[0],endColorComponents[1],
        endColorComponents[2],endColorComponents[3]
    };
    
    //渐变系数数组
    CGFloat locations[2] = {0.0,1.0};
    
    //创建渐变对象(参数：颜色空间、颜色分量的强度值数组、渐变系数数组、设置的渐变系数个数)
    CGGradientRef gradient= CGGradientCreateWithColorComponents(colorSpace, components, locations, 2);
    
    MYLog(@"drawsplitelineView %@",NSStringFromCGRect(self.frame));
    //3.画渐变(参数：             上下文、  渐变对象、  起点、        径向半径、终点、    径向半径、   渐变模式)
    CGContextDrawRadialGradient(context, gradient, self.center, 60, self.center, 140, kCGGradientDrawsBeforeStartLocation);
    //渐变模式：
    //kCGGradientDrawsAfterEndLocation      //向外发散
    //kCGGradientDrawsBeforeStartLocation   //向里发散
    //4.清理
    CGColorSpaceRelease(colorSpace);
    CGGradientRelease(gradient);
}
@end
