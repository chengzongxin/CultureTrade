//
//  SearchProductController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/4/7.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SuperViewController.h"
@interface SearchProductController : SuperViewController
@property (nonatomic,retain) UISearchController *searchController;
@property (nonatomic,retain) NSMutableArray *codeArray;
@property (nonatomic,retain) NSMutableArray *nameArray;
@property (nonatomic,retain) NSMutableArray *filterCodeArray;
@property (nonatomic,retain) NSMutableArray *filterNameArray;
@property (nonatomic,retain) UITableView *tableView;
@end
