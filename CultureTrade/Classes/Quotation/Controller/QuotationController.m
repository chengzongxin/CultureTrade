//
//  QuotationController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "QuotationController.h"
#import "UIBarButtonItem+helper.h"
#import "QuotationCell.h"
#import "QuotaAnalysisController.h"
#import "QuotaionBanner.h"
#import "NSTradeEngine.h"
#import "GlobalModel.h"
#import "SelectPrefer.h"
#import "SelectController.h"
#import "TitleViewButton.h"
#import "LeftCell.h"
#import "SearchProductController.h"
#define kLeftTableWith 70
#define kMainTableViewWithRatio 4

@interface QuotationController () <UITableViewDataSource,UITableViewDelegate,NSTradeEngineDelegate,NSNetHelperDelegate>
{
    NSMutableArray *_symbolArray;
    NSMutableArray *_sortUnitArray;
//    NSThread* mThrad;
    NSTimer* mTimer;
    BOOL _isSelect;
}
@end

@implementation QuotationController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addTitleView];
    
    [self addBanner];
    
    [self addTableView];
    
    [self clickQuota];
    
    [self showProgress:@"Loading..."];
}


- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NSTradeEngine sharedInstance].delegate = self;
    [[NSTradeEngine sharedInstance] requestPankouData];
    
    if (GLOBAL.sortUnitArray != NULL) {
        if (_isSelect) {
            _sortUnitArray = [self filterPerferIn:GLOBAL.sortUnitArray];
        }else{
            _sortUnitArray = GLOBAL.sortUnitArray;
        }
        [_mainTableView reloadData];
        [_leftTableView reloadData];
    }
    [self startTimer];
}


- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [NSTradeEngine sharedInstance].delegate = NULL;// 由于
    [self stopTimer];
}

- (void)startTimer
{
    if (mTimer == nil) { // 30秒刷一次
        mTimer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:2.0f target:self selector:@selector(refreshPanKouData) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSDefaultRunLoopMode];
        
    }
}

- (void)stopTimer
{
    [mTimer invalidate];
    mTimer = nil;
}


- (void)refreshPanKouData
{
    [[NSTradeEngine sharedInstance] requestPankouData];
}

- (void)addTitleView
{
    // 设置行情中心标题
    UIImage *titleImage = [UIImage imageNamed:@"left"];
    CGFloat width = titleImage.size.width;
    CGFloat height = titleImage.size.height;
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,{width*2,height}}];
//    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    self.navigationItem.titleView = titleView;
    
    TitleViewButton *quotation = [TitleViewButton buttonWithType:UIButtonTypeCustom];
    quotation.frame = {CGPointZero,{width,height}};
    [quotation setTitle:@"行情" icon:@"left" selected:@"left_selected"];
    [quotation addTarget:self action:@selector(clickQuota) forControlEvents:UIControlEventTouchUpInside];
    [titleView addSubview:quotation];
    
    TitleViewButton *select = [TitleViewButton buttonWithType:UIButtonTypeCustom];
    select.frame = {{width,0},{width,height}};
    [select setTitle:@"自选" icon:@"right" selected:@"right_selected"];
    [select addTarget:self action:@selector(clickSelect) forControlEvents:UIControlEventTouchUpInside];
    [titleView addSubview:select];
    
    self.navigationItem.rightBarButtonItem = [UIBarButtonItem itemWithIcon:@"search_quota" highlightedIcon:nil target:self action:@selector(clickSearch)];
}

- (void)clickSearch
{
    SearchProductController *searchController = [[SearchProductController alloc] init];
    [self presentViewController:searchController animated:YES completion:nil];
//    [self.navigationController pushViewController:searchController animated:YES];
}

- (void)clickQuota
{
    _isSelect = NO;
    _sortUnitArray = GLOBAL.sortUnitArray;
    [_mainTableView reloadData];
    [_leftTableView reloadData];
    [self switchStatus];
}

- (void)clickSelect
{
    _isSelect = YES;
    if (GLOBAL.sortUnitArray != NULL) {
        _sortUnitArray = [self filterPerferIn:GLOBAL.sortUnitArray];
        [_mainTableView reloadData];
        [_leftTableView reloadData];
    }
    [self switchStatus];
}

- (void)switchStatus
{
    NSArray *btnArray = self.navigationItem.titleView.subviews;
    TitleViewButton *quotaBtn = btnArray[0];
    TitleViewButton *selectBtn = btnArray[1];
    if (_isSelect) { // 自选
        [quotaBtn setBackgroundImage:[UIImage imageNamed:@"left"] forState:UIControlStateNormal];
        [quotaBtn setTitleColor:COLOR_QUOTA_BANNAR_TEXT forState:UIControlStateNormal];
        quotaBtn.titleLabel.font = [UIFont systemFontOfSize:12];
        [selectBtn setBackgroundImage:[UIImage imageNamed:@"right_selected"] forState:UIControlStateNormal];
        [selectBtn setTitleColor:COLOR_GOLD_THEME forState:UIControlStateNormal];
        selectBtn.titleLabel.font = [UIFont systemFontOfSize:13];
        
    }else{      // 行情
        [quotaBtn setBackgroundImage:[UIImage imageNamed:@"left_selected"] forState:UIControlStateNormal];
        [quotaBtn setTitleColor:COLOR_GOLD_THEME forState:UIControlStateNormal];
        quotaBtn.titleLabel.font = [UIFont systemFontOfSize:13];
        [selectBtn setBackgroundImage:[UIImage imageNamed:@"right"] forState:UIControlStateNormal];
        [selectBtn setTitleColor:COLOR_QUOTA_BANNAR_TEXT forState:UIControlStateNormal];
        selectBtn.titleLabel.font = [UIFont systemFontOfSize:12];
    }
}

- (NSMutableArray *)filterPerferIn:(NSMutableArray *)sortUnitArray
{
    NSMutableArray *array = [NSMutableArray array];
    for (SortUnit *s in sortUnitArray) {
        NSDictionary *localDict = [SelectPrefer readFromFile];
        if (localDict == nil) { // 没有自选
            return NULL;
        }else{                  // 有自选
            NSString *key = [NSString stringWithFormat:@"%d",s.m_CodeInfo.m_uiCode];
            if ([[localDict objectForKey:key] boolValue]) // value = 1
                [array addObject:s];
        }
    }
    return array;
}

- (void)addBanner
{
    _leftBanner = [[LeftBanner alloc] initWithFrame:CGRectMake(0, 0, kLeftTableWith, kQuotaBannerHeight)];
    _banner = [[QuotaionBanner alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width*kMainTableViewWithRatio, kQuotaBannerHeight)];
}


- (void)addTableView
{
     CGRect leftRect = CGRectMake(0, 0, kLeftTableWith, self.view.frame.size.height - kDockHeight - 64);
    _leftTableView = [[UITableView alloc] initWithFrame:leftRect style:UITableViewStylePlain];
    _leftTableView.delegate = self;
    _leftTableView.dataSource = self;
    _leftTableView.bounces=NO;
    _leftTableView.rowHeight = KQuotaRowHeight;
    _leftTableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _leftTableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _leftTableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    [_leftTableView setShowsVerticalScrollIndicator:NO];
    [_leftTableView setShowsHorizontalScrollIndicator:NO];
    _leftTableView.tag = 0;
    
    UIScrollView *leftScrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, kLeftTableWith, self.view.frame.size.height - kDockHeight - 64)];
    [leftScrollView addSubview:_leftTableView];
    leftScrollView.bounces=NO;
    leftScrollView.contentSize=CGSizeMake(kLeftTableWith,0);
    [self.view addSubview:leftScrollView];
    
    CGRect rect = CGRectMake(0, 0 ,self.view.frame.size.width*kMainTableViewWithRatio, self.view.frame.size.height - kDockHeight - 64);
    _mainTableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _mainTableView.delegate = self;
    _mainTableView.dataSource = self;
    _mainTableView.bounces=NO;
    _mainTableView.rowHeight = KQuotaRowHeight;
    _mainTableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _mainTableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _mainTableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    _mainTableView.separatorInset = UIEdgeInsetsMake(0, -2000, 0, -2000);
    _mainTableView.tag = 1;
    
    UIScrollView *scrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(kLeftTableWith, 0, self.view.frame.size.width-kLeftTableWith, self.view.frame.size.height - kDockHeight - 64)];
    [scrollView addSubview:_mainTableView];
    scrollView.bounces=NO;
    scrollView.contentSize=CGSizeMake(self.view.frame.size.width*kMainTableViewWithRatio,0);
    [self.view addSubview:scrollView];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY= _mainTableView.contentOffset.y;
    CGPoint leftOffsetY = _leftTableView.contentOffset;
    leftOffsetY.y=offsetY;
    _leftTableView.contentOffset=leftOffsetY;
    if(offsetY==0){
        _leftTableView.contentOffset=CGPointZero;
    }
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    if (tableView == _leftTableView) {
        return _leftBanner;
    }else{
        return _banner;
    }
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return _banner.frame.size.height;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _sortUnitArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (tableView.tag == 0) {
        static NSString *cellID = @"LeftCell";
        LeftCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[LeftCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithSortUnit:(SortUnit *)[_sortUnitArray objectAtIndex:indexPath.row]];
    }else{
        static NSString *cellID = @"QuotationCell";
        QuotationCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[QuotationCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithSortUnit:(SortUnit *)[_sortUnitArray objectAtIndex:indexPath.row]];
    }
}

// 取消选中,选择cell
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    MYLOGFUN;
    GLOBAL.sortUnit = _sortUnitArray[indexPath.row];
    for (StockInfoNS *stockInfo in GLOBAL.stockInfoArray) { // 获取缓存initmarket中对应的stockinfo对象
        if (stockInfo.m_uiCode == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
            GLOBAL.stockInfo = stockInfo;
            GLOBAL.sortUnit.m_uiPreClose = stockInfo.m_uiPrevClose;
        }
    }
    for (SymbolModel *symbol in GLOBAL.symbolArray) {
        if ([symbol.productID intValue] == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
            GLOBAL.symbolModel = symbol;
        }
    }
    // 同时取消选中
    [self.leftTableView deselectRowAtIndexPath:indexPath animated:YES];
    [self.mainTableView deselectRowAtIndexPath:indexPath animated:YES];
    
    [self presentViewController:[[QuotaAnalysisController alloc] init] animated:NO completion:nil];
}


- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath
{// 同时选中
    [self.leftTableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionNone];
    [self.mainTableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionNone];
    return YES;
}

- (void)viewDidLayoutSubviews
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([self.mainTableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [self.mainTableView setSeparatorInset:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-3, 0, 0)];
    }
    
    if ([self.mainTableView respondsToSelector:@selector(setLayoutMargins:)]) {
        [self.mainTableView setLayoutMargins:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-3, 0, 0)];
    }
    
    if ([self.leftTableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [self.leftTableView setSeparatorInset:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-3, 0, 0)];
    }
    
    if ([self.leftTableView respondsToSelector:@selector(setLayoutMargins:)]) {
        [self.leftTableView setLayoutMargins:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-2, 0, 0)];
    }
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([cell respondsToSelector:@selector(setSeparatorInset:)]) {
        [cell setSeparatorInset:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-2, 0, 0)];
    }
    
    if ([cell respondsToSelector:@selector(setLayoutMargins:)]) {
        [cell setLayoutMargins:UIEdgeInsetsMake(0, self.mainTableView.frame.size.width*-2, 0, 0)];
    }
}


- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len
{
    [_sortUnitArray removeAllObjects];
    if (_isSelect) {
        if (GLOBAL.sortUnitArray != NULL) _sortUnitArray = [self filterPerferIn:GLOBAL.sortUnitArray];
    }else{
        _sortUnitArray = GLOBAL.sortUnitArray;
    }
    [_mainTableView reloadData];
    [_leftTableView reloadData];
    [self closeProgressSuccess:@"Success!"];
}

-(void) on_net_status_rsp:(int)type nFlag:(int)nFlag{
    NSLog(@"连接状态= type=%d, nFlag=%d", type, nFlag); // type 1 = trade, 2 = quote
    dispatch_async(dispatch_get_main_queue(), ^{
        if (nFlag!=2) {
            [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
        }else{
            [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
        }
    });
}


@end
