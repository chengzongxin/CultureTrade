//
//  TipView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/2.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "TipView.h"

@implementation TipView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.8];
    self.layer.borderWidth = 0.5f;
    self.layer.borderColor = [UIColor whiteColor].CGColor;
    [self initialization];
    return self;
}

- (void)initialization
{
    
    CGFloat width = self.frame.size.width;
    CGFloat height = 15;
    
    _date = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, width, height)];
    [self setCell:_date];
    [self addSubview:_date];
    
    _openPrice = [[UILabel alloc] initWithFrame:CGRectMake(0, 15, width, height)];
    [self setCell:_openPrice];
    [self addSubview:_openPrice];
    
    _highestPrice = [[UILabel alloc] initWithFrame:CGRectMake(0, 30, width, height)];
    [self setCell:_highestPrice];
    [self addSubview:_highestPrice];
    
    _lowestPrice = [[UILabel alloc] initWithFrame:CGRectMake(0, 45, width, height)];
    [self setCell:_lowestPrice];
    [self addSubview:_lowestPrice];
    
    _closePrice = [[UILabel alloc] initWithFrame:CGRectMake(0, 60, width, height)];
    [self setCell:_closePrice];
    [self addSubview:_closePrice];
    
    _volume = [[UILabel alloc] initWithFrame:CGRectMake(0, 75, width, height)];
    [self setCell:_volume];
    [self addSubview:_volume];
    
    _raiseRange = [[UILabel alloc] initWithFrame:CGRectMake(0, 90, width, height)];
    [self setCell:_raiseRange];
    [self addSubview:_raiseRange];
}

- (void)setCell:(UILabel *)lable
{
    lable.font = [UIFont systemFontOfSize:9];
    lable.textColor = [UIColor whiteColor];
    lable.lineBreakMode = NSLineBreakByWordWrapping;
    lable.textAlignment = NSTextAlignmentLeft;
}

@end
