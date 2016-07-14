//
//  PurchaseCell.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SymbolModel.h"
@interface PurchaseCell : UITableViewCell
@property (nonatomic,assign) int labelCount;
- (PurchaseCell *)initWithSymbol:(SymbolModel *)symbol;
@end
