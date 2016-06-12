//
//  EntrustCancleOrderController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "EntrustCancleOrderController.h"
#import "UIBarButtonItem+helper.h"
#import "BasicAlertView.h"
#import "OrderModel.h"
#import "GlobalModel.h"

#define kBannerHeight 20
#define kCellHeight 35

@interface EntrustCancleOrderController () 

@end

@implementation EntrustCancleOrderController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    
    [self addTitleView];
    
    [self addBanner];
    
    [self addTableView];
    
    _orderArray = [NSMutableArray array];
}

- (void)addTitleView
{
//    UIImage *titleImage = [UIImage imageNamed:@"entrustCancelOrderTitle"];
//    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
//    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
//    self.navigationItem.titleView = titleView;
    
    self.title = @"今日委托";
}
- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)addBanner
{
    _banner = [[EntrustCancelBanner alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, kBannerHeight)];
    [self.view addSubview:_banner];
}

- (void)addTableView
{
    CGRect rect = CGRectMake(0, 40 + 5,self.view.frame.size.width, self.view.frame.size.height - 40 - 5 - 64);
    _tableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _tableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _tableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    _tableView.bounces = YES;
    [self.view addSubview:_tableView];
}

#pragma mark - UITableView Delegate


- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return kCellHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _orderArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"EntrustCellID";
    EntrustCancelCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[EntrustCancelCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    
    NSDictionary *orderDict = [_orderArray objectAtIndex:indexPath.row];
    OrderModel *orderModel = [OrderModel oderWithDict:orderDict];
    
    cell.stockID.text = orderModel.stockID;
    cell.stockName.text = orderModel.stockName;
    cell.entrustPrice.text = [NSString stringWithFormat:@"%0.2f",[orderModel.entrustPrice floatValue]];
    cell.entrustDeal.text = orderModel.askNum;
    cell.status.text = @"已下单";
    
    if ([orderModel.orderType isEqualToString:@"0B"]) {
        cell.buySell.text = @"买";
    }else if ([orderModel.orderType isEqualToString:@"0S"]) {
        cell.buySell.text = @"卖";
    }
    
    for (SortUnit *sortUnit in GLOBAL.sortUnitArray) {
        if ([orderModel.stockID intValue] == sortUnit.m_CodeInfo.m_uiCode) {
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
    NSDictionary *orderDict = [_orderArray objectAtIndex:indexPath.row];
    OrderModel *order = [OrderModel oderWithDict:orderDict];
    NSString *title = order.stockName;
    if (title.length > 7) {
        title = [NSString stringWithFormat:@"%@...",[order.stockName substringToIndex:7]];
    }
    NSString *context = [NSString stringWithFormat:@"名称:%@         价格:%0.2f\n数量:%@",title,[order.entrustPrice floatValue],order.askNum];
    BasicAlertView *alertView = [BasicAlertView alertViewWithTitle:@"撤单信息!"
                                                subtitle:@"您是否确认撤单?"
                                                 message:context
                                                 buttons:[NSArray arrayWithObjects:@"confirm", @"cancel", nil]
                                            afterDismiss:^(int buttonIndex) {
                                                if (buttonIndex == 0) {
                                                    [self alertViewClickConfirmButtonAtIndex:order.orderID];
                                                }
                                            }];
    
    [alertView show];
}

- (void)alertViewClickConfirmButtonAtIndex:(NSString *)orderID
{
    [[NSTradeEngine sharedInstance] trade_request_cancel_order:orderID];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
}


- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
    [self request_today_order];
}

- (void)request_today_order
{
    [[NSTradeEngine sharedInstance] trade_request_today_entrustNS];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
}


- (void)trade_ui_today_entrust_rsp:(NSString *)data
{
    _orderArray = [NSMutableArray array];
    NSError *error = nil;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[data dataUsingEncoding:NSUTF8StringEncoding]
                                                         options:NSJSONReadingMutableContainers
                                                           error:&error];
    NSMutableArray *array = [str2dict objectForKey:@"ORDER"];
    NSMutableArray *extraArray = [NSMutableArray arrayWithArray:array];
    for (NSDictionary *orderDict in array) {
        OrderModel *orderModel = [OrderModel oderWithDict:orderDict];
        
        int exeState = [orderModel.exeState intValue];
        if (exeState != 1) [extraArray removeObject:orderDict];
    }
    _orderArray = [NSMutableArray arrayWithArray:extraArray];
//    _tableView.frame = CGRectMake(_tableView.frame.origin.x, _tableView.frame.origin.y, _tableView.frame.size.width, _tableView.frame.size.height*_orderArray.count);
    [_tableView reloadData];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
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


@end
