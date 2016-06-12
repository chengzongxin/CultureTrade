//
//  PriceListCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/3.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "PriceListCell.h"

@implementation PriceListCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    
    CGFloat x = 0;
    CGFloat y = -8;
    CGFloat width = self.frame.size.width / 4;
    CGFloat height = self.frame.size.height;
    
    
    CGRect rect1 = CGRectMake(x, y, width, height);
    _price = [self setCellWithFrame:rect1];
    
    CGRect rect2 = CGRectMake(width, y, width, height);
    _dealPrice = [self setCellWithFrame:rect2];
    
    CGRect rect3 = CGRectMake(width * 2 , 8, width, 12);
    _ratio = [[UIImageView alloc] initWithImage:[UIImage createImageWithColor:[UIColor whiteColor]]];
    _ratio.frame = rect3;
    [self.contentView addSubview:_ratio];
    
    CGRect rect4 = CGRectMake(width * 3, y, width, height);
    _buyRatio = [self setCellWithFrame:rect4];
    
    self.backgroundColor = COLOR_VC_BG;
    
    return self;
}

- (UILabel *)setCellWithFrame:(CGRect)rect
{
    UILabel *label = [[UILabel alloc] initWithFrame:rect];
    [label setTextAlignment:NSTextAlignmentCenter];
    label.lineBreakMode = NSLineBreakByWordWrapping;
    label.textColor = [UIColor whiteColor];
    label.font = [UIFont systemFontOfSize:13];
    [self.contentView addSubview:label];
    return label;
}

@end
