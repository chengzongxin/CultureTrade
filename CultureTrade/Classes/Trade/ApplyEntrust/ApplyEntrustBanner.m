//
//  ApplyEntrustBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/20.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyEntrustBanner.h"

@interface ApplyEntrustBanner ()
@property (nonatomic,assign) int count;
@end

@implementation ApplyEntrustBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    self.backgroundColor = COLOR_APPLYPURCHASE_BANNER_BG;
    
    return self;
}


- (void)setCount:(int)count textIndex:(int)index
{
    // 商品边框取宽度的1/5，剩下的4/5，分给剩下的5个
    CGFloat width = self.frame.size.width / count;
    CGFloat height = self.frame.size.height;
    
    for (int i = 0 ; i < count; i ++) {
        UILabel *lable = [[UILabel alloc] initWithFrame:(CGRect){{width*i,0},{width,height}}];
        lable.text = LocalizedStringByInt(index+i);
        lable = [self setLabel:lable];
        [self addSubview:lable];
    }
}

- (UILabel *)setLabel:(UILabel *)lable
{
    lable.textColor = COLOR_APPLYPURCHASE_BANNER_TEXT;
    lable.font = [UIFont systemFontOfSize:12];
    lable.textAlignment = NSTextAlignmentCenter;
    return lable;
}


@end
