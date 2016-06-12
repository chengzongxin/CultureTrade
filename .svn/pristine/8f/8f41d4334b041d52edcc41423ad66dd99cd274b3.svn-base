//
//  BuyInCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/7.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BuyInCell.h"

@implementation BuyInCell

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
    
}

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
    CGFloat width = self.frame.size.width / 7 ;
    CGFloat middleSpace = self.frame.size.width / 7;
    CGSize size = (CGSize){width,self.frame.size.height};
    
    _buy = [self setCell];
    _buy.frame = (CGRect){{0,0},size};
    
    _buyPrice = [self setCell];
    _buyPrice.frame = (CGRect){{width,0},size};
    
    _buyShouShu = [self setCell];
    _buyShouShu.frame = (CGRect){{width * 2,0},size};
    
    _sell = [self setCell];
    _sell.frame = (CGRect){{width * 3 + middleSpace,0},size};
    
    _sellPrice = [self setCell];
    _sellPrice.frame = (CGRect){{width * 4 + middleSpace,0},size};
    
    _sellShouShu = [self setCell];
    _sellShouShu.frame = (CGRect){{width * 5 + middleSpace,0},size};
}

- (UILabel *)setCell
{
    UILabel *lable = [[UILabel alloc] init];
    lable.font = [UIFont systemFontOfSize:12];
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
