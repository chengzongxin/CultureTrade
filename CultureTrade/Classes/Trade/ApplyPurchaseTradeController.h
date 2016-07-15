//
//  ApplyPurchaseTradeController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CombiInputText.h"
#import "SuperViewController.h"
#import "UITouchScrollView.h"
#import "TableViewWithBlock.h"
@interface ApplyPurchaseTradeController : SuperViewController <NSTradeEngineDelegate,UITextFieldDelegate>
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
//@property (nonatomic,retain) UILabel *codeLabel;
//@property (nonatomic,retain) UILabel *code;
//@property (nonatomic,retain) UILabel *nameLabel;
//@property (nonatomic,retain) UILabel *name;
//@property (nonatomic,retain) UILabel *singlePriceLabel;
//@property (nonatomic,retain) UILabel *singlePrice;
//@property (nonatomic,retain) UILabel *applyVolLabel;
//@property (nonatomic,retain) UITextField *applyVol;
//@property (nonatomic,retain) UILabel *maxCanBuyLabel;
//@property (nonatomic,retain) UILabel *maxCanBuy;
@property (nonatomic,retain) UIButton *cancel;
@property (nonatomic,retain) UIButton *confirm;
@end
