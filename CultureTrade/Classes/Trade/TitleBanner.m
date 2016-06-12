//
//  TitleBanner.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/16.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TitleBanner.h"
#define kBannerFont [UIFont fontWithName:@"Helvetica-Bold" size:12]

@implementation TitleBanner

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    self.backgroundColor = COLOR_ENTRUST_BANNAR_BG;
    
    return self;
}

- (void)setBannerTitles:(NSArray *)bannerTitles
{
    [self addLablesWithArray:bannerTitles];
}

- (void)addLablesWithArray:(NSArray *)bannerTitles
{
    int count = (int)bannerTitles.count;
    CGFloat labelY = 0;
    CGFloat labelW = self.frame.size.width / count;
    CGFloat labelH = kBannerFont.lineHeight;
    
    for (int i = 0; i < count; i ++) {
        UILabel *label = [self setLableWithRect:CGRectMake(labelW * i, labelY, labelW, labelH)];
        label.text = bannerTitles[i];
        [self addSubview:label];
    }
}

- (UILabel *)setLableWithRect:(CGRect)rect
{
    UILabel *lable = [[UILabel alloc] initWithFrame:rect];
    lable.font = kBannerFont;
    lable.textColor = COLOR_GOLD_THEME;
    lable.textAlignment = NSTextAlignmentCenter;
    return lable;
}

@end
