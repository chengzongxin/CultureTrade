//
//  DockItem.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "DockItem.h"
#import "UIImage+helper.h"
#define kImageRatio 0.5

@implementation DockItem

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    self.titleLabel.textAlignment = NSTextAlignmentLeft;
    
    self.titleLabel.font = [UIFont systemFontOfSize:12];
    
    [self setTitleColor:[UIColor colorWithHexString:@"#785104" withAlpha:1] forState:UIControlStateNormal];
    
    [self setTitleColor:COLOR_GOLD_THEME forState:UIControlStateSelected];
    
//    self.layer.borderColor = [Main_BG_Color CGColor];
//    
//    self.layer.borderWidth = 0.3f;
    
    self.imageView.contentMode = UIViewContentModeRight;
    
//    [self setBackgroundImage:[UIImage strechImageWithImg:@"bottom"] forState:UIControlStateNormal];
//    [self setBackgroundColor:[UIColor colorWithPatternImage:[UIImage strechImageWithImg:@"bottom"]]];
//    UIImage *ItemBackground = [UIImage createImageWithColor:Main_BG_Color];
//    [self setBackgroundImage:ItemBackground forState:UIControlStateSelected];
    
    return self;
}

- (void)setHighlighted:(BOOL)highlighted{
    [self setBackgroundImage:[UIImage createImageWithColor:COLOR_TABBAR_SELECTBG] forState:UIControlStateSelected];
    if (self.selectedColor) {
        [self setBackgroundImage:[UIImage createImageWithColor:self.selectedColor] forState:UIControlStateSelected];
    }
}



- (void)setSelectedColor:(UIColor *)color
{
    _selectedColor = color;   //  这里用self.selected = color; 会造成死循环
    [self setHighlighted:YES];
}


- (CGRect)imageRectForContentRect:(CGRect)contentRect
{
    CGFloat x = -3;
    CGFloat y = 0;
    CGFloat w = contentRect.size.width * kImageRatio;
    CGFloat h = contentRect.size.height;
    return CGRectMake(x, y, w, h);
}

- (CGRect)titleRectForContentRect:(CGRect)contentRect
{
    CGFloat x = contentRect.size.width * kImageRatio+3;
    CGFloat y = 0;
    CGFloat w = contentRect.size.width * (1 - kImageRatio);
    CGFloat h = contentRect.size.height;
    return CGRectMake(x, y, w, h);
}



@end
