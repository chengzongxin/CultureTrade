//
//  LeftBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 16/4/6.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "LeftBanner.h"

@implementation LeftBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    self.backgroundColor = COLOR_QUOTA_BANNAR_BG;
    // 商品边框取宽度的1/5，剩下的4/5，分给剩下的5个
    
    UILabel *goods = [[UILabel alloc] initWithFrame:(CGRect){CGPointZero,self.frame.size}];
    goods.text = LocalizedStringByInt(1302);
    goods = [self setLabel:goods];
    [self addSubview:goods];
    return self;
}

- (UILabel *)setLabel:(UILabel *)lable
{
    lable.textColor = COLOR_QUOTA_BANNAR_TEXT;
    lable.font = [UIFont systemFontOfSize:12];
    lable.textAlignment = NSTextAlignmentCenter;
    return lable;
}


@end
