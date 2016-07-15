//
//  ApplyPurchaseTradeController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyPurchaseTradeController.h"
#import "GlobalModel.h"
#define kCellHeight 36
#define kTextFieldMargin 15
#define TextFiledX 10
#define TextFiledY 5
#define TextFiledW ScreenSize.width - 2 * TextFiledX
#define TextFiledH 35
@interface ApplyPurchaseTradeController ()

@end

@implementation ApplyPurchaseTradeController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
//    [self addView];
    
    [self addTextFied];
    
    [self addButton];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self fillData];
}

- (void)addTextFied
{
    _productID = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin, TextFiledW, TextFiledH)];
    _productID.delegate = self;
    _productID.leftLabel.text = LocalizedStringByInt(2701);
    _productID.font = FONT_BUYIN_TEXTFIELD;
//    _productID.style = CombiInputTextList;
    _productID.userInteractionEnabled = NO;
    [self.view addSubview:_productID];
//    [_productID.downListButton addTarget:self action:@selector(changeOpenStatus:) forControlEvents:UIControlEventTouchUpInside];
    
    _productName = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + TextFiledH + TextFiledY, TextFiledW, TextFiledH)];
    _productName.delegate = self;
    _productName.leftLabel.text = LocalizedStringByInt(2702);
    _productName.font = FONT_BUYIN_TEXTFIELD;
//    _productName.style = CombiInputTextList;
    _productName.userInteractionEnabled = NO;
    [self.view addSubview:_productName];
//    [_productName.downListButton addTarget:self action:@selector(changeOpenStatus:) forControlEvents:UIControlEventTouchUpInside];
    
    _buyInPrice = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*2, TextFiledW, TextFiledH)];
    _buyInPrice.delegate = self;
    _buyInPrice.leftLabel.text = LocalizedStringByInt(2703);
    _buyInPrice.font = FONT_BUYIN_TEXTFIELD;
    _buyInPrice.keyboardType = UIKeyboardTypeDecimalPad;
    _buyInPrice.userInteractionEnabled = NO;
    [self.view addSubview:_buyInPrice];
    
    _buyInNumber = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*3, TextFiledW, TextFiledH)];
    _buyInNumber.delegate = self;
    _buyInNumber.leftLabel.text = LocalizedStringByInt(2704);
    _buyInNumber.font = FONT_BUYIN_TEXTFIELD;
    _buyInNumber.keyboardType = UIKeyboardTypeNumberPad;
    [self.view addSubview:_buyInNumber];
    
    _maxCanBuy = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*4, TextFiledW, TextFiledH)];
    _maxCanBuy.leftLabel.text = LocalizedStringByInt(2705);
    _maxCanBuy.font = FONT_BUYIN_TEXTFIELD;
    _maxCanBuy.userInteractionEnabled = NO;
    [self.view addSubview:_maxCanBuy];
}

- (void)addView
{
    CGFloat x = 60;
    CGFloat y = 100;
    CGFloat width = 80;
    CGFloat height = 44;
    for (int i = 0; i < 5; i++) {
        UILabel *lable = [[UILabel alloc] initWithFrame:(CGRect){{x,y + height*i},{width,height}}];
        lable.text = LocalizedStringByInt(2701+i);
        [self setLabelWithLabel:lable];
        [self.view addSubview:lable];
    }
    
    
//    _code = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y},{width,height}}];
//    _name = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height},{width,height}}];
//    _singlePrice = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height*2},{width,height}}];
//    _applyVol = [[UITextField alloc] initWithFrame:(CGRect){{x + 100,y + height*3},{width,height}}];
//    _applyVol.backgroundColor = [UIColor whiteColor];
//    _applyVol.keyboardType = UIKeyboardTypeNumberPad;
////    _maxCanBuy = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height*4},{width,height}}];
//    [self setLabelWithLabel:_code];
//    [self setLabelWithLabel:_name];
//    [self setLabelWithLabel:_singlePrice];
////    [self setLabelWithLabel:_maxCanBuy];
//    [self.view addSubview:_code];
//    [self.view addSubview:_name];
//    [self.view addSubview:_singlePrice];
//    [self.view addSubview:_applyVol];
//    [self.view addSubview:_maxCanBuy];
//
//    _cancel = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//    _cancel.frame = (CGRect){{50,CGRectGetMaxY(_maxCanBuy.frame)},{150,60}};
//    [_cancel setTitle:LocalizedStringByInt(2706) forState:UIControlStateNormal];
//    [_cancel addTarget:self action:@selector(clickClear) forControlEvents:UIControlEventTouchUpInside];
//    [self.view addSubview:_cancel];
//    
//    _confirm = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//    _confirm.frame = (CGRect){{200,CGRectGetMaxY(_maxCanBuy.frame)},{150,60}};
//    [_confirm setTitle:LocalizedStringByInt(2707) forState:UIControlStateNormal];
//    [_confirm addTarget:self action:@selector(clickConfirm) forControlEvents:UIControlEventTouchUpInside];
//    [self.view addSubview:_confirm];
}

- (void)addButton
{
    CGSize size = [UIImage imageNamed:@"confirm"].size;
    CGFloat btnX = (self.view.frame.size.width - size.width * 2 - 10)/2;
    CGFloat btnY = CGRectGetMaxY(_maxCanBuy.frame)+30;
    
    CGRect clearRect = (CGRect){{btnX,btnY},size};
    UIButton *clearBtn =  [self addButtonWithRect:clearRect Img:@"clear"];
    [clearBtn addTarget:self action:@selector(clickClear) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:clearBtn];
    
    CGRect confirmRect = (CGRect){{CGRectGetMaxX(clearBtn.frame)+10,btnY},size};
    UIButton *confirmBtn =  [self addButtonWithRect:confirmRect Img:@"confirm"];
    [confirmBtn addTarget:self action:@selector(clickConfirm) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:confirmBtn];
    
}

- (UIButton *)addButtonWithRect:(CGRect)rect Img:(NSString *)img
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = rect;
    UIImage *Img = [UIImage imageNamed:img];
    [btn setImage:Img forState:UIControlStateNormal];
    return btn;
}

- (void)setLabelWithLabel:(UILabel *)label
{
    label.textColor = [UIColor whiteColor];
}

- (void)fillData
{
    SymbolModel *symbol = GLOBAL.symbolModel;
    _productID.text = symbol.productID;
    _productName.text = symbol.productName;
    _buyInPrice.text = symbol.singlePrice;
    _buyInNumber.text = @"";
    _maxCanBuy.text = symbol.maxnumAsk;
    
//    _code.text = symbol.productID;
//    _name.text = symbol.productName;
//    _singlePrice.text = symbol.singlePrice;
//    _applyVol.text = @"";
//    _maxCanBuy.text = symbol.maxnumAsk;
}

- (void)clickClear
{
    _buyInNumber.text = @"";
}

- (void)clickConfirm
{
    if (_buyInNumber.text.length == 0) {
        showAlert(@"请输入申购数量");
        return;// 窗口堆叠导致点不掉
    }
    
    [[NSTradeEngine sharedInstance] trade_open_market_order:_productID.text direction:@"7B" price:[_buyInPrice.text floatValue] shoushu:[_buyInNumber.text intValue]];
    [NSTradeEngine sharedInstance].delegate = self;
    [self showProgress:@"Loading..."];
}

- (void)trade_ui_openorder_rsp:(int)nSeq nRet:(int)nRet orderticket:(NSString *)orderticket{
    [super dissmissProgress];
    if (nRet > 0) {
        BasicAlertView *alertView = [BasicAlertView alertViewWithTitle:@"下单成功"
                                                              subtitle:@""
                                                               message:nil
                                                               buttons:[NSArray arrayWithObject:@"confirm"]
                                                          afterDismiss:^(int buttonIndex) {
                                                              MYLog(@"%d",buttonIndex);
                                                          }];
        [alertView show];
    }else{
        BasicAlertView *alertView = [BasicAlertView alertViewWithTitle:LocalizedStringByInt(nRet)
                                                              subtitle:@""
                                                               message:nil
                                                               buttons:[NSArray arrayWithObject:@"confirm"]
                                                          afterDismiss:^(int buttonIndex) {
                                                              MYLog(@"%d",buttonIndex);
                                                          }];
        [alertView show];
    }
    
}

@end
