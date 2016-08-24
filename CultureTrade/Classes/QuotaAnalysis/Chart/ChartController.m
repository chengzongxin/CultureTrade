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

#define kButtonHeight 20

@interface ChartController ()  <QuotaAnalysisControllerDelegate,TimeLineViewDelegate>
{
    TimeLineView *_timeLineView;
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
                                                                   self.view.frame.size.width,
                                                                   self.view.frame.size.height - kQuotaGuideViewHeight - kQuotaBannerHeight  - kQuotaButtonListHeight- kDockHeight - 40)];
    _timeLineView.delegate = self;

    [self.view addSubview:_timeLineView];
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

@end
