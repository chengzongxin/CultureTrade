//
//  GuideView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "GuideView.h"
#import "GlobalModel.h"
#import "SelectPrefer.h"
#define kXMargin 20
#define kYMargin 5

@implementation GuideView

//singleton_implementation(GuideView)

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.backgroundColor = COLOR_VC_BG;
    [SelectPrefer checkLocalFile];
    [self showLable];
    
    return self;
}

- (void)showLable
{
    CGFloat width = self.frame.size.width / 3;
    CGFloat height = self.frame.size.height / 3;
    
    _price = [self builtLabelWithRect:CGRectMake(kXMargin, kYMargin, width, height * 2)];
    _price.font = [UIFont systemFontOfSize:30];
    
    _increase = [self builtLabelWithRect:CGRectMake(kXMargin, height * 2 - kYMargin, width / 2, height)];
    
    _increasePercent = [self builtLabelWithRect:CGRectMake(width / 2, height * 2 - kYMargin, width / 2, height)];
    
    _todayOpen = [self builtLabelWithRect:CGRectMake(width, kYMargin, width, height)];
    
    _yesterdayClose = [self builtLabelWithRect:CGRectMake(width * 2 - kXMargin / 2, kYMargin, width, height)];
    
    _highest = [self builtLabelWithRect:CGRectMake(width, height, width, height)];
    
    _lowest = [self builtLabelWithRect:CGRectMake(width * 2 - kXMargin / 2, height, width, height)];
    
    _dealVolume = [self builtLabelWithRect:CGRectMake(width, height * 2 - kYMargin, width, height)];
    
    _dealTurnover = [self builtLabelWithRect:CGRectMake(width * 2 - kXMargin / 2,height * 2 - kYMargin,width,height)];

    _plus = [UIButton buttonWithType:UIButtonTypeCustom];
    CGSize size = [UIImage imageNamed:@"addSelect"].size;
    _plus.frame = CGRectMake(self.frame.size.width - size.width -8 , 2,size.width,size.height);
//    [_plus setImage:[UIImage imageNamed:@"addSelect"] forState:UIControlStateNormal];
//    [_plus setImage:[UIImage imageNamed:@"delSelect"] forState:UIControlStateHighlighted];
    [self updateAddSelect];
    [_plus addTarget:self action:@selector(addSelect:) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:_plus];

    
}

- (void)updateAddSelect
{
    NSString *productId = [NSString stringWithFormat:@"%d",GLOBAL.sortUnit.m_CodeInfo.m_uiCode];
    BOOL value = [[SelectPrefer readFromFileForKey:productId] boolValue];
    if (value) {
        [_plus setImage:[UIImage imageNamed:@"delSelect"] forState:UIControlStateNormal];
    }else{
        [_plus setImage:[UIImage imageNamed:@"addSelect"] forState:UIControlStateNormal];
    }
}

- (void)addSelect:(UIButton *)btn
{
    NSString *productId = [NSString stringWithFormat:@"%d",GLOBAL.sortUnit.m_CodeInfo.m_uiCode];
    
    BOOL value = [[SelectPrefer readFromFileForKey:productId] boolValue];
    [SelectPrefer writeToFileForKey:productId value:[NSString stringWithFormat:@"%d",!value]];
    [self updateAddSelect];
    MYLOGFUN;
}


- (UILabel *)builtLabelWithRect:(CGRect)rect
{
    UILabel *lable = [[UILabel alloc] initWithFrame:CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height)];
    lable.textColor = [UIColor whiteColor];
    lable.textAlignment = NSTextAlignmentJustified;
    [lable setLineBreakMode:NSLineBreakByCharWrapping];
//    [lable sizeToFit];
    lable.font = [UIFont systemFontOfSize:11];
    [self addSubview:lable];
    return lable;
}

- (void)fillColor
{
    if (_highest.text) {
        NSMutableAttributedString *mAttrStr = [[NSMutableAttributedString alloc] initWithString:_highest.text];
        NSRange range = NSMakeRange(3, _highest.text.length - 3);
        [mAttrStr addAttribute:NSForegroundColorAttributeName value:[UIColor redColor] range:range];
        [_highest setAttributedText:mAttrStr];
    }
    
    if (_lowest.text) {
        NSMutableAttributedString *mAttrStr2 = [[NSMutableAttributedString alloc] initWithString:_lowest.text];
        NSRange range2 = NSMakeRange(3, _lowest.text.length - 3);
        [mAttrStr2 addAttribute:NSForegroundColorAttributeName value:[UIColor greenColor] range:range2];
        [_lowest setAttributedText:mAttrStr2];
    }
    
    if ([_increase.text floatValue] > 0) {  // 红色
        _price.textColor = [UIColor redColor];
        _increase.textColor = [UIColor redColor];
        _increasePercent.textColor = [UIColor redColor];
    }else if ([_increase.text floatValue] < 0){                                      // 绿色
        _price.textColor = [UIColor greenColor];
        _increase.textColor = [UIColor greenColor];
        _increasePercent.textColor = [UIColor greenColor];
    }else if ([_increase.text floatValue] == 0){   // 白色
        _price.textColor = [UIColor whiteColor];
        _increase.textColor = [UIColor whiteColor];
        _increasePercent.textColor = [UIColor whiteColor];
        _highest.textColor = [UIColor whiteColor];
        _lowest.textColor = [UIColor whiteColor];
    }
}

@end
