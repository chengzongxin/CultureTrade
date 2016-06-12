//
//  ModifyPasswordController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/3/17.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "SuperViewController.h"
#import "BuyTextField.h"
#import "BaseTextField.h"
#import "UITouchScrollView.h"
#import "CombiInputText.h"
typedef enum : NSUInteger {
    isMainPassword =  1,
    isMoneyPassword = 2,
    isPhonePassword = 3,
} PasswordType;


@interface ModifyPasswordController : SuperViewController<UITextFieldDelegate,UIScrollViewDelegate,NSTradeEngineDelegate>
{
    CombiInputText *_passwordType;
    CombiInputText *_oldPassword;
    CombiInputText *_newPassword;
    UITouchScrollView *_scroll;
    UIPickerView *_pickView;
}

@property (nonatomic,retain) UIPickerView *pickView;
@end
