//
//  UIBarButtonItem+helper.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/27.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "UIBarButtonItem+helper.h"

@implementation UIBarButtonItem (helper)

- (id)initWithIcon:(NSString *)icon highlightedIcon:(NSString *)highlighted target:(id)target action:(SEL)action
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    UIImage *img = [UIImage imageNamed:icon];
    [btn setImage:img forState:UIControlStateNormal];
    
    if (highlighted) {
        [btn setImage:[UIImage imageNamed:highlighted] forState:UIControlStateHighlighted];
    }
    
    btn.frame = (CGRect){CGPointZero,img.size};
    
    [btn addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    
    return [self initWithCustomView:btn];
    
}

+ (id)itemWithIcon:(NSString *)icon highlightedIcon:(NSString *)highlighted target:(id)target action:(SEL)action
{
    return [[self alloc] initWithIcon:icon highlightedIcon:highlighted target:target action:action];
}

- (id)initWithTitle:(NSString *)title icon:(NSString *)icon target:(id)target action:(SEL)action
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    UIImage *img = [UIImage imageNamed:icon];
//    [btn setTitle:title forState:UIControlStateNormal];
    btn.titleLabel.text = title;
    [btn setImage:img forState:UIControlStateNormal];
    
    btn.frame = (CGRect){CGPointZero,img.size};
    
    [btn addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    
    return [self initWithCustomView:btn];
}

+ (id)itemWithTitle:(NSString *)title icon:(NSString *)icon target:(id)target action:(SEL)action
{
    return [[self alloc] initWithTitle:title icon:icon target:target action:action];
}
@end
