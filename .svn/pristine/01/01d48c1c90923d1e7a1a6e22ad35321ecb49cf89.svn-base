//
//  DataModel.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/5.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "DataModel.h"
#import "tagdef.h"
@implementation DataModel
#pragma 根据映射关系来给Model的属性赋值
- (NSDictionary *)propertyMapDic
{
    return nil;
}


-(void) assginToPropertyWithNoMapDictionary: (NSDictionary *) data{
    ///获取字典和Model属性的映射关系
    NSDictionary *propertyMapDic = [self propertyMapDic];
    
    ///转化成key和property一样的字典，然后调用assginToPropertyWithDictionary方法
    
    NSArray *dicKey = [data allKeys];
    
    
    NSMutableDictionary *tempDic = [[NSMutableDictionary alloc] initWithCapacity:dicKey.count];
    
    for (int i = 0; i < dicKey.count; i ++) {
        NSString *key = dicKey[i];
        [tempDic setObject:data[key] forKey:propertyMapDic[key]];
    }
    
    [self assginToPropertyWithDictionary:tempDic];
    
}
/************************************************************************
 *把字典赋值给当前实体类的属性
 *参数：字典
 *适用情况：当网络请求的数据的key与实体类的属性相同时可以通过此方法吧字典的Value
 *        赋值给实体类的属性
 ************************************************************************/

-(void) assginToPropertyWithDictionary: (NSDictionary *) data{
    
    if (data == nil) {
        return;
    }
    
    ///1.获取字典的key
    NSArray *dicKey = [data allKeys];
    
    ///2.循环遍历字典key, 并且动态生成实体类的setter方法，把字典的Value通过setter方法
    ///赋值给实体类的属性
    for (int i = 0; i < dicKey.count; i ++) {
        
        ///2.1 通过getSetterSelWithAttibuteName 方法来获取实体类的set方法
        SEL setSel = [self creatSetterWithPropertyName:dicKey[i]];
        
        if ([self respondsToSelector:setSel]) {
            ///2.2 获取字典中key对应的value
            NSString  *value = [NSString stringWithFormat:@"%@", data[dicKey[i]]];
            
            ///2.3 把值通过setter方法赋值给实体类的属性
            [self performSelectorOnMainThread:setSel
                                   withObject:value
                                waitUntilDone:[NSThread isMainThread]];
        }
        
    }
    
}

#pragma mark -- 通过字符串来创建该字符串的Setter方法，并返回
- (SEL)creatSetterWithPropertyName:(NSString *)propertyName{
    
    //1.首字母大写
    //    propertyName = propertyName.capitalizedString;
    
    NSString *firstChar = [propertyName substringToIndex:1];
    NSString *otherChar = [propertyName substringFromIndex:1];
    firstChar = firstChar.capitalizedString;
    NSString *traceString = [NSString stringWithFormat:@"%@%@",firstChar,otherChar];
    
    
    //2.拼接上set关键字
    propertyName = [NSString stringWithFormat:@"set%@:", traceString];
    
    //3.返回set方法
    return NSSelectorFromString(propertyName);
}

@end

@implementation RealMinsUnitNS

@end

@implementation NowDataNS

@end

@implementation VolPriceNS

@end

@implementation TraceUnitNS

@end

@implementation StockInfoNS

@end

@implementation CodeInfo

@end

@implementation SortUnit
- (SortUnit *)initWithArray:(NSArray *)fieldArr
{
    self = [[SortUnit alloc] init];
    self.m_CodeInfo = [[CodeInfo alloc] init];
    self.m_CodeInfo.m_uiCode =          [fieldArr[0] intValue];
    self.m_CodeInfo.m_usMarketType =    [fieldArr[1] intValue];
    self.m_uiOpenPrice =                [fieldArr[2] floatValue]*0.001;//今开盘
    self.m_uiMaxPrice =                 [fieldArr[3] floatValue]*0.001;//最高价
    self.m_uiMinPrice =                 [fieldArr[4] floatValue]*0.001;//最低价
    self.m_uiNewPrice =                 [fieldArr[5] floatValue]*0.001;//最新价
    self.m_uiAverage =                  [fieldArr[6] floatValue]*0.001;//均价(new add)
    self.m_uiVolume =                   [fieldArr[7] intValue];//成交量总手
    self.m_uiCurVol =                   [fieldArr[8] intValue];//成交量现手
    self.m_uiSum =                      [fieldArr[9] floatValue];//成交金额
    self.m_uiSellPrice =                [fieldArr[10] floatValue]*0.001;//委卖价
    self.m_uiBuyPrice =                 [fieldArr[11] floatValue]*0.001;//委买价
    self.m_uiSellVol =                  [fieldArr[12] intValue];//委卖量
    self.m_uiBuyVol =                   [fieldArr[13] intValue];//委买量
    self.m_iRatio =                     [fieldArr[14] intValue];//委比
    return self;
}

+ (SortUnit *)sortUnitWithArray:(NSArray *)fieldArr
{
    return [[SortUnit alloc] initWithArray:fieldArr];
}

- (NSString *)productID
{
    return [NSString stringWithFormat:@"%d",self.m_CodeInfo.m_uiCode];
}

@end

@implementation MoneyHold


@end


@implementation Order

- (NSDictionary *)propertyMapDic
{
    return @{@"132":@"applyCount",
             @"150":@"orderState",
             @"32":@"turnOverVol",
             @"37":@"orderID",
             @"40":@"orderType",
             @"44":@"orderPrice",
             @"48":@"productID",
             @"9205":@"orderTime",
             @"9210":@"orderCancelVol",
             @"9402":@"productName"
             };
}

- (Order *)initWithDictionary:(NSDictionary *)dict
{
        self = [super init];
        if (self) {
            if ([self propertyMapDic] == nil) {// 属性名与Key对应
                [super assginToPropertyWithDictionary:dict];
            } else {
                [super assginToPropertyWithNoMapDictionary:dict];
            }
        }
        return self;
}

+ (Order *)orderWithDictionary:(NSDictionary *)dict
{
    return [[self alloc] initWithDictionary:dict];
}

@end


@implementation TurnOver

 - (NSDictionary *)propertyMapDic
{
    return @{@"1":@"user",
             @"12":@"commision",
             @"31":@"turnOverPrice",
             @"32":@"turnOverVol",
             @"384":@"totalMoney",
             @"40":@"orderType",
             @"48":@"productID",
             @"60":@"timeForce",
             @"9402":@"productName",
             @"9501":@"contractNO",
             @"9502":@"opponentID",
             @"9503":@"opponentName"
             };
}

+ (instancetype)turnOverWithDictionary:(NSDictionary *)dict
{
    return [[self alloc] initWithDictionary:dict];
}

- (instancetype)initWithDictionary: (NSDictionary *) data{
    {
        self = [super init];
        if (self) {
            if ([self propertyMapDic] == nil) {// 属性名与Key对应
                [super assginToPropertyWithDictionary:data];
            } else {
                [super assginToPropertyWithNoMapDictionary:data];
            } 
        } 
        return self; 
    } 
}
@end
/*
@property (nonatomic,retain) NSString *userID;//1
@property (nonatomic,retain) NSString *performState;//150		//订单执行状态
@property (nonatomic,retain) NSString *detail;//58      //备注
@property (nonatomic,retain) NSString *operatorType;//8031    //操作类型
@property (nonatomic,retain) NSString *time;// 8106    //时间
@property (nonatomic,retain) NSString *ID;//8116    //ID
@property (nonatomic,retain) NSString *verificationTime;//8117    //审核时间
@property (nonatomic,retain) NSString *money;//9311	//转帐金额
 */
@implementation TransferQuery

- (NSDictionary *)propertyMapDic
{
    return @{INT2STRING(FIX_TAG_BODY_ACCTID):@"userID",
             INT2STRING(FIX_TAG_BODY_EXECUTETYPE):@"performState",
             INT2STRING(FIX_TAG_BODY_TEXT):@"detail",
             INT2STRING(FIX_TAG_BODY_OPERATORTYPE):@"operatorType",
             INT2STRING(FIX_TAG_BODY_TIME):@"time",
             INT2STRING(FIX_TAG_BODY_ID):@"ID",
             INT2STRING(FIX_TAG_BODY_VERIFICATIONTIME):@"verificationTime",
             INT2STRING(FIX_TAG_INTPRTCL_MONEY):@"money"
             };
}

+ (instancetype)transferQueryWithDictionary:(NSDictionary *)dict
{
    return [[self alloc] initWithDictionary:dict];
}

- (instancetype)initWithDictionary:(NSDictionary *)data{
    {
        self = [super init];
        if (self) {
            if ([self propertyMapDic] == nil) {// 属性名与Key对应
                [super assginToPropertyWithDictionary:data];
            } else {
                [super assginToPropertyWithNoMapDictionary:data];
            }
        }
        return self;
    }
}

@end

@implementation ShareHold

/*
 
 "31" : "80.00000",
 "48" : "602001",
 "9402" : "\270ĸ￪\267\305\310\375ʮ\326\334\304\352С\260\346",
 "9403" : "1",
 "9404" : "0",
 "9405" : "1",
 "9406" : "0",
 "9407" : "80.00000
 
@interface ShareHold : DataModel
@property (nonatomic,retain) NSString *turnOverProce;
@property (nonatomic,retain) NSString *productID;
@property (nonatomic,retain) NSString *productName;
@property (nonatomic,retain) NSString *totalBalance;
@property (nonatomic,retain) NSString *freezeVolume;
@property (nonatomic,retain) NSString *availableVolume;
@property (nonatomic,retain) NSString *otherFreeze;
@property (nonatomic,retain) NSString *keepCost;
 */

- (NSDictionary *)propertyMapDic
{
    return @{INT2STRING(FIX_TAG_BODY_TURNOVERPRICE):@"turnOverPrice",
             INT2STRING(FIX_TAG_BODY_SECURITYID):@"productID",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTNAME):@"productName",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTPOWERBALANCE):@"totalBalance",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTFREEZEVOLUME):@"freezeVolume",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTAVIABLVOLUME):@"availableVolume",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTOTHERFREEZE):@"otherFreeze",
             INT2STRING(FIX_TAG_INTPRTCL_PRODUCTKEEPCOST):@"keepCost"
             };
}

+ (instancetype)shareHoldWithDictionary:(NSDictionary *)dict
{
    return [[self alloc] initWithDictionary:dict];
}

- (instancetype)initWithDictionary:(NSDictionary *)data
{
    self = [super init];
    if (self) {
        if ([self propertyMapDic] == nil) {// 属性名与Key对应
            [super assginToPropertyWithDictionary:data];
        } else {
            [super assginToPropertyWithNoMapDictionary:data];
        }
    }
    return self;
    
}
@end






