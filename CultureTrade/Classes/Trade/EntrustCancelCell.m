//
//  EntrustCancelCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "EntrustCancelCell.h"

@implementation EntrustCancelCell


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.backgroundColor = COLOR_QUOTA_CELL_BG;
        [self addLables];
    }
    return self;
}


- (void)addLables
{
    CGFloat labelY = 10;
    CGFloat width = self.frame.size.width/6;
    CGFloat height = kCellTextFont.lineHeight;
    
    _stockName = [self setCellWithFrame:CGRectMake(0, 3, width, height)];
    _stockID = [self setCellWithFrame:CGRectMake(0, height+3, width, height)];
    _stockName.numberOfLines = 0;
    _entrustPrice = [self setCellWithFrame:CGRectMake(width, labelY, width, height)];
    _entrustDeal = [self setCellWithFrame:CGRectMake(width * 2, labelY, width, height)];
    _status = [self setCellWithFrame:CGRectMake(width * 3, labelY, width, height)];
    _buySell = [self setCellWithFrame:CGRectMake(width * 4, labelY, width, height)];
    _nowPrice = [self setCellWithFrame:CGRectMake(width * 5, labelY, width, height)];
}

- (UILabel *)setCellWithFrame:(CGRect)frame
{
    UILabel *lable = [[UILabel alloc] initWithFrame:frame];
    lable.font = kCellTextFont;
    lable.lineBreakMode = NSLineBreakByWordWrapping;
    [lable setTextAlignment:NSTextAlignmentCenter];
    lable.textColor = [UIColor whiteColor];
    [self.contentView addSubview:lable];
    return lable;
}

- (void)setFrame:(CGRect)frame
{
    frame.size.width = ScreenSize.width;
    
    [super setFrame:frame];
}


@end
