//
//  DataModel.h
//  CultureTrade
//
//  Created by SZFT1 on 16/1/5.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DataModel : NSObject
- (NSDictionary *)propertyMapDic;
-(void) assginToPropertyWithNoMapDictionary: (NSDictionary *) data;
-(void) assginToPropertyWithDictionary: (NSDictionary *) data;
- (SEL)creatSetterWithPropertyName:(NSString *)propertyName;
@end

@interface RealMinsUnitNS : NSObject
@property (nonatomic,assign) short	m_usTime;//距离第一次开盘时间
@property (nonatomic,assign) float	m_uiNewPrice;
@property (nonatomic,assign) int	m_uiVolume;
@property (nonatomic,assign) int	m_uiKeepVol;//持仓量,适用于期货市场
@property (nonatomic,assign) float	m_uiAverage;//平均价格
@property (nonatomic,assign) int	m_uiBuyVol;//买盘数量
@property (nonatomic,assign) int	m_uiSellVol;//卖盘数量
@end

@interface NowDataNS : NSObject
@property (nonatomic,assign) float	m_uiCode;
@property (nonatomic,assign) float	m_usTime;//距第一次开盘时间数
@property (nonatomic,assign) float	m_uiOpenPrice;//今开盘
@property (nonatomic,assign) float	m_uiMaxPrice;//最高价
@property (nonatomic,assign) float	m_uiMinPrice;//最低价
@property (nonatomic,assign) float	m_uiNewPrice;//最新价
@property (nonatomic,assign) float	m_uiAverage;//均价
@property (nonatomic,assign) int	m_uiVolume;//成交量总手
@property (nonatomic,assign) int	m_uiCurVol;//成交量现手
@property (nonatomic,assign) int	m_uiSum;//成交金额，期货中没有此项
@property (nonatomic,assign) int	m_uiBuyVol;//主买，对于指数为上涨家数
@property (nonatomic,assign) int	m_uiSellVol;//主卖，对于指数为下跌家数
@property (nonatomic,assign) char	m_ucVolPrices;//量价个数
    // SVolPrice*	m_pstVolPrice;//量价结构
@end

@interface VolPriceNS : NSObject
@property (nonatomic,assign) float		m_uiPrice;//价格
@property (nonatomic,assign) int		m_uiVolume;//量
@end

//交易品种成交分笔结构
@interface TraceUnitNS : NSObject
@property (nonatomic,assign) short	m_usTime;     //距离开盘时间的分钟数
@property (nonatomic,assign) int	m_uiNewPrice; //最新价
@property (nonatomic,assign) int	m_uiVolume;   //成交量
@property (nonatomic,assign) int    m_uiBuyVol;   //委买量
@property (nonatomic,assign) int    m_uiSellVol;  //委卖量
@property (nonatomic,assign) int	m_uiBuyPrice; //委买价，对于指数则表示上涨家数
@property (nonatomic,assign) int	m_uiSellPrice;//委卖价，对于指数则表示下跌家数
@property (nonatomic,assign) int	m_uiSum;      //成交金额, 持仓量适用于期货市场
@end

@interface StockInfoNS : NSObject
@property (nonatomic,copy) NSString *m_acName;//名称
@property (nonatomic,copy) NSString *m_acSpell;
@property (nonatomic,assign) int    m_uiCode;//代码
@property (nonatomic,assign) int    m_usMarketType;//市场标识
@property (nonatomic,assign) float  m_uiPrevClose;//昨收价
@property (nonatomic,assign) int    m_ui5DayVol;//五日平均总手
@property (nonatomic,assign) float  m_uiUpLimit;//涨停板限制,
@property (nonatomic,assign) float  m_uiDownLimit;//跌停板限制
@property (nonatomic,assign) int    m_uiShares;//流通股数
@property (nonatomic,assign) int    m_uiReserves; // 库存量
@property (nonatomic,assign) int  m_sAsset;//每股净资产
@end

@interface  CodeInfo :NSObject
@property (nonatomic,assign) short	m_usMarketType;//市场标识
@property (nonatomic,assign) int		m_uiCode;//证券代码
@end

@interface  SortUnit : NSObject
@property (nonatomic,retain) CodeInfo	*m_CodeInfo;//代码值
@property (nonatomic,assign) float    m_uiOpenPrice;//今开盘
@property (nonatomic,assign) float	m_uiMaxPrice;//最高价
@property (nonatomic,assign) float	m_uiMinPrice;//最低价
@property (nonatomic,assign) float	m_uiNewPrice;//最新价
@property (nonatomic,assign) float    m_uiAverage;//均价
@property (nonatomic,assign) int	m_uiVolume;//成交量总手
@property (nonatomic,assign) int	m_uiCurVol;//成交量现手
@property (nonatomic,assign) float	m_uiSum;//成交金额
@property (nonatomic,assign) float	m_uiSellPrice;//委卖价
@property (nonatomic,assign) float	m_uiBuyPrice;//委买价
@property (nonatomic,assign) int	m_uiSellVol;//委卖量
@property (nonatomic,assign) int	m_uiBuyVol;//委买量
@property (nonatomic,assign) int    m_iRatio;//委比

@property (nonatomic,retain) NSString *productName;
@property (nonatomic,retain) NSString *productID;
@property (nonatomic,assign) float  m_uiPreClose;//昨收
@property (nonatomic,assign) int    m_uiShares;//流通股数
@property (nonatomic,assign) int    m_uiReserves;//库存量
@property (nonatomic,assign) int    m_ui5DayVol;//五日平均总手
@property (nonatomic,assign) int    m_usOpenTime;//开盘时间
+ (SortUnit *)sortUnitWithArray:(NSArray *)fieldArr;
- (SortUnit *)initWithArray:(NSArray *)fieldArr;
@end


@interface MoneyHold : NSObject
@property (nonatomic,assign) double accountID;
@property (nonatomic,assign) double accountBalance;
@property (nonatomic,assign) double accountFreeze;
@property (nonatomic,assign) double clearDate;
@property (nonatomic,assign) double totalBalance;
@property (nonatomic,assign) double canOut;
@property (nonatomic,assign) double otherFreeze;
@end

@interface Order : DataModel
@property (nonatomic,retain) NSString *applyCount;//132		//申请数量
@property (nonatomic,retain) NSString *orderState;//150     //订单执行状态 '1 :已提交 \r\n2 成交 \r\n3 部分成交 \r\n4主动撤单  \r\n5交易所拒绝 \r\n6已冻结 \r\n7 其他 \r\n 8 已过期',
@property (nonatomic,retain) NSString *turnOverVol;//32		//成交量
@property (nonatomic,retain) NSString *orderID;	//		37		//订单编号
@property (nonatomic,retain) NSString *orderType;//				40		//订单类型
@property (nonatomic,retain) NSString *orderPrice;//44		//委托价格
@property (nonatomic,retain) NSString *productID;//				48		//证券编号
@property (nonatomic,retain) NSString *orderTime;//				9205	//委托时间
@property (nonatomic,retain) NSString *orderCancelVol;//			9210	//撤单数量
@property (nonatomic,retain) NSString *productName;//	9402			//产品名称
+ (Order *)orderWithDictionary:(NSDictionary *)dict;
- (Order *)initWithDictionary:(NSDictionary *)dict;
@end

@interface TurnOver : DataModel
@property (nonatomic,retain) NSString *user;// 1 user
@property (nonatomic,retain) NSString *commision;//				12		//手续费
@property (nonatomic,retain) NSString *turnOverPrice;//			31		//成交价格
@property (nonatomic,retain) NSString *turnOverVol;//			32		//成交量
@property (nonatomic,retain) NSString *totalMoney;//			384		//成交总金额
@property (nonatomic,retain) NSString *orderType;//				40		//订单类型 0B限价买入0S限价卖出
@property (nonatomic,retain) NSString *productID;//				48		//证券编号
@property (nonatomic,retain) NSString *timeForce;//				60		//成交时间
@property (nonatomic,retain) NSString *productName;//           9402	//产品名称
@property (nonatomic,retain) NSString *contractNO;//            9501	//成交合同编号
@property (nonatomic,retain) NSString *opponentID;//            9502	//成交对手方编号
@property (nonatomic,retain) NSString *opponentName;//          9503	//成交对手方名称
+ (instancetype)turnOverWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithDictionary: (NSDictionary *) data;
@end

@interface TransferQuery : DataModel
@property (nonatomic,retain) NSString *userID;//1
@property (nonatomic,retain) NSString *performState;//150		//订单执行状态
@property (nonatomic,retain) NSString *detail;//58      //备注
@property (nonatomic,retain) NSString *operatorType;//8031    //操作类型
@property (nonatomic,retain) NSString *time;// 8106    //时间
@property (nonatomic,retain) NSString *ID;//8116    //ID
@property (nonatomic,retain) NSString *verificationTime;//8117    //审核时间
@property (nonatomic,retain) NSString *money;//9311	//转帐金额
+ (instancetype)transferQueryWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithDictionary:(NSDictionary *)data;
@end


@interface ShareHold : DataModel
@property (nonatomic,retain) NSString *turnOverPrice;
@property (nonatomic,retain) NSString *productID;
@property (nonatomic,retain) NSString *productName;
@property (nonatomic,retain) NSString *totalBalance;
@property (nonatomic,retain) NSString *freezeVolume;
@property (nonatomic,retain) NSString *availableVolume;
@property (nonatomic,retain) NSString *otherFreeze;
@property (nonatomic,retain) NSString *keepCost;
+ (instancetype)shareHoldWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithDictionary:(NSDictionary *)data;
@end

@interface ApplyPurchaseEntrust : DataModel
@property (nonatomic,retain) NSString *askvol;
@property (nonatomic,retain) NSString *contractcode;
@property (nonatomic,retain) NSString *customid;
@property (nonatomic,retain) NSString *endid;
@property (nonatomic,retain) NSString *productname;
@property (nonatomic,retain) NSString *productstatus;
@property (nonatomic,retain) NSString *psgcommision;
@property (nonatomic,retain) NSString *sgcommision;
@property (nonatomic,retain) NSString *signdatetime;
@property (nonatomic,retain) NSString *signprice;
@property (nonatomic,retain) NSString *signstate;
@property (nonatomic,retain) NSString *signvol;
@property (nonatomic,retain) NSString *startid;
@property (nonatomic,retain) NSString *symbolcode;
+ (instancetype)applyPurchaseEntrustWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithDictionary:(NSDictionary *)data;
@end

@interface ApplyPurchaseSuccess : DataModel
@property (nonatomic,retain) NSString *askvol;
@property (nonatomic,retain) NSString *contractcode;
@property (nonatomic,retain) NSString *customid;
@property (nonatomic,retain) NSString *endid;
@property (nonatomic,retain) NSString *productname;
@property (nonatomic,retain) NSString *productstatus;
@property (nonatomic,retain) NSString *psgcommision;
@property (nonatomic,retain) NSString *sgcommision;
@property (nonatomic,retain) NSString *signdatetime;
@property (nonatomic,retain) NSString *signprice;
@property (nonatomic,retain) NSString *signstate;
@property (nonatomic,retain) NSString *signvol;
@property (nonatomic,retain) NSString *startid;
@property (nonatomic,retain) NSString *symbolcode;
+ (instancetype)applyPurchaseSuccessWithDictionary:(NSDictionary *)dict;
- (instancetype)initWithDictionary:(NSDictionary *)data;
@end


