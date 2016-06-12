//
//  BaseViewController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/10.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BaseViewController.h"

@interface BaseViewController ()

@end

@implementation BaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.view.backgroundColor = COLOR_VC_BG;
//    self.view.backgroundColor = BaseCell_Color;
//    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:nil];
    
}

#pragma mark - ===点击空白区域隐藏键盘===
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}


@end
