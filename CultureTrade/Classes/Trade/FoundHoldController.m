//
//  FoundHoldController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/24.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "FoundHoldController.h"
#import "UIBarButtonItem+helper.h"
#import "ShareHoldCell.h"

@interface FoundHoldController ()
{
    NSTimer *mTimer;
    UIView *_headView;
    UITableView *_tableView;
}
@end

@implementation FoundHoldController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = LocalizedStringByInt(2500);
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    
    [self addHeadView];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)addButtonListView{}


- (void)addHeadView
{
    UIView* customView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 44.0)] ;
    
    customView.backgroundColor = [UIColor blackColor];
    
    CGFloat width = customView.frame.size.width/4;
    CGFloat height = customView.frame.size.height;
    
    for (int i = 0; i< 4 ; i++) {
        UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(i*width, 0, width, height)];
        label.textColor = COLOR_GOLD_THEME;
        label.font = [UIFont boldSystemFontOfSize:12];
        label.textAlignment = NSTextAlignmentCenter;
        label.tag = i;
        [customView addSubview:label];
    }
    
    for (UILabel *label in customView.subviews) {
        switch (label.tag) {
            case 0:
                label.text = @"代码/名称";
                break;
            case 1:
                label.text = @"最新/成本";
                break;
            case 2:
                label.text = @"持仓/可用";
                break;
            case 3:
                label.text = @"市值/盈亏";
                break;
            default:
                break;
        }
    }
    _headView = customView;
}

- (void)addTableView
{
    CGRect tbFrame = CGRectMake(0, 0, ScreenSize.width, self.view.frame.size.height - 64 - CGRectGetMaxY(_guideView.frame));
    _tableView = [[UITableView alloc] initWithFrame:tbFrame style:UITableViewStylePlain];
    _tableView.rowHeight = 54;
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.alpha = 0.7;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;   // UNAVAILABLE IOS_7 EARLY
    _tableView.separatorInset = UIEdgeInsetsZero;
    _tableView.separatorColor = [UIColor blackColor];
    _tableView.backgroundColor = COLOR_CELL_BG;
    _tableView.bounces = NO;
    MYLog(@"_guideView.frame = %@",NSStringFromCGRect(_guideView.frame));
    
    CGRect frame = CGRectMake(0, CGRectGetMaxY(_guideView.frame), ScreenSize.width, self.view.frame.size.height - 64 - CGRectGetMaxY(_guideView.frame));
    UIScrollView *scrollView=[[UIScrollView alloc] initWithFrame:frame];
    [scrollView addSubview:_tableView];
    scrollView.bounces=NO;
    scrollView.contentSize=CGSizeMake(0,0);
    [self.view addSubview:scrollView];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return GLOBAL.shareHoldArray.count;
}

// 固定悬浮需要实现headView代理方法
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    return _headView;
}

//别忘了设置高度
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 44.0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifer = @"FoundHoldCell";
    ShareHoldCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifer];
    if (cell == nil) {
        cell = [[ShareHoldCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentifer];
    }
    ShareHold *shareHold = GLOBAL.shareHoldArray[indexPath.row];
    return [cell cellWithShareHold:shareHold];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
}


- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self startTimer];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self stopTimer];
}

- (void)startTimer // 30秒刷一次
{
    if (mTimer == nil) { // 30秒刷一次
        mTimer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:5.0f target:self selector:@selector(processEvent) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSDefaultRunLoopMode];
        
    }
}

- (void)stopTimer
{
    [mTimer invalidate];
    mTimer = nil;
    
}

- (void)processEvent
{
    NSThread* mThrad = [[NSThread alloc] initWithTarget:self selector:@selector(loadData) object:nil];
    [mThrad start];
}


- (void)loadData
{
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
    [[NSTradeEngine sharedInstance] requestPankouData];
    [[NSTradeEngine sharedInstance] trade_request_shareholdNS];
}

- (void)trade_ui_sharehold_rsp:(int)nRet sharehold:(NSString *)sharehold
{
    [_tableView reloadData];
}

- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len
{
    [super quote_ui_pankou_rsp:data len:len];
    [_tableView reloadData];
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
