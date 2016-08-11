//
//  NSTradeEngine.m
//  jzjyClient
//
//  Created by zengxw on 13-11-21.
//  Copyright (c) 2013年 MyCompany. All rights reserved.
//
#import "NSTradeEngine.h"
#import "NSTradeBizUtils.h"

#include "nethelper.h"
#include "global.h"
#import "NSNetHelper.h"
#import "TradeEngine.h"
#include "jxtnethelper.h"
#include "TradeEngine.h"
#include "nethelper.h"
#import "msgbase.h"
#import "NSTradeBizUtils.h"
#import "GlobalModel.h"
#import "tagdef.h"
#define kAccountKey     @"Account"
#define kPasswordKey    @"Password"


NSTradeEngine *instanceEngine = nil;

NSString * const DELCARE_NOTIFY  = @"DELCARE_NOTIFY";
NSString * const REFRESH_NOTIFY = @"REFRESH_NOTIFY";

@implementation NSTradeEngine

@synthesize delegate = _delegate;

void on_sign_change_notify(int status) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_sign_change_notify:)])
    {
        [[NSTradeEngine setup].delegate on_sign_change_notify: status];
    }
}


void quote_detail_report_notify(int nSeq, int nRet, char* sybolCode) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_quote_detail_report_notify:nRet:symbol:)])
    {
        NSString* nsymbol = toNString(sybolCode);
        [[NSTradeEngine setup].delegate on_quote_detail_report_notify: nSeq nRet:nRet symbol:nsymbol];
    }
}
void quote_complex_ranking_notify(int nSeq) {
    
}
void quote_real_detail_notify(char* sybolCode) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_quote_real_detail_notify:)])
    {
        NSString* nsymbol = toNString(sybolCode);
        [[NSTradeEngine setup].delegate on_quote_real_detail_notify: nsymbol];
    }
}

void call_funds_flowing(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_funds_flowing_rsp:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_funds_flowing_rsp: nSeq nRet:nRet];
    }
}

void on_apply_delivery_sellte_rsp(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_apply_delivery_sellte_rsp:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_apply_delivery_sellte_rsp: nSeq nRet:nRet];
    }
}

void on_apply_in_store_rsp(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_apply_in_store_rsp:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_apply_in_store_rsp: nSeq nRet:nRet];
    }
}

void on_in_store_order_response(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_op_in_store_order_response:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_op_in_store_order_response:nSeq nRet:nRet];
    }
}

void on_bargain_notify(int nRet) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_bargain_notify:)])
    {
        [[NSTradeEngine setup].delegate on_bargain_notify:nRet];
    }
}


void on_sellte_notify(int nRet) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_sellte_notify:)])
    {
        [[NSTradeEngine setup].delegate on_sellte_notify:nRet]; 
    }
}

void on_outstore_order_notify(int nRet) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_outstore_order_notify:)])
    {
        [[NSTradeEngine setup].delegate on_outstore_order_notify:nRet];
    }
}

void on_apply_instore_notify(int nRet) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_apply_instore_notify:)])
    {
        [[NSTradeEngine setup].delegate on_apply_instore_notify:nRet];
    }
}

void on_cang_order_notify(int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_cang_order_notify:)])
    {
        [[NSTradeEngine setup].delegate on_cang_order_notify:nRet];
    }
}

void on_apply_delivery_notify(int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_apply_delivery_notify:)])
    {
        [[NSTradeEngine setup].delegate on_apply_delivery_notify:nRet];
    }
}

void on_instore_order_notify(int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_instore_order_notify:)])
    {
        [[NSTradeEngine setup].delegate on_instore_order_notify:nRet];
    }
}

void on_cangorder_to_instore_notify(int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_cangorder_to_instore_notify:)])
    {
        [[NSTradeEngine setup].delegate on_cangorder_to_instore_notify:nRet];
    }
}

void op_cang_order_response(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_cang_order_response:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_cang_order_response:nSeq nRet:nRet];
    }
}

void on_cang_to_instore_order_response(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_cang_to_in_store_order_response:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_cang_to_in_store_order_response:nSeq nRet:nRet];
    }
}

void on_trade_apply_delivery_intention_rsp(int nSeq, int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_trade_apply_delivery_intention_rsp:nRet:)])
    {
        [[NSTradeEngine setup].delegate on_trade_apply_delivery_intention_rsp:nSeq nRet:nRet];
    }
}

void on_modify_passwd_rsp(int nRet){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector: @selector(on_modify_passwd_rsp:)])
    {
        [[NSTradeEngine setup].delegate on_modify_passwd_rsp: nRet];
    }
}

void on_delcare_notify(int annId){
    [[NSNotificationCenter defaultCenter] postNotificationName: DELCARE_NOTIFY object: [NSString stringWithFormat:@"%d", annId]];
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector: @selector(on_delcare_notify:)])
    {
        [[NSTradeEngine setup].delegate on_delcare_notify: annId];
    }
}
    
void on_delcare_content_rsp(int nSeq, const char *  szContent){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector: @selector(on_delcare_content_rsp:content:)])
    {
        NSString* str = toNString(szContent);
        [[NSTradeEngine setup].delegate on_delcare_content_rsp: nSeq content: str];
    }
}

//登录应答
void trade_ui_login_rsp(int type, int nRet)
{
    MYLOGFUN;
    MYLog(@"sharedInstance.delegate = %@,%d",[NSTradeEngine sharedInstance].delegate,[[NSTradeEngine sharedInstance].delegate respondsToSelector:@selector(trade_login_rsp_to_ui:nType:)]);
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_login_rsp_to_ui:nType:)])
    {
        [[NSTradeEngine setup].delegate trade_login_rsp_to_ui:nRet nType:type];
    }
}

//登录进度通知
void trade_ui_login_progress_rsp(int progress, int max)
{
    MYLOGFUN;
    MYLog(@"%d,%d",progress,max);
    NSLog(@"trade_ui_login_progress_rsp progress %d", progress);
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector: @selector(trade_ui_login_progress_rsp:max:)])
    {
        [[NSTradeEngine setup].delegate trade_ui_login_progress_rsp: progress max: max];
    }
}

//踢用户下线
void trade_ui_kick_notify ()
{
    [[NSNetHelper setup] trade_ui_kick_notify];
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_kick_notify)])
    {
        [[NSTradeEngine setup].delegate trade_ui_kick_notify];
    }
}




//历史订单刷新通知
void trade_ui_hisorder_rsp (int nSeq, int nRet, const char * ordercodes)
{
//    NSLog(@"trade_ui_hisorder_rsp %d ordercodes:%s", nRet, ordercodes);
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_hisorder_rsp:nRet:symbolcodes:)])
    {
        NSString* ocode = toNString(ordercodes);
        [[NSTradeEngine setup].delegate trade_ui_hisorder_rsp:nSeq nRet:nRet symbolcodes:ocode];
    }
}



//持仓单刷新通知
void trade_ui_holdorder_rsp (int nRet,const char * opencodes, const char * limitcodes, const char * closecodes)
{
//    NSLog(@"trade_ui_holdorder_rsp %d opencodes:%s limitcodes:%s closecodes:%s", nRet, opencodes, limitcodes, closecodes);
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_holdorder_rsp:opencodes:limitcodes:closecodes:)])
    {
        NSString* ocode = toNString(opencodes);
        NSString* lcode = toNString(limitcodes);
        NSString* ccode = toNString(closecodes);
        [[NSTradeEngine setup].delegate trade_ui_holdorder_rsp:nRet
                                                     opencodes:ocode
                                                    limitcodes:lcode
                                                    closecodes:ccode];
    }
}

//资金刷新通知
//void trade_ui_funds_change_rsp (int nRet)
//{
////    NSLog(@"trade_ui_funds_change_rsp %d ", nRet);
//    
//    if( [NSTradeEngine setup].delegate != nil
//       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_funds_change_rsp:)])
//    {
//        [[NSTradeEngine setup].delegate trade_ui_moneyhold_rsp:nRet];
//    }
//}

//下单应答
void trade_ui_openorder_rsp(int nSeq, int nRet, long long orderticket)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_openorder_rsp:nRet:orderticket:)])
    {
        NSString* oticket = [NSString stringWithFormat:@"%lld", orderticket]; //toNString(orderticket);
        [[NSTradeEngine setup].delegate trade_ui_openorder_rsp:nSeq nRet:nRet orderticket:oticket];
    }
}

//汇总持仓视图 商品信息变化
void trade_ui_openorder_stat_rsp (int nRet, const char * symbolcode)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_openorder_stat_rsp:symbolcode:)])
    {
        NSString* scode = toNString(symbolcode);
        [[NSTradeEngine setup].delegate trade_ui_openorder_stat_rsp:nRet symbolcode:scode];
    }
}


void quote_ui_min_data_recv_notify(int nSeq, char* symbolCode)
{
    NSTradeEngine* pEngine = [NSTradeEngine setup];
    if(pEngine.delegate != nil
       && [pEngine.delegate respondsToSelector:@selector(quote_ui_min_data_recv_notify:symbol:)])
    {
        [pEngine.delegate quote_ui_min_data_recv_notify:nSeq symbol:[NSString stringWithUTF8String:symbolCode]];
    }
}

void quote_ui_record_data_recv_notify(int nSeq, char* symbolCode)
{
    NSTradeEngine* pEngine = [NSTradeEngine setup];
    if(pEngine.delegate != nil
       && [pEngine.delegate respondsToSelector:@selector(quote_ui_record_data_recv_notify:symbol:)])
        [pEngine.delegate quote_ui_record_data_recv_notify:nSeq symbol:[NSString stringWithUTF8String:symbolCode]];
}

void quote_ui_his_data_recv_notify(int nSeq, char cType, char* symbolCode)
{
    NSTradeEngine* pEngine = [NSTradeEngine setup];
    if(pEngine.delegate != nil
       && [pEngine.delegate respondsToSelector:@selector(quote_ui_his_data_recv_notify:cType:symbol:)])
    {
        [pEngine.delegate quote_ui_his_data_recv_notify:nSeq cType:cType symbol:[NSString stringWithUTF8String:symbolCode]];
    }
}

void trade_ui_refresh_complete(int nRet){
    [[NSNotificationCenter defaultCenter] postNotificationName: REFRESH_NOTIFY object:[NSString stringWithFormat:@"%d", nRet]];
    NSTradeEngine* pEngine = [NSTradeEngine setup];
    if(pEngine.delegate != nil
       && [pEngine.delegate respondsToSelector:@selector(trade_ui_refresh_complete:)])
    {
        [pEngine.delegate trade_ui_refresh_complete: nRet];
    }
}

void on_history_change_notify(char * symbolcode, int price, long long second) {
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_history_change_notify:price:second:)])
    {
        NSString* str = [NSString stringWithUTF8String:symbolcode];
        [[NSTradeEngine setup].delegate on_history_change_notify:str price:price second:second];
    }
}

//实时行情  2014.11.11
void on_quote_real_notify(char * symbolcode, double price, long long second){
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_quote_real_notify:price:second:)])
    {
        NSString* str = [NSString stringWithUTF8String:symbolcode];
        [[NSTradeEngine setup].delegate on_quote_real_notify:str price:price second:second];
    }
}

#pragma mark -
#pragma mark 今日委托，今日成交
void trade_ui_today_deal_rspUI(const char *data)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_today_deal_rsp:)])
    {
        NSString* nsDataStr = toNString(data);
        [[NSTradeEngine setup].delegate trade_ui_today_deal_rsp:nsDataStr];
    }
}

void trade_ui_today_entrust_rspUI(const char *data)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_today_entrust_rsp:)])
    {
        NSString* nsDataStr = toNString(data);
        [[NSTradeEngine setup].delegate trade_ui_today_entrust_rsp:nsDataStr];
    }
}

#pragma mark -
#pragma mark 历史委托，历史成交
void trade_ui_historyorder_rspUI(int nSeq,char *orders)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_historyorder_rsp:orders:)])
    {
        NSString* nsDataStr = toNString(orders);
        [[NSTradeEngine setup].delegate trade_ui_historyorder_rsp:nSeq orders:nsDataStr];;
    }
}

void trade_ui_historyturnover_rspUI(int nSeq,char *deals)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_historyturnover_rsp:deals:)])
    {
        NSString* nsDataStr = toNString(deals);
        [[NSTradeEngine setup].delegate trade_ui_historyturnover_rsp:nSeq deals:nsDataStr];
    }
}

void trade_ui_pretrade_rspUI(int nSeq,char *pretrades)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_pretrade_rsp:pretrade:)])
    {
        NSString* nsDataStr = toNString(pretrades);
        [[NSTradeEngine setup].delegate trade_ui_pretrade_rsp:nSeq pretrade:nsDataStr];
    }
}

void trade_ui_canceltrade_rspUI(int nSeq,char *canceltrade)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_canceltrade_rsp:canceltrades:)])
    {
        NSString* nsDataStr = toNString(canceltrade);
        [[NSTradeEngine setup].delegate trade_ui_canceltrade_rsp:nSeq canceltrades:nsDataStr];
    }
}

void trade_ui_sharehold_rspUI(int nRet,const char *data)
{
    NSString* nsDataStr = toNString(data);
    NSData *binarydata = [nsDataStr dataUsingEncoding:NSUTF8StringEncoding];
    NSArray *jsonArray = [NSJSONSerialization JSONObjectWithData:binarydata options:0 error:nil];
    
    GLOBAL.shareHoldArray = [NSMutableArray array];
    for (NSDictionary *dict in jsonArray) {
        ShareHold *shareHold = [ShareHold shareHoldWithDictionary:dict];
        [GLOBAL.shareHoldArray addObject:shareHold];
    }
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_sharehold_rsp:sharehold:)])
    {
        
        [[NSTradeEngine setup].delegate trade_ui_sharehold_rsp:nRet sharehold:nsDataStr];
    }
}

void quote_ui_login_rspUI(int nRet)
{
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_login_rsp:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_login_rsp:nRet];
    }
}
// 历史K线回调
void quote_ui_hisKDataFirst_rspUI(int type,unsigned char *data,int count)
{
    NSString *nsData = [NSString toNSString:(char *)data];
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_hisKDataFirst_rsp:data:count:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_hisKDataFirst_rsp:type data:nsData count:count];
    }
}

#pragma mark -
#pragma mark 当日K线
void quote_ui_hisKDataCurDate_rspUI(int type,unsigned char *data,int count)
{
    NSString *nsData = [NSString toNSString:(char *)data];
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_hisKDataCurDate_rsp:data:count:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_hisKDataCurDate_rsp:type data:nsData count:count];
    }
}

// 实时数据
void quote_ui_stkDealData_rspUI(unsigned char *data)
{
    NSString *nsData = [NSString toNSString:(char *)data];
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_stkDealData_rsp:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_stkDealData_rsp:nsData];
    }
}


void quote_ui_initMarket_rspUI(const char *data,int pLen)
{
    NSString *nsstr = [NSString toNSString:data];
    GLOBAL.stockInfoArray = [NSMutableArray array];
    NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[nsstr componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    [strDataArray removeLastObject]; //移除最后一行空元素
    for (int i = 0; i < strDataArray.count; i++)
    {
        NSString *strLine = [strDataArray objectAtIndex:i];
        //判断是否有空置
        
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        
        if (filedArr.count <= 2) {  // 不包含逗号，都是商品名
            GLOBAL.stockNameArray = strDataArray;
            return;
        }
        if (filedArr.count < 8) continue;
        StockInfoNS *stockInfo =    [[StockInfoNS alloc] init];
        stockInfo.m_uiCode =        [filedArr[0] intValue];
        stockInfo.m_usMarketType =  [filedArr[1] intValue];
        stockInfo.m_uiPrevClose =   [filedArr[2] intValue]*0.001;
        stockInfo.m_ui5DayVol =     [filedArr[3] intValue];
        stockInfo.m_uiUpLimit =     [filedArr[4] intValue]*0.001;
        stockInfo.m_uiDownLimit =   [filedArr[5] intValue]*0.001;
        stockInfo.m_uiShares =      [filedArr[6] intValue];
        stockInfo.m_uiReserves =    [filedArr[7] intValue];
        stockInfo.m_sAsset =        [filedArr[8] intValue];
        stockInfo.m_acName =        GLOBAL.stockNameArray[i];
//        stockInfo.m_acSpell =       filedArr[1];
        [GLOBAL.stockInfoArray addObject:stockInfo];
    }
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_initMark_rsp:len:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_initMark_rsp:nsstr len:pLen];
    }
    [[NSTradeEngine sharedInstance] requestPankouData];
}

- (float)getPreClosePrice:(int)productID
{
    for (StockInfoNS *s in GLOBAL.stockInfoArray) {
        if (s.m_uiCode == productID) {
            return s.m_uiPrevClose;
        }
    }
    return 0;
}

//商品信息变化 symbolcodes
void trade_ui_symbol_change_notice_rspUI(int nRet, const char * allSymbols)
{
    NSString* symbols = toNString(allSymbols);
    NSData *data = [symbols dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    NSArray *array = [jsonDic objectForKey:@"SYMBOLS"];
    GLOBAL.symbolArray = [NSMutableArray array];
    for (NSDictionary *dict in array) {
        SymbolModel *s = [SymbolModel symbolWithDictionary:dict];
        if ([s.productState isEqualToString:@"\u0002"])
            [GLOBAL.symbolArray addObject:s];  // 只保留状态为2的
    }
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_symbol_change_notice:symbolcodes:)])
    {
        
        [[NSTradeEngine setup].delegate trade_ui_symbol_change_notice:nRet symbolcodes:symbols];
    }
    
//    [[NSTradeEngine sharedInstance] requestPankouData]; // 请求盘口数据
}

- (void)requestPankouData
{
    NSString *products = [NSString string];
    for (StockInfoNS *s in GLOBAL.stockInfoArray) {
        if ([products isEqualToString:@""]) {
            products = [NSString stringWithFormat:@"%d",s.m_uiCode];
        }else{
            products = [NSString stringWithFormat:@"%@,%d",products,s.m_uiCode];
        }
    }
    [[NSTradeEngine sharedInstance] sendReportData:products]; // 请求盘口数据
}

void quote_ui_pankou_rspUI(const char *data,int len)    // 盘口数据应答
{
    NSString *nsData = [NSString toNSString:(char *)data];
    GLOBAL.sortUnitArray = [NSMutableArray array];
    NSMutableArray *strArray = [NSMutableArray arrayWithArray:[nsData componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    [strArray removeLastObject];//移除最后一个空元素
    for (int i = 0; i<strArray.count; i++) {
        NSString *strLine = strArray[i];
        NSArray *fieldArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        SortUnit *sortUnit = [SortUnit sortUnitWithArray:fieldArr]; // 拆分数组
        if (sortUnit.m_CodeInfo.m_uiCode < 0) break;
        
        for (StockInfoNS *initMarket in GLOBAL.stockInfoArray) { // fill SortUnit._uiPreclose,productname
            if (initMarket.m_uiCode == sortUnit.m_CodeInfo.m_uiCode) {
                sortUnit.m_uiPreClose = initMarket.m_uiPrevClose;
                sortUnit.productName = initMarket.m_acName;
                sortUnit.m_uiShares = initMarket.m_uiShares;//流通股数
                sortUnit.m_uiReserves = initMarket.m_uiReserves;//库存量
                sortUnit.m_ui5DayVol = initMarket.m_ui5DayVol;//五日平均总手
                sortUnit.m_usOpenTime = 0;//开盘时间
            }
        }
        
        [GLOBAL.sortUnitArray addObject:sortUnit];
    }
    
    
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(quote_ui_pankou_rsp:len:)])
    {
        [[NSTradeEngine setup].delegate quote_ui_pankou_rsp:nsData len:len];
    }
}


void trade_ui_moneyhold_rspUI(char *data)
{
    cJSON *root;
    cJSON *retcode;
    cJSON *retinfo;
    root=cJSON_Parse(data);
    int nRet = -1;
    if (root){
        retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
        retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
        if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
            cJSON *jNode = NULL;
            GLOBAL.moneyHold = [[MoneyHold alloc] init];
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTID);
            if(NULL != jNode) GLOBAL.moneyHold.accountID = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTBALANCE);
            if(NULL != jNode) GLOBAL.moneyHold.accountBalance = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTFREEZEMONEY);
            if(NULL != jNode) GLOBAL.moneyHold.accountFreeze = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_CLEARDATE);
            if(NULL != jNode) GLOBAL.moneyHold.clearDate = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_TOTALBALANCE);
            if(NULL != jNode) GLOBAL.moneyHold.totalBalance = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_CANOUT);
            if(NULL != jNode) GLOBAL.moneyHold.canOut = atof(jNode->valuestring);
            
            jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_OTHERFREEZE);
            if(NULL != jNode) GLOBAL.moneyHold.otherFreeze = atof(jNode->valuestring);
            
            if( [NSTradeEngine setup].delegate != nil
               && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_moneyhold_rsp:)])
            {
                [[NSTradeEngine sharedInstance].delegate trade_ui_moneyhold_rsp:1];
            }
            
        } else if (NULL != retcode){
            if( [NSTradeEngine setup].delegate != nil
               && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_moneyhold_rsp:)])
            {
                [[NSTradeEngine sharedInstance].delegate trade_ui_moneyhold_rsp:0];
            }
        }
    }
    cJSON_Delete(root);
}

void on_inout_money_rsp(int nSeq,int nRet,const char *data){
    NSString *nsData = [NSString toNSString:(char *)data];
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(on_inout_money_rsp:nRet:data:)])
    {
        [[NSTradeEngine setup].delegate on_inout_money_rsp:nSeq nRet:nRet data:nsData];
    }
}



void trade_ui_transferquery_rspUI(int nRet,const char *data)
{
//    cJSON *root = cJSON_Parse(data);
//    cJSON *results = cJSON_GetObjectItem(root, "INTPRTCL");
//    char *cResults = cJSON_Print(results);
    NSString *nsData = [NSString stringWithFormat:@"%s",data];//[NSString toNSString:(const char *)cResults];
    if( [NSTradeEngine setup].delegate != nil
       && [[NSTradeEngine setup].delegate respondsToSelector:@selector(trade_ui_transferquery_rsp:results:)])
    {
        [[NSTradeEngine setup].delegate trade_ui_transferquery_rsp:nRet results:nsData];
    }
}

void cache_user_pwd_ui(char **user,char **pwd)
{
    *user = toCString([[NSUserDefaults standardUserDefaults] objectForKey:kAccountKey]);
    
    *pwd = toCString([[NSUserDefaults standardUserDefaults] objectForKey:kPasswordKey]);
}

+(NSTradeEngine*) sharedInstance {
    if (instanceEngine == nil) {
        instanceEngine = [[NSTradeEngine alloc] init];
        init_engine();
        [instanceEngine init_trade_engine];
        //        const char * path = toCString(DocumentDirectory());
        //        setUserLogFileDirectoryPath(path);
    }
    return instanceEngine;
    
}
- (NSString *)getBankName
{
    const char *bankname = getBankName();
    NSString *nsBankname = [NSString toNSString:(char *)bankname];
    return nsBankname;
}

- (float)getNewPrice:(int)code
{
    for (SortUnit *s in GLOBAL.sortUnitArray) {
        if (s.m_CodeInfo.m_uiCode == code)
            return s.m_uiNewPrice;
    }
    return NULL;
}


+(NSTradeEngine*)setup
{
    return [NSTradeEngine sharedInstance];
}

-(NSString *)getNativeVersion{
    return toNString(getVersion());
}

-(void) call_refresh_account_info
{
    refresh_account_info();
}

-(int)sendTraceDataReq:(NSString*)codeName
{
    if([codeName length] > 0)
    {
        const char* str = [codeName UTF8String];
        return nhp_send_quote_STK_trace_data_req(str, 200);
    }
    return 0;
}

-(int)sendMinDataReq:(NSString*)codeName
{
    if([codeName length] > 0)
    {
        const char* str = [codeName UTF8String];
        return nhp_send_quote_STK_min_data_req(str, (int)[codeName length]);
    }
    return 0;
}

-(int)sendKLineDataReq:(NSString*)codeName kLineType:(int)nType
{
    if([codeName length] > 0)
    {
        const char* str = [codeName UTF8String];
        //需要得到个数
        int nCount = getSymbolKLineDataCount(str, nType);
        return nhp_send_quote_his_data_req((const char*)str, nType, nCount, 2);//,0);
    }
    return 0;
}

-(int)sendKLineDataReq:(NSString*)codeName kLineType:(int)nType kLineCount:(int)nReqCount
{
    if([codeName length] > 0) {
        const char* str = [codeName UTF8String];
        //需要得到个数
        int nCount = getSymbolKLineDataCount(str, nType);
        nReqCount -= nCount;
        return nhp_send_quote_his_data_req((const char*)str, nType, nCount, nReqCount <=0 ? 0 : nReqCount);
    }
    return 0;
}

-(int)sendDefaultKLineReq:(NSString*)codeName kLineType:(int)nType
{
    if([codeName length] > 0)
    {
        const char* str = [codeName UTF8String];
        return nhp_send_quote_his_data_req((const char*)str, nType, 0, 2);
    }
    return 0;
}

//-(void)sendSymbolDataInfo
//{
//    nhp_send_symbol_data_info_req();
//}
//-(void)sendInitMarket
//{
//    nhp_send_symbol_data_info_req();
//}
//
//-(void)sendReportData
//{
//    nhp_send_report_data_info_req();
//    nhp_send_zyj_report_data_info_req();
//}

-(BOOL)symbolDataForCode:(NSString*)symbolCode beginTime:(int*)pBeginTime totalTime:(int*)pTotalTime
{
   char* code = (char*)symbolCode.UTF8String;
   if(get_symbol_data_min_begin_end_time(code, pBeginTime, pTotalTime))
       return  YES;
    return NO;
}

// 刷新盘口
-(void)sendReportData:(NSString *)productIDs
{
    char *cProductIDs = toCString(productIDs);
    quote_reportData_req(cProductIDs);
}

//请求商品信息
-(void) trade_request_symbol
{
    trade_request_symbol();
}

//查历史单据 %04d-%02d-%02d
-(int) trade_request_hisorder:(NSString *)startdate enddate:( NSString *)enddate
{
    char * sdate = toCString(startdate);
    char * edate = toCString(enddate);
    return trade_request_hisorder(sdate, edate);	
}


//查历史单据 %04d-%02d-%02d
-(int) trade_request_zyj_hisorder:(NSString *)startdate enddate:( NSString *)enddate
{
   const char * sdate = toCString(startdate);
   const char * edate = toCString(enddate);
    return trade_request_zyj_hisorder(sdate, edate);
}

//查持仓单
-(void) trade_request_order
{
    trade_request_order();
}

//查资金
-(void) trade_request_funds
{
    trade_request_funds();
}

- (int)trade_request_historyorder_fromDate:(NSString *)fromDate toDate:(NSString *)toDate
{
    char * startDate = toCString(fromDate);
    char * endDate = toCString(toDate);
    return trade_request_historyorder(startDate, endDate);
}
- (int)trade_request_historyturnover_fromDate:(NSString *)fromDate toDate:(NSString *)toDate
{
    char * startDate = toCString(fromDate);
    char * endDate = toCString(toDate);
    return trade_request_historyturnover(startDate, endDate);
}



//下限价单
//-(int)trade_open_limit_order:(NSString *)symbolcode  shoushu:(double)shoushu  sl:(double)sl  tp:(double)tp  ordertype:(char)ordertype price:( double) price type:(char)type date:(NSString*)date
//{
//    char * cpSymbol = toCString(symbolcode);
//    char * cpDate = toCString(date);
//    return trade_open_limit_order(cpSymbol, shoushu, sl, tp, ordertype, price, type, cpDate);
//}

//-(NSString*)getNSStringLimitHint:(NSString *)symbolcode ordertype:(char)ordertype
//{
//    char * cpSymbol = toCString(symbolcode);
//    const char* hit = getLimitHintToString(cpSymbol, ordertype);
//    return toNString(hit);
//}

//修改持仓单
-(int) trade_modify_open_order:(NSString *)orderticket sl:(double)sl  tp:( double) tp
{
    //char * cpOrderticket = toCString(orderticket);
    long long cpOrderticket = orderticket.longLongValue;
    return trade_modify_open_order(cpOrderticket, sl, tp);
}

//修改限价单
-(int)trade_modify_limit_order:(NSString *)orderticket price:( double) price  sl:( double) sl  tp:( double) tp
{
    //char * cpOrderticket = toCString(orderticket);
    long long cpOrderticket = orderticket.longLongValue;
    return trade_modify_limit_order(cpOrderticket, price, sl, tp);
}

//删除限价单
-(int)trade_cancel_limit_order:(NSString *)orderticket
{
//    char * cpOrderticket = toCString(orderticket);
    long long cpOrderticket = orderticket.longLongValue;
    return trade_cancel_limit_order(cpOrderticket);
}

//平仓
-(int) trade_close_order:(NSString *)orderticket shoushu:( double) shoushu rqty:(double)rqty diff:(int)diff
{
//    char * cpOrderticket = toCString(orderticket);
    long long cpOrderticket = orderticket.longLongValue;
    return trade_close_order(cpOrderticket, shoushu, rqty,diff);
}

//撤消
-(int) trade_cancel_zyj_order:(NSString *)orderticket{
    long long cpOrderticket = orderticket.longLongValue;
    return trade_cancel_zyj_order(cpOrderticket);
}

//开仓
-(int) trade_open_limit_zyj_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price {
    char* pCode = toCString(symbolcode);
    return trade_open_limit_zyj_order(direction, pCode, shoushu, price);
}
//平今仓
-(int) trade_close_limit_zyj_day_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price {
    char* pCode = toCString(symbolcode);
    return trade_close_limit_zyj_day_order(direction, pCode, shoushu, price);

}
//平昨仓
-(int) trade_close_limit_zyj_yesterday_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price {
    char* pCode = toCString(symbolcode);
    return trade_close_limit_zyj_yesterday_order(direction, pCode, shoushu, price);

}

//获取用户属性
//用户账号
 -(NSString *)getUserAccount
{
    return toNString(getUserAccount());
}

//用户状态
-(int) getUserStatus
{
    return  getUserStatus();
}

//用户名
 -(NSString *)getUserName
{
    return toNString(getUserName());
}
//session
-(NSString *)getSession
{
    return toNString(getSession());
}


-(NSString *)getLoginUser{
    return toNString(getLoginUser());
}

//获取商品属性
//获取所有商品编码
 -(NSString *)getAllSymbolCode
{
    return toNString(getAllSymbolCode());
}

//商品名
 -(NSString *)getSymbolname:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    return toNString(getSymbolname(cpSymbol));
}

-(int) getSymbolType:(NSString *)symbolcode{
    char * cpSymbol = toCString(symbolcode);
    return getSymbolType(cpSymbol);
}

-(NSString*)getStrPrice:(double)price dot:(int)dot
{
    NSString * strprice = @"";
    if (dot == 0) {
        strprice = [NSString stringWithFormat:@"%d", (int)price];
    } else if( 1 == dot) {
        strprice = [NSString stringWithFormat:@"%0.1f", price];
    } else if( 2 == dot) {
        strprice = [NSString stringWithFormat:@"%0.2f", price];
    }  else if( 3 == dot) {
        strprice = [NSString stringWithFormat:@"%0.3f", price];
    } else if( 4 == dot) {
        strprice = [NSString stringWithFormat:@"%0.4f", price];
    } else if( 5 == dot) {
        strprice = [NSString stringWithFormat:@"%0.5f", price];
    } else if( 6 == dot) {
        strprice = [NSString stringWithFormat:@"%0.6f", price];
    } else {
        strprice = [NSString stringWithFormat:@"%0.5f", price];
    }
    return strprice;
}

//买价
-(NSString*) getAskPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getAskPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    NSString* strAskPrice = [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
//    NSLog(@"symbol [%@] strAskPrice [%@]", symbolcode, strAskPrice);
    return strAskPrice;
}

//卖价
-(NSString*) getBidPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getBidPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    NSString* strBidPrice =  [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
//    NSLog(@"symbol [%@] strBidPrice [%@]", symbolcode, strBidPrice);
    return strBidPrice;
}

//行情时间
-(long long)getLastTick:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    return getLastTick(cpSymbol);
}

//最高
-(NSString*) getHighPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getHighPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//最低
-(NSString*) getLowPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getLowPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//昨收价
-(NSString*) getYesterDayClosePrice:(NSString *)symbolcode{
    char * cpSymbol = toCString(symbolcode);
    double db = getYesterDayClosePrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//昨结价
-(NSString*) getSettlPrice:(NSString *)symbolcode{
    char * cpSymbol = toCString(symbolcode);
    double db = getSettlPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//开盘价
-(NSString*) getOpenPrice:(NSString *)symbolcode {
    char * cpSymbol = toCString(symbolcode);
    double db = getOpenPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//最新价
-(NSString*) getLastPrice:(NSString *)symbolcode{
    char * cpSymbol = toCString(symbolcode);
    double db = getLastPrice(cpSymbol);
    if(db < 0.0) return PATTER;
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

-(NSString*) getIntPriceBySymbol:(NSString *)symbolcode price:(int)price{
    char * cpSymbol = toCString(symbolcode);
    int decimal = getPriceDecimal(cpSymbol);
    double db = price / pow(10.0, decimal);
    return [self getStrPrice:db dot: decimal];
}

-(NSString*) getIntQty:(int)qty{
    double db = qty / pow(10.0, 2);
    return [self getStrPrice:db dot: 2];
}

//价格涨跌方向 -1跌 0平 1涨
-(int)getPriceDirection:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    return getPriceDirection(cpSymbol);
}

-(NSString*)getNormalPrice:(int)nPrice symbolNameCode:(NSString*)symbolNameCode
{
    char * cpSymbol = toCString(symbolNameCode);
    int dot = getPriceDecimal(cpSymbol);
    double d = nPrice * 1.0 / pow(10, dot);
    return [self getStrPrice:d dot:dot];
}

//获取订单属性
//汇总
//获取持仓单商品集合
 -(NSString *)getHoldSymbolCodes
{
    return toNString(getHoldSymbolCodes());
}
-(NSString*)getHoldTicketsBySymbol:(NSString*)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    return toNString(getHoldTicketsBySymbol(cpSymbol));
}

//持仓商品的总买单量
-(NSString*) getHoldTotalBuyNum:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getHoldTotalBuyNum(cpSymbol);
    return [self getStrPrice:db dot:2];
}

//持仓商品的总卖单量
-(NSString*) getHoldTotalSellNum:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getHoldTotalSellNum(cpSymbol);
    return [self getStrPrice:db dot:2];
}
//持仓商品的总利息
-(NSString*) getHoldTotalSumInterest:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getHoldTotalSumInterest(cpSymbol);
    return [self getStrPrice:db dot:2];
}
//持仓商品的总佣金
-(NSString*)getHoldTotalCommision:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getHoldTotalCommision(cpSymbol);
    return [self getStrPrice:db dot:2];
}

-(int) getDecimal:(NSString*) symbolcode{
    char * cpSymbol = toCString(symbolcode);
    return getPriceDecimal(cpSymbol);
}

//持仓商品的总浮动盈亏
-(NSString*) getHoldTotalFpl:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getHoldTotalFpl(cpSymbol);
    return [self getStrPrice:db dot:2];
}
//获取限价单商品集合
 -(NSString *)getLimitSymbolCodes
{
    return  toNString(getLimitSymbolCodes());
}

-(NSString*)getLimitTicketsBySymbol:(NSString*)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    return toNString(getLimitTicketsBySymbol(cpSymbol));
}

//限价商品的总买单量
-(NSString*) getLimitTotalBuyNum:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getLimitTotalBuyNum(cpSymbol);
    return [self getStrPrice:db dot:2];
}
//限价商品的总卖单量
-(NSString*) getLimitTotalSellNum:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db =  getLimitTotalSellNum(cpSymbol);
    return [self getStrPrice:db dot:2];
}

//限价商品的平均买价
-(NSString*)getLimitAverageBuyPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getLimitAverageBuyPrice(cpSymbol);
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//限价商品的平均卖价
-(NSString*) getLimitAverageSellPrice:(NSString *)symbolcode
{
    char * cpSymbol = toCString(symbolcode);
    double db = getLimitAverageSellPrice(cpSymbol);
    return [self getStrPrice:db dot:getPriceDecimal(cpSymbol)];
}

//获取所有持仓单单号
 -(NSString *)getHoldOrderAllTickets
{
    return toNString(getHoldOrderAllTickets());
}

//获取所有限价单单号
 -(NSString *)getLimitOrderAllTickets
{
    return toNString(getLimitOrderAllTickets());
}

//获取历史单据单号
 -(NSString *)getHisOrderAllTickets
{
    return toNString(getHisOrderAllTickets());
}

//获取历史单据单号
-(NSString *)getHisZYJOrderAllTickets
{
    return toNString(getHisZYJOrderAllTickets());
}

-(char) getOpenClose:(NSString *)ticket{
    char * cpTicket = toCString(ticket);
    return getOpenClose(cpTicket);
}

//属性
-(int) getTradeOrderType:(NSString *)ticket{
    char * cpTicket = toCString(ticket);
    return getTradeOrderType(cpTicket);
}

-(char) getOrderType:(NSString *)ticket{
    char * cpTicket = toCString(ticket);
    return getOrderType(cpTicket);
}

//商品
 -(NSString *)getSymbol:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getSymbol(cpTicket));
}

//订单时间(挂单开仓时间)
-(NSString*)getOrderTime:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getOrderTime(cpTicket));
}

//子订单类型
-(char)getSubOrderType:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return getSubOrderType(cpTicket);
}

//订单价格(挂单开仓价格)
-(NSString*)getOrderPrice:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getOrderPrice(cpTicket);
//    NSString * symbolcode = [self getSymbol:ticket];
//    return [self getStrPrice:db dot:getPriceDecimal([utilsGlobal getCStringFromNSString:symbolcode])];
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}//orderprice	订单价格	-(double)	10,4

//订单数量(挂单手数)
-(NSString*) getOrderQty:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getOrderQty(cpTicket);
    return [self getStrPrice:db dot:2];
}

//止盈价格
-(NSString*)getSlPrice:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getSlPrice(cpTicket);
//    NSString * symbolcode = [self getSymbol:ticket];
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}//止盈价格	double	10,4

//止损价格
-(NSString*) getStopPx:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getStopPx(cpTicket);
//    NSString * symbolcode = [self getSymbol:ticket];
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}//tpprice	止损价格	double	10,4

//买卖方向
-(char) getSide:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return getSide(cpTicket);
}//direction	买/卖	Char 	1

//开仓 开仓价格
-(NSString*) getExecuteprice:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getExecuteprice(cpTicket);
    char * symbolcode = toCString([self getSymbol:ticket]);
//    return [self getStrPrice:db dot:getPriceDecimal([utilsGlobal getCStringFromNSString:symbolcode])];
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}
-(NSString*) getCumQty:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getCumQty(cpTicket);
    return [self getStrPrice:db dot:2];
}
-(NSString*)getTransactTime:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getTransactTime(cpTicket));
}
-(NSString*) getFPL:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    const char * cpSymbol = getSymbol(cpTicket);
    int symbolType =  getSymbolType((char*)cpSymbol);
    if(symbolType == ZS_TYPE){
        if(getAskPrice((char*)cpSymbol) <= 0) return PATTER;
    } else {
        if(getCurrPrice((char*)cpSymbol) <= 0) return PATTER;
    }
    double db = getFPL(cpTicket);
    return [self getStrPrice: db dot: 2];
}
-(int) getFPLDirection:(NSString *)ticket
{
   char * cpTicket = toCString(ticket);
    double db = getFPL(cpTicket);
    if(db>0) return 1;
    else if(db<0) return -1;
    else return 0;
}

-(int)getTicketPriceDecimal:(NSString*)ticket
{
    char * cpTicket = toCString(ticket);
    return getTicketPriceDecimal(cpTicket);
}

//利息
-(NSString*)getSumInterest:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getSumInterest(cpTicket);
    return [self getStrPrice:db dot:2];
}//累计递延费	Double 	20,4

//开仓价格
-(NSString*) getOpenOrderPrice:(NSString *)ticket {
    char * cpTicket = toCString(ticket);
    double db = getOpenOrderPrice(cpTicket);
    return [self getStrPrice:db dot:2];
}

//佣金 (开仓手续费 + 平仓手续费)
-(NSString*) getCommision:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getCommision(cpTicket);
    return [self getStrPrice:db dot:2];
}


-(NSString*) getCloseCommision:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getCloseCommision(cpTicket);
    return [self getStrPrice:db dot:2];
}

-(NSString*) getOpenCommision:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getOpenCommision(cpTicket);
    return [self getStrPrice:db dot:2];
}

//平仓单号
 -(NSString *)getCloseticket:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getCloseticket(cpTicket));
}
//平仓价格
-(NSString*) getCloseprice:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getCloseprice(cpTicket);
//    NSString * symbolcode =  [self getSymbol:ticket];
//    return [self getStrPrice:db dot:getPriceDecimal([utilsGlobal getCStringFromNSString:symbolcode])];
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}

//平仓数量
-(NSString*) getCloseordersize:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getCloseordersize(cpTicket);
    return [self getStrPrice:db dot:2];
}

//平仓时间
-(NSString*)getClosetime:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getClosetime(cpTicket));
}

//盈亏
-(NSString*) getPL:(NSString *)ticket
{
    char * cpTicket = toCString(ticket);
    double db = getPL(cpTicket);
    return [self getStrPrice:db dot:2];
}


-(char)getExpirationType:(NSString*) ticket
{
    char * cpTicket = toCString(ticket);
    return getExpirationType(cpTicket);
}

-(NSString*)getExpiration:(NSString*) ticket
{
    char * cpTicket = toCString(ticket);
    return toNString(getExpiration(cpTicket));
}

-(char)getOrderStatus:(NSString*)ticket{
    char * cpTicket = toCString(ticket);
    return getOrderStatus(cpTicket);
}

//获取资金属性

//可用保证金
-(NSString*)getBalanceMargin
{
    double db = getBalanceMargin();
    return [self getStrPrice:db dot:2];
}

-(NSString*)getFreezeMargin
{
    double db = getFreezeMargin();
    return [self getStrPrice:db dot:2];
}

-(NSString*)getTotalFPL
{
    double db = getTotalFPL();
    return [self getStrPrice:db dot:2];
}

//占用保证金
-(NSString*)getUsedMargin
{
    double db= getUsedMargin();
    return [self getStrPrice:db dot:2];
}

//余额 (可用 +　占用　＋ 冻结)
-(NSString*)getBalance
{
    double db = getBalance();
    return [self getStrPrice:db dot:2];
}

-(NSString*) getTmpValue {
    double db = getTmpValue();
    return [self getStrPrice:db dot:2];
}

-(NSString*)getTotalClosePL{
    double db = getTotalClosePL();
    return [self getStrPrice:db dot:2];
}

-(NSString*) getRisk{
    double db = getRisk();
    if(db<=0.0) return @"--";
    return [[self getStrPrice:db dot:2] stringByAppendingString: @"%"];
}

-(int) getRiskLevel{
    return getRiskLevel();
}

-(NSString*)getHisorderAllPL
{
    double db = getHisorderAllPL();
    return [self getStrPrice:db dot:2];
}

//获取所有平仓单单号
-(NSString*)getCloseOrderAllTickets
{
    return toNString(getCloseOrderAllTickets());
}
//获取当天平仓单的总盈亏
-(NSString*)getCloseorderAllPL
{
    double db = getCloseorderAllPL();
    return [self getStrPrice:db dot:2];
}

//是否登录成功
-(BOOL) isLogin{
    NSString* str =[self getLoginedAccountName];
    return nil != str && str.length > 0 ? YES : NO;
}

//
-(NSString * ) getLoginedAccountName{
    return toNString(getUserName());
}

-(double) getCurrPrice:(NSString *)symbolcode{
    char * cpTicket = toCString(symbolcode);
    return getCurrPrice(cpTicket);
}

-(int) getMarketHint:(NSString*) symbolcode orderType:(char)sub_order_type hint:(SL_TP_Hint*)hint
{
    char * cpsymbolcode = toCString(symbolcode);
    return getMarketHint(cpsymbolcode, sub_order_type, hint);
}

-(int) getLimitHint:(NSString*) symbolcode price:(double)price orderType:(char)sub_order_type hint:(SL_TP_Hint*)hint;
{
    char * cpsymbolcode = toCString(symbolcode);
    return getLimitHint(cpsymbolcode, price, sub_order_type, hint);
//    hint2->price =hint.price;
////    NS_SL_TP_Hint* h = [[NS_SL_TP_Hint alloc] init];
////    h.price = toNString(hint.price);
//    printf("NNN  price=%s sl=%s, tp=%s\n",hint.price, hint.SlPrice, hint.TpPrice);
//    return hint;
}



/*公告接口*/
-(int) getDeclareAnnouceSize{
    return getDeclareAnnouceSize();
}
-(NSString*) getDeclareAnnouceID{//公告ID
    return toNString(getDeclareAnnouceID());
}
-(long long) getDeclareTime:(int) annouceID{
    return getDeclareTime(annouceID);
}
-(NSString*) getDelcare:(int) annouceID{//公告发布者
    return toNString(getDelcare(annouceID));
}
-(NSString*) getDelcareTitle:(int) annouceID {//公告标题
    return toNString(getDelcareTitle(annouceID));
}
-(NSString*) getDelcareContent:(int) annouceID {//公告内容
    return toNString(getDelcareContent(annouceID));
}
-(char) getDelcareReadStatus:(int) annouceID {//阅读状态 1已阅读 2 未阅读
    return getDelcareReadStatus(annouceID);
}


-(int) trade_request_modify_passwd:(NSString *)type oldpass:(NSString*)oldpass newpass:(NSString*)newpass {
    const char* ptype = toCString(type);
    const char* poldpass = toCString(oldpass);
    const char* pnewpass = toCString(newpass);
    return trade_request_modify_passwd(ptype, poldpass, pnewpass);
}
/* END*/

-(int)trade_request_declare_title{
    return trade_request_declare_title();
}

-(int)trade_request_declare_content:(int)aid{
    return trade_request_declare_content(aid);
}

-(NSString*) getTicketByBargainId:(NSString*)ticket{
    char* pp = toCString(ticket);
    return  [NSString stringWithFormat:@"%lld", getTicketByBargainId(pp)];
}

-(NSString*) getBargainId:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [NSString stringWithFormat:@"%lld", getBargainId(pp)];
}

-(char) getIsYesDay:(NSString*)ticket {
    char* pp = toCString(ticket);
    return getIsYesDay(pp);
}

-(NSString *) getDeliveryMargin:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [NSString stringWithFormat: @"%.2f", getDeliveryMargin(pp)];
}

-(char) getIsClose:(NSString*)ticket {
    char* pp = toCString(ticket);
    return getIsClose(pp);
}

-(NSString *) getDeliveryPayment:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [NSString stringWithFormat: @"%.2f", getDeliveryPayment(pp)];
}

-(NSString *) getOrderticket:(NSString*)ticket{
    char* pp = toCString(ticket);
    return [NSString stringWithFormat: @"%lld", getOrderticket(pp)];
}

-(NSString*) getHoldQty:(NSString*)ticket {
    char* pp = toCString(ticket);
    double db = getHoldQty(pp);
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}

-(NSString*) getHoldPrice:(NSString*)ticket {
    char* pp = toCString(ticket);
    double db = getHoldPrice(pp);
    char * symbolcode = toCString([self getSymbol:ticket]);
    return [self getStrPrice:db dot:getPriceDecimal(symbolcode)];
}


-(NSString*) getExecuteTime:(NSString*)ticket {
    char* pp = toCString(ticket);
    const char * pch = getExecuteTime(pp);
    return toNString(pch);
}


-(NSDate*) getTimeIntervalSince1970:(long long) ticks{
    return [NSDate dateWithTimeIntervalSince1970:ticks];
}


-(int) getSymbolTraceDataCount:(NSString*) symbol {
    const char* pp = toCString(symbol);
    return getSymbolTraceDataCount(pp);
}

-(int) getSymbolTraceData:(NSString*) symbol arr:(TraceUnit*) arr size:(int)size {
    const char* pp = toCString(symbol);
    return getSymbolTraceData(pp, (CSTraceUnit*)arr, size);
}

-(int) getZSSymbolTraceData:(NSString*) symbol data:(TrendData*)data size:(int)size {
    const char* pp = toCString(symbol);
    return getZSSymbolTraceData(pp, data, size);
}

-(int) getSymbolMinuteDataCount:(NSString*) symbol {
    const char* pp = toCString(symbol);
    return getSymbolMinuteDataCount(pp);
}

-(int) getSymbolMinuteData:(NSString*) symbol arr:(TraceUnit*) arr size:(int)size {
    const char* pp = toCString(symbol);
    return getSymbolMinuteData(pp, (CSTraceUnit*)arr, size);
}

-(int) getSymbolKLineDataCount:(NSString*) symbol nType:(char)nType {
    const char* pp = toCString(symbol);
    return getSymbolKLineDataCount(pp, nType);
}

-(int) getZSSymbolKLineData:(NSString*) symbol nType:(char)nType data:(HistoryKData*)data size:(int)size{
    const char* pp = toCString(symbol);
    return getZSSymbolKLineData(pp, nType, data, size);
}

-(int) getZYQSymbolKLineData:(NSString*) symbol nType:(int)nType data:(HistoryKData*)data size:(int)size{
    const char* pp = toCString(symbol);
    return getZYQSymbolKLineData(pp, nType, data, size);
}

-(int) getZSSymbolMinuteData:(NSString*) symbol data:(TrendData*) data size:(int)size {
    const char* pp = toCString(symbol);
    return getZSSymbolMinuteData(pp, data, size);
}

-(BOOL) isZS {
    return getIsZS() == 1 ? TRUE : FALSE;
}

-(BOOL) isZYJ {
    return getIsZYJ() == 1 ? TRUE : FALSE;
}

-(BOOL) isStore {
    return getIsStore() == 1 ? TRUE : FALSE;
}

-(BOOL) isZLStore {
    return getIsZLStore() == 1 ? TRUE : FALSE;
}

//获取所有今天持仓历史单据
-(NSString *) getZYJToDayHistoryOrderAllTickets{
    return toNString(getZYJToDayHistoryOrderAllTickets());
}

//获取所有交收单据
-(NSString*) getSettleOrderAllTickets {
    return toNString(getSettleOrderAllTickets());
}

//获取成交单据
-(NSString*) getBarginOrderAllTickets {
    return toNString(getBarginOrderAllTickets());
}

-(NSString*) getCounterPartExecuteTicket:(NSString*)ticket {
    char* pp = toCString(ticket);
    long long t = getCounterPartExecuteTicket(pp);
    return [NSString stringWithFormat:@"%lld", t];
}

-(NSString*) getDeferredCompensation:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [self getStrPrice:getDeferredCompensation(pp) dot:2];
}

-(NSString*) getSellCangId:(NSString*)ticket {
    char* pp = toCString(ticket);
    return toNString(getSellCangId(pp));
}

//买方货款
-(NSString*) getBuyPayOut:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [self getStrPrice:getBuyPayOut(pp) dot:2];
}

-(NSString*) getDelegateTime:(NSString*)ticket  {
    char* pp = toCString(ticket);
    return toNString(getDelegateTime(pp));
}

-(NSString*) getMatchWeight:(NSString*)ticket  {
    char* pp = toCString(ticket);
    return [self getStrPrice:getMatchWeight(pp) dot:4];
}

-(NSString*)  getMatchDate:(NSString*)ticket {
    char* pp = toCString(ticket);
    return toNString(getMatchDate(pp));
}

-(NSString*) getDayStep:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [NSString stringWithFormat:@"%ld", getDayStep(pp)];
}


-(NSString*) getMargin:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [self getStrPrice: getMargin(pp) dot:2];
}


-(NSString*) getBargainCommision:(NSString*)ticket {
    char* pp = toCString(ticket);
    return [self getStrPrice: getBargainCommision(pp) dot:2];
}

-(int) query_apply_delivery_intention{
    return query_apply_delivery_intention();
}

-(int) getSymbolShowOrder:(NSString*)symbolcode{
    char* pp = toCString(symbolcode);
    return getSymbolShowOrder(pp);
}

//重新提交提货意向
-(int) submit_delivery_intention:(NSString*)mathId weight:(double)weight{
    
    const char* szMathId = toCString(mathId);
    return trade_submit_delivery_intention(szMathId, weight);
}

//提交提货意向
-(int) apply_delivery_intention:(NSString*)mathId weight:(double)weight dateBegin:(NSString*)dateBegin dateEnd:(NSString*)dateEnd deliverymen:(NSString*)deliverymen deliveryIdCode:(NSString*)deliveryIdCode telphone:(NSString*)telphone address:(NSString*)address zipCode:(NSString*)zipCode invoice:(char)invoice invoicetitle:(NSString*)invoicetitle invCompayName:(NSString*)invCompayName invCompayAddress:(NSString*)invCompayAddress invCompayTelephone:(NSString*)invCompayTelephone invBankaccount:(NSString*)invBankaccount invBankDeposit:(NSString*)invBankDeposit taxregistryNumber:(NSString*)taxregistryNumber {
    
    const char* szMathId = toCString(mathId);						     //配对ID
    const char* szDeliveryDateBegin = toCString(dateBegin);                    //提货开始日期
    const char* szDeliveryDateEnd = toCString(dateEnd);                     //提货结束日期
    const char* szDeliverymen = toCString(deliverymen);                   //提货人
    const char* szDeliveryIdCode = toCString(deliveryIdCode);                    //提货人身份证
    const char* szTelphone = toCString(telphone);                          //联系电话
    const char* szAddress = toCString(address);                           //联系地址
    const char* szZipCode = toCString(zipCode);                           //邮政编码

    const char* szInvoicetitle = toCString(invoicetitle);                      //发票抬头
    const char* szInvCompayName = toCString(invCompayName);                     //公司名称
    const char* szInvCompayAddress = toCString(invCompayAddress);                  //公司地址
    const char* szInvCompayTelephone = toCString(invCompayTelephone);                //公司电话
    const char* szInvBankaccount = toCString(invBankaccount);                    //银行账号
    const char* szInvBankDeposit = toCString(invBankDeposit);                    //开户行
    const char* szTaxregistryNumber = toCString(taxregistryNumber);
    
    return trade_apply_delivery_intention(szMathId, weight, szDeliveryDateBegin, szDeliveryDateEnd, szDeliverymen, szDeliveryIdCode, szTelphone, szAddress, szZipCode, invoice, szInvoicetitle, szInvCompayName, szInvCompayAddress, szInvCompayTelephone, szInvBankaccount, szInvBankDeposit, szTaxregistryNumber);
}

//修改提货意向
-(int) mod_delivery_intention:(NSString*)mathId weight:(double)weight dateBegin:(NSString*)dateBegin dateEnd:(NSString*)dateEnd deliverymen:(NSString*)deliverymen deliveryIdCode:(NSString*)deliveryIdCode telphone:(NSString*)telphone address:(NSString*)address zipCode:(NSString*)zipCode invoice:(char)invoice invoicetitle:(NSString*)invoicetitle invCompayName:(NSString*)invCompayName invCompayAddress:(NSString*)invCompayAddress invCompayTelephone:(NSString*)invCompayTelephone invBankaccount:(NSString*)invBankaccount invBankDeposit:(NSString*)invBankDeposit taxregistryNumber:(NSString*)taxregistryNumber {
    
    const char* szMathId = toCString(mathId);						     //配对ID
    const char* szDeliveryDateBegin = toCString(dateBegin);                    //提货开始日期
    const char* szDeliveryDateEnd = toCString(dateEnd);                     //提货结束日期
    const char* szDeliverymen = toCString(deliverymen);                   //提货人
    const char* szDeliveryIdCode = toCString(deliveryIdCode);                    //提货人身份证
    const char* szTelphone = toCString(telphone);                          //联系电话
    const char* szAddress = toCString(address);                           //联系地址
    const char* szZipCode = toCString(zipCode);                           //邮政编码
    
    const char* szInvoicetitle = toCString(invoicetitle);                      //发票抬头
    const char* szInvCompayName = toCString(invCompayName);                     //公司名称
    const char* szInvCompayAddress = toCString(invCompayAddress);                  //公司地址
    const char* szInvCompayTelephone = toCString(invCompayTelephone);                //公司电话
    const char* szInvBankaccount = toCString(invBankaccount);                    //银行账号
    const char* szInvBankDeposit = toCString(invBankDeposit);                    //开户行
    const char* szTaxregistryNumber = toCString(taxregistryNumber);
    
    return mod_apply_delivery_intention(szMathId, weight, szDeliveryDateBegin, szDeliveryDateEnd, szDeliverymen, szDeliveryIdCode, szTelphone, szAddress, szZipCode, invoice, szInvoicetitle, szInvCompayName, szInvCompayAddress, szInvCompayTelephone, szInvBankaccount, szInvBankDeposit, szTaxregistryNumber);
}

//提交“入库申请”请求
-(int) submit_apply_in_store_order:(NSString*)cid symbol:(NSString*)symbol aweight:(double)aweight unit:(double)unit idate:(NSString*)idate mdate:(NSString*)mdate lotnumber:(NSString*)lotnumber package:(NSString*)package factory:(NSString*)factory {
    const char* pcid = toCString(cid);
    const char* psymbol = toCString(symbol);
    const char* pidate = toCString(idate);
    const char* pmdate = toCString(mdate);
    const char* plotnumber = toCString(lotnumber);
    const char* ppackage = toCString(package);
    const char* pfactory = toCString(factory);
    return submit_apply_in_store_order(pcid ,psymbol, aweight, unit,  pidate, pmdate, plotnumber, ppackage, pfactory);
}

//修改“入库申请”请求
-(int) mod_apply_in_store_order:(NSString*)importid cid:(NSString*) cid symbol:(NSString*)symbol aweight:(double)aweight unit:(double)unit idate:(NSString*)idate mdate:(NSString*)mdate lotnumber:(NSString*)lotnumber package:(NSString*)package factory:(NSString*)factory {
    const char* piid = toCString(importid);
    const char* pcid = toCString(cid);
    const char* psymbol = toCString(symbol);
    const char* pidate = toCString(idate);
    const char* pmdate = toCString(mdate);
    const char* plotnumber = toCString(lotnumber);
    const char* ppackage = toCString(package);
    const char* pfactory = toCString(factory);
    return mod_apply_in_store_order(piid, pcid ,psymbol, aweight, unit, pidate, pmdate, plotnumber, ppackage, pfactory);
}

-(int) trade_cancle_apply_in_store_order:(NSString*)importid {
    const char* piid = toCString(importid);
    return trade_cancle_apply_in_store_order(piid);
}

//操作“入库申请”
-(int) trade_apply_in_store_order:(NSString*)importid {
    const char* piid = toCString(importid);
    return trade_apply_in_store_order(piid);
}

-(NSString*) getDeliveryStoreIds {
    return toNString(getDeliveryStoreIds());
}

-(NSString*) getStoreNameById:(NSString*)ids{
    return toNString(getStoreNameById(ids.longLongValue));
}

-(NSString*) getStoreFullNameById:(NSString*)ids {
    return toNString(getStoreFullNameById(ids.longLongValue));
}

-(NSString*) getStoreCodeById:(NSString*)ids{
    return toNString(getStoreCodeById(ids.longLongValue));
}

-(NSString*) getStoreAddressById:(NSString*)ids{
    return toNString(getStoreAddressById(ids.longLongValue));
}

-(NSString*) getStoreAuthenticatedPersonById:(NSString*)ids{
    return toNString(getStoreAuthenticatedPersonById(ids.longLongValue));
}

-(NSString*) getStoreKeeperById:(NSString*)ids{
    return toNString(getStoreKeeperById(ids.longLongValue));
}

-(NSString*) getStoreZipCodeById:(NSString*)ids{
    return toNString(getStoreZipCodeById(ids.longLongValue));
}

-(NSString*) getStoreFaxById:(NSString*)ids{
    return toNString(getStoreFaxById(ids.longLongValue));
}

-(NSString*) getStoreEmailById:(NSString*)ids{
    return toNString(getStoreEmailById(ids.longLongValue));
}

-(NSString*) getApplyByStoreId:(NSString*)iStoreid {
    return [NSString stringWithFormat:@"%lld", getApplyByStoreId(iStoreid.longLongValue)];
}

-(NSString*) getApplyInStoreAllApplyId {
    return toNString(getApplyInStoreAllApplyId());
}

-(NSString*) getInStoreAllApplyId {
    return toNString(getInStoreAllApplyId());
}

-(NSString*) getOutStoreAllIds{
    return toNString(getOutStoreAllIds());
}

-(NSString*) getCangOrderAllIds{
    return toNString(getCangOrderAllIds());
}

-(NSString*) getCangToInStoreOrderAllIds{
    return toNString(getCangToInStoreOrderAllIds());
}

-(NSString*) getBalanceWeight:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [self getStrPrice: getBalanceWeight(pp) dot:3];
}

-(NSString*) getFreezeImportWeight:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [self getStrPrice: getFreezeImportWeight(pp) dot:3];
}

-(NSString*) getFreezeCloseWeight:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [self getStrPrice: getFreezeCloseWeight(pp) dot:3];
}

-(NSString*) getFreezeWaitWeight:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [self getStrPrice: getFreezeWaitWeight(pp) dot:3];
}

-(NSString*) getCloseWeight:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [self getStrPrice: getCloseWeight(pp) dot:3];
}

-(char) getCangType:(NSString*)ticket{//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}
    const char* pp = toCString(ticket);
    return getCangType(pp);
}

-(NSString*) getCangDate:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return toNString(getCangDate(pp));
}

-(NSString*) getApplyDate:(NSString*)ticket {//申请时间
    const char* pp = toCString(ticket);
    return toNString(getApplyDate(pp));
}

-(char) getCangStatus:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return getCangStatus(pp);
}

-(char) getExportStatus:(NSString*)ticket {// '出库状态'
    const char* pp = toCString(ticket);
    return getExportStatus(pp);
}

-(NSString*) getExportdate:(NSString*)iApplyid { // '出库时间'
    const char* pp = toCString(iApplyid);
    return toNString(getExportdate(pp));
}

-(NSString*) getDeliverynumber:(NSString*)iApplyid {//提货码-
    const char* pp = toCString(iApplyid);
    return toNString(getDeliverynumber(pp));
}

-(NSString*) getDeliveryMen:(NSString*)iApplyid{//提货人名称
    const char* pp = toCString(iApplyid);
    return toNString(getDeliveryMen(pp));
}

-(NSString*) getIdCode:(NSString*)iApplyid{//提货人身份证
    const char* pp = toCString(iApplyid);
    return toNString(getIdCode(pp));
}

-(NSString*) getTelphone:(NSString*)iApplyid{//提货人电话号码
    const char* pp = toCString(iApplyid);
    return toNString(getTelphone(pp));
}

-(NSString*) getOverPlusweight:(NSString*)iApplyid {//剩余重量
    const char* pp = toCString(iApplyid);
    return [self getStrPrice:getOverPlusweight(pp) dot:3];
}

-(NSString*) getImportid:(NSString*)iApplyid { // '入库单ID',
    const char* pp = toCString(iApplyid);
    return [NSString stringWithFormat:@"%d", getImportid(pp)];
}

-(NSString*) getWarehouseid:(NSString*)iApplyid{  // '交收仓库',
    const char* pp = toCString(iApplyid);
    return [NSString stringWithFormat:@"%d", getWarehouseid(pp)];
}

-(NSString*) getAccountgroupid:(NSString*)iApplyid{// '机构',
    const char* pp = toCString(iApplyid);
    return [NSString stringWithFormat:@"%d", getAccountgroupid(pp)];
}

-(NSString*) getSymbolgroupid:(NSString*)iApplyid{// '品种名称',
    const char* pp = toCString(iApplyid);
    return [NSString stringWithFormat:@"%d", getSymbolgroupid(pp)];
}

-(NSString*) getInStoreSymbolcode:(NSString*)iApplyid{// '商品代码',
    const char* pp = toCString(iApplyid);
    return toNString(getInStoreSymbolcode(pp));
}

-(NSString*) getImportdate:(NSString*)iApplyid{// '入库时间',
    const char* pp = toCString(iApplyid);
    return toNString(getImportdate(pp));
}

-(char) getImportstatus:(NSString*)iApplyid{// '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',
    const char* pp = toCString(iApplyid);
    return getImportstatus(pp);
}
-(char) getApplystatus:(NSString*)iApplyid {
    const char* pp = toCString(iApplyid);
    return getApplystatus(pp);
}
-(NSString*) getRejectreason:(NSString*)iApplyid{// '拒绝原因',
    const char* pp = toCString(iApplyid);
    return toNString(getRejectreason(pp));
}

-(NSString*) getCheckresult:(NSString*)iApplyid{// '检验结果',
    const char* pp = toCString(iApplyid);
    return toNString(getCheckresult(pp));
}

-(NSString*) getApplyweight:(NSString*)iApplyid{// '申请重量',
    const char* pp = toCString(iApplyid);
    return [self getStrPrice:getApplyweight(pp) dot:3];
}

-(NSString*) getImportweight:(NSString*)iApplyid{// '入库重量'
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getImportweight(pp) dot:3];
}

-(NSString*) getFreezeexportweight:(NSString*)iApplyid{
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getFreezeexportweight(pp) dot:3];
}

-(NSString*) getExportweight:(NSString*)iApplyid{
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getExportweight(pp) dot:3];
}

-(NSString*) getFreezecangweight:(NSString*)iApplyid{
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getFreezecangweight(pp) dot:3];
}

-(NSString*) getCangweight:(NSString*)iApplyid{
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getCangweight(pp) dot:3];
}

-(NSString*) getUnitweight:(NSString*)iApplyid{ // '每件重量',
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getUnitweight(pp) dot:3];
}

-(NSString*) getQuantity:(NSString*)iApplyid{ // '件数',
    const char* pp = toCString(iApplyid);
    return [self getStrPrice: getQuantity(pp) dot:3];
}

-(NSString*) getGoodsnumber:(NSString*)iApplyid{// '货号',
    const char* pp = toCString(iApplyid);
    return toNString(getGoodsnumber(pp));
}

-(NSString*) getLot:(NSString*)iApplyid{// '批号',
    const char* pp = toCString(iApplyid);
    return toNString(getLot(pp));
}

-(NSString*) getMade:(NSString*)iApplyid{// '生产年月',
    const char* pp = toCString(iApplyid);
    return toNString(getMade(pp));
}

-(NSString*) getPackage:(NSString*)iApplyid{// '包装方式',
    const char* pp = toCString(iApplyid);
    return toNString(getPackage(pp));
}

-(NSString*) getFactory:(NSString*)iApplyid{// '生产厂商',
    const char* pp = toCString(iApplyid);
    return toNString(getFactory(pp));
}

-(NSString*) getWarehousetransactor:(NSString*)iApplyid{// '仓库经办人',
    const char* pp = toCString(iApplyid);
    return toNString(getWarehousetransactor(pp));
}

-(NSString*) getWarehousemanager:(NSString*)iApplyid{// '仓库负责人',
    const char* pp = toCString(iApplyid);
    return toNString(getWarehousemanager(pp));
}

-(NSString*) getClienttransactor:(NSString*)iApplyid{// '客户经办人'
    const char* pp = toCString(iApplyid);
    return toNString(getClienttransactor(pp));
}

-(int) getAttributeNum:(NSString*)iApplyid{
    const char* pp = toCString(iApplyid);
    return getAttributeNum(pp);
}

-(NSString*) getAttributeIdByIndex:(NSString*)iApplyid idx:(int)index{
    const char* pp = toCString(iApplyid);
    return [NSString stringWithFormat:@"%d", getAttributeIdByIndex(pp, index)];
}

-(NSString*) getAttributeValueByIndex:(NSString*)iApplyid idx:(int)index{
    const char* pp = toCString(iApplyid);
    return toNString(getAttributeValueByIndex(pp, index));
}

-(NSString*)  getApplyDeliveryIds{
    return toNString(getApplyDeliveryIds());
}

-(NSString*) getApplyId:(NSString*)ids {
    const char* pp = toCString(ids);
    return  [NSString stringWithFormat: @"%lld", getApplyid(pp)];
}

-(NSString*)  getApplyDeliverymen:(NSString*)ticket{ // '提货人',
    const char* pp = toCString(ticket);
    return toNString(getApplyDeliverymen(pp));
}

-(NSString*) getApplyIdcode:(NSString*)ticket{ // '身份证',
        const char* pp = toCString(ticket);
return toNString(getApplyIdcode(pp));
}

-(NSString*)  getApplyTelephone:(NSString*)ticket{ // '联系电话',
    const char* pp = toCString(ticket);
    return toNString(getApplyTelephone(pp));
}

-(NSString*)  getApplyAddress:(NSString*)ticket{ // '联系地址',
    const char* pp = toCString(ticket);
return toNString(getApplyAddress(pp));
}

-(NSString*)  getApplyZipcode:(NSString*)ticket{ // '邮政编码',
    const char* pp = toCString(ticket);
return toNString(getApplyZipcode(pp));
}

-(NSString*) getApplyInvoicetitle:(NSString*)ticket{ // '发票抬头',
    const char* pp = toCString(ticket);
return toNString(getApplyInvoicetitle(pp));
}

-(NSString*)  getApplyCompanyname:(NSString*)ticket{ // '公司名称',
    const char* pp = toCString(ticket);
return toNString(getApplyCompanyname(pp));
}

-(NSString*)  getApplyCompanyaddress:(NSString*)ticket{ // '公司地址',
    const char* pp = toCString(ticket);
return toNString(getApplyCompanyaddress(pp));
}

-(NSString*)  getApplyCompanytelephone:(NSString*)ticket{// '公司电话',
    const char* pp = toCString(ticket);
return toNString(getApplyCompanytelephone(pp));
}

-(NSString*)  getApplyBankaccount:(NSString*)ticket{ // '银行账号',
    const char* pp = toCString(ticket);
return toNString(getApplyBankaccount(pp));
}

-(NSString*)  getApplyBankdeposit:(NSString*)ticket{ // '开户行',
    const char* pp = toCString(ticket);
return toNString(getApplyBankdeposit(pp));
}

-(NSString*)  getApplyTaxregistrynumber:(NSString*)ticket{ // '税务登记号',
    const char* pp = toCString(ticket);
return toNString(getApplyTaxregistrynumber(pp));
}

-(NSString*)  getApplyDeliverydatebegin:(NSString*)ticket{ // '提货日期D5',
    const char* pp = toCString(ticket);
return toNString(getApplyDeliverydatebegin(pp));
}

-(NSString*)  getApplyDeliverydateend:(NSString*)ticket{ // '提货日期D6',
    const char* pp = toCString(ticket);
return toNString(getApplyDeliverydateend(pp));
}

-(NSString*)  getApplyDeliverynumber:(NSString*)ticket{ // '提货号',
    const char* pp = toCString(ticket);
return toNString(getApplyDeliverynumber(pp));
}

-(NSString*)  getApplySymbolcode:(NSString*)ticket{ // '商品代码',
    const char* pp = toCString(ticket);
return toNString(getApplySymbolcode(pp));
}

-(NSString*)  getApplyWarehousefeedate:(NSString*)ticket{ // '仓储费截止日期',
    const char* pp = toCString(ticket);
return toNString(getApplyWarehousefeedate(pp));
}

-(NSString*)  getApplyRejectreason:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return toNString(getApplyRejectreason(pp));
}

-(NSString*)  getApplySettlementdate:(NSString*)ticket{ // '交割日期',
    const char* pp = toCString(ticket);
    return toNString(getApplySettlementdate(pp));
}

-(NSString*) getApplyMatchId:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return [NSString stringWithFormat:@"%d", getApplyMatchId(pp)];
}

-(NSString*) getApplyWarehouseid:(NSString*)ticket{ // '仓库ID',
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%d", getApplyWarehouseid(pp)];
}

-(NSString*) getApplyCangId:(NSString*)ticket{	// 仓单ID
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%d", getApplyCangId(pp)];
}

-(NSString*) getApplyExecuteticket:(NSString*)ticket{ // '成交单号',
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%d", getApplyExecuteticket(pp)];
}

-(NSString*) getApplyCounterpartyexecuteticket:(NSString*)ticket{ // '对手单号',
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%d", getApplyCounterpartyexecuteticket(pp)];
}

-(NSString*) getApplyDeliveryweight:(NSString*)ticket{ // '提货重量',
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%.3f", getApplyDeliveryweight(pp)];
}

-(NSString*) getApplySettlementprirce:(NSString*)ticket{ // '交割价格',
    const char* pp = toCString(ticket);
return [NSString stringWithFormat:@"%.3f", getApplySettlementprirce(pp)];
}

-(char) getApplyDeliverystatus:(NSString*)ticket{ // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',
    const char* pp = toCString(ticket);
    return getApplyDeliverystatus(pp);
}

-(char) getApplyIsinvoice:(NSString*)ticket{ // '是否需要发票',
    const char* pp = toCString(ticket);
    return getApplyIsinvoice(pp);
}

-(char) getApplyIscancel:(NSString*)ticket{ // '是否撤销',
    const char* pp = toCString(ticket);
    return getApplyIscancel(pp);
}

-(int) trade_cang_to_instore_order:(NSString*)ticket weight:(double)weight{
    const char* pp = toCString(ticket);
    return trade_cang_to_instore_order(pp, weight);
}

//提交 “仓单转入库单”
-(int) trade_submit_cang_to_instore_order:(NSString*)ticket{
    const char* pp = toCString(ticket);
    return trade_submit_cang_to_instore_order(pp);
}

//撤消 “仓单转入库单”
-(int) trade_cancel_cang_to_instore_order:(NSString*)ticket {
    const char* pp = toCString(ticket);
    return trade_cancel_cang_to_instore_order(pp);
}

-(int) trade_current_monery_flowing {
    return trade_current_monery_flowing();
}

-(int) trade_monery_flowing:(NSString*) start end:(NSString*) end{
    const char* pp = toCString(start);
    const char* ppend = toCString(end);
    return trade_monery_flowing(pp, ppend);
}

//入库单转仓单
-(int) trade_instore_to_cang_order:(NSString*)ticket weight:(double)weight {
    const char* pp = toCString(ticket);
    return  trade_instore_to_cang_order(pp, weight);
}

//入库单转出库单
-(int) trade_instore_to_exportstore_order:(NSString*)ticket deliveryName:(NSString*)deliveryName weight:(double)weight idCode:(NSString*)idCode telPhone:(NSString*)telPhone {
    const char* pp = toCString(ticket);
    const char* pdeliveryName = toCString(deliveryName);
    const char* pidCode = toCString(idCode);
    const char* ptelPhone = toCString(telPhone);
    return trade_instore_to_exportstore_order(pp, pdeliveryName, weight, pidCode, ptelPhone);
}


//提交买 "申报交收" 请求
-(int) trade_buy_apply_delivery_settle:(NSString*)symbolcode qty:(double)qty cancel:(BOOL)iscancel {
    const char* pp = toCString(symbolcode);
    return trade_buy_apply_delivery_settle(pp, qty, iscancel ? SELLTEMENT_AUTO_CANCEL_YES : SELLTEMENT_AUTO_CANCEL_NO);
}

//提交卖 "申报交收" 请求
-(int) trade_sell_apply_delivery_settle:(NSString*)symbolcode qty:(double)qty cangid:(NSString*)cangid cancel:(BOOL)iscancel {
    const char* pp = toCString(symbolcode);
    const char* pp2 = toCString(cangid);
    return trade_sell_apply_delivery_settle(pp, qty, pp2, iscancel ? SELLTEMENT_AUTO_CANCEL_YES : SELLTEMENT_AUTO_CANCEL_NO);
}

-(BOOL) isRefresh {
    return isRefresh() == 0 ? TRUE : FALSE;
}

-(void) init_trade_engine{
    trade_ui_events uievents;
    uievents.trade_ui_login_rsp = &trade_ui_login_rsp;
    uievents.trade_ui_login_progress_rsp = &trade_ui_login_progress_rsp;
    uievents.call_funds_flowing = &call_funds_flowing;
    
    
    uievents.trade_ui_kick_notify = &trade_ui_kick_notify;
    
    uievents.trade_ui_hisorder_rsp = &trade_ui_hisorder_rsp;
    uievents.trade_ui_holdorder_rsp = &trade_ui_holdorder_rsp;
    
    uievents.trade_ui_openorder_rsp = &trade_ui_openorder_rsp;
    uievents.trade_ui_openorder_stat_rsp = &trade_ui_openorder_stat_rsp;
    uievents.on_quote_minute_data_notify = &quote_ui_min_data_recv_notify;
    uievents.on_quote_history_data_notify = &quote_ui_his_data_recv_notify;
    uievents.on_quote_trace_data_notify = &quote_ui_record_data_recv_notify;
    uievents.trade_ui_refresh_complete = &trade_ui_refresh_complete;
    
    //新加事件
    uievents.quote_real_notify = &on_quote_real_notify;
    uievents.history_change_notify=&on_history_change_notify;
    
    uievents.delcare_content_rsp = &on_delcare_content_rsp;
    uievents.delcare_notify = &on_delcare_notify;
    uievents.modify_passwd_rsp = &on_modify_passwd_rsp;
    
    uievents.apply_delivery_intention_rsp = &on_trade_apply_delivery_intention_rsp;
    uievents.op_cang_to_in_store_order_response = &on_cang_to_instore_order_response;
    uievents.op_cang_order_response = &op_cang_order_response;
    
    
    uievents.on_apply_delivery_sellte_rsp = &on_apply_delivery_sellte_rsp;
        uievents.on_sign_change_notify = &on_sign_change_notify;
    
    //仓库通知道
    uievents.on_bargain_notify = &on_bargain_notify;
    uievents.on_sellte_notify = &on_sellte_notify;
    uievents.on_outstore_order_notify = &on_outstore_order_notify;
    uievents.on_apply_instore_notify = &on_apply_instore_notify;
    uievents.on_cang_order_notify = &on_cang_order_notify;
    uievents.on_apply_delivery_notify = &on_apply_delivery_notify;
    uievents.on_instore_order_notify = &on_instore_order_notify;
    uievents.on_cangorder_to_instore_notify = &on_cangorder_to_instore_notify;
    
    uievents.op_in_store_order_response = &on_in_store_order_response;
    uievents.apply_in_store_rsp = &on_apply_in_store_rsp;
    
    uievents.on_quote_detail_report_notify = &quote_detail_report_notify;
    uievents.on_quote_complex_ranking_notify = &quote_complex_ranking_notify;
    uievents.on_quote_real_detail_notify = &quote_real_detail_notify;
    
    uievents.on_inout_money_rsp = &on_inout_money_rsp;
    
    
#pragma mark -
#pragma mark 文交所回调事件
    /*************************************************************
                            文交所
     ************************************************************/
    
    // 交易
    uievents.trade_ui_symbol_change_notice = &trade_ui_symbol_change_notice_rspUI;
    uievents.trade_ui_today_deal_rsp = &trade_ui_today_deal_rspUI;
    uievents.trade_ui_today_entrust_rsp = &trade_ui_today_entrust_rspUI;
    uievents.trade_ui_sharehold_rsp = &trade_ui_sharehold_rspUI;
    uievents.trade_ui_historyorder_rsp = &trade_ui_historyorder_rspUI;
    uievents.trade_ui_historyturnover_rsp = &trade_ui_historyturnover_rspUI;
    uievents.trade_ui_pretrade_rsp = &trade_ui_pretrade_rspUI;
    uievents.trade_ui_canceltrade_rsp = &trade_ui_canceltrade_rspUI;
    
    // 行情
    uievents.quote_ui_login_rsp = &quote_ui_login_rspUI;
    uievents.quote_ui_hisKDataFirst_rsp = &quote_ui_hisKDataFirst_rspUI;
    uievents.quote_ui_hisKDataCurDate_rsp = &quote_ui_hisKDataCurDate_rspUI;
    uievents.quote_ui_stkDealData_rsp = &quote_ui_stkDealData_rspUI;
    uievents.quote_ui_initMarket_rsp = &quote_ui_initMarket_rspUI;
    uievents.quote_ui_pankou_rsp = &quote_ui_pankou_rspUI;
    uievents.trade_ui_moneyhold_rsp = &trade_ui_moneyhold_rspUI;
    uievents.trade_ui_transferquery_rsp = &trade_ui_transferquery_rspUI;
    set_trade_ui_events(uievents);
    
    
    
    /**
     *  call back from nethelp.c
     */
    callback_logic callbacklogic;
    callbacklogic.cache_user_pwd = &cache_user_pwd_ui;
    set_callback_logics(callbacklogic);
}

//请求登录 带地址信息
-(void)userloginwithnet:(NSString*)account password:(NSString*)password tradeip:(NSString*)tradeip tradeport:(int)tradeport quoteip:(NSString *)quoteip  quoteport:(int)quoteport
{
    char * cpaccount = toCString(account);
    char * cpassword = toCString(password);
    char * cptradeip = toCString(tradeip);
    char * cpquoteip = toCString(quoteip);
    userloginwithnet(cpaccount, cpassword, cptradeip, tradeport, cpquoteip, quoteport);
}

//请求行情 带地址信息
- (void)loginquotewithnet:(NSString *)quoteip  quoteport:(int)quoteport
{
    char * cpquoteip = toCString(quoteip);
    loginquotewithnet(cpquoteip, quoteport);
}

-(BOOL) add_balance:(NSString*) server port:(int) port{
    char * cptradeip = toCString(server);
    return 1 == add_balance(cptradeip, port) ? TRUE : FALSE;
}

-(void) clear_balance{
    clear_balance();
}

-(void) set_login_info:(NSString*)account password:(NSString*)password{
    char * cpaccount = toCString(account);
    char * cpassword = toCString(password);
    set_login_info(cpaccount, cpassword);
}

-(void) start_balance {
    start_balance();
}

-(NSString*) getSymbolNewQty:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolNewQty(psymbol) dot: 2];
}
-(NSString*) getSymbolNewPrice:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolNewPrice(psymbol) dot:getPriceDecimal(psymbol)];
}
-(int) getSymbolTurnoverDirec:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return getSymbolTurnoverDirec(psymbol);
}
-(NSString*) getSymbolPriceRiseDown:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolPriceRiseDown(psymbol) dot: 2];
}
-(NSString*) getSymbolPriceRiseDownRate:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolPriceRiseDownRate(psymbol) dot: 2];
}
-(NSString*) getSymbolTotalTurnoverFunds:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolTotalTurnoverFunds(psymbol) dot: 2];
}
-(NSString*) getSymbolOrderRate:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolOrderRate(psymbol) dot: 2];
}
-(NSString*) getSymbolQtyRate:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolQtyRate(psymbol) dot: 2];
}
-(NSString*) getSymbolTotalTurnoverQty:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolTotalTurnoverQty(psymbol) dot: 2];
}
-(NSString*) getSymbolBuyVolPrice:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolBuyVolPrice(psymbol) dot: getPriceDecimal(psymbol)];
}
-(NSString*) getSymbolBuyVolQty:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolBuyVolQty(psymbol) dot: 2];
}
-(NSString*) getSymbolSellVolPrice:(NSString*) symbol
{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolSellVolPrice(psymbol) dot: getPriceDecimal(psymbol)];
}
-(NSString*) getSymbolSellVolQty:(NSString*) symbol{
    char* psymbol = toCString(symbol);
    return [self getStrPrice: getSymbolSellVolQty(psymbol) dot: 2];
}
-(int) request_zyq_pankou_detail:(NSString*) symbol{
    char* psymbol = toCString(symbol);
    return request_zyq_pankou_detail(psymbol);
}

-(int) getNowDataVolPirce:(NSString*) symbol data:(STUNowDataVolPirce*)stuNowDataVolPirce {
    char* psymbol = toCString(symbol);
    return getNowDataVolPirce(psymbol, stuNowDataVolPirce);
}

-(int) getZYJSymbolTraceData:(NSString*) symbol data:(STUTraceUnit_ZYQ*) stuTraceUnit_ZYQ size:(int) size {
    char* psymbol = toCString(symbol);
    return getSymbolTraceData_ZYQ(psymbol, stuTraceUnit_ZYQ, size);
}
-(int) getZYJSymbolSTKMineData:(NSString*) symbol data:(STUSTKMinUnit_ZYQ*) stuSTKMinUnit_ZYQ size:(int) size {
    char* psymbol = toCString(symbol);
    return getSymbolMinData_ZYQ(psymbol, stuSTKMinUnit_ZYQ, size);
}

//-(int) trade_in_monery:(NSString*)password monery:(double) monery phonePwd:(NSString*)phonePwd remark:(NSString*)remark state:(NSDictionary*) state {
//    const char* ppassword = toCString(password);
//    const char* premark = toCString(remark);
//    const char* pphonePwd = toCString(phonePwd);
//    NSMutableDictionary* mMutableDictionary;
//    if(state) {
//        mMutableDictionary = [[NSMutableDictionary alloc] initWithDictionary: state];
//    } else {
//        mMutableDictionary = [[NSMutableDictionary alloc] init];
//    }
//    [mMutableDictionary setObject: [[NSTradeBizUtils sharedInstance] localIPAddress] forKey: @"address"];
//    [mMutableDictionary setObject: @"NATIVE" forKey: @"trade_type"];
//    NSError* error;
//    NSData *jsonData = [NSJSONSerialization dataWithJSONObject: mMutableDictionary
//                                                       options:NSJSONWritingPrettyPrinted
//                                                         error:&error];
//    const char* pstate = toCString(  [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]);
//    return trade_in_monery(ppassword, monery, pphonePwd, premark, pstate);
//}
//
//-(int) trade_out_monery:(NSString*) password monery:(double)monery phonePwd:(NSString*)phonePwd remark:(NSString*)remark state:(NSDictionary*)state {
//    const char* ppassword = toCString(password);
//    const char* pphonePwd = toCString(phonePwd);
//    const char* premark = toCString(remark);
//    NSMutableDictionary* mMutableDictionary;
//    if(state) {
//        mMutableDictionary = [[NSMutableDictionary alloc] initWithDictionary: state];
//    } else {
//        mMutableDictionary = [[NSMutableDictionary alloc] init];
//    }
//    [mMutableDictionary setObject: [[NSTradeBizUtils sharedInstance] localIPAddress] forKey: @"address"];
//    [mMutableDictionary setObject: @"NATIVE" forKey: @"trade_type"];
//    NSError* error;
//    NSData *jsonData = [NSJSONSerialization dataWithJSONObject: mMutableDictionary
//                                                       options: NSJSONWritingPrettyPrinted
//                                                         error: &error];
//    const char* pstate = toCString(  [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]);
//    return trade_out_monery(ppassword, monery, pphonePwd, premark, pstate);
//}

//-(int) trade_sign:(NSString*) tel bankName:(NSString*) bankName bankAccount:(NSString*) bankAccount idCode:(NSString*) idCode password:(NSString*) password phonePwd:(NSString*) phonePwd remark:(NSString*) remark {
//    const char * ptel = toCString(tel);
//    const char * pbankName = toCString(bankName);
//    const char * pbankAccount = toCString(bankAccount);
//    const char * pidCode = toCString(idCode);
//    const char * ppassword = toCString(password);
//    const char * premark = toCString(remark);
//    const char * pphonePwd = toCString(phonePwd);
//    return trade_sign(ptel, pbankName, pbankAccount, pidCode, ppassword, pphonePwd, premark);
//}
//
//-(int) trade_unsign:(NSString*) tel bankName:(NSString*) bankName bankAccount:(NSString*) bankAccount idCode:(NSString*) idCode password:(NSString*) password phonePwd:(NSString*) phonePwd remark:(NSString*) remark {
//    const char * ptel = toCString(tel);
//    const char * pbankName = toCString(bankName);
//    const char * pbankAccount = toCString(bankAccount);
//    const char * pidCode = toCString(idCode);
//    const char * ppassword = toCString(password);
//    const char * premark = toCString(remark);
//    const char * pphonePwd = toCString(phonePwd);
//    return trade_unsign(ptel, pbankName, pbankAccount, pidCode, ppassword, pphonePwd, premark);
////    return trade_unsign(toCString(tel), toCString(bankName), toCString(bankAccount), toCString(idCode), toCString(password), toCString(remark));
//}

-(int) trade_inout_moneny_type:(int)type money:(double)money moneyPwd:(NSString *)moneyPwd bankPwd:(NSString *)bankPwd
{
    char * cMoneyPwd = toCString(moneyPwd);
    char * cBankPwd = toCString(bankPwd);
    
    return trade_request_inout_money(type, money, cMoneyPwd, cBankPwd);
}

-(NSString*) getFundFlowTickets {
    return toNString(getFundFlowTickets());
}
-(NSString*) getFundFlowAccountAlias:(long long) ids {
    return toNString(getFundFlowAccountAlias(ids));
}
-(NSString*) getFundFlowExecuteTime:(long long) ids {
    return toNString(getFundFlowExecuteTime(ids));
}
-(NSString*) getFundFlowAccountId:(long long) ids {
    return [NSString stringWithFormat: @"%lld", getFundFlowAccountId(ids)];
}
-(char) getFundFlowOperatortype:(long long) ids {
    return getFundFlowOperatortype(ids);
}
-(NSString*) getFundFlowChangeFunds:(long long) ids {
    return [self getStrPrice: getFundFlowChangeFunds(ids) dot: 2];
}
-(NSString*) getFundFlowBalance:(long long) ids {
    return [self getStrPrice: getFundFlowBalance(ids) dot: 2];
}
-(long long) getFundFlowAssociateticket:(long long) ids {
    return getFundFlowAssociateticket(ids);
}

-(NSString*) getInOutMoneryids {
    return toNString(getInOutMoneryids());
}
-(NSString*) getInOutMoneryApplyTime:(long long) ids{
    return toNString(getInOutMoneryApplyTime(ids));
}
-(NSString*) getInOutMoneryAuditTime:(long long) ids {
    return toNString(getInOutMoneryAuditTime(ids));
}
-(NSString*) getInOutMoneryRemark:(long long) ids  {
    return toNString(getInOutMoneryRemark(ids));
}
-(char) getInOutMoneryExecutestatus:(long long) ids {
    return getInOutMoneryExecutestatus(ids);
}
-(long long) getInOutMoneryId:(long long) ids{
    return getInOutMoneryId(ids);
}
-(char) getInOutMoneryOperatortype:(long long) ids{
    return getInOutMoneryOperatortype(ids);
}
-(NSString*) getInOutMoneryAmount:(long long) ids {
    return [self getStrPrice: getInOutMoneryAmount(ids) dot: 2];
}

-(BOOL) isSignStatus{
    return getSignStatus() == 1 ? TRUE : FALSE;
}
-(BOOL) isSuportInMonery {
    return getSuportInMonery() == 1 ? TRUE : FALSE;
}
-(BOOL) isSuportOutMonery {
    return getSuportOutMonery() == 1 ? TRUE : FALSE;
}
-(int) getBankPasswordType {
    return getBankPasswordType();
}
-(BOOL) isSuportSign {
    return getSuportSign();
}
-(BOOL) isSuportUnSign {
    return getSuportUnSign();
}

//下市价单
-(int)trade_open_market_order:(NSString *)symbolcode direction:(NSString *)direction price:(double)price shoushu:(int)shoushu
{
    char * cpSymbol = toCString(symbolcode);
    char * cpDirection = toCString(direction);
    return trade_open_market_order(cpSymbol, shoushu, 0, 0, cpDirection, price, 0);
}


- (int)trade_request_today_entrustNS
{
    return trade_request_today_entrust();
}

- (int)trade_request_today_dealNS
{
    return trade_request_today_deal();
}

- (int)trade_request_cancel_order:(NSString *)orderID
{
    char * order = toCString(orderID);
    return trade_request_cancel_order(order);
}

- (int)trade_request_shareholdNS
{
    return trade_request_sharehold();
}

- (int)trade_request_moneyholdNS
{
    return trade_request_moneyhold();
}

- (int)quote_kline_reqNS:(int)klineType productID:(NSString *)productID
{
    char * cproductID = toCString(productID);
    return quote_kline_req(klineType, cproductID);
}

- (int)quote_stktrace_reqNS:(NSString *)productID
{
    char * cproductID = toCString(productID);
    return quote_stktrace_req(cproductID);
}

- (int)trade_request_transferquery
{
    return trade_query_inout_monery();
}

- (int)trade_request_pretrade:(NSString *)securityID type:(NSString *)orderType
{
    char * csecurityID = toCString(securityID);
    char * corderType = toCString(orderType);
    return trade_pretrade_req(csecurityID, corderType);
}

@end
