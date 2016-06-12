//
//  BaseNavigationController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/27.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BaseNavigationController.h"
#import "BaseNavigationBar.h"

@interface BaseNavigationController ()
@property (nonatomic,strong) BaseNavigationBar *baseNavigationBar;
@end

@implementation BaseNavigationController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.navigationBar setBackgroundImage:[UIImage strechImageWithImg:@"top"] forBarMetrics:UIBarMetricsDefault];
    self.navigationBar.layer.shadowColor = [UIColor blackColor].CGColor;
    self.navigationBar.layer.shadowOffset = CGSizeMake(0, 5);
    self.navigationBar.layer.shadowOpacity = 0.5;
    [self.navigationBar setTitleVerticalPositionAdjustment:0 forBarMetrics:UIBarMetricsDefault];
    [self.navigationBar setTitleTextAttributes:@{
                                                 NSForegroundColorAttributeName : COLOR_NAVIGATION_TITLE,
                                                 NSFontAttributeName : [UIFont boldSystemFontOfSize:14]
                                                 //                                  NSShadowAttributeName : [NSValue valueWithUIOffset:UIOffsetZero]
                                                 }];
    
    UIBarButtonItem *backItem = self.navigationItem.backBarButtonItem;
    [backItem setBackgroundImage:[UIImage imageNamed:@"back"] forState:UIControlStateNormal style:UIBarButtonItemStyleDone barMetrics:UIBarMetricsDefault];
//    [backItem setBackgroundImage:[UIImage imageNamed:@"back"] forState:UIControlStateNormal barMetrics:UIBarMetricsDefault];
}

- (id)init {
    self = [super initWithNavigationBarClass:[BaseNavigationBar class] toolbarClass:nil];
    if(self) {
        // Custom initialization here, if needed.
    }
    return self;
}

- (id)initWithRootViewController:(UIViewController *)rootViewController {
    self = [super initWithNavigationBarClass:[BaseNavigationBar class] toolbarClass:nil];
    if(self) {
        self.viewControllers = @[rootViewController];
    }
//    self.navigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
    return self;
}

//- (void)pushViewController:(UIViewController *)viewController animated:(BOOL)animated
//{
//    MYLOGFUN;
//    [super pushViewController:viewController animated:animated];
//}

@end
