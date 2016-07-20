//
//  GlobalModel.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/14.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SymbolModel.h"
#import "DataModel.h"

@interface GlobalModel : NSObject 
singleton_interface(GlobalModel)
// 商品列表
@property (nonatomic,retain) SymbolModel *symbolModel;
@property (nonatomic,retain) NSMutableArray *symbolArray;

@property (nonatomic,retain) StockInfoNS *stockInfo;
@property (nonatomic,retain) NSMutableArray *stockNameArray;
@property (nonatomic,retain) NSMutableArray *stockInfoArray;


@property (nonatomic,retain) VolPriceNS *volPrice;
@property (nonatomic,retain) NSMutableArray *volPriceArray;
// NowData
@property (nonatomic,retain) NowDataNS *nowData;
// 盘面数据
@property (nonatomic,retain) SortUnit *sortUnit;
@property (nonatomic,retain) NSMutableArray *sortUnitArray;

// 资金持仓
@property (nonatomic,retain) MoneyHold *moneyHold;

// 份额持仓
@property (nonatomic,retain) ShareHold *shareHold;
@property (nonatomic,retain) NSMutableArray *shareHoldArray;

// 申购委托

@end
