//
//  ApplyEntrustCell.h
//  CultureTrade
//
//  Created by SZFT1 on 16/7/20.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DataModel.h"
@interface ApplyEntrustCell : UITableViewCell
@property (nonatomic,assign) int labelCount;
- (ApplyEntrustCell *)initWithEntrust:(ApplyPurchaseEntrust *)entrust;
- (ApplyEntrustCell *)initWithSuccess:(ApplyPurchaseSuccess *)suceess;
@end
