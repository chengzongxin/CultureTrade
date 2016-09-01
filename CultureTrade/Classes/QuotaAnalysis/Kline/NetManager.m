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

- (void)loadHisKData:(NSString *)productID type:(int)type requestIndex:(int)index finish:(finishBlock)finishBlock error:(errorBlock)errorBlock
{
    self.type = (type - HISKDATA > 0)?(type - HISKDATA):(type - HISKDATAFIRST);
    self.productID = productID;
    _finishBlock = finishBlock;
    [self cacheLocalKLine:productID type:[NSString stringWithFormat:@"%u",type]];// 加载本地
    
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    [[NSTradeEngine sharedInstance] quote_kline_reqNS:productID type:type requestIndex:index];
    _stockCurDateString = @"";
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
}
// 回调返回stock对象数组
- (void)quote_ui_hisKDataFirst_rsp:(int)type data:(NSString *)data count:(int)count
{
    if (self.type!= type) return;  // 不是当前需要的K线
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
    if (self.type != type) return;  // 不是当前需要的K线
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    if ([data isEqual:@""] || data == nil) {
        MYLog(@"quote_ui_hisKDataCurDate_rsp null data");
        return;
    }
    if ([_stockCurDateString hasSuffix:data]) { // curData 数据先到 normal
        return;
    }else{
        _stockCurDateString = data;
    }
}


// strDataArray -> strData -> filedArr
- (NSMutableArray *)loadStockWithArray:(NSMutableArray *)strDataArray
{
    if ([[strDataArray lastObject] isEqualToString:@""] || [strDataArray lastObject] == NULL) {
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
        
        [stockArray addObject:stock];
    }
    // MA5,MA10,MA20,MAVOL5,MAVOL10
    stockArray = [self CalculationMALine:stockArray];
    
    return stockArray;
}



//  calculation by stock object
- (NSMutableArray *)CalculationMALine:(NSMutableArray *)stockArray
{
    for (int i = 0; i < stockArray.count; i++) {
        Stock *stock = stockArray[i];
        stock.MA5 = [self averageWithMAData:stockArray MArange:NSMakeRange(i, 5) expectMAProperty:@"closePrice"];
        stock.MA10 = [self averageWithMAData:stockArray MArange:NSMakeRange(i, 10) expectMAProperty:@"closePrice"];
        stock.MA20 = [self averageWithMAData:stockArray MArange:NSMakeRange(i, 20) expectMAProperty:@"closePrice"];
        stock.MAVOL5 = [self averageWithMAData:stockArray MArange:NSMakeRange(i, 5) expectMAProperty:@"volume"];
        stock.MAVOL10 = [self averageWithMAData:stockArray MArange:NSMakeRange(i, 10) expectMAProperty:@"volume"];
    }
    return  stockArray;
}

- (float)averageWithMAData:(NSArray *)stockArray MArange:(NSRange)MArange expectMAProperty:(NSString *)MAProperty
{
    if (MArange.location < MArange.length - 1) return 0.0f;
    float sum = 0.0f;
    NSArray *needValueArr = [stockArray subarrayWithRange:NSMakeRange(MArange.location - MArange.length + 1, MArange.length)];
    
    for (Stock *s in needValueArr) {
//        unsigned int count;
//        const char *name = [MAProperty UTF8String];
//        objc_property_t properties = class_getProperty([Stock class], name);
        float value = [[s valueForKey:MAProperty] floatValue];
        sum += value;
    }
    return sum/MArange.length;
}

- (void)trade_login_rsp_to_ui:(int)nRet nType:(int)nType
{
    if ([_delegate respondsToSelector:@selector(login_rsp_ui:type:)]) {
        [_delegate login_rsp_ui:nRet type:nType];
    }
}

- (NSMutableArray *)reCalculationMALine:(NSMutableArray *)showArray willShowArray:(NSMutableArray *)willShowArray
{
    NSMutableArray *allArray = [NSMutableArray array];
    // merge
    for (Stock *s in showArray) {
        [allArray addObject:s];
    }
    for (Stock *s in willShowArray) {
        [allArray addObject:s];
    }
    
    for (int i = 0; i < allArray.count; i++) {
        Stock *stock =allArray[i];
        stock.MA5 = [self averageWithMAData:allArray MArange:NSMakeRange(i, 5) expectMAProperty:@"closePrice"];
        stock.MA10 = [self averageWithMAData:allArray MArange:NSMakeRange(i, 10) expectMAProperty:@"closePrice"];
        stock.MA20 = [self averageWithMAData:allArray MArange:NSMakeRange(i, 20) expectMAProperty:@"closePrice"];
        stock.MAVOL5 = [self averageWithMAData:allArray MArange:NSMakeRange(i, 5) expectMAProperty:@"volume"];
        stock.MAVOL10 = [self averageWithMAData:allArray MArange:NSMakeRange(i, 10) expectMAProperty:@"volume"];
    }
    NSArray *array = [allArray subarrayWithRange:NSMakeRange(allArray.count - showArray.count , showArray.count)];
    return [NSMutableArray arrayWithArray:array];
}


/*
//  calculation by NSString Object
- (float)averageWithMAdata:(NSArray *)strDataArray range:(NSRange)range indexOfArray:(int)index
{
    if (range.location < range.length - 1) {  //很久之前第一天，往前没有数据,就不画那一天的K线
        return 0;
    }
    // loc = 0,len = 5 显示的从昨天开始最近5天收盘价,第0个元素是当天的，第0+1个元素是昨天的，从0+1个元素开始算
    float sum = 0; int j = 0;
    for (NSUInteger i = range.location; i < range.location + range.length; i ++) {
        
        // loc - i = 46 - (46,5)
        
        NSString *strLine = strDataArray[range.location - j];    //取昨天往前5天的
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        sum += [filedArr[index] floatValue];
        j++;
    }
    return sum / range.length;
}
 */
@end
