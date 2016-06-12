//
//  BaseCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BaseCell.h"
#import "SymbolModel.h"
#define kGoodsLableRatio 0.2
#define kQuotaoCellYInrect 10
#define kCount 19
#define kTableViewWidthRatio 4

@implementation BaseCell

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
    CGFloat width = self.frame.size.width / kCount;
    CGFloat height = self.frame.size.height;
    
    _openPrice =        [self setCellWithRect:(CGRect){{width * 0,10},{width,height}}];
    _range =            [self setCellWithRect:(CGRect){{width * 1,10},{width,height}}];
    _maxPrice =         [self setCellWithRect:(CGRect){{width * 2,10},{width,height}}];
    _preClose =         [self setCellWithRect:(CGRect){{width * 3,10},{width,height}}];
    _avePrice =         [self setCellWithRect:(CGRect){{width * 4,10},{width,height}}];
    _curPrice =         [self setCellWithRect:(CGRect){{width * 5,10},{width,height}}];
    _raiseDown =        [self setCellWithRect:(CGRect){{width * 6,10},{width,height}}];
    _minPrice =         [self setCellWithRect:(CGRect){{width * 7,10},{width,height}}];
    _curVolume =        [self setCellWithRect:(CGRect){{width * 8,10},{width,height}}];
    _buyPrice =         [self setCellWithRect:(CGRect){{width * 9,10},{width,height}}];
    _buyVolume =        [self setCellWithRect:(CGRect){{width * 10,10},{width,height}}];
    _sellPrice =        [self setCellWithRect:(CGRect){{width * 11,10},{width,height}}];
    _sellVolume =       [self setCellWithRect:(CGRect){{width * 12,10},{width,height}}];
    _turnOverVol =      [self setCellWithRect:(CGRect){{width * 13,10},{width,height}}];
    _inventories =      [self setCellWithRect:(CGRect){{width * 14,10},{width,height}}];
    _turnOverMoney =    [self setCellWithRect:(CGRect){{width * 15,10},{width,height}}];
    _volumeRatio =      [self setCellWithRect:(CGRect){{width * 16,10},{width,height}}];
    _entrustRatio =     [self setCellWithRect:(CGRect){{width * 17,10},{width,height}}];
    _turnOverRate =     [self setCellWithRect:(CGRect){{width * 18,10},{width,height}}];
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

/*
 @property (nonatomic,retain) UILabel *openPrice;
 @property (nonatomic,retain) UILabel *range;
 @property (nonatomic,retain) UILabel *maxPrice;
 @property (nonatomic,retain) UILabel *preClose;
 @property (nonatomic,retain) UILabel *avePrice;
 @property (nonatomic,retain) UILabel *curPrice;
 @property (nonatomic,retain) UILabel *raiseDown;
 @property (nonatomic,retain) UILabel *minPrice;
 @property (nonatomic,retain) UILabel *curVolume;
 @property (nonatomic,retain) UILabel *buyPrice;
 @property (nonatomic,retain) UILabel *buyVolume;
 @property (nonatomic,retain) UILabel *sellPrice;
 @property (nonatomic,retain) UILabel *sellVolume;
 @property (nonatomic,retain) UILabel *turnOverVol;
 @property (nonatomic,retain) UILabel *inventories;
 @property (nonatomic,retain) UILabel *turnOverMoney;
 @property (nonatomic,retain) UILabel *volumeRatio;
 @property (nonatomic,retain) UILabel *entrustRatio;
 @property (nonatomic,retain) UILabel *turnOverRate;
 
 @interface  SortUnit : NSObject
 @property (nonatomic,retain) CodeInfo	*m_CodeInfo;//代码值
 @property (nonatomic,assign) int    m_uiOpenPrice;//今开盘
 @property (nonatomic,assign) int	m_uiMaxPrice;//最高价
 @property (nonatomic,assign) int	m_uiMinPrice;//最低价
 @property (nonatomic,assign) int	m_uiNewPrice;//最新价
 @property (nonatomic,assign) int    m_uiAverage;//均价
 @property (nonatomic,assign) int	m_uiVolume;//成交量总手
 @property (nonatomic,assign) int	m_uiCurVol;//成交量现手
 @property (nonatomic,assign) int	m_uiSum;//成交金额
 @property (nonatomic,assign) int	m_uiSellPrice;//委卖价
 @property (nonatomic,assign) int	m_uiBuyPrice;//委买价
 @property (nonatomic,assign) int	m_uiSellVol;//委卖量
 @property (nonatomic,assign) int	m_uiBuyVol;//委买量
 @property (nonatomic,assign) int    m_iRatio;//委比
 
 @property (nonatomic,retain) NSString *productName;
 @property (nonatomic,retain) NSString *productID;
 @property (nonatomic,assign) float  m_uiPreClose;//昨收
 @property (nonatomic,assign) int    m_uiShares;//流通股数
 @property (nonatomic,assign) int    m_uiReserves;//库存量
 @property (nonatomic,assign) int    m_ui5DayVol;//五日平均总手
 @property (nonatomic,assign) int    m_usOpenTime;//开盘时间
 + (SortUnit *)sortUnitWithArray:(NSArray *)fieldArr;
 - (SortUnit *)initWithArray:(NSArray *)fieldArr;
 @end

 */
- (BaseCell *)initWithSortUnit:(SortUnit *)sortUnit
{
    float curPrice = sortUnit.m_uiNewPrice;
    float preClose = sortUnit.m_uiPreClose;
    
    self.openPrice.text =   [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiOpenPrice];
    self.range.text =       [NSString stringWithFormat:@"%0.2f%%",(curPrice - preClose)/preClose*100];
    self.maxPrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiMaxPrice];
    self.preClose.text =    [NSString stringWithFormat:@"%0.2f",preClose];
    self.avePrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiAverage];
    self.curPrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiNewPrice];
    self.raiseDown.text =   [NSString stringWithFormat:@"%0.2f",curPrice - preClose];
    self.minPrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiMinPrice];
    self.curVolume.text =   [NSString stringWithFormat:@"%d",sortUnit.m_uiCurVol];
    self.buyPrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiBuyPrice];
    self.buyVolume.text =   [NSString stringWithFormat:@"%d",sortUnit.m_uiBuyVol];
    self.sellPrice.text =   [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiSellPrice];
    self.sellVolume.text =  [NSString stringWithFormat:@"%d",sortUnit.m_uiSellVol];
    self.turnOverVol.text = [NSString stringWithFormat:@"%d",sortUnit.m_uiVolume];
    self.inventories.text = [NSString stringWithFormat:@"%d",sortUnit.m_uiReserves];
    self.turnOverMoney.text = [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiSum];
    
    NSDateFormatter *dfm = [[NSDateFormatter alloc] init];
    [dfm setDateFormat:@"HH:mm"];
    NSString *nowDate = [dfm stringFromDate:[NSDate date]];
    
    NSArray *timeArr = [nowDate componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@":"]];
    int totalMin = 0;
    int hour = [timeArr[0] intValue];
    int min = [timeArr[1] intValue];
    
    totalMin = hour*60+min;
    if (totalMin < 570) {
        totalMin = 0;
    }else if (totalMin < 690){
        totalMin -= 570;
    }else if (totalMin < 780){
        totalMin = 120;
    }else if (totalMin < 900){
        totalMin = totalMin - 780 + 120;
    }else{
        totalMin = 240;
    }
    
    if (sortUnit.m_uiVolume*totalMin*sortUnit.m_ui5DayVol) {
        self.volumeRatio.text = [NSString stringWithFormat:@"%0.2f",(float)sortUnit.m_uiVolume*(float)(240/totalMin)/(float)sortUnit.m_ui5DayVol];
    }else{
        self.volumeRatio.text = @"-";
    }
    
    
    self.entrustRatio.text = [NSString stringWithFormat:@"%0.2f%%",sortUnit.m_iRatio*0.01];
    
    self.turnOverRate.text = [NSString stringWithFormat:@"%0.2f%%",((float)sortUnit.m_uiVolume/(float)sortUnit.m_uiShares)*100];
    if (sortUnit.m_uiShares == 0) {
        self.turnOverRate.text = [NSString stringWithFormat:@"0.00%%"];
    }
    
    self.openPrice.textColor = [self.openPrice.text floatValue] > preClose ? COLOR_QUOTA_INCRASE_RED:COLOR_QUOTA_DECRASE_GREEN;
    self.maxPrice.textColor = [self.maxPrice.text floatValue] > preClose ? COLOR_QUOTA_INCRASE_RED:COLOR_QUOTA_DECRASE_GREEN;
    self.minPrice.textColor = [self.minPrice.text floatValue] > preClose ? COLOR_QUOTA_INCRASE_RED:COLOR_QUOTA_DECRASE_GREEN;
    self.buyPrice.textColor = [self.buyPrice.text floatValue] > preClose ? COLOR_QUOTA_INCRASE_RED:COLOR_QUOTA_DECRASE_GREEN;
    self.sellPrice.textColor = [self.sellPrice.text floatValue] > preClose ? COLOR_QUOTA_INCRASE_RED:COLOR_QUOTA_DECRASE_GREEN;
    // xia
    self.openPrice.textColor = [self.openPrice.text floatValue] == preClose ? [UIColor whiteColor]:self.openPrice.textColor;
    self.maxPrice.textColor = [self.maxPrice.text floatValue] == preClose ? [UIColor whiteColor]:self.maxPrice.textColor;
    self.minPrice.textColor = [self.minPrice.text floatValue] == preClose ? [UIColor whiteColor]:self.minPrice.textColor;
    self.buyPrice.textColor = [self.buyPrice.text floatValue] == preClose ? [UIColor whiteColor]:self.buyPrice.textColor;
    self.sellPrice.textColor = [self.sellPrice.text floatValue] == preClose ? [UIColor whiteColor]:self.sellPrice.textColor;
    
    if (curPrice == 0 || preClose == 0) {
    }else{
        if (curPrice >= preClose) { // 涨
            self.range.textColor = COLOR_QUOTA_INCRASE_RED;
            self.curPrice.textColor = COLOR_QUOTA_INCRASE_RED;
            self.raiseDown.textColor = COLOR_QUOTA_INCRASE_RED;
            self.avePrice.textColor = COLOR_QUOTA_INCRASE_RED;
        }else{                      // 跌
            self.range.textColor = COLOR_QUOTA_DECRASE_GREEN;
            self.curPrice.textColor = COLOR_QUOTA_DECRASE_GREEN;
            self.raiseDown.textColor = COLOR_QUOTA_DECRASE_GREEN;
            self.avePrice.textColor = COLOR_QUOTA_DECRASE_GREEN;
        }
    }
    
    if (sortUnit.m_iRatio > 0) {
        self.entrustRatio.textColor = COLOR_QUOTA_INCRASE_RED;
    }else if (sortUnit.m_iRatio == 0){
        self.entrustRatio.textColor = [UIColor whiteColor];
    }else{
        self.entrustRatio.textColor = COLOR_QUOTA_DECRASE_GREEN;
    }
    
    
    // 为0时显示-  数据为0时处理
    NSArray *contentViews = self.contentView.subviews;
    for (id view in contentViews) {
        if (view == self.curVolume ||
            view == self.buyPrice ||
            view == self.buyVolume ||
            view == self.sellPrice ||
            view == self.sellVolume ||
            view == self.turnOverVol ||
            view == self.inventories ||
            view == self.turnOverMoney ||
            view == self.turnOverRate) continue;
        if([[(UILabel *)view text] floatValue] == 0.0f) [(UILabel *)view setText:@"-"];
    }
    
    if (curPrice*preClose == 0) { // 最新和昨收 有一个=0
        self.range.text = @"-";
        self.raiseDown.text =   @"-";
    }else{
        self.range.text =       [NSString stringWithFormat:@"%0.2f%%",(curPrice - preClose)/preClose*100];
        self.avePrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiAverage];
        self.raiseDown.text =   [NSString stringWithFormat:@"%0.2f",curPrice - preClose];
    }
    self.avePrice.text =    [NSString stringWithFormat:@"%0.2f",sortUnit.m_uiAverage];
    
    return self;
}

@end
