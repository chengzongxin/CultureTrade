//
//  MainController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "MainController.h"
#import "Dock.h"
#import "BaseNavigationController.h"
#import "HomeController.h"
#import "SelectController.h"
#import "QuotationController.h"
#import "MeController.h"
#define kAccountKey @"Account"
#define kPasswordKey @"Password"
#define kServerIP       @"ServerIP"

@interface MainController () <UINavigationControllerDelegate>

@end

@implementation MainController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addChildViewControllers];
    
    [self addDockItem];
    
    [_dock addSpliteLine];
#if BALANCE_ENABLE
    [self useBalace];
#else
    [self initalization];
#endif
//    [[NSTradeEngine setup] userloginwithnet:@"xigua" password:@"a123456" tradeip:@"192.168.3.166" tradeport:TRADESERVER_PORT quoteip:@"192.168.0.194" quoteport:QUOTESERVER_PORT];
//    [[NSTradeEngine setup] userloginwithnet:@"chengzongxin" password:@"123456" tradeip:@"192.168.0.194" tradeport:TRADESERVER_PORT quoteip:@"192.168.0.194" quoteport:QUOTESERVER_PORT];
    
}

- (void)useBalace
{
//    [[NSTradeEngine sharedInstance] add_balance:INNER_BALANCE_IP port:OUTER_BALANCE_PORT];
    [[NSTradeEngine sharedInstance] add_balance:@"192.168.0.168" port:5213];
    [[NSTradeEngine sharedInstance] start_balance];
}

- (void)initalization
{
    MYLog(@"get remember data!!!");
    MYLog(@"%@",[self getRememberServerIP]);
    MYLog(@"%@",[self getRememberAccount]);
    MYLog(@"%@",[self getRememberPassword]);
    if ([self getRememberAccount]&&[self getRememberPassword]&&[self getRememberServerIP]) { // 记住了用户密码，服务器IP同时登陆交易和行情
        [[NSTradeEngine setup] userloginwithnet:[self getRememberAccount] password:[self getRememberPassword] tradeip:[self getRememberServerIP] tradeport:TRADESERVER_PORT quoteip:[self getRememberServerIP] quoteport:QUOTESERVER_PORT];
    }else{
        if ([self getRememberServerIP]) { // 记住了服务器IP，匿名登陆行情服务器
            [[NSTradeEngine setup] loginquotewithnet:[self getRememberServerIP] quoteport:QUOTESERVER_PORT];
        }else{  //什么都没记住，匿名登陆默认服务器
            [[NSTradeEngine setup] loginquotewithnet:QUOTESERVER_IP quoteport:QUOTESERVER_PORT];
        }
    }
}

- (void)addChildViewControllers
{
    QuotationController *quota = [[QuotationController alloc] init];
    BaseNavigationController *nav3 = [[BaseNavigationController alloc] initWithRootViewController:quota];
    nav3.delegate = self;
    [self addChildViewController:nav3];
    
    HomeController *home = [[HomeController alloc] init];
    BaseNavigationController *nav1 = [[BaseNavigationController alloc] initWithRootViewController:home];
    nav1.delegate = self;
    [self addChildViewController:nav1];
    
//    SelectController *select = [[SelectController alloc] init];
//    BaseNavigationController *nav2 = [[BaseNavigationController alloc] initWithRootViewController:select];
//    nav2.delegate = self;
//    [self addChildViewController:nav2];
    
    MeController *me = [[MeController alloc] init];
    BaseNavigationController *nav4 = [[BaseNavigationController alloc] initWithRootViewController:me];
    nav4.delegate = self;
    [self addChildViewController:nav4];

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

- (void)addDockItem
{
//    [_dock addItemWithIcon:@"selfSelect" title:LocalizedStringByInt(1201)]; // 自选
    
    [_dock addItemWithIcon:@"quota" title:LocalizedStringByInt(1301)];   // 行情
    
    [_dock addItemWithIcon:@"trade" title:LocalizedStringByInt(1101)];    // 交易
    
    [_dock addItemWithIcon:@"mine" title:LocalizedStringByInt(1401)];   // 个人
    
    [self.view addSubview:_dock];
    
    [_dock clickFirstItem]; //第一次进入界面高亮第一个控制器按钮
}


- (NSString *)getRememberAccount
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kAccountKey];
}

- (NSString *)getRememberPassword
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kPasswordKey];
}

- (NSString *)getRememberServerIP
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kServerIP];
}

@end
