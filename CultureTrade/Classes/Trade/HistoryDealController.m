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
#import "SZCalendarPicker.h"
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
    
    [self addDatePickView];
    
    [self addBanner];
    
    [self addTableView];
    
    [self searchOneMonth];
}

- (void)addTitleView
{
    self.title = LocalizedStringByInt(2301);
}

- (void)addBanner
{
    _banner = [[EntrustCancelBanner alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, kBannerHeight)];
    UILabel *label = _banner.subviews[0];
    label.text = @"成交价";
    [self.view addSubview:_banner];
    _banner.frame = CGRectMake(0, CGRectGetMaxY(_dateLabel.frame) + 12 + 10, self.view.frame.size.width, kBannerHeight);
}

- (void)addTableView
{
    [super addTableView];
    CGRect frame = _tableView.frame;
    frame.origin.y = CGRectGetMaxY(_dateLabel.frame) + 12 + 10 + 10 + kBannerHeight;
    frame.size.height = self.view.frame.size.height - frame.origin.y - 64;;
    _tableView.frame = frame;
}


- (void)addDatePickView
{
    UIView *backView = [[UIView alloc] init];
    [self.view addSubview:backView];
    
    UIImage *searchImg = [UIImage imageNamed:@"apply_search"];
    CGFloat x = 20;
    CGFloat y = 15;
    CGFloat widthBtn = self.view.frame.size.width / 3.5;
    CGFloat widthLab = 30;
    CGFloat height = searchImg.size.height;
    
    _dateLabel = [[UILabel alloc] initWithFrame:CGRectMake(x, y, widthLab, height)];
    _dateLabel.text = @"日期:";
    _dateLabel.textColor = [UIColor whiteColor];
    [_dateLabel setFont:[UIFont systemFontOfSize:12]];
    [self.view addSubview:_dateLabel];
    
    _startBtn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    _startBtn.frame = CGRectMake(CGRectGetMaxX(_dateLabel.frame)+10, y, widthBtn, height);
    _startBtn.backgroundColor = [UIColor blackColor];
    _startBtn.layer.cornerRadius = 10.0f;
    [_startBtn addTarget:self action:@selector(clickStartBtn) forControlEvents:UIControlEventTouchUpInside];
    [_startBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.view addSubview:_startBtn];
    
    _toLabel = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetMaxX(_startBtn.frame)+10, y, widthLab, height)];
    _toLabel.text = @"到";
    _toLabel.textColor = [UIColor whiteColor];
    [_toLabel setFont:[UIFont systemFontOfSize:12]];
    [self.view addSubview:_toLabel];
    
    _endBtn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    _endBtn.frame = CGRectMake(CGRectGetMaxX(_toLabel.frame), y, widthBtn, height);
    _endBtn.backgroundColor = [UIColor blackColor];
    _endBtn.layer.cornerRadius = 10.0f;
    [_endBtn addTarget:self action:@selector(clickEndBtn) forControlEvents:UIControlEventTouchUpInside];
    [_endBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [self.view addSubview:_endBtn];
    
    _searchBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    _searchBtn.frame = CGRectMake(CGRectGetMaxX(_endBtn.frame)+20, y, searchImg.size.width, searchImg.size.height);
    [_searchBtn setImage:searchImg forState:UIControlStateNormal];
    [_searchBtn setImage:[UIImage imageNamed:@"apply_search_selected"] forState:UIControlStateHighlighted];
    [_searchBtn addTarget:self action:@selector(clickSearch) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_searchBtn];
    
    // set back view
    backView.frame = (CGRect){CGPointZero,{self.view.frame.size.width,CGRectGetMaxY(_dateLabel.frame)+10}};
    backView.backgroundColor = COLOR_APPLYPURCHASE_TOPTOOL_BG;
}

- (void)clickStartBtn
{
    SZCalendarPicker *calendarPicker = [SZCalendarPicker showOnView:self.view];
    calendarPicker.today = [NSDate date];
    calendarPicker.date = calendarPicker.today;
    calendarPicker.frame = CGRectMake(0, self.view.frame.size.height - 352, self.view.frame.size.width, 352);
    calendarPicker.calendarBlock = ^(NSInteger day, NSInteger month, NSInteger year){
        NSString *str = [NSString stringWithFormat:@"%li-%02li-%02li",(long)year,(long)month,(long)day];
        [_startBtn setTitle:str forState:UIControlStateNormal];
        
        MYLog(@"_startBtn.currentTitle  = %@",_startBtn.currentTitle );
    };
}

- (void)clickEndBtn
{
    SZCalendarPicker *calendarPicker = [SZCalendarPicker showOnView:self.view];
    calendarPicker.today = [NSDate date];
    calendarPicker.date = calendarPicker.today;
    calendarPicker.frame = CGRectMake(0, self.view.frame.size.height - 352, self.view.frame.size.width, 352);
    calendarPicker.calendarBlock = ^(NSInteger day, NSInteger month, NSInteger year){
        NSString *str = [NSString stringWithFormat:@"%li-%02li-%02li",(long)year,(long)month,(long)day];
        [_endBtn setTitle:str forState:UIControlStateNormal];
        MYLog(@"endbtn = %@",str);
    };
}

- (void)clickSearch
{
    if (!(_startBtn.currentTitle && _endBtn.currentTitle)) { // null
        showAlert(@"请输入完整的日期");
        return;
    }
    
    NSDateFormatter *dateformat = [[NSDateFormatter alloc] init];
    [dateformat setDateFormat:@"yyyy-MM-dd"];
    
    NSDate *startDate = [dateformat dateFromString:_startBtn.currentTitle];
    NSDate *endDate = [dateformat dateFromString:_endBtn.currentTitle];
    
    NSComparisonResult resulut = [startDate compare:endDate];
    if (resulut  == NSOrderedAscending) { // start < end
        MYLog(@"start < end ");
    }else if (resulut == NSOrderedSame){  // start == end
        MYLog(@"start = end ");
    }else if (resulut == NSOrderedDescending){  // start > end  ! illigle
        showAlert(@"开始日期应小于结束日期");
        return;
    }
    
    [self request_histurnoverStart:_startBtn.currentTitle end:_endBtn.currentTitle];
}

-(void)searchOneMonth
{
    [drawer hideAnimated:YES];
    NSDate* mydate = [NSDate date];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    
    NSDateComponents *adcomps = [[NSDateComponents alloc] init];
    [adcomps setYear:0];
    NSDate *newdate = [calendar dateByAddingComponents:adcomps toDate:mydate options:0];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-01"];// 00:00"];
    NSString *start = [formatter stringFromDate:newdate];
    [_startBtn setTitle:start forState:UIControlStateNormal];
    
    
    NSDateComponents *adcomps2 = [[NSDateComponents alloc] init];
    NSDate *newdate2 = [calendar dateByAddingComponents:adcomps2 toDate:mydate options:0];
    [formatter setDateFormat:@"yyyy-MM-dd"];// HH:mm"];
    NSString *end = [formatter stringFromDate:newdate2];;
    [_endBtn setTitle:end forState:UIControlStateNormal];
    [self request_histurnoverStart:start end:end];
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
//    [self scrollViewWillBeginDragging: nil];
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
