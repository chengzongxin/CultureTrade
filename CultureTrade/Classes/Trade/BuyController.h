//
//  BuyController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/4.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SuperViewController.h"
#import "BaseViewController.h"
#import "CombiInputText.h"
#import "UITouchScrollView.h"
#import "TableViewWithBlock.h"
@interface BuyController : SuperViewController <UITableViewDataSource,UITableViewDelegate,NSTradeEngineDelegate,UITextFieldDelegate,UIScrollViewTouchesDelegate>
{
    CombiInputText *_productID;
    CombiInputText *_productName;
    CombiInputText *_quoteType;
    CombiInputText *_buyInPrice;
    CombiInputText *_buyInNumber;
    CombiInputText *_maxCanBuy;
    UITouchScrollView *_scroll;
    TableViewWithBlock *_tableBlock;
    
    NSTimer *mTimer;
    NSThread *mThread;
    BOOL isOpened;
    int index;
}

@property (nonatomic,retain) CombiInputText *productID;
@property (nonatomic,retain) CombiInputText *productName;
@property (nonatomic,retain) CombiInputText *quoteType;
@property (nonatomic,retain) CombiInputText *buyInPrice;
@property (nonatomic,retain) CombiInputText *buyInNumber;
@property (nonatomic,retain) CombiInputText *maxCanBuy;
@property (nonatomic,retain) UITableView *tableView;
@property (nonatomic,retain) TableViewWithBlock *tableBlock;
@property (retain, nonatomic) UIButton *openButton;

- (void)addTitleView;
- (void)addTextFied;
- (void)moniterTextField;
- (void)addDownList;
- (void)fillData;
- (void)loadData;
- (void)startTimer;
- (void)stopTimer;
- (void)requestPreTrade;
- (void)ClickClear;
@end
