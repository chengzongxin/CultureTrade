//
//  EntrustCancleOrderController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "EntrustCancleOrderController.h"


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
    
    self.title = @"委托撤单";
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
    cell.entrustDeal.text = [NSString stringWithFormat:@"%@/%@",orderModel.askNum,orderModel.dealVolume];
    // '1 :已提交 \r\n2 成交 \r\n3 部分成交 \r\n4主动撤单  \r\n5交易所拒绝 \r\n6已冻结 \r\n7 其他 \r\n 8 已过期',
    switch ([orderModel.exeState intValue]) {
        case 1:
            cell.status.text = @"已提交";
            break;
        case 2:
            cell.status.text = @"成交";
            break;
        case 3:
            cell.status.text = @"部分成交";
            break;
        case 4:
            cell.status.text = @"主动撤单";
            break;
        case 5:
            cell.status.text = @"交易所拒绝";
            break;
        case 6:
            cell.status.text = @"已冻结";
            break;
        case 7:
            cell.status.text = @"其他";
            break;
        case 8:
            cell.status.text = @"已过期";
            break;
            
        default:
            break;
    }
    
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
                                                    if ([self isBeforeOpenMarket5Minite]) {
                                                        [self performSelector:@selector(showForbitAlert) withObject:nil afterDelay:0.05f];
                                                        return ;
                                                    }
                                                    [self alertViewClickConfirmButtonAtIndex:order.orderID];
                                                    [self showProgress:@"Loading..."];
                                                }
                                            }];
    
    [alertView show];
}

- (BOOL)isBeforeOpenMarket5Minite
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"HH:mm:ss"];
    NSString *str = [dateFormatter stringFromDate:[NSDate date]];
    NSArray *filedArr = [str componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@":"]];
    int hour = [filedArr[0] intValue];
    int minute = [filedArr[1] intValue];
    if (hour == 9) {
        if (minute >= 20 && minute <= 24) {
            return YES;
        }
    }
    return NO;
}

- (void)showForbitAlert
{
    showAlert(@"撮合交易时间内禁止撤单!");
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
    [self showProgress:@"Loading..."];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
}


- (void)trade_ui_today_entrust_rsp:(NSString *)data
{
    [self closeProgressSuccess:@"Success!"];
    
//    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
//    [formatter setDateFormat:@"HH:mm"];
//    NSString *now = [formatter stringFromDate:[NSDate date]];
    
//    if ([now compare:@"15:30"] == NSOrderedDescending) return; //大于15:30
    
    _orderArray = [NSMutableArray array];
    NSError *error = nil;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[data dataUsingEncoding:NSUTF8StringEncoding]
                                                         options:NSJSONReadingMutableContainers
                                                           error:&error];
    NSMutableArray *array = [str2dict objectForKey:@"ORDER"];
    NSMutableArray *extraArray = [NSMutableArray arrayWithArray:array];
    
    if ([str2dict allValues].count == 1) {
        if ([[[str2dict allValues] objectAtIndex:0] intValue] == -100001) {
            showAlert(LocalizedStringByInt(-100001));
            return;
        }
    }
    for (NSDictionary *orderDict in array) {
        OrderModel *orderModel = [OrderModel oderWithDict:orderDict];
        
        int exeState = [orderModel.exeState intValue];
        if (exeState != 1 && exeState != 3) [extraArray removeObject:orderDict];
    }
    _orderArray = [NSMutableArray arrayWithArray:extraArray];
    [_tableView reloadData];
    
}


- (void)trade_ui_canceltrade_rsp:(int)nSeq canceltrades:(NSString *)canceltrades
{
    MYLog(@"%@",canceltrades);
    [self request_today_order];
    /*  直接从回复结果删除，不重新请求
    NSError *error;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[canceltrades dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    NSString *value = str2dict[INT2STRING(FIX_TAG_BODY_EXECUTETYPE)];
    if ([value isEqualToString:@"0"]) {
     
    }
     */
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
