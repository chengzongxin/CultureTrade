//
//  UIBarButtonItem+helper.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/27.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIBarButtonItem (helper)

+ (id)itemWithIcon:(NSString *)icon highlightedIcon:(NSString *)highlighted target:(id)target action:(SEL)action;
- (id)initWithIcon:(NSString *)icon highlightedIcon:(NSString *)highlighted target:(id)target action:(SEL)action;
- (id)initWithTitle:(NSString *)title icon:(NSString *)icon target:(id)target action:(SEL)action;
+ (id)itemWithTitle:(NSString *)title icon:(NSString *)icon target:(id)target action:(SEL)action;
@end
