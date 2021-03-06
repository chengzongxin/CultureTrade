//
//  MeController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NSString+helper.h"
#import "MeFunctionCell.h"
#import "UIBarButtonItem+helper.h"
#import "MeGuideView.h"
#import "ButtonListView.h"
#import "BuyController.h"
#import "BaseNavigationController.h"
#import "SellController.h"
#import "EntrustCancleOrderController.h"
#import "QueryController.h"
#import "GlobalModel.h"
#import "InOutMoneyController.h"
#import "TransferQueryController.h"
#import "ModifyPasswordController.h"
#import "SuperViewController.h"
#import <YWFeedbackFMWK/YWFeedbackKit.h>

@interface MeController : SuperViewController<UITableViewDataSource,UITableViewDelegate,ButtonListViewDelegate,NSTradeEngineDelegate>
{
    UIView *_customView;
    UITableView *_tableView;
    MeGuideView *_guideView;
    ButtonListView *_buttonListView;
    
}
@property (nonatomic,retain) UITableView *tableView;
@property (nonatomic,retain) UIImageView *faceImageView;
@property (nonatomic,retain) UILabel *nameLable;
@property (nonatomic, strong) YWFeedbackKit *feedbackKit;
- (void)addGuideView;

- (void)addButtonListView;

- (void)addTableView;
@end
