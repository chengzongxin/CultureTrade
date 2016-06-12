//
//  DealDetailCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/3.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "DealDetailCell.h"

@implementation DealDetailCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    
    CGFloat x = 0;
    CGFloat y = -8;
    CGFloat width = self.frame.size.width / 4;
    CGFloat height = self.frame.size.height;
    
    CGRect rect1 = CGRectMake(x, y, width, height);
    _fenbi = [self setCellWithFrame:rect1];
   
    CGRect rect2 = CGRectMake(width, y, width, height);
    _price = [self setCellWithFrame:rect2];

    CGFloat widthSmall = self.frame.size.width / 6;
    
    CGRect rect3 = CGRectMake(width * 2, y, widthSmall, height);
    _xianshou = [self setCellWithFrame:rect3];

    CGRect rect4 = CGRectMake(CGRectGetMaxX(_xianshou.frame), y, widthSmall, height);
    _open = [self setCellWithFrame:rect4];

    CGRect rect5 = CGRectMake(CGRectGetMaxX(_open.frame), y, widthSmall, height);
    _ping = [self setCellWithFrame:rect5];

    self.backgroundColor = COLOR_VC_BG;
    
    return self;
}

- (UILabel *)setCellWithFrame:(CGRect)rect
{
    UILabel *label = [[UILabel alloc] initWithFrame:rect];
    [label setTextAlignment:NSTextAlignmentCenter];
//    label.lineBreakMode = 0;
    label.textColor = [UIColor whiteColor];
    label.font = [UIFont systemFontOfSize:13];
    [self.contentView addSubview:label];
    return label;
}

@end
