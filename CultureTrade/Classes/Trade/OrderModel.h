//
//  OrderModel.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/16.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface OrderModel : NSObject
@property (nonatomic,retain) NSString *askNum;               // 132 申请数量
@property (nonatomic,retain) NSString *exeState;             // 150 执行状态
@property (nonatomic,retain) NSString *dealVolume;           //32 成交量
@property (nonatomic,retain) NSString *orderID;               // 37 订单编号
//40 订单类型 0B限价买入0S限价卖出 0C限价撤单 7B 新股申购买入 7C新股申购撤单 XB 本方最优买入，XS本方最优买出，XC撤单 YB对方最优买入，YS对方最优卖出 YC撤单
@property (nonatomic,retain) NSString *orderType;
@property (nonatomic,retain) NSString *entrustPrice;         //44 委托价格
@property (nonatomic,retain) NSString *stockID;              //48 证券编号
@property (nonatomic,retain) NSString *date;                 // 9205 日期时间
@property (nonatomic,retain) NSString *cancerOrderVolume;    // 9210 撤单数量
@property (nonatomic,retain) NSString *stockName;            //9402 产品名称

+ (OrderModel *)oderWithDict:(NSDictionary *)dict;
- (OrderModel *)initWithDict:(NSDictionary *)dict;
@end
