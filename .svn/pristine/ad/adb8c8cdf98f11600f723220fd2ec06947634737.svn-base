//
//  MeGuideView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/3.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "MeGuideView.h"
#import "SpliteLineView.h"
@implementation MeGuideView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initalization];
    return self;
}

- (void)initalization
{
    [self addLables];
    [self addSpliteLine];
}

- (void)addLables
{
    
    self.backgroundColor = COLOR_ME_GUIDE_BG;
    
    CGFloat width = self.frame.size.width  / 3;
    CGFloat height = self.frame.size.height / 2;
    CGFloat lineHeight = FONT_ME_GUIDE_TEXT.lineHeight;
    CGFloat margin = 20;
    CGFloat interval = 5;
    
    _foundTag = [self addLablesWith:CGRectMake(0, margin, width, lineHeight)];
    _foundTag.text = LocalizedStringByInt(1403);
    _foundTag.font = FONT_ME_GUIDE_TAG;
    
    _foundLable = [self addLablesWith:CGRectMake(0, CGRectGetMaxY(_foundTag.frame) + interval, width, lineHeight)];
    _foundLable.text = @"--";
    
    _monthDealTag = [self addLablesWith:CGRectMake(width, margin, width, lineHeight)];
    _monthDealTag.text = LocalizedStringByInt(1404);
    _monthDealTag.font = FONT_ME_GUIDE_TAG;
    
    _monthDealLable = [self addLablesWith:CGRectMake(width, CGRectGetMaxY(_monthDealTag.frame) + interval, width, lineHeight)];
    _monthDealLable.text = @"--";
    
    _sucRatioTag = [self addLablesWith:CGRectMake(width * 2, margin, width, lineHeight)];
    _sucRatioTag.text = LocalizedStringByInt(1405);
    _sucRatioTag.font = FONT_ME_GUIDE_TAG;
    
    _sucRatioLabel = [self addLablesWith:CGRectMake(width * 2, CGRectGetMaxY(_sucRatioTag.frame) + interval, width, lineHeight)];
    _sucRatioLabel.text = @"--";
    
    _totalGainTag = [self addLablesWith:CGRectMake(0, height + margin, width, lineHeight)];
    _totalGainTag.text = LocalizedStringByInt(1406);
    _totalGainTag.font = FONT_ME_GUIDE_TAG;
    
    _totalGainLable = [self addLablesWith:CGRectMake(0, CGRectGetMaxY(_totalGainTag.frame) + interval, width, lineHeight)];
    _totalGainLable.text = @"--";
    [self changeColor:_totalGainLable];
    
    _thisMonthGainTag = [self addLablesWith:CGRectMake(width, height + margin, width, lineHeight)];
    _thisMonthGainTag.text = LocalizedStringByInt(1407);
    _thisMonthGainTag.font = FONT_ME_GUIDE_TAG;
    
    _thisMonthGainLable = [self addLablesWith:CGRectMake(width, CGRectGetMaxY(_thisMonthGainTag.frame) + interval, width, lineHeight)];
    _thisMonthGainLable.text = @"--";
    [self changeColor:_thisMonthGainLable];
    
    _thisWeekGainTag = [self addLablesWith:CGRectMake(width * 2, height + margin, width, lineHeight)];
    _thisWeekGainTag.text = LocalizedStringByInt(1408);
    _thisWeekGainTag.font = FONT_ME_GUIDE_TAG;
    
    _thisWeekGainLabel = [self addLablesWith:CGRectMake(width * 2, CGRectGetMaxY(_thisWeekGainTag.frame) + interval, width, lineHeight)];
    _thisWeekGainLabel.text = @"--";
    [self changeColor:_thisWeekGainLabel];
}

- (UILabel *)addLablesWith:(CGRect)rect
{
    UILabel *lable = [[UILabel alloc] initWithFrame:rect];
    lable.textAlignment = NSTextAlignmentCenter;
    lable.textColor = COLOR_ME_GUIDE_TEXT;
    lable.font = FONT_ME_GUIDE_TEXT;
    [self addSubview:lable];
    return lable;
}

- (void)changeColor:(UILabel *)label
{
    if ([label.text floatValue] > 0) {
        label.textColor = [UIColor redColor];
    }else{
        label.textColor = [UIColor greenColor];
    }
}

- (void)addSpliteLine
{
    float x = self.frame.size.width / 3;
    float y = self.frame.size.height / 2;
    
    float width = self.frame.size.width;
    float height = self.frame.size.height;
    float lineWith = 0.5f;
    SpliteLineView *spliteLineVer1 = [[SpliteLineView alloc] init];
    spliteLineVer1.frame = CGRectMake(x, 0, lineWith, height);
    [self addSubview:spliteLineVer1];
    
    SpliteLineView *spliteLineVer2 = [[SpliteLineView alloc] init];
    spliteLineVer2.frame = CGRectMake(x * 2, 0, lineWith, height);
    [self addSubview:spliteLineVer2];
    
    SpliteLineView *spliteLineHor4 = [[SpliteLineView alloc] init];
    spliteLineHor4.frame = CGRectMake(0, y, width, lineWith);
    [self addSubview:spliteLineHor4];
    
//    SpliteLineView *spliteLineHor5 = [[SpliteLineView alloc] init];
//    spliteLineHor5.frame = CGRectMake(0, y * 2, width, lineWith);
//    [self addSubview:spliteLineHor5];
}





@end
