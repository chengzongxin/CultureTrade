//
//  DockController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "DockController.h"
#import "Dock.h"


@interface DockController () <DockDelegate>

@end

@implementation DockController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _dock = [[Dock alloc] init];
    _dock.frame = CGRectMake(0, self.view.frame.size.height - kDockHeight, ScreenSize.width, kDockHeight);
    _dock.delegate = self;
    
    [self.view addSubview:_dock];
    
    [_dock addSpliteLine];
}


- (void)dock:(Dock *)dock itemSelectedFrom:(int)from to:(int)to
{
    NSLog(@"%d,%d",from,to);
    
    
    if (to < 0 || to >= self.childViewControllers.count) return;
    
    UIViewController *oldVC = self.childViewControllers[from];
    [oldVC.view removeFromSuperview];
    
    UIViewController *newVC = self.childViewControllers[to];
    newVC.view.frame = CGRectMake(0, 0, ScreenSize.width, ScreenSize.height - kDockHeight);
    [self.view addSubview:newVC.view];

}


@end
