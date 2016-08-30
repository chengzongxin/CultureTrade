//
//  AppDelegate.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/21.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "AppDelegate.h"
#import "NewFeatureController.h"
#import "LoginController.h"
#import "MainController.h"
#import "BaseNavigationController.h"
#import "QuotaAnalysisController.h"
#import "NSNetHelper.h"
#import "NSTradeEngine.h"
#import "BaseNavigationController.h"
#import <CoreLocation/CoreLocation.h>
#import "MobClick.h"
#import "UMOnlineConfig.h"
#import "BuyController.h"
#import <JSPatch/JSPatch.h>
#define XcodeAppVersion [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]
#define UMENG_APPKEY @"56de339567e58ea82800198f"
@interface AppDelegate ()<CLLocationManagerDelegate>
{
    CLLocationManager * _locationManager;
    BOOL isLogined;
}
@property (assign, nonatomic) UIBackgroundTaskIdentifier bgTask;

@property (strong, nonatomic) dispatch_block_t expirationHandler;
@property (assign, nonatomic) BOOL jobExpired;
@property (assign, nonatomic) BOOL background;
@end

@implementation AppDelegate

- (void)umengTrack {
    Class cls = NSClassFromString(@"UMANUtil");
    SEL deviceIDSelector = @selector(openUDIDString);
    NSString *deviceID = nil;
    if(cls && [cls respondsToSelector:deviceIDSelector]){
        deviceID = [cls performSelector:deviceIDSelector];
    }
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:@{@"oid" : deviceID}
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:nil];
    
    NSLog(@"%@", [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]);
    //    [MobClick setCrashReportEnabled:NO]; // 如果不需要捕捉异常，注释掉此行
    [MobClick setLogEnabled:YES];  // 打开友盟sdk调试，注意Release发布时需要注释掉此行,减少io消耗
    [MobClick setAppVersion:XcodeAppVersion]; //参数为NSString * 类型,自定义app版本信息，如果不设置，默认从CFBundleVersion里取
    //
    [MobClick startWithAppkey:UMENG_APPKEY reportPolicy:SEND_INTERVAL channelId:@"Cheng"];
    //   reportPolicy为枚举类型,可以为 REALTIME, BATCH,SENDDAILY,SENDWIFIONLY几种
    //   channelId 为NSString * 类型，channelId 为nil或@""时,默认会被被当作@"App Store"渠道
    [MobClick setBackgroundTaskEnabled:YES];/** 设置是否开启background模式, 默认YES. */
    [UMOnlineConfig updateOnlineConfigWithAppkey:UMENG_APPKEY];
}

- (void)startJSPatch
{
    [JSPatch startWithAppKey:@"04340411983efc39"];
    
    //用来检测回调的状态，是更新或者是执行脚本之类的，相关信息，会打印在你的控制台
    [JSPatch setupCallback:^(JPCallbackType type, NSDictionary *data, NSError *error) {
        NSLog(@"error-->%@",error);
        switch (type) {
            case JPCallbackTypeUpdate: {
                NSLog(@"更新脚本 %@ %@", data, error);
                break;
            }
            case JPCallbackTypeRunScript: {
                NSLog(@"执行脚本 %@ %@", data, error);
                break;
            }
            case JPCallbackTypeCondition: {
                NSLog(@"条件下发 %@ %@", data, error);
                break;
            }
            case JPCallbackTypeGray: {
                NSLog(@"灰度下发 %@ %@", data, error);
                break;
            }
            default:
                break;
        }
    }];
#ifdef DEBUG
    [JSPatch setupDevelopment];
#endif
    [JSPatch sync];
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    //  友盟的方法本身是异步执行，所以不需要再异步调用
    [self umengTrack];
    
    [self startJSPatch];
    
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window.backgroundColor = [UIColor whiteColor];
    
    [[NSNetHelper setup] initNet];
    
    UIApplication* app = [UIApplication sharedApplication];
    app.delegate = self;
    __weak AppDelegate* selfRef = self;
    isLogined = YES;
    
    self.expirationHandler = ^{  //创建后台自唤醒，当180s时间结束的时候系统会调用这里面的方法
        [app endBackgroundTask:selfRef.bgTask];
        selfRef.bgTask = UIBackgroundTaskInvalid;
        selfRef.bgTask = [app beginBackgroundTaskWithExpirationHandler:selfRef.expirationHandler];
//        NSLog(@"Expired");
        selfRef.jobExpired = YES;
//        while(selfRef.jobExpired)
//        {
////             spin while we wait for the task to actually end.
//            NSLog(@"等待180s循环进程的结束");
//            [NSThread sleepForTimeInterval:1];
//        }
        // Restart the background task so we can run forever.
        [selfRef startBackgroundTask];
    };
    
    
    // Assume that we're in background at first since we get no notification from device that we're in background when
    // app launches immediately into background (i.e. when powering on the device or when the app is killed and restarted)
    [self monitorBatteryStateInBackground];
    _locationManager = [[CLLocationManager alloc] init];
    _locationManager.delegate = self;
    [_locationManager requestAlwaysAuthorization];//添加这句
    if(![CLLocationManager locationServicesEnabled] || ([CLLocationManager authorizationStatus] == kCLAuthorizationStatusDenied))//判断定位服务是否打开
    {
        MYLog(@"please open location service!");
    }
    
    self.window.rootViewController = [[MainController alloc] init];
    
    /*
    NSString *key = (NSString *)kCFBundleVersionKey;
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    
    NSString *saveVersion = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
    if ([version isEqualToString:saveVersion]) {
        application.statusBarHidden = NO;
        
        // debug
        self.window.rootViewController = [[BaseNavigationController alloc] initWithRootViewController:[[LoginController alloc] init]];
    } else {
        [[NSUserDefaults standardUserDefaults] setObject:version forKey:key];
        [[NSUserDefaults standardUserDefaults] synchronize];
    
    
        self.window.rootViewController = [[NewFeatureController alloc] init];
    }
     
     */
    [self.window makeKeyAndVisible];
    
    return YES;
}


- (void)monitorBatteryStateInBackground
{
    self.background = YES;
    [self startBackgroundTask];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    if (_bgTask!=UIBackgroundTaskInvalid) {
        [application endBackgroundTask:_bgTask];
        _bgTask = UIBackgroundTaskInvalid;
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    NSLog(@"App is active");
    [UIApplication sharedApplication].applicationIconBadgeNumber=0;//取消应用程序通知脚标
    [_locationManager stopUpdatingLocation];
    self.background = NO;
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    MYLOGFUN;
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    //if([self bgTask])
    if(isLogined)//当登陆状态才启动后台操作
    {
        self.bgTask = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:self.expirationHandler];
        NSLog(@"Entered background");
        [self monitorBatteryStateInBackground];
    }
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error//当定位服务不可用出错时，系统会自动调用该函数
{
    NSLog(@"定位服务出错");
    if([error code]==kCLErrorDenied)//通过error的code来判断错误类型
    {
        //Access denied by user
        MYLog(@"未开启定位服务\n客户端保持后台功能需要调用系统的位置服务\n请到设置中打开位置服务");
        showDetailAlert(@"错误", @"未开启定位服务\n客户端保持后台功能需要调用系统的位置服务\n请到设置中打开位置服务");
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations//当用户位置改变时，系统会自动调用，这里必须写一点儿代码，否则后台时间刷新不管用
{
    NSLog(@"位置改变，必须做点儿事情才能刷新后台时间");
    CLLocation *loc = [locations lastObject];
    //NSTimeInterval backgroundTimeRemaining = [[UIApplication sharedApplication] backgroundTimeRemaining];
    //NSLog(@"Background Time Remaining = %.02f Seconds",backgroundTimeRemaining);
    // Lat/Lon
    float latitudeMe = loc.coordinate.latitude;
    float longitudeMe = loc.coordinate.longitude;
    latitudeMe = 0.0;
    longitudeMe = 0.0;
}

- (void)startBackgroundTask
{
//    NSLog(@"Restarting task");
    
    if(isLogined)//当登陆状态才进入后台循环
    {
        // Start the long-running task.
//        NSLog(@"登录状态后台进程开启");
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            // When the job expires it still keeps running since we never exited it. Thus have the expiration handler
            // set a flag that the job expired and use that to exit the while loop and end the task.
            NSInteger count=0;
            BOOL NoticeNoBackground=false;//只通知一次标志位
            BOOL FlushBackgroundTime=false;//只通知一次标志位
            _locationManager.distanceFilter = kCLDistanceFilterNone;//任何运动均接受，任何运动将会触发定位更新
            _locationManager.desiredAccuracy = kCLLocationAccuracyHundredMeters;//定位精度
            while(self.background && !self.jobExpired)
            {
                NSLog(@"进入后台进程循环");
                [NSThread sleepForTimeInterval:0.1];
                count++;
                if(count>60)//每60s进行一次开启定位，刷新后台时间
                {
                    count=0;
                    [_locationManager startUpdatingLocation];
                    NSLog(@"开始位置服务");
                    [NSThread sleepForTimeInterval:0.1];
                    [_locationManager stopUpdatingLocation];
                    NSLog(@"停止位置服务");
                    FlushBackgroundTime=false;
                }
                if(!isLogined)//未登录或者掉线状态下关闭后台
                {
                    NSLog(@"保持在线进程失效，退出后台进程");
                    showAlert(@"保持在线失效，登录已被注销，请重新登录");
                    [[UIApplication sharedApplication] endBackgroundTask:self.bgTask];
                    return;//退出循环
                }
                NSTimeInterval backgroundTimeRemaining = [[UIApplication sharedApplication] backgroundTimeRemaining];
                NSLog(@"Background Time Remaining = %.02f Seconds",backgroundTimeRemaining);
                if(backgroundTimeRemaining<30&&NoticeNoBackground==false)
                {
//                    showAlert(@"向系统申请长时间保持后台失败，请结束客户端重新登录");
                    NoticeNoBackground=true;
                }
                //测试后台时间刷新
                if(backgroundTimeRemaining>200&&FlushBackgroundTime==false)
                {
                    [[NSNotificationCenter defaultCenter] postNotificationName:@"MessageUpdate" object:@"刷新后台时间成功\n"];
                    FlushBackgroundTime=true;
//                    [InterfaceFuncation ShowLocalNotification:@"刷新后台时间成功"];
                }
                if (backgroundTimeRemaining>86400) {//进入到前台了，趋于无限大
                    self.background = NO;
                }
            }
            self.jobExpired = NO;
            
        });
    }
}


//一、始终隐藏状态栏
//如果在App中需要状态栏一直是隐藏着的，可以在AppDelegate的application:didFinishLaunchingWithOptions:函数中进行设置，比如下面这段示意代码可以让状态栏以淡出的方式隐藏起来：
//复制代码
//- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions { // Override point for customization after application launch.
//    [application setStatusBarHidden:NO withAnimation:UIStatusBarAnimationFade];
//    // Add the view controller's view to the window and display.
//    [self.window addSubview:viewController.view];
//    [self.window makeKeyAndVisible]; return YES;
//}
//    复制代码
//    二、APP启动时隐藏状态栏
//    用了上面的方法之后，App在运行过程中，状态栏确实被隐藏起来了，但是我发现在App启动的那个瞬间，还是可以看到状态栏的，然后一 闪即过。虽然时间很短暂，看着还是很不舒服。为了让状态栏从启动的时候就隐藏起来，可以修改-Info.plist。如 果在Xcode中修改，在根结点Infomation Property List下面新加一项“Status bar is initially hidden”（不用手动输入，可以直接在下拉菜单中选取）。这是个BOOL类型的键值，将Value栏中的复选框勾选上即可。
//    
//    
//    
//    
//    三、在运行程序过程中隐藏状态栏
//    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
//    
//    
//    实例：
//    复制代码
//    - (void)viewDidLoad { [super viewDidLoad]; // Do any additional setup after loading the view, typically from a nib. flag = 0; UIView *view = [[UIControl alloc]init]; view.backgroundColor = [UIColor redColor]; [(UIControl *)view addTarget:self action:@selector(touchpicture:) forControlEvents:UIControlEventTouchUpInside]; [view setFrame:CGRectMake(0, 0, 320, 480)]; [self.view addSubview:view]; }
//        复制代码
//        复制代码
//        -(void)touchpicture:(UIView *)sender{ if (flag == 0) { [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide]; flag = 1; }else { [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationSlide]; flag = 0; } }
//
//
//
@end
