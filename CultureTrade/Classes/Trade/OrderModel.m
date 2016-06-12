//
//  OrderModel.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/16.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "OrderModel.h"

@implementation OrderModel
+ (OrderModel *)oderWithDict:(NSDictionary *)dict
{
    return [[OrderModel alloc] initWithDict:dict];
}

- (OrderModel *)initWithDict:(NSDictionary *)dict
{
    OrderModel *order = [[OrderModel alloc] init];
    
    order.askNum = dict[@"132"];               // 132 申请数量
    order.exeState = dict[@"150"];             // 150 执行状态
    order.dealVolume = dict[@"32"];           //32 成交量
    order.orderID = dict[@"37"];               // 37 订单编号
    order.orderType = dict[@"40"];
    order.entrustPrice = dict[@"44"];         //44 委托价格
    order.stockID = dict[@"48"];              //48 证券编号
    order.date = dict[@"9205"];                 // 9205 日期时间
    order.cancerOrderVolume = dict[@"9210"];    // 9210 撤单数量
    order.stockName = dict[@"9402"];            //9402 产品名称
    
    return order;
}

- (void)setDate:(NSString *)date
{
//    // 输入格式
//    NSDateFormatter *inputFormat = [[NSDateFormatter alloc] init];
//    [inputFormat setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
//    NSDate *inputDate = [inputFormat dateFromString:date];
//    // 输出格式
//    NSDateFormatter *outputFormat = [[NSDateFormatter alloc] init];
//    [outputFormat setDateFormat:@"HH:mm:ss"];
//    NSString *formatDate = [outputFormat stringFromDate:inputDate];
//    _date = formatDate;
    _date = [NSString stringFormatFromFullDate:date];
}

@end
