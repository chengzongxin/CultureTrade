//
//  ScaleButton.m
//  CultureTrade
//
//  Created by SZFT1 on 16/8/17.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ScaleButton.h"

@implementation ScaleButton

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.titleLabel.font = [UIFont systemFontOfSize: 10.0];
    self.layer.cornerRadius = 10.0f;
    self.layer.borderColor = [UIColor whiteColor].CGColor;
    self.layer.borderWidth = 0.5f;
    self.layer.masksToBounds = YES;
    self.backgroundColor = [UIColor clearColor];
    return self;
}

@end
