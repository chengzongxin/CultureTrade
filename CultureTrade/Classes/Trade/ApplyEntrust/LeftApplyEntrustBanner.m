//
//  LeftApplyEntrustBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/20.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "LeftApplyEntrustBanner.h"

@implementation LeftApplyEntrustBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    self.backgroundColor = COLOR_APPLYPURCHASE_BANNER_BG;
    // 商品边框取宽度的1/5，剩下的4/5，分给剩下的5个
    
    UILabel *goods = [[UILabel alloc] initWithFrame:(CGRect){CGPointZero,self.frame.size}];
    goods.text = LocalizedStringByInt(1302);
    goods = [self setLabel:goods];
    [self addSubview:goods];
    return self;
}

- (UILabel *)setLabel:(UILabel *)lable
{
    lable.textColor = COLOR_APPLYPURCHASE_BANNER_TEXT;
    lable.font = [UIFont systemFontOfSize:12];
    lable.textAlignment = NSTextAlignmentCenter;
    return lable;
}


@end
