//
//  ButtonListView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/1.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "ButtonListView.h"
#import "NSTradeEngine.h"
#define Scale_Factory 0.8
#define Scale_Factory_i4 0.8

@implementation ButtonListView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    [self addButton];
    
    return self;
}


- (void)addButton
{
    CGFloat kEdge = 10;
    
    CGFloat x = kEdge;
    CGFloat y = 0;
    
    CGSize size = [UIImage imageNamed:@"buy"].size;
    CGFloat width = size.width;
    CGFloat height = size.height;
    
    if (IS_IPHONE5) {
        MYLog(@"%f",App_Frame_Height);
        width = width * Scale_Factory;
        height = height * Scale_Factory;
    }else if (IS_IPHONE6){
        MYLog(@"%f",App_Frame_Height);
    }else if (IS_IPHONE6PLUS){
        MYLog(@"%f",App_Frame_Height);
        kEdge = 15;
        x = kEdge;
    }else if (IS_IPHONE4){
        MYLog(@"%f",App_Frame_Height);
        width = width * Scale_Factory_i4;
        height = height * Scale_Factory_i4;
    }
    
    CGRect rect1 = CGRectMake(x, y, width, height);
    _buy = [self addButtonWithRect:rect1 Img:@"buy"];
    [_buy addTarget:self action:@selector(buyIn) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:_buy];
    
    CGRect rect2 = CGRectMake(CGRectGetMaxX(_buy.frame) + kEdge, y, width, height);
    _sell = [self addButtonWithRect:rect2 Img:@"sell"];
    [_sell addTarget:self action:@selector(sellOut) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:_sell];
    
    CGRect rect3 = CGRectMake(CGRectGetMaxX(_sell.frame) + kEdge, y, width, height);
    _cancelOrder =  [self addButtonWithRect:rect3 Img:@"cancelOrder"];
    [_cancelOrder addTarget:self action:@selector(deleteOrder) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:_cancelOrder];
    
    CGRect rect4 = CGRectMake(CGRectGetMaxX(_cancelOrder.frame) + kEdge, y, width, height);
    _query =  [self addButtonWithRect:rect4 Img:@"query"];
    [_query addTarget:self action:@selector(queryOrder) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:_query];
    
}

- (UIButton *)addButtonWithRect:(CGRect)rect Img:(NSString *)img
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = rect;
    UIImage *Img = [UIImage imageNamed:img];
    if (IS_IPHONE5) {
        [Img scaleToSize:CGSizeMake(rect.size.width*Scale_Factory, rect.size.height*Scale_Factory)];
    }
    [btn setImage:Img forState:UIControlStateNormal];
    return btn;
}

- (void)buyIn
{
    if ([self.delegate respondsToSelector:@selector(buy)]) {
        [self.delegate buy];
    }
}

- (void)sellOut
{
    if ([self.delegate respondsToSelector:@selector(sell)]) {
        [self.delegate sell];
    }
}

- (void)deleteOrder
{
    if ([self.delegate respondsToSelector:@selector(cancelOrder)]) {
        [self.delegate cancelOrder];
    }
}

- (void)queryOrder
{
    if ([self.delegate respondsToSelector:@selector(queryOrder)]) {
        [self.delegate queryOrder];
    }
}
@end
