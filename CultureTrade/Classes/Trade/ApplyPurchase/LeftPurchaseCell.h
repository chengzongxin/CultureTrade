//
//  LeftPurchaseCell.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
@class SymbolModel;
@interface LeftPurchaseCell : UITableViewCell
@property (nonatomic,retain) UILabel *productName;
@property (nonatomic,retain) UILabel *productID;
- (LeftPurchaseCell *)initWithSymbol:(SymbolModel *)symbol;
@end
