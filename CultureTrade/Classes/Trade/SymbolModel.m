//
//  SymbolModel.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/11.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "SymbolModel.h"
#import "tagdef.h"

@implementation SymbolModel

+ (SymbolModel *)symbolWithDictionary:(NSDictionary *)dict
{
    return [[SymbolModel alloc] initWithDictionary:dict];
}

- (SymbolModel *)initWithDictionary:(NSDictionary *)dict
{
    SymbolModel *symbol = [[SymbolModel alloc] init];
    
    symbol.productID =      [dict objectForKey:INT2STRING(FIX_TAG_BODY_SECURITYID)];//55
    symbol.productName =    [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_PRODUCTNAME)];//8604
    symbol.singlePrice =    [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_SINGLEPRICE)];//dict[@"9605"];
    symbol.totalVolume =    [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_TOTALAMOUNT)];//dict[@"9606"];
    symbol.reserveVolume =  [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_RESERVEVOLMOUNT)];//dict[@"9608"]
    symbol.publicVolume =   [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_ISSUEVOLUME)];//dict[@"9609"];
    symbol.percent =        [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_PRECENT)];//dict[@"9610"];
    symbol.productState =   [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_PRODUCTSTATE)];//dict[@"9612"];
    symbol.tradeState =     [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_TRADESTATUS)];//dict[@"9620"];
    symbol.minimumAsk =     [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_MINNUMASK)];//dict[@"9622"];
    symbol.maxnumAsk =      [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_MAXNUMASK)];//dict[@"9623"];
    symbol.tradeType =      [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_TRADETYPE)];//dict[@"9621"];
    symbol.publicUserID =   [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_PUBLICUSERID)];//dict[@"9622"];
    symbol.signDate =       [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_SIGNDATE)];//dict[@"913"];
    symbol.publicDate =     [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_PUBLICDATE)];//dict[@"9625"];
    symbol.destoryDate =    [dict objectForKey:INT2STRING(FIX_TAG_INTPRTCL_DESTORYDATE)];//dict[@"9627"];
    
    return symbol;
}

@end
