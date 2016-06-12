//
//  GuideView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GuideView : UIView
@property (nonatomic,retain) UILabel *price;
@property (nonatomic,retain) UILabel *increase;
@property (nonatomic,retain) UILabel *increasePercent;
@property (nonatomic,retain) UILabel *todayOpen;
@property (nonatomic,retain) UILabel *yesterdayClose;
@property (nonatomic,retain) UILabel *highest;
@property (nonatomic,retain) UILabel *lowest;
@property (nonatomic,retain) UILabel *dealVolume;
@property (nonatomic,retain) UILabel *dealTurnover;
@property (nonatomic,retain) UIButton *plus;
- (void)fillColor;

//singleton_interface(GuideView)

@end
