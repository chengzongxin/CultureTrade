//
//  MeController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "MeController.h"
#import "LoginController.h"
#import "FeedbackController.h"
#import "AboutController.h"
#import "GlobalModel.h"
#import "TWMessageBarManager.h"


@interface MeController ()
{
    NSTimer *mTimer;
    NSArray *_tableDataArray;
}
@end

@implementation MeController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addTitleView];
    
    [self addGuideView];
    
//    [self addButtonListView];
    
    [self addTableView];
    
    [self initalization];
}

- (void)initalization
{
    NSString *plistPath = [[NSBundle mainBundle] pathForResource:@"MeControllerData.plist" ofType:nil];
    _tableDataArray = [NSArray arrayWithContentsOfFile:plistPath];
}
     
- (void)addTitleView
{
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithTitle:@"Sure" icon:@"user_face" target:self action:@selector(clickUser)];
}

- (void)clickUser
{
    [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
}

- (void)addGuideView
{
    _guideView = [[MeGuideView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, SIZE_ME_GUIDE_HEIGHT)];
    [self.view addSubview:_guideView];
}

- (void)addButtonListView
{
    CGRect rect = CGRectMake(0,CGRectGetMaxY(_guideView.frame) + 10,
                             self.view.frame.size.width,
                             kQuotaButtonListHeight);
    _buttonListView = [[ButtonListView alloc] initWithFrame:rect];
    _buttonListView.delegate = self;
    [self.view addSubview:_buttonListView];
}

- (void)addTableView
{
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(_guideView.frame), ScreenSize.width, ScreenSize.height - 64*2 + 20 - CGRectGetMaxY(_guideView.frame)) style:UITableViewStyleGrouped];
    _tableView.rowHeight = 44;
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.alpha = 0.7;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;   // UNAVAILABLE IOS_7 EARLY
    _tableView.separatorInset = UIEdgeInsetsMake(0, 0, 0, 0);
    _tableView.separatorColor = [UIColor blackColor];
    _tableView.backgroundColor = [UIColor blackColor];
    [self.view addSubview:_tableView];
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    
    UIView* customView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 44.0)] ;
    
    customView.backgroundColor = [UIColor blackColor];
    
    UILabel * headerLabel = [[UILabel alloc] initWithFrame:CGRectZero] ;
    
    headerLabel.backgroundColor = [UIColor clearColor];
    
    headerLabel.opaque =  NO;
    
    headerLabel.textColor = [UIColor whiteColor];
    
    headerLabel.highlightedTextColor = [UIColor lightGrayColor];
    
    headerLabel.font = [UIFont boldSystemFontOfSize:15];
    
    headerLabel.frame = CGRectMake(0, 0, self.view.frame.size.width, 44.0);
    
    headerLabel.text = _tableDataArray[section][0];
    
    headerLabel.textAlignment = NSTextAlignmentCenter;
    
    [customView addSubview:headerLabel];
    
    _customView = customView;
    
    return customView;
    
}

//别忘了设置高度
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 44.0;
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    return 0.1;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return _tableDataArray[section][0];
}


// 取消选中
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    
    if (![self isLogin]) {
        [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
        return;
    }
    
    if (indexPath.section == 0) { // 第一组
        if (indexPath.row == 0) {
            [self.navigationController pushViewController:[[InOutMoneyController alloc] init] animated:YES];
        }else{
            [self.navigationController pushViewController:[[TransferQueryController alloc] init] animated:YES];
        }
    }else{  // 第二组
        if (indexPath.row == 0){
            [self.navigationController pushViewController:[[ModifyPasswordController alloc] init] animated:YES];
        }else if (indexPath.row == 1){
            self.feedbackKit = [[YWFeedbackKit alloc] initWithAppKey:BAICHUAN_APPKEY];
            
#warning 设置App自定义扩展反馈数据
            _feedbackKit.extInfo = @{@"loginTime":[[NSDate date] description],
                                     @"visitPath":@"登陆->关于->反馈",
                                     @"应用自定义扩展信息":@"开发者可以根据需要设置不同的自定义信息，方便在反馈系统中查看"};
            
#warning 自定义反馈页面配置
            /*
             * bgColor：消息气泡背景色，如"#58adf1"
             * color：消息内容文字颜色，如"#ffffff"
             * avatar：当前登录账号的头像，使用图片地址
             * toAvatar：客服账号的头像，使用图片地址
             */
            _feedbackKit.customUIPlist = @{@"bgColor":@"#00bfff"};
            
            [self _openFeedbackViewController];
            
//            [self.navigationController pushViewController:[[FeedbackController alloc] init] animated:YES];
        }else if (indexPath.row == 2){
            [self.navigationController pushViewController:[[AboutController alloc] init] animated:YES];
        }
    }
}

- (void)_openFeedbackViewController
{
    __weak typeof(self) weakSelf = self;
    
    [_feedbackKit makeFeedbackViewControllerWithCompletionBlock:^(YWFeedbackViewController *viewController, NSError *error) {
        if ( viewController != nil ) {
#warning 这里可以设置你需要显示的标题
            viewController.title = @"反馈界面";
            
            UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:viewController];
            [weakSelf presentViewController:nav animated:YES completion:nil];
            
            viewController.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"关闭" style:UIBarButtonItemStylePlain target:weakSelf action:@selector(actionQuitFeedback)];
            
            viewController.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"清除缓存" style:UIBarButtonItemStylePlain
                                                                                              target:weakSelf action:@selector(actionCleanMemory:)];
            
            __weak typeof(nav) weakNav = nav;
            
            [viewController setOpenURLBlock:^(NSString *aURLString, UIViewController *aParentController) {
                UIViewController *webVC = [[UIViewController alloc] initWithNibName:nil bundle:nil];
                UIWebView *webView = [[UIWebView alloc] initWithFrame:webVC.view.bounds];
                webView.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
                
                [webVC.view addSubview:webView];
                [weakNav pushViewController:webVC animated:YES];
                [webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:aURLString]]];
            }];
        } else {
            NSString *title = [error.userInfo objectForKey:@"msg"]?:@"接口调用失败，请保持网络通畅！";
            [[TWMessageBarManager sharedInstance] showMessageWithTitle:title description:nil
                                                                  type:TWMessageBarMessageTypeError];
        }
    }];
}

- (void)actionQuitFeedback
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)actionCleanMemory:(id)sender
{
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
}


- (BOOL)isLogin
{
    return [[NSTradeEngine setup] isLogin];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return _tableDataArray.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [_tableDataArray[section] count] - 1;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifer = @"cellIdentifiered";
    MeFunctionCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifer];
    if (cell == nil) {
        cell = [[MeFunctionCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifer];
    }
    
    NSDictionary *dict = _tableDataArray[indexPath.section][indexPath.row + 1];

    cell.icon = dict[@"icon"];
    [cell setTextString:dict[@"title"]];
    cell.cellType = (CellType)[dict[@"type"] integerValue];
    
    return cell;
}


- (void)buy
{
    [self.navigationController pushViewController:[[BuyController alloc] init] animated:YES ];
}

- (void)sell
{
    [self.navigationController pushViewController:[[SellController alloc] init] animated:YES ];
}

- (void)cancelOrder
{
    [self.navigationController pushViewController:[[EntrustCancleOrderController alloc] init] animated:YES ];
}

- (void)queryOrder
{
    [self.navigationController pushViewController:[[QueryController alloc] init] animated:YES];
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


- (void)trade_ui_moneyhold_rsp:(int)nRet
{
    if (nRet) {
        [self fillGuideView];
    }else{
        MYLog(@"error  nRet = %d",nRet);
    }
}

- (void)fillGuideView
{
    _guideView.foundLable.text = [NSString changePriceByFloat:GLOBAL.moneyHold.totalBalance];
    _guideView.monthDealLable.text = [NSString changePriceByFloat:GLOBAL.moneyHold.accountBalance];
    _guideView.sucRatioLabel.text = [NSString changePriceByFloat:GLOBAL.moneyHold.accountFreeze];
    _guideView.thisMonthGainLable.text = [NSString changePriceByFloat:GLOBAL.moneyHold.totalBalance];
    _guideView.thisWeekGainLabel.text = [NSString changePriceByFloat:GLOBAL.moneyHold.canOut];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if ([[NSTradeEngine setup] isLogin]){
        self.title = [[NSTradeEngine setup] getLoginedAccountName];
        [self fillGuideView];
    }else{
        self.title = @"个人中心";
        _guideView.foundLable.text = @"0.00";
        _guideView.monthDealLable.text = @"0.00";
        _guideView.sucRatioLabel.text = @"0.00";
        _guideView.totalGainLable.text = @"0.00";
        _guideView.thisMonthGainLable.text = @"0.00";
        _guideView.thisWeekGainLabel.text = @"0.00";
    }
    
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
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
    
}

- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len
{
    float allCost = 0.0f;
    for (ShareHold *shareHold in GLOBAL.shareHoldArray) {
        float newPrice = [[NSTradeEngine sharedInstance] getNewPrice:[shareHold.productID intValue]];
        allCost += (newPrice - [shareHold.keepCost floatValue])*[shareHold.totalBalance intValue];
    }
    _guideView.totalGainLable.text = [NSString changePriceByFloat:allCost];
    if (allCost > 0) {
        _guideView.totalGainLable.textColor = [UIColor redColor];
    }else if (allCost == 0){
        _guideView.totalGainLable.textColor = [UIColor whiteColor];
    }else{
        _guideView.totalGainLable.textColor = [UIColor greenColor];
    }
    
}

@end
