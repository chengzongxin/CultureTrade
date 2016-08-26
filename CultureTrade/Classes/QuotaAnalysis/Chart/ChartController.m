//
//  ChartController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "ChartController.h"
#import "UIBarButtonItem+helper.h"
#import "GuideView.h"
#import "LineView.h"
#import "UIColor+helper.h"
#import "Stock.h"
#import "GlobalModel.h"
#import "BuyController.h"
#import "SellController.h"
#import "EntrustCancleOrderController.h"
#import "QueryController.h"
#import "TimeLineView.h"
#import "LoginController.h"
#import "SearchProductController.h"
#import "FiveQuotationView.h"

#define kButtonHeight 20

@interface ChartController ()  <QuotaAnalysisControllerDelegate,TimeLineViewDelegate>
{
    TimeLineView *_timeLineView;
    FiveQuotationView *_fiveQuotationView;
}
@end

@implementation ChartController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addTitleView];
    
    [self addLineView];
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(click)];
}

- (void)addTitleView
{
    UIImage *titleImage = [UIImage imageNamed:@"quotaAnalyze"];
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    
    UIView *view = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,{titleImage.size.width,titleImage.size.height*2}}];
    view.backgroundColor = [UIColor clearColor];
    
    UILabel *name = [[UILabel alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    name.text = [NSString stringWithFormat:@"%@",GLOBAL.sortUnit.productName];
    name.backgroundColor = [UIColor clearColor];
    name.textColor = COLOR_GOLD_THEME;
    name.font = [UIFont systemFontOfSize:12];
    name.textAlignment = NSTextAlignmentCenter;
    [view addSubview:name];
    
    UILabel *code = [[UILabel alloc] initWithFrame:(CGRect){{0,titleImage.size.height},titleImage.size}];
    code.text = [NSString stringWithFormat:@"%@",GLOBAL.sortUnit.productID];
    code.backgroundColor = [UIColor clearColor];
    code.textColor = COLOR_GOLD_THEME;
    code.font = [UIFont systemFontOfSize:12];
    code.textAlignment = NSTextAlignmentCenter;
    [view addSubview:code];
    
    self.navigationItem.titleView = view;
}


- (void)addLineView
{
    _timeLineView = [[TimeLineView alloc] initWithFrame:CGRectMake(0,
                                                                   kQuotaGuideViewHeight+10,
                                                                   self.view.frame.size.width*0.7,
                                                                   self.view.frame.size.height - kQuotaGuideViewHeight - kQuotaBannerHeight  - kQuotaButtonListHeight- kDockHeight - 40)];
    _timeLineView.delegate = self;
    [self.view addSubview:_timeLineView];
    
    _fiveQuotationView = [[FiveQuotationView alloc] initWithFrame:CGRectMake(self.view.frame.size.width*0.7,
                                                                   kQuotaGuideViewHeight+10,
                                                                   self.view.frame.size.width*0.3,
                                                                   self.view.frame.size.height - kQuotaGuideViewHeight - kQuotaBannerHeight  - kQuotaButtonListHeight- kDockHeight - 40)];
    _fiveQuotationView.sellQuotation = [NSMutableArray arrayWithArray:@[@"-",@"-",@"-",@"-",@"-"]];
    _fiveQuotationView.sellVolume = [NSMutableArray arrayWithArray:@[@"-",@"-",@"-",@"-",@"-"]];
    _fiveQuotationView.buyQuotation = [NSMutableArray arrayWithArray:@[@"-",@"-",@"-",@"-",@"-"]];
    _fiveQuotationView.buyVolume = [NSMutableArray arrayWithArray:@[@"-",@"-",@"-",@"-",@"-"]];
    [self.view addSubview:_fiveQuotationView];
}

- (void)viewDidAppear:(BOOL)animated
{
    MYLOGFUN;
    [self.view insertSubview:self.QuotaAnalysisController.guideView aboveSubview:self.view];
    [self.view insertSubview:self.QuotaAnalysisController.buttonListView aboveSubview:self.view];
    self.QuotaAnalysisController.delegate = self;
    [NSTradeEngine sharedInstance].delegate = _timeLineView;
    [_timeLineView viewWillApper];
}

- (void)viewWillDisappear:(BOOL)animated
{
    MYLOGFUN;
    [super viewWillDisappear:animated];
    [_timeLineView viewWillDisapper];
    [NSTradeEngine sharedInstance].delegate = nil;
    
}


- (UIButton *)addButtonWithRect:(CGRect)rect Img:(NSString *)img Title:(NSString *)title
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = rect;
    UIImage *Img = [UIImage strechImageWithImg:img];
    [btn setBackgroundImage:Img forState:UIControlStateNormal];
    [btn setTitle:title forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    btn.titleLabel.font = [UIFont systemFontOfSize:12];
    return btn;
}

- (void)click
{
    if ([self.presentingViewController isKindOfClass:[SearchProductController class]]){
        UIViewController *preVC = self.presentingViewController;
        [self dismissViewControllerAnimated:NO completion:^{
        [preVC dismissViewControllerAnimated:NO completion:nil];
        }];
    }else{
        [self dismissViewControllerAnimated:YES completion:nil];
    }
}


-(void)setButtonAttr:(UIButton*)button{
    button.backgroundColor = [UIColor blackColor];
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
}
-(void)setButtonAttrWithClick:(UIButton*)button{
    button.backgroundColor = [UIColor colorWithHexString:@"cccccc" withAlpha:1];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
}

- (void)buy
{
    if (![self isLogin]) {
        [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
        return;
    }
    [self.navigationController pushViewController:[[BuyController alloc] init] animated:YES ];
}

- (void)sell
{
    if (![self isLogin]) {
        [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
        return;
    }
    [self.navigationController pushViewController:[[SellController alloc] init] animated:YES ];
}

- (void)cancelOrder
{
    if (![self isLogin]) {
        [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
        return;
    }
    [self.navigationController pushViewController:[[EntrustCancleOrderController alloc] init] animated:YES ];
}

- (void)queryOrder
{
    if (![self isLogin]) {
        [self.navigationController pushViewController:[[LoginController alloc] init] animated:YES];
        return;
    }
    [self.navigationController pushViewController:[[QueryController alloc] init] animated:YES];
}

- (BOOL)isLogin
{
    return [[NSTradeEngine setup] isLogin];
}

- (void)login_rsp_ui:(int)nRet type:(int)nType
{
    [super trade_login_rsp_to_ui:nRet nType:nType];
}

- (void)fiveQuotation_rsp:(NSMutableArray *)array
{
    if (array.count < 10) {
        MYLog(@"fiveQuotation_rsp count < 10");
        return;
    }
    NSMutableArray *sellQuotationArr = [NSMutableArray array];
    NSMutableArray *sellVolumeArr = [NSMutableArray array];
    NSMutableArray *buyQuotationArr = [NSMutableArray array];
    NSMutableArray *buyVolumeArr =[NSMutableArray array];
    for (int i = 0; i < 5; i++) {
        VolPriceNS *volPrice = array[i];
        [sellQuotationArr addObject:[NSString stringWithFormat:@"%0.2f",volPrice.m_uiPrice]];
        [sellVolumeArr addObject:[NSString stringWithFormat:@"%d",volPrice.m_uiVolume]];
    }
    for (int i = 5; i < 10; i++) {
        VolPriceNS *volPrice = array[i];
        [buyQuotationArr addObject:[NSString stringWithFormat:@"%0.2f",volPrice.m_uiPrice]];
        [buyVolumeArr addObject:[NSString stringWithFormat:@"%d",volPrice.m_uiVolume]];
    }
    _fiveQuotationView.sellQuotation = sellQuotationArr;
    _fiveQuotationView.sellVolume = sellVolumeArr;
    _fiveQuotationView.buyQuotation = buyQuotationArr;
    _fiveQuotationView.buyVolume = buyVolumeArr;
}
@end
