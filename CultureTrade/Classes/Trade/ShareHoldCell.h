//
//  ShareHoldCell.h
//  CultureTrade
//
//  Created by SZFT1 on 16/3/24.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ShareHoldCell : UITableViewCell
{
    UILabel *_name;
    UILabel *_code;
    UILabel *_newPrice;
    UILabel *_turnOverPrice;
    UILabel *_shareHold;
    UILabel *_available;
    UILabel *_marketPrice;
    UILabel *_cost;
}

- (ShareHoldCell *)cellWithShareHold:(ShareHold *)shareHold;
@end
