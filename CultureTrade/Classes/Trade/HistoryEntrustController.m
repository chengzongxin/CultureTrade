//
//  HistoryEntrustController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/9.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "HistoryEntrustController.h"
#import "JZNavigationBarDrawer.h"
#import "UIButton+helper.h"
#import "UIBarButtonItem+helper.h"
#import "DataModel.h"
#import "JZSelectDateViewController.h"
#import "GlobalModel.h"

@interface HistoryEntrustController () <NSTradeEngineDelegate>
{
    UIBarButtonItem * btn;
    JZNavigationBarDrawer *drawer;
    int nSeq;
    NSMutableArray *_hisOrderArray;
    //    BOOL is_close_order;
}
//@property (strong, nonatomic) REMenu *menu;

@property(strong, nonatomic) NSArray * aryHisTickets;

-(IBAction)showFilter:(id)sender;

@end

@implementation HistoryEntrustController

- (void)backBtnPressed:(id)sender{
    
}

-(IBAction)showFilter:(id)sender{
    [drawer showNavigationBar:self animated:YES];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationItem.rightBarButtonItem = [UIBarButtonItem itemWithIcon:@"search" highlightedIcon:nil target:self action:@selector(showFilter:)];
    
    [self setExtraCellLineHidden: _tableView];
    
    // Init a drawer with default size
    drawer = [[JZNavigationBarDrawer alloc] initWithFrame:CGRectMake(0, 0, App_Frame_Width, 80)];
    
    // Assign the table view as the affected scroll view of the drawer.
    // This will make sure the scroll view is properly scrolled and updated
    // when the drawer is shown.
    drawer.scrollView = _tableView;
    // Add some buttons to the drawer.
    
    UIButton* btToMonth = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 80, 60)];
    [btToMonth setImage:[UIImage imageNamed:@"view_detail"] withTitle:@"当月" forState:UIControlStateNormal];
    [btToMonth addTarget:self action:@selector(toMonth) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton* btThree = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 80, 60)];
    [btThree setImage:[UIImage imageNamed:@"view_detail"] withTitle:@"三月" forState:UIControlStateNormal];
    [btThree addTarget:self action:@selector(threeMonth) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton* btAll = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 80, 60)];
    [btAll setImage:[UIImage imageNamed:@"view_detail"] withTitle:@"全部" forState:UIControlStateNormal];
    [btAll addTarget:self action:@selector(all_report) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton* btCustom = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 80, 60)];
    [btCustom setImage:[UIImage imageNamed:@"view_detail"] withTitle:@"按时间段" forState:UIControlStateNormal];
    [btCustom addTarget:self action:@selector(customDate) forControlEvents:UIControlEventTouchUpInside];
    
    //    UIBarButtonItem *button0 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    //	UIBarButtonItem *button1 = [[UIBarButtonItem alloc] initWithCustomView:btClose];
    UIBarButtonItem *button2 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    UIBarButtonItem *button3 = [[UIBarButtonItem alloc] initWithCustomView:btToMonth];
    UIBarButtonItem *button4 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    UIBarButtonItem *button5 = [[UIBarButtonItem alloc] initWithCustomView:btThree];
    UIBarButtonItem *button6 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    UIBarButtonItem *button7 = [[UIBarButtonItem alloc] initWithCustomView:btAll];
    UIBarButtonItem *button8 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    UIBarButtonItem *button9 = [[UIBarButtonItem alloc] initWithCustomView:btCustom];
    UIBarButtonItem *button10 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:self action:0];
    
    drawer.items = @[button2, button3, button4, button5, button6, button7, button8, button9, button10];
    
    [self toMonth];
}

- (void)addTitleView
{
    self.title = LocalizedStringByInt(2201);
}

-(void) toMonth
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    //NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateComponents *adcomps = [[NSDateComponents alloc] init];
    [adcomps setYear:0];
    NSDate *newdate = [calendar dateByAddingComponents:adcomps toDate:mydate options:0];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-01 00:00"];
    NSString *start = [formatter stringFromDate:newdate];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSString *end = [formatter stringFromDate:newdate2];;
    [self request_his_order:start end:end];
}

-(void) threeMonth
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateComponents *adcomps = [[NSDateComponents alloc] init];
    [adcomps setYear:0];
    [adcomps setMonth:-3];
    [adcomps setDay:0];
    NSDate *newdate = [calendar dateByAddingComponents:adcomps toDate:mydate options:0];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd 00:00"];
    NSString *start = [formatter stringFromDate:newdate];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    NSDateFormatter* formatter2 = [[NSDateFormatter alloc] init];
    [formatter2 setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSString *end = [formatter2 stringFromDate:newdate2];
    [self request_his_order:start end:end];
}
-(void) customDate
{
    [drawer hideAnimated:YES];
    JZSelectDateViewController* controller = [[JZSelectDateViewController alloc] init];
    controller.requestType = isHistoryOrder;
    controller.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController:controller animated:YES] ;
//    CGRect frame = _banner.frame;
//    frame.origin.y += 300;
//    _banner.frame = frame;
//    
//    UIDatePicker *startDate = [[UIDatePicker alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width,100)];
//    startDate.datePickerMode = UIDatePickerModeDate;
//    NSLocale *loc = [[NSLocale alloc] initWithLocaleIdentifier:@"zh_CN"];//chinese
//    startDate.locale = loc;
//    startDate.maximumDate = [NSDate date];
//    [startDate addTarget:self action:@selector(selectStartDate:) forControlEvents:UIControlEventValueChanged];
//    [self.view addSubview:startDate];
//    
//    UIDatePicker *endDate = [[UIDatePicker alloc] initWithFrame:CGRectMake(0, 120, self.view.frame.size.width,100)];
//    endDate.datePickerMode = UIDatePickerModeDate;
//    endDate.locale = loc;
//    endDate.maximumDate = [NSDate date];
//    [endDate addTarget:self action:@selector(selectEndDate:) forControlEvents:UIControlEventValueChanged];
//    [self.view addSubview:endDate];
}

- (void)selectStartDate:(UIDatePicker *)startPicker
{
    MYLog(@"%@",startPicker.date);
}

- (void)selectEndDate:(UIDatePicker *)endPicker
{
    MYLog(@"%@",endPicker.date);
}

-(void) all_report
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    NSString *end = [formatter stringFromDate:newdate2];;
    
    [self request_his_order:@"2010-01-01 00:00" end:end];
}

-(void) request_his_order:(NSString*)start end:(NSString*)end
{
    [self showProgress:nil];
//    nSeq = [[NSTradeEngine setup] trade_request_hisorder:start enddate:end];
    nSeq = [[NSTradeEngine setup] trade_request_historyorder_fromDate:start toDate:end];
}

- (void)scrollViewWillBeginDragging:(UIScrollView*)scrollView
{
    if(drawer && drawer.visible) [drawer hideAnimated:YES];
}

-(void)query:(NSString*) sdate eDate:(NSString*)eDate
{
    [[NSTradeEngine setup] trade_request_historyorder_fromDate:sdate toDate:eDate];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NSTradeEngine setup].delegate = self;
}

- (void)request_today_order{} // 不加载今日委托

-(void) viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear: animated];
    [self scrollViewWillBeginDragging:nil];
}
-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear: animated];
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

-(void)setExtraCellLineHidden: (UITableView *)tableView
{
    UIView *view = [UIView new];
    view.backgroundColor = [UIColor clearColor];
    [tableView setTableFooterView:view];
}

- (void)trade_ui_historyorder_rsp:(int)nSeq orders:(NSString *)orders
{
    [self closeProgressSuccess:@"Success!"];
    _hisOrderArray = [NSMutableArray array];
    NSError *error;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[orders dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    NSArray *jsonArray = [str2dict objectForKey:@"ORDER"];
    
    for (NSDictionary *dict in jsonArray) {
        Order *order = [Order orderWithDictionary:dict];
        [_hisOrderArray addObject:order];
    }
    [_tableView reloadData];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _hisOrderArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"orderID";
    EntrustCancelCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[EntrustCancelCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    
    Order *order = _hisOrderArray[indexPath.row];
    
    
    cell.stockID.text = order.productID;
    if ([order.orderType isEqualToString:@"0B"]) {
        cell.buySell.text = @"买";
    }else if ([order.orderType isEqualToString:@"0S"]) {
        cell.buySell.text = @"卖";
    }
    cell.stockName.text = order.productName;
    cell.entrustPrice.text = [NSString stringWithFormat:@"%0.2f",[order.orderPrice floatValue]];
    cell.entrustDeal.text = order.applyCount;
    // '1 :已提交 \r\n2 成交 \r\n3 部分成交 \r\n4主动撤单  \r\n5交易所拒绝 \r\n6已冻结 \r\n7 其他 \r\n 8 已过期',
    switch ([order.orderState intValue]) {
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
    
    for (SortUnit *sortUnit in GLOBAL.sortUnitArray) {
        if ([order.productID intValue] == sortUnit.m_CodeInfo.m_uiCode) {
            cell.nowPrice.text = [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiNewPrice];
            if (sortUnit.m_uiNewPrice == 0.0) {
                cell.nowPrice.text = @"--";
            }
        }
    }
    
//    cell.nowPrice.text = order.orderTime;
//    cell.nowPrice.font = [UIFont fontWithName:@"Helvetica-Bold" size:10];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
