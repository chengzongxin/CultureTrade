//
//  SuperViewController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/27.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "SuperViewController.h"
#import "MMProgressHUD.h"
#import "CombiInputText.h"
#import "UIBarButtonItem+helper.h"
#import "UIView+Toast.h"

@interface SuperViewController () <UITextFieldDelegate>
{
    NSTimer *mTimer;
}
@end

@implementation SuperViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector (doneButtonshow:) name: UIKeyboardDidShowNotification object:nil];
    self.view.backgroundColor = COLOR_VC_BG;
    
    [NSNetHelper setup].delegate = self;
    
    [self addTitleView];
}

- (void)addTitleView
{
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(clickBack)];
}

- (void)clickBack
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.view endEditing:YES];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    [self clearTimer];
}

-(void)showProgress:(NSString*)title
{
    [UIApplication sharedApplication].networkActivityIndicatorVisible = TRUE;
    [self showProgressByDelay:title delayInSeconds:10];
}

-(void) showProgressByDelay:(NSString*)title delayInSeconds:(int)delayInSeconds
{
    [MMProgressHUD setPresentationStyle:MMProgressHUDPresentationStyleShrink];
    [MMProgressHUD showWithTitle:nil status:title];
    [self clearTimer];
    mTimer = [NSTimer scheduledTimerWithTimeInterval:delayInSeconds target:self selector:@selector(progress_timeout) userInfo:nil repeats:NO];
    
}
-(void)progress_timeout
{
    [UIApplication sharedApplication].networkActivityIndicatorVisible = FALSE;
    [MMProgressHUD dismissWithError:@"提交超时"];
    [self clearTimer];
}

-(void) clearTimer{
    if(mTimer != nil)
        [mTimer invalidate];
    mTimer = nil;
}

-(void) closeProgress:(NSString*)title
{
    [self clearTimer];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = FALSE;
    if(nil != title)
        [MMProgressHUD dismissWithError:title];
    else
        [MMProgressHUD dismiss];
}

- (void)closeProgressSuccess:(NSString *)title
{
    [self clearTimer];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = FALSE;
    [MMProgressHUD dismissWithSuccess:title];
}

- (void)dissmissProgress
{
    [self clearTimer];
    [UIApplication sharedApplication].networkActivityIndicatorVisible = FALSE;
    [MMProgressHUD dismiss];
}

#pragma mark - ===点击空白区域隐藏键盘===
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}

- (void)textFieldDidBeginEditing:(UITextField *)textField{}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [self textFieldShouldEndEditing:textField];
}

#pragma mark - 键盘上弹
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    MYLog(@"Beging Editing！");
    //键盘高度216 + ex 50 ,判断是否遮挡住
    CGFloat offset = self.view.frame.size.height - (textField.frame.origin.y + textField.frame.size.height + 216 + 50);
    MYLog(@"offset = %f",offset);
    
    if (offset <= 0) {
        [UIView animateWithDuration:0.3f animations:^{
            
            CGRect frame = self.view.frame;
            MYLog(@"%@",NSStringFromCGRect(frame));
            frame.origin.y = offset;
            self.view.frame = frame;
        }];
    }
    
    return YES;
}

#pragma mark -屏幕恢复
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    MYLog(@"Ending Editing!");
    
    [UIView animateWithDuration:0.3f animations:^{
        CGRect frame = self.view.frame;
        MYLog(@"%@",NSStringFromCGRect(frame));
        frame.origin.y = 64.0;
        self.view.frame = frame;
    }];
    
    return YES;
}


- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
    if ([text isEqualToString:@"\n"]) {
        [textView resignFirstResponder];
//        [_buyInPrice resignFirstResponder];
//        [_buyInNumber resignFirstResponder];
        return NO;
    }
    return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    MYLOGFUN;
    [textField resignFirstResponder];
    return YES;
}

- (void)ClickClear
{
    NSArray *array = self.view.subviews;
    for (id view in array) {
        if ([view isKindOfClass:[CombiInputText class]]) {
             [(CombiInputText *)view setText:@""];
        }
    }
}

- (void)on_net_status_rsp:(int)type nFlag:(int)nFlag
{
    // 底层Connet断开也会回调，上层Reachablity框架也会回调，做下区分
    MYLOGFUN;
    MYLog(@"type=%d,nFlag=%d",type,nFlag);
    if (type == 9) {
        if (nFlag == 2) {
            [self.view makeToast:@"3G网络正在使用!"];
        }else{
            [self.view makeToast:@"3G网络连接失败,请检查设置!"];
        }
    }else if(type == 10){
        if (nFlag == 2) {
            [self.view makeToast:@"WiFi网络正在使用!"];
        }else{
            [self.view makeToast:@"WiFi网络连接失败,请检查设置!"];
        }
    }
}

@end
