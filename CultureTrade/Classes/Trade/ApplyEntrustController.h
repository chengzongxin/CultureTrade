//
//  ApplyEntrustController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/19.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "SuperViewController.h"
#import "SZCalendarPicker.h"
#import "LeftApplyEntrustBanner.h"
#import "LeftApplyEntrustCell.h"
#import "ApplyEntrustBanner.h"
#import "ApplyEntrustCell.h"
#import "GlobalModel.h"
#define kLeftTableWith 70
#define kMainTableViewWithRatio 2.2
@interface ApplyEntrustController : SuperViewController<NSTradeEngineDelegate,UITableViewDelegate,UITableViewDataSource>
{
    UILabel *_dateLabel;
    UILabel *_toLabel;
    UIButton *_startBtn;
    UIButton *_endBtn;
    UIButton *_searchBtn;
    
    UITableView *_leftTable;
    UITableView *_mainTable;
    
    NSMutableData *_datas;
}

@property (nonatomic,retain) UILabel *dateLabel;
@property (nonatomic,retain) UILabel *toLabel;
@property (nonatomic,retain) UIButton *startBtn;
@property (nonatomic,retain) UIButton *endBtn;
@property (nonatomic,retain) UIButton *searchBtn;

@property (nonatomic,retain) UITableView *leftTable;
@property (nonatomic,retain) UITableView *mainTable;
@end
