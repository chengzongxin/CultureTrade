//
//  QuotaionBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/20.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "QuotaionBanner.h"
#define kCount 19

@implementation QuotaionBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    self.backgroundColor = COLOR_QUOTA_BANNAR_BG;
    // 商品边框取宽度的1/5，剩下的4/5，分给剩下的5个
    CGFloat width = self.frame.size.width / kCount;
    CGFloat height = self.frame.size.height;
    
    for (int i = 0 ; i < kCount; i ++) {
        UILabel *lable = [[UILabel alloc] initWithFrame:(CGRect){{width*i,0},{width,height}}];
        lable.text = LocalizedStringByInt(1303+i);
        lable = [self setLabel:lable];
        [self addSubview:lable];
    }
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
