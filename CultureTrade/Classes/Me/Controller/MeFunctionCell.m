//
//  MeFunctionCell.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/29.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "MeFunctionCell.h"
#import "ToggleView.h"

#define kIconXMarginToCenter 50
#define kCenterOffset 10
@interface MeFunctionCell () <ToggleViewDelegate>
@end

@implementation MeFunctionCell



- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        _textString = [[UILabel alloc] init];
        _icon = [[UIImageView alloc] init];
        [self.contentView addSubview:_textString];
        [self.contentView addSubview:_icon];
        self.backgroundColor = COLOR_CELL_BG;
        [self setSeparatorInset:UIEdgeInsetsZero];
    }
    
    return self;
}

- (void)setCellType:(CellType)cellType
{
    _cellType = cellType;
    
    if (cellType == kCellTypeArrow) {
        _rightArrow = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"go"]];
        self.accessoryView = _rightArrow;
    } else if (cellType == kCellTypeNone){
        _rightLable = [[UILabel alloc] init];
        self.accessoryView = _rightLable;
    } else if (cellType == kCellTypeNone){
        self.accessoryView = nil;
    } else if (cellType == kCellTypeSwitch){
        CGSize size = [UIImage imageNamed:@"off_bg"].size;
        CGRect rect = CGRectMake(0, 0, size.width, size.height);
        _rightSwitch = [[ToggleView alloc] initWithFrame:rect toggleViewType:ToggleViewTypeNoLabel toggleBaseType:ToggleBaseTypeChangeImage toggleButtonType:ToggleButtonTypeChangeImage];
        _rightSwitch.toggleDelegate = self;
        _rightSwitch.selectedButton = ToggleButtonSelectedRight;
        self.accessoryView = _rightSwitch;
    }

}

- (void)selectLeftButton
{
    MYLOGFUN;
}

- (void)selectRightButton
{
    MYLOGFUN;
}

- (void)setIcon:(NSString *)icon
{
    _icon.image = [UIImage imageNamed:icon];
    _icon.frame = CGRectMake(20, kCenterOffset, _icon.image.size.width,_icon.image.size.height);
//    _icon.center = CGPointMake(self.center.x - kIconXMarginToCenter, self.center.y - kCenterOffset);
}

- (void)setTextString:(NSString *)text
{
    
    _textString.frame = CGRectMake(40, kCenterOffset, 70, 20);
//    _textString.center = CGPointMake(self.center.x, self.center.y - kCenterOffset);
    _textString.text = text;
    _textString.lineBreakMode = NSLineBreakByWordWrapping;
    _textString.font = [UIFont systemFontOfSize:13];
    _textString.textColor = [UIColor whiteColor];
    _textString.textAlignment = NSTextAlignmentCenter;
}

- (void)setFrame:(CGRect)frame
{
    frame.size.width = ScreenSize.width;
    
    [super setFrame:frame];
}

@end
