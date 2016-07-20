//
//  ApplyEntrustCell.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/20.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyEntrustCell.h"
#define kGoodsLableRatio 0.2
#define kQuotaoCellYInrect 10
#define kTableViewWidthRatio 2.2
@implementation ApplyEntrustCell

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.backgroundColor = COLOR_APPLYPURCHASE_CELL_BG;
        
    }
    return self;
}

- (void)setLabelCount:(int)labelCount
{
    [self addLables:labelCount];
}

- (void)addLables:(int)count
{   // 3 长 3 短
    CGFloat width = self.frame.size.width / count;
    CGFloat height = self.frame.size.height;
    
    for (int i = 0; i < count; i ++) {
        [self setCellWithRect:(CGRect){{width * i,10},{width,height}}];
    }
    
}

- (UILabel *)setCellWithRect:(CGRect)frame
{
    UILabel *lable = [[UILabel alloc] init];
    lable.frame = frame;
    lable.font = [UIFont systemFontOfSize:12];
    lable.lineBreakMode = NSLineBreakByWordWrapping;
    [lable setTextAlignment:NSTextAlignmentCenter];
    lable.textColor = [UIColor whiteColor];
    [self.contentView addSubview:lable];
    return lable;
}

- (void)setFrame:(CGRect)frame
{
    frame.size.width = ScreenSize.width * kTableViewWidthRatio;
    
    [super setFrame:frame];
}

- (ApplyEntrustCell *)initWithSymbol:(SymbolModel *)symbol
{
    
    UILabel *publicVolume = self.contentView.subviews[0];
    UILabel *singlePrice = self.contentView.subviews[1];
    UILabel *minimumAsk = self.contentView.subviews[2];
    UILabel *maxnumAsk = self.contentView.subviews[3];
    UILabel *signDate = self.contentView.subviews[4];
    UILabel *publicDate = self.contentView.subviews[5];
    UILabel *destoryDate = self.contentView.subviews[6];
    
    publicVolume.text = symbol.publicVolume;
    singlePrice.text = symbol.singlePrice;
    minimumAsk.text = symbol.minimumAsk;
    maxnumAsk.text = symbol.maxnumAsk;
    signDate.text = symbol.signDate;
    publicDate.text = symbol.publicDate;
    destoryDate.text = symbol.destoryDate;
    
    return self;
    
}

@end
