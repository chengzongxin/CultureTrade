//
//  QueryController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "QueryController.h"
#import "EntrustCancelCell.h"
#import "UIBarButtonItem+helper.h"
#import "BasicAlertView.h"
#import "NSTradeEngine.h"
#import "ButtonListView.h"
#import "GlobalModel.h"
#define kBannerHeight 20
#define kCellHeight 35

typedef struct trunOver {
    NSString *customerID;//1
    NSString *commision;//12
    NSString *turnOverPrice;//31
    NSString *turnOverVol;//32
    NSString *totalMoney;//684
    NSString *orderType;//40 0B,0S,
    NSString *securityID;//48 ID
    NSString *timeForce;//60
    NSString *productName;//9402
    NSString *contractCode;//9501//成交合同编号
    NSString *opponentID;//9502 //成交对手方编号
    NSString *opponentName;//9503 //成交对手方名称
}trunOverStruct;

@interface QueryController () 
{
    NSMutableArray *_turnOverArray;
}
@end

@implementation QueryController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    
    [self addTitleView];
   
    [self addBanner];
    
    [self addTableView];
    
    [NSTradeEngine sharedInstance].delegate = self;
    _turnOverArray = [NSMutableArray array];
}

- (void)addTitleView
{
//    UIImage *titleImage = [UIImage imageNamed:@"queryTitle"];
//    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
//    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
//    self.navigationItem.titleView = titleView;
     self.title = @"今日成交";
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)addBanner
{
    _banner = [[EntrustCancelBanner alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, kBannerHeight)];
    UILabel *label = _banner.subviews[0];
    label.text = @"成交价";
    [self.view addSubview:_banner];
}

- (void)addTableView
{
    CGRect rect = CGRectMake(0, 40 + 5,self.view.frame.size.width, self.view.frame.size.height - 40 - 5 - 65);
    _tableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _tableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _tableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    [self.view addSubview:_tableView];
}

#pragma mark - UITableView Delegate


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return kCellHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _turnOverArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"EntrustCellID";
    EntrustCancelCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[EntrustCancelCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    NSDictionary *dict = [_turnOverArray objectAtIndex:indexPath.row];
//    OrderModel *orderModel = [OrderModel oderWithDict:turnOverDict];
    
    trunOverStruct trunOver = {
        dict[INT2STRING(1)],
        dict[INT2STRING(12)],
        dict[INT2STRING(31)],
        dict[INT2STRING(32)],
        dict[INT2STRING(684)],
        dict[INT2STRING(40)],
        dict[INT2STRING(48)],
        dict[INT2STRING(60)],
        dict[INT2STRING(9402)],
        dict[INT2STRING(9501)],
        dict[INT2STRING(9502)],
        dict[INT2STRING(9503)]
    };
    cell.stockID.text = trunOver.securityID;
    cell.stockName.text = trunOver.productName;
    float turnPrice = [trunOver.turnOverPrice floatValue];
    cell.entrustPrice.text = [NSString stringWithFormat:@"%0.2f",turnPrice];
    cell.entrustDeal.text = trunOver.turnOverVol;
    cell.status.text = @"成交";
    if ([trunOver.orderType isEqualToString:@"0B"]) {
        cell.buySell.text = @"买";
    }else if ([trunOver.orderType isEqualToString:@"0S"]) {
        cell.buySell.text = @"卖";
    }
//    cell.nowPrice.text = [NSString stringFormatFromFullDate:trunOver.timeForce];
    for (SortUnit *sortUnit in GLOBAL.sortUnitArray) {
        if ([trunOver.securityID intValue] == sortUnit.m_CodeInfo.m_uiCode) {
            cell.nowPrice.text = [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiNewPrice];
            if (sortUnit.m_uiNewPrice == 0.0) {
                cell.nowPrice.text = @"--";
            }
        }
    }
    return cell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_tableView deselectRowAtIndexPath:indexPath animated:YES];
}


- (void)viewDidLayoutSubviews
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([self.tableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [self.tableView setSeparatorInset:UIEdgeInsetsMake(0, 0, 0, 0)];
    }
    
    if ([self.tableView respondsToSelector:@selector(setLayoutMargins:)]) {
        [self.tableView setLayoutMargins:UIEdgeInsetsMake(0, 0, 0, 0)];
    }
}

-(void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([cell respondsToSelector:@selector(setSeparatorInset:)]) {
        [cell setSeparatorInset:UIEdgeInsetsZero];
    }
    
    if ([cell respondsToSelector:@selector(setLayoutMargins:)]) {
        [cell setLayoutMargins:UIEdgeInsetsZero];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self request_today_turnover];
}

- (void)request_today_turnover
{
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    [[NSTradeEngine sharedInstance] trade_request_today_dealNS];
    [self showProgress:@"Loading..."];
}

- (void)trade_ui_today_deal_rsp:(NSString *)data
{
    [self closeProgressSuccess:@"Success!"];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"HH:mm"];
    NSString *now = [formatter stringFromDate:[NSDate date]];
    
    if ([now compare:@"15:30"] == NSOrderedDescending) return; //大于15:30
    
    NSError *error;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[data dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    NSArray *array = [str2dict objectForKey:@"TURNOVER"];
    _turnOverArray = [NSMutableArray arrayWithArray:array];
    [_tableView reloadData];
}

@end
