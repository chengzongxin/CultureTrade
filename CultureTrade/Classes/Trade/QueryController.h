//
//  QueryController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BaseViewController.h"
#import "EntrustCancelBanner.h"
#import "SuperViewController.h"
@interface QueryController : SuperViewController <UITableViewDataSource,UITableViewDelegate,NSTradeEngineDelegate>
{
    EntrustCancelBanner *_banner;
    UITableView *_tableView;
}
@property (nonatomic,retain) UITableView *tableView;
- (void)addBanner;
- (void)request_today_turnover;
- (void)addTitleView;
- (void)addTableView;
@end
