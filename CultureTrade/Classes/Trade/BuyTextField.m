//
//  BuyTextField.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/16.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BuyTextField.h"

@implementation BuyTextField

- (void)drawPlaceholderInRect:(CGRect)rect
{
//    [super drawPlaceholderInRect:rect];
    
    [[self placeholder] drawInRect:rect withAttributes:@{
                                                         NSFontAttributeName : [UIFont boldSystemFontOfSize:12],
                                                         NSForegroundColorAttributeName : [UIColor colorWithHexString:@"#FFFFFF" withAlpha:1]
                                                         }];
}

//控制编辑文本的位置
-(CGRect)editingRectForBounds:(CGRect)bounds
{
    //return CGRectInset( bounds, 10 , 0 );
    
    CGRect inset = CGRectMake(bounds.origin.x+30, bounds.origin.y, bounds.size.width -10, bounds.size.height);//更好理解些
    return inset;
}

@end
