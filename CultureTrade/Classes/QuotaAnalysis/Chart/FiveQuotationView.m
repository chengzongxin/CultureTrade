//
//  FiveQuotationView.m
//  CultureTrade
//
//  Created by SZFT1 on 16/8/25.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "FiveQuotationView.h"

@implementation FiveQuotationView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    _sellQuotation = [NSMutableArray array];
    _sellVolume = [NSMutableArray array];
    _buyQuotation = [NSMutableArray array];
    _buyVolume = [NSMutableArray array];
    self.font = [UIFont boldSystemFontOfSize:12];
    [self addFiveQuotation];
    return self;
}

- (void)addFiveQuotation
{
    CGFloat viewWidth = self.frame.size.width;
    CGFloat viewHeight = self.frame.size.height * 0.45;
    CGFloat labelWidth = 40;
    CGFloat labelHeight = viewHeight/5;
    UIView *sellView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, viewWidth, viewHeight-5)];
    sellView.layer.borderColor = COLOR_SPLITELINE.CGColor;
    sellView.layer.borderWidth = 0.5f;
    [self setLableWithframe:CGRectMake(5, 0, labelWidth, labelHeight) title:@"卖⑤" color:[UIColor greenColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*1 , labelWidth, labelHeight) title:@"卖④" color:[UIColor greenColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*2 , labelWidth, labelHeight) title:@"卖③" color:[UIColor greenColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*3 , labelWidth, labelHeight) title:@"卖②" color:[UIColor greenColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*4 , labelWidth, labelHeight) title:@"卖①" color:[UIColor greenColor] addIn:sellView];
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth, labelHeight*i, labelWidth, labelHeight) color:[UIColor greenColor] addIn:sellView];
        [_sellQuotation addObject:label];
    }
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth*2, labelHeight*i, labelWidth, labelHeight) color:[UIColor greenColor] addIn:sellView];
        [_sellVolume addObject:label];
    }
    [self addSubview:sellView];
    
    
    UIView *midderBanner = [[UIView alloc] initWithFrame:CGRectMake(0, viewHeight-5, viewWidth,20)];
    [self setMiddleLabelAddIn:midderBanner];
    [self addSubview:midderBanner];
    
    
    UIView *buyView = [[UIView alloc] initWithFrame:CGRectMake(0, viewHeight+20-5, viewWidth, viewHeight-5)];
    buyView.layer.borderColor = COLOR_SPLITELINE.CGColor;
    buyView.layer.borderWidth = 0.5f;
    [self setLableWithframe:CGRectMake(5, 0, labelWidth, labelHeight) title:@"买①" color:[UIColor redColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight , labelWidth, labelHeight) title:@"买②" color:[UIColor redColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*2 , labelWidth, labelHeight) title:@"买③" color:[UIColor redColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*3 , labelWidth, labelHeight) title:@"买④" color:[UIColor redColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*4 , labelWidth, labelHeight) title:@"买⑤" color:[UIColor redColor] addIn:buyView];
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth, labelHeight*i, labelWidth, labelHeight) color:[UIColor redColor] addIn:buyView];
        [_buyQuotation addObject:label];
    }
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth*2, labelHeight*i, labelWidth, labelHeight) color:[UIColor redColor] addIn:buyView];
        [_buyVolume addObject:label];
    }
    
    [self addSubview:buyView];
}

- (void)setLableWithframe:(CGRect)frame title:(NSString *)title color:(UIColor *)color addIn:(UIView *)view
{
    UILabel *label = [[UILabel alloc] initWithFrame:frame];
    label.font = self.font;
    label.text = title;
    label.textColor = color;
    [view addSubview:label];
}

- (UILabel *)setLableWithframe:(CGRect)frame color:(UIColor *)color addIn:(UIView *)view
{
    UILabel *label = [[UILabel alloc] initWithFrame:frame];
    label.font = self.font;
    label.textColor = color;
    [view addSubview:label];
    return label;
}

- (void)setMiddleLabelAddIn:(UIView *)view
{
    UILabel *turnOverPrice = [[UILabel alloc] initWithFrame:CGRectMake(5, 0, 40, 20)];
    turnOverPrice.text = @"成交";
    turnOverPrice.font = self.font;
    turnOverPrice.textColor = [UIColor whiteColor];
    [view addSubview:turnOverPrice];
    
    UILabel *currentVolume = [[UILabel alloc] initWithFrame:CGRectMake(45, 0, 40, 20)];
    currentVolume.text = @"现手";
    currentVolume.font = self.font;
    currentVolume.textColor = [UIColor whiteColor];
    [view addSubview:currentVolume];
}

- (void)setBuyQuotation:(NSMutableArray *)buyQuotation
{
    for (int i = 0; i < _buyQuotation.count; i++) {
        ((UILabel *)_buyQuotation[i]).text = buyQuotation[i];
    }
}
- (void)setSellQuotation:(NSMutableArray *)sellQuotation
{
    for (int i = 0; i < _sellQuotation.count; i++) {
        ((UILabel *)_sellQuotation[i]).text = sellQuotation[i];
    }
}
- (void)setBuyVolume:(NSMutableArray *)buyVolume
{
    for (int i = 0; i < _buyVolume.count; i++) {
        ((UILabel *)_buyVolume[i]).text = buyVolume[i];
    }
}
- (void)setSellVolume:(NSMutableArray *)sellVolume
{
    for (int i = 0; i < _sellVolume.count; i++) {
        ((UILabel *)_sellVolume[i]).text = sellVolume[i];
    }
}


@end
