//
//  TitleViewButton.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/31.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TitleViewButton.h"

@implementation TitleViewButton

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.layer.borderWidth = 1.0f;
    self.titleLabel.font = [UIFont systemFontOfSize:12];
    self.titleLabel.textColor = COLOR_NAVIGATION_TITLE;
    self.layer.borderWidth = 0.0f;
    return self;
}

- (void)setTitle:(NSString *)title icon:(NSString *)icon selected:(NSString *)selected
{
    [self setImage:[UIImage imageNamed:title] forState:UIControlStateNormal];
    [self setImage:[UIImage imageNamed:selected] forState:UIControlStateNormal];
    [self setTitle:title forState:UIControlStateNormal];
    
}

- (void)setImage:(UIImage *)image forState:(UIControlState)state{}
@end
