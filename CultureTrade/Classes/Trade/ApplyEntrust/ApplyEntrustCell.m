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

// 申购委托
- (ApplyEntrustCell *)initWithEntrust:(ApplyPurchaseEntrust *)entrust
{
    int askvol = [entrust.askvol intValue];
    float signprice = [entrust.signprice floatValue];
    int signstate = [entrust.signstate intValue];
    int signvol = [entrust.signvol intValue];
    float psgcommision = [entrust.psgcommision floatValue];
    
    
    UILabel *publicVolume = self.contentView.subviews[0];
    UILabel *singlePrice = self.contentView.subviews[1];
    UILabel *minimumAsk = self.contentView.subviews[2];
    UILabel *maxnumAsk = self.contentView.subviews[3];
    UILabel *signDate = self.contentView.subviews[4];
    UILabel *publicDate = self.contentView.subviews[5];
    UILabel *destoryDate = self.contentView.subviews[6];
    UILabel *entrustDate = self.contentView.subviews[7];
    UILabel *settlementDate = self.contentView.subviews[8];
    
    publicVolume.text = entrust.contractcode;
    singlePrice.text = entrust.askvol;
    minimumAsk.text = entrust.signprice;
    maxnumAsk.text = [NSString stringWithFormat:@"%0.2f",askvol*signprice];
    
    if (signstate==1) {
        signDate.text = [NSString stringWithFormat:@"%0.2f",(askvol-signvol)*signprice];
    }else{
        signDate.text = @"0";
    }
    
    publicDate.text = [NSString stringWithFormat:@"%0.2f",askvol*signvol*psgcommision/100];
    destoryDate.text = [NSString stringWithFormat:@"%0.2f",(askvol-signvol)*signprice*psgcommision/100];
    
    entrustDate.text = entrust.signdatetime;
    entrustDate.numberOfLines = 0;
    settlementDate.text = entrust.signdatetime;
    settlementDate.numberOfLines = 0;
    return self;
}

// 中签查询
- (ApplyEntrustCell *)initWithSuccess:(ApplyPurchaseSuccess *)suceess
{
    UILabel *publicVolume = self.contentView.subviews[0];
    UILabel *singlePrice = self.contentView.subviews[1];
    UILabel *minimumAsk = self.contentView.subviews[2];
    UILabel *maxnumAsk = self.contentView.subviews[3];
    UILabel *signDate = self.contentView.subviews[4];
    UILabel *publicDate = self.contentView.subviews[5];
    UILabel *destoryDate = self.contentView.subviews[6];
    
    publicVolume.text = suceess.contractcode;
    singlePrice.text = suceess.signvol;
    minimumAsk.text = suceess.signprice;
    maxnumAsk.text = suceess.contractcode;
    
    int signvol = [suceess.signvol intValue];
    float signprice = [suceess.signprice floatValue];
   
    signDate.text = [NSString stringWithFormat:@"%0.2f",signvol*signprice];
    publicDate.text = suceess.sgcommision;
    destoryDate.text = suceess.signdatetime;
    destoryDate.numberOfLines = 0;
    return self;

}

@end
