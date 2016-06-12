//
//  BuyInModel.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/4.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Buy : NSObject
@property (nonatomic,assign) float price;
@property (nonatomic,assign) int shoushu;
@end

@interface Sell : NSObject

@end

@interface BuyInModel : NSObject
@property (nonatomic,retain) Buy *buy;
@property (nonatomic,retain) Sell *sell;

@end
