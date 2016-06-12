//
//  TransferQueryController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/3/16.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "SuperViewController.h"
#import "TitleBanner.h"

@interface TransferQueryController : SuperViewController <UITableViewDataSource,UITableViewDelegate,NSTradeEngineDelegate>
{
    TitleBanner *_titleBanner;
    UITableView *_tableView;
}
@end
