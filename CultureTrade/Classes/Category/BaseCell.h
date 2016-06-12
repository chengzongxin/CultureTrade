//
//  BaseCell.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
@class SymbolModel;
@interface BaseCell : UITableViewCell
@property (nonatomic,retain) UILabel *openPrice;
@property (nonatomic,retain) UILabel *range;
@property (nonatomic,retain) UILabel *maxPrice;
@property (nonatomic,retain) UILabel *preClose;
@property (nonatomic,retain) UILabel *avePrice;
@property (nonatomic,retain) UILabel *curPrice;
@property (nonatomic,retain) UILabel *raiseDown;
@property (nonatomic,retain) UILabel *minPrice;
@property (nonatomic,retain) UILabel *curVolume;
@property (nonatomic,retain) UILabel *buyPrice;
@property (nonatomic,retain) UILabel *buyVolume;
@property (nonatomic,retain) UILabel *sellPrice;
@property (nonatomic,retain) UILabel *sellVolume;
@property (nonatomic,retain) UILabel *turnOverVol;
@property (nonatomic,retain) UILabel *inventories;
@property (nonatomic,retain) UILabel *turnOverMoney;
@property (nonatomic,retain) UILabel *volumeRatio;
@property (nonatomic,retain) UILabel *entrustRatio;
@property (nonatomic,retain) UILabel *turnOverRate;


- (BaseCell *)initWithSortUnit:(SortUnit *)sortUnit;
@end
