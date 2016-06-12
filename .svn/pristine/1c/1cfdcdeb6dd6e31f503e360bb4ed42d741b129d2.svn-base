//
//  InOutMoneyController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/3/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "SuperViewController.h"
#import "BuyTextField.h"
#import "BaseTextField.h"
#import "UITouchScrollView.h"
#import "CombiInputText.h"

@interface InOutMoneyController : SuperViewController<UITextFieldDelegate,UIScrollViewDelegate,UIPickerViewDelegate,UIPickerViewDataSource,NSTradeEngineDelegate>
{
    CombiInputText *_transferMoneyType;
    CombiInputText *_transferBank;
    CombiInputText *_transferMoney;
    CombiInputText *_fundPassword;
    CombiInputText *_bankPassword;
    UITouchScrollView *_scroll;
    UIButton *_downList;
    NSTimer *mTimer;
    NSThread *mThread;
}

@property (nonatomic,retain) CombiInputText *transferMoneyType;
@property (nonatomic,retain) CombiInputText *transferBank;
@property (nonatomic,retain) CombiInputText *transferMoney;
@property (nonatomic,retain) CombiInputText *fundPassword;
@property (nonatomic,retain) CombiInputText *bankPassword;
@property (nonatomic,retain) UIButton *downList;
@property (nonatomic,retain) UIPickerView *transferMoneyTypePickView;
@property (nonatomic,retain) UIPickerView *transferBankPickView;
@end
