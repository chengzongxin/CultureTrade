//
//  SymbolModel.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/11.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SymbolModel : NSObject

@property (nonatomic,copy) NSString *productID;		//,产品编号");
@property (nonatomic,copy) NSString *productName;		//, "商品名");    1
@property (nonatomic,copy) NSString *totalPrice;		//, "基准货币");
@property (nonatomic,copy) NSString *totalVolume;		//总份数");///    2
@property (nonatomic,copy) NSString *singlePrice;		//每份金额	;     3
@property (nonatomic,copy) NSString *reserveVolume;		//保留份数;
@property (nonatomic,copy) NSString *publicVolume;		//发行份数);
@property (nonatomic,copy) NSString *tradeState;        //交易状态  9620
@property (nonatomic,copy) NSString *minimumAsk;		//最低申购手数
@property (nonatomic,copy) NSString *maxnumAsk;		//最大申购手数
@property (nonatomic,copy) NSString *percent;		//申购率;             4
@property (nonatomic,copy) NSString *tradeType;		//,交易方式");
@property (nonatomic,copy) NSString *publicUserID;		//, "发布产品人的编号");
@property (nonatomic,copy) NSString *productState;		//, "状态");
@property (nonatomic,copy) NSString *signDate;		//, "挂牌日期");
@property (nonatomic,copy) NSString *publicDate;		//, "发行日期");
@property (nonatomic,copy) NSString *destoryDate;		//, "截止日期");
@property (nonatomic,copy) NSString *askVolume;		//,"已申购份数");

+ (SymbolModel *)symbolWithDictionary:(NSDictionary *)dict;
- (SymbolModel *)initWithDictionary:(NSDictionary *)dict;
@end
