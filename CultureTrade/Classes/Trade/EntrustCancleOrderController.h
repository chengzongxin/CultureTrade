//
//  EntrustCancleOrderController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"
#import "EntrustCancelBanner.h"
#import "SuperViewController.h"
#import "EntrustCancelCell.h"
#import "SZCalendarPicker.h"
#import "OrderModel.h"
#import "GlobalModel.h"
#import "UIBarButtonItem+helper.h"
#import "BasicAlertView.h"
#import "tagdef.h"
#import "NSDate+helper.h"
@interface EntrustCancleOrderController : SuperViewController<UITableViewDataSource,UITableViewDelegate,NSTradeEngineDelegate>
{
    EntrustCancelBanner *_banner;
    NSMutableArray *_orderArray;
    UITableView *_tableView;
}
@property (nonatomic,retain) UITableView *tableView;
- (void)addTitleView;
- (void)addBanner;
- (void)addTableView;
@end
