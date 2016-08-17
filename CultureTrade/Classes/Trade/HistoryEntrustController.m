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
#define kBannerHeight 20
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
    
    [self addDatePickView];
    
    [self addBanner];
    
    [self addTableView];
    
    [self searchOneMonth];
}

- (void)addBanner
{
    [super addBanner];
    _banner.frame = CGRectMake(0, CGRectGetMaxY(_dateLabel.frame) + 12 + 10, self.view.frame.size.width, kBannerHeight);
}

- (void)addTableView
{
    [super addTableView];
    CGRect frame = _tableView.frame;
    frame.origin.y = CGRectGetMaxY(_dateLabel.frame) + 12 + 10 + 10 + kBannerHeight;
    frame.size.height = self.view.frame.size.height - frame.origin.y - 64;
    _tableView.frame = frame;
}

- (void)addTitleView
{
    self.title = LocalizedStringByInt(2201);
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
    
    [self request_his_order:_startBtn.currentTitle end:_endBtn.currentTitle];
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
    [self request_his_order:start end:end];
}

-(void) request_his_order:(NSString*)start end:(NSString*)end
{
    [self showProgress:nil];
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


-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NSTradeEngine setup].delegate = self;
}

- (void)request_today_order{} // 不加载今日委托

-(void) viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear: animated];
//    [self scrollViewWillBeginDragging:nil];
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
    if ([str2dict allValues].count == 1) {
        if ([[[str2dict allValues] objectAtIndex:0] intValue] == -100001) {
            showAlert(LocalizedStringByInt(-100001));
            return;
        }
    }
    
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
    cell.entrustDeal.text = [NSString stringWithFormat:@"%@/%@",order.applyCount,order.turnOverVol];
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
