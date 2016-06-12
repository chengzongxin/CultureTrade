//
//  QuotationController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "LeftBanner.h"
#import "SuperViewController.h"
@class QuotaionBanner;
@interface QuotationController : SuperViewController
{
    LeftBanner *_leftBanner;
    QuotaionBanner *_banner;
}
@property (nonatomic,retain) UITableView *leftTableView;
@property (nonatomic,retain) UITableView *mainTableView;
@end
