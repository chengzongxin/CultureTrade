//
//  UIButton+helper.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/10.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "UIButton+helper.h"

@implementation UIButton (helper)
- (void)setImage:(UIImage *)image withTitle:(NSString *)title forState:(UIControlState)stateType {
    //UIEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right)
    CGSize titleSize = [title sizeWithFont:[UIFont systemFontOfSize:12.0f]];
    [self.imageView setContentMode:UIViewContentModeCenter];
    [self setImageEdgeInsets:UIEdgeInsetsMake(-15.0, 0.0, 0.0, -titleSize.width)];
    [self setImage:image forState:stateType];
    
    [self.titleLabel setContentMode:UIViewContentModeCenter];
    [self.titleLabel setBackgroundColor:[UIColor clearColor]];
    [self.titleLabel setFont:[UIFont systemFontOfSize:12.0f]];
    [self.titleLabel setTextColor:[UIColor whiteColor]];
    [self setTitleEdgeInsets:UIEdgeInsetsMake(image.size.height, -image.size.width, 0.0, 0.0)];
    [self setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self setTitle:title forState:stateType];
}
@end
