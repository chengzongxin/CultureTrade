//
//  NetManager.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/25.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "NetManager.h"
#import "AsyncURLConnection.h"
#import "Stock.h"

@implementation NetManager
@synthesize _finishBlock = _finishBlock;
@synthesize _errorBlock = _errorBlock;
- (instancetype)init
{
    self = [super init];
    [self initialization];
    return self;
}

- (void)initialization
{
    [NSTradeEngine sharedInstance].delegate = self;
    _stockArray = [NSMutableArray array];
    _stockCurDateString = [NSString string];
    self.isLoading = NO;
    self.req_url = @"http://ichart.yahoo.com/table.csv"; //@"http://ichart.yahoo.com/table.csv?s=399001.SZ&g=d";
    self.req_stockCode = @"601888.SS"; // 股票代码 规则是：沪股代码末尾加.ss，深股代码末尾加.sz。如浦发银行的代号是：600000.SS
    self.req_type = @"d"; // 日K线类型
}

// 回调返回stock对象数组

- (void)loadStockData:(finishBlock)finishBlock error:(errorBlock)errorBlock
{
    MYLOGFUN;
    NSString *url = [NSString stringWithFormat:@"%@?s=%@&g=%@",self.req_url,self.req_stockCode,self.req_type];
    [ASyncURLConnection request:url completeBlock:^(NSData *data) {
        dispatch_async(dispatch_get_main_queue(), ^{
            MYLog(@"get dataing~!");
            NSString *str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[str componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
            [strDataArray removeObjectAtIndex:0];  // 忽略第一个数组字符串  "Date,Open,High,Low,Close,Volume,Adj Close"
            [strDataArray removeLastObject];  // 忽略最后一个空字符串  ""
            NSMutableArray *stockArray = [self loadStockWithArray:strDataArray];
            finishBlock(stockArray);
        });
    } errorBlock:^(NSError *error){
        errorBlock(error);
    }];
}

#pragma mark -
#pragma mark 缓存K线数据到本地
- (void)isFirstUpdate:(NSString *)productIDStr type:(NSString *)typeStr
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSString *fileName = LocalKLineDirectory(productIDStr, typeStr);
    if([fm fileExistsAtPath:fileName] == YES)
    {
        NSLog(@"File is exist!");
    }else{
        NSError *error;
        BOOL success = [fm createDirectoryAtPath:fileName withIntermediateDirectories:YES attributes:nil error:&error];  // 如果没有中间文件夹直接创建
        if (!success) {
            NSLog(@"%@",error);
        }else{
            NSLog(@"%@",[fm contentsOfDirectoryAtPath:fileName error:nil]);
        }
    }
}
// 加载本地
- (void)cacheLocalKLine:(NSString *)productID type:(NSString *)type
{
    
    [self isFirstUpdate:productID type:type];
    NSString *data = [NSString stringWithContentsOfFile:LocalKLineFile(self.productID,type) encoding:NSUTF8StringEncoding error:nil];
    
    NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[data componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    if (strDataArray.count < 10) return;
    _stockArray = [self loadStockWithArray:strDataArray];
    if (_finishBlock == NULL) {
        return;
    }
    _finishBlock(_stockArray);   // finish is null 当迅速转换到K线界面时
}

- (void)loadHisKData:(int)type productID:(NSString *)productID finish:(finishBlock)finishBlock error:(errorBlock)errorBlock
{
    self.type = (type - HISKDATA > 0)?(type - HISKDATA):(type - HISKDATAFIRST);
    self.productID = productID;
    _finishBlock = finishBlock;
    [self cacheLocalKLine:productID type:[NSString stringWithFormat:@"%u",type]];// 加载本地
    
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    [[NSTradeEngine sharedInstance] quote_kline_reqNS:type productID:productID];
    _stockCurDateString = @"";
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
}
// 回调返回stock对象数组
- (void)quote_ui_hisKDataFirst_rsp:(int)type data:(NSString *)data count:(int)count
{
    int iType = (type - HISKDATA > 0)?(type - HISKDATA):(type - HISKDATAFIRST);
//    if (self.type!= iType) return;  // 不是当前需要的K线
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    data = [NSString stringWithFormat:@"%@%@",data,_stockCurDateString];
    NSString *typeStr = [NSString stringWithFormat:@"%d",self.type];
    [data writeToFile:LocalKLineFile(self.productID, typeStr) atomically:YES encoding:NSUTF8StringEncoding error:nil];
    
    NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[data componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    _stockArray = [self loadStockWithArray:strDataArray];
//    [_stockArray addObjectsFromArray:_stockCurDateArray];
    if (_finishBlock == NULL) {
        return;
    }
    _finishBlock(_stockArray);   // finish is null 当迅速转换到K线界面时
}


- (void)quote_ui_hisKDataCurDate_rsp:(int)type data:(NSString *)data count:(int)count
{
    int iType = (type - HISKDATA > 0)?(type - HISKDATA):(type - HISKDATAFIRST);
//    if (self.type != iType) return;  // 不是当前需要的K线
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    if ([data isEqual:@""] || data == nil) {
        MYLog(@"quote_ui_hisKDataCurDate_rsp null data");
        return;
    }
    if ([_stockCurDateString containsString:data]) { // curData 数据先到
        return;
    }
    _stockCurDateString = data;
}


// strDataArray -> strData -> filedArr
- (NSMutableArray *)loadStockWithArray:(NSMutableArray *)strDataArray
{
    if ([[strDataArray lastObject] isEqualToString:@""]) {
        MYLog(@"===============last NULL===============");
        [strDataArray removeLastObject];
    }
    
    NSMutableArray *stockArray = [NSMutableArray array];
    
    for (int i = 0 ; i < strDataArray.count;i ++)
    {
        NSString *strLine = [strDataArray objectAtIndex:i];
        //每一个数组字符串再拆分成N个数组，对应一个stock对象
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        
        Stock *stock = [[Stock alloc] init];
//        stock.date = [format dateFromString:dateString];
        stock.date = (NSString *)filedArr[0];
        
        // Open
        stock.openPrice = [filedArr[1] floatValue]*0.001;
        // High
        stock.highestPrice = [filedArr[2] floatValue]*0.001;
        // Low
        stock.lowestPrice = [filedArr[3] floatValue]*0.001;
        // Close
        stock.closePrice = [filedArr[4] floatValue]*0.001;
        // Volume
        stock.volume = [filedArr[6] intValue];
        
        // MA5,MA10,MA20
        stock.MA5 = [self averageWithMAdata:strDataArray range:NSMakeRange(i, 5)];
        
        stock.MA10 = [self averageWithMAdata:strDataArray range:NSMakeRange(i, 10)];
        
        stock.MA20 = [self averageWithMAdata:strDataArray range:NSMakeRange(i, 20)];
        [stockArray addObject:stock];
        
    }
    
    return stockArray;
}

- (float)averageWithMAdata:(NSArray *)strDataArray range:(NSRange)range
{
    // loc = 0,len = 5 显示的从昨天开始最近5天收盘价,第0个元素是当天的，第0+1个元素是昨天的，从0+1个元素开始算
    float sum = 0; int j = 1;
    for (NSUInteger i = range.location; i < range.location + range.length; i ++) {
        if (range.length > i) {  //很久之前第一天，往前没有数据,就不画那一天的K线
            return 0;
        }

        // loc - i = 46 - (46,5)
        
        NSString *strLine = strDataArray[range.location - j];    //取昨天往前5天的
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        sum += [filedArr[4] floatValue];
        j++;
    }
    return sum / range.length * 0.001;
}

@end
