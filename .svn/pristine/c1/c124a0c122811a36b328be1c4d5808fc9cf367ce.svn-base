//
//  QuotaAnalysisController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "QuotaAnalysisController.h"
#import "BaseNavigationController.h"
#import "ChartController.h"
#import "KlineController.h"
#import "PriceListController.h"
#import "DockItem.h"
#import "BuyController.h"
#import "SellController.h"
#import "EntrustCancleOrderController.h"
#import "QueryController.h"
#import "GlobalModel.h"
#import "tradedef.h"

@interface QuotaAnalysisController () <ButtonListViewDelegate,NSTradeEngineDelegate,UINavigationControllerDelegate>

@end

@implementation QuotaAnalysisController


- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.navigationBarHidden = NO;
    self.view.backgroundColor = [UIColor blackColor];
   
    [self addChildController];
    
    [self addDock];
    
//    [_dock addSpliteLine]; // 添加分割线
    
    [self addGuideView];
    
    [self addButtonListView];
    
    [self loadData];
}

- (void)addChildController
{
    ChartController *chart = [[ChartController alloc] init];
    BaseNavigationController *nav1 = [[BaseNavigationController alloc] initWithRootViewController:chart];
    nav1.delegate = self;
    chart.QuotaAnalysisController = self;
    [self addChildViewController:nav1];
    
    KlineController *kLine = [[KlineController alloc] init];
    BaseNavigationController *nav2 = [[BaseNavigationController alloc] initWithRootViewController:kLine];
    nav2.delegate = self;
    kLine.QuotaAnalysisController = self;
    [self addChildViewController:nav2];
    
//    PriceListController *priceList = [[PriceListController alloc] init];
//    BaseNavigationController *nav3 = [[BaseNavigationController alloc] initWithRootViewController:priceList];
//    nav3.delegate = self;
//    priceList.QuotaAnalysisController = self;
//    [self addChildViewController:nav3];
}

- (void)addDock
{
    [_dock addItemWithIcon:@"chart" title:@"图表"];
    [_dock addItemWithIcon:@"kline" title:@"K线"];
    _dock.backgroundColor = COLOR_QUOTA_ANALY_DOCK_BG;
//    [_dock addItemWithIcon:@"priceList" title:@"分价表"];
    _dock.alpha = 0.5;
    [_dock setDockItemColorNormal:[UIColor whiteColor] selected:COLOR_QUOTA_ANALY_DOCK_TIT];
    
    for (int i = 0 ; i < _dock.subviews.count ; i++) {
        DockItem *dockItem = _dock.subviews[i];
        [dockItem setSelectedColor:COLOR_QUOTA_ANALY_DOCK_SEL];
        
    }
    
    [_dock clickFirstItem]; //第一次进入界面高亮第一个控制器按钮
}

- (void)loadData
{
    [NSTradeEngine sharedInstance].delegate = self;
//    [[NSTradeEngine sharedInstance] quote_kline_reqNS:MINUTES5_HISK productID:GLOBAL.symbolModel.productID];
}

- (void)addGuideView
{
    _guideView = [[GuideView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, kQuotaGuideViewHeight)];
    [self.view addSubview:_guideView];
    
    _guideView.price.text = @"--";
    _guideView.increase.text = @"--";
    _guideView.increasePercent.text = @"--";
    _guideView.todayOpen.text = @"今开: --";
    _guideView.yesterdayClose.text = @"昨收: --";
    _guideView.highest.text = @"最高: --";
    _guideView.lowest.text = @"最低: --";
    _guideView.dealVolume.text = @"成交量: --";
    _guideView.dealTurnover.text = @"成交额: --";
    
    [_guideView fillColor];
}

- (void)addButtonListView
{
    CGRect rect = CGRectMake(0,
                             self.view.frame.size.height - NavigationBarHeight - kQuotaGuideViewHeight - kDockHeight - kQuotaButtonListHeight - 20,
                             self.view.frame.size.width,
                             kQuotaButtonListHeight);
    
    if (IS_IPHONE4||IS_IPHONE5) {
        rect.origin.y += 10;
    }
    
    _buttonListView = [[ButtonListView alloc] initWithFrame:rect];
    _buttonListView.delegate = self;
    [self.view addSubview:_buttonListView];
    
//    MYLog(@"add button list view frame  ===== %@",NSStringFromCGRect(rect));
//    MYLog(@"%@",NSStringFromCGRect(_dock.frame));
//    MYLog(@"%@",NSStringFromCGRect(self.view.frame));
}

#pragma mark -
#pragma mark 实现导航控制器代理方法
// 导航控制器即将显示新的控制器,,此时MainController的view是667，主控制器不在导航中，导航中的控制器高度为623
- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    UIViewController *root = navigationController.viewControllers[0];
    
    if (root != viewController) {   // 不是根控制器
        // applicationFrame                 {{0, 20}, {375, 647}}
        // navigationController.view.frame  {{0, 0}, {375, 667}}
        // root.view.frame                  {{0, 64}, {375, 603}}
        
        CGRect frame = navigationController.view.frame;
        frame.size.height = [UIScreen mainScreen].applicationFrame.size.height + 20;
        navigationController.view.frame = frame;
        
        // 添加dock到根控制器的view上面
        [_dock removeFromSuperview];
        CGRect dockFrame = _dock.frame;
        dockFrame.origin.y = root.view.frame.size.height - _dock.frame.size.height;
        _dock.frame = dockFrame;
        [root.view addSubview:_dock];
    }
    
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    UIViewController *root = navigationController.viewControllers[0];
    
    if (viewController == root) {
        // applicationFrame                 {{0, 20}, {375, 647}}
        // navigationController.view.frame  {{0, 0}, {375, 623}}
        // root.view.frame                  {{0, 64}, {375, 559}}
        //        CGRect frame = navigationController.view.frame;
        //        frame.size.height = [UIScreen mainScreen].applicationFrame.size.height + 20;// - _dock.frame.size.height;
        //        navigationController.view.frame = frame;
        [_dock removeFromSuperview];
        CGRect dockFrame = _dock.frame;
        dockFrame.origin.y = self.view.frame.size.height - _dock.frame.size.height; //root.view.frame.size.height + 20,// - _dock.frame.size.height;
        _dock.frame = dockFrame;
        [self.view addSubview:_dock];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)buy
{
    if ([self.delegate respondsToSelector:@selector(buy)]) {
        [self.delegate buy];
    }
}

- (void)sell
{
    if ([self.delegate respondsToSelector:@selector(sell)]) {
        [self.delegate sell];
    }
}

- (void)cancelOrder
{
    if ([self.delegate respondsToSelector:@selector(cancelOrder)]) {
        [self.delegate cancelOrder];
    }
}

- (void)queryOrder
{
    if ([self.delegate respondsToSelector:@selector(queryOrder)]) {
        [self.delegate queryOrder];
    }
}

@end
