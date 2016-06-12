//
//  EntrustCancelBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "EntrustCancelBanner.h"
#define kBannerFont [UIFont fontWithName:@"Helvetica-Bold" size:12]

@implementation EntrustCancelBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.backgroundColor = COLOR_ENTRUST_BANNAR_BG;
    [self addLables];
    return self;
}

- (void)addLables
{
    CGFloat labelY = 0;
    CGFloat labelW = self.frame.size.width /6;
    CGFloat labelH = kBannerFont.lineHeight;
    
    UILabel *entrustPrice = [self setLableWithRect:CGRectMake(labelW, labelY, labelW, labelH)];
    entrustPrice.text = LocalizedStringByInt(1901);
    
    UILabel *entrustDeal = [self setLableWithRect:CGRectMake(labelW*2, labelY, labelW, labelH)];
    entrustDeal.text = LocalizedStringByInt(1902);
    
    UILabel *status = [self setLableWithRect:CGRectMake(labelW*3, labelY, labelW, labelH)];
    status.text = LocalizedStringByInt(1903);
    
    UILabel *buySell = [self setLableWithRect:CGRectMake(labelW*4, labelY, labelW, labelH)];
    buySell.text = LocalizedStringByInt(1904);
    
    UILabel *nowPrice = [self setLableWithRect:CGRectMake(labelW*5, labelY, labelW, labelH)];
    nowPrice.text = LocalizedStringByInt(1905);
}

- (UILabel *)setLableWithRect:(CGRect)rect
{
    UILabel *lable = [[UILabel alloc] initWithFrame:rect];
    lable.font = kBannerFont;
    lable.textColor = COLOR_GOLD_THEME;
    lable.textAlignment = NSTextAlignmentCenter;
    [self addSubview:lable];
    return lable;
}

@end
