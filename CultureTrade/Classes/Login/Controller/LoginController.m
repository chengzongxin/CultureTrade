//
//  LoginViewController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "LoginController.h"
#import "MainController.h"
#import "NSString+helper.h"
#import "UIBarButtonItem+helper.h"
#import "BaseTextField.h"
#import "NSTradeEngine.h"
#import "SDProgressView.h"
#import "BasicAlertView.h"
#import "TraderController.h"
#import "ASProgressPopUpView.h"
#import "BaseNavigationBar.h"
#define kAccountKey     @"Account"
#define kPasswordKey    @"Password"
#define kServerIP       @"ServerIP"

@interface LoginController () 
{
    BaseTextField *_account;
    BaseTextField *_password;
    ToggleView *_switchAccount;
    ToggleView *_switchPassword;
    BOOL _rememberAccount;
    BOOL _rememberPassword;
    float _progress;

    NSString *_serverIP;
    ASProgressPopUpView *_progressView;
    
    UIButton *_anonyLoginBtn;
    UIButton *_loginBtn;
}
@property (strong, nonatomic) NSMutableArray *onOffViewArray;
@end

@implementation LoginController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    NSThread.sleepForTimeInterval(3.0)
//    sleep(2); // 启动页面等待
    
//    [self addNavigationButton];

    [self addNavigationButton];
    
    [self addView];
    
    [self initSetting];
    
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onlineConfigCallBack:) name:UMOnlineConfigDidFinishedNotification object:nil];
}

- (void)addNavigationButton
{
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    self.navigationItem.rightBarButtonItem = [UIBarButtonItem itemWithIcon:@"trader" highlightedIcon:nil target:self action:@selector(findTrader)];
//    self.navigationItem.rightBarButtonItem = [UIBarButtonItem itemWithIcon:@"refresh" highlightedIcon:nil target:self action:nil];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)findTrader
{
    TraderController *trader = [[TraderController alloc] init];
    [self.navigationController pushViewController:trader animated:YES];
}


- (void)initSetting
{
    if ([self getRememberServerIP]) {
        self.tradeIP = [self getRememberServerIP];
        self.quotaIP = [self getRememberServerIP];
    }else{
        self.tradeIP = TRADESERVER_IP;
        self.quotaIP = QUOTESERVER_IP;
    }
#if ONLINECONFIG
    if (_serverIP) {  // 在线参数
        self.tradeIP = _serverIP;
        self.quotaIP = _serverIP;
    }
#endif
    
    _switchAccount.selectedButton = ToggleButtonSelectedRight;
    _switchPassword.selectedButton = ToggleButtonSelectedRight;
    _progress = 0.0f;
    _serverIP = nil;
    _progressView = [[ASProgressPopUpView alloc] initWithProgressViewStyle:UIProgressViewStyleBar];
    _progressView.frame = CGRectMake(0, 0, self.view.frame.size.width, 2);
    _progressView.popUpViewAnimatedColors = @[[UIColor redColor], [UIColor orangeColor], [UIColor greenColor]];
    [self.view addSubview:_progressView];
}

- (void)addView
{
    // 头像文本框
    UIImageView *faceImg = [[UIImageView alloc] init];
    faceImg.frame = CGRectMake(0, 0, 100, 100);
    faceImg.center = CGPointMake(ScreenSize.width / 2, 120);
    faceImg.image = [UIImage imageNamed:@"face"];
    
    // 添加边框
    CALayer *layer = [faceImg layer];
    layer.borderColor = [COLOR(161, 158, 150, 0.2) CGColor];
    layer.borderWidth = 5.0f;
    layer.cornerRadius = 20.0; // 设置圆角
    layer.masksToBounds = YES; // 边框可见
    
    // 添加四个边阴影
    faceImg.layer.shadowColor = [UIColor grayColor].CGColor; // 阴影颜色
    faceImg.layer.shadowOffset = CGSizeMake(0, 0); // 偏移距离
    faceImg.layer.shadowOpacity = 0.5;  // 不透明度
    faceImg.layer.shadowRadius = 10; // 半径
    [self.view addSubview:faceImg];
    
    //账户文本框
    float accontX,accontY,accontW,accontH;
    accontX = 30;
    accontY = 210;
    accontW = ScreenSize.width - 2 * accontX;
    accontH = 27;
    _account = [[BaseTextField alloc] initWithFrame:CGRectMake(accontX,accontY,accontW,accontH)];
    _account.delegate = self;
    _account.text = [self getRememberAccount];
    _account.placeholder = LocalizedStringByInt(1004);
    [self.view addSubview:_account];
    
    //密码文本框
    float passwordX,passwordY,passwordW,passwordH;
    passwordX = accontX;
    passwordY = CGRectGetMaxY(_account.frame) + 15;
    passwordW = accontW;
    passwordH = accontH;
    _password = [[BaseTextField alloc] initWithFrame:CGRectMake(passwordX, passwordY, passwordW, passwordH)];
    [_password setSecureTextEntry:YES]; //验证
    _password.text = [self getRememberPassword];
    _password.delegate = self;
    _password.placeholder = LocalizedStringByInt(1005);
    [self.view addSubview:_password];
    
    // 记住账号
    float betweenLeft = 50,betweenSwitch = 3,betweenCenter = 20;
    
    float aX,aY,aW,aH;
    aX = betweenLeft - 5 ;
    aY = CGRectGetMaxY(_password.frame) + 20;
    aW = (ScreenSize.width - 2 * betweenLeft - 2 * betweenSwitch - 2 * 50 - betweenCenter)/2;
    aH = 25;
    UILabel *lable1 = [[UILabel alloc] initWithFrame:CGRectMake(aX, aY, aW, aH)];
    lable1.lineBreakMode = NSLineBreakByClipping;
    lable1.font = FONT_LOGIN_LABEL_TEXT;
    lable1.text = LocalizedStringByInt(1001);
    lable1.textColor = [UIColor whiteColor];

    
    [self.view addSubview:lable1];
    
    float asX,asY;
    asX = CGRectGetMaxX(lable1.frame) + betweenSwitch;
    asY = aY;

    CGSize size = [UIImage imageNamed:@"off_bg"].size;
    CGRect rect = CGRectMake(asX, asY, size.width, size.height);
    _switchAccount = [[ToggleView alloc] initWithFrame:rect toggleViewType:ToggleViewTypeNoLabel toggleBaseType:ToggleBaseTypeChangeImage toggleButtonType:ToggleButtonTypeChangeImage];
    _switchAccount.toggleDelegate = self;
    [self.view addSubview:_switchAccount];
    
    
    // 记住密码
    float pX,pY,pW,pH;
    pX = CGRectGetMaxX(_switchAccount.frame) + betweenCenter;
    pY = aY;
    pW = aW;
    pH = aH;
    UILabel *lable2 = [[UILabel alloc] initWithFrame:CGRectMake(pX, pY, pW, pH)];
    lable2.lineBreakMode = NSLineBreakByClipping;
    lable2.font = FONT_LOGIN_LABEL_TEXT;
    lable2.text = LocalizedStringByInt(1002);
    lable2.textColor = [UIColor whiteColor];
    [self.view addSubview:lable2];
    
    
    //密码
    float psX,psY;
    psX = CGRectGetMaxX(lable2.frame) + betweenSwitch;
    psY = aY;
    CGRect switchPsw = CGRectMake(psX, psY, size.width, size.height);
    _switchPassword = [[ToggleView alloc] initWithFrame:switchPsw toggleViewType:ToggleViewTypeNoLabel toggleBaseType:ToggleBaseTypeChangeImage toggleButtonType:ToggleButtonTypeChangeImage];
    _switchPassword.toggleDelegate = self;
    [self.view addSubview:_switchPassword];
    
    
    UIImage *anonyloginImg = [UIImage imageNamed:@"anonylogin"];
    UIButton *anonybtn = [UIButton buttonWithType:UIButtonTypeCustom];
    anonybtn.frame = CGRectMake(accontX + 20, CGRectGetMaxY(_switchAccount.frame) + 20, anonyloginImg.size.width,anonyloginImg.size.height);
    [anonybtn setTintColor:[UIColor whiteColor]];
    [anonybtn setImage:[UIImage imageNamed:@"anonylogin"] forState:UIControlStateNormal];
    [anonybtn addTarget:self action:@selector(anonylogin:) forControlEvents:UIControlEventTouchDown];
    [self.view addSubview:anonybtn];
    _anonyLoginBtn = anonybtn;
    
    UILabel *anonyLabel = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetMaxX(anonybtn.frame) + 10, anonybtn.frame.origin.y, 50,anonyloginImg.size.height)];
    anonyLabel.text = @"匿名登录";
    anonyLabel.font = [UIFont systemFontOfSize:12];
    anonyLabel.textColor = COLOR_GOLD_THEME;
    [self.view addSubview:anonyLabel];
    
    UIImage *btnImg = [UIImage imageNamed:@"login"];
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(accontX, CGRectGetMaxY(_switchAccount.frame) + 60, accontW,btnImg.size.height);
    btn.center = CGPointMake(self.view.center.x, btn.center.y);
    [btn setTintColor:[UIColor whiteColor]];
    [btn setBackgroundImage:btnImg forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(login) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btn];
    _loginBtn = btn;
}


#pragma mark - 记住账户密码
//框架只支持监听一个按钮的点击时间，出现两个的时候要判断是触发了哪个
- (void)selectLeftButton
{
    if (_switchAccount.toggleButton.frame.origin.x == 0 && _switchPassword.toggleButton.frame.origin.x == 0) { //不记住账户,不记住密码
        MYLog(@"不记住账户,不记住密码");
        _rememberAccount = NO;
        _rememberPassword = NO;
    }else if (_switchAccount.toggleButton.frame.origin.x == 0) {
        MYLog(@"不记住账户");
        _rememberAccount = NO;
        _rememberPassword = YES;
    }else{
        MYLog(@"不记住密码");
        _rememberAccount = YES;
        _rememberPassword = NO;
    }
}

- (void)selectRightButton
{
    if (_switchAccount.toggleButton.frame.origin.x != 0 && _switchPassword.toggleButton.frame.origin.x != 0) { //激活记住账户,记住密码
        MYLog(@"记住账户,记住密码");
        _rememberAccount = YES;
        _rememberPassword = YES;
    }else if (_switchAccount.toggleButton.frame.origin.x != 0) {
        MYLog(@"记住账户");
        _rememberAccount = YES;
        _rememberPassword = NO;
    }else{
        MYLog(@"记住密码");
        _rememberAccount = NO;
        _rememberPassword = YES;
    }
}

- (NSString *)getRememberServerIP
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kServerIP];
}

- (NSString *)getRememberAccount
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kAccountKey];
}

- (NSString *)getRememberPassword
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:kPasswordKey];
}

-(void)rememberAccount
{
    [[NSUserDefaults standardUserDefaults] setObject:_account.text forKey:kAccountKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)rememberPassword
{
    [[NSUserDefaults standardUserDefaults] setObject:_password.text forKey:kPasswordKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)rememberServerIP
{
    [[NSUserDefaults standardUserDefaults] setObject:self.tradeIP forKey:kServerIP];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)unRememberAccount
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:kAccountKey];
}

- (void)unRememberPassword
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:kPasswordKey];
}

- (void)rememberAccountOrPassword
{
    if (_rememberAccount) {
        [self rememberAccount];
    }else{
        [self unRememberAccount];
    }
    
    if (_rememberPassword) {
        [self rememberPassword];
    }else{
        [self unRememberPassword];
    }
    
    [self rememberServerIP];
}

- (void)onlineConfigCallBack:(NSNotification *)notification {
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    NSLog(@"online config has fininshed and params = %@", notification.userInfo);
    NSDictionary *paramDict = notification.userInfo;
    _serverIP = [paramDict objectForKey:@"serverIP"];
}


- (void)login
{
    if (_anonyLoginBtn.selected) {
        [self anonylogin];
        return;
    }
    
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UMOnlineConfigDidFinishedNotification object:nil];
    
    if (_account.text.length == 0 || _password.text.length == 0){
        showAlert(@"请输入账号和密码");
        return;
    }
        
    
    [self rememberAccountOrPassword];
    
    [self checkNetwork];
    
    [self sendLoginPackage];
    
    [self showProgress];
}

- (void)anonylogin
{
    [self rememberServerIP];
    
    [[NSTradeEngine setup] loginquotewithnet:self.quotaIP quoteport:QUOTESERVER_PORT];
    
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)anonylogin:(UIButton *)anonybtn
{
    anonybtn.selected = !anonybtn.selected;
    if (anonybtn.selected) {
        [anonybtn setImage:[UIImage imageNamed:@"anonylogin_selected"] forState:UIControlStateNormal];
    }else{
        [anonybtn setImage:[UIImage imageNamed:@"anonylogin"] forState:UIControlStateNormal];
    }
    
    _account.enabled = !anonybtn.selected;
    _password.enabled = !anonybtn.selected;
}

- (void)showProgress
{
    [super showProgressByDelay:@"Logining..." delayInSeconds:15];
}

- (void)sendLoginPackage
{
    NSString *account = _account.text;
    NSString *password = _password.text;
    
    int tradeport = TRADESERVER_PORT;
    int quotaport = QUOTESERVER_PORT;
    
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
    /*
     优先级
     
     1.切换交易商服务器
     2.ONLINECONFIG
     3.宏定义
     
     */
    
    [[NSTradeEngine sharedInstance] userloginwithnet:account password:password tradeip:self.tradeIP tradeport:tradeport quoteip:self.quotaIP quoteport:quotaport];
    
    [MobClick profileSignInWithPUID:@"Login" provider:account];
}

#pragma mark -
#pragma mark 网络回调
- (void)trade_login_rsp_to_ui:(int)nRet nType:(int)nType
{
    if (nRet != 1) {
        NSString *message = [NSString stringWithFormat:@"%@",LocalizedStringByInt(nRet)];
        BasicAlertView *alert = [BasicAlertView alertViewWithTitle:@"登陆失败"
                                                          subtitle:nil
                                                           message:message
                                                           buttons:[NSArray arrayWithObject:@"confirm"]
                                                      afterDismiss:^(int buttonIndex) {
                                                          MYLog(@"%d",buttonIndex);
                                                          [self closeProgress:nil];
                                                      }];
        [alert show];
        [super dissmissProgress];
    }else{
        [super closeProgressSuccess:@"Success!"];
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)trade_ui_login_progress_rsp:(int)progress max:(int)max_progress
{
    dispatch_async(dispatch_get_main_queue(), ^{
        float curProgress = _progressView.progress;
        for (int i = 0; i < 33; i++) {
            curProgress += 0.01f;
            [_progressView setProgress:curProgress animated:YES];
        }
    });
    
    if (progress == max_progress) {
//        [super closeProgressSuccess:@"Success!"];
//        [self presentViewController:[[MainController alloc] init] animated:YES completion:nil];
//        [self.navigationController popViewControllerAnimated:YES];
//        [self dismissViewControllerAnimated:YES completion:nil];
    }
}


- (void)on_net_status_rsp:(int)type nFlag:(int)nFlag
{
    MYLOGFUN;
    MYLog(@"%d,%d",type,nFlag);
}

- (void)quote_ui_login_rsp:(int)nRet
{
    MYLOGFUN;
    MYLog(@"11111111%d",nRet);
}

- (void)checkNetwork
{
    if([[NSNetHelper setup] getNetState] == NoNetWork){
        showAlert(@"no network");// 有可能是检测不到网线链接。只是别wifi和3G
        return;
    }
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    if (textField == _password)
        [super textFieldShouldBeginEditing:_account];
    if (textField == _account)
        [super textFieldShouldBeginEditing:_password];
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [textField endEditing:YES];
}

@end
