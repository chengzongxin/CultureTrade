//
//  TipView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/2.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TipView : UIView
@property (nonatomic,retain) UILabel *date;
@property (nonatomic,retain) UILabel *openPrice;
@property (nonatomic,retain) UILabel *highestPrice;
@property (nonatomic,retain) UILabel *lowestPrice;
@property (nonatomic,retain) UILabel *closePrice;
@property (nonatomic,retain) UILabel *volume;
@property (nonatomic,retain) UILabel *raiseRange;
@end
