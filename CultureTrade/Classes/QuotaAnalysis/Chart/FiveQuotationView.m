//
//  FiveQuotationView.m
//  CultureTrade
//
//  Created by SZFT1 on 16/8/25.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "FiveQuotationView.h"
#import "GlobalModel.h"

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
    CGFloat labelWidth = viewWidth / 4;
    CGFloat labelHeight = viewHeight/5;
    UIView *sellView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, viewWidth, viewHeight-5)];
    sellView.layer.borderColor = COLOR_SPLITELINE.CGColor;
    sellView.layer.borderWidth = 0.5f;
    [self setLableWithframe:CGRectMake(5, 0, labelWidth, labelHeight) title:@"卖⑤" color:[UIColor whiteColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*1 , labelWidth, labelHeight) title:@"卖④" color:[UIColor whiteColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*2 , labelWidth, labelHeight) title:@"卖③" color:[UIColor whiteColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*3 , labelWidth, labelHeight) title:@"卖②" color:[UIColor whiteColor] addIn:sellView];
    [self setLableWithframe:CGRectMake(5, viewHeight/5*4 , labelWidth, labelHeight) title:@"卖①" color:[UIColor whiteColor] addIn:sellView];
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth, labelHeight*i, labelWidth*2, labelHeight) color:[UIColor greenColor] addIn:sellView];
        [_sellQuotation addObject:label];
    }
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth*3, labelHeight*i, labelWidth, labelHeight) color:[UIColor whiteColor] addIn:sellView];
        [_sellVolume addObject:label];
    }
    [self addSubview:sellView];
    
    
    UIView *midderBanner = [[UIView alloc] initWithFrame:CGRectMake(0, viewHeight-5, viewWidth,20)];
    [self setMiddleLabelAddIn:midderBanner];
    [self addSubview:midderBanner];
    
    
    UIView *buyView = [[UIView alloc] initWithFrame:CGRectMake(0, viewHeight+20-5, viewWidth, viewHeight-5)];
    buyView.layer.borderColor = COLOR_SPLITELINE.CGColor;
    buyView.layer.borderWidth = 0.5f;
    [self setLableWithframe:CGRectMake(5, 0, labelWidth, labelHeight) title:@"买①" color:[UIColor whiteColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight , labelWidth, labelHeight) title:@"买②" color:[UIColor whiteColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*2 , labelWidth, labelHeight) title:@"买③" color:[UIColor whiteColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*3 , labelWidth, labelHeight) title:@"买④" color:[UIColor whiteColor] addIn:buyView];
    [self setLableWithframe:CGRectMake(5, labelHeight*4 , labelWidth, labelHeight) title:@"买⑤" color:[UIColor whiteColor] addIn:buyView];
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth, labelHeight*i, labelWidth*2, labelHeight) color:[UIColor redColor] addIn:buyView];
        [_buyQuotation addObject:label];
    }
    for (int i = 0; i < 5; i++) {
        UILabel *label = [self setLableWithframe:CGRectMake(5+labelWidth*3, labelHeight*i, labelWidth, labelHeight) color:[UIColor whiteColor] addIn:buyView];
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
    label.textAlignment = NSTextAlignmentCenter;
    [view addSubview:label];
    return label;
}

- (void)setMiddleLabelAddIn:(UIView *)view
{
    CGFloat width = self.frame.size.width/4;
    CGFloat height = self.font.lineHeight;
    CGFloat leftMargin = 3;
    
    UILabel *turnOverPrice = [[UILabel alloc] initWithFrame:CGRectMake(leftMargin, 0, width, height)];
    turnOverPrice.text = @"成交";
    turnOverPrice.font = [UIFont boldSystemFontOfSize:10];
    turnOverPrice.textAlignment = NSTextAlignmentLeft;
    turnOverPrice.textColor = [UIColor whiteColor];
    [view addSubview:turnOverPrice];
    
    _turnOver = [[UILabel alloc] initWithFrame:CGRectMake(width, 0, width, height)];
    _turnOver.font = [UIFont boldSystemFontOfSize:10];
    _turnOver.textAlignment = NSTextAlignmentRight;
    _turnOver.textColor = [UIColor whiteColor];
    [view addSubview:_turnOver];
    
    UILabel *currentVolume = [[UILabel alloc] initWithFrame:CGRectMake(leftMargin + width * 2, 0, width, height)];
    currentVolume.text = @"现量";
    currentVolume.font = [UIFont boldSystemFontOfSize:10];
    currentVolume.textAlignment = NSTextAlignmentLeft;
    currentVolume.textColor = [UIColor whiteColor];
    [view addSubview:currentVolume];
    
    _curVolume = [[UILabel alloc] initWithFrame:CGRectMake(width * 3 - leftMargin, 0, width, height)];
    _curVolume.font = [UIFont boldSystemFontOfSize:10];
    _curVolume.textAlignment = NSTextAlignmentRight;
    _curVolume.textColor = [UIColor whiteColor];
    [view addSubview:_curVolume];
}

- (void)setBuyQuotation:(NSMutableArray *)buyQuotation
{
    for (int i = 0; i < _buyQuotation.count; i++) {
        ((UILabel *)_buyQuotation[i]).text = buyQuotation[i];
        if ([((UILabel *)_buyQuotation[i]).text floatValue] > GLOBAL.sortUnit.m_uiPreClose) {  // 大于昨收，红色
            ((UILabel *)_buyQuotation[i]).textColor = [UIColor redColor];
        }else{
            if ([((UILabel *)_buyQuotation[i]).text floatValue] == 0) {  // 小于昨收，当为0时白色，value = --
                ((UILabel *)_buyQuotation[i]).textColor = [UIColor whiteColor];
                ((UILabel *)_buyQuotation[i]).text = @"--";
            }else{                                                                  // 不为0时，绿色
                ((UILabel *)_buyQuotation[i]).textColor = [UIColor greenColor];
            }
        }
    }
}
- (void)setSellQuotation:(NSMutableArray *)sellQuotation
{
    for (int i = 0; i < _sellQuotation.count; i++) {
        ((UILabel *)_sellQuotation[i]).text = sellQuotation[i];
        if ([((UILabel *)_sellQuotation[i]).text floatValue] > GLOBAL.sortUnit.m_uiPreClose) {  // 大于昨收，红色
            ((UILabel *)_sellQuotation[i]).textColor = [UIColor redColor];
        }else{
            if ([((UILabel *)_sellQuotation[i]).text floatValue] == 0) {  // 小于昨收，当为0时白色，value = --
                ((UILabel *)_sellQuotation[i]).textColor = [UIColor whiteColor];
                ((UILabel *)_sellQuotation[i]).text = @"--";
            }else{                                                                  // 不为0时，绿色
                ((UILabel *)_sellQuotation[i]).textColor = [UIColor greenColor];
            }
        }
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

- (void)setMidBanner
{
    self.turnOver.text = [NSString stringWithFormat:@"%0.2f",GLOBAL.nowData.m_uiNewPrice];
    if (GLOBAL.nowData.m_uiNewPrice > GLOBAL.sortUnit.m_uiPreClose) {  // 大于昨收，红色
        self.turnOver.textColor = [UIColor redColor];
    }else{
        if (GLOBAL.nowData.m_uiNewPrice == 0) {  // 小于昨收，当为0时白色，value = --
            self.turnOver.textColor = [UIColor whiteColor];
            self.turnOver.text = @"--";
        }else{                                                                  // 不为0时，绿色
            self.turnOver.textColor = [UIColor greenColor];
        }
    }
    self.curVolume.text = [NSString stringWithFormat:@"%d",GLOBAL.nowData.m_uiCurVol];
}

@end
