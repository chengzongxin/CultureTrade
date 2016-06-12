//
//  ButtonListView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/1.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol ButtonListViewDelegate <NSObject>
@optional
- (void)buy;
- (void)sell;
- (void)cancelOrder;
- (void)queryOrder;

@end

@interface ButtonListView : UIView
@property (nonatomic,retain) UIButton *buy;
@property (nonatomic,retain) UIButton *sell;
@property (nonatomic,retain) UIButton *cancelOrder;
@property (nonatomic,retain) UIButton *query;

@property (nonatomic,weak) id<ButtonListViewDelegate> delegate;

@end
