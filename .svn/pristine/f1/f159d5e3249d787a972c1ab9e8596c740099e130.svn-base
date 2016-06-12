//
//  HistoryDealController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/9.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "HistoryDealController.h"
#import "JZNavigationBarDrawer.h"
#import "UIButton+helper.h"
#import "UIBarButtonItem+helper.h"
#import "DataModel.h"
#import "EntrustCancelCell.h"
#import "JZSelectDateViewController.h"
#import "GlobalModel.h"
#define kBannerHeight 20
#define kCellHeight 35

@interface HistoryDealController () <NSTradeEngineDelegate,UITableViewDataSource,UITableViewDelegate>
{
    UIBarButtonItem * btn;
    JZNavigationBarDrawer *drawer;
    int nSeq;
    NSMutableArray *_hisTurnOverArray;
    //    BOOL is_close_order;
}
//@property (strong, nonatomic) REMenu *menu;

@property(strong, nonatomic) NSArray * aryHisTickets;

-(IBAction)showFilter:(id)sender;

@end

@implementation HistoryDealController

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
    
    
    drawer.scrollView = _tableView;
    
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
}

- (void)addTitleView
{
    self.title = LocalizedStringByInt(2301);
}

-(void) toMonth
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateComponents *adcomps = [[NSDateComponents alloc] init];
    [adcomps setYear:0];
    NSDate *newdate = [calendar dateByAddingComponents:adcomps toDate:mydate options:0];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-01"];
    NSString *start = [formatter stringFromDate:newdate];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSString *end = [formatter stringFromDate:newdate2];;
    [self request_histurnoverStart:start end:end];
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
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSString *start = [formatter stringFromDate:newdate];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    NSString *end = [formatter stringFromDate:newdate2];
    [self request_histurnoverStart:start end:end];
}
-(void) customDate
{
    [drawer hideAnimated:YES];
    JZSelectDateViewController* controller = [[JZSelectDateViewController alloc] init];
    controller.requestType = isHistoryTurnOver;
    controller.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController:controller animated:YES] ;
}
-(void) all_report
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    NSString *end = [formatter stringFromDate:newdate2];;
    
    [self request_histurnoverStart:@"2010-01-01" end:end];
}

-(void) request_histurnoverStart:(NSString*)start end:(NSString*)end
{
    [self showProgress:nil];
    nSeq = [[NSTradeEngine setup] trade_request_historyturnover_fromDate:start toDate:end];
}

- (void)scrollViewWillBeginDragging:(UIScrollView*)scrollView
{
    if(drawer && drawer.visible) [drawer hideAnimated:YES];
}


-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NSTradeEngine setup].delegate = self;
    
}

- (void)request_today_turnover{} // 不加载今日成交


-(void) viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear: animated];
    [self scrollViewWillBeginDragging: nil];
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

- (void)trade_ui_historyturnover_rsp:(int)nSeq deals:(NSString *)deals
{
    [self dissmissProgress];
    _hisTurnOverArray = [NSMutableArray array];
    NSError *error;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[deals dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    
    if ([str2dict allValues].count == 1) {
        if ([[[str2dict allValues] objectAtIndex:0] intValue] == -100001) {
            showAlert(LocalizedStringByInt(-100001));
            return;
        }
    }
    
    NSArray *jsonArray = [str2dict objectForKey:@"TURNOVER"];
    for (NSDictionary *dict in jsonArray) {
        TurnOver *turnOver = [TurnOver turnOverWithDictionary:dict];
        [_hisTurnOverArray addObject:turnOver];
    }
    [_tableView reloadData];
    
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _hisTurnOverArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"turnOverID";
    EntrustCancelCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[EntrustCancelCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    
    TurnOver *turnOver = _hisTurnOverArray[indexPath.row];
    
    if ([turnOver.orderType isEqualToString:@"0B"]) {
        cell.buySell.text = @"买";
    }else if ([turnOver.orderType isEqualToString:@"0S"]) {
        cell.buySell.text = @"卖";
    }
    cell.stockID.text = turnOver.productID;
    cell.stockName.text = turnOver.productName;
    cell.entrustPrice.text = [NSString stringWithFormat:@"%0.2f",[turnOver.turnOverPrice floatValue]];
    cell.entrustDeal.text = turnOver.turnOverVol;
    cell.status.text = @"成交";//turnOver.user;
//    cell.nowPrice.text = turnOver.timeForce;
    for (SortUnit *sortUnit in GLOBAL.sortUnitArray) {
        if ([turnOver.productID intValue] == sortUnit.m_CodeInfo.m_uiCode) {
            cell.nowPrice.text = [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiNewPrice];
            if (sortUnit.m_uiNewPrice == 0.0) {
                cell.nowPrice.text = @"--";
            }
        }
    }
    
    return cell;
}

@end
