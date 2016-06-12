//
//  LeftCell.m
//  CultureTrade
//
//  Created by SZFT1 on 16/4/6.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "LeftCell.h"
#define kGoodsLableRatio 0.2
#define kQuotaoCellYInrect 10

@implementation LeftCell

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
{   // 3 长 3 短
    CGSize frame = self.frame.size;
    
    _productName = [self setCell];
    _productName.frame = (CGRect){{0,0},frame};
    _productName.numberOfLines = 0;
    _productName.font = [UIFont systemFontOfSize:13];
    
    _productID = [self setCell];
    _productID.frame = (CGRect){{0,20},frame};
    _productID.textColor = COLOR_QUOTA_STOCKID_TEXT;
    
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
    frame.size.width = 70;
    
    [super setFrame:frame];
}

- (LeftCell *)initWithSortUnit:(SortUnit *)sortUnit
{
    self.productName.text = [NSString stringWithFormat:@"%@",sortUnit.productName];
    self.productID.text = [NSString stringWithFormat:@"%d",sortUnit.m_CodeInfo.m_uiCode];
    return self;
}
@end
