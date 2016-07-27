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
#define kServerIP   @"ServerIP"
#define kBalanceIP  @"BalanceIP"
#define kBalancePort @"BalancePort"

@interface MainController () <UINavigationControllerDelegate>

@end

@implementation MainController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addChildViewControllers];
    
    [self addDockItem];
    
    [_dock addSpliteLine];
#if BALANCE_ENABLE
    [self LoginWithCacheIP];
    
    [self useOuterBalance];      // wd

//    [self useInnerBanlance];    // test
#else
    [self initalization];
#endif
}

- (void)LoginWithCacheIP
{
    NSString *server = [self getRememberBalanceIP];
    int port = [self getRememberBalancePort];
    if (server&&port){
        MYLog(@"LoginWithCacheIP server = %@,port = %d",server,port);
        [self useBalaceWith:server port:port];
    }
}


- (void)useInnerBanlance
{
    [self useBalaceWith:INNER_BALANCE_IP port:INNER_BALANCE_PORT];
}

- (void)useOuterBalance
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onlineConfigCallBack:) name:UMOnlineConfigDidFinishedNotification object:nil];
}


- (void)onlineConfigCallBack:(NSNotification *)notification {
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    NSLog(@"online config has fininshed and params = %@", notification.userInfo);
    NSDictionary *paramDict = notification.userInfo;
    NSString *serverIP = [paramDict objectForKey:@"serverIP"];
    int serverPort = [[paramDict objectForKey:@"serverPort"] intValue];
    
    if ([serverIP isEqualToString:[self getRememberBalanceIP]]&&(serverPort == [self getRememberBalancePort])) return;  // same as cache
    
    // diff as cache
    MYLog(@"diff as chche,outter server = [%@],port = [%d],cache server = [%@],port = [%d]",serverIP,serverPort,[self getRememberBalanceIP],[self getRememberBalancePort]);
    [self useBalaceWith:serverIP port:serverPort];    // login balance
    [self rememberBalanceIP:serverIP Port:serverPort]; // cache
}


- (void)useBalaceWith:(NSString *)ip port:(int)port
{
    [[NSTradeEngine sharedInstance] clear_balance];
    [[NSTradeEngine sharedInstance] add_balance:ip port:port];
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


- (void)rememberBalanceIP:(NSString *)ip Port:(int)port
{
    [[NSUserDefaults standardUserDefaults] setObject:ip forKey:kBalanceIP];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    [[NSUserDefaults standardUserDefaults] setObject:@(port) forKey:kBalancePort];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (NSString *)getRememberBalanceIP
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kBalanceIP];
}

- (int)getRememberBalancePort
{
    NSNumber *number = [[NSUserDefaults standardUserDefaults] objectForKey:kBalancePort];
    return [number intValue];
}

@end
