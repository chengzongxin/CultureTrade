//
//  ApplyPurchaseTradeController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyPurchaseTradeController.h"
#import "GlobalModel.h"
@interface ApplyPurchaseTradeController ()

@end

@implementation ApplyPurchaseTradeController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self addView];
    
    [self addButton];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self fillData];
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
    
    
    _code = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y},{width,height}}];
    _name = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height},{width,height}}];
    _singlePrice = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height*2},{width,height}}];
    _applyVol = [[UITextField alloc] initWithFrame:(CGRect){{x + 100,y + height*3},{width,height}}];
    _applyVol.backgroundColor = [UIColor whiteColor];
    _applyVol.keyboardType = UIKeyboardTypeNumberPad;
    _maxCanBuy = [[UILabel alloc] initWithFrame:(CGRect){{x + 100,y + height*4},{width,height}}];
    [self setLabelWithLabel:_code];
    [self setLabelWithLabel:_name];
    [self setLabelWithLabel:_singlePrice];
    [self setLabelWithLabel:_maxCanBuy];
    [self.view addSubview:_code];
    [self.view addSubview:_name];
    [self.view addSubview:_singlePrice];
    [self.view addSubview:_applyVol];
    [self.view addSubview:_maxCanBuy];
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
    _code.text = symbol.productID;
    _name.text = symbol.productName;
    _singlePrice.text = symbol.singlePrice;
    _applyVol.text = @"";
    _maxCanBuy.text = symbol.maxnumAsk;
}

- (void)clickClear
{
    _applyVol.text = @"";
}

- (void)clickConfirm
{
    if (_applyVol.text.length == 0) {
        showAlert(@"请输入申购数量");
        return;// 窗口堆叠导致点不掉
    }
    
    [[NSTradeEngine sharedInstance] trade_open_market_order:_code.text direction:@"7B" price:[_singlePrice.text floatValue] shoushu:[_applyVol.text intValue]];
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
