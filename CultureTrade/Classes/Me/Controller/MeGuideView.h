//
//  MeGuideView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/3.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MeGuideView : UIView
{
    UILabel *_foundTag;
    UILabel *_foundLable;
    UILabel *_monthDealTag;
    UILabel *_monthDealLable;
    UILabel *_sucRatioTag;
    UILabel *_sucRatioLabel;
    UILabel *_totalGainTag;
    UILabel *_totalGainLable;
    UILabel *_thisMonthGainTag;
    UILabel *_thisMonthGainLable;
    UILabel *_thisWeekGainTag;
    UILabel *_thisWeekGainLabel;
}

@property (nonatomic,retain) UILabel *foundLable;
@property (nonatomic,retain) UILabel *monthDealLable;
@property (nonatomic,retain) UILabel *sucRatioLabel;
@property (nonatomic,retain) UILabel *totalGainLable;
@property (nonatomic,retain) UILabel *thisMonthGainLable;
@property (nonatomic,retain) UILabel *thisWeekGainLabel;

@end
