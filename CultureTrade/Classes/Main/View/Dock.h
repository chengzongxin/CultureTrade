//
//  Dock.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Dock;

@protocol DockDelegate <NSObject>
@optional
- (void)dock:(Dock *)dock itemSelectedFrom:(int)from to:(int)to;

@end


@interface Dock : UIView

@property (nonatomic,weak) id<DockDelegate> delegate;

- (void)addItemWithIcon:(NSString *)icon title:(NSString *)title;
- (void)setDockItemColorNormal:(UIColor *)normal selected:(UIColor *)selected;
- (void)addSpliteLine;
- (void)clickFirstItem;
- (void)clickItem:(int)index;
@end
