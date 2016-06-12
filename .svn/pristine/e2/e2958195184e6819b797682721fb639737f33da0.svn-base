//
//  TransferQueryCell.m
//  CultureTrade
//
//  Created by SZFT1 on 16/4/21.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TransferQueryCell.h"
#define kTextTimeFont [UIFont fontWithName:@"Helvetica-Bold" size:10]
@implementation TransferQueryCell

- (void)addLables
{
    CGFloat labelY = 10;
    CGFloat width = self.frame.size.width/4;
    CGFloat height = kCellTextFont.lineHeight;

    //Operation
    _stockName = [self setCellWithFrame:CGRectMake(0, labelY, width, height)];
    //Time
    _entrustPrice = [self setCellWithFrame:CGRectMake(width, 3, width, height*2)];
//    _entrustPrice.font = kTextTimeFont;
    _entrustPrice.numberOfLines = 0;
    _entrustPrice.lineBreakMode = NSLineBreakByWordWrapping;
    //Money
    _entrustDeal = [self setCellWithFrame:CGRectMake(width * 2, labelY, width, height)];
    //Time
    _status = [self setCellWithFrame:CGRectMake(width * 3, labelY, width, height)];

//    //Status
//    _buySell = [self setCellWithFrame:CGRectMake(width * 4, labelY, width, height)];
//    //Commision
//    _nowPrice = [self setCellWithFrame:CGRectMake(width * 5, labelY, width, height)];
}
@end
