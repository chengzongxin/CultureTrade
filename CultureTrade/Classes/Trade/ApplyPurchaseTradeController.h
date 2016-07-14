//
//  ApplyPurchaseTradeController.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SuperViewController.h"
@interface ApplyPurchaseTradeController : SuperViewController <NSTradeEngineDelegate>
//@property (nonatomic,retain) UILabel *codeLabel;
@property (nonatomic,retain) UILabel *code;
//@property (nonatomic,retain) UILabel *nameLabel;
@property (nonatomic,retain) UILabel *name;
//@property (nonatomic,retain) UILabel *singlePriceLabel;
@property (nonatomic,retain) UILabel *singlePrice;
//@property (nonatomic,retain) UILabel *applyVolLabel;
@property (nonatomic,retain) UITextField *applyVol;
//@property (nonatomic,retain) UILabel *maxCanBuyLabel;
@property (nonatomic,retain) UILabel *maxCanBuy;
@property (nonatomic,retain) UIButton *cancel;
@property (nonatomic,retain) UIButton *confirm;
@end
