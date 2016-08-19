//
//  SuperViewController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/1/27.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SuperViewController : UIViewController<NSNetHelperDelegate>
{
    UIButton *doneButton;
}
// 屏幕上弹
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField;
// 屏幕恢复
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField;

- (void)textFieldDidBeginEditing:(UITextField *)textField;

- (void)textFieldDidEndEditing:(UITextField *)textField;
// 点击空白区域隐藏键盘
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;

-(void)showProgress:(NSString*)title;

-(void)showProgressByDelay:(NSString*)title delayInSeconds:(int)delayInSeconds;

-(void)closeProgress:(NSString*)title;

- (void)closeProgressSuccess:(NSString *)title;

- (void)dissmissProgress;

- (void)ClickClear;

@end

