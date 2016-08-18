//
//  NetManager.h
//  CultureTrade
//
//  Created by SZFT1 on 15/11/25.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef void(^finishBlock)(NSMutableArray *stockArray);
typedef void(^errorBlock)(NSError *error);

@interface NetManager : NSObject <NSTradeEngineDelegate>
{
    finishBlock _finishBlock;
    errorBlock _errorBlock;
    NSMutableArray *_stockArray;
    NSString *_stockCurDateString;
}
@property (nonatomic,strong) finishBlock _finishBlock;
@property (nonatomic,strong) errorBlock _errorBlock;
@property (nonatomic,retain) NSString *req_url;
@property (nonatomic,retain) NSString *req_stockCode;
@property (nonatomic,retain) NSString *req_type;

@property (nonatomic,retain) NSString *productID;
@property (nonatomic,retain) NSString *type;
@property (nonatomic,assign) BOOL isSuccessLoad;
@property (nonatomic,assign) BOOL isLoading;
// 回调返回stock对象数组
- (void)loadHisKData:(KLineType)type first:(BOOL)isFitst productID:(NSString *)productID finish:(finishBlock)finishBlock error:(errorBlock)errorBlock; //加载文交所
- (void)loadStockData:(finishBlock)finishBlock error:(errorBlock)errorBlock; // 加载搜狐雅虎财经K线
@end
