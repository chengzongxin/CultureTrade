//
//  LeftApplyEntrustCell.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/20.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
@class SymbolModel;
@interface LeftApplyEntrustCell : UITableViewCell
@property (nonatomic,retain) UILabel *productName;
@property (nonatomic,retain) UILabel *productID;
- (LeftApplyEntrustCell *)initWithSymbol:(SymbolModel *)symbol;
@end
