//
//  Dock.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "Dock.h"
#import "DockItem.h"
#import "NSString+helper.h"
#import "UIImage+helper.h"

@interface Dock()
{
    DockItem *_selectedItem;
}

@end
@implementation Dock

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
//    self.backgroundColor = Tabbar_Backgrond_Color;
    self.backgroundColor = [UIColor colorWithPatternImage:[UIImage strechImageWithImg:@"bottom"]];
    self.layer.shadowColor = [UIColor blackColor].CGColor;
    self.layer.shadowOffset = CGSizeMake(0, -5);
    self.layer.shadowOpacity = 0.5;
    
    return self;
}


- (void)addItemWithIcon:(NSString *)icon title:(NSString *)title
{
    DockItem *item = [[DockItem alloc] init];
    [item setTitle:title forState:UIControlStateNormal];
    item.titleLabel.font = [UIFont systemFontOfSize:11];
    [item setImage:[UIImage imageNamed:icon] forState:UIControlStateNormal];
    [item setImage:[UIImage imageNamed:[icon fileAppend:@"_selected"]] forState:UIControlStateSelected];
    [item addTarget:self action:@selector(itemClick:) forControlEvents:UIControlEventTouchDown];
    
    [self addSubview:item];
    
    int count = (int)self.subviews.count;
    CGFloat height = self.frame.size.height;
    CGFloat width = self.frame.size.width / count;
    
    if (count == 1) {
        [self itemClick:item];
    }
    
    for (int i = 0 ; i < count ; i++) {
        DockItem *dockItem = self.subviews[i];
        dockItem.tag = i;
        dockItem.frame = CGRectMake(width * i, 0, width, height);
        
    }
}


- (void)setDockItemColorNormal:(UIColor *)normal selected:(UIColor *)selected
{
    for (id dockItem in self.subviews) {
        if ([dockItem isKindOfClass:[DockItem class]]) {
            [dockItem setTitleColor:normal forState:UIControlStateNormal];
            [dockItem setTitleColor:selected forState:UIControlStateSelected];
//            [dockItem setKImageRatio:0.5];
        }
    }
}

- (void)clickFirstItem
{
    DockItem *dockItem = self.subviews.firstObject;
    dockItem.highlighted = YES;
    
}

- (void)clickItem:(int)index
{
    DockItem *item = self.subviews[index];
    item.highlighted = YES;
    [self itemClick:item];
}


- (void)addSpliteLine
{
    
    int count = (int)self.subviews.count;
    CGFloat height = self.frame.size.height;
    CGFloat width = self.frame.size.width / count;
    
    
    for (int i = 0 ; i < count ; i++) {
        if (i == 0) continue;
        CGRect rect = CGRectMake(width * i - 0.5, 0, 1, height);
        [self addsplieteViewWithRect:rect];
    }
//    [self addsplieteViewWithRect:CGRectMake(160, 0, 5, 44)];
}

- (void)addsplieteViewWithRect:(CGRect)rect
{
    
    UIImageView *spliteImg = [[UIImageView alloc] init];
    spliteImg.frame = rect;
    spliteImg.image = [UIImage imageNamed:@"sepliteLine"];
//    spliteImg.image = [UIImage createImageWithColor:[UIColor grayColor]];
    [self addSubview:spliteImg];
    
    
}

- (void)itemClick:(DockItem *)item
{
    if ([_delegate respondsToSelector:@selector(dock:itemSelectedFrom:to:)]) {
        [_delegate dock:self itemSelectedFrom:(int)_selectedItem.tag to:(int)item.tag];
    }
    _selectedItem.selected = NO;
    
    item.selected = YES;
    
    _selectedItem = item;
}

@end
