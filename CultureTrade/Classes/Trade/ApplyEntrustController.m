//
//  ApplyEntrustController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/19.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyEntrustController.h"

@interface ApplyEntrustController ()
{
    NSMutableArray *_applyEntrustArray;
}
@end

@implementation ApplyEntrustController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"申购委托";
    
    self.view.backgroundColor = COLOR_APPLYPURCHASE_VC_BG;
    
    [self addDatePickView];
    
    [self addTableView];
    
    [self initailization];
    
    [self startRequest];
}

- (void)initailization
{
    NSString *endDateStr = [NSDate stringFromDate:[NSDate date]];
    NSString *startDateStr = [NSDate fewDaysAgo:7];
    [_startBtn setTitle:startDateStr forState:UIControlStateNormal];
    [_endBtn setTitle:endDateStr forState:UIControlStateNormal];
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
    
    [self startRequest];
}

- (void)addTableView
{
    CGRect leftRect = CGRectMake(0, 0, kLeftTableWith, self.view.frame.size.height - 64);
    UITableView *leftTable = [[UITableView alloc] initWithFrame:leftRect style:UITableViewStylePlain];
    leftTable.delegate = self;
    leftTable.dataSource = self;
    leftTable.bounces=NO;
    leftTable.rowHeight = KQuotaRowHeight;
    leftTable.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    leftTable.separatorColor = COLOR_CELL_SEPLITELINE;
    leftTable.backgroundColor = COLOR_QUOTA_TABLE_BG;
    [leftTable setShowsVerticalScrollIndicator:NO];
    [leftTable setShowsHorizontalScrollIndicator:NO];
    leftTable.tag = 0;
    _leftTable = leftTable;
    
    UIScrollView *leftScrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(_dateLabel.frame) + 12, kLeftTableWith, self.view.frame.size.height -64)];
    [leftScrollView addSubview:leftTable];
    leftScrollView.bounces=NO;
    leftScrollView.contentSize=CGSizeMake(kLeftTableWith,0);
    [self.view addSubview:leftScrollView];
    
    CGRect rect = CGRectMake(0, 0 ,self.view.frame.size.width*kMainTableViewWithRatio, self.view.frame.size.height -64);
    UITableView *mainTable = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    mainTable.delegate = self;
    mainTable.dataSource = self;
    mainTable.bounces=NO;
    mainTable.rowHeight = KQuotaRowHeight;
    mainTable.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    mainTable.separatorColor = COLOR_CELL_SEPLITELINE;
    mainTable.backgroundColor = COLOR_QUOTA_TABLE_BG;
    mainTable.separatorInset = UIEdgeInsetsMake(0, -2000, 0, -2000);
    mainTable.tag = 1;
    _mainTable = mainTable;
    
    UIScrollView *scrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(kLeftTableWith, CGRectGetMaxY(_dateLabel.frame) + 12, self.view.frame.size.width-kLeftTableWith, self.view.frame.size.height -64)];
    [scrollView addSubview:mainTable];
    scrollView.bounces=NO;
    scrollView.contentSize=CGSizeMake(self.view.frame.size.width*kMainTableViewWithRatio,0);
    [self.view addSubview:scrollView];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY= _mainTable.contentOffset.y;
    CGPoint leftOffsetY = _leftTable.contentOffset;
    leftOffsetY.y=offsetY;
    _leftTable.contentOffset=leftOffsetY;
    if(offsetY==0){
        _leftTable.contentOffset=CGPointZero;
    }
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    if (tableView == _leftTable) {
        return [[LeftApplyEntrustBanner alloc] initWithFrame:CGRectMake(0, 0, kLeftTableWith, kQuotaBannerHeight)];
    }else if (tableView == _mainTable){
        ApplyEntrustBanner *mainBanner = [[ApplyEntrustBanner alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width*kMainTableViewWithRatio, kQuotaBannerHeight)];
        [mainBanner setCount:9 textIndex:2801];
        return mainBanner;
    }else{
        return NULL;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ApplyPurchaseEntrust *applyEntrust = _applyEntrustArray[indexPath.row];
    if (tableView.tag == 0) {
        static NSString *cellID = @"LeftApplyPurchaseEntrustCell";
        LeftApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[LeftApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithEntrust:applyEntrust];
    }else{
        static NSString *cellID = @"ApplyPurchaseEntrustCell";
        ApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[ApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
            cell.labelCount = 9;
        }
        return [cell initWithEntrust:applyEntrust];
    }
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return kQuotaBannerHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _applyEntrustArray.count;
}

// 取消选中,选择cell
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_leftTable deselectRowAtIndexPath:indexPath animated:YES];
    [_mainTable deselectRowAtIndexPath:indexPath animated:YES];
}

- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath
{// 同时选中
    [_leftTable selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionNone];
    [_mainTable selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionNone];
    return YES;
}

-(void)startRequest {
    
    [self showProgress:@"Loading..."];
    
    NSString *accountName = [[NSTradeEngine sharedInstance] getLoginedAccountName];
    
    NSString *session =[[NSTradeEngine sharedInstance] getSession];
    
    NSString *strURL = [NSString stringWithFormat:ENTRUSTQUERY,accountName,session,_startBtn.currentTitle,_endBtn.currentTitle];
    
    MYLog(@"strURL = %@",strURL);
    
    NSURL *url = [NSURL URLWithString:strURL];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    NSURLConnection *connection = [[NSURLConnection alloc]
                                   initWithRequest:request
                                   delegate:self];
    if (connection) {
        _datas = [NSMutableData new];
    }
}
#pragma mark- NSURLConnection 回调方法
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [_datas appendData:data];
}
-(void) connection:(NSURLConnection *)connection didFailWithError: (NSError *)error {
    NSLog(@"%@",[error localizedDescription]);
}
- (void) connectionDidFinishLoading: (NSURLConnection*) connection {
    NSLog(@"请求完成…");
    NSDictionary* dict = [NSJSONSerialization JSONObjectWithData:_datas 
                                                         options:NSJSONReadingAllowFragments error:nil]; 
    MYLog(@"connectionDidFinishLoading = %@",dict);
    
    _applyEntrustArray = [NSMutableArray array];
    NSArray *entrusts = dict[@"data"];
    
    for (NSDictionary *subdict in entrusts) {
        ApplyPurchaseEntrust *entrust = [ApplyPurchaseEntrust applyPurchaseEntrustWithDictionary:subdict];
        [_applyEntrustArray addObject:entrust];
    }
    
    [_leftTable reloadData];
    [_mainTable reloadData];
    
    [self closeProgressSuccess:@"Success!"];
}



- (void)viewDidLayoutSubviews
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([_mainTable respondsToSelector:@selector(setSeparatorInset:)]) {
        [_mainTable setSeparatorInset:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_mainTable respondsToSelector:@selector(setLayoutMargins:)]) {
        [_mainTable setLayoutMargins:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_leftTable respondsToSelector:@selector(setSeparatorInset:)]) {
        [_leftTable setSeparatorInset:UIEdgeInsetsMake(0, _leftTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_leftTable respondsToSelector:@selector(setLayoutMargins:)]) {
        [_leftTable setLayoutMargins:UIEdgeInsetsMake(0, _leftTable.frame.size.width*-2, 0, 0)];
    }
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([cell respondsToSelector:@selector(setSeparatorInset:)]) {
        [cell setSeparatorInset:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-2, 0, 0)];
    }
    
    if ([cell respondsToSelector:@selector(setLayoutMargins:)]) {
        [cell setLayoutMargins:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-2, 0, 0)];
    }
}




@end
