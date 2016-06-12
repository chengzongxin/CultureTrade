//
//  MeFunctionCell.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/29.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ToggleView;

typedef enum{
    kCellTypeNone = 1,
    kCellTypeLable,
    kCellTypeArrow,
    kCellTypeSwitch
} CellType;

@interface MeFunctionCell : UITableViewCell
{
    UIImageView *_icon;
    UILabel *_textString;
}
@property (nonatomic,assign) CellType cellType;
@property (nonatomic,retain) ToggleView *rightSwitch;
@property (nonatomic,retain) UILabel *rightLable;
@property (nonatomic,retain) UIImageView *rightArrow;


- (void)setIcon:(NSString *)icon;
- (void)setTextString:(NSString *)text;

@end
