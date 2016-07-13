//
//  TradeEngine.cpp
//  jxtMsgClient
//
//  Created by zengxw on 13-11-10.
//
//
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#include "TradeEngine.h"
#include "CalcInd.h"
#include "DataDef.h"

#include "global.h"
#include "nethelper.h"
#include "jxt_log.h"
#include "SHA1.h"
#include "tradebizUtils.h"


using namespace std;

#include <algorithm>

#ifdef __ANDROID__
#ifndef INT32_MAX
#define INT32_MAX        2147483647
#endif
#endif

#define    REFRESH_INTERVAL_SECONDS (60 * 5)
#define    CODESCACHESIZE        8192
#define    QTY_DECIMAL           (2)
#define    gEPSINON                (0.000000001)
#define             REPORT_SIZE  (50)//报价牌请求个数


typedef struct _SeqMap
{
    char symbol[32];//商品
    int cType;//类型
}SeqMap;
std::map<int, SeqMap>  seqContainer;

//typedef struct _QuoteDetailData
//{
//
//} QuoteDetailData;
//typedef std::map<string, QuoteDetailData>  SymbolQuoteDetailContainer;
//SymbolQuoteDetailContainer symbolQuoteDetailContainer;

//商品容器
typedef std::map<string, CSymbolParam *> SymbolContainer;
SymbolContainer symbolContainer;
vector<int> symbolSortNumContainer;
char symbolStrCodes[CODESCACHESIZE];
char *productList;
//资金返回
CFundsRes m_Funds;
CMoneyHolder m_MoneyHolder;
CPersonalInfo m_PersonalInfo;

map<long long, CFundsFlowRes*> fundsFlowContainer;//资金流水
char fundsFlowids[CODESCACHESIZE * 2];

map<long long, CMoneryInOutRes*> moneyInOutContainer;//出入金
char inoutmoneryids[CODESCACHESIZE * 2];

typedef map<long long, CFindOrderRes*> OrderContainer;

//历史单据
OrderContainer hisorderContainer;
//vector<long long> hisordercodeContainer;
char hisorderStrCodes[CODESCACHESIZE * 2];

//中远期历史
OrderContainer hiszyjOrderContainer;
char hiszyjorderStrCodes[CODESCACHESIZE * 2];

//持仓单
OrderContainer holdorderContainer;
vector<long long> holdordercodeContainer;
char holdorderStrCodes[CODESCACHESIZE];
char holderSymbolCodes[CODESCACHESIZE];

//平仓单
OrderContainer closeorderContainer;
vector<long long> closeordercodeContainer;
char closeorderStrCodes[CODESCACHESIZE];

//中远期今平仓单
OrderContainer closezyjorderContainer;
char closezyjorderStrCodes[CODESCACHESIZE];

//限价单
OrderContainer limitorderContainer;
vector<long long> limitordercodeContainer;
char limitorderStrCodes[CODESCACHESIZE];
char limitSymbolCodes[CODESCACHESIZE];

//成交单
OrderContainer bargainorderContainer;
vector<long long> bargainordercodeContainer;
char bargainorderStrCodes[CODESCACHESIZE];
char bargainSymbolCodes[CODESCACHESIZE];

//交收单据
OrderContainer settleorderContainer;
vector<long long> settleordercodeContainer;
char settleorderStrCodes[CODESCACHESIZE];
char settleSymbolCodes[CODESCACHESIZE];

//只在这里使用模块
typedef vector<CSHisKUnit> TKHisUnitArray;
typedef struct _KLineDataGroup
{
    int m_nKineDataType;           //保存了该数据的种类型
    TKHisUnitArray m_arrKLineData;  //保存该种K线类型的数据
}KLineDataGroup;

typedef map<char, KLineDataGroup*> KLineDataGroupContainer;//按K线类型
typedef map<string, KLineDataGroupContainer> TKLineDataContainer;//所有的K线数据都保存在这里 使用名字与组对应的方法
TKLineDataContainer klineDataContainer;

//中远期 历史
typedef struct _ZYQKLineDataGroup
{
    int m_nKineDataType;           //保存了该数据的种类型
    vector<STUHisKUnit_ZYQ> m_arrKLineData;  //保存该种K线类型的数据
}ZYQKLineDataGroup;
typedef map<int, ZYQKLineDataGroup> ZYQKLineDataGroupContainer;
typedef map<string, ZYQKLineDataGroupContainer> ZYQKLineDataContainer;
ZYQKLineDataContainer zyqKLineDataContainer;

typedef std::map<string, vector<STUTraceUnit_ZYQ>> ZYQTraceUnitArrayContainer;
typedef std::map<string, vector<STUSTKMinUnit_ZYQ>> ZYQMinUnitArrayContainer;
ZYQTraceUnitArrayContainer zyqTraceUnitArrayContainer;
ZYQMinUnitArrayContainer zyqMinUnitArrayContainer;

trade_ui_events tradeuievents;

//行情分时数据
typedef vector<CSTraceUnit> TraceUnitArray;
typedef std::map<string, TraceUnitArray*> TraceUnitArrayContainer;
TraceUnitArrayContainer traceUnitContainer;

//行情数据分笔
TraceUnitArrayContainer traceUnitRecodeContainer;

//公告数据
typedef std::map<int, CDeclareTitleDesc*> DeclareDescArrayContainer;
DeclareDescArrayContainer declareDescContainer;
char declareDescId[1024];

//交收仓库信息
typedef std::map<long long, COutCAUDeliveryWarehouseInfo*> DeliveryStoreInfoArrayContainer;
DeliveryStoreInfoArrayContainer deliveryStoreInfoArrayContainer;
char deliveryStoreInfo[1024];

//提货意向
typedef std::map<long long, CDeliveryInfo*> DeliveryInfoArrayContainer;

DeliveryInfoArrayContainer deliveryInfoArrayContainer;
char deliveryInfo[1024];

typedef std::map<long long, CStoreOrderInfo*> StoreOrderArrayContainer;

//入库单信息
StoreOrderArrayContainer inStoreOrderArrayContainer;
char inStoreOrderInfo[1024];

//入库申请单
StoreOrderArrayContainer applyInStoreOrderArrayContainer;
char applyInStoreOrderInfo[1024];

//出库单
StoreOrderArrayContainer outStoreOrderArrayContainer;
char outStoreOrderInfo[1024];

//仓单
StoreOrderArrayContainer cangOrderArrayContainer;
char cangOrderInfo[1024];

//仓单转入库单
StoreOrderArrayContainer cangToInStoreOrderArrayContainer;
char cangToInStoreOrderInfo[1024];

//请求map
typedef std::map<int, char*> RequestContainer;
RequestContainer mRequestContainer;

// 初始化行情
typedef vector<SStockInfo> StockInfoArray;
StockInfoArray m_vecStockInfo;

typedef vector<CStuSymbolData> SymbolInfoArray;
SymbolInfoArray m_vecSymbolInfo;

typedef struct _ZYJSTUSymbolTradeInfo {
    STUSymbolTradeTime_ZYQ stuSymbolTradeTime[5];
    STUSymbolInfo_ZYQ stuSymbolInfo;
    STUMarketInfo_ZYQ stuMarketInfo;
    STURealDataDetailPush_Ext_ZYQ stuRealDataDetailPush_Ext;
    STURealPublicDataPush stuRealPublicDataPush;
    STUNowDataVolPirce stuNowDataVolPirces[MAX_ZYQ_VOL_PRICE];
    unsigned short m_lenDirec;
} ZYJSTUSymbolTradeInfo;
typedef std::map<string, ZYJSTUSymbolTradeInfo> ZYJSymbolInfoArray;
ZYJSymbolInfoArray zyjSymbolInfo;

char LimitPriceHint[1024];
char HoldSymbolTickets[1024];
char LimitSymbolTickets[1024];

int nNowProgress = 0;
int max_progress = 3;

int nPankouNotice = 0;
int nRealNotice = 0;

//int nNeedClearHoldOrders = 0;
int nNeedClearHisOrders = 0;
int refresh_ing = 0;
//#define VALIDATE_TYPE_DAY '2'
//#define VALIDATE_TYPE_ALL '1'

extern "C"
{
#include "cJSON.h"
#include "global.h"
#include "nethelper.h"
#include "tagdef.h"
#include "msgbase.h"
#include "jxtnethelper.h"
    int  USERLOGINED = 0;
    char Jurisdiction[20]; //交易类型
    char LoginedAccountID[64];
    char LoginedSession[256];
    char LoginedAccountName[64];
    char LoginedAccountStatus[8];
    char LoginedNeedChangePwd[8];
    char LoginedAccountType[8];
    
    char ChannelCode;
    char BankNo[64];
    int SignStatus;
    char BankName[108];
    char ChannelUrl[1024];
    char Telephone[12];
    char BankCardNumber[64];
    char Identifier[19];
    
    
    long long toMod(long long ticket, int nDecimal) {
        return ticket % nDecimal;
    }
    
    void addSeq(int nSeq, int type, const char* tag) {
        SeqMap his;
        memset(&his, 0, sizeof(SeqMap));
        his.cType = type;
        strcpy(his.symbol, tag);
        seqContainer.insert(make_pair(nSeq, his));
    }
    
    int getSeq(int nSeq, int& type, char* tag) {
        map<int, SeqMap>::iterator iter = seqContainer.find(nSeq);
        if (iter != seqContainer.end()) {
            SeqMap map = (iter->second);
            type = map.cType;
            strcpy(tag, map.symbol);
            seqContainer.erase(nSeq);
            return 1;
        }
        return 0;
    }
    
    void getSHA1(char* hash, const unsigned char* buf, u_int32_t len){
        SHA1_CTX ctx;
        SHA1Init(&ctx);
        unsigned char tmp[21];
        SHA1Update(&ctx, buf, len);
        SHA1Final(tmp, &ctx);
        char tmp2[21];
        memset(tmp2, 0, sizeof(tmp2));
        for (int i = 0; i < 20; i++) {
            char b[10];
            sprintf(b, "%02x",(unsigned char)tmp[i]);
            strcat(tmp2, b);
        }
        strcpy(hash, tmp2);
    }
    
    void clearFundsFlowContainer(map<long long, CFundsFlowRes*> * container)
    {
        map<long long, CFundsFlowRes*>::iterator it = container->begin();
        map<long long, CFundsFlowRes*>::iterator itend = container->end();
        for ( ; it != itend; ) {
            CFundsFlowRes * order = (CFundsFlowRes*)it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    
    //  bug  1
    void clearMoneryInOutContainer(map<long long, CMoneryInOutRes*> * container)
    {
        map<long long, CMoneryInOutRes*>::iterator it = container->begin();
        map<long long, CMoneryInOutRes*>::iterator itend = container->end();
        for ( ; it != itend; ) {
            CMoneryInOutRes * order = (CMoneryInOutRes*)it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    void clearordercontainer(OrderContainer * container)
    {
        OrderContainer::iterator it = container->begin();
        OrderContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            CFindOrderRes * order = (CFindOrderRes * )it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    void clearStoreOrdercontainer(StoreOrderArrayContainer * container)
    {
        StoreOrderArrayContainer::iterator it = container->begin();
        StoreOrderArrayContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            CStoreOrderInfo * order = (CStoreOrderInfo * )it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    void clearDeliverySettleOrdercontainer(DeliveryStoreInfoArrayContainer * container)
    {
        DeliveryStoreInfoArrayContainer::iterator it = container->begin();
        DeliveryStoreInfoArrayContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            COutCAUDeliveryWarehouseInfo * order = (COutCAUDeliveryWarehouseInfo * )it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    
    void clearDeliveryOrdercontainer(DeliveryInfoArrayContainer * container)
    {
        DeliveryInfoArrayContainer::iterator it = container->begin();
        DeliveryInfoArrayContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            CDeliveryInfo * order = (CDeliveryInfo * )it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    void clearKLineContainer(TKLineDataContainer* container){
        TKLineDataContainer::iterator it = container->begin();
        TKLineDataContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            KLineDataGroupContainer::iterator itChild = it->second.begin();
            KLineDataGroupContainer::iterator itChildend = it->second.end();
            for(; itChild != itChildend;) {
                KLineDataGroup* group = (KLineDataGroup*)itChild->second;
                delete group;
                group = NULL;
                it->second.erase(itChild++);
            }
            container->erase(it++);
        }
    }
    
    void clearUnit(TraceUnitArrayContainer* container){
        TraceUnitArrayContainer::iterator it = container->begin();
        TraceUnitArrayContainer::iterator itend = container->end();
        for ( ; it != itend; ) {
            TraceUnitArray * order = (TraceUnitArray * )it->second;
            delete order;
            order = NULL;
            container->erase(it++);
        }
    }
    
    //回收内存 清空缓存等
    void recoveryQuoteMemory(){
        printBusLogEx("回收行情内存");
        zyqKLineDataContainer.clear();
        zyqMinUnitArrayContainer.clear();
        zyqTraceUnitArrayContainer.clear();
        clearKLineContainer(&klineDataContainer);
        clearUnit(&traceUnitContainer);
        clearUnit(&traceUnitRecodeContainer);
    }
    
    //回收内存 清空缓存等
    void recoveryMemory(){
        printBusLogEx("回收内存");
        recoveryQuoteMemory();
        clearordercontainer(&hisorderContainer);
        clearordercontainer(&hiszyjOrderContainer);
    }
    
    //清除缓存数据
    void clearCache(){
        printBusLogEx("清空缓存数据中.............................");
        recoveryMemory();
        symbolSortNumContainer.clear();
        symbolContainer.clear();
        declareDescContainer.clear();
        
        clearordercontainer(&holdorderContainer);
        clearordercontainer(&closeorderContainer);
        clearordercontainer(&limitorderContainer);
        clearordercontainer(&settleorderContainer);
        clearordercontainer(&bargainorderContainer);
        clearordercontainer(&closezyjorderContainer);
        
        bargainordercodeContainer.clear();
        inStoreOrderArrayContainer.clear();
        applyInStoreOrderArrayContainer.clear();
        
        holdordercodeContainer.clear();
        closeordercodeContainer.clear();
        limitordercodeContainer.clear();
        
        settleordercodeContainer.clear();
        
        clearordercontainer(&bargainorderContainer);
        
        bargainordercodeContainer.clear();
        
        clearStoreOrdercontainer(&inStoreOrderArrayContainer);
        clearStoreOrdercontainer(&applyInStoreOrderArrayContainer);
        clearStoreOrdercontainer(&outStoreOrderArrayContainer);
        clearStoreOrdercontainer(&cangOrderArrayContainer);
        clearStoreOrdercontainer(&cangToInStoreOrderArrayContainer);
        
        clearDeliveryOrdercontainer(&deliveryInfoArrayContainer);
        clearDeliverySettleOrdercontainer(&deliveryStoreInfoArrayContainer);
    }
    
    void resetCangToInStoreOrder(){
        memset(cangToInStoreOrderInfo, 0, sizeof(cangToInStoreOrderInfo));
        StoreOrderArrayContainer::iterator it;
        for(it = cangToInStoreOrderArrayContainer.begin(); it != cangToInStoreOrderArrayContainer.end(); ++it){
            char szTmp[20];
            if(strlen(cangToInStoreOrderInfo) > 0) sprintf(szTmp, ",%lld", it->second->iCangToImportId);
            else sprintf(szTmp, "%lld", it->second->iCangToImportId);
            strcat(cangToInStoreOrderInfo, szTmp);
        }
    }
    
    void resetCangOrder(){
        memset(cangOrderInfo, 0, sizeof(cangOrderInfo));
        StoreOrderArrayContainer::iterator it;
        for(it = cangOrderArrayContainer.begin(); it != cangOrderArrayContainer.end(); ++it){
            char szTmp[20];
            if(strlen(cangOrderInfo) > 0) sprintf(szTmp, ",%lld", it->second->iCangid);
            else sprintf(szTmp, "%lld", it->second->iCangid);
            strcat(cangOrderInfo, szTmp);
        }
    }
    
    cJSON* getcJsonRoot() {
        cJSON *root;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        return root;
    }
    
    int check_valid_quote_real_rsp(char * symbolcode)
    {
        SymbolContainer::iterator iter = symbolContainer.find(symbolcode);
        if (iter != symbolContainer.end()) {
            return 1;
        } else{
            return 0;
        }
    }
    
    ////////////////////
    CFindOrderRes * getCFindOrderRes(char * ticket);
    //    void cJSON_AddStringToObjectEx(cJSON * root, int tag, const char *content)
    //    {
    //        char tmp[6];
    //        memset(tmp, 0, 6);
    //        sprintf(tmp, "%d", tag);
    //        cJSON_AddStringToObject(root, tmp, content);
    //    }
    //
    //    void cJSON_AddDoubleToObjectEx(cJSON * root, int tag, double content)
    //    {
    //        char tmp[6];
    //        memset(tmp, 0, 6);
    //        sprintf(tmp, "%d", tag);
    //        cJSON_AddNumberToObject(root, tmp, content);
    //    }
    //
    //    cJSON * cJSON_GetObjectItemEx(cJSON * root, int tag)
    //    {
    //        char tmp[6];
    //        memset(tmp, 0, 6);
    //        sprintf(tmp, "%d", tag);
    //        return cJSON_GetObjectItem(root, tmp);
    //    }
    
    double getrealtips(double dbtips, int ndecial)
    {
        double db = dbtips;
        if (ndecial == 1) {
            return db / 10;
        }
        if (ndecial == 2) {
            return db / 100;
        }
        if (ndecial == 3) {
            return db / 1000;
        }
        if (ndecial == 4) {
            return db / 10000;
        }
        if (ndecial == 5) {
            return db / 100000;
        }
        return db;
    }
    
    void resetstringcodes(char * allcodes, int len, vector<long long> vec)
    {
        memset(allcodes, 0, len);
        vector<long long>::iterator iter;
        for ( iter = vec.begin() ; iter != vec.end() ; iter++ )
        {
            long long code = (long long)*iter;
            char str[25];
            if(strlen(allcodes) > 0) sprintf(str, ",%lld", code);
            else sprintf(str, "%lld", code);
            strcat(allcodes, str);
        }
        //        int lastindex = strlen(allcodes) -1;
        //        if(lastindex > -1)
        //        {
        //        	allcodes[lastindex] = '\0';
        //        }
    }
    
    void resetstringcodesSTRING(char * allcodes, int len, vector<string> vec)
    {
        memset(allcodes, 0, len);
        vector<string>::iterator iter;
        for ( iter = vec.begin() ; iter != vec.end() ; iter++ )
        {
            string code = (string)*iter;
            strcat(allcodes, code.c_str());
            strcat(allcodes, ",");
        }
        int lastindex = (int)strlen(allcodes) -1;
        if(lastindex > -1)
        {
            allcodes[lastindex] = '\0';
        }
    }
    
    void resetsymbolcodes()
    {
        //        vector<int> vectmp = symbolSortNumContainer;
        //        symbolSortNumContainer.clear();
        //        for (; ; ) {
        //            if (vectmp.size() == 0) {
        //                break;
        //            }
        //
        //            vector<int>::iterator itmin = vectmp.begin();
        //            int min = *itmin;
        //            for ( vector<int>::iterator it =  vectmp.begin(); it < vectmp.end(); it++) {
        //                if ( *it  < min) {
        //                    min = *it;
        //                    itmin = it;
        //                }
        //            }
        //            symbolSortNumContainer.push_back(min);
        //            vectmp.erase(itmin);
        //        }
        //
        //        //std::sort(symbolSortNumContainer.begin(),symbolSortNumContainer.end());
        //
        vector<string> tmpSymbolCodes;
        tmpSymbolCodes.clear();//
        //        vector<int>::iterator iter;
        //        for ( iter = symbolSortNumContainer.begin() ; iter != symbolSortNumContainer.end() ; iter++ )
        //        {
        //            int snum = *iter;
        SymbolContainer::iterator it;
        for (it = symbolContainer.begin(); it != symbolContainer.end(); it++) {
            CSymbolParam * symbol = it->second;
            //                if (snum == symbol->iShowOrder) {
            tmpSymbolCodes.push_back(symbol->ssSymbolcode);
            //                }
        }
        //        }
        resetstringcodesSTRING(symbolStrCodes, CODESCACHESIZE, tmpSymbolCodes);
    }
    
    //    void resethisordercodes()
    //    {
    //        resetstringcodes(hisorderStrCodes, CODESCACHESIZE, hisordercodeContainer);
    //    }
    int __OrderIdCompare(long long a1,long long a2){
        return a1>a2;
    }
    
    void resetholdordercodes()
    {
        holdordercodeContainer.clear();
        OrderContainer::iterator it = holdorderContainer.begin();
        OrderContainer::iterator itend = holdorderContainer.end();
        for ( ; it != itend; it++) {
            CFindOrderRes * order = (CFindOrderRes * )it->second;
            if(order->orderType == ZS_TYPE)
                holdordercodeContainer.push_back(order->iOrderticket);
            else if(order->orderType == ZYJ_TYPE)
                holdordercodeContainer.push_back(order->iBargainId);
        }
        std::sort(holdordercodeContainer.begin(), holdordercodeContainer.end(), &__OrderIdCompare);
        resetstringcodes(holdorderStrCodes, CODESCACHESIZE, holdordercodeContainer);
    }
    
    void resetcloseordercodes()
    {
        closeordercodeContainer.clear();
        OrderContainer::iterator it = closeorderContainer.begin();
        OrderContainer::iterator itend = closeorderContainer.end();
        for ( ; it != itend; it++) {
            CFindOrderRes * order = (CFindOrderRes * )it->second;
            closeordercodeContainer.push_back(order->iOrderticket);
        }
        std::sort(closeordercodeContainer.begin(), closeordercodeContainer.end(), &__OrderIdCompare);
        resetstringcodes(closeorderStrCodes, CODESCACHESIZE, closeordercodeContainer);
    }
    
    void resetlimitordercodes()
    {
        limitordercodeContainer.clear();
        OrderContainer::iterator it = limitorderContainer.begin();
        OrderContainer::iterator itend = limitorderContainer.end();
        for ( ; it != itend; it++) {
            CFindOrderRes * order = (CFindOrderRes * )it->second;
            limitordercodeContainer.push_back(order->iOrderticket);
            //            if(order->orderType == ZS_TYPE)
            //                limitordercodeContainer.push_back(order->iOrderticket);
            //            else if(order->orderType == ZYJ_TYPE)
            //                holdordercodeContainer.push_back(order->iBargainId);
        }
        std::sort(limitordercodeContainer.begin(), limitordercodeContainer.end(), &__OrderIdCompare);
        resetstringcodes(limitorderStrCodes, CODESCACHESIZE, limitordercodeContainer);
    }
    
    void resetsettleordercodes()
    {
        std::sort(settleordercodeContainer.begin(), settleordercodeContainer.end(), &__OrderIdCompare);
        resetstringcodes(settleorderStrCodes, CODESCACHESIZE, settleordercodeContainer);
    }
    
    void resetbargainordercodes()
    {
        std::sort(bargainordercodeContainer.begin(), bargainordercodeContainer.end(), &__OrderIdCompare);
        resetstringcodes(bargainorderStrCodes, CODESCACHESIZE, bargainordercodeContainer);
    }
    
    void setpricedirection(double lastprice, double newprice, int * direction) {
        if (lastprice == newprice) {
            *direction = 0;
        } else if( lastprice > newprice) {
            *direction = -1;
        } else {
            *direction = 1;
        }
    }
    
    double getfpl(double newprice, double holdprice, double num, double danwei, int tag) {
        if(newprice <= 0.00001 || holdprice <= 0.00001) return 0.0;
        double diffPrice = newprice - holdprice;
        return (diffPrice == 0.0) ? 0.0 : (diffPrice) * num * danwei * tag;
    }
    
    
    void setholdorderfpl(CFindOrderRes * holdorder, CSymbolParam * symbol)
    {
        if (holdorder->chSide == DIRECTION_TYPE_BUY) {
            holdorder->fFpl = getfpl(symbol->fBidPrice, holdorder->dbExecuteprice, holdorder->dbCumQty, symbol->dbAgreeunit, 1);
        } else{
            holdorder->fFpl = getfpl(symbol->fAskPrice, holdorder->dbExecuteprice, holdorder->dbCumQty, symbol->dbAgreeunit, -1);
        }
    }
    
    void setholdzyjorderfpl(CFindOrderRes * holdorder, CSymbolParam * symbol)
    {
        if (holdorder->chSide == DIRECTION_TYPE_BUY) {
            holdorder->fFpl = getfpl(symbol->fCurPrice, holdorder->dbHolderPrice, holdorder->dbCumQty, symbol->dbAgreeunit, 1);
        } else{
            holdorder->fFpl = getfpl(symbol->fCurPrice, holdorder->dbHolderPrice, holdorder->dbCumQty, symbol->dbAgreeunit, -1);
        }
    }
    
    void updateFPL(){
        double dbFPL = 0.0;
        double dbRisk = 0.0;
        double dbClosePL = 0.0;
        double dbTmpValue = 0.0;
        int iLevel = 0;
        
        OrderContainer::iterator itorder;
        for (itorder = holdorderContainer.begin(); itorder !=holdorderContainer.end() ; itorder++) {
            CFindOrderRes* holdorder = itorder->second;
            SymbolContainer::iterator iter = symbolContainer.find(holdorder->ssSymbol.c_str());
            if (iter != symbolContainer.end()) {
                CSymbolParam * symbol = iter->second;
                if(symbol->flastPrice > 0.0) {
                    if(symbol->symbolType == ZS_TYPE)
                        setholdorderfpl(holdorder, symbol);
                    else if(symbol->symbolType == ZYJ_TYPE)
                        setholdzyjorderfpl(holdorder, symbol);
                    dbFPL += holdorder->fFpl;
                    //                dbClosePL += holdorder->dbOpenClosePl;
                }
            }
        }
        
        //        OrderContainer::iterator it;
        //        for (it = closeorderContainer.begin(); it !=closeorderContainer.end() ; it++) {
        //            CFindOrderRes* holdorder = itorder->second;
        //            dbClosePL += holdorder->dbPl;
        //        }
        
        for (itorder = closeorderContainer.begin(); itorder !=closeorderContainer.end() ; itorder++) {
            CFindOrderRes* holdorder = itorder->second;
            SymbolContainer::iterator iter = symbolContainer.find(holdorder->ssSymbol.c_str());
            if (iter != symbolContainer.end()) {
                dbClosePL += holdorder->dbPl;
            }
        }
        //        m_Funds.dbFreezeMargin + getBalanceMargin() + m_Funds.dbUsedMargin;
        //        m_Funds.dbBalanceMargin + getTotalFPL()
        
        //double balance =  m_Funds.dbFreezeMargin + (m_Funds.dbBalanceMargin + dbFPL) + m_Funds.dbUsedMargin;
        //dbTmpValue = balance + m_Funds.dbCredit ;
        
        
        double balance =  m_Funds.dbFreezeMargin + m_Funds.dbBalanceMargin  + m_Funds.dbUsedMargin;
        dbTmpValue = balance + dbFPL + m_Funds.dbCredit ;
        
        //        double dbTmpValue2 = m_Funds.dbOpenbalance + dbFPL + m_Funds.dbCredit;
        //        if(dbTmpValue != dbTmpValue2) {
        //            printBusLogEx("净值=%.2f  %.2f", dbTmpValue, dbTmpValue2);
        //        }
        //        if(m_Funds.dbOpenbalance != balance) {
        //        }
        if(m_Funds.chRiskcalctype == '2') {
            if(m_Funds.dbUsedMargin > gEPSINON) {
                dbRisk = (dbTmpValue / m_Funds.dbUsedMargin) *  100.0;
                if (dbRisk >= gEPSINON && dbRisk <= (m_Funds.dbStopOutLevel + m_Funds.dbMarginCallLevel) / 2 ) {
                    iLevel = 2;
                } else if (dbRisk >= gEPSINON && dbRisk <= m_Funds.dbMarginCallLevel) {
                    iLevel = 1;
                } else {
                    iLevel = 0;
                }
            }
        } else if (dbTmpValue > gEPSINON) {
            dbRisk = (m_Funds.dbUsedMargin / dbTmpValue)  * 100.0;
            if (dbRisk >= gEPSINON && dbRisk >= (m_Funds.dbStopOutLevel + m_Funds.dbMarginCallLevel) / 2 ) {
                iLevel = 2;
            } else if (dbRisk >= gEPSINON && dbRisk >= m_Funds.dbMarginCallLevel) {
                iLevel = 1;
            } else {
                iLevel = 0;
            }
        }
        m_Funds.dbRisk = dbRisk;
        m_Funds.iLevel = iLevel;
        m_Funds.dbClosePL = dbClosePL;
        m_Funds.dbFPL = dbFPL;
        m_Funds.dbTmpValue = dbTmpValue;
    }
    
    void set_trade_ui_events(trade_ui_events events)
    {
        tradeuievents = events;
    }
    
    void set_login_progress_rsp() {
        nNowProgress++;
        tradeuievents.trade_ui_login_progress_rsp(nNowProgress, max_progress);
        if(refresh_ing != 0 && nNowProgress > 1 && nNowProgress == max_progress){
            refresh_ing = 0;
            tradeuievents.trade_ui_refresh_complete(0);
        }
        printBusLogEx("progress=[%d]  max_progress=[%d]", nNowProgress, max_progress);
    }
    
    // 启动业务
    void request_trade_info() {
//        if(getIsZS() == 1)  max_progress += 2;
//        if(getIsZYJ() == 1) max_progress += 5;
//        if(getIsStore() == 1) max_progress += 7;
//        trade_request_funds();
        int iCount=0;
        char ch;
        do{
            ch = Jurisdiction[iCount];
            if(ch == ACCESS_MAKE_MARKET || strlen(Jurisdiction) == 0) {
                trade_request_symbol();
//                trade_request_order();
                trade_request_moneyhold();//资产持仓
                if(isWifi() == WIFI) {
                }
            } else if(ch == ACCESS_LONG_TREM) {
                zyj_trade_request_symbol();
                zyj_trade_hold_order();
                zyj_trade_settle_order();
                zyj_trade_delegate_order();
                zyj_trade_deal_order();
            } else if(ch == ACCESS_WAREHOUSE) {
                query_apply_delivery_intention();
                query_sellt_store();
                query_in_store_order();
                query_apply_in_store();
                trade_query_cang();
                trade_query_cang_to_instore_order();
                trade_query_out_store_order();
                if(isWifi() == WIFI) {

                }
            } else if(ch == ACCESS_NEUTRAL_WAREHOUSE) {
                
            }
            iCount += 1;
        } while (iCount<strlen(Jurisdiction));
//        trade_request_declare_title();
        trade_query_inout_monery();
    }
    
    long long before = 0;
    
    void handler_trade_refresh(int n){
        refresh_begin();
        refresh_ing = 1;
        clearCache();
        request_trade_info();
        refresh_end();
    }
    
    int isRefresh(){
        time_t timel;
        time(&timel);
        if(timel - before < REFRESH_INTERVAL_SECONDS) return 1;
        return 0;
    }
    
    int refresh_account_info(){
        time_t timel;
        time(&timel);
        if(timel - before < REFRESH_INTERVAL_SECONDS) {
            tradeuievents.trade_ui_refresh_complete(1);
            return 1;
        }
        before = timel;
        printBusLogEx("正在刷新");
        nhp_refresh();
        return 0;
    }
    int getBankPasswordType() {
        switch (ChannelCode) {
            case '9':return 1;
            case 'F':return 2;
            case '7':case 'B':return 3;
            default: return 0;
        }
        return 0;
    }
    int getSuportOutMonery() {
        switch (ChannelCode) {
            case '1':
            case '3':
            case '4':
            case '5':
            case '6':
            case '9':
            case 'A':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G': return 1;
            default: return 0;
        }
        return 0;
    }
    int getSuportInMonery() {
        switch (ChannelCode) {
            case '1':
            case '3':
            case '4':
            case '5':
            case '6':
            case '9':
            case 'A':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G': return 1;
            default: return 0;
        }
        return 0;
    }
    int getSuportSign() {
        switch (ChannelCode) {
            case 'B':
            case 'F':
            case '8':
            case '9': return 1;
            default: return 0;
        }
        return 0;
    }
    int getSuportUnSign() {
        switch (ChannelCode) {
            case 'A':
            case 'B':
            case 'F':
            case '8':
            case '9': return 1;
            default: return 0;
        }
        return 0;
    }
    void trade_login_rsp(char * json)
    {
        clearCache();
        memset(Jurisdiction, 0, sizeof(Jurisdiction));
        refresh_ing = 0;
        cJSON *root;
        cJSON *retinfo;
        cJSON *accountid;
        cJSON *session;
        cJSON *accountname;
        cJSON *accountstatus;
//        cJSON *needchangepwd;
        cJSON *accounttype;
//        printBusLogEx("trade_login_rsp json:%s\n",json);
        root = cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){ //如果登录成功
                USERLOGINED = 1;
                accountid = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_ACCTID);
                session   = cJSON_GetObjectItemEx(root, FIX_TAG_HEADER_SESSION);
                accountname   = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_USERNAME);
                accountstatus   = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_USERSTATUS);
                //                needchangepwd   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_NEEDCHANGEPWD);
                accounttype   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_ACCOUNTTYPE);
                cJSON *szJurisdiction   = cJSON_GetObjectItemEx(root, FIX_TAG_JURISDICTION);
                
                cJSON *szChannelCode   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_CHANNELCODE);
                cJSON *szChannelUrl   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_CHANNELURL);
                //                cJSON *szUpgradeUrl   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_UPGRADEURL);
                cJSON *szBankNo   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_BANKCUSTOMERNO);
                cJSON *szSignStatus   = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_BANKSIGNSTATUS);
                cJSON *szBankName  = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_BANKID);
                cJSON *szTelephone  =   cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_TELEPHONE);
                cJSON *szBankCardNO = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_BANKCARDNUM); // ex
                cJSON *szIdentifier = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_IDCARD_RECV);
                //                char ChannelCode;
                //                int SignStatus;
                memset(BankNo, 0, sizeof(BankNo));
                memset(BankName, 0, sizeof(BankName));
                memset(ChannelUrl, 0, sizeof(ChannelUrl));
                memset(Telephone, 0, sizeof(Telephone));
                memset(BankCardNumber, 0, sizeof(BankCardNumber));
                memset(Identifier, 0, sizeof(Identifier));
                memset(LoginedAccountID, 0, 64);
                memset(LoginedSession, 0, 256);
                memset(LoginedAccountName, 0, 256);
                memset(LoginedAccountStatus, 0, 256);
                memset(LoginedNeedChangePwd, 0, 256);
                memset(LoginedAccountType, 0, 256);
                
                strcpy(LoginedAccountID, accountid->valuestring);
                strcpy(LoginedSession, session->valuestring);
                strcpy(LoginedAccountName, accountname->valuestring);
                strcpy(LoginedAccountStatus, accountstatus->valuestring);
                //                strcpy(LoginedNeedChangePwd, needchangepwd->valuestring);
                //                strcpy(LoginedAccountType, accounttype->valuestring);
                
                if(NULL != szChannelCode) ChannelCode = szChannelCode->valuestring[0];
                if(NULL != szSignStatus) SignStatus = atoi(szSignStatus->valuestring);
                if(NULL != szChannelUrl) strcpy(ChannelUrl, szChannelUrl->valuestring);
                if(NULL != szBankNo) strcpy(BankNo, szBankNo->valuestring);
                if(NULL != szBankName) strcpy(BankName, szBankName->valuestring);
                if(NULL != szTelephone) strcpy(Telephone, szTelephone->valuestring);
                if(NULL != szBankCardNO) strcpy(BankCardNumber, szBankCardNO->valuestring);
                if(NULL != szIdentifier) strcpy(Identifier, szIdentifier->valuestring);
                //2.0
                if(NULL != szJurisdiction) strcpy(Jurisdiction, szJurisdiction->valuestring);
                
                nRet = TRADE_MSGTYPE_RES_OK;
            }
        }
        cJSON_Delete(root);
        tradeuievents.trade_ui_login_rsp(TRADE_TYPE, nRet);
        if(TRADE_MSGTYPE_RES_OK == nRet) {
            request_trade_info();
            set_login_progress_rsp(); // 需要放到初始化交易信息后面，最大进度条初始化
            printBusLogEx("初始化登陆:step:[%d] 登陆应答",nNowProgress);
        } else {
//            nhp_stop_session();    //首次自动登录交易失败会断开行情链路导致bug
        }
    }
    
    int getSignStatus(){
        return SignStatus;
    }
    
    //登录
    void trade_login(char * account, char * password) {
        cJSON *root;
        char *out;
        
        printBusLogEx("login account: %s password: %s", account, password);
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, account);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_CUSTOMPWD, password);
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("login json:\n%s\n", out);
        USERLOGINED = 0;
        nNowProgress = 0;
        nhp_trade_login(out);
        
        free(out);
        //        free(buf);
        printBusLogEx("trade_login end");
    }
    
    //
    void get_ordertickets_from_json(cJSON * root)
    {
        string orderkey = "ORDERTICKET";
        cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
        if(NULL == orders) return;
        int ordercount = cJSON_GetArraySize(orders);
        
        for (int i = 0; i < ordercount; i++) {
            cJSON * order = cJSON_GetArrayItem(orders, i);
            CFindOrderRes *findOrder = new CFindOrderRes;
            convert_order_from_jsonnode(ZS_TYPE, order, (char*)findOrder);
            if (findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER && findOrder->chIsClose == TRADE_TYPE_OPEN)
            {
                holdorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                holdordercodeContainer.push_back(findOrder->iOrderticket);
            } else {
                if (findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                    closeorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                    closeordercodeContainer.push_back(findOrder->iOrderticket);
                } else {
                    if (findOrder->chOrdStatus != ORDER_STATUS_TURN_OVER &&
                        findOrder->chOrdStatus != ORDER_STATUS_CANCEL)
                    {
                        findOrder->dbCumQty = findOrder->dbOrderQty;
                        limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                        limitordercodeContainer.push_back(findOrder->iOrderticket);
                    } else {
                        closeorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                        closeordercodeContainer.push_back(findOrder->iOrderticket);
                    }
                }
            }
        }
        resetholdordercodes();
        resetlimitordercodes();
        resetcloseordercodes();
    }
    
    void get_funds_from_json(cJSON * root) {
        
        cJSON *jNode = NULL;
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTID);
        if(NULL != jNode) m_MoneyHolder.dAccountID = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTBALANCE);
        if(NULL != jNode) m_MoneyHolder.dAccountBalance = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_ACCTFREEZEMONEY);
        if(NULL != jNode) m_MoneyHolder.dAccountFreezeMoney = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_CLEARDATE);
        if(NULL != jNode) m_MoneyHolder.dClearDate = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_TOTALBALANCE);
        if(NULL != jNode) m_MoneyHolder.dTotalBalance = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_CANOUT);
        if(NULL != jNode) m_MoneyHolder.dCanOut = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_OTHERFREEZE);
        if(NULL != jNode) m_MoneyHolder.dOtherFreeze = atof(jNode->valuestring);
        
//        tradeuievents.trade_ui_moneyhold_rsp(TRADE_MSGTYPE_RES_OK);
//        cJSON* jNode = NULL;
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_BALANCEMARGIN);
//        if(NULL != jNode) m_Funds.dbBalanceMargin = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_USERMARGIN);
//        if(NULL != jNode) m_Funds.dbUsedMargin = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_FREEZEMARGIN);
//        if(NULL != jNode) m_Funds.dbFreezeMargin = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_LOCKMARGIN);
//        if(NULL != jNode) m_Funds.dbLockMargin = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_CREDIT);
//        if(NULL != jNode) m_Funds.dbCredit = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_COMMISION);
//        if(NULL != jNode) m_Funds.dbCommision = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_PL);
//        if(NULL != jNode) m_Funds.dbPl = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_INTEREST);
//        if(NULL != jNode) m_Funds.dbInterest = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_IN);
//        if(NULL != jNode) m_Funds.dbIn = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_OUT);
//        if(NULL != jNode) m_Funds.dbOut = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_ADJUSTMENT);
//        if(NULL != jNode) m_Funds.dbAdjustment = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_OPENBALANCE);
//        if(NULL != jNode) m_Funds.dbOpenbalance = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_LASTMARGIN);
//        if(NULL != jNode) m_Funds.dbLastmargin = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_FPL);
//        if(NULL != jNode) m_Funds.dbFpl = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_SUMPOSITION);
//        if(NULL != jNode) m_Funds.dbSumposition = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_CURRENCYTYPE);
//        if(NULL != jNode)  m_Funds.ssCurrency = jNode->valuestring;
//        
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_CURRENCYTYPE);
//        if(NULL != jNode)  m_Funds.ssCurrency = jNode->valuestring;
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_MARGINCALLLEVEL);
//        if(NULL != jNode)  m_Funds.dbMarginCallLevel = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_STOPOUTLEVEL);
//        if(NULL != jNode) m_Funds.dbStopOutLevel = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_RISKCALCTYPE);
//        if(NULL != jNode)  m_Funds.chRiskcalctype = jNode->valuestring[0];
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_TRADEORDERMODE);
//        if(NULL != jNode)  m_Funds.chTradeOrderMode = jNode->valuestring[0];
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_NODEBTCLEAR);
//        if(NULL != jNode)  m_Funds.chNodebtclear = jNode->valuestring[0];
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_MININMOUNTEVERYTIME);
//        if(NULL != jNode)  m_Funds.dbMininmounteverytime = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_INOUTCOMMISION);
//        if(NULL != jNode)  m_Funds.dbInoutcommision = atof(jNode->valuestring);
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_INOUTCOMMISIONTYPE);
//        if(NULL != jNode)  m_Funds.chInoutcommisiontype = jNode->valuestring[0];
//        
//        jNode= cJSON_GetObjectItemEx(root, FIX_TAG_BODY_SYMBOL_ROULE_GROUP_NAME);
//        if(NULL != jNode)  m_Funds.ssSymbolRuleGroupName = jNode->valuestring;
        
//        tradeuievents.trade_ui_funds_change_rsp(TRADE_MSGTYPE_RES_OK);
        
    }
    void removeLimitOrderRes(long long ticket){
        map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(ticket);
        if (iter != limitorderContainer.end())
        {
            CFindOrderRes* pTmp = iter->second;
            limitorderContainer.erase(iter);
            delete pTmp;
        }
        vector<long long>::iterator l;
        for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
            long long s = (long long)(*l);
            if(s == ticket) {
                limitordercodeContainer.erase(l);
                break;
            }
        }
    }
    void removeHoldOrderRes(long long ticket){
        map<long long, CFindOrderRes*>::iterator iter =  holdorderContainer.find(ticket);
        if (iter != holdorderContainer.end())
        {
            CFindOrderRes* pTmp = iter->second;
            holdorderContainer.erase(iter);
            delete pTmp;
        }
        vector<long long>::iterator l;
        for(l = holdordercodeContainer.begin(); l != holdordercodeContainer.end(); l++) {
            long long s = (long long)(*l);
            if(s == ticket) {
                holdordercodeContainer.erase(l);
                break;
            }
        }
    }
    
    void removeCloseOrderRes(long long ticket){
        map<long long, CFindOrderRes*>::iterator iter =  closeorderContainer.find(ticket);
        if (iter != closeorderContainer.end())
        {
            CFindOrderRes* pTmp = iter->second;
            closeorderContainer.erase(iter);
            delete pTmp;
        }
        vector<long long>::iterator l;
        for(l = closeordercodeContainer.begin(); l != closeordercodeContainer.end(); l++) {
            long long s = (long long)(*l);
            if(s == ticket) {
                closeordercodeContainer.erase(l);
                break;
            }
        }
    }
    
    void addHoldOrderRes(long long ticket, CFindOrderRes* findOrder){
        holdorderContainer.insert(make_pair(ticket, findOrder));
        holdordercodeContainer.push_back(ticket);
    }
    
    void addCloseOrderRes(long long ticket, CFindOrderRes* findOrder){
        closeorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
        closeordercodeContainer.push_back(findOrder->iOrderticket);
    }
    
    void addLimitOrderRes(long long ticket, CFindOrderRes* findOrder){
        removeLimitOrderRes(ticket);
        limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
        limitordercodeContainer.push_back(findOrder->iOrderticket);
    }
    
    void trade_open_order_rsp(int nSeq, char * json) {
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        long long orderticket;
        string orderprice;
        long long ticket = 0;
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){ // 下单正常
                orderticket = atoll(cJSON_GetObjectItemEx(root, FIX_TAG_BODY_ORDERID)->valuestring);
                orderprice  = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_EXECUTETYPE)->valuestring;
                int orderState = atoi(cJSON_GetObjectItemEx(root, FIX_TAG_BODY_EXECUTETYPE)->valuestring);
                //                orderticket
                tradeuievents.trade_ui_openorder_rsp(nSeq, orderState, orderticket);
                
                
                get_funds_from_json(root);
                string orderkey = "ORDERTICKET";
                cJSON *orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL == orders) return;
                int ordercount = cJSON_GetArraySize(orders);
                for (int i = 0; i < ordercount; i++) {
                    cJSON * order = cJSON_GetArrayItem(orders, i);
                    CFindOrderRes *findOrder = new CFindOrderRes;
                    convert_order_from_jsonnode(ZS_TYPE, order, (char*)findOrder);
                    ticket = findOrder->iOrderticket;
                    printBusLogEx("chOrderType=[%c] chOrdStatus=[%c] chIsClose=[%c]",findOrder->chOrderType, findOrder->chOrdStatus, findOrder->chIsClose);
                    switch (findOrder->chOrderType) {
                        case ORDER_TYPE_MARKET: {
                            if(findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER){
                                if(findOrder->chIsClose == TRADE_TYPE_OPEN) {
                                    removeHoldOrderRes(findOrder->iOrderticket);
                                    addHoldOrderRes(findOrder->iOrderticket, findOrder);
                                } else if(findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                                    removeHoldOrderRes(findOrder->iOrderticket);
                                    addCloseOrderRes(findOrder->iOrderticket, findOrder);
                                }
                            }
                            break;
                        }
                        case ORDER_TYPE_LIMIT: {
                            if(findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER){
                                if(findOrder->chIsClose == TRADE_TYPE_OPEN) {
                                    removeLimitOrderRes(findOrder->iOrderticket);
                                    addHoldOrderRes(findOrder->iOrderticket, findOrder);
                                } else if(findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                                    removeHoldOrderRes(findOrder->iOrderticket);
                                    addCloseOrderRes(findOrder->iOrderticket, findOrder);
                                }
                            } else if(findOrder->chOrdStatus==ORDER_STATUS_CANCEL) {
                                removeLimitOrderRes(findOrder->iOrderticket);
                                addCloseOrderRes(findOrder->iOrderticket, findOrder);
                            } else {
                                findOrder->dbCumQty = findOrder->dbOrderQty;
                                removeLimitOrderRes(findOrder->iOrderticket);
                                addLimitOrderRes(findOrder->iOrderticket, findOrder);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    
                }
                updateFPL();
                nRet = TRADE_MSGTYPE_RES_OK;
            }
            else{  // 异常情况
                nRet = atoi(retcode->valuestring);
                tradeuievents.trade_ui_openorder_rsp(nSeq, nRet, 0);
            }
        }
        resetholdordercodes();
        resetlimitordercodes();
        resetcloseordercodes();
        cJSON_Delete(root);
        printBusLogEx("trade_open_order_rsp end");
//        tradeuievents.trade_ui_openorder_rsp(nSeq, nRet, ticket);  // 回调两次
    }
    
    int getHint(CSymbolParam * symbolParam , double price, char sub_order_type, SL_TP_Hint* hint) {
        if (symbolParam == NULL) return -1;
        
        int result = -2;
        double real_sl_pips = getrealtips(symbolParam->dbSlpips, symbolParam->nDecimal);
        double real_tp_pips = getrealtips(symbolParam->dbTppips, symbolParam->nDecimal);
        double real_limitorstop_pips = getrealtips(symbolParam->dbLimitorstoppips, symbolParam->nDecimal);
        
        double buy_pips = getrealtips(symbolParam->dbBuypips, symbolParam->nDecimal);
        
        double slPrice = 0;
        double tpPrice = 0;
        char fmt[10];
        char slfmt[10];
        char tpfmt[10];
        switch (sub_order_type) {
            case SUB_ORDER_TYPE_BUY: {
                sprintf(fmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                slPrice = symbolParam->fBidPrice - real_sl_pips;
                tpPrice = symbolParam->fAskPrice + real_tp_pips;
                result= CHECK_SUCCESS;
                break;
            }
            case SUB_ORDER_TYPE_SELL:{
                sprintf(fmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                slPrice = symbolParam->fAskPrice + real_sl_pips;
                tpPrice = symbolParam->fBidPrice  - real_tp_pips;
                result= CHECK_SUCCESS;
                break;
            }
            case SUB_ORDER_TYPE_BUY_LIMIT: {
                sprintf(fmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                slPrice = price - buy_pips - real_sl_pips;
                tpPrice = price + real_tp_pips;
                //                price = symbolParam->fAskPrice  - buy_pips - real_limitorstop_pips;
                //                price = symbolParam->fAskPrice  - real_limitorstop_pips;
                price = symbolParam->fBidPrice  - real_limitorstop_pips;
                result= CHECK_SUCCESS;
                break;
            }
            case SUB_ORDER_TYPE_BUY_STOP: {
                sprintf(fmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                slPrice = price - buy_pips - real_sl_pips;
                tpPrice = price + real_tp_pips;
                price = symbolParam->fAskPrice + real_limitorstop_pips;
                result= CHECK_SUCCESS;
                break;
            }
            case SUB_ORDER_TYPE_SELL_LIMIT: {
                sprintf(fmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                slPrice = price + buy_pips + real_sl_pips;
                tpPrice = price - real_tp_pips;
                //                price = symbolParam->fBidPrice + real_limitorstop_pips + buy_pips;
                price = symbolParam->fAskPrice + real_limitorstop_pips;
                result= CHECK_SUCCESS;
                break;
            }
            case SUB_ORDER_TYPE_SELL_STOP: {
                sprintf(fmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                sprintf(slfmt, "%s%d%s",">= %.", symbolParam->nDecimal, "f");
                sprintf(tpfmt, "%s%d%s","<= %.", symbolParam->nDecimal, "f");
                slPrice = price + buy_pips + real_sl_pips;
                tpPrice = price - real_tp_pips;
                price = symbolParam->fBidPrice - real_limitorstop_pips;
                result= CHECK_SUCCESS;
                break;
            }
        }
        if(result == CHECK_SUCCESS){
            char ssSLtmp[32];
            char ssTptmp[32];
            char sstmp[32];
            memset(ssSLtmp, 0, sizeof(ssSLtmp));
            memset(ssTptmp, 0, sizeof(ssTptmp));
            memset(sstmp, 0, sizeof(sstmp));
            
            if(price > 0.0) sprintf(sstmp, fmt, price);
            strcpy(hint->price, sstmp);
            
            if(slPrice > 0.0) sprintf(ssSLtmp, slfmt, slPrice);
            strcpy(hint->SlPrice, ssSLtmp);
            
            if(tpPrice > 0.0) sprintf(ssTptmp, tpfmt, tpPrice);
            strcpy(hint->TpPrice, ssTptmp);
            printBusLogEx("price%s sl%s, tp%s\n",hint->price, hint->SlPrice, hint->TpPrice);
        }
        return result;
    }
    
    int getLimitHint(char * symbolcode, double price, char sub_order_type, SL_TP_Hint* hint){
        CSymbolParam * symbolParam = NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        if (it != symbolContainer.end()) {
            symbolParam = it->second;
        }
        
        if (symbolParam == NULL) {
            return -1;
        }
        return getHint(symbolParam, price, sub_order_type, hint);
    }
    
    int getMarketHint(char * symbolcode, char sub_order_type, SL_TP_Hint* hint){
        CSymbolParam * symbolParam = NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        if (it != symbolContainer.end()) {
            symbolParam = it->second;
        }
        
        if (symbolParam == NULL) {
            return -1;
        }
        
        double price = 0;
        switch ( sub_order_type) {
            case 'b':
            case SUB_ORDER_TYPE_BUY: {
                price = symbolParam->fAskPrice;
                sub_order_type = SUB_ORDER_TYPE_BUY;
                break;
            }
            case 's':
            case SUB_ORDER_TYPE_SELL:{
                price = symbolParam->fBidPrice;
                sub_order_type = SUB_ORDER_TYPE_SELL;
                break;
            }
        }
        return getHint(symbolParam, price, sub_order_type, hint);
    }
    
    int check_tp(char * symbolcode,  char direction, double tp, double price){
        CSymbolParam * symbolParam = NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        if (it != symbolContainer.end()) symbolParam = it->second;
        if (symbolParam == NULL) return CHECK_NOSYMBOL;//无此商品编码
        double real_tp_pips = getrealtips(symbolParam->dbTppips, symbolParam->nDecimal);
        if (direction == DIRECTION_TYPE_SELL) {
            if (tp > 0.0 && (price - tp) <  real_tp_pips) {
                return CHECK_TPTOOLOW;
            }
        } else{
            if (tp > 0.0 && (tp - price) <  real_tp_pips) {
                return CHECK_TPTOOLOW;
            }
        }
        return 0;
    }
    int check_sl(char * symbolcode,  char direction, double sl, double price){
        CSymbolParam * symbolParam = NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        if (it != symbolContainer.end()) symbolParam = it->second;
        if (symbolParam == NULL) return CHECK_NOSYMBOL;//无此商品编码
        double real_sl_pips = getrealtips(symbolParam->dbSlpips, symbolParam->nDecimal);
        if (direction == DIRECTION_TYPE_SELL) {
            if (sl > 0.0 && (sl - price) < real_sl_pips) {
                return CHECK_SLTOOLOW;
            }
        } else{
            if (sl > 0.0 && (price - sl) < real_sl_pips) {
                return CHECK_SLTOOLOW;
            }
        }
        return 0;
    }
    int trade_open_orderex(char * symbolcode, char direction, double price, int shoushu) {
        cJSON *root;
        char *out;
        char sstmp[32];
        string strspace;
        
        /* //无此商品编码
        CSymbolParam * symbolParam = NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        
        if (it != symbolContainer.end()) symbolParam = it->second;
        
         
        if (symbolParam == NULL) return CHECK_NOSYMBOL;//无此商品编码
        */
        //        price = Rounding(price, symbolParam->nDecimal);
        //        sl = Rounding(sl, symbolParam->nDecimal);
        //        tp = Rounding(tp, symbolParam->nDecimal);
        //        shoushu = Rounding(shoushu, QTY_DECIMAL);
        //        rqty = Rounding(rqty, QTY_DECIMAL);
        
        printBusLogEx("trade_open_order start");
        
        strspace = "";
        root=cJSON_CreateObject();
        
        
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%s", symbolcode);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, sstmp);
        
        memset(sstmp, 0, 32);
        if (direction == 'b') {
            sprintf(sstmp, "%s", "0B");
        }else if (direction == 's'){
            sprintf(sstmp, "%s", "0S");
        }
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERTYPE, sstmp);
        
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%f", price);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERPRICE, sstmp);
        
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%d", shoushu);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_APPLICATIONVOL, sstmp);
        
        out=cJSON_Print(root);
        
        cJSON_Delete(root);
        
        printBusLogEx("trade_open_order json:%s", out);
        
        int nSeq = nhp_trade_open(out);
        
        free(out);
        
        printBusLogEx("trade_open_order end");
        return nSeq;
    }
    
    int trade_open_market_order(char * symbolcode, int shoushu, double sl, double tp, char direction, double price, int diff)
    {
        return trade_open_orderex(symbolcode,direction,price,shoushu);
    }
    //int trade_open_orderex(char * symbolcode, char direction, double price, double shoushu)
    
    int trade_open_limit_order(char * symbolcode, double shoushu, double sl, double tp, char ordertype, double price, char valdate_type, char * valdate)
    {
        char direction;
        if(ordertype == SUB_ORDER_TYPE_BUY_LIMIT || ordertype == SUB_ORDER_TYPE_BUY_STOP ) {
            direction = DIRECTION_TYPE_BUY;
        } else{
            direction = DIRECTION_TYPE_SELL;
        }
        return trade_open_orderex(symbolcode, direction, price, shoushu);
    }
    
    
    int trade_modify_limit_order(long long orderticket, double price, double sl, double tp)
    {
        OrderContainer::iterator it = limitorderContainer.find(orderticket);
        CFindOrderRes * order = NULL;
        if (it != limitorderContainer.end()) {
            order = it->second;
        }
        
        printBusLogEx("trade_modify_limit_order %lld limitorderContainersize%d limitorderStrCodes:%s", orderticket,
                      limitorderContainer.size(), limitorderStrCodes);
        if (order == NULL) {
            vector<long long>::iterator l;
            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                long long s = (long long)(*l);
                if(s == orderticket) {
                    limitordercodeContainer.erase(l);
                    break;
                }
            }
            resetlimitordercodes();
            return CHECK_ORDERCODEERROR;
        }
        char* valdate =(char*) order->ssExpirationdate.c_str();
        char valdate_type = order->chExpiration;
        //        return trade_open_orderex(TRADE_FLAG_MODIFY_LIMIT, TRADE_TYPE_OPEN, ORDER_TYPE_LIMIT, (char)order->chOrderType,order->chSide,(char*)order->ssSymbol.c_str(), orderticket, order->dbOrderQty, sl, tp, price, valdate_type,valdate, 0,-1);
        return 1;
    }
    
    int trade_cancel_limit_order(long long orderticket)
    {
        OrderContainer::iterator it = limitorderContainer.find(orderticket);
        CFindOrderRes * order = NULL;
        if (it != limitorderContainer.end()) {
            order = it->second;
        }
        
        printBusLogEx("trade_cancel_limit_order %lld limitorderContainersize%d limitorderStrCodes:%s", orderticket,
                      limitorderContainer.size(), limitorderStrCodes);
        
        if (order == NULL) return CHECK_ORDERCODEERROR;
        
        //        return trade_open_order(TRADE_FLAG_CANCEL, '\0', ORDER_TYPE_LIMIT, (char)order->chOrderType,(char)order->chSide,(char*)order->ssSymbol.c_str(), orderticket, order->dbOrderQty,order->dbSlPrice, order->dbStopPx, order->dbPrice,0,-1);
        return 1;
    }
    
    
    int trade_modify_open_order(long long orderticket, double sl, double tp)
    {
        OrderContainer::iterator it = holdorderContainer.find(orderticket);
        CFindOrderRes * order = NULL;
        if (it != holdorderContainer.end()) {
            order = it->second;
        }
        printBusLogEx("trade_modify_open_order%lld holdorderContainersize%d holdorderStrCodes:%s", orderticket,
                      holdorderContainer.size(), holdorderStrCodes);
        if (order == NULL) return CHECK_ORDERCODEERROR;
        double price = order->dbExecuteprice;
        char * ssSymbol = (char*)order->ssSymbol.c_str();
        if(order->chSide == DIRECTION_TYPE_BUY){
            price = getBidPrice(ssSymbol);
        } else {
            price = getAskPrice(ssSymbol);
        }
        //        return trade_open_order(TRADE_FLAG_MODIFY_SL_TP, TRADE_TYPE_OPEN, ORDER_TYPE_MARKET, order->chOrderType,order->chSide, (char*)order->ssSymbol.c_str(), orderticket, order->dbCumQty, sl, tp, price, 0, -1);
        return 1;
    }
    int trade_close_order(long long orderticket, double shoushu, double rqty, int diff)
    {
        OrderContainer::iterator it = holdorderContainer.find(orderticket);
        CFindOrderRes * order = NULL;
        if (it != holdorderContainer.end()) {
            order = it->second;
        }
        if (order == NULL)  return CHECK_ORDERCODEERROR;
        printBusLogEx("trade_modify_limit_order %lld ", orderticket);
        //        return trade_open_order(TRADE_FLAG_NEW, TRADE_TYPE_CLOSE, '\0', order->chOrderType,order->chSide, (char*)order->ssSymbol.c_str(), orderticket, shoushu, order->dbSlPrice, order->dbStopPx, order->dbExecuteprice,rqty, diff);
        return 1;
    }
    void trade_request_moneyhold_rsp(char * json)
    {
        tradeuievents.trade_ui_moneyhold_rsp(json);
//        cJSON *root;
//        cJSON *retcode;
//        cJSON *retinfo;
//        printBusLogEx("trade_request_funds_rsp json:%s",json);
//        root=cJSON_Parse(json);
//        int nRet = -1;
//        if (root){
//            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
//            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
//            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
////                get_funds_from_json(root);
//                tradeuievents.trade_ui_moneyhold_rsp(json);
//            } else if (NULL != retcode){
//                tradeuievents.trade_ui_moneyhold_rsp(json);
//            }
//        }
//        cJSON_Delete(root);
//        printBusLogEx("trade_request_funds_rsp end");
    }
    
    int trade_request_funds()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_funds start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_funds json:%s", out);
        int nSeq = nhp_trade_funds(out);
        free(out);
        printBusLogEx("trade_request_funds end");
        return nSeq;
    }
    
    int trade_query_inout_monery(){
        cJSON *root= getcJsonRoot();
        char *out;
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_query_inout_monery json:%s", out);
        int nSeq = nhp_query_trade_inout_monery(out);
        free(out);
        printBusLogEx("trade_query_inout_monery end");
        return nSeq;
    }
    
    int trade_current_monery_flowing() {
        cJSON *root;
        root=cJSON_CreateObject();
        char *out;
        char sstmp[64];
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", get_str_user());
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, sstmp);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_current_monery_flowing json:%s", out);
        int nSeq = nhp_trade_current_monery_flowing(out);
        free(out);
        printBusLogEx("trade_current_monery_flowing end");
        return nSeq;
    }
    
    int trade_monery_flowing(const char* start, const char* end) {
        if(NULL == start || NULL == end) return CHECK_FAIL;
        cJSON *root;
        
        root=cJSON_CreateObject();
        
        char *out;
        char sstmp[64];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", get_str_user());
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, sstmp);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", start);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDSTARTDATE, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", end);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDENDDATE, sstmp);
        
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_monery_flowing json:%s", out);
        int nSeq = nhp_trade_monery_flowing(out);
        free(out);
        printBusLogEx("trade_monery_flowing end");
        return nSeq;
    }
    
    
    void handler_inout_monery(int nSeq, const char * json) {
        cJSON *root;
//        printBusLogEx("handler_inout_monery json:%s",json);
        int nRet=-1;
        root=cJSON_Parse(json);
        if (root){
//            clearMoneryInOutContainer(&moneyInOutContainer);
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                
//                memset(inoutmoneryids, 0, sizeof(inoutmoneryids));
                string key = "INTPRTCL";
                cJSON * arrays = cJSON_GetObjectItem(root, key.c_str());
                if(NULL != arrays) {
                    tradeuievents.trade_ui_transferquery_rsp(nRet,json);
                    
                    
                    
//                    int count = cJSON_GetArraySize(arrays);
//                    for (int i = 0; i < count; i++) {
//                        cJSON * cjson = cJSON_GetArrayItem(arrays, i);
//                        CMoneryInOutRes* res = new CMoneryInOutRes;
//                        convert_monery_inout_info_from_jsonnode(cjson, (const char*)res);
                    
//                        moneyInOutContainer.insert(make_pair(i, res)); //crash code
//                        char tmp[10];
//                        memset(&tmp, 0, sizeof(tmp));
//                        sprintf(tmp, "%d", i);
//                        if(strlen(inoutmoneryids) > 0) strcat(inoutmoneryids, ",");
//                        strcat(inoutmoneryids, tmp);
//                        if(res->chOperatortype == MONEY_IN_OUT_STOP_CONTRACT && res->chExecutestatus == '1') {
//                            //                            SignStatus = 0;
//                        } else if(res->chOperatortype == MONEY_IN_OUT_CONTRACT && res->chExecutestatus == '1') {
//                            //                            SignStatus = 1;
//                        }
//                    }
                }else{
                    tradeuievents.trade_ui_transferquery_rsp(nRet,"");
                }
            }
        }
        cJSON_Delete(root);
        printBusLogEx("SignStatus=[%d]", SignStatus);
        printBusLogEx("handler_inout_monery end");
    }
    const char * getInOutMoneryids() {
        return inoutmoneryids;
    }
    
    const char * getInOutMoneryApplyTime(long long ids) {                //请求时间
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->ssApplyTime.c_str();
        return NULL;
    }
    const char * getInOutMoneryAuditTime(long long ids) {                 //审核时间
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->ssAuditTime.c_str();
        return NULL;
    }
    const char * getInOutMoneryRemark(long long ids) {                   //备注
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->ssRemark.c_str();
        return NULL;
    }
    char getInOutMoneryExecutestatus(long long ids) {              //执行状态0=未处理,1=已处理,2=作废',
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->chExecutestatus;
        return '\0';
    }
    long long getInOutMoneryId(long long ids) {                     //ID
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->iId;
        return 0;
    }
    char getInOutMoneryOperatortype(long long ids) {                //操作类型
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->chOperatortype;
        return '\0';
    }
    double getInOutMoneryAmount(long long ids) {					//入出金金额
        map<long long, CMoneryInOutRes*>::iterator it = moneyInOutContainer.find(ids);
        if(it != moneyInOutContainer.end()) return it->second->dbAmount;
        return 0.0;
    }
    
    void handler_funds_flowing(int nSeq,const char * json) {
        cJSON *root;
        printBusLogEx("handler_funds_flowing json:%s",json);
        int nRet=-1;
        root=cJSON_Parse(json);
        if (root){
            clearFundsFlowContainer(&fundsFlowContainer);
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                memset(fundsFlowids, 0, sizeof(fundsFlowids));
                string key = "ASSOCIATETICKET";
                cJSON * arrays = cJSON_GetObjectItem(root, key.c_str());
                if(NULL != arrays) {
                    int count = cJSON_GetArraySize(arrays);
                    for (int i = 0; i < count; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(arrays, i);
                        CFundsFlowRes* res = new CFundsFlowRes;
                        convert_funds_flow_info_from_jsonnode(cjson, (const char*)res);
                        fundsFlowContainer.insert(make_pair(i, res));
                        char tmp[10];
                        memset(&tmp, 0, sizeof(tmp));
                        sprintf(tmp, "%d", i);
                        if(strlen(fundsFlowids) > 0) strcat(fundsFlowids, ",");
                        strcat(fundsFlowids, tmp);
                    }
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.call_funds_flowing(nSeq, nRet);
        printBusLogEx("handler_funds_flowing end");
    }
    
    
    const char * getFundFlowTickets() {
        return fundsFlowids;
    }
    const char * getFundFlowAccountAlias(long long ids){                 //请求时间
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->ssAccountAlias.c_str();
        return NULL;
    }
    const char * getFundFlowExecuteTime(long long ids) {                 //审核时间
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->ssExecuteTime.c_str();
        return NULL;
    }
    long long getFundFlowAccountId(long long ids){                      //ID
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->iAccountId;
        return 0;
    }
    char getFundFlowOperatortype(long long ids) {                //操作类型
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->chOperatortype;
        return '\0';
    }
    double getFundFlowChangeFunds(long long ids){//入出金金额
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->dbChangeFunds;
        return 0.0;
    }
    double getFundFlowBalance(long long ids){
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->dbBalance;
        return 0.0;
    }
    long long getFundFlowAssociateticket(long long ids) {
        map<long long, CFundsFlowRes*>::iterator it = fundsFlowContainer.find(ids);
        if(it != fundsFlowContainer.end()) return it->second->iAssociateticket;
        return 0.0;
    }
    
    
    void trade_request_order_rsp(char * json)
    {
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        printBusLogEx("trade_request_order_rsp json:%s",json);
        int nRet=-1;
        root=cJSON_Parse(json);
        if (root){
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                get_ordertickets_from_json(root);
                updateFPL();
                tradeuievents.trade_ui_holdorder_rsp(TRADE_MSGTYPE_RES_OK, holdorderStrCodes, limitorderStrCodes, closeorderStrCodes);
            }
        }
        cJSON_Delete(root);
        printBusLogEx("trade_request_order_rsp end");
    }
    
    int trade_request_order()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_order start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_order json:%s", out);
        int nSeq = nhp_trade_order(out);
        free(out);
        printBusLogEx("trade_request_order end");
        return nSeq;
    }
    
    int trade_request_zyj_hisorder(const char * startdate, const char * enddate)
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_zyj_trade_hisorder start");
        root=cJSON_CreateObject();
        
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, get_str_user());
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDSTARTDATE, startdate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDENDDATE, enddate);
        
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_zyj_hisorder json:%s", out);
        int nSeq = nhp_zyj_trade_hisorder(out);
        free(out);
        printBusLogEx("trade_request_zyj_hisorder end");
        nNeedClearHisOrders = 1;
        return nSeq;
    }
    
    int old_histroy_seq = -1;
    void trade_request_hisorder_rsp(int nSeq, char * json) {
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        cJSON *qzretcode;
        cJSON *orders;
        printBusLogEx("trade_request_hisorder_rsp json:%s",json);
        root=cJSON_Parse(json);
        int nRet=-1;
        int tt = 0;
        if (root){
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            qzretcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE_JSON);
            if (qzretcode == NULL) {
                if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                    string orderkey = "ORDERTICKET";
                    orders = cJSON_GetObjectItem(root, orderkey.c_str());
                    if(NULL != orders){
                        if(nSeq == 0 || old_histroy_seq != nSeq) {
                            clearordercontainer(&hisorderContainer);
                            memset(hisorderStrCodes, 0, sizeof(hisorderStrCodes));
                        }
                        int ordercount = cJSON_GetArraySize(orders);
                        vector<long long> __tmp;
                        for (int i = 0; i < ordercount; i++) {
                            cJSON * order = cJSON_GetArrayItem(orders, i);
                            CFindOrderRes *findOrder = new CFindOrderRes;
                            convert_order_from_jsonnode(ZS_TYPE, order, (char*)findOrder);
                            hisorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                            __tmp.push_back(findOrder->iOrderticket);
                            
                        }
                        std::sort(__tmp.begin(), __tmp.end(), &__OrderIdCompare);
                        for (int i = 0; i < __tmp.size(); i++){
                            char tmp[32];
                            memset(tmp, 0, sizeof(tmp));
                            if(strlen(hisorderStrCodes) > 0) sprintf(tmp, ",%lld", __tmp[i]);
                            else sprintf(tmp, "%lld",  __tmp[i]);
                            strcat(hisorderStrCodes, tmp);
                        }
                    }
                    orderkey = "CAUBARGAIN";
                    orders = cJSON_GetObjectItem(root, orderkey.c_str());
                    if(NULL != orders){
                        tt = 1;
                        if(nSeq == 0 || old_histroy_seq != nSeq) {
                            clearordercontainer(&hiszyjOrderContainer);
                            memset(hiszyjorderStrCodes, 0, sizeof(hiszyjorderStrCodes));
                        }
                        int ordercount = cJSON_GetArraySize(orders);
                        for (int i = 0; i < ordercount; i++) {
                            long long id  = i + 1;
                            cJSON * order = cJSON_GetArrayItem(orders, i);
                            CFindOrderRes *findOrder = new CFindOrderRes;
                            convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                            hiszyjOrderContainer.insert(make_pair(id, findOrder));
                            char tmp[32];
                            memset(tmp, 0, sizeof(tmp));
                            if(strlen(hiszyjorderStrCodes) > 0) sprintf(tmp, ",%lld", id);
                            else sprintf(tmp, "%lld", id);
                            strcat(hiszyjorderStrCodes, tmp);
                        }
                    }
                }
            }
        }
        cJSON_Delete(root);
        old_histroy_seq = nSeq;
        if(tt == 1) tradeuievents.trade_ui_hisorder_rsp(nSeq, nRet, hiszyjorderStrCodes);
        else tradeuievents.trade_ui_hisorder_rsp(nSeq, nRet, hisorderStrCodes);
        printBusLogEx("trade_request_hisorder_rsp end");
    }
    
    int trade_request_hisorder( char * startdate, char * enddate)
    {
        cJSON *root;
        char *out;
        
        printBusLogEx("trade_request_hisorder start");
        
        root=cJSON_CreateObject();
        
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, get_str_user());
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDSTARTDATE, startdate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FINDENDDATE, enddate);
        
        out=cJSON_Print(root);
        
        cJSON_Delete(root);
        
        printBusLogEx("trade_request_hisorder json:%s", out);
        
        int nSeq = nhp_trade_hisorder(out);
        
        free(out);
        
        printBusLogEx("trade_request_hisorder end");
        
        nNeedClearHisOrders = 1;
        return nSeq;
    }
    
    void trade_request_symbol_rsp(char * json)
    {
        cJSON *root;
        
        cJSON *retcode;
        cJSON *retinfo;
        cJSON *symbols;
        //printBusLogEx("trade_request_symbol_rsp1 json:%s",json);
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root){
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                
                string symbolskey = "SYMBOLS";
                symbols = cJSON_GetObjectItem(root, symbolskey.c_str());
                if(NULL == symbols) return;
                int symbolcount = cJSON_GetArraySize(symbols);
                
                for (int i = 0; i < symbolcount; i++) {
                    cJSON * symbol = cJSON_GetArrayItem(symbols, i);
                    string symbolcode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_SECURITYID)->valuestring;
                    if (symbolcode.length() < 1) {
                        continue;
                    }
                    
                    SymbolContainer::iterator it = symbolContainer.find(symbolcode);
                    CSymbolParam * sysbolParam;
                    if (it != symbolContainer.end()) {
                        sysbolParam = (CSymbolParam*)(*it).second;
                    } else{
                        sysbolParam = new CSymbolParam;
                        sysbolParam->ssSymbolcode = symbolcode;
                        symbolContainer.insert(make_pair(symbolcode, sysbolParam));
                    }
                    convert_symbol_from_jsonnode(ZS_TYPE, (char*)sysbolParam, symbol);
                }
                
                
                resetsymbolcodes();  //android
                //                memset(symbolStrCodes, 0, sizeof(symbolStrCodes));
                //                sprintf(symbolStrCodes, json);
                strcpy(symbolStrCodes, json);
                tradeuievents.trade_ui_symbol_change_notice(TRADE_MSGTYPE_RES_OK, symbolStrCodes);
            }
        }
        cJSON_Delete(root);
        printBusLogEx("初始化登陆:step:[%d] 请求商品列表应答",nNowProgress);
    }
    
    
    
    
    
    
    void trade_kick_notify()
    {
        tradeuievents.trade_ui_kick_notify();
    }
    
    void quote_change_end(int count, char * symbolcodes) {
        if (USERLOGINED == 1) {
            tradeuievents.trade_ui_holdorder_rsp(TRADE_MSGTYPE_RES_OK, getHoldOrderAllTickets(), NULL, NULL);
            tradeuievents.trade_ui_openorder_stat_rsp(TRADE_MSGTYPE_RES_OK, symbolcodes);
            tradeuievents.trade_ui_symbol_change_notice(TRADE_MSGTYPE_RES_OK, symbolcodes);
//            tradeuievents.trade_ui_moneyhold_rsp(TRADE_MSGTYPE_RES_OK);
        }
    }
    
    void convertToTime(uint64_t serverTime, int* pDate, int* pTime)
    {
        if(pTime && pDate)
        {
            //东八区
            const time_t sTime = (const time_t)serverTime;
            struct tm *p = localtime(&sTime);
            int time = p->tm_sec + p->tm_min * 100 + p->tm_hour * 10000;
            int date = p->tm_mday + (p->tm_mon + 1) * 100 + (p->tm_year + 1900) * 10000;
            *pTime = time;
            *pDate = date;
        }
    }
    
    int get_symbol_data_min_begin_end_time(char* symbolCode, int* pBeginTime, int* pTotalTime) {
        if(m_vecSymbolInfo.empty() || NULL == pBeginTime || NULL == pTotalTime)  return 0;
        SymbolInfoArray::iterator element;
        for(element = m_vecSymbolInfo.begin(); element != m_vecSymbolInfo.end(); element ++) {
            CStuSymbolData oData = *element;
            if(0 == strcmp(oData.sCode.szCode, symbolCode)) {
                *pBeginTime = oData.nBeginTime / 60 * 100 + oData.nBeginTime % 60;
                *pTotalTime = oData.nTotalTradeTime;
                return 1;
            }
        }
        return 0;
    }
    
    
    
    //    double getlastPrice(char* symbolCode)
    //    {
    //        if(symbolContainer.empty())
    //        {
    //            SymbolContainer::iterator iter = symbolContainer.find(symbolCode);
    //            if (iter != symbolContainer.end()) {
    //                CSymbolParam * symbol = iter->second;
    //                return symbol->fCurPrice;
    //            }
    //        }
    //        return 0.0;
    //    }
    
    unsigned int getCurDataTime(unsigned int date, unsigned int time)
    {
        return (date % 1000000) * 10000 + time % 10000;
    }
    
    int updateKLine(KLineDataGroup* pDesGroup, KLineDataGroup* pSrcGroup)
    {
        TKHisUnitArray::iterator element;
        int lastCompareRes = -1;
        for(element = pSrcGroup->m_arrKLineData.begin(); element != pSrcGroup->m_arrKLineData.end(); element ++)
        {
            int i = 0;
            CSHisKUnit oUnit1 = *element;
            TKHisUnitArray::iterator element2;
            for(element2 = pDesGroup->m_arrKLineData.begin(); element2 != pDesGroup->m_arrKLineData.end(); element2 ++, i ++)
            {
                CSHisKUnit oUnit2 = *element2;
                int nDesDateTime = getCurDataTime(oUnit2.uiDate, oUnit2.uiTime);
                int nSrcDateTime = getCurDataTime(oUnit1.uiDate, oUnit1.uiTime);
                if(nDesDateTime == nSrcDateTime)
                {
                    lastCompareRes = 0;
                    oUnit2.uiOpenPrice = oUnit1.uiOpenPrice;//开盘价
                    oUnit2.uiMaxPrice = oUnit1.uiMaxPrice;//高
                    oUnit2.uiMinPrice = oUnit1.uiMinPrice;//低
                    oUnit2.uiClosePrice = oUnit1.uiClosePrice;//收
                    break;
                }
                else if(nDesDateTime < nSrcDateTime)//当前值比添加值时间小 继续查找
                    lastCompareRes = -1;
                else if(nDesDateTime > nSrcDateTime)//当前值比添加值时间大 直接插入
                {
                    if(-1 == lastCompareRes)//有跳过一条数据插入这条数据
                    {
                        lastCompareRes = 0;
                        //对于最尾处 先括容量再插入需要
                        if(element == pSrcGroup->m_arrKLineData.end()) {
                            pDesGroup->m_arrKLineData.push_back(oUnit2);
                        }
                        pDesGroup->m_arrKLineData.insert(element2, oUnit1);
                        break;
                    }
                }
            }
            if(-1 == lastCompareRes)//一直比当前数组里值大 添加到最尾
            {
                pDesGroup->m_arrKLineData.push_back(oUnit1);
            }
        }
        return 1;
    }
    
    
    
    
    
    //获取K线条数
    int getSymbolKLineDataCount(const char* utfSymbol, char nType)
    {
        string strSymbol = utfSymbol;
        int type = getSymbolType((char*)utfSymbol);
        switch (type) {
            case ZS_TYPE: {
                TKLineDataContainer::iterator it = klineDataContainer.find(strSymbol);
                if(it != klineDataContainer.end()){
                    KLineDataGroupContainer subChild = it->second;
                    KLineDataGroupContainer::iterator gc_it = subChild.find(nType);
                    if(gc_it != subChild.end()){
                        return gc_it->second->m_arrKLineData.size();
                    }
                }
                break;
            }
            case ZYJ_TYPE: {
                ZYQKLineDataContainer::iterator it = zyqKLineDataContainer .find(strSymbol);
                if(it != zyqKLineDataContainer.end()){
                    ZYQKLineDataGroupContainer  subChild = it->second;
                    ZYQKLineDataGroupContainer::iterator gc_it = subChild.find(nType);
                    if(gc_it != subChild.end()){
                        return gc_it->second.m_arrKLineData.size();
                    }
                }
                break;
            }
            default:
                break;
        }
        
        return 0;
        //        int i = 0;
        //        string strSymbol = utfSymbol;
        //        TKLineDataContainer::iterator itorder;
        //        for (itorder = klineDataContainer.begin(); itorder !=klineDataContainer.end() ; itorder++) {
        //            string str = itorder->first;
        //            if(0 == str.compare(utfSymbol)) {
        //                KLineDataGroup* array = itorder->second;
        //                if(nType == array->m_nKineDataType)
        //                {
        //                    return array->m_arrKLineData.size();
        ////                TKHisUnitArray::iterator element;
        ////                for(element = array->m_arrKLineData.begin(); element != array->m_arrKLineData.end(); element ++, i ++)
        ////                {
        ////                    CSHisKUnit oUnit = *element;
        ////                    arr[i].uiDate = oUnit.uiDate;
        ////                    arr[i].uiTime = oUnit.uiTime;
        ////                    arr[i].uiOpenPrice = oUnit.uiOpenPrice;//开盘价
        ////                    arr[i].uiMaxPrice = oUnit.uiMaxPrice;//高
        ////                    arr[i].uiMinPrice = oUnit.uiMinPrice;//低
        ////                    arr[i].uiClosePrice = oUnit.uiClosePrice;//收
        ////                    myPrintf("data [%d], time [%d], o[%d], c[%d]\n", oUnit.uiDate, oUnit.uiTime, oUnit.uiOpenPrice, oUnit.uiClosePrice);
        ////                }
        ////                    break;
        //                }
        //            }
        //        }
        //        return i;
    }
    
    
    
    //获取做市商K线数据
    int getZYQSymbolKLineData(const char* utfSymbol, int nType, HistoryKData* histroy, int nMaxSize){
        int i = 0;
        string strSymbol = utfSymbol;
        ZYQKLineDataContainer::iterator it = zyqKLineDataContainer.find(strSymbol);
        if(it != zyqKLineDataContainer.end()){
            ZYQKLineDataGroupContainer subChild = it->second;
            ZYQKLineDataGroupContainer::iterator gc_it = subChild.find(nType);
            if(gc_it != subChild.end()){
                ZYQKLineDataGroup array = gc_it->second;
                double ema12 = 0.0,ema26 = 0.0,dea = 0.0;
                double dif_old = 0.0, dea_old=0.0;
                int size = array.m_arrKLineData.size();
                for(i = 0; i < size; i++)  {
                    if(i >= nMaxSize) break;
                    STUHisKUnit_ZYQ oUnit = array.m_arrKLineData[i];
                    histroy->data[i].date = oUnit.m_oTDateTime.m_iUpdateDataTime;
                    histroy->data[i].openPrice = oUnit.m_uiOpenPrice;//开盘价
                    histroy->data[i].maxPrice = oUnit.m_uiMaxPrice;//高
                    histroy->data[i].minPrice = oUnit.m_uiMinPrice;//低
                    histroy->data[i].closePrice = oUnit.m_uiClosePrice;//收
                    //                    printBusLogEx("open= [%d]  close= [%d]", histroy->data[i].openPrice, histroy->data[i].closePrice);
                    double dif = 0;
                    double macd = 0;
                    if(i == 0) {
                        ema12 = oUnit.m_uiClosePrice * 1.0;
                        ema26 = oUnit.m_uiClosePrice * 1.0;
                        histroy->max = oUnit.m_uiMaxPrice;
                        histroy->min = oUnit.m_uiMinPrice;
                    } else {
                        //ema12 = ema12 * 11.0 / 13.0 + oUnit.uiClosePrice * 2.0 / 13.0;
                        //ema26 = ema26 * 25.0 / 27.0 + oUnit.uiClosePrice * 2.0 / 27.0;
                        ema12 = ema12 * 0.8462 + oUnit.m_uiClosePrice * 0.1538;
                        ema26 = ema26 * 0.9259 + oUnit.m_uiClosePrice * 0.0741;
                        if(histroy->max < oUnit.m_uiMaxPrice) histroy->max = oUnit.m_uiMaxPrice;
                        if(histroy->min > oUnit.m_uiMinPrice) histroy->min = oUnit.m_uiMinPrice;
                    }
                    dif = ema12 - ema26;
                    //dea = dif_old * 8.0 / 10.0 + dif * 2.0 / 10.0;
                    dea = dea_old * 0.8 + dif * 0.2;
                    macd = 2.0 * (dif - dea);
                    dif_old = dif;
                    dea_old = dea;
                    histroy->data[i].mMACD[0] = (int)(macd * 100);
                    histroy->data[i].mMACD[1] = (int)(dif * 100);
                    histroy->data[i].mMACD[2] = (int)(dea * 100);
                    histroy->price = oUnit.m_uiClosePrice;
                }
            }
        }
        CCalcInd_MA oCalcMA;
        oCalcMA.CalcMA((histroy->data), i);
        return i;
    }
    //获取做市商K线数据
    int getZSSymbolKLineData(const char* utfSymbol, char nType, HistoryKData* histroy, int nMaxSize){
        int i = 0;
        string strSymbol = utfSymbol;
        TKLineDataContainer::iterator it = klineDataContainer.find(strSymbol);
        if(it != klineDataContainer.end()){
            KLineDataGroupContainer subChild = it->second;
            KLineDataGroupContainer::iterator gc_it = subChild.find(nType);
            if(gc_it != subChild.end()){
                KLineDataGroup* array = gc_it->second;
                TKHisUnitArray::iterator element;
                double ema12 = 0.0,ema26 = 0.0,dea = 0.0;
                double dif_old = 0.0, dea_old=0.0;
                for(element = array->m_arrKLineData.begin(); element != array->m_arrKLineData.end(); element ++, i ++)
                {
                    if(i >= nMaxSize) break;
                    CSHisKUnit oUnit = *element;
                    histroy->data[i].date = oUnit.uiDate;
                    histroy->data[i].time = oUnit.uiTime;
                    histroy->data[i].openPrice = oUnit.uiOpenPrice;//开盘价
                    histroy->data[i].maxPrice = oUnit.uiMaxPrice;//高
                    histroy->data[i].minPrice = oUnit.uiMinPrice;//低
                    histroy->data[i].closePrice = oUnit.uiClosePrice;//收
                    
                    double dif = 0;
                    double macd = 0;
                    if(i == 0) {
                        ema12 = oUnit.uiClosePrice * 1.0;
                        ema26 = oUnit.uiClosePrice * 1.0;
                        histroy->max = oUnit.uiMaxPrice;
                        histroy->min = oUnit.uiMinPrice;
                    } else {
                        ema12 = ema12 * 11.0 / 13.0 + oUnit.uiClosePrice * 2.0 / 13.0;
                        ema26 = ema26 * 25.0 / 27.0 + oUnit.uiClosePrice * 2.0 / 27.0;
                        if(histroy->max < oUnit.uiMaxPrice) histroy->max = oUnit.uiMaxPrice;
                        if(histroy->min > oUnit.uiMinPrice) histroy->min = oUnit.uiMinPrice;
                    }
                    dif = ema12 - ema26;
                    dea = dif_old * 8.0 / 10.0 + dif * 2.0 / 10.0;
                    macd = 2.0 * (dif - dea);
                    dif_old = dif;
                    dea_old = dea;
                    histroy->data[i].mMACD[0] = (int)(macd * 100);
                    histroy->data[i].mMACD[1] = (int)(dif * 100);
                    histroy->data[i].mMACD[2] = (int)(dea * 100);
                    histroy->price = oUnit.uiClosePrice;
                }
            }
        }
        CCalcInd_MA oCalcMA;
        oCalcMA.CalcMA((histroy->data), i);
        return i;
    }
    
    //获取K线部分   此方法过时
    int getSymbolKLineData(const char* utfSymbol, char nType, CSHisKUnitMA* arr, int nMaxSize)
    {
        int i = 0;
        string strSymbol = utfSymbol;
        TKLineDataContainer::iterator it = klineDataContainer.find(strSymbol);
        if(it != klineDataContainer.end()){
            KLineDataGroupContainer subChild = it->second;
            KLineDataGroupContainer::iterator gc_it = subChild.find(nType);
            if(gc_it != subChild.end()){
                KLineDataGroup* array = gc_it->second;
                TKHisUnitArray::iterator element;
                for(element = array->m_arrKLineData.begin(); element != array->m_arrKLineData.end(); element ++, i ++)
                {
                    CSHisKUnit oUnit = *element;
                    arr[i].uiDate = oUnit.uiDate;
                    arr[i].uiTime = oUnit.uiTime;
                    arr[i].uiOpenPrice = oUnit.uiOpenPrice;//开盘价
                    arr[i].uiMaxPrice = oUnit.uiMaxPrice;//高
                    arr[i].uiMinPrice = oUnit.uiMinPrice;//低
                    arr[i].uiClosePrice = oUnit.uiClosePrice;//收
                }
            }
        }
        CCalcInd_MA oCalcMA;
        oCalcMA.Calc(arr, i);
        return i;
        //        int i = 0;
        //        string strSymbol = utfSymbol;
        //        TKLineDataContainer::iterator itorder;
        //        for (itorder = klineDataContainer.begin(); itorder !=klineDataContainer.end() ; itorder++) {
        //            string str = itorder->first;
        //            if(0 == str.compare(utfSymbol)) {
        //                KLineDataGroup* array = itorder->second;
        //                TKHisUnitArray::iterator element;
        //                for(element = array->m_arrKLineData.begin(); element != array->m_arrKLineData.end(); element ++, i ++)
        //                {
        //                    CSHisKUnit oUnit = *element;
        //                    arr[i].uiDate = oUnit.uiDate;
        //                    arr[i].uiTime = oUnit.uiTime;
        //                    arr[i].uiOpenPrice = oUnit.uiOpenPrice;//开盘价
        //                    arr[i].uiMaxPrice = oUnit.uiMaxPrice;//高
        //                    arr[i].uiMinPrice = oUnit.uiMinPrice;//低
        //                    arr[i].uiClosePrice = oUnit.uiClosePrice;//收
        ////                    myPrintf("data [%d], time [%d], o[%d], c[%d]\n", oUnit.uiDate, oUnit.uiTime, oUnit.uiOpenPrice, oUnit.uiClosePrice);
        //                }
        //                break;
        //            }
        //        }
        //
        //        oCalcMA.Calc(arr, i);
        //        return i;
    }
    
    int get_symbol_num()
    {
        return (int)symbolContainer.size();
    }
    
    
    //行情登录
    void quote_login_rsp(int nRet){
        //清空行情数据
        if(nRet == 1) {
            recoveryQuoteMemory();
        }
        tradeuievents.quote_ui_login_rsp(nRet);
    }
    
    
    ////////////////////////////
    //获取用户属性
    //用户账号
    const char * getUserAccount()
    {
        return LoginedAccountID;
    }
    //用户状态
    int    getUserStatus()
    {
        return atoi( LoginedAccountStatus );
    }
    //用户名
    const char * getUserName()
    {
        return LoginedAccountName;
    }
    
    const char * getLoginUser() {
        return get_str_user();
    }
    
    //获取商品属性
    //获取所有商品编码
    const char * getAllSymbolCode()
    {
        return symbolStrCodes;
    }
    CSymbolParam * getSymbolPraram(char * symbolcode)
    {
        if(NULL == symbolcode) return NULL;
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        if (it != symbolContainer.end()) {
            return  it->second;
        }
        return NULL;
    }
    
    
    double getSymbolNewQty(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_uiNewQty/100.0;
        }
        return 0;
    }
    double getSymbolNewPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end() && symbol != NULL) {
            return it->second.stuRealPublicDataPush.m_uiNewPrice/ pow(10.0, (int)symbol->nDecimal);
        }
        return 0.0;
    }
    char getSymbolTurnoverDirec(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_chTurnoverDirec;
        }
        return '\0';
    }
    double getSymbolPriceRiseDown(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_iPriceRiseDown/100.0;
        }
        return 0.0;
    }
    double getSymbolPriceRiseDownRate(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_iPriceRiseDownRate/100.0;
        }
        return 0.0;
    }
    double getSymbolTotalTurnoverFunds(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_iTotalTurnoverFunds/100.0;
        }
        return 0.0;
    }
    double getSymbolOrderRate(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_uiOrderRate/100.0;
        }
        return 0.0;
    }
    double getSymbolQtyRate(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_uiQtyRate/100.0;
        }
        return 0.0;
    }
    double getSymbolTotalTurnoverQty(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuRealPublicDataPush.m_uiTotalTurnoverQty/100.0;
        }
        return 0.0;
    }
    double getSymbolBuyVolPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end() && symbol != NULL) {
            return it->second.stuNowDataVolPirces[0].m_buyVol.m_uiPrice / pow(10.0, (int)symbol->nDecimal);
        }
        return 0.0;
    }
    double getSymbolBuyVolQty(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuNowDataVolPirces[0].m_buyVol.m_uiQty;
        }
        return 0.0;
    }
    double getSymbolSellVolPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end() && symbol != NULL) {
            return it->second.stuNowDataVolPirces[0].m_sellVol.m_uiPrice / pow(10.0, (int)symbol->nDecimal);
        }
        return 0.0;
    }
    double getSymbolSellVolQty(char * symbolcode)
    {
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            return it->second.stuNowDataVolPirces[0].m_sellVol.m_uiQty / 100.0;
        }
        return 0.0;
    }
    int getNowDataVolPirce(char * symbolcode, STUNowDataVolPirce* stuNowDataVolPirce)
    {
        memset(stuNowDataVolPirce, 0, sizeof(STUNowDataVolPirce) * 5);
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            memcpy(stuNowDataVolPirce, it->second.stuNowDataVolPirces, sizeof(it->second.stuNowDataVolPirces));
            return 1;
        }
        return 0;
    }
    //2014.7.24 修正获取商品名称时返回代码
    //商品名
    const char * getSymbolname(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->ssSymbolname.c_str();
        }
        return NULL;
    }
    int getSymbolShowOrder(char * symbolcode){
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->iShowOrder;
        }
        return 0;
    }
    int getSymbolType(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->symbolType;
        }
        return 0;
    }
    int getSymbolStepSize(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->dbStepsize;
        }
        return 1;
    }
    void setYesterDayClosePrice(char* symbolCode, double price)
    {
        SymbolContainer::iterator iter = symbolContainer.find(symbolCode);
        SymbolContainer::iterator iter2 = symbolContainer.end();
        if (iter != iter2)
        {
            CSymbolParam * symbol = iter->second;
            symbol->fYesterdayClosePrice = price;
        }
    }
    
    //买价
    double getAskPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fAskPrice;
        }
        return 0.0;
    }
    //卖价
    double getBidPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fBidPrice;
        }
        return 0.0;
    }
    //行情时间
    long long getLastTick(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->lastTick;
        }
        return 0.0;
    }
    //最高
    double getHighPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fMaxPrice;
        }
        return 0.0;
    }
    //最低
    double getLowPrice(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fMinPrice;
        }
        return 0.0;
    }
    
    
    //昨收价
    double getYesterDayClosePrice(char* symbolCode)
    {
        //        SymbolContainer::iterator iter = symbolContainer.find(symbolCode);
        //        SymbolContainer::iterator iter2 = symbolContainer.end();
        //        if (iter != iter2) {
        //            CSymbolParam * symbol = iter->second;
        //            return symbol->fYesterdayClosePrice;
        //        }
        //        return 0.0;
        CSymbolParam * symbol = getSymbolPraram(symbolCode);
        if (symbol != NULL) {
            return symbol->fYesterdayClosePrice;
        }
        return 0.0;
    }
    
    //昨结价
    double getSettlPrice(char * symbolcode) {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fSettlPrice;
        }
        return 0.0;
    }
    
    //开盘价
    double getOpenPrice(char * symbolcode) {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fOpenPrice;
        }
        return 0.0;
    }
    
    //最新价
    double getLastPrice(char * symbolcode) {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->fCurPrice;
        }
        return 0.0;
    }
    
    //价格涨跌方向 -1跌 0平 1涨
    int getPriceDirection(char * symbolcode)
    {
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->nCurPriceDirection;
        }
        return 0;
    }
    //价格小数点位数
    int getPriceDecimal(char * symbolcode)
    {
        if (symbolcode == NULL) {
            return 0;
        }
        CSymbolParam * symbol = getSymbolPraram(symbolcode);
        if (symbol != NULL) {
            return symbol->nDecimal;
        }
        return 0;
    }
    //最新行情价格
    double getCurrPrice(char* symbolCode){
        if (symbolCode == NULL) {
            return 0;
        }
        CSymbolParam * symbol = getSymbolPraram(symbolCode);
        if (symbol != NULL) {
            return symbol->fCurPrice;
        }
        return 0;
    }
    //获取订单属性
    //汇总
    //获取持仓单商品集合
    const char * getHoldSymbolCodes()
    {
        OrderContainer::iterator it;
        memset(holderSymbolCodes, 0, CODESCACHESIZE);
        vector<string> tmpSymbolcodes;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            char tmp[32];
            memset(tmp, 0, sizeof(tmp));
            sprintf(tmp, "%s", order->ssSymbol.c_str());
            vector<string>::iterator result = find(tmpSymbolcodes.begin(), tmpSymbolcodes.end(), tmp);
            if (result == tmpSymbolcodes.end()) {
                tmpSymbolcodes.push_back(tmp);
                if(strlen(holderSymbolCodes) > 0) strcat(holderSymbolCodes, ",");
                strcat(holderSymbolCodes, tmp);
            }
            //            if(!strstr(holderSymbolCodes, tmp)) {
            //                if(strlen(holderSymbolCodes) > 0) strcat(holderSymbolCodes, ",");
            //                strcat(holderSymbolCodes, tmp);
            //            }
        }
        return holderSymbolCodes;
    }
    //获取持仓单商品名
    const char * getHoldSymbolName(char * symbolcode)
    {
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                return order->ssSymbol.c_str();
            }
        }
        return NULL;
    }
    //持仓商品的总买单量
    double getHoldTotalBuyNum(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_BUY) {
                sum += order->dbCumQty;
            }
        }
        return sum;
    }
    //持仓商品的总卖单量
    double getHoldTotalSellNum(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_SELL) {
                sum += order->dbCumQty;
            }
        }
        return sum;
    }
    //持仓商品的总利息
    double getHoldTotalSumInterest(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                sum += order->dbSumInterest;
            }
        }
        return sum;
    }
    //持仓商品的总佣金
    double getHoldTotalCommision(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                sum += order->dbCloseCommision + order->dbOpenCommision;
            }
        }
        return sum;
    }
    //持仓商品的总浮动盈亏
    double getHoldTotalFpl(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                sum += order->fFpl;
            }
        }
        return sum;
    }
    //获取商品对应的持仓单号
    const char * getHoldTicketsBySymbol(char * symbolcode)
    {
        memset(HoldSymbolTickets, 0, 1024);
        OrderContainer::iterator it;
        for (it = holdorderContainer.begin(); it != holdorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                char str[25];
                sprintf(str, "%lld", order->iOrderticket);
                strcat(HoldSymbolTickets, str);
                strcat(HoldSymbolTickets, ",");
            }
        }
        if (strlen(HoldSymbolTickets) > 0) {
            HoldSymbolTickets[strlen(HoldSymbolTickets) - 1] = '\0';
        }
        return HoldSymbolTickets;
    }
    //获取限价单商品集合
    const char * getLimitSymbolCodes()
    {
        OrderContainer::iterator it;
        memset(limitSymbolCodes, 0, CODESCACHESIZE);
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            char tmpcode[64];
            memset(tmpcode, 0, 64);
            strcat(tmpcode, order->ssSymbol.c_str());
            strcat(tmpcode, ",");
            if (!strstr(limitSymbolCodes, tmpcode)) {
                strcat(limitSymbolCodes, tmpcode);
            }
        }
        limitSymbolCodes[strlen(limitSymbolCodes) - 1] = '\0';
        return limitSymbolCodes;
    }
    //获取限价单商品名
    const char * getLimitSymbolName(char * symbolcode)
    {
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                return  order->ssSymbol.c_str();
            }
        }
        return NULL;
    }
    //限价商品的总买单量
    double getLimitTotalBuyNum(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_BUY) {
                sum += order->dbOrderQty;
            }
        }
        return sum;
    }
    //限价商品的总卖单量
    double getLimitTotalSellNum(char * symbolcode)
    {
        double sum = 0.0;
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_SELL) {
                sum += order->dbOrderQty;
            }
        }
        return sum;
    }
    //限价商品的平均买价
    double getLimitAverageBuyPrice(char * symbolcode)
    {
        double sumqty = 0.0;
        double sumprice = 0.0;
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_BUY) {
                sumqty += order->dbOrderQty;
                sumprice += order->dbOrderQty * order->dbPrice;
            }
        }
        if (sumqty > 0.0) {
            return sumprice / sumqty;
        }
        return 0.0;
    }
    //限价商品的平均卖价
    double getLimitAverageSellPrice(char * symbolcode)
    {
        double sumqty = 0.0;
        double sumprice = 0.0;
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0
                && order->chSide == DIRECTION_TYPE_SELL) {
                sumqty += order->dbOrderQty;
                sumprice += order->dbOrderQty * order->dbPrice;
            }
        }
        if (sumqty > 0.0) {
            return sumprice / sumqty;
        }
        return 0.0;
    }
    //获取商品对应的限价单号单号
    const char * getLimitTicketsBySymbol(char * symbolcode)
    {
        memset(LimitSymbolTickets, 0, 1024);
        OrderContainer::iterator it;
        for (it = limitorderContainer.begin(); it != limitorderContainer.end(); it++) {
            CFindOrderRes * order = it->second;
            if (strcmp(symbolcode, order->ssSymbol.c_str()) == 0) {
                char str[25];
                sprintf(str, "%lld", order->iOrderticket);
                strcat(LimitSymbolTickets, str);
                strcat(LimitSymbolTickets, ",");
            }
        }
        if (strlen(LimitSymbolTickets) > 0) {
            LimitSymbolTickets[strlen(LimitSymbolTickets) - 1] = '\0';
        }
        return LimitSymbolTickets;
    }
    
    //获取所有持仓单单号
    const char * getHoldOrderAllTickets()
    {
        return holdorderStrCodes;
    }
    //获取所有限价单单号
    const char * getLimitOrderAllTickets()
    {
        return limitorderStrCodes;
    }
    //获取所有平仓单单号
    const char * getCloseOrderAllTickets()
    {
        return closeorderStrCodes;
    }
    double getCloseorderAllPL()
    {
        double plsum = 0.0;
        OrderContainer::iterator itorder;
        for (itorder = closeorderContainer.begin(); itorder !=closeorderContainer.end() ; itorder++)
        {
            CFindOrderRes* hisorder = itorder->second;
            plsum += hisorder->dbPl;
        }
        return plsum;
    }
    //获取历史单据单号
    const char * getHisOrderAllTickets()
    {
        return hisorderStrCodes;
    }
    
    const char * getHisZYJOrderAllTickets(){
        return hiszyjorderStrCodes;
    }
    
    double getHisorderAllPL()
    {
        double plsum = 0.0;
        OrderContainer::iterator itorder;
        for (itorder = hisorderContainer.begin(); itorder !=hisorderContainer.end() ; itorder++) {
            CFindOrderRes* hisorder = itorder->second;
            plsum += hisorder->dbPl;
        }
        return plsum;
    }
    
    CFindOrderRes * getCFindOrderRes(char * oticket){
        if(NULL == oticket) return NULL;
        long long ticket = atoll(oticket);
        OrderContainer::iterator it = holdorderContainer.find(ticket);
        if (it != holdorderContainer.end()) {
            return it->second;
        }
        
        it = limitorderContainer.find(ticket);
        if (it != limitorderContainer.end()) {
            return it->second;
        }
        
        it = closeorderContainer.find(ticket);
        if (it != closeorderContainer.end()) {
            return it->second;
        }
        
        it = closezyjorderContainer.find(ticket);
        if (it != closezyjorderContainer.end()) {
            return it->second;
        }
        
        //做市商历史
        it = hisorderContainer.find(ticket);
        if (it != hisorderContainer.end()) {
            return it->second;
        }
        
        it = settleorderContainer.find(ticket);//交收单
        if (it != settleorderContainer.end()) {
            return it->second;
        }
        
        it = bargainorderContainer.find(ticket);//成交单
        if (it != bargainorderContainer.end()) {
            return it->second;
        }
        
        //中远期历史
        it = hiszyjOrderContainer.find(ticket);
        if (it != hiszyjOrderContainer.end()) {
            return it->second;
        }
        return NULL;
    }
    
    
    
    //属性
    char getOpenClose(char* ticket){
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chOpenClose;
        }
        return '\0';
    }
    
    char getIsClose(char* ticket){
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chIsClose;
        }
        return '\0';
    }
    
    //商品
    char getOrderType(char* ticket){
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chOrderType;
        }
        return '\0';
    }
    
    int getTradeOrderType(char* ticket){
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->orderType;
        }
        return 1;
    }
    
    const char * getSymbol(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->ssSymbol.c_str();
        }
        return NULL;
    }
    //订单时间(挂单开仓时间)
    const char * getOrderTime(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return  order->ssOrderTime.c_str() ;
        }
        return NULL;
    }
    //成交时间(挂单开仓时间)
    const char * getExecuteTime(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return  order->ssExecuteTime.c_str() ;
        }
        return NULL;
    }
    //子订单类型
    char getSubOrderType(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chSubOrderType;
        }
        return 0.0;
    }
    //效力类型
    char getExpirationType(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chExpiration;
        }
        return 0.0;
    }
    
    //效力时间
    const char* getExpiration(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->ssExpirationdate.c_str();
        }
        return NULL;
    }
    
    
    //订单价格(挂单开仓价格)
    double getOrderPrice(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbPrice;
        }
        return 0.0;
    }//orderprice	订单价格	double	10,4
    //订单数量(挂单手数)
    double getOrderQty(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbOrderQty;
        }
        return 0.0;
    }
    //止盈价格
    double getSlPrice(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbSlPrice;
        }
        return 0.0;
    }//止盈价格	double	10,4
    //止损价格
    double getStopPx(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbStopPx;
        }
        return 0.0;
    }//tpprice	止损价格	double	10,4
    //买卖方向
    char getSide(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chSide;
        }
        return 0;
    }//direction	买/卖	Char 	1
    
    char getOrderStatus(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chOrdStatus;
        }
        return 0;
    }
    
    //执行价格
    double getExecuteprice(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbExecuteprice;
        }
        return 0.0;
    }
    
    //执行价格	double	10,4	订单执行的价格
    //开仓 手数
    double getCumQty(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbCumQty;
        }
        return 0.0;
    }//executesize	成交数量	double	20,4
    //开仓 开仓时间
    const char * getTransactTime(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->ssTransactTime.c_str();
        }
        return NULL;
    }//executetime	完成时间	Char 	24	订单完成时间
    //开仓 浮动盈亏（接收行情变化通知后要更新界面）
    double getFPL(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->fFpl;
        }
        return 0.0;
    }
    double getMargin(char * ticket){
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbMargin;
        }
        return 0.0;
        
    }
    //利息
    double getSumInterest(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbSumInterest;
        }
        return 0.0;
    }//累计递延费	Double 	20,4
    
    //开仓价格
    double getOpenOrderPrice(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbOpenPrice;
        }
        return 0.0;
    }
    
    //佣金 (开仓手续费 + 平仓手续费)
    double getCommision(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbCloseCommision + order->dbOpenCommision;
        }
        return 0.0;
    }
    
    double getCloseCommision(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbCloseCommision;
        }
        return 0.0;
    }
    
    double getOpenCommision(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbOpenCommision;
        }
        return 0.0;
    }
    
    long long getOrderticket(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->iOrderticket;
        }
        return 0;
    }
    
    double getDeliveryMargin(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbDeliveryMargin;
        }
        return 0.0;
    }
    
    double getDeliveryPayment(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbDeliveryPayment;
        }
        return 0.0;
    }
    
    double getHoldQty(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbHolderSize;
        }
        return 0.0;
    }
    
    double getHoldPrice(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbHolderPrice;
        }
        return 0.0;
    }
    
    long long getTicketByBargainId(char * ticket) {
        OrderContainer::iterator it ;
        for(it=settleorderContainer.begin(); it != settleorderContainer.end(); ++it){
            if(it->second->iBargainId == atoll(ticket)) return it->second->iOrderticket;
        }
        return 0;
    }
    
    long long getBargainId(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->iBargainId;
        }
        return 0;
    }
    
    double getBargainCommision(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbBargainCommision;
        }
        return 0.0;
    }
    
    char getIsYesDay(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->chYesday;
        }
        return '\0';
    }
    
    //平仓单号
    const char * getCloseticket(char * ticket) {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->ssCloseticket.c_str();
        }
        return NULL;
    }
    //平仓价格
    double getCloseprice(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbCloseprice;
        }
        return 0.0;
    }
    //平仓数量
    double getCloseordersize(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbCloseordersize;
        }
        return 0.0;
    }
    //平仓时间
    const char * getClosetime(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return  order->ssClosetime.c_str() ;
        }
        return NULL;
    }
    
    //盈亏
    double getPL(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            return order->dbPl;
        }
        return 0.0;
    }
    
    int getTicketPriceDecimal(char * ticket)
    {
        CFindOrderRes * order = getCFindOrderRes(ticket);
        if (order != NULL) {
            CSymbolParam * symbol = getSymbolPraram((char*)order->ssSymbol.c_str());
            if (symbol != NULL) {
                return symbol->nDecimal;
            }
        }
        return 0;
    }
    
    //获取资金属性
    double getTotalClosePL(){
        return m_Funds.dbClosePL;
    }
    
    double getRisk(){
        return m_Funds.dbRisk;
    }
    
    int getRiskLevel(){
        return m_Funds.iLevel;
    }
    
    double getTmpValue()
    {
        return m_Funds.dbTmpValue;
    }
    //总浮动盈亏
    double getTotalFPL()
    {
        return m_Funds.dbFPL;
    }
    
    //可用保证金
    double getBalanceMargin()
    {
        return m_Funds.dbBalanceMargin + getTotalFPL();
    }
    
    //冻结保证金
    double getFreezeMargin(){
        return m_Funds.dbFreezeMargin;
    }
    
    //占用保证金
    double getUsedMargin()
    {
        return m_Funds.dbUsedMargin;
    }
    
    //余额 (可用 +　占用　＋ 冻结)
    double getBalance()
    {
        //return m_Funds.dbOpenbalance;//m_Funds.dbFreezeMargin + getBalanceMargin() + m_Funds.dbUsedMargin;
        //        return m_Funds.dbOpenbalance - m_Funds.dbOut + m_Funds.dbIn;
        return m_Funds.dbFreezeMargin + m_Funds.dbBalanceMargin + m_Funds.dbUsedMargin;
    }
    
    const char* getVersion(){
        return VERSION;
    }
    
    //分笔行情获取
    int nhp_send_quote_STK_trace_data_req(const char* szCode, int nszCodeSize) {
        return send_quote_STK_trace_data_req(szCode, nszCodeSize);
    }
    //分时行情获取
    int nhp_send_quote_STK_min_data_req(const char* szCode, int nszCodeSize){
        return send_quote_STK_min_data_req(szCode, nszCodeSize);
    }
    
    //k线行情
    int nhp_send_quote_his_data_req(const char* szCode, int nPeriodicType, int nNum, int nReqCount){
        //TODO: 后续需要增加判断是否继续请求
        if(nReqCount <= 0) {
            tradeuievents.on_quote_history_data_notify(0, nPeriodicType, (char*)szCode);
            return 0;
        }
        return send_quote_his_data_req(szCode, nPeriodicType, nNum, nReqCount);
    }
    
    /*****************************START*******************************/
    //2.0
    //获取用户权限
    const char* getJurisdiction(){
        return Jurisdiction;
    }
    
    //查竞价帐户商品信息请求
    int zyj_trade_request_symbol() {
        cJSON *root;
        char *out;
        printBusLogEx("zyj_trade_request_symbol start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("zyj_trade_request_symbol json:%s", out);
        int nSeq = zyj_nhp_trade_request_symbol(out);
        free(out);
        printBusLogEx("zyj_trade_request_symbol end");
        return nSeq;
    }
    
    //竟价商品
    int call_zyj_symbol_handler(int nSeq,const char * json) {
        printBusLogEx("callback_zyj_symbol_handler start json:%s",json);
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        root=cJSON_Parse(json);
        int nRet=-1;
        int ordercount = 0;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUDQRYSYMTICKET";
                cJSON * symbols = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != symbols) {
                    ordercount = cJSON_GetArraySize(symbols);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * symbol = cJSON_GetArrayItem(symbols, i);
                        string symbolcode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_SECURITYID)->valuestring;
                        if (symbolcode.length() < 1) {
                            continue;
                        }
                        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
                        CSymbolParam * sysbolParam = NULL;
                        if (it != symbolContainer.end()) {
                            sysbolParam = (CSymbolParam*)(*it).second;
                        } else{
                            sysbolParam = new CSymbolParam;
                            sysbolParam->ssSymbolcode = symbolcode;
                            symbolContainer.insert(make_pair(symbolcode, sysbolParam));
                        }
                        convert_symbol_from_jsonnode(ZYJ_TYPE, (char*)sysbolParam, symbol);
                    }
                }
                memset(symbolStrCodes, 0, sizeof(symbolStrCodes));
                sprintf(symbolStrCodes, json);
                resetsymbolcodes();
                tradeuievents.trade_ui_symbol_change_notice(TRADE_MSGTYPE_RES_OK, symbolStrCodes);
                if(!isUpdate_ZYQ_Report()) {
                    request_all_zyq__quote_pankou();
                }
            }
        }
        
        cJSON_Delete(root);
        printBusLogEx("callback_zyj_symbol_handler end");
        return ordercount;
    }
    
    //竞价委托单单请求
    int zyj_trade_delegate_order(){
        cJSON *root;
        char *out;
        printBusLogEx("zyj_trade_request_order start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("zyj_trade_request_order json:%s", out);
        int nSeq = zyj_nhp_trade_delegate_order(out);
        free(out);
        printBusLogEx("zyj_trade_request_order end");
        return nSeq;
    }
    
    //竞价委托单单请求
    void call_zyj_delegate_order_handler(int nSeq,const char * json){
        printBusLogEx("callback_zyj_delegate_order_handler start json:%s", json);
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUORDER";
                cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        if(findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER) {
                            delete findOrder;
                            findOrder = NULL;
                            continue;
                        }
                        limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                        limitordercodeContainer.push_back(findOrder->iOrderticket);
                    }
                }
                resetlimitordercodes();
            }
        }
        cJSON_Delete(root);
        printBusLogEx("callback_zyj_delegate_order_handler end");
    }
    
    void resetclosezyjorder() {
        memset(closezyjorderStrCodes, 0, sizeof(closezyjorderStrCodes));
        OrderContainer::iterator it;
        for(it = closezyjorderContainer.begin(); it != closezyjorderContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(closezyjorderStrCodes) > 0) sprintf(szTmp, ",%lld", it->second->iBargainId);
            else sprintf(szTmp, "%lld", it->second->iBargainId);
            strcat(closezyjorderStrCodes, szTmp);
        }
    }
    
    //竟价持仓单查询请求
    int zyj_trade_hold_order() {
        cJSON *root;
        char *out;
        printBusLogEx("zyj_trade_request_order start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("zyj_trade_request_order json:%s", out);
        int nSeq = zyj_nhp_trade_hold_order(out);
        free(out);
        printBusLogEx("zyj_trade_request_order end");
        return nSeq;
    }
    
    //竞价持仓单单响应
    void call_zyj_hold_order_handler(int nSeq, const char * json){
        printBusLogEx("callback_zyj_hold_order_handler start %d  %s", nSeq, json);
        cJSON *root;
        int nRet=-1;
        root=cJSON_Parse(json);
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUBARGAIN";
                cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        //if(findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                        if(findOrder->chIsClose != TRADE_TYPE_OPEN) {
                            closezyjorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                            continue;
                        }
                        holdorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                        holdordercodeContainer.push_back(findOrder->iBargainId);
                    }
                    resetholdordercodes();
                    resetclosezyjorder();
                    //更新浮动盈亏
                    updateFPL();
                    tradeuievents.trade_ui_holdorder_rsp(TRADE_MSGTYPE_RES_OK, holdorderStrCodes, settleorderStrCodes, bargainSymbolCodes);
                }
            }
        }
        cJSON_Delete(root);
        printBusLogEx("callback_zyj_hold_order_handler end");
    }
    
    //竟价交收单
    int zyj_trade_settle_order() {
        cJSON *root;
        char *out;
        printBusLogEx("zyj_trade_settle_order start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("zyj_trade_settle_order json:%s", out);
        int nSeq = zyj_nhp_trade_settle_order(out);
        free(out);
        printBusLogEx("zyj_trade_settle_order end");
        return nSeq;
    }
    
    //竞价交收单响应
    void call_zyj_settle_order_handler(int nSeq,const char * json){
        printBusLogEx("call_zyj_settle_order_handler start %d  %s", nSeq, json);
        cJSON *root;
        int nRet=-1;
        root=cJSON_Parse(json);
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CANG";
                cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        settleorderContainer.insert(make_pair(findOrder->iCounterPartExecuteTicket, findOrder));
                        settleordercodeContainer.push_back(findOrder->iCounterPartExecuteTicket);
                    }
                    resetsettleordercodes();
                }
            }
        }
        cJSON_Delete(root);
        printBusLogEx("call_zyj_settle_order_handler end");
    }
    
    //竞价成交单单请求
    int zyj_trade_deal_order(){
        printBusLogEx("zyj_trade_request_order start");
        cJSON *root;
        char *out;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("zyj_trade_request_order json:%s", out);
        int nSeq = zyj_nhp_trade_deal_order(out);
        free(out);
        printBusLogEx("zyj_trade_request_order end");
        return nSeq;
    }
    
    //竞价成交单单请求
    void call_zyj_deal_order_handler(int nSeq, const char * json) {
        printBusLogEx("callback_zyj_deal_order_handler start %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            //            cJSON *retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUPOSITION";
                cJSON * jsonArrays = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArrays) {
                    int ordercount = cJSON_GetArraySize(jsonArrays);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(jsonArrays, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        long long idx = findOrder->iOrderticket + findOrder->iBargainId;
                        bargainorderContainer.insert(make_pair(idx, findOrder));
                        bargainordercodeContainer.push_back(idx);
                    }
                    resetbargainordercodes();
                }
            }
        }
        cJSON_Delete(root);
        printBusLogEx("callback_zyj_deal_order_handler end");
    }
    
    //新加接口
    //中远期
    int trade_open_zyj_orderex(char trade_flag, char trade_type, char order_type, char sub_order_type, char direction, const char * symbolcode, long long orderticket, double shoushu, double price, char valdate_type, char * valdate) {
        //下单 2014.7.30 增加反手与偏离点差
        //        int trade_open_orderex(char trade_flag, char trade_type, char order_type, char sub_order_type, char direction, char * symbolcode, long long orderticket, double shoushu, double sl, double tp, double price, char valdate_type, char * valdate, double rqty, int diff)
        //        {
        //        if (trading == 1) {
        //            return CHECK_SUCCESS;
        //        }
        //        //        diff=-1;
        cJSON *root;
        char *out;
        char sstmp[32];
        string strspace;
        
        CSymbolParam * symbolParam = NULL;
        
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        
        if (it != symbolContainer.end()) symbolParam = it->second;
        
        if (symbolParam == NULL) return CHECK_NOSYMBOL;//无此商品编码
        
        if (shoushu < symbolParam->dbMinpersize) return CHECK_SIZETOOSMALL;//交易数量小于最低限制
        
        if (shoushu > symbolParam->dbMaxpersize) return CHECK_SIZETOOLARGE;//交易数量大于最大限制
        
        if(price > INT32_MAX) return CHECK_PRICE;
        
        //增加步长判断
        if(Rounding(shoushu, QTY_DECIMAL) > Rounding(symbolParam->dbMinpersize, QTY_DECIMAL)) {
            double nstep = Rounding(shoushu, QTY_DECIMAL) - Rounding(symbolParam->dbMinpersize, QTY_DECIMAL);
            if(nstep < symbolParam->dbStepsize) {
                return CHECK_STEP_SIZE;
            }
            int fm = (int)(Rounding(nstep, QTY_DECIMAL) * 100.0) %  (int)(Rounding(symbolParam->dbStepsize, QTY_DECIMAL) * 100.0);
            if(fm != 0){
                return CHECK_STEP_SIZE;
            }
        }
        
        printBusLogEx("trade_open_zyj_order start");
        
        strspace = "";
        root=getcJsonRoot();
        //
        //    FIX_TAG_BODY_ADVTYPE		// "下单标志"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", trade_flag);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ADVTYPE, sstmp);
        
        //    FIX_TAG_BODY_OPENCLOSE		//"下单类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", trade_type);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPENCLOSE, sstmp);
        
        //    FIX_TAG_BODY_ORDERID		//"订单号"
        if (orderticket > 0) {
            char str[25];
            sprintf(str, "%lld", orderticket);
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERID, str);
        } else{
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERID, strspace.c_str());
        }
        
        //    FIX_TAG_BODY_SYMBOL		//"商品"
        if (symbolcode != NULL) {
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, symbolcode);
        } else{
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, strspace.c_str());
        }
        
        //    FIX_TAG_BODY_ORDERQTY		//"手数"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%f", shoushu);
        
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERQTY, sstmp);
        
        //    FIX_TAG_BODY_ORDERPRICE	//"委托价格"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%f", price);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERPRICE, sstmp);
        
        
        //    FIX_TAG_BODY_ORDERTYPE		//"单据类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", order_type);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERTYPE, sstmp);
        
        //    FIX_TAG_BODY_SUBORDERTYPE	// "交易类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", sub_order_type);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SUBORDERTYPE, sstmp);
        
        //    FIX_TAG_BODY_FILLPOLLCY	//"归档"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FILL_POLLCY_TYPE_ALL_OR_NONE);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FILLPOLLCY, sstmp);
        
        //    FIX_TAG_BODY_VALIDATETYPE	// "效力类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", valdate_type);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_VALIDATETYPE, sstmp);
        
        
        //    FIX_TAG_BODY_ENDDATE		//"截止日期时间"
        memset(sstmp, 0, 32);
        if(NULL != valdate) sprintf(sstmp, "%s", valdate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ENDDATE, sstmp);
        
        
        //    FIX_TAG_BODY_SIDE			// "买卖方向"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", direction);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SIDE, sstmp);
        
        
        // FIX_TAG_BODY_OPERATORTYPE	// "操作类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", ORDER_OPERATOR_TYPE_NORMAL);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPERATORTYPE, sstmp);
        
        //FIX_TAG_BODY_OPERATORID	// "操作ID"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FIX_TAG_BODY_OPERATORID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPERATORID, sstmp);
        
        
        //FIX_TAG_BODY_PARTYID	// "对手ID"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FIX_TAG_BODY_PARTYID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_PARTYID, sstmp);
        
        //    FIX_TAG_BODY_TEXT			//"备注"
        memset(sstmp, 0, 32);
        //sprintf(sstmp, "%c", ORDER_OPERATOR_TYPE_NORMAL);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TEXT, sstmp);
        
        out=cJSON_Print(root);
        
        cJSON_Delete(root);
        
        printBusLogEx("trade_open_zyj_order json:%s", out);
        
        nhp_zyj_trade_open(out);
        
        free(out);
        
        printBusLogEx("trade_open_zyj_order end");
        
        //        trading = 1;
        
        return CHECK_SUCCESS;
    }
    
    
    //撤消中远期
    int trade_cancel_zyj_order(long long orderticket) {
        OrderContainer::iterator it = limitorderContainer.find(orderticket);
        CFindOrderRes * order = NULL;
        if (it != limitorderContainer.end()) {
            order = it->second;
        }
        
        printBusLogEx("trade_cancel_limit_order %lld limitorderContainersize%d limitorderStrCodes:%s", orderticket,
                      limitorderContainer.size(), limitorderStrCodes);
        if (order == NULL) {
            return CHECK_ORDERCODEERROR;
        }
        
        char trade_type='1';
        return trade_open_zyj_orderex(TRADE_FLAG_CANCEL, trade_type, order->orderType, order->chSubOrderType, order->chSide, order->ssSymbol.c_str(), orderticket, order->dbOrderQty, order->dbPrice, '\0',  NULL);
    }
    
    //委
    //    int trade_open_zyj_order(char trade_type, char order_type, char sub_order_type, char direction, const char * symbolcode, double shoushu, double price, char valdate_type, char * valdate) {
    //        return trade_open_zyj_orderex(TRADE_FLAG_NEW, trade_type, order_type, sub_order_type, direction, symbolcode, 0, shoushu, price, valdate_type,  valdate);
    //    }
    //委托开仓
    int trade_open_limit_zyj_order(char direction, const char * symbolcode, double shoushu, double price) {
        return trade_open_zyj_orderex(TRADE_FLAG_NEW, TRADE_TYPE_OPEN, ORDER_TYPE_LIMIT, '\0', direction, symbolcode, 0, shoushu, price, VALIDATE_TYPE_DAY,  NULL);
    }
    //平今仓
    int trade_close_limit_zyj_day_order(char direction, const char * symbolcode, double shoushu, double price) {
        return trade_open_zyj_orderex(TRADE_FLAG_NEW, TRADE_TYPE_CLOSE_TODAY, ORDER_TYPE_LIMIT, '\0', direction, symbolcode, 0, shoushu, price, VALIDATE_TYPE_DAY,  NULL);
    }
    //平昨仓
    int trade_close_limit_zyj_yesterday_order(char direction, const char * symbolcode, double shoushu, double price) {
        return trade_open_zyj_orderex(TRADE_FLAG_NEW, TRADE_TYPE_CLOSE_TESTERDAY, ORDER_TYPE_LIMIT, '\0', direction, symbolcode, 0, shoushu, price, VALIDATE_TYPE_DAY,  NULL);
    }
    
    //公告抬头查询
    int trade_request_declare_title(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        int nSeq = nhp_trade_request_declare_title(out);
        free(out);
        return nSeq;
    }
    
    //公告内容查询
    int trade_request_declare_content(int id){
        char *out;
        cJSON *root = getcJsonRoot();
        char cId[10];
        sprintf(cId, "%d", id);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DECLAREID, cId);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_declare_content %s\n", out);
        int nSeq = nhp_trade_request_declare_content(id, out);
        free(out);
        addSeq(nSeq, id, "");
        return nSeq;
    }
    
    //公告抬头响应
    void declare_title_rsp(int nSeq,const char * json){
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUTITLE";
                cJSON * sizeJson = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != sizeJson) {
                    int ordercount = cJSON_GetArraySize(sizeJson);
                    for(int i = 0; i < ordercount;i++){
                        CDeclareTitleDesc* tiles = new CDeclareTitleDesc;
                        cJSON * cjson = cJSON_GetArrayItem(sizeJson, i);
                        convert_declare_from_jsonnode(cjson, (char*)tiles);
                        declareDescContainer.insert(make_pair(tiles->nAnnouceID, tiles));
                    }
                }
            }
        }
        cJSON_Delete(root);
    }
    
    //公告内容响应
    void declare_content_rsp(int nSeq,const char * json){
        map<int, SeqMap>::iterator iter = seqContainer.find(nSeq);
        //需要加上周期值 如果当前请求数据量为0时，则接收无法判定了
        if (iter == seqContainer.end()) return;
        SeqMap map = iter->second;
        int id = map.cType;
        seqContainer.erase(iter);
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK){
                string orderkey = "CAUCONTENT";
                cJSON * sizeJson = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != sizeJson) {
                    int ordercount = cJSON_GetArraySize(sizeJson);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * cjson = cJSON_GetArrayItem(sizeJson, i);
                        cJSON* jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DECLARECONTENT);//公告时间
                        if(NULL != jNode) {
                            DeclareDescArrayContainer::iterator it = declareDescContainer.find(id);
                            if(it != declareDescContainer.end()) {
                                CDeclareTitleDesc*  cd =  it->second;
                                cd->szContent = jNode->valuestring;
                                tradeuievents.delcare_content_rsp(id, jNode->valuestring);
                            }
                        }
                    }
                }
            }
        }
        cJSON_Delete(root);
    }
    
    //公告推送
    void declare_notify(const char * json){
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK) {
                CDeclareTitleDesc* tiles = new CDeclareTitleDesc;
                convert_declare_from_jsonnode(root, (char*)tiles);
                declareDescContainer.insert(make_pair(tiles->nAnnouceID, tiles));
                tradeuievents.delcare_notify(tiles->nAnnouceID);
            }
        }
        cJSON_Delete(root);
    }
    
    /*****************************公告属性 START*******************************/
    int getDeclareAnnouceSize(){
        return (int)(declareDescContainer.size());
    }
    
    int __declareIdCompare(long long a1,long long a2){
        return a1>a2;
    }
    
    //公告ID
    const char* getDeclareAnnouceID(){
        memset(declareDescId, 0, sizeof(declareDescId));
        int i = 0;
        int size = (int)declareDescContainer.size();
        DeclareDescArrayContainer::iterator it;
        vector<int> ids;
        for (it = declareDescContainer.begin(); it != declareDescContainer.end(); it++, i++) {
            ids.push_back(it->second->nAnnouceID);
        }
        std::sort(ids.begin(), ids.end(), &__declareIdCompare);
        size_t len = ids.size();
        for (size_t i =0; i < len; i ++) {
            int id = ids[i];
            it = declareDescContainer.find(id);
            if(it != declareDescContainer.end()) {
                char tmpcode[10];
                memset(tmpcode, 0, sizeof(tmpcode));
                sprintf(tmpcode, "%d", it->second->nAnnouceID);
                if(size>0 && i<size-1)
                    strcat(tmpcode, ",");
                if (!strstr(declareDescId, tmpcode)) {
                    strcat(declareDescId, tmpcode);
                }
            }
        }
        return declareDescId;
    }
    
    long long getDeclareTime(int annouceID){
        DeclareDescArrayContainer::iterator it = declareDescContainer.find(annouceID);
        if(it != declareDescContainer.end()) {
            return it->second->tTime;
        }
        return 0;
    }
    
    const char* getDelcare(int annouceID){//公告发布者
        DeclareDescArrayContainer::iterator it = declareDescContainer.find(annouceID);
        if(it != declareDescContainer.end()) {
            return it->second->szDelcare.c_str();
        }
        return NULL;
    }
    
    const char* getDelcareTitle(int annouceID) {//公告标题
        DeclareDescArrayContainer::iterator it = declareDescContainer.find(annouceID);
        if(it != declareDescContainer.end()) {
            return it->second->szTitle.c_str();
        }
        return NULL;
    }
    
    const char* getDelcareContent(int annouceID) {//公告内容
        DeclareDescArrayContainer::iterator it = declareDescContainer.find(annouceID);
        if(it != declareDescContainer.end()) {
            if(strlen(it->second->szContent.c_str()) > 0) it->second->cReadStatus = '1';
            return it->second->szContent.c_str();
        }
        return NULL;
    }
    
    char getDelcareReadStatus(int annouceID) {//阅读状态 1已阅读 2 未阅读
        DeclareDescArrayContainer::iterator it = declareDescContainer.find(annouceID);
        if(it != declareDescContainer.end()) {
            return it->second->cReadStatus;
        }
        return '\0';
    }
    
    /*****************************END*******************************/
    
    /******************************修改帐户信息 START******************************/
    
    int trade_request_modify_passwd(const char *type, const char* oldpass, const char* newpass){
        char *out;
        cJSON *root = getcJsonRoot();
        
        char sstmp[10];
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", type);
        
        char hash[50];
        getSHA1(hash, (unsigned char*)oldpass, (u_int32_t)strlen(oldpass));
        const char* oPass = (char*)hash;
        
        char nhash[50];
        getSHA1(nhash, (unsigned char*)newpass, (u_int32_t)strlen(newpass));
        const char* nPass = (char*)nhash;
        
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_PWDTYPE, sstmp);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_CUSTOMPWD, oPass);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_NEWPSWD, nPass);
        out = cJSON_Print(root);
        int nSeq = nhp_trade_request_modify_passwd(out);
        printBusLogEx("modify_passwd = %s", out);
        cJSON_Delete(root);
        free(out);
        return nSeq;
    }
    
    void handler_modifypasswd_rsp(int nSeq, const char* json) {
        printBusLogEx("handler_modifypasswd_rsp %s\n==========================================", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if(NULL != retcode) nRet = atoi(retcode->valuestring);
        }
        cJSON_Delete(root);
        printBusLogEx("handler_modifypasswd_rsp end");
        tradeuievents.modify_passwd_rsp(nRet);
    }
    
    //TODO: 中远期单据返回
    void handler_open_zyj_order_rsp(int nSeq,const char * json){
        //CAUORDER 委托单 CAUPOSITION 持仓单  CAUBARGAIN成交单
        printBusLogEx("handler_open_zyj_order_rsp json=%s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        long long ticks = 0;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                //获取资金
                get_funds_from_json(root);
                string orderkey = "CAUORDER";//中远期委托单
                cJSON *orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    long long ticket = 0;
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        ticket = findOrder->iOrderticket;
                        if (findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                            findOrder->chOrdStatus != ORDER_STATUS_TURN_OVER &&
                            findOrder->chOrdStatus != ORDER_STATUS_CANCEL) {
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                            if(findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER){// || findOrder->chOrdStatus == ORDER_STATUS_PART_OVER) {
                                delete findOrder;
                                findOrder = NULL;
                                continue;
                            }
                            //                            findOrder->dbCumQty = findOrder->dbOrderQty;
                            limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                            limitordercodeContainer.push_back(findOrder->iOrderticket);
                        } else if(findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                                  findOrder->chOrdStatus==ORDER_STATUS_CANCEL){//如果限价单，取消时删除数据
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                            //                            findOrder->dbCumQty = findOrder->dbOrderQty;
                            limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                            limitordercodeContainer.push_back(findOrder->iOrderticket);
                        }  else if(findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                                   findOrder->chOrdStatus==ORDER_STATUS_TURN_OVER){//如果限价单，成交删除数据
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                        }
                    }
                    resetlimitordercodes();
                }
                
                orderkey = "CAUPOSITION";//中远期持仓单
                orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        map<long long, CFindOrderRes*>::iterator iter =  holdorderContainer.find(findOrder->iBargainId);
                        if (iter != holdorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            holdorderContainer.erase(iter);
                            delete pTmp;
                        }
                        vector<long long>::iterator l;
                        for(l = holdordercodeContainer.begin(); l != holdordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == findOrder->iBargainId) {
                                holdordercodeContainer.erase(l);
                                break;
                            }
                        }
                        
                        if(findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                            closezyjorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                            continue;
                        }
                        
                        if(findOrder->chIsClose == TRADE_TYPE_CLOSE||
                           findOrder->chIsClose == TRADE_TYPE_CLOSE_TODAY ||
                           findOrder->chIsClose == TRADE_TYPE_CLOSE_TESTERDAY ||
                           findOrder->chIsClose == TRADE_TYPE_ERASE) {
                            delete findOrder;
                            findOrder = NULL;
                            continue;
                        }
                        holdorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                        holdordercodeContainer.push_back(findOrder->iBargainId);
                    }
                    resetclosezyjorder();
                    resetholdordercodes();
                }
                
                orderkey = "CAUBARGAIN";//成交单
                orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        long long idx = findOrder->iOrderticket + findOrder->iBargainId;
                        map<long long, CFindOrderRes*>::iterator iter =  bargainorderContainer.find(idx);
                        if (iter != bargainorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            bargainorderContainer.erase(iter);
                            delete pTmp;
                        }
                        vector<long long>::iterator l;
                        for(l = bargainordercodeContainer.begin(); l != bargainordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == idx) {
                                bargainordercodeContainer.erase(l);
                                break;
                            }
                        }
                        bargainorderContainer.insert(make_pair(idx, findOrder));
                        bargainordercodeContainer.push_back(idx);
                    }
                    resetbargainordercodes();
                    tradeuievents.on_bargain_notify(nRet);
                }
            }
        }
        cJSON_Delete(root);
        updateFPL();
        tradeuievents.trade_ui_openorder_rsp(nSeq, nRet, ticks);
    }
    
    //提交 "申报交收" 请求
    int trade_apply_delivery_settleex(const char * symbolcode, char direction, double qty, const char * cangid, char iscancel){
        cJSON *root;
        char *out;
        char sstmp[32];
        string strspace;
        
        CSymbolParam * symbolParam = NULL;
        
        SymbolContainer::iterator it = symbolContainer.find(symbolcode);
        
        if (it != symbolContainer.end()) symbolParam = it->second;
        
        if (symbolParam == NULL) return CHECK_NOSYMBOL;//无此商品编码
        
        if(direction == DIRECTION_TYPE_SELL && NULL == cangid) return CHECK_PARAMS_FAIL;
        
        printBusLogEx("trade_apply_delivery_settleex start");
        
        strspace = "";
        root=getcJsonRoot();
        
        //    FIX_TAG_BODY_ADVTYPE		// "下单标志"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", TRADE_FLAG_NEW);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ADVTYPE, sstmp);
        
        //    FIX_TAG_BODY_OPENCLOSE		//"下单类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", TRADE_TYPE_DELIVERY);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPENCLOSE, sstmp);
        
        //    FIX_TAG_BODY_SYMBOL		//"商品"
        if (symbolcode != NULL) {
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, symbolcode);
        } else{
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, strspace.c_str());
        }
        
        //FIX_TAG_BODY_ORDERQTY		//"手数"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%f", qty);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERQTY, sstmp);
        
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", iscancel);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_CANCELORDER_TYPE, sstmp);
        
        //    FIX_TAG_BODY_FILLPOLLCY	//"归档"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FILL_POLLCY_TYPE_ALL_OR_NONE);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FILLPOLLCY, sstmp);
        
        //    FIX_TAG_BODY_SIDE			// "买卖方向"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", direction);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SIDE, sstmp);
        
        
        // FIX_TAG_BODY_OPERATORTYPE	// "操作类型"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", ORDER_OPERATOR_TYPE_NORMAL);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPERATORTYPE, sstmp);
        
        //FIX_TAG_BODY_OPERATORID	// "操作ID"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FIX_TAG_BODY_OPERATORID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPERATORID, sstmp);
        
        //FIX_TAG_BODY_PARTYID	// "对手ID"
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FIX_TAG_BODY_PARTYID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_PARTYID, sstmp);
        
        memset(sstmp, 0, 32);
        sprintf(sstmp, "%c", FIX_TAG_BODY_DELIVERYTYPE);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERYTYPE, sstmp);
        
        if(NULL != cangid) { //仓单
            memset(sstmp, 0, 32);
            sprintf(sstmp, "%s", cangid);
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_CANG_ID, sstmp);
        } else  {
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_CANG_ID, strspace.c_str());
        }
        out=cJSON_Print(root);
        int nSeq = nhp_apply_delivery_settle(out);
        cJSON_Delete(root);
        printBusLogEx("trade_apply_delivery_settleex end json:%s", out);
        free(out);
        return nSeq;
    }
    
    //提交买 "申报交收" 请求
    int trade_buy_apply_delivery_settle(const char * symbolcode, double qty, char iscancel){
        return trade_apply_delivery_settleex(symbolcode, DIRECTION_TYPE_BUY, qty, NULL, iscancel);
    }
    
    //提交卖 "申报交收" 请求
    int trade_sell_apply_delivery_settle(const char * symbolcode, double qty, const char* cangid, char iscancel){
        return trade_apply_delivery_settleex(symbolcode, DIRECTION_TYPE_SELL, qty, cangid, iscancel);
    }
    
    //TODO: 提交交收申报响应  交收申报通知
    void handler_apply_delivery_sellte_rsp(int nSeq, const char * json) {
        printBusLogEx("handler_apply_delivery_sellte_rsp %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                //获取资金
                get_funds_from_json(root);
                
                //如下为单据信息
                string orderkey = "CAUDY";//交收单
                cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        // TODO: 增到平仓单
                        //已配对
                        if(findOrder->chOrdStatus == CAU_DELIVERY_STATUS_MATCH) {
                            closezyjorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                            continue;
                        }
                        
                        //删除之前的单据
                        map<long long, CFindOrderRes*>::iterator iter =  settleorderContainer.find(findOrder->iCounterPartExecuteTicket);
                        if (iter != settleorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            settleorderContainer.erase(iter);
                            delete pTmp;
                        }
                        settleorderContainer.insert(make_pair(findOrder->iCounterPartExecuteTicket, findOrder));
                        
                        //删除之前的的单号
                        vector<long long>::iterator l;
                        for(l = settleordercodeContainer.begin(); l != settleordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == findOrder->iOrderticket) {
                                settleordercodeContainer.erase(l);
                                break;
                            }
                        }
                        settleordercodeContainer.push_back(findOrder->iCounterPartExecuteTicket);
                    }
                    resetsettleordercodes();
                    resetclosezyjorder();
                    tradeuievents.on_sellte_notify(nRet);
                }
                
                orderkey = "CAUDYORDER";//中远期委托单
                orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    long long ticket = 0;
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        ticket = findOrder->iOrderticket;
                        if (findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                            findOrder->chOrdStatus != ORDER_STATUS_TURN_OVER &&
                            findOrder->chOrdStatus != ORDER_STATUS_CANCEL) {
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                            if(findOrder->chOrdStatus == ORDER_STATUS_TURN_OVER){// || findOrder->chOrdStatus == ORDER_STATUS_PART_OVER) {
                                delete findOrder;
                                findOrder = NULL;
                                continue;
                            }
                            //findOrder->dbCumQty = findOrder->dbOrderQty;
                            limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                            limitordercodeContainer.push_back(findOrder->iOrderticket);
                        } else if(findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                                  findOrder->chOrdStatus==ORDER_STATUS_CANCEL){//如果限价单，取消时删除数据
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                            //findOrder->dbCumQty = findOrder->dbOrderQty;
                            limitorderContainer.insert(make_pair(findOrder->iOrderticket, findOrder));
                            limitordercodeContainer.push_back(findOrder->iOrderticket);
                        }  else if(findOrder->chOrderType == ORDER_TYPE_LIMIT &&
                                   findOrder->chOrdStatus==ORDER_STATUS_TURN_OVER){//如果限价单，成交删除数据
                            map<long long, CFindOrderRes*>::iterator iter =  limitorderContainer.find(findOrder->iOrderticket);
                            if (iter != limitorderContainer.end()) {
                                CFindOrderRes* pTmp = iter->second;
                                limitorderContainer.erase(iter);
                                delete pTmp;
                            }
                            vector<long long>::iterator l;
                            for(l = limitordercodeContainer.begin(); l != limitordercodeContainer.end(); l++) {
                                long long s = (long long)(*l);
                                if(s == findOrder->iOrderticket) {
                                    limitordercodeContainer.erase(l);
                                    break;
                                }
                            }
                        }
                    }
                }
                
                orderkey = "CAUDYPOSITION";//中远期持仓单
                orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        map<long long, CFindOrderRes*>::iterator iter =  holdorderContainer.find(findOrder->iBargainId);
                        if (iter != holdorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            holdorderContainer.erase(iter);
                            delete pTmp;
                        }
                        vector<long long>::iterator l;
                        for(l = holdordercodeContainer.begin(); l != holdordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == findOrder->iBargainId) {
                                holdordercodeContainer.erase(l);
                                break;
                            }
                        }
                        
                        // TODO: 已平仓增到平仓单
                        if(findOrder->chIsClose == TRADE_TYPE_CLOSE) {
                            closezyjorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                            continue;
                        }
                        if(findOrder->chIsClose == TRADE_TYPE_CLOSE||
                           findOrder->chIsClose == TRADE_TYPE_CLOSE_TODAY ||
                           findOrder->chIsClose == TRADE_TYPE_CLOSE_TESTERDAY ||
                           findOrder->chIsClose == TRADE_TYPE_ERASE) {
                            delete findOrder;
                            findOrder = NULL;
                            continue;
                        }
                        
                        holdorderContainer.insert(make_pair(findOrder->iBargainId, findOrder));
                        holdordercodeContainer.push_back(findOrder->iBargainId);
                    }
                    resetclosezyjorder();
                    resetholdordercodes();
                }
                
                orderkey = "CAUDYBARGAIN";//成交单
                orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        long long idx = findOrder->iOrderticket + findOrder->iBargainId;
                        map<long long, CFindOrderRes*>::iterator iter =  bargainorderContainer.find(idx);
                        if (iter != bargainorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            bargainorderContainer.erase(iter);
                            delete pTmp;
                        }
                        vector<long long>::iterator l;
                        for(l = bargainordercodeContainer.begin(); l != bargainordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == idx) {
                                bargainordercodeContainer.erase(l);
                                break;
                            }
                        }
                        bargainorderContainer.insert(make_pair(idx, findOrder));
                        bargainordercodeContainer.push_back(idx);
                    }
                    resetbargainordercodes();
                    tradeuievents.on_bargain_notify(nRet);
                }
                resetholdordercodes();
                resetlimitordercodes();
            }
        }
        cJSON_Delete(root);
        updateFPL();
        tradeuievents.on_apply_delivery_sellte_rsp(nSeq, nRet);
    }
    
    int getIsZS(){
        char ch;
        int iCount = 0;
        do{
            ch = Jurisdiction[iCount];
            if(ch == ACCESS_MAKE_MARKET || strlen(Jurisdiction) == 0) return 1;
            iCount += 1;
        } while (iCount<strlen(Jurisdiction));
        return 0;
    }
    
    int getIsZYJ() {
        char ch;
        int iCount = 0;
        do{
            ch = Jurisdiction[iCount];
            if(ch == ACCESS_LONG_TREM) return 1;
            iCount += 1;
        } while (iCount<strlen(Jurisdiction));
        return 0;
    }
    
    int getIsStore(){
        char ch;
        int iCount = 0;
        do{
            ch = Jurisdiction[iCount];
            if(ch == ACCESS_WAREHOUSE) return 1;
            iCount += 1;
        } while (iCount<strlen(Jurisdiction));
        return 0;
    }
    
    int getIsZLStore(){
        char ch;
        int iCount = 0;
        do{
            ch = Jurisdiction[iCount];
            if(ch == ACCESS_NEUTRAL_WAREHOUSE) return 1;
            iCount += 1;
        } while (iCount<strlen(Jurisdiction));
        return 0;
    }
    
    //获取所有今天持仓历史单据
    const char * getZYJToDayHistoryOrderAllTickets() {
        return closezyjorderStrCodes;
    }
    
    //获取所有交收单据
    const char * getSettleOrderAllTickets() {
        return settleorderStrCodes;
    }
    
    //获取成交单据
    const char * getBarginOrderAllTickets() {
        return bargainorderStrCodes;
    }
    
    long long getCounterPartExecuteTicket(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->iCounterPartExecuteTicket;
        return 0;
    }
    
    double getDeferredCompensation(char* ticket){
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->dbDeferredCompensation;
        return 0;
    }
    
    const char* getSellCangId(char* ticket){
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->ssSellCangId.c_str();
        return 0;
    }
    
    //买方货款
    double getBuyPayOut(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->dbBuyPayOut;
        return 0;
    }
    
    double getMatchWeight(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->dbMatchWeight;
        return 0;
    }
    
    const char* getMatchDate(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->ssMatchDate.c_str();
        return 0;
    }
    
    const char* getDelegateTime(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->ssDelegateTime.c_str();
        return 0;
    }
    
    long getDayStep(char* ticket) {
        CFindOrderRes* res = getCFindOrderRes(ticket);
        if(NULL != res) return res->iDayStep;
        return 0;
    }
    
    void resetApplyDeliveryIntention(){
        memset(deliveryInfo, 0, sizeof(deliveryInfo));
        DeliveryInfoArrayContainer::iterator it;
        for(it = deliveryInfoArrayContainer.begin(); it != deliveryInfoArrayContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(deliveryInfo) >0) sprintf(szTmp, ",%lld", it->second->iDeliveryid);
            else sprintf(szTmp, "%lld", it->second->iDeliveryid);
            strcat(deliveryInfo, szTmp);
        }
    }
    
    //查询提货意向
    int query_apply_delivery_intention() {
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("query_apply_delivery_intention = %s", out);
        int nSeq = nhp_query_apply_delivery_intention(out);
        free(out);
        return nSeq;
    }
    
    //处理提货意向
    void handler_query_apply_delivery_intention(int nSeq, const char* json) {
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CDeliveryInfo* info = new CDeliveryInfo;
                        convert_delivery_info_from_jsonnode(cjson, (char*)info);
                        deliveryInfoArrayContainer.insert(make_pair(info->iDeliveryid, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetApplyDeliveryIntention();
    }
    
    int trade_delivery_intentionex(int ismod, const char* szMathId,						     //配对ID
                                   double dbDeliveryweight,                       //提货重量;
                                   const char* szDeliveryDateBegin,                    //提货开始日期
                                   const char* szDeliveryDateEnd,                      //提货结束日期
                                   const char* szDeliverymen,                       //提货人
                                   const char* szDeliveryIdCode,                    //提货人身份证
                                   const char* szTelphone,                          //联系电话
                                   const char* szAddress,                           //联系地址
                                   const char* szZipCode,                           //邮政编码
                                   char  chIsInvoice,                            //发票
                                   const char* szInvoicetitle,                      //发票抬头
                                   const char* szInvCompayName,                     //公司名称
                                   const char* szInvCompayAddress,                  //公司地址
                                   const char* szInvCompayTelephone,                //公司电话
                                   const char* szInvBankaccount,                    //银行账号
                                   const char* szInvBankDeposit,                    //开户行
                                   const char* szTaxregistryNumber                  //税务登记号
    ) {
        if(!isValidShenFenzheng(szDeliveryIdCode)) return CHECK_PARAMS_FAIL;
        
        if(NULL == szMathId)  return CHECK_PARAMS_FAIL;
        
        if(NULL == szDeliveryIdCode) return CHECK_PARAMS_FAIL;
        
        if(dbDeliveryweight <= 0) return CHECK_PARAMS_FAIL;
        
        if(NULL == szTelphone || isPhone(szTelphone) != 0) return CHECK_PHONE_PARAMS_FAIL;
        
        if(NULL == szInvCompayTelephone || isPhone(szInvCompayTelephone) != 0) return CHECK_PHONE_PARAMS_FAIL;
        
        char *out;
        cJSON *root = getcJsonRoot();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_MATCH_ID, szMathId);
        
        string sspace;
        char szValue[10];
        memset(szValue, 0, sizeof(szValue));
        sprintf(szValue, "%.4f", dbDeliveryweight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_WEIGHT, szValue);
        
        if(NULL != szDeliveryDateBegin) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_DATE_BEGIN, szDeliveryDateBegin);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_DATE_BEGIN, sspace.c_str());
        
        if(NULL != szDeliveryDateEnd) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_DATE_END, szDeliveryDateEnd);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_DATE_END, sspace.c_str());
        
        if(NULL != szDeliverymen) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_MEN, szDeliverymen);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_MEN, sspace.c_str());
        
        if(NULL != szDeliveryIdCode) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IDCARD, szDeliveryIdCode);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IDCARD, sspace.c_str());
        
        if(NULL != szTelphone) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TEL, szTelphone);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TEL, sspace.c_str());
        
        if(NULL != szAddress) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ADDRESS, szAddress);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ADDRESS, sspace.c_str());
        
        if(NULL != szZipCode) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ZIPCODE, szZipCode);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ZIPCODE, sspace.c_str());
        
        memset(szValue, 0, sizeof(szValue));
        sprintf(szValue, "%c", chIsInvoice);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IS_INVOICE, szValue);
        
        if(NULL != szInvoicetitle) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_INVOICE_TITLE, szInvoicetitle);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_INVOICE_TITLE, sspace.c_str());
        
        if(NULL != szInvCompayName) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_NAME, szInvCompayName);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_NAME, sspace.c_str());
        
        if(NULL != szInvCompayAddress) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_ADDRESS, szInvCompayAddress);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_ADDRESS, sspace.c_str());
        
        if(NULL != szInvCompayTelephone) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_TEL, szInvCompayTelephone);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_COMPAY_TEL, sspace.c_str());
        
        if(NULL != szInvBankaccount) cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKCARDNO, szInvBankaccount);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKCARDNO, sspace.c_str());
        
        if(NULL != szInvBankDeposit) cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKNAME, szInvBankDeposit);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKNAME, sspace.c_str());
        
        if(NULL != szTaxregistryNumber) cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TAXREGISTRY_NUMBER, szTaxregistryNumber);
        else cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TAXREGISTRY_NUMBER, sspace.c_str());
        
        out=cJSON_Print(root);
        cJSON_Delete(root);
        
        int nSeq = 0;
        if(ismod == 1) {
            nSeq = nhp_mod_apply_delivery_intention(out);
            printBusLogEx("mod_apply_delivery_intention = %s", out);
        } else if(ismod == 0){
            nSeq = nhp_apply_delivery_intention(out);
            printBusLogEx("apply_delivery_intention = %s", out);
        }
        free(out);
        return nSeq;
    }
    
    int trade_apply_delivery_intention(const char* szMathId,						     //配对ID
                                       double dbDeliveryweight,                       //提货重量;
                                       const char* szDeliveryDateBegin,                    //提货开始日期
                                       const char* szDeliveryDateEnd,                      //提货结束日期
                                       const char* szDeliverymen,                       //提货人
                                       const char* szDeliveryIdCode,                    //提货人身份证
                                       const char* szTelphone,                          //联系电话
                                       const char* szAddress,                           //联系地址
                                       const char* szZipCode,                           //邮政编码
                                       char  chIsInvoice,                            //发票
                                       const char* szInvoicetitle,                      //发票抬头
                                       const char* szInvCompayName,                     //公司名称
                                       const char* szInvCompayAddress,                  //公司地址
                                       const char* szInvCompayTelephone,                //公司电话
                                       const char* szInvBankaccount,                    //银行账号
                                       const char* szInvBankDeposit,                    //开户行
                                       const char* szTaxregistryNumber                  //税务登记号
    ) {
        return trade_delivery_intentionex(0,  szMathId, dbDeliveryweight,szDeliveryDateBegin, szDeliveryDateEnd, szDeliverymen,  szDeliveryIdCode, szTelphone, szAddress,  szZipCode, chIsInvoice, szInvoicetitle, szInvCompayName, szInvCompayAddress, szInvCompayTelephone, szInvBankaccount, szInvBankDeposit,szTaxregistryNumber);
    }
    
    //重新提交
    int trade_submit_delivery_intention(const char* szMathId, double weight) {
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[32];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%.3f", weight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_WEIGHT, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", szMathId);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%c", CAU_WHD_OP_COMMIT);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_TYPE, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_submit_delivery_intention = %s", out);
        int nSeq = nhp_op_apply_delivery_intention(out);
        free(out);
        return nSeq;
    }
    
    //修改
    int mod_apply_delivery_intention(const char* szMathId,						     //配对ID
                                     double dbDeliveryweight,                       //提货重量;
                                     const char* szDeliveryDateBegin,                    //提货开始日期
                                     const char* szDeliveryDateEnd,                      //提货结束日期
                                     const char* szDeliverymen,                       //提货人
                                     const char* szDeliveryIdCode,                    //提货人身份证
                                     const char* szTelphone,                          //联系电话
                                     const char* szAddress,                           //联系地址
                                     const char* szZipCode,                           //邮政编码
                                     char  chIsInvoice,                            //发票
                                     const char* szInvoicetitle,                      //发票抬头
                                     const char* szInvCompayName,                     //公司名称
                                     const char* szInvCompayAddress,                  //公司地址
                                     const char* szInvCompayTelephone,                //公司电话
                                     const char* szInvBankaccount,                    //银行账号
                                     const char* szInvBankDeposit,                    //开户行
                                     const char* szTaxregistryNumber                  //税务登记号
    ) {
        return trade_delivery_intentionex(1,szMathId, dbDeliveryweight,szDeliveryDateBegin, szDeliveryDateEnd, szDeliverymen,  szDeliveryIdCode, szTelphone, szAddress,  szZipCode, chIsInvoice, szInvoicetitle, szInvCompayName, szInvCompayAddress, szInvCompayTelephone, szInvBankaccount, szInvBankDeposit,szTaxregistryNumber);
    }
    
    void handler_apply_delivery_intention(int nSeq, const char* json) {
        printBusLogEx("handler_apply_delivery_intention = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CDeliveryInfo* info = new CDeliveryInfo;
                        convert_delivery_info_from_jsonnode(cjson, (char*)info);
                        map<long long, CDeliveryInfo*>::iterator iter =  deliveryInfoArrayContainer.find(info->iDeliveryid);
                        if (iter != deliveryInfoArrayContainer.end()) {
                            CDeliveryInfo* pTmp = iter->second;
                            deliveryInfoArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        
                        deliveryInfoArrayContainer.insert(make_pair(info->iDeliveryid, info));
                    }
                    resetApplyDeliveryIntention();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.apply_delivery_intention_rsp(nSeq, nRet);
    }
    
    //////////////////////////////
    //请求“入库申请”请求
    int apply_in_store_orderex(const char* importid, const char* cid, const char* symbol, double aweight, double unit,  const char* idate, const char* mdate, const char* lotnumber, const char* package, const char* factory){
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[64];
        
        if(unit == 0.0) return CHECK_PARAMS_FAIL;
        int qty = (int)(aweight/unit);
        if(qty <= 0) return CHECK_PARAMS_FAIL;
        if(importid != NULL) {
            memset(sstmp, 0, sizeof(sstmp));// 1142 入库申请单ID
            sprintf(sstmp, "%s", importid);
            cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_APPLY_IMPORT_ID, sstmp);
        }
        
        memset(sstmp, 0, sizeof(sstmp));// 1114 仓库ID
        sprintf(sstmp, "%s", cid);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_WAREHOUSEID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));// 55  商品代码
        sprintf(sstmp, "%s", symbol);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//申请重量
        sprintf(sstmp, "%.3f", aweight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_APPLY_WEIGHT, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//单位重量
        sprintf(sstmp, "%.3f", unit);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_UNIT_WEIGHT, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//1117 个数
        sprintf(sstmp, "%d", qty);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_QUANTITY, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//入库日期
        sprintf(sstmp, "%s", idate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IMPORT_DATE, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//生产日期
        sprintf(sstmp, "%s", mdate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_MADE_DATE, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//批号
        sprintf(sstmp, "%s", lotnumber);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_LOT_NUMBER, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//打包方式
        sprintf(sstmp, "%s", package);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_PACKAGE, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));//生产厂商
        sprintf(sstmp, "%s", factory);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_FACTORY, sstmp);
        
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("apply_in_store_orderex = %s", out);
        
        int nSeq = 0;
        if(importid!=NULL) nSeq = nhp_mod_apply_in_store(out);
        else nSeq = nhp_submit_apply_in_store(out);
        free(out);
        return nSeq;
    }
    
    int trade_op_apply_in_store_order(const char * applyid, char op){
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[32];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", applyid);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%c", op);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_TYPE, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_op_apply_in_store = %s", out);
        int nSeq = nhp_op_apply_in_store(out);
        free(out);
        return nSeq;
    }
    
    //提交操作“入库申请”
    int trade_apply_in_store_order(const char * applyid){
        return trade_op_apply_in_store_order(applyid, CAU_WHD_OP_COMMIT);
    }
    
    //取消操作“入库申请”
    int trade_cancle_apply_in_store_order(const char * applyid){
        return trade_op_apply_in_store_order(applyid, CAU_WHD_OP_CANCEL);
    }
    
    //提交“入库申请”请求
    int submit_apply_in_store_order(const char* cid, const char* symbol, double aweight, double unit, const char* idate, const char* mdate, const char* lotnumber, const char* package, const char* factory){
        return apply_in_store_orderex(NULL, cid, symbol, aweight, unit, idate, mdate, lotnumber, package, factory);
    }
    
    //修改“入库申请”请求
    int mod_apply_in_store_order(const char* importid, const char* cid, const char* symbol, double aweight, double unit, const char* idate, const char* mdate, const char* lotnumber, const char* package, const char* factory){
        return apply_in_store_orderex(importid, cid, symbol, aweight, unit, idate, mdate, lotnumber, package, factory);
    }
    
    void resetApplyInStoreInfo(){
        memset(applyInStoreOrderInfo, 0, sizeof(applyInStoreOrderInfo));
        StoreOrderArrayContainer::iterator it;
        for(it = applyInStoreOrderArrayContainer.begin(); it != applyInStoreOrderArrayContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(applyInStoreOrderInfo) >0) sprintf(szTmp, ",%lld", it->second->iApplyid);
            else sprintf(szTmp, "%lld", it->second->iApplyid);
            strcat(applyInStoreOrderInfo, szTmp);
        }
    }
    
    //请求“入库申请”响应
    void handler_apply_in_store_rsp(int nSeq, const char* json){
        printBusLogEx("handler_apply_in_store_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  applyInStoreOrderArrayContainer.find(info->iApplyid);
                        if (iter != applyInStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            applyInStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        if(info->chApplystatus == '6') {
                            delete info;
                            info = NULL;
                            continue;
                        }
                        applyInStoreOrderArrayContainer.insert(make_pair(info->iApplyid, info));
                    }
                    resetApplyInStoreInfo();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.apply_in_store_rsp(nSeq, nRet);
    }
    
    //查询 “入库请求” 请求
    int query_apply_in_store(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("query_in_store_order = %s", out);
        int nSeq = nhp_query_apply_in_store(out);
        free(out);
        return nSeq;
    }
    
    //查询 “入库请求” 响应
    void handler_query_apply_in_store_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_apply_in_store_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        if(info->chApplystatus == IMPORT_APPLY_STATUS_COMPLETE) {
                            delete info;
                            info = NULL;
                            continue;
                        }
                        applyInStoreOrderArrayContainer.insert(make_pair(info->iApplyid, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetApplyInStoreInfo();
    }
    
    //查询”入库单”请求
    int query_in_store_order(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("query_in_store_order = %s", out);
        int nSeq = nhp_query_in_store_order(out);
        free(out);
        return nSeq;
    }
    
    void resetInStoreOrderInfo(){
        memset(inStoreOrderInfo, 0, sizeof(inStoreOrderInfo));
        StoreOrderArrayContainer::iterator it;
        for (it=inStoreOrderArrayContainer.begin(); it!=inStoreOrderArrayContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(inStoreOrderInfo) > 0) sprintf(szTmp, ",%d", it->second->iImportid);
            else sprintf(szTmp, "%d", it->second->iImportid);
            strcat(inStoreOrderInfo, szTmp);
        }
    }
    
    //查询”入库单”响应
    void handler_query_in_store_order_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_in_store_order_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        inStoreOrderArrayContainer.insert(make_pair(info->iImportid, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetInStoreOrderInfo();
    }
    
    //获取”商品属性”请求
    int query_in_store_goods_property(){
        //        char *out;
        //        cJSON *root = getcJsonRoot();
        //        out = cJSON_Print(root);
        //        cJSON_Delete(root);
        //        printBusLogEx("query_in_store_goods_property = %s", out);
        //        int nSeq = nhp_query_in_store_goods_property(out);
        //        free(out);
        //        return nSeq;
        return 0;
    }
    
    //获取”商品属性”响应
    void handler_query_in_store_goods_property_rsp(int nSeq, const char* json){
        //        printBusLogEx("handler_query_in_store_goods_property_rsp = %s", json);
        //        cJSON *root;
        //        root=cJSON_Parse(json);
        //        int nRet = -1;
        //        if (root) {
        //            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
        //            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
        //                string orderkey = "CANG";
        //                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
        //                if(NULL != jsonArray) {
        //                    int ordercount = cJSON_GetArraySize(jsonArray);
        //                    for (int i = 0; i < ordercount; i++) {
        //
        //                    }
        //                }
        //            }
        //        }
        //        cJSON_Delete(root);
    }
    
    void resetSelltStoreInfo(){
        memset(deliveryStoreInfo, 0, sizeof(deliveryStoreInfo));
        DeliveryStoreInfoArrayContainer::iterator it;
        for(it = deliveryStoreInfoArrayContainer.begin(); it != deliveryStoreInfoArrayContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(deliveryStoreInfo) > 0) sprintf(szTmp, ",%lld", it->second->iWareHouseId);
            else sprintf(szTmp, "%lld", it->second->iWareHouseId);
            strcat(deliveryStoreInfo, szTmp);
        }
    }
    
    //获取”交收仓库信息”请求
    int query_sellt_store(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("query_sellt_store = %s", out);
        int nSeq = nhp_query_sellt_store(out);
        free(out);
        return nSeq;
    }
    
    //获取”交收仓库信息”响应
    void handler_query_sellte_store_info_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_sellte_store_info_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        COutCAUDeliveryWarehouseInfo* info = new COutCAUDeliveryWarehouseInfo;
                        convert_delivery_store_info_from_jsonnode(cjson, (char*)info);
                        deliveryStoreInfoArrayContainer.insert(make_pair(info->iWareHouseId, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetSelltStoreInfo();
    }
    
    
    //查询仓单请求
    int trade_query_cang(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_query_cang = %s", out);
        int nSeq = nhp_query_cang_order(out);
        free(out);
        return nSeq;
    }
    
    //查询仓单响应
    void handler_query_cang_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_cang_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        //        memset(cangOrderInfo, 0, sizeof(cangOrderInfo));
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        cangOrderArrayContainer.insert(make_pair(info->iCangid, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetCangOrder();
    }
    
    //  TODO: 仓单转入库单
    //提交 “仓单转入库单”
    int trade_cang_to_instore_order(const char * ticket, double weight) {
        
        if(NULL == ticket) return CHECK_PARAMS_FAIL;
        
        if(weight <= 0) return CHECK_PARAMS_FAIL;
        
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[32];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%.3f", weight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_WEIGHT, sstmp);
        
        memset(sstmp, 0, sizeof(ticket));
        sprintf(sstmp, "%s", ticket);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_cang_to_instore_order = %s", out);
        int nSeq = nhp_cang_to_instore_order(out);
        free(out);
        return nSeq;
    }
    
    //处理仓单转入库单
    void handler_cang_to_instore_order_response(int nSeq, const char * json){
        printBusLogEx("handler_cang_to_instore_order_response %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  cangOrderArrayContainer.find(info->iCangid);
                        if (iter != cangOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            cangOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        cangOrderArrayContainer.insert(make_pair(info->iCangid, info));
                    }
                    resetCangOrder();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.op_cang_order_response(nSeq, nRet);
    }
    
    //操作 “仓单转入库单”
    int trade_op_cang_to_instore_order(const char * ticket, char opt) {
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[32];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", ticket);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%c", opt);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_TYPE, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_cang_to_instore_order = %s", out);
        int nSeq = nhp_op_cang_to_instore_order(out);
        free(out);
        return nSeq;
    }
    
    int trade_cancel_cang_to_instore_order(const char * ticket) {
        return trade_op_cang_to_instore_order(ticket, CAU_WHD_OP_CANCEL);
    }
    
    int trade_submit_cang_to_instore_order(const char * ticket) {
        return trade_op_cang_to_instore_order(ticket, CAU_WHD_OP_COMMIT);
    }
    
    void handler_op_cang_to_instore_order_response(int nSeq, const char * json){
        printBusLogEx("handler_op_cang_to_instore_order_response %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  cangToInStoreOrderArrayContainer.find(info->iCangToImportId);
                        if (iter != cangToInStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            cangToInStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        cangToInStoreOrderArrayContainer.insert(make_pair(info->iCangToImportId, info));
                    }
                    resetCangToInStoreOrder();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.op_cang_to_in_store_order_response(nSeq, nRet);
    }
    //仓单转入库单请求
    int trade_query_cang_to_instore_order(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_query_out_store_order = %s", out);
        int nSeq = nhp_query_cang_to_instore_order(out);
        free(out);
        return nSeq;
    }
    
    //仓单转入库单响应
    void handler_query_cang_to_in_store_order_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_cang_to_instore_order_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        memset(cangToInStoreOrderInfo, 0, sizeof(cangToInStoreOrderInfo));
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        cangToInStoreOrderArrayContainer.insert(make_pair(info->iCangToImportId, info));
                        //                        char szTmp[20];
                        //                        if(strlen(cangToInStoreOrderInfo) > 0) sprintf(szTmp, ",%lld", info->iCangToImportId);
                        //                        else sprintf(szTmp, "%lld", info->iCangToImportId);
                        //                        strcat(cangToInStoreOrderInfo, szTmp);
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetCangToInStoreOrder();
    }
    
    //查询”出库单”请求
    int trade_query_out_store_order(){
        char *out;
        cJSON *root = getcJsonRoot();
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_query_out_store_order = %s", out);
        int nSeq = nhp_query_out_store_order(out);
        free(out);
        return nSeq;
    }
    void resetExportStoreInfo(){
        memset(outStoreOrderInfo, 0, sizeof(outStoreOrderInfo));
        StoreOrderArrayContainer::iterator it;
        for (it=outStoreOrderArrayContainer.begin(); it!=outStoreOrderArrayContainer.end(); ++it) {
            char szTmp[20];
            if(strlen(outStoreOrderInfo) > 0) sprintf(szTmp, ",%lld", it->second->iExportid);
            else sprintf(szTmp, "%lld", it->second->iExportid);
            strcat(outStoreOrderInfo, szTmp);
        }
    }
    
    //查询”出库单”响应
    void handler_query_out_store_order_rsp(int nSeq, const char* json){
        printBusLogEx("handler_query_out_store_order_rsp = %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        outStoreOrderArrayContainer.insert(make_pair(info->iExportid, info));
                    }
                }
            }
        }
        cJSON_Delete(root);
        resetExportStoreInfo();
    }
    
    const char* getDeliveryStoreIds(){
        return deliveryStoreInfo;
    }
    
    const char* getStoreNameById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szWareHouseName.c_str();
        }
        return NULL;
    }
    
    const char* getStoreFullNameById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szWareHouseFullName.c_str();
        }
        return NULL;
    }
    
    const char* getStoreCodeById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szWareHouseCode.c_str();
        }
        return NULL;
    }
    
    const char* getStoreAddressById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szAddress.c_str();
        }
        return NULL;
    }
    
    const char* getStoreAuthenticatedPersonById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szAuthenticatedPerson.c_str();
        }
        return NULL;
    }
    
    const char* getStoreKeeperById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->szKeeper.c_str();
        }
        return NULL;
    }
    
    const char* getStoreZipCodeById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->ssZipCode.c_str();
        }
        return NULL;
    }
    
    const char* getStoreFaxById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->ssZipCode.c_str();
        }
        return NULL;
    }
    
    const char* getStoreEmailById(long long id){
        DeliveryStoreInfoArrayContainer::iterator it = deliveryStoreInfoArrayContainer.find(id);
        if (it != deliveryStoreInfoArrayContainer.end()) {
            return it->second->ssZipCode.c_str();
        }
        return NULL;
    }
    
    CStoreOrderInfo* getFindStoreInfo(const char* oticket){
        if(NULL == oticket) return NULL;
        long long ticket = atoll(oticket);
        StoreOrderArrayContainer::iterator it = inStoreOrderArrayContainer.find(ticket);
        
        //        DeliveryInfoArrayContainer::iterator it = deliveryInfoArrayContainer.find(ticket);
        //        if (it != deliveryInfoArrayContainer.end()) {
        //            return it->second;
        //        }
        
        if (it != inStoreOrderArrayContainer.end()) {
            return it->second;
        }
        
        it = applyInStoreOrderArrayContainer.find(ticket);
        if (it != applyInStoreOrderArrayContainer.end()) {
            return it->second;
        }
        
        it = outStoreOrderArrayContainer.find(ticket);
        if (it != outStoreOrderArrayContainer.end()) {
            return it->second;
        }
        
        it = cangOrderArrayContainer.find(ticket);
        if (it != cangOrderArrayContainer.end()) {
            return it->second;
        }
        
        it = cangToInStoreOrderArrayContainer.find(ticket);
        if (it != cangToInStoreOrderArrayContainer.end()) {
            return it->second;
        }
        
        return NULL;
    }
    
    long long getApplyByStoreId(long long id) {
        StoreOrderArrayContainer::iterator it;
        for(it=inStoreOrderArrayContainer.begin(); it!=inStoreOrderArrayContainer.end(); ++it) {
            if(it->second->iImportid == id) return it->second->iApplyid;
        }
        return 0;
    }
    
    const char * getApplyInStoreAllApplyId(){
        return applyInStoreOrderInfo;
    }
    
    const char * getInStoreAllApplyId(){
        return inStoreOrderInfo;
    }
    
    const char * getOutStoreAllIds(){
        return outStoreOrderInfo;
    }
    
    const char * getCangOrderAllIds(){
        return cangOrderInfo;
    }
    
    const char * getCangToInStoreOrderAllIds(){
        return cangToInStoreOrderInfo;
    }
    
    const char * getApplyDate(const char *ticket){//申请时间
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->ssApplyDate.c_str();
        return NULL;
    }
    
    //可用重量
    double getBalanceWeight(const char *ticket)
    {
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return res->dbCangweight - res->dbFreezeImportWeight - res->dbFreezeWaitWeight - res->dbFreezeCloseWeight;
        return 0.0;
    }
    
    double getFreezeImportWeight(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->dbFreezeImportWeight;
        return 0.0;
    }
    
    double getFreezeCloseWeight(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->dbFreezeCloseWeight;
        return 0.0;
    }
    
    double getFreezeWaitWeight(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->dbFreezeWaitWeight;
        return 0.0;
    }
    
    double getCloseWeight(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->dbCloseWeight;
        return 0.0;
    }
    
    char getCangType(const char *ticket){//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->chCangType;
        return '\0';
    }
    
    const char * getCangDate(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->ssCangDate.c_str();
        return NULL;
    }
    
    char getCangStatus(const char *ticket){
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->chCangStatus;
        return '\0';
    }
    
    const char * getExportdate(const char *iApplyid){ // '出库时间',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->ssExportdate.c_str();
        return NULL;
    }
    
    char getExportStatus(const char *ticket){ // '出库状态',
        CStoreOrderInfo* res = getFindStoreInfo(ticket);
        if(NULL != res) return  res->chExportStatus;
        return '\0';
    }
    
    int getImportid(const char *iApplyid){ // '入库单ID',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iImportid;
        return 0;
    }
    
    long long getApplyid(const char *id){ // '接收ID',
        CStoreOrderInfo* res = getFindStoreInfo(id);
        if(NULL != res) return  res->iApplyid;
        return 0;
    }
    
    int getWarehouseid(const char *iApplyid){  // '交收仓库',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iWarehouseid;
        return 0;
    }
    
    int getAccountgroupid(const char *iApplyid){// '机构',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iAccountgroupid;
        return 0;
    }
    
    const char*  getDeliverynumber(const char *iApplyid){//提货码-
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->ssDeliverynumber.c_str();
        return NULL;
    }
    
    const char*  getDeliveryMen(const char *iApplyid){//提货人名称
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->ssDeliveryMen.c_str();
        return NULL;
    }
    
    const char*  getIdCode(const char *iApplyid){//提货人身份证
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->ssIdCode.c_str();
        return NULL;
    }
    const char*  getTelphone(const char *iApplyid){//提货人电话号码
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->ssTelphone.c_str();
        return NULL;
    }
    
    int getSymbolgroupid(const char *iApplyid){ // '品种名称',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iSymbolgroupid;
        return 0;
    }
    
    double getOverPlusweight(const char *iApplyid) {//剩余重量
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbImportweight - res->dbExportweight - res->dbCangweight - res->dbFreezeWaitWeight - res->dbFreezeexportweight - res->dbFreezecangweight;
        return 0.0;
    }
    
    const char* getInStoreSymbolcode(const char *iApplyid){// '商品代码',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szSymbolcode.c_str();
        return NULL;
    }
    
    const char* getImportdate(const char *iApplyid){ // '入库时间',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szImportdate.c_str();
        return NULL;
    }
    char getImportstatus(const char *iApplyid){// '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->chImportstatus;
        return '\0';
    }
    
    char getApplystatus(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->chApplystatus;
        return '\0';
    }
    
    const char* getRejectreason(const char *iApplyid){// '拒绝原因',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szRejectreason.c_str();
        return NULL;
    }
    const char* getCheckresult(const char *iApplyid){// '检验结果',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szCheckresult.c_str();
        return NULL;
    }
    double getApplyweight(const char *iApplyid){// '申请重量',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbApplyweight;
        return 0.0;
    }
    double getImportweight(const char *iApplyid){// '入库重量',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbImportweight;
        return 0.0;
    }
    double getFreezeexportweight(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbFreezeexportweight;
        return 0.0;
    }
    double getExportweight(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbExportweight;
        return 0.0;
    }
    double getFreezecangweight(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbFreezecangweight;
        return 0.0;
    }
    double getCangweight(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbCangweight;
        return 0.0;
    }
    double getUnitweight(const char *iApplyid){ // '每件重量',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->dbUnitweight;
        return 0.0;
    }
    int getQuantity(const char *iApplyid){ // '件数',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iQuantity;
        return 0;
    }
    const char* getGoodsnumber(const char *iApplyid){// '货号',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szGoodsnumber.c_str();
        return NULL;
    }
    
    const char* getLot(const char *iApplyid){// '批号',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szLot.c_str();
        return NULL;
    }
    const char* getMade(const char *iApplyid){// '生产年月',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szMade.c_str();
        return NULL;
    }
    const char* getPackage(const char *iApplyid){// '包装方式',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szPackage.c_str();
        return NULL;
    }
    const char* getFactory(const char *iApplyid){// '生产厂商',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szFactory.c_str();
        return NULL;
    }
    
    const char* getWarehousetransactor(const char *iApplyid){// '仓库经办人',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szWarehousetransactor.c_str();
        return NULL;
    }
    const char* getWarehousemanager(const char *iApplyid){// '仓库负责人',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szWarehousemanager.c_str();
        return NULL;
    }
    const char* getClienttransactor(const char *iApplyid){// '客户经办人',
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->szClienttransactor.c_str();
        return NULL;
    }
    
    int getAttributeNum(const char *iApplyid){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->iAttributeNum;
        return 0;
    }
    
    int getAttributeIdByIndex(const char *iApplyid, int index){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->arrAttributArrValue[index].iAttributeId;
        return 0;
    }
    
    const char* getAttributeValueByIndex(const char *iApplyid, int index){
        CStoreOrderInfo* res = getFindStoreInfo(iApplyid);
        if(NULL != res) return  res->arrAttributArrValue[index].szAttvalue.c_str();
        return NULL;
    }
    
    //交收变更通知
    void handler_sellte_notify(const char * json){
        printBusLogEx("handler_sellte_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON * orders = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != orders) {
                    int ordercount = cJSON_GetArraySize(orders);
                    for(int i = 0; i < ordercount;i++){
                        cJSON * order = cJSON_GetArrayItem(orders, i);
                        CFindOrderRes *findOrder = new CFindOrderRes;
                        convert_order_from_jsonnode(ZYJ_TYPE, order, (char*)findOrder);
                        //删除之前的单据
                        map<long long, CFindOrderRes*>::iterator iter =  settleorderContainer.find(findOrder->iCounterPartExecuteTicket);
                        if (iter != settleorderContainer.end()) {
                            CFindOrderRes* pTmp = iter->second;
                            settleorderContainer.erase(iter);
                            delete pTmp;
                        }
                        settleorderContainer.insert(make_pair(findOrder->iCounterPartExecuteTicket, findOrder));
                        
                        //删除之前的的单号
                        vector<long long>::iterator l;
                        for(l = settleordercodeContainer.begin(); l != settleordercodeContainer.end(); l++) {
                            long long s = (long long)(*l);
                            if(s == findOrder->iOrderticket) {
                                settleordercodeContainer.erase(l);
                                break;
                            }
                        }
                        settleordercodeContainer.push_back(findOrder->iCounterPartExecuteTicket);
                    }
                    resetsettleordercodes();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_sellte_notify(nRet);
    }
    
    //出库单变更通知
    void handler_outstore_order_notify(const char * json){
        printBusLogEx("handler_outstore_order_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  outStoreOrderArrayContainer.find(info->iExportid);
                        if (iter != outStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            outStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        outStoreOrderArrayContainer.insert(make_pair(info->iExportid, info));
                    }
                    resetExportStoreInfo();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_outstore_order_notify(nRet);
    }
    
    //“入库申请”状态系统推送
    void handler_apply_instore_notify(const char * json){
        printBusLogEx("handler_apply_instore_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  applyInStoreOrderArrayContainer.find(info->iApplyid);
                        if (iter != applyInStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            applyInStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        if(info->chApplystatus == '6') {
                            delete info;
                            info = NULL;
                            continue;
                        }
                        applyInStoreOrderArrayContainer.insert(make_pair(info->iApplyid, info));
                    }
                    resetApplyInStoreInfo();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_apply_instore_notify(nRet);
    }
    
    //仓单更改通知
    void handler_cang_order_notify(const char * json){
        printBusLogEx("handler_cang_order_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  cangOrderArrayContainer.find(info->iCangid);
                        if (iter != cangOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            cangOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        cangOrderArrayContainer.insert(make_pair(info->iCangid, info));
                    }
                    resetCangOrder();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_cang_order_notify(nRet);
    }
    
    //提货意向”状态推送
    void handler_apply_delivery_notify(const char * json){
        printBusLogEx("handler_apply_delivery_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CDeliveryInfo* info = new CDeliveryInfo;
                        convert_delivery_info_from_jsonnode(cjson, (char*)info);
                        map<long long, CDeliveryInfo*>::iterator iter =  deliveryInfoArrayContainer.find(info->iDeliveryid);
                        if (iter != deliveryInfoArrayContainer.end()) {
                            CDeliveryInfo* pTmp = iter->second;
                            deliveryInfoArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        deliveryInfoArrayContainer.insert(make_pair(info->iDeliveryid, info));
                    }
                    resetApplyDeliveryIntention();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_apply_delivery_notify(nRet);
    }
    
    //入库单更改通知
    void handler_instore_order_notify(const char * json){
        printBusLogEx("handler_instore_order_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        map<long long, CStoreOrderInfo*>::iterator iter =  inStoreOrderArrayContainer.find(info->iImportid);
                        if (iter != inStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            inStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        inStoreOrderArrayContainer.insert(make_pair(info->iImportid, info));
                    }
                    resetInStoreOrderInfo();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_instore_order_notify(nRet);
    }
    
    //”仓单转入库单“变更通知
    void handler_cangorder_to_instore_notify(const char * json){
        printBusLogEx("handler_cangorder_to_instore_notify %s", json);
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                string orderkey = "CANG";
                cJSON *jsonArray = cJSON_GetObjectItem(root, orderkey.c_str());
                if(NULL != jsonArray) {
                    int ordercount = cJSON_GetArraySize(jsonArray);
                    for (int i = 0; i < ordercount; i++) {
                        cJSON * cjson = cJSON_GetArrayItem(jsonArray, i);
                        CStoreOrderInfo* info = new CStoreOrderInfo;
                        convert_in_store_order_from_jsonnode(cjson, (char*)info);
                        
                        map<long long, CStoreOrderInfo*>::iterator iter =  cangToInStoreOrderArrayContainer.find(info->iCangToImportId);
                        if (iter != cangToInStoreOrderArrayContainer.end()) {
                            CStoreOrderInfo* pTmp = iter->second;
                            cangToInStoreOrderArrayContainer.erase(iter);
                            delete pTmp;
                        }
                        cangToInStoreOrderArrayContainer.insert(make_pair(info->iCangToImportId, info));
                    }
                    resetCangToInStoreOrder();
                }
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_cangorder_to_instore_notify(nRet);
    }
    
    
    CDeliveryInfo* getFindApplyDeliveryInfo(const char* oticket){
        if(NULL == oticket) return NULL;
        long long ticket = atoll(oticket);
        DeliveryInfoArrayContainer::iterator it = deliveryInfoArrayContainer.find(ticket);
        if (it != deliveryInfoArrayContainer.end()) {
            return it->second;
        }
        return NULL;
    }
    
    const char*  getApplyDeliveryIds(){
        return deliveryInfo;
    }
    
    const char*  getApplyDeliverymen(const char * ticket){ // '提货人',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssDeliverymen.c_str();
        return NULL;
    }
    
    const char*  getApplyIdcode(const char * ticket){ // '身份证',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssIdcode.c_str();
        return NULL;
    }
    
    const char*  getApplyTelephone(const char * ticket){ // '联系电话',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssTelephone.c_str();
        return NULL;
    }
    const char*  getApplyAddress(const char * ticket){ // '联系地址',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssAddress.c_str();
        return NULL;
    }
    const char*  getApplyZipcode(const char * ticket){ // '邮政编码',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssZipcode.c_str();
        return NULL;
    }
    
    const char*  getApplyInvoicetitle(const char * ticket){ // '发票抬头',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssInvoicetitle.c_str();
        return NULL;
    }
    const char*  getApplyCompanyname(const char * ticket){ // '公司名称',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssCompanyname.c_str();
        return NULL;
    }
    const char*  getApplyCompanyaddress(const char * ticket){ // '公司地址',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssCompanyaddress.c_str();
        return NULL;
    }
    const char*  getApplyCompanytelephone(const char * ticket){ // '公司电话',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssCompanytelephone.c_str();
        return NULL;
    }
    const char*  getApplyBankaccount(const char * ticket){ // '银行账号',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssBankaccount.c_str();
        return NULL;
    }
    const char*  getApplyBankdeposit(const char * ticket){ // '开户行',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssBankdeposit.c_str();
        return NULL;
    }
    const char*  getApplyTaxregistrynumber(const char * ticket){ // '税务登记号',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssTaxregistrynumber.c_str();
        return NULL;
    }
    
    const char*  getApplyDeliverydatebegin(const char * ticket){ // '提货日期D5',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssDeliverydatebegin.c_str();
        return NULL;
    }
    const char*  getApplyDeliverydateend(const char * ticket){ // '提货日期D6',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssDeliverydateend.c_str();
        return NULL;
    }
    const char*  getApplyDeliverynumber(const char * ticket){ // '提货号',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssDeliverynumber.c_str();
        return NULL;
    }
    const char*  getApplySymbolcode(const char * ticket){ // '商品代码',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssSymbolcode.c_str();
        return NULL;
    }
    const char*  getApplyWarehousefeedate(const char * ticket){ // '仓储费截止日期',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssWarehousefeedate.c_str();
        return NULL;
    }
    const char*  getApplyRejectreason(const char * ticket){
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssRejectreason.c_str();
        return NULL;
    }
    const char*  getApplySettlementdate(const char * ticket){ // '交割日期',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->ssSettlementdate.c_str();
        return NULL;
    }
    
    long long getApplyDeliveryid(const char * ticket){ // '提货单ID',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iDeliveryid;
        return 0;
    }
    int getApplyMatchId(const char * ticket){
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iMatchId;
        return 0;
    }
    
    int getApplyWarehouseid(const char * ticket){ // '仓库ID',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iWarehouseid;
        return 0;
    }
    int getApplyCangId(const char * ticket){	// 仓单ID
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iCangId;
        return 0;
    }
    int getApplyExecuteticket(const char * ticket){ // '成交单号',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iExecuteticket;
        return 0;
    }
    int getApplyCounterpartyexecuteticket(const char * ticket){ // '对手单号',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->iCounterpartyexecuteticket;
        return 0;
    }
    double getApplyDeliveryweight(const char * ticket){ // '提货重量',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->dbDeliveryweight;
        return 0.0;
    }
    
    double getApplySettlementprirce(const char * ticket){ // '交割价格',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->dbSettlementprirce;
        return 0.0;
    }
    char getApplyDeliverystatus(const char * ticket){ // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->chDeliverystatus;
        return '\0';
    }
    char getApplyIsinvoice(const char * ticket){ // '是否需要发票',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->chIsinvoice;
        return '\0';
    }
    char getApplyIscancel(const char * ticket){ // '是否撤销',
        CDeliveryInfo* res = getFindApplyDeliveryInfo(ticket);
        if(res != NULL) return res->chIscancel;
        return '\0';
    }
    //入库单转仓单
    int trade_instore_to_cang_order(const char *  ticket, double weight){
        
        if(ticket == NULL) return CHECK_PARAMS_FAIL;
        
        if(weight <= 0) return CHECK_PARAMS_FAIL;
        
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[32];
        
        memset(sstmp, 0, sizeof(sstmp));
        //        sprintf(sstmp, "%d", getImportid(ticket));
        sprintf(sstmp, "%s", ticket);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%.3f", weight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_WEIGHT, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_instore_to_cang_order = %s", out);
        int nSeq = nhp_instore_to_cang_order(out);
        free(out);
        return nSeq;
    }
    //入库单转出库单
    int trade_instore_to_exportstore_order(const char *ticket, const char * deliveryName, double weight, const char *dCode, const char *telPhone){
        if(NULL == dCode || !isValidShenFenzheng(dCode)) return CHECK_PARAMS_FAIL;
        if(ticket == NULL) return CHECK_PARAMS_FAIL;
        if(weight <= 0) return CHECK_PARAMS_FAIL;
        if(NULL == telPhone || isPhone(telPhone) != 0) return CHECK_PHONE_PARAMS_FAIL;
        char *out;
        cJSON *root = getcJsonRoot();
        char sstmp[128];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", ticket);
        //        sprintf(sstmp, "%d", getImportid(ticket));
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_ID, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%.3f", weight);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_OPT_WEIGHT, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", deliveryName);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_DELIVERY_MEN, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", dCode);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IDCARD, sstmp);
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", telPhone);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TEL, sstmp);
        
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_instore_to_cang_order = %s", out);
        int nSeq = nhp_instore_to_exportstore_order(out);
        free(out);
        return nSeq;
    }
    
    void handler_op_in_store_order_response(int nSeq, const char * json){
        cJSON *root;
        root=cJSON_Parse(json);
        int nRet = -1;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
            }
        }
        cJSON_Delete(root);
        tradeuievents.op_in_store_order_response(nSeq, nRet);
    }
    
    //
    void userloginwithnet(char * account, char * password, char * tradeip, int tradeport, char * quoteip, int quoteport) {
        //        init_engine();
        
        memset(LoginedAccountName, 0, 256);
        
        set_login_info(account, password);
        //        printBusLogEx("login on:%s %d   %s %d  by user:%s pass:%s",tradeip, tradeport, quoteip, quoteport, account, newPass);
        nhp_init_net_login_info(tradeip, tradeport, quoteip, quoteport);
    }
    
    void loginquotewithnet(char * quoteip, int quoteport)
    {
        memset(LoginedAccountName, 0, 256);
        
        nhp_init_net_login_quote(quoteip, quoteport);
    }
    
    
    int add_balance(char * server, int port){
        return nhp_add_balance(server, port);
    }
    void clear_balance(){
        nhp_clear_balance();
    }
    
    void set_login_info(char * suser, char *  password) {
        //2015.01.08 登录密码加密
        //        char hash[50];
        //        getSHA1(hash, (unsigned char*)password, strlen(password));
        //        char* newPass = (char*)hash;
        //        nhp_set_login_info(suser, newPass);
        nhp_set_login_info(suser, password);
    }
    
    void start_balance() {
        nhp_start_balance();
    }
    
    
    
    //实时行情
    void quote_real_rsp(const char * pData, int pLen) {
        int iCount = pLen /  24;
        char symbolcodes[256];
        memset(symbolcodes, 0, sizeof(symbolcodes));
        for(int i = 0; i < iCount; i++) {
            //每一笔数据u
            CStuRecvQuoteInfo* quote = (CStuRecvQuoteInfo*)(pData + i * 24);
            if (check_valid_quote_real_rsp(quote->sCode.szCode) != 1)  continue;
            char * symbolcode = quote->sCode.szCode;
            SymbolContainer::iterator iter = symbolContainer.find(symbolcode);
            if (iter != symbolContainer.end()) {
                CSymbolParam * symbol = iter->second;
                if(strlen(symbolcodes) > 0) strcat(symbolcodes, ",");
                strcat(symbolcodes, quote->sCode.szCode);
                double price = quote->uiPrice / pow(10.0, (int)quote->sCode.cPriceUnit);
                //                double price = Rounding(quote->uiPrice / pow(10.0, (int)quote->sCode.cPriceUnit), symbol->nDecimal);
                long long second = quote->lastTick;
                
                //如果行情小于0.0001时，丢弃行情
                if (price < 0.0001) continue;
                
                symbol->lastTick = second;
                setpricedirection(symbol->fCurPrice, price , &symbol->nCurPriceDirection);
                symbol->flastPrice = symbol->fCurPrice; //设置上一口价格
                symbol->fCurPrice = price;
                symbol->fAskPrice = price + getrealtips(symbol->dbBuypips, symbol->nDecimal);
                symbol->fBidPrice = price + getrealtips(symbol->dbSellpips, symbol->nDecimal);
                if (symbol->fMaxPrice < price ) symbol->fMaxPrice = price;
                if (symbol->fMinPrice > price) symbol->fMinPrice = price;
                symbol->fClosePrice = price;
                //            int n = 1;
                //            for(int i = 0; i < symbol->nDecimal;  i ++)
                //                n *= 10;
                //这里设置分笔与分时数据 向最后增加
                //分时 查找是否有缓存
                string str = symbolcode;
                //            myPrintf("real_rsp time [%lld]\n", second);
                //            int currPrice = (int)floor(price* pow(10, symbol->nDecimal));
                int currPrice = (int)(price* pow(10, (double)symbol->nDecimal));
                int date = 0;
                int time = 0;
                convertToTime(second, &date, &time);
                TraceUnitArrayContainer::iterator it = traceUnitContainer.find(str);
                if (!traceUnitContainer.empty() && it != traceUnitContainer.end()) {
                    TraceUnitArray* pArr = it->second;
                    if(pArr) {
                        TraceUnitArray::iterator element;
                        unsigned int newTime = time / 100;
                        for(element = pArr->begin(); element != pArr->end(); element ++)
                        {
                            CSTraceUnit& oUnit = *element;
                            unsigned int oldTime = oUnit.uiTime;
                            if(newTime == oldTime) {
                                //oUnit.uiPrice = (int)floor(price * n);//价格
                                oUnit.uiPrice= currPrice;
                                oUnit.uiDate = date;
                                tradeuievents.on_quote_minute_data_notify(0,symbolcode);
                                break;
                            }
                        }
                    }
                }
                
                //分笔
                TraceUnitArrayContainer::iterator it2 = traceUnitRecodeContainer.find(str);
                if (!traceUnitRecodeContainer.empty() && it2 != traceUnitRecodeContainer.end()) {
                    TraceUnitArray* pArr = it2->second;
                    if(pArr && pArr->size() > 0)
                    {
                        CSTraceUnit oUnit;
                        oUnit.uiDate = date;
                        oUnit.uiTime = time;
                        //oUnit.uiPrice = (int)floor(price * n);//价格
                        oUnit.uiPrice= currPrice;
                        pArr->push_back(oUnit);
                    }
                    tradeuievents.on_quote_trace_data_notify(0,symbolcode);
                }
                
                //K线
                //查找替换
                TKLineDataContainer::iterator itK = klineDataContainer.find(str);
                if(itK != klineDataContainer.end()){
                    KLineDataGroupContainer subChild = itK->second;
                    KLineDataGroupContainer::iterator itKChild;
                    time_t lSecd = (time_t)second;
                    //                struct tm * timeinfo = localtime(&lSecd);
                    //后续需要加上时区处理
                    struct tm timeinfo;
                    if(localtime_r(&lSecd, &timeinfo) == NULL) continue;
                    
                    int year = timeinfo.tm_year + 1900;
                    int month = timeinfo.tm_mon + 1;
                    int day = timeinfo.tm_mday;
                    int hour = timeinfo.tm_hour;
                    int	min = timeinfo.tm_min;
                    //int sec = timeinfo.tm_sec;
                    int wday = timeinfo.tm_wday;
                    int _date = year * 10000 + month * 100 + day;
                    
                    int _time = hour * 100 + min;
                    
                    for(itKChild=subChild.begin();itKChild!=subChild.end();++itKChild) {
                        //更新
                        KLineDataGroup *kLine = itKChild->second;
                        TKHisUnitArray::iterator element2;
                        for(int i = kLine->m_arrKLineData.size() - 1; i>=0; i--) {
                            CSHisKUnit& unit = kLine->m_arrKLineData[i];// *element2;
                            switch (itKChild->first) {
                                case MINUTE1_HISK: {
                                    if(_time != unit.uiTime) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime = _time;
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(_time == unit.uiTime) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case MINUTES5_HISK: {
                                    if(_time >= unit.uiTime + 5) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime = (hour)*100 + min - min % 5;
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(unit.uiTime+5>_time && _time >= unit.uiTime) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case MINUTES10_HISK: {
                                    if(_time >= unit.uiTime + 10) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime = hour * 100 + min - min % 10;
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(unit.uiTime+10>_time && _time >= unit.uiTime) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case MINUTES30_HISK: {
                                    if(_time >= unit.uiTime + 30)  {
                                        CSHisKUnit unit2;
                                        unit2.uiTime = hour * 100 + min -  min % 30;
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(unit.uiTime + 30 > _time && _time >= unit.uiTime){
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case MINUTES60_HISK: {
                                    int _hour =(hour) * 100;
                                    if(_hour >= unit.uiTime + 100) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime = (hour + 1) * 100;//_time + 60 - (_time % 60);
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(_hour >= unit.uiTime && _hour < unit.uiTime + 100){
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case DAY_HISK: {
                                    if( _date > unit.uiDate) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime=0;
                                        unit2.uiDate = _date;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(_date == unit.uiDate) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case WEEK_HISK: {
                                    lSecd = (time_t)(second - (wday-1) * 24 * 60 * 60);
                                    if(localtime_r(&lSecd, &timeinfo) == NULL) return ;
                                    int _syear = timeinfo.tm_year + 1900;
                                    int _smonth = timeinfo.tm_mon + 1;
                                    int _sday = timeinfo.tm_mday;
                                    int _sdate = _syear * 10000 + _smonth * 100 + _sday;
                                    
                                    lSecd = (time_t)(second + (7 - wday) * 24 * 60 * 60);
                                    if(localtime_r(&lSecd, &timeinfo) == NULL) return ;
                                    
                                    int _eyear = timeinfo.tm_year + 1900;
                                    int _emonth = timeinfo.tm_mon + 1;
                                    int _eday = timeinfo.tm_mday;
                                    int _edate = _eyear * 10000 + _emonth * 100 + _eday;
                                    
                                    //_date	int	20141127	20141127
                                    if( _edate < _date) {
                                        lSecd = (time_t)(second + (7 - wday) * 24 * 60 * 60 + 24 * 60 * 60);
                                        if(localtime_r(&lSecd, &timeinfo) == NULL) return ;
                                        _syear = timeinfo.tm_year + 1900;
                                        _smonth = timeinfo.tm_mon + 1;
                                        _sday = timeinfo.tm_mday;
                                        _sdate = _syear * 10000 + _smonth * 100 + _sday;
                                        CSHisKUnit unit2;
                                        unit2.uiTime=0;
                                        unit2.uiDate = _sdate;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = currPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(_sdate <=  unit.uiDate &&  unit.uiDate <= _edate) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                case MONTH_HISK: { //20141100
                                    if( _date >= unit.uiDate + 100) {
                                        CSHisKUnit unit2;
                                        unit2.uiTime=0;
                                        unit2.uiDate = year * 10000 + (month) * 100;
                                        unit2.uiClosePrice = currPrice;
                                        unit2.uiOpenPrice = unit.uiOpenPrice;
                                        unit2.uiMaxPrice = currPrice;
                                        unit2.uiMinPrice = currPrice;
                                        kLine->m_arrKLineData.push_back(unit2);
                                    } else if(_date >= unit.uiDate && _date < unit.uiDate + 100) {
                                        unit.uiClosePrice = currPrice;
                                        unit.uiMaxPrice = unit.uiMaxPrice>=unit.uiClosePrice?unit.uiMaxPrice:unit.uiClosePrice;
                                        unit.uiMinPrice = unit.uiMinPrice<=unit.uiClosePrice?unit.uiMinPrice:unit.uiClosePrice;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        }
                    }
                    tradeuievents.history_change_notify(symbolcode, currPrice, second);
                }
                tradeuievents.quote_real_notify(symbolcode, price, second);
            }
        }
        if(strlen(symbolcodes) > 0) {
            //更新浮动盈亏
            updateFPL();
            quote_change_end(iCount, symbolcodes);
            printBusLogEx("quote_real_rsp [%s]\n", symbolcodes);
        }
    }
    
    
    
    void quote_record_data_recv_end(int nSeq, char* pData, int pLen)
    {
        int type;
        char name[64];
        if(getSeq(nSeq, type, name)) {
            //        map<int, SeqMap>::iterator iter = seqContainer.find(nSeq);
            //        //需要加上周期值 如果当前请求数据量为0时，则接收无法判定了
            //        if (iter == seqContainer.end()) return;
            //        SeqMap map = iter->second;
            //        char* name = map.symbol;
            //            seqContainer.erase(nSeq);
            CSTraceUnit* array = (CSTraceUnit*)pData;
            int arrLen = pLen/sizeof(CSTraceUnit);
            printBusLogEx("quote_record_data_recv_end name [%s]  len = %d\n", name, arrLen);
            
            TraceUnitArray* traceArray = new TraceUnitArray();
            traceArray->clear();
            
            unsigned int yesterdayPrice = 0;
            int i = 0;
            for(i = 0; i < arrLen; i ++) {
                CSTraceUnit oUnit;
                oUnit.uiDate = array[i].uiDate;
                oUnit.uiTime = array[i].uiTime;
                
                oUnit.uiPrice = array[i].uiPrice;
                if(0 == i) {
                    yesterdayPrice = oUnit.uiPrice;
                }
                traceArray->push_back(oUnit);
            }
            //设置CLOSE
            int dot = getPriceDecimal(name);
            int nVal = 1;
            for(int k = 0; k < dot; k ++)
                nVal *= 10;
            double d = yesterdayPrice * 1.0 / nVal;
            setYesterDayClosePrice(name, d);
            
            string str = name;
            TraceUnitArrayContainer::iterator it = traceUnitRecodeContainer.find(str);
            if (it != traceUnitRecodeContainer.end()) {
                TraceUnitArray* pArr = it->second;
                delete pArr;
                it->second = traceArray;
            }
            else{
                traceUnitRecodeContainer.insert(make_pair(str, traceArray));
            }
        }
        tradeuievents.on_quote_trace_data_notify(nSeq,  name);
    }
    
    
    int request_zs_trace_data(char* szCode, int nSize)
    {
        SCodeInfo pCodeInfo;
        memset(&pCodeInfo, 0, sizeof(SCodeInfo));
        memcpy(pCodeInfo.szCode, szCode, strlen(szCode));
        pCodeInfo.cPriceUnit = 0;
        int pLen = sizeof(SCodeInfo);
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&pCodeInfo, sizeof(SCodeInfo));
        int nSeq = nhp_send_quote_req(STKTRACEDATA, pData, pLen);
        delete []  pData;
        //        SeqMap his;
        //        strcpy(his.symbol, szCode);
        //        seqContainer.insert(make_pair(nSeq, his));
        addSeq(nSeq, 0, szCode);
        return nSeq;
    }
    
    //TODO: 分时
    void quote_min_data_recv_end(int nSeq, char* pData, int pLen)
    {
        //        map<int, SeqMap>::iterator iter = seqContainer.find(nSeq);
        //        //需要加上周期值 如果当前请求数据量为0时，则接收无法判定了
        //        if (iter == seqContainer.end()) return;
        //        SeqMap map = iter->second;
        //        char* name = map.symbol;
        //            seqContainer.erase(nSeq);
        int type;
        char name[64];
        if(getSeq(nSeq, type, name)) {
            int arrLen = pLen/sizeof(CSTraceUnit);
            CSTraceUnit* array = (CSTraceUnit*)pData;
            printBusLogEx("quote_min_data_recv_end == name [%s]\n", name);
            int nBeginTime = 0;
            int nTotalTime = 0;
            if(!get_symbol_data_min_begin_end_time(name, &nBeginTime, &nTotalTime))
                return;
            
            TraceUnitArray* traceArray = new TraceUnitArray();
            traceArray->clear();
            unsigned int yesterdayPrice = 0;
            int i = 0;
            int k = 0;
            int nFindIndex = 0;
            for(k = 0; k < nTotalTime; k ++)
            {
                CSTraceUnit oUnit;
                oUnit.uiDate = 0;
                oUnit.uiTime = nBeginTime + (k % 60) + (k / 60 * 100);
                if(oUnit.uiTime >= 2400)
                    oUnit.uiTime -= 2400;
                oUnit.uiPrice = 0;
                
                for(i = nFindIndex; i < arrLen; i ++) {
                    //                myPrintf("data [%d], time [%d], price[%d]\n", array[i].uiDate, array[i].uiTime, array[i].uiPrice);
                    if(array[i].uiTime != oUnit.uiTime)
                        continue;
                    
                    nFindIndex = i;
                    oUnit.uiDate = array[i].uiDate;
                    oUnit.uiTime = array[i].uiTime;
                    oUnit.uiPrice = array[i].uiPrice;
                    break;
                }
                traceArray->push_back(oUnit);
            }
            //设置CLOSE
            int dot = getPriceDecimal(name);
            int nVal = 1;
            for(int k = 0; k < dot; k ++)
                nVal *= 10;
            double d = yesterdayPrice * 1.0 / nVal;
            if(d > 1)
                setYesterDayClosePrice(name, d);
            
            string str = name;
            TraceUnitArrayContainer::iterator it = traceUnitContainer.find(str);
            if (it != traceUnitContainer.end()) {
                TraceUnitArray* pArr = it->second;
                delete pArr;
                it->second = traceArray;
            }
            else{
                traceUnitContainer.insert(make_pair(str, traceArray));
            }
        }
        tradeuievents.on_quote_minute_data_notify(nSeq, name);
    }
    
    //分时
    int request_zs_min_data(char* szCode, int nSize)
    {
        SCodeInfo pCodeInfo;
        memset(&pCodeInfo, 0, sizeof(SCodeInfo));
        memcpy(pCodeInfo.szCode, szCode, strlen(szCode));
        pCodeInfo.cPriceUnit = 0;
        int pLen = sizeof(SCodeInfo);
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&pCodeInfo, sizeof(SCodeInfo));
        int nSeq = nhp_send_quote_req(STKMINUTEDATA, pData, pLen);
        delete []  pData;
        //        SeqMap his;
        //        memset(&his, 0, sizeof(SeqMap));
        //        strcpy(his.symbol, szCode);
        //        seqContainer.insert(make_pair(nSeq, his));
        addSeq(nSeq, 0, szCode);
        return nSeq;
    }
    
    
    //kline数据
    void quote_kline_data_recv_end(int nSeq, char* pData, int pLen)
    {
        map<int, SeqMap>::iterator iter = seqContainer.find(nSeq);
        //需要加上周期值 如果当前请求数据量为0时，则接收无法判定了
        if (iter == seqContainer.end()) return;
        SeqMap map = iter->second;
        char* name = map.symbol;
        int nCurType = map.cType;
        seqContainer.erase(nSeq);
        int arrLen = pLen / sizeof(CSHisKUnit);
        CSHisKUnit* array = (CSHisKUnit*)(pData);
        KLineDataGroup* pGroup = new KLineDataGroup();
        pGroup->m_arrKLineData.clear();
        int dot = getPriceDecimal(name);
        TKHisUnitArray m_arrKLineData;
        for(int i = 0; i < arrLen; i ++) {
            if(array[i].uiOpenPrice>(50000*pow(10, (double)dot))) continue;
            CSHisKUnit oUnit;
            oUnit.uiDate = array[i].uiDate;
            oUnit.uiTime = array[i].uiTime;
            
            oUnit.uiOpenPrice = array[i].uiOpenPrice;//开盘价
            oUnit.uiMaxPrice = array[i].uiMaxPrice;//高
            oUnit.uiMinPrice = array[i].uiMinPrice;//低
            oUnit.uiClosePrice = array[i].uiClosePrice;//收
            
            m_arrKLineData.push_back(oUnit);
        }
        pGroup->m_arrKLineData = m_arrKLineData;
        pGroup->m_nKineDataType = nCurType;
        //查找替换
        TKLineDataContainer::iterator it = klineDataContainer.find(name);
        if(it != klineDataContainer.end()){
            KLineDataGroupContainer subChild = it->second;
            KLineDataGroupContainer::iterator gc_it = subChild.find(nCurType);
            KLineDataGroup* pCurGroup = NULL;
            if(gc_it != subChild.end()){
                pCurGroup = gc_it->second;
                if(pCurGroup->m_nKineDataType == pGroup->m_nKineDataType)
                {
                    //更新内容了
                    updateKLine(pCurGroup, pGroup);
                    delete pGroup;
                }
            } else {
                subChild.insert(make_pair(nCurType, pGroup));
                it->second = subChild;
            }
        } else {
            KLineDataGroupContainer groupContainer;
            groupContainer.insert(make_pair(nCurType, pGroup));
            klineDataContainer.insert(make_pair(name, groupContainer));
        }
        tradeuievents.on_quote_history_data_notify(nSeq, nCurType, name);
        printBusLogEx("quote_kline_data_recv_end name=%s len=%d nType=%d\n", name, nSeq, nCurType);
    }
    
    int request_zs_kline(const char* szCode, int nPeriodicType, int hasNum, int nReqCount)
    {
        SHisotyDataReq pHisDataReq;
        pHisDataReq.nDataType = nPeriodicType;
        pHisDataReq.nHasNum = hasNum;
        pHisDataReq.nAskNum = nReqCount;
        if(pHisDataReq.nAskNum <= 0 && 0 != pHisDataReq.nHasNum) {
            pHisDataReq.nHasNum = 0;
            pHisDataReq.nAskNum = 100;
        }
        SCodeInfo pCodeInfo;
        strcpy(pCodeInfo.szCode, szCode);
        pCodeInfo.cPriceUnit = 0;
        
        int pLen = sizeof(SHisotyDataReq) + sizeof(SCodeInfo);
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&pHisDataReq, sizeof(SHisotyDataReq));
        memcpy(pData + sizeof(SHisotyDataReq), (const void *)&pCodeInfo, sizeof(SCodeInfo));
        int nSeq = nhp_send_quote_req(HISKDATA, pData, pLen);
        delete []  pData;
        addSeq(nSeq, nPeriodicType, szCode);
        return nSeq;
    }
    
    int getSymbolMinuteDataCount(const char* utfSymbol)
    {
        int i = 0;
        string strSymbol = utfSymbol;
        int type = getSymbolType((char* )utfSymbol);
        if(type == ZYJ_TYPE) {
            ZYQMinUnitArrayContainer::iterator itorder;
            for (itorder = zyqMinUnitArrayContainer.begin(); itorder !=zyqMinUnitArrayContainer.end() ; itorder++) {
                string str = itorder->first;
                if(0 == str.compare(utfSymbol)) {
                    return itorder->second.size();
                }
            }
        } else if(type == ZS_TYPE) {
            TraceUnitArrayContainer::iterator itorder;
            for (itorder = traceUnitContainer.begin(); itorder !=traceUnitContainer.end() ; itorder++) {
                string str = itorder->first;
                if(0 == str.compare(utfSymbol)) {
                    return itorder->second->size();
                }
            }
        }
        return i;
    }
    int getZSSymbolMinuteData(const char* utfSymbol, TrendData* trend, int nMaxSize)
    {
        int i = 0;
        int n=0;
        string strSymbol = utfSymbol;
        TraceUnitArrayContainer::iterator itorder;
        for (itorder = traceUnitContainer.begin(); itorder !=traceUnitContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(utfSymbol)) {
                TraceUnitArray* array = itorder->second;
                TraceUnitArray::iterator element;
                int max = 0;
                int min = 0;
                for(element = array->begin(); element != array->end(); element ++, i ++) {
                    CSTraceUnit oUnit = *element;
                    if(oUnit.uiPrice == 0) continue;
                    trend->data[n].date = oUnit.uiDate;
                    trend->data[n].time = oUnit.uiTime;
                    trend->data[n].price = oUnit.uiPrice;
                    int price = oUnit.uiPrice;
                    trend->data[n].price = price;
                    if(n == 0) {
                        max = price;
                        min = price;
                    } else {
                        if(max < price) max = price;
                        if(min > price) min = price;
                    }
                    n+=1;
                }
                trend->max = max;
                trend->min = min;
                break;
            }
        }
        return n;
    }
    
    int getSymbolMinuteData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize)
    {
        int i = 0;
        string strSymbol = utfSymbol;
        TraceUnitArrayContainer::iterator itorder;
        for (itorder = traceUnitContainer.begin(); itorder !=traceUnitContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(utfSymbol)) {
                TraceUnitArray* array = itorder->second;
                TraceUnitArray::iterator element;
                //                int size =  getSymbolMinuteDataCount(utfSymbol);
                for(element = array->begin(); element != array->end() && i < nMaxSize; element ++, i ++)
                {
                    CSTraceUnit oUnit = *element;
                    //                    if(oUnit.uiDate == 0) continue;
                    arr[i].uiDate = oUnit.uiDate;
                    arr[i].uiTime = oUnit.uiTime;
                    arr[i].uiPrice = oUnit.uiPrice;
                }
                break;
            }
        }
        return i;
    }
    
    int getSymbolTraceDataCount(const char* utfSymbol)
    {
        int i = 0;
        string strSymbol = utfSymbol;
        int type = getSymbolType((char* )utfSymbol);
        if(type == ZYJ_TYPE) {
            ZYQTraceUnitArrayContainer::iterator itorder;
            for (itorder = zyqTraceUnitArrayContainer.begin(); itorder !=zyqTraceUnitArrayContainer.end() ; itorder++) {
                string str = itorder->first;
                if(0 == str.compare(utfSymbol)) {
                    return itorder->second.size();
                }
            }
        } else if(type == ZS_TYPE) {
            TraceUnitArrayContainer::iterator itorder;
            for (itorder = traceUnitRecodeContainer.begin(); itorder !=traceUnitRecodeContainer.end() ; itorder++) {
                string str = itorder->first;
                if(0 == str.compare(utfSymbol)) {
                    return itorder->second->size();
                }
            }
        }
        return i;
    }
    
    int getZSSymbolTraceData(const char* utfSymbol, TrendData* trend, int nMaxSize)
    {
        int i = 0;
        int n=0;
        string strSymbol = utfSymbol;
        TraceUnitArrayContainer::iterator itorder;
        for (itorder = traceUnitRecodeContainer.begin(); itorder !=traceUnitRecodeContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(utfSymbol)) {
                TraceUnitArray* array = itorder->second;
                TraceUnitArray::iterator element;
                int max = 0;
                int min = 0;
                for(element = array->begin(); element != array->end(); element ++, i ++) {
                    CSTraceUnit oUnit = *element;
                    if(oUnit.uiPrice == 0) continue;
                    trend->data[n].date = oUnit.uiDate;
                    trend->data[n].time = oUnit.uiTime;
                    trend->data[n].price = oUnit.uiPrice;
                    int price = trend->data[n].price;
                    if(n == 0) {
                        max = price;
                        min = price;
                    } else {
                        if(max < price) max = price;
                        if(min > price) min = price;
                    }
                    n+=1;
                }
                trend->max = max;
                trend->min = min;
                break;
            }
        }
        return n;
    }
    
    int getSymbolTraceData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize)
    {
        int i = 0;
        string strSymbol = utfSymbol;
        TraceUnitArrayContainer::iterator itorder;
        for (itorder = traceUnitRecodeContainer.begin(); itorder !=traceUnitRecodeContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(utfSymbol)) {
                TraceUnitArray* array = itorder->second;
                TraceUnitArray::iterator element;
                for(element = array->begin(); element != array->end(); element ++, i ++)
                {
                    CSTraceUnit oUnit = *element;
                    arr[i].uiDate = oUnit.uiDate;
                    arr[i].uiTime = oUnit.uiTime;
                    arr[i].uiPrice = oUnit.uiPrice;
                }
                break;
            }
        }
        return i;
    }
    
    // TODO: 中远期行情
    //盘面数据
    int trade_zyq_quote_report(STUReportDataIndex_ZYQ* idx, STUSimpleSymbol_ZYQ* symbols) {
        int pLen = sizeof(STUReportDataIndex_ZYQ) + sizeof(STUSimpleSymbol_ZYQ) * idx->m_usReqDesSymoblNum;
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)idx, sizeof(STUReportDataIndex_ZYQ));
        if(idx->m_usReqDesSymoblNum > 0)
            memcpy(pData + sizeof(STUReportDataIndex_ZYQ), (const void *)symbols, sizeof(STUSimpleSymbol_ZYQ) * idx->m_usReqDesSymoblNum);
        int nSeq = nhp_send_zyq_quote_report(pData, pLen);
        delete [] pData;
        return nSeq;
    }
    
    void request_all_zyq__quote_pankou(){
        int nSize = 0;
        SymbolContainer::iterator it;
        for (it = symbolContainer.begin(); it != symbolContainer.end(); it++) {
            CSymbolParam * symbol = it->second;
            char* sycode = (char*)symbol->ssSymbolcode.c_str();
            int type = getSymbolType(sycode);
            if(type==ZYJ_TYPE) nSize+=1;
        }
        nSize = nSize / REPORT_SIZE;
        if(nSize % REPORT_SIZE != 0) nSize+=1;
        for(int i = 0; i <= nSize; i++) {
            STUReportDataIndex_ZYQ indx;
            indx.m_usMarketType = MARKET_TYPE_ALL;
            indx.m_ucSeqType=REDATA_SEQ_TYPE_ASC;
            indx.m_usReqNum = REPORT_SIZE;
            indx.m_usBeginPos = REPORT_SIZE * i;
            indx.m_usReqDesSymoblNum = 0;
            indx.m_ucOrderBy = RPDATA_ORDER_BY_SYMOBL;
            trade_zyq_quote_report(&indx, NULL);
        }
    }
    
    //
    int handler_zyj_quote_pankou(int nSeq, const char* pData, int pLen) {
        int iCnt =0;
        char symbolcodes[1024];
        memset(symbolcodes, 0, 1024);
        int offest = 0;
        STUReportDataIndex_ZYQ* stuDataIndex = (STUReportDataIndex_ZYQ*)pData;
        offest += sizeof(STUReportDataIndex_ZYQ);
        STUCommLenDirec* stuLenDirec = (STUCommLenDirec*)(pData + offest);
        offest+=sizeof(STUCommLenDirec);
        //        STUReportDataResp_ZYQ* stuReportDatas = (STUReportDataResp_ZYQ*)&pData[offest];
        int marketType = stuDataIndex->m_usMarketType;
        for(int i =0; i<stuLenDirec->m_lenDirec; i++) {
            if(offest+1>pLen) break;
            STUReportDataResp_ZYQ* stuReportData = (STUReportDataResp_ZYQ*)(pData + offest);
            offest+=sizeof(STUReportDataResp_ZYQ);
            string symbolcode = stuReportData->m_oSSymobl.m_szSymbolCode;
            SymbolContainer::iterator iter = symbolContainer.find(symbolcode);
            if (iter != symbolContainer.end()) {
                CSymbolParam * symbol = iter->second;
                if(marketType != 0 && marketType != atoi(symbol->ssSymbolgroupid.c_str())) continue;
                if(strlen(symbolcodes) > 0) strcat(symbolcodes, ",");
                strcat(symbolcodes,symbolcode.c_str());
                int decimal = (int)stuReportData->m_oSSymobl.m_ucPriceUnit;
                double weishu = pow(10.0, decimal);
                
                double openprice = stuReportData->m_oReportPublicData.m_uiOpenPrice / weishu;
                double maxprice = stuReportData->m_oReportPublicData.m_uiMaxPrice / weishu;
                double minprice = stuReportData->m_oReportPublicData.m_uiMinPrice / weishu;
                double newprice =stuReportData->m_oReportPublicData.m_uiNewPrice / weishu;
                long long time = stuReportData->m_oTDateTime.m_iUpdateDataTime;
                double ffSettlPrice = stuReportData->m_oReportPublicData.m_uiYesSetllPrice / weishu;
                double ffYesterdayClosePrice = stuReportData->m_oReportPublicData.m_uiYesClosePrice /weishu;
                double ask = stuReportData->m_oQtyPriceV1.m_buyVol.m_uiPrice / weishu;
                double bid = stuReportData->m_oQtyPriceV1.m_sellVol.m_uiPrice / weishu;
                
                symbol->fOpenPrice = openprice;
                symbol->fMaxPrice = maxprice;
                symbol->fMinPrice = minprice;
                symbol->fSettlPrice = ffSettlPrice;//昨结
                symbol->fYesterdayClosePrice = ffYesterdayClosePrice;//昨收
                //                if(symbol->flastPrice != symbol->fCurPrice)
                //                    symbol->flastPrice = symbol->fCurPrice; //设置上一口价格
                symbol->fCurPrice = newprice;
                symbol->fAskPrice = ask;// +  getrealtips(symbol->dbBuypips, symbol->nDecimal);
                symbol->fBidPrice = bid;// + getrealtips(symbol->dbSellpips, symbol->nDecimal);
                symbol->lastTick = time;
                setpricedirection(symbol->fYesterdayClosePrice, newprice, &symbol->nCurPriceDirection);
                
                
                
                iCnt += 1 ;
                printBusLogEx("zyj_quote_pankou_rsp %s  %f %f \n", symbolcode.c_str(), maxprice, minprice);
            }
            ZYJSymbolInfoArray::iterator it = zyjSymbolInfo.find(symbolcode);
            if(it != zyjSymbolInfo.end()) {
                ZYJSTUSymbolTradeInfo& info = it->second;
                info.stuRealPublicDataPush.m_iPriceRiseDown = stuReportData->m_oReportPublicData.m_iPriceRiseDown;
                info.stuRealPublicDataPush.m_chTurnoverDirec = stuReportData->m_oReportPublicData.m_chTurnoverDirec;
                info.stuRealPublicDataPush.m_iPriceRiseDownRate = stuReportData->m_oReportPublicData.m_iPriceRiseDownRate;
                info.stuRealPublicDataPush.m_iTotalTurnoverFunds = stuReportData->m_oReportPublicData.m_iTotalTurnoverFunds;
                info.stuRealPublicDataPush.m_uiNewQty = stuReportData->m_oReportPublicData.m_uiNewQty;
                info.stuRealPublicDataPush.m_uiTotalTurnoverQty = stuReportData->m_oReportPublicData.m_uiTotalTurnoverQty;
                memcpy(&(info.stuNowDataVolPirces), &(stuReportData->m_oQtyPriceV1), sizeof(STUNowDataVolPirce));
            }
        }
        if(strlen(symbolcodes) > 0) {
            //更新浮动盈亏
            updateFPL();
            quote_change_end(iCnt, symbolcodes);
        }
        return iCnt;
    }
    
    void handler_zyj_quote_pankou_detail(int nSeq, const char* pData, int nLen) {
        int nRet = -1;
        int offest = 0;
        STUSimpleSymbol_ZYQ* stuSimpleSymbol_ZYQ = (STUSimpleSymbol_ZYQ*)(pData + offest);
        offest += sizeof(stuSimpleSymbol_ZYQ);
        STUReportDataDetailResp_ZYQ* stuReportDataDetailResp_ZYQ = (STUReportDataDetailResp_ZYQ*)(pData + offest);
        STUNowDataVolPirce* stuNowDataVolPirce = NULL;
        if(stuReportDataDetailResp_ZYQ->m_usVolNum>0){
            offest += sizeof(STUReportDataDetailResp_ZYQ);
            stuNowDataVolPirce = (STUNowDataVolPirce*)(pData + offest);
            ZYJSymbolInfoArray::iterator it = zyjSymbolInfo.find(stuSimpleSymbol_ZYQ->m_szSymbolCode);
            if(it != zyjSymbolInfo.end()) {
                ZYJSTUSymbolTradeInfo& info = it->second;
                info.stuRealPublicDataPush.m_iPriceRiseDown = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_iPriceRiseDown;
                info.stuRealPublicDataPush.m_chTurnoverDirec = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_chTurnoverDirec;
                info.stuRealPublicDataPush.m_iPriceRiseDownRate = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_iPriceRiseDownRate;
                info.stuRealPublicDataPush.m_iTotalTurnoverFunds = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_iTotalTurnoverFunds;
                info.stuRealPublicDataPush.m_uiNewQty = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_uiNewQty;
                info.stuRealPublicDataPush.m_uiTotalTurnoverQty = stuReportDataDetailResp_ZYQ->m_oReportPublicData.m_uiTotalTurnoverQty;
                info.stuRealDataDetailPush_Ext.m_uiPOrderBuyQty = stuReportDataDetailResp_ZYQ->m_oExt.m_uiPOrderBuyQty;
                info.stuRealDataDetailPush_Ext.m_uiPOrderSaleQty = stuReportDataDetailResp_ZYQ->m_oExt.m_uiPOrderSaleQty;
                info.stuRealDataDetailPush_Ext.m_iOrderDiff = stuReportDataDetailResp_ZYQ->m_oExt.m_iOrderDiff;
                memcpy(&(it->second.stuNowDataVolPirces), stuNowDataVolPirce, sizeof(STUNowDataVolPirce) * stuReportDataDetailResp_ZYQ->m_usVolNum);
                nRet = FIX_MSGTYPE_RES_OK;
            }
        }
        tradeuievents.on_quote_detail_report_notify(nSeq, nRet, stuSimpleSymbol_ZYQ->m_szSymbolCode);
    }
    
    int request_zyq_pankou_detail(char* symbolCode){
        CSymbolParam* symbol = getSymbolPraram(symbolCode);
        if(NULL == symbol) return -1;
        STUSimpleSymbol_ZYQ s;
        memset(&s, 0, sizeof(STUSimpleSymbol_ZYQ));
        strcpy(s.m_szSymbolCode, symbolCode);
        s.m_ucPriceUnit = symbol->nDecimal;
        s.m_usMarketType = atoi(symbol->ssSymbolgroupid.c_str());
        int pLen = sizeof(STUSimpleSymbol_ZYQ) ;
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&s, sizeof(STUSimpleSymbol_ZYQ));
        int nSeq = nhp_send_quote_req(REPORTDATA_DETAIL_ZYQ, pData, pLen);
        delete []  pData;
        return nSeq;
    }
    
    int get_zyq_symbol_trade_begin_end_time(char* symbolCode, int* pBeginTime, int* m_nEndTime) {
        if(zyjSymbolInfo.empty() || NULL == pBeginTime || NULL == m_nEndTime)  return 0;
        std::map<string, ZYJSTUSymbolTradeInfo>::iterator it = zyjSymbolInfo.find(symbolCode);
        if(it != zyjSymbolInfo.end()) {
            *pBeginTime = it->second.stuSymbolTradeTime[0].m_nBeginTime;
            *m_nEndTime = it->second.stuSymbolTradeTime[0].m_nEndTime;
            return 1;
        }
        return 0;
    }
    
    //初始化中远期市场
    void update_zyq_symbol_data_info(unsigned char* pData, int nCount)
    {
        zyjSymbolInfo.clear();
        int offest = 0;
        STUInitMarketLenDirec_ZYQ* stuMarketLenDirec_ZYQ = (STUInitMarketLenDirec_ZYQ*)(pData+offest);
        offest = sizeof(STUInitMarketLenDirec_ZYQ);
        std::map<int, STUMarketInfo_ZYQ> stuMarketInfos;
        STUMarketInfo_ZYQ* stuMarketInfo_ZYQ = NULL;
        if(stuMarketLenDirec_ZYQ->m_usMarkets>0) {
            stuMarketInfo_ZYQ = (STUMarketInfo_ZYQ*)&pData[offest];
            offest += sizeof(STUMarketInfo_ZYQ) * stuMarketLenDirec_ZYQ->m_usMarkets;
            if(offest > nCount) return;
            for(int i=0; i < stuMarketLenDirec_ZYQ->m_usMarkets;i++){
                stuMarketInfos.insert(make_pair(stuMarketInfo_ZYQ[i].m_usMarketType, stuMarketInfo_ZYQ[i]));
            }
        }
        
        vector<STUSymbolTradeTime_ZYQ> symbolTimes;
        if(stuMarketLenDirec_ZYQ->m_usTradeTimeNum>0) {
            STUSymbolTradeTime_ZYQ* stuSymbolTradeTime_ZYQ = (STUSymbolTradeTime_ZYQ*)&pData[offest];
            offest += sizeof(STUSymbolTradeTime_ZYQ) * stuMarketLenDirec_ZYQ->m_usTradeTimeNum;
            if(offest > nCount) return;
            for(int i=0; i < stuMarketLenDirec_ZYQ->m_usTradeTimeNum;i++){
                STUSymbolTradeTime_ZYQ stuSymbolTradeTime;
                memset(&stuSymbolTradeTime, 0, sizeof(stuSymbolTradeTime));
                stuSymbolTradeTime.m_nBeginTime = stuSymbolTradeTime_ZYQ[i].m_nBeginTime;
                stuSymbolTradeTime.m_ucTheDay = stuSymbolTradeTime_ZYQ[i].m_ucTheDay;
                stuSymbolTradeTime.m_nEndTime = stuSymbolTradeTime_ZYQ[i].m_nEndTime;
                symbolTimes.push_back(stuSymbolTradeTime);
            }
        }
        STUSymbolInfo_ZYQ* stuSymbolInfo_ZYQ = NULL;
        if(stuMarketLenDirec_ZYQ->m_usSymboNum >0) {
            stuSymbolInfo_ZYQ = (STUSymbolInfo_ZYQ*)&pData[offest];
            offest += sizeof(STUSymbolInfo_ZYQ) * stuMarketLenDirec_ZYQ->m_usSymboNum;
            if(offest > nCount) return;
            for(int i = 0; i < stuMarketLenDirec_ZYQ->m_usSymboNum; i++) {
                ZYJSTUSymbolTradeInfo __time;
                memset(&__time, 0, sizeof(ZYJSTUSymbolTradeInfo));
                memcpy(&(__time.stuSymbolInfo), &stuSymbolInfo_ZYQ[i], sizeof(STUSymbolInfo_ZYQ));
                std::map<int, STUMarketInfo_ZYQ>::iterator it = stuMarketInfos.find(stuSymbolInfo_ZYQ[i].m_usMarketType);
                if(it != stuMarketInfos.end()) {
                    memcpy(&(__time.stuMarketInfo), &(it->second), sizeof(STUMarketInfo_ZYQ));
                }
                int pSize = 32;
                char ps[pSize];
                itobs(stuSymbolInfo_ZYQ[i].m_usTradeTimeIndex, ps);
                for(int i = 0; i < stuMarketLenDirec_ZYQ->m_usTradeTimeNum; i++) {
                    if(ps[pSize-(i+1)] == '1') {
                        STUSymbolTradeTime_ZYQ tradeTime = symbolTimes[i];
                        memcpy(&(__time.stuSymbolTradeTime[__time.m_lenDirec]), &tradeTime, sizeof(STUSymbolTradeTime_ZYQ));
                        __time.m_lenDirec += 1;
                    }
                }
                zyjSymbolInfo.insert(make_pair(stuSymbolInfo_ZYQ[i].m_szSymbolCode, __time));
                //                int pBeginTime, m_nEndTime;
                //                get_zyq_symbol_trade_begin_end_time( stuSymbolInfo_ZYQ[i].m_szSymbolCode, &pBeginTime, &m_nEndTime);
            }
        }
        request_all_zyq__quote_pankou();
    }
    
    //实时行情
    void handler_zyj_quote_real_rsp(const char * pData, int nLen) {
        char symbolcodes[128];
        memset(symbolcodes, 0, sizeof(symbolcodes));
        int offest = 0;
        STUSimpleSymbol_ZYQ* pSymbol = (STUSimpleSymbol_ZYQ*)pData;
        offest += sizeof(STUSimpleSymbol_ZYQ);
        STURealDataPush_ZYQ* pDataPush = (STURealDataPush_ZYQ*)(pData + offest);
        string symbolcode = pSymbol->m_szSymbolCode;
        SymbolContainer::iterator iter = symbolContainer.find(symbolcode);
        if (iter != symbolContainer.end()) {
            if(strlen(symbolcodes) > 0) strcat(symbolcodes, ",");
            strcat(symbolcodes, pSymbol->m_szSymbolCode);
            int decimal = (int)pSymbol->m_ucPriceUnit;
            double weishu = pow(10.0, decimal);
            double price = pDataPush->m_oRealPublicData.m_uiNewPrice / weishu;
            if (price < 0.0001) return;
            long long datetime = pDataPush->m_oTDateTime.m_iUpdateDataTime;
            double ask = pDataPush->m_oQtyPriceV1.m_buyVol.m_uiPrice / weishu;
            double bid = pDataPush->m_oQtyPriceV1.m_sellVol.m_uiPrice / weishu;
            CSymbolParam * symbol = iter->second;
            symbol->lastTick = datetime;
            setpricedirection(symbol->fYesterdayClosePrice, ask , &symbol->nCurPriceDirection);
            symbol->flastPrice = symbol->fCurPrice; //设置上一口价格
            symbol->fCurPrice = price;
            symbol->fAskPrice = ask;//price + getrealtips(symbol->dbBuypips, symbol->nDecimal);
            symbol->fBidPrice = bid;//price + getrealtips(symbol->dbSellpips, symbol->nDecimal);
            if (symbol->fMaxPrice < price ) {
                symbol->fMaxPrice = price;
            }
            if (symbol->fMinPrice > price) {
                symbol->fMinPrice = price;
            }
            symbol->fClosePrice = price;
            
            //更新kline数据
            //            memcpy(&(symbol->stuRealPublicDataPush), &(pDataPush->m_oRealPublicData), sizeof(STURealPublicDataPush));
            //            memcpy(&(symbol->stuNowDataVolPirces), &(pDataPush->m_oQtyPriceV1), sizeof(STUNowDataVolPirce));
        }
        ZYJSymbolInfoArray::iterator it = zyjSymbolInfo.find(symbolcode);
        if(it != zyjSymbolInfo.end()) {
            memcpy(&(it->second.stuRealPublicDataPush), &(pDataPush->m_oRealPublicData), sizeof(STURealPublicDataPush));
            memcpy(&(it->second.stuNowDataVolPirces), &(pDataPush->m_oQtyPriceV1), sizeof(STUNowDataVolPirce));
        }
        if(strlen(symbolcodes) > 0) {
            //更新浮动盈亏
            updateFPL();
            quote_change_end(1, symbolcodes);
            printBusLogEx("zyj_quote_real_rsp end %s\n", symbolcodes);
        }
    }
    
    int request_zyq_kline(char* symbolCode, int ktype, int hasNum, int reqCount){
        CSymbolParam* symbol = getSymbolPraram(symbolCode);
        if(NULL == symbol) return -1;
        STUSimpleSymbol_ZYQ s;
        strcpy(s.m_szSymbolCode, symbolCode);
        s.m_ucPriceUnit = symbol->nDecimal;
        s.m_usMarketType = atoi(symbol->ssSymbolgroupid.c_str());
        STUHisKIndexDirec klineAttr;
        klineAttr.m_iHasNum =hasNum;
        klineAttr.m_iReqNum = reqCount;
        klineAttr.m_ucHisType = ktype;
        
        int pLen = sizeof(STUSimpleSymbol_ZYQ) + sizeof(STUHisKIndexDirec);
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&s, sizeof(STUSimpleSymbol_ZYQ));
        memcpy(pData + sizeof(STUSimpleSymbol_ZYQ), (const void *)&klineAttr, sizeof(STUHisKIndexDirec));
        int nSeq = nhp_send_quote_req(HISKDATA_ZYQ, pData, pLen);
        delete []  pData;
        addSeq(nSeq, ktype, symbolCode);
        return nSeq;
    }
    
    void handler_zyq_min_data_recv_end(int nSeq, const char* pData, int nLen) {
        int offest = 0;
        STUSimpleSymbol_ZYQ* stuSimpleSymbol = (STUSimpleSymbol_ZYQ*)pData;
        offest += sizeof(STUSimpleSymbol_ZYQ);
        STUCommLenDirec* stuCommLenDirec = (STUCommLenDirec*)(pData+offest);
        offest += sizeof(STUCommLenDirec);
        CSymbolParam* param = getSymbolPraram(stuSimpleSymbol->m_szSymbolCode);
        if(NULL == param) return;
        if(stuCommLenDirec->m_lenDirec>0) {
            STUSTKMinUnit_ZYQ* stuSTKMinUnits =  (STUSTKMinUnit_ZYQ*)(pData+offest);
            ZYQMinUnitArrayContainer::iterator it = zyqMinUnitArrayContainer.find(stuSimpleSymbol->m_szSymbolCode);
            vector<STUSTKMinUnit_ZYQ> arrs;
            if(it != zyqMinUnitArrayContainer.end()) zyqMinUnitArrayContainer.erase(stuSimpleSymbol->m_szSymbolCode);
            for(int i = 0; i < stuCommLenDirec->m_lenDirec; i++) {
                STUSTKMinUnit_ZYQ stuMinUnit;
                memset(&stuMinUnit, 0, sizeof(STUSTKMinUnit_ZYQ));
                stuMinUnit.m_uiTrunoverQty = stuSTKMinUnits[i].m_uiTrunoverQty;
                stuMinUnit.m_uiTrunoverFunds = stuSTKMinUnits[i].m_uiTrunoverFunds;
                stuMinUnit.m_uiTurnoverPrice = stuSTKMinUnits[i].m_uiTurnoverPrice;
                stuMinUnit.m_oTDateTime.m_iTradeDay = stuSTKMinUnits[i].m_oTDateTime.m_iTradeDay;
                stuMinUnit.m_oTDateTime.m_iUpdateDataTime = stuSTKMinUnits[i].m_oTDateTime.m_iUpdateDataTime;
                arrs.push_back(stuMinUnit);
            }
            zyqMinUnitArrayContainer.insert(make_pair(stuSimpleSymbol->m_szSymbolCode, arrs));
        }
        tradeuievents.on_quote_minute_data_notify(nSeq, stuSimpleSymbol->m_szSymbolCode);
    }
    int getSymbolMinData_ZYQ(const char* symbol, STUSTKMinUnit_ZYQ* stuSTKMinUnit_ZYQ, int iSize){
        int i = 0;
        string strSymbol = symbol;
        ZYQMinUnitArrayContainer::iterator itorder;
        for (itorder = zyqMinUnitArrayContainer.begin(); itorder !=zyqMinUnitArrayContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(symbol)) {
                for(i = 0; i < itorder->second.size(); i++) {
                    if(i >= iSize) return i;
                    STUSTKMinUnit_ZYQ unit = itorder->second[i];
                    stuSTKMinUnit_ZYQ[i].m_uiTrunoverQty = unit.m_uiTrunoverQty;
                    stuSTKMinUnit_ZYQ[i].m_uiTrunoverFunds = unit.m_uiTrunoverFunds;
                    stuSTKMinUnit_ZYQ[i].m_uiTurnoverPrice = unit.m_uiTurnoverPrice;
                    stuSTKMinUnit_ZYQ[i].m_oTDateTime.m_iTradeDay = unit.m_oTDateTime.m_iTradeDay;
                    stuSTKMinUnit_ZYQ[i].m_oTDateTime.m_iUpdateDataTime = unit.m_oTDateTime.m_iUpdateDataTime;
                }
            }
        }
        return i;
    }
    // 分时
    int request_zyq_min_data(char* symbolCode, int nSize){
        CSymbolParam* symbol = getSymbolPraram(symbolCode);
        if(NULL == symbol) return -1;
        STUSimpleSymbol_ZYQ s;
        memset(&s, 0, sizeof(STUSimpleSymbol_ZYQ));
        strcpy(s.m_szSymbolCode, symbolCode);
        s.m_ucPriceUnit = symbol->nDecimal;
        s.m_usMarketType = atoi(symbol->ssSymbolgroupid.c_str());
        
        int pLen = sizeof(STUSimpleSymbol_ZYQ) ;
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&s, sizeof(STUSimpleSymbol_ZYQ));
        int nSeq = nhp_send_quote_req(STKMINUTEDATA_ZYQ, pData, pLen);
        delete []  pData;
        return nSeq;
    }
    
    void handler_zyq_trace_data_recv_end(int nSeq, const char* pData, int nLen) {
        int offest = 0;
        STUSimpleSymbol_ZYQ* stuSimpleSymbol = (STUSimpleSymbol_ZYQ*)pData;
        offest += sizeof(STUSimpleSymbol_ZYQ);
        STUCommLenDirec* stuCommLenDirec = (STUCommLenDirec*)(pData+offest);
        offest += sizeof(STUCommLenDirec);
        if(stuCommLenDirec->m_lenDirec>0) {
            STUTraceUnit_ZYQ* stuTraceUnits =  (STUTraceUnit_ZYQ*)(pData+offest);
            ZYQTraceUnitArrayContainer::iterator it = zyqTraceUnitArrayContainer.find(stuSimpleSymbol->m_szSymbolCode);
            vector<STUTraceUnit_ZYQ> arrs;
            if(it != zyqTraceUnitArrayContainer.end()) zyqTraceUnitArrayContainer.erase(stuSimpleSymbol->m_szSymbolCode);
            for(int i = 0; i < stuCommLenDirec->m_lenDirec; i++) {
                STUTraceUnit_ZYQ stuTraceUnit;
                stuTraceUnit.m_uiPrice = stuTraceUnits[i].m_uiPrice;
                stuTraceUnit.m_uiQty = stuTraceUnits[i].m_uiQty;
                stuTraceUnit.m_chTurnoverDirec = stuTraceUnits[i].m_chTurnoverDirec;
                stuTraceUnit.m_oTDateTime.m_iTradeDay = stuTraceUnits[i].m_oTDateTime.m_iTradeDay;
                stuTraceUnit.m_oTDateTime.m_iUpdateDataTime = stuTraceUnits[i].m_oTDateTime.m_iUpdateDataTime;
                arrs.push_back(stuTraceUnit);
            }
            zyqTraceUnitArrayContainer.insert(make_pair(stuSimpleSymbol->m_szSymbolCode, arrs));
        }
        tradeuievents.on_quote_trace_data_notify(nSeq, stuSimpleSymbol->m_szSymbolCode);
    }
    int getSymbolTraceData_ZYQ(const char* symbol, STUTraceUnit_ZYQ* stuTraceUnit_ZYQ, int iSize){
        int i = 0;
        string strSymbol = symbol;
        ZYQTraceUnitArrayContainer::iterator itorder;
        for (itorder = zyqTraceUnitArrayContainer.begin(); itorder !=zyqTraceUnitArrayContainer.end() ; itorder++) {
            string str = itorder->first;
            if(0 == str.compare(symbol)) {
                for(i = 0; i < itorder->second.size(); i++) {
                    STUTraceUnit_ZYQ stuTraceUnit = itorder->second[i];
                    stuTraceUnit_ZYQ[i].m_uiPrice = stuTraceUnit.m_uiPrice;
                    stuTraceUnit_ZYQ[i].m_uiQty = stuTraceUnit.m_uiQty;
                    stuTraceUnit_ZYQ[i].m_chTurnoverDirec = stuTraceUnit.m_chTurnoverDirec;
                    stuTraceUnit_ZYQ[i].m_oTDateTime.m_iTradeDay = stuTraceUnit.m_oTDateTime.m_iTradeDay;
                    stuTraceUnit_ZYQ[i].m_oTDateTime.m_iUpdateDataTime = stuTraceUnit.m_oTDateTime.m_iUpdateDataTime;
                }
            }
        }
        return i;
    }
    // 成交明细
    int request_zyq_trace_data(char* symbolCode, int nSize){
        CSymbolParam* symbol = getSymbolPraram(symbolCode);
        if(NULL == symbol) return -1;
        STUSimpleSymbol_ZYQ s;
        memset(&s, 0, sizeof(STUSimpleSymbol_ZYQ));
        strcpy(s.m_szSymbolCode, symbolCode);
        s.m_ucPriceUnit = symbol->nDecimal;
        s.m_usMarketType = atoi(symbol->ssSymbolgroupid.c_str());
        
        STUCommLenDirec len;
        len.m_lenDirec=nSize;
        int pLen = sizeof(STUSimpleSymbol_ZYQ) + sizeof(STUCommLenDirec);
        char* pData = new char[pLen];
        memset(pData, 0, pLen);
        memcpy(pData, (const void *)&s, sizeof(STUSimpleSymbol_ZYQ));
        memcpy(pData, (const void *)&len, sizeof(STUCommLenDirec));
        int nSeq = nhp_send_quote_req(STKTRACEDATA_ZYQ, pData, pLen);
        delete []  pData;
        return nSeq;
    }
    
    
    void handler_zyj_quote_real_detail(const char* pData, int nLen) {
        printBusLogEx("handler_zyj_quote_real_detail ");
        int offest = 0;
        STUSimpleSymbol_ZYQ* stuSimpleSymbol = (STUSimpleSymbol_ZYQ*)(pData + offest);
        offest += sizeof(STUSimpleSymbol_ZYQ);
        STURealDataDetailPush_ZYQ* stuRealDataDetailPush = (STURealDataDetailPush_ZYQ*)(pData + offest);
        offest += sizeof(STURealDataDetailPush_ZYQ);
        if(stuRealDataDetailPush->m_usVolNum > 0) {
            STUNowDataVolPirce* stuNowDataVolPirces = (STUNowDataVolPirce*)(pData + offest);
            //            SymbolContainer::iterator iter = symbolContainer.find(stuSimpleSymbol->m_szSymbolCode);
            //            if (iter != symbolContainer.end()) {
            //                CSymbolParam * symbol = iter->second;
            //                memcpy(&(symbol->stuNowDataVolPirces), stuNowDataVolPirces, sizeof(STUNowDataVolPirce) * stuRealDataDetailPush->m_usVolNum);
            //            }
            ZYJSymbolInfoArray::iterator it = zyjSymbolInfo.find(stuSimpleSymbol->m_szSymbolCode);
            if(it != zyjSymbolInfo.end()) {
                ZYJSTUSymbolTradeInfo& unit = it->second;
                memcpy(&(unit.stuNowDataVolPirces), stuNowDataVolPirces, sizeof(STUNowDataVolPirce) * stuRealDataDetailPush->m_usVolNum);
            }
        }
        tradeuievents.on_quote_real_detail_notify(stuSimpleSymbol->m_szSymbolCode);
    }
    
    void handler_zyj_complex_ranking(int nSeq, const char* pData, int nLen) {
        printBusLogEx("handler_zyj_complex_ranking ");
        int offest = 0;
        for(;;) {
            STUComplexRankingHead_ZYQ* stuComplexRanking = (STUComplexRankingHead_ZYQ*)(pData+offest);
            offest += sizeof(STUComplexRankingHead_ZYQ);
            if(nLen < offest) break;
            int nSize = stuComplexRanking->m_usRankinkDataNum;
            STUComplexRankingData_ZYQ* stuComplexRankingDatas = (STUComplexRankingData_ZYQ*)(pData+offest);
            offest += sizeof(STUComplexRankingData_ZYQ) * nSize;
            for(int i = 0;i < nSize; i++) {
                STUComplexRankingData_ZYQ stuComplexRankingData;
                stuComplexRankingData.m_usNewPrice =  stuComplexRankingDatas[i].m_usNewPrice;
            }
        }
        tradeuievents.on_quote_complex_ranking_notify(nSeq);
    }
    
    int request_zyq_complex_ranking(){
        return nhp_send_quote_req(COMPLEX_RANKING_ZYQ, NULL, 0);
    }
    
    //开休市
    void handler_openclose_market(int type, int status) {
        printBusLogEx("开休市 type=%d status=%d", type, status);
    }
    
    int updateZYQKLine(ZYQKLineDataGroup* pDesGroup, ZYQKLineDataGroup* pSrcGroup) {
        vector<STUHisKUnit_ZYQ>::iterator element;
        int lastCompareRes = -1;
        for(element = pSrcGroup->m_arrKLineData.begin(); element != pSrcGroup->m_arrKLineData.end(); element ++) {
            int i = 0;
            STUHisKUnit_ZYQ oUnit1 = *element;
            vector<STUHisKUnit_ZYQ>::iterator element2;
            for(element2 = pDesGroup->m_arrKLineData.begin(); element2 != pDesGroup->m_arrKLineData.end(); element2 ++, i ++) {
                STUHisKUnit_ZYQ oUnit2 = *element2;
                long long nDesDateTime = oUnit2.m_oTDateTime.m_iUpdateDataTime;
                long long nSrcDateTime = oUnit1.m_oTDateTime.m_iUpdateDataTime;
                if(nDesDateTime == nSrcDateTime) {
                    lastCompareRes = 0;
                    oUnit2.m_uiClosePrice = oUnit1.m_uiClosePrice;
                    oUnit2.m_uiMaxPrice = oUnit1.m_uiMaxPrice;
                    oUnit2.m_uiMinPrice = oUnit1.m_uiMinPrice;
                    oUnit2.m_uiOpenPrice = oUnit1.m_uiOpenPrice;
                    oUnit2.m_uiTrunoverQty = oUnit1.m_uiTrunoverQty;
                    oUnit2.m_oTDateTime.m_iTradeDay = oUnit1.m_oTDateTime.m_iTradeDay;
                    oUnit2.m_oTDateTime.m_iUpdateDataTime = oUnit1.m_oTDateTime.m_iUpdateDataTime;
                    break;
                } else if(nDesDateTime < nSrcDateTime) {//当前值比添加值时间小 继续查找
                    lastCompareRes = -1;
                } else if(nDesDateTime > nSrcDateTime) {//当前值比添加值时间大 直接插入
                    if(-1 == lastCompareRes) {//有跳过一条数据插入这条数据
                        lastCompareRes = 0;
                        //对于最尾处 先括容量再插入需要
                        if(element == pSrcGroup->m_arrKLineData.end()) {
                            pDesGroup->m_arrKLineData.push_back(oUnit2);
                        }
                        pDesGroup->m_arrKLineData.insert(element2, oUnit1);
                        break;
                    }
                }
            }
            if(-1 == lastCompareRes)//一直比当前数组里值大 添加到最尾
            {
                pDesGroup->m_arrKLineData.push_back(oUnit1);
            }
        }
        return 1;
    }
    
    void handler_zyq_kline_data_recv_end(int nSeq, const char* pData, int nLen) {
        char name[30];
        int nCurType = 0;
        if(getSeq(nSeq, nCurType, name)) {
            int offest = 0;
            STUSimpleSymbol_ZYQ* symbol = (STUSimpleSymbol_ZYQ*)pData;
            offest += sizeof(STUSimpleSymbol_ZYQ);
            STUCommLenDirec* lenDirec = (STUCommLenDirec*)(pData + offest);
            offest += sizeof(STUCommLenDirec);
            int size = lenDirec->m_lenDirec;
            if(strcmp(symbol->m_szSymbolCode, name) == 0 && nLen >= offest + sizeof(STUHisKUnit_ZYQ) * size) {
                STUHisKUnit_ZYQ* hisKunits = (STUHisKUnit_ZYQ*)(pData + offest);
                ZYQKLineDataGroup pGroup;
                vector<STUHisKUnit_ZYQ> arrKUnit;
                for(int i = 0; i < size; i++) {
                    STUHisKUnit_ZYQ hisKunit;
                    memset(&hisKunit, 0, sizeof(STUHisKUnit_ZYQ));
                    hisKunit.m_uiClosePrice = hisKunits[i].m_uiClosePrice;
                    hisKunit.m_uiMaxPrice = hisKunits[i].m_uiMaxPrice;
                    hisKunit.m_uiMinPrice = hisKunits[i].m_uiMinPrice;
                    hisKunit.m_uiOpenPrice = hisKunits[i].m_uiOpenPrice;
                    hisKunit.m_uiTrunoverQty = hisKunits[i].m_uiTrunoverQty;
                    hisKunit.m_oTDateTime.m_iTradeDay = hisKunits[i].m_oTDateTime.m_iTradeDay;
                    hisKunit.m_oTDateTime.m_iUpdateDataTime = hisKunits[i].m_oTDateTime.m_iUpdateDataTime;
                    //                    printBusLogEx("back open= [%d]  close= [%d]", hisKunits[i].m_uiOpenPrice, hisKunits[i].m_uiClosePrice);
                    arrKUnit.push_back(hisKunit);
                }
                pGroup.m_arrKLineData = arrKUnit;
                pGroup.m_nKineDataType = nCurType;
                //查找替换
                ZYQKLineDataContainer::iterator it = zyqKLineDataContainer.find(name);
                if(it != zyqKLineDataContainer.end()){
                    ZYQKLineDataGroupContainer subChild = it->second;
                    ZYQKLineDataGroupContainer::iterator gc_it = subChild.find(nCurType);
                    if(gc_it != subChild.end()){
                        ZYQKLineDataGroup pCurGroup = gc_it->second;
                        if(pCurGroup.m_nKineDataType == pGroup.m_nKineDataType) {
                            updateZYQKLine(&pCurGroup, &pGroup);
                        }
                    } else {
                        subChild.insert(make_pair(nCurType, pGroup));
                        it->second = subChild;
                    }
                } else {
                    ZYQKLineDataGroupContainer groupContainer;
                    groupContainer.insert(make_pair(nCurType, pGroup));
                    zyqKLineDataContainer.insert(make_pair(name, groupContainer));
                }
            }
        }
        tradeuievents.on_quote_history_data_notify(nSeq, nCurType, name);
        printBusLogEx("handler_zyq_kline_data_recv_end name=%s len=%d nType=%d\n", name, nSeq, nCurType);
    }
    
    //K线
    int send_quote_his_data_req(const char* szCode, int nPeriodicType, int hasNum, int nReqCount){
        int type = getSymbolType((char*)szCode);
        type = ZS_TYPE;//ex
        if(type == ZS_TYPE) {
            return request_zs_kline(szCode, nPeriodicType, hasNum, nReqCount);
        } else if(type == ZYJ_TYPE) {
            return request_zyq_kline((char*)szCode, nPeriodicType, hasNum, nReqCount);
        }
        return 0;
    }
    //分笔
    int send_quote_STK_trace_data_req(const char* szCode, int nSize) {
        int type = getSymbolType((char*)szCode);
        if(type == ZS_TYPE) {
            return request_zs_trace_data((char*)szCode, nSize);
        } else if(type == ZYJ_TYPE) {
            return request_zyq_trace_data((char*)szCode, nSize);
        }
        return 0;
    }
    //分时
    int send_quote_STK_min_data_req(const char* szCode, int nSize){
        int type = getSymbolType((char*)szCode);
        if(type == ZS_TYPE) {
            return request_zs_min_data((char*)szCode, nSize);
        } else if(type == ZYJ_TYPE) {
            return request_zyq_min_data((char*)szCode, nSize);
        }
        return 0;
    }
    int trade_request_inout_money(int transferType,double money,const char * moneyPwd,const char *bankPwd)
    {
        if(money <= gEPSINON) return CHECK_PARAMS_FAIL;
        cJSON *root= getcJsonRoot();
        char *out;
        char sstmp[1024];
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%d", transferType);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_DIRECTION, sstmp); // 操作类型 I / O
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%.2f", money);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_MONEY, sstmp); // 转账金额
        
        memset(sstmp, 0, sizeof(sstmp));
        if(Telephone[0] != 0 ) sprintf(sstmp, "%s", Telephone);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_TEL, sstmp); // 电话号码
        
        memset(sstmp, 0, sizeof(sstmp));
        if(BankName[0] != 0) sprintf(sstmp, "%s",BankName);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKNAME, sstmp);  // 开户行
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", BankCardNumber);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKCARDNO, sstmp);  // 开户账户 -- 
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s",Identifier);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_IDCARD, sstmp); // 身份证号
        
        char hash[50];
        memset(hash, 0, sizeof(hash));
        if(NULL != moneyPwd) getSHA1(hash, (unsigned char*)moneyPwd, (u_int32_t)strlen(moneyPwd));
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", hash);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_ACCTPWD, sstmp); // 转账密码（资金密码）
        
        memset(sstmp, 0, sizeof(sstmp));
        sprintf(sstmp, "%s", bankPwd);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_BANKPWD, sstmp);   // 银行密码
        
        out=cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_inout_monery json:%s", out);
        int nSeq = nhp_trade_inout_monery(out);
        free(out);
        printBusLogEx("trade_inout_monery end");
        return nSeq;
    }
    
    void handler_sign_status_notice(int nSeq, const char* json) {
        int nRet = -1;
        cJSON *root = cJSON_Parse(json);
        int t = SignStatus;
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                CMoneryInOutRes* res = new CMoneryInOutRes;
                convert_monery_inout_info_from_jsonnode(root, (const char*)res);
                cJSON* szSignStatus = cJSON_GetObjectItemEx(root, FIX_TAG_BODY_BANKSIGNSTATUS);
                if(NULL != szSignStatus) SignStatus = atoi(szSignStatus->valuestring);
            }
        }
        cJSON_Delete(root);
        if(t != SignStatus) tradeuievents.on_sign_change_notify(SignStatus);
    }
    
    void handler_inout_money_rsp(int nSeq, const char* json) {
        int nRet = -1;
        cJSON *root = cJSON_Parse(json);
        if (root) {
            cJSON *retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == 0) {
                if(nRet == 0) trade_query_inout_monery();
            }
        }
        cJSON_Delete(root);
        tradeuievents.on_inout_money_rsp(nSeq,nRet,json);
    }
    
    /***********************************
                文交所
     **********************************/
    
    
    int trade_request_symbol()
    {
        // 取缓存
//        if (productList != NULL)
//        {
//            tradeuievents.trade_ui_symbol_change_notice(TRADE_MSGTYPE_RES_OK, productList);
//        }
        
        cJSON *root;
        char *out;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        printBusLogEx("trade_request_symbol LoginedSession = %s",LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        int nSeq = nhp_trade_request_symbol(out);
        free(out);
        return nSeq;
    }
    
    
    int trade_request_today_entrust()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_today_entrust start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_today_entrust json:%s", out);
        int nSeq = nhp_trade_today_entrust(out);
        free(out);
        printBusLogEx("trade_request_today_entrust end");
        return nSeq;
    }
    
    int trade_request_today_deal()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_today_deal start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_today_deal json:%s", out);
        int nSeq = nhp_trade_today_deal(out);
        free(out);
        printBusLogEx("trade_request_today_deal end");
        return nSeq;
    }
    
    int trade_request_cancel_order(char * orderID)
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_cancel_order start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERID, orderID);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_cancel_order json:%s", out);
        int nSeq = nhp_trade_cancel_order(out);
        free(out);
        printBusLogEx("trade_request_cancel_order end");
        return nSeq;
    }
    
    int trade_request_sharehold()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_sharehold start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_sharehold json:%s", out);
        int nSeq = nhp_trade_sharehold_rsp(out);
        free(out);
        printBusLogEx("trade_request_sharehold end");
        return nSeq;
    }
    
    int trade_request_moneyhold()
    {
        cJSON *root;
        char *out;
        printBusLogEx("trade_request_moneyhold start");
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ACCTID, LoginedAccountID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_moneyhold json:%s", out);
        int nSeq = nhp_trade_moneyhold_req(out);
        free(out);
        printBusLogEx("trade_request_moneyhold end");
        return nSeq;
    }
    
    void trade_request_today_entrust_rsp(char * json)
    {
        tradeuievents.trade_ui_today_entrust_rsp(json);
        
    }
    
    void trade_request_today_deal_rsp( char * json)
    {
        tradeuievents.trade_ui_today_deal_rsp(json);
    }
    
    void trade_request_sharehold_rsp(char * json)
    {
        cJSON *root;
        cJSON *retcode;
        cJSON *retinfo;
        string orderprice;
        printBusLogEx("trade_request_sharehold_rsp json:%s",json);
        root=cJSON_Parse(json);
        int nRet=-1;
        if (root)
        {
            retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
            retinfo = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSINFO);
            if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK)
            {
                string key = "SHAREHOLD";
                cJSON *value = cJSON_GetObjectItem(root, key.c_str());//获取orderkey对应的value
                char *data = cJSON_Print(value);
                if(NULL == data) return;
                tradeuievents.trade_ui_sharehold_rsp(nRet,data);
            }
        }
    }
    
    void trade_historyorder_rsp(int nSeq,char *json)
    {
        tradeuievents.trade_ui_historyorder_rsp(nSeq,json);
    }
    
    void trade_historyturnover_rsp(int nSeq,char *json)
    {
        tradeuievents.trade_ui_historyturnover_rsp(nSeq,json);
    }
    
    void trade_pretrade_rsp(int nSeq,char *json)
    {
        tradeuievents.trade_ui_canceltrade_rsp(nSeq,json);
    }
    
    void trade_canceltrade_rsp(int nSeq,char *json)
    {
        tradeuievents.trade_ui_canceltrade_rsp(nSeq,json);
    }
    
    void queto_hisKDataFirst_rsp(int nCmd ,unsigned char* data,int nLen)
    {
        tradeuievents.quote_ui_hisKDataFirst_rsp(data,nLen);
    }
    
    void quote_hisKDataCurDate_rsp(int nCmd ,unsigned char* data,int nLen)
    {
        tradeuievents.quote_ui_hisKDataCurDate_rsp(data,nLen);
    }
    void quote_stkDealDate_rsp(unsigned char* data)
    {
        tradeuievents.quote_ui_stkDealData_rsp(data);
    }
    
    //时间结构
    struct	SPhase
    {
        unsigned	short	m_usOpenTime;//开市时间
        unsigned	short	m_usCloseTime;//闭市时间
    };
    /*
    struct	SStockInfo
    {
        char		m_acName[60];//名称
        char		m_acSpell[4];
        unsigned	int	m_uiCode;//代码
        unsigned	short	m_usMarketType;//市场标识
        unsigned	int	m_uiPrevClose;//昨收价
        unsigned	int 	m_ui5DayVol;//五日平均总手
        unsigned	int 	m_uiUpLimit;//涨停板限制,
        unsigned	int	m_uiDownLimit;//跌停板限制
        unsigned	int	m_uiShares;//流通股数
        unsigned int          m_uiReserves; // 库存量
        short		m_sAsset;//每股净资产
    };
    */
    struct	SMarketInfo
    {
        unsigned	short	m_usMarketType;// 请求市场标识
        unsigned    char    m_ucMarketName[13];//市场类型名，用于分类
        unsigned	int     m_uiTodayDate;	//今日日期
        unsigned	short	m_usStocks;		// 证券个数
        //SStockInfo*	m_pstStockInfo;	// 证券初始化信息
    };
    
    
    struct	SInitMarket
    {
        unsigned	short	m_usType;		//命令类型
        unsigned	short	m_usMarketType;	//市场类型
        unsigned	int	m_uiTodayDate;	//今日日期
        unsigned	char	m_ucPhases;		//交易时间段
        SPhase		m_pstPhase[3];
        unsigned	char	m_ucMarkets;	//市场个数
        //	SMarketInfo*	m_pstMarket;	//市场信息
    };
    
    void quote_initMarket_rsp(const char* pData, int pLen)
    {
//        printBusLogEx("市场商品列表 [TradeEngine layer] \n%s", pData);
        
        set_login_progress_rsp();  // 初始化市场应答
        printBusLogEx("初始化登陆:step:[%d] 初始化市场应答",nNowProgress);
        /*
        SInitMarket* pInitMarket = (SInitMarket*)pData;
        int nCount = pInitMarket->m_ucMarkets;
        if(nCount > 0 && pInitMarket) m_vecStockInfo.clear();
        
        int pos = sizeof(SInitMarket);
        for (int n = 0; n < nCount; n ++) {
            pos += sizeof(SMarketInfo)*n;
            SMarketInfo* pMarketInfo = (SMarketInfo*)(pData + pos);
            for (int i = 0; i < pMarketInfo->m_usStocks; i++) {
                pos += sizeof(SStockInfo)*i;
                SStockInfo *pStockInfo; //= (SStockInfo)(pData + pos);
                memcpy(pStockInfo, pData + pos, sizeof(SStockInfo));
                m_vecStockInfo.push_back(*pStockInfo);
            }
        }
         */
        tradeuievents.quote_ui_initMarket_rsp(pData,pLen);
    }
    
    char* get_initmarket_productName(int symbolCode) {
        if(m_vecStockInfo.empty())  return 0;
        StockInfoArray::iterator element;
        for(element = m_vecStockInfo.begin(); element != m_vecStockInfo.end(); element ++) {
            SStockInfo oData = *element;
            if (oData.m_uiCode == symbolCode) {
                char *name = oData.m_acName;
                return name;
            }
        }
        return 0;
    }
    
    int get_initmarket_preclose(int symbolCode) {
        if(m_vecStockInfo.empty())  return 0;
        StockInfoArray::iterator element;
        for(element = m_vecStockInfo.begin(); element != m_vecStockInfo.end(); element ++) {
            SStockInfo oData = *element;
            if (oData.m_uiCode == symbolCode) {
                int preclose = oData.m_uiPrevClose;
                return preclose;
            }
        }
        return 0;
    }
    
    //TODO: OTC行情
    //盘面数据
    int quote_pankou_rsp(int nSeq, const char* pData, int pLen) {
        
        tradeuievents.quote_ui_pankou_rsp(pData,pLen);
        set_login_progress_rsp();  // 盘面数据应答
        printBusLogEx("初始化登陆:step:[%d] 盘面数据应答 ",nNowProgress);
        /*
        char symbolcodes[1024];
        memset(symbolcodes, 0, 1024);
        int iCount = pLen / sizeof(CSNowData);
        int iCnt = 0;
        for(int i =0;i < iCount;i++) {
            //每一笔数据u
            CSNowData* pNow = (CSNowData*)(pData + i * sizeof(CSNowData)); //sizeof(CSNowData)
            if (check_valid_quote_real_rsp(pNow->sCode.szCode) != 1)  continue;
            if(strlen(symbolcodes) > 0) strcat(symbolcodes, ",");
            strcat(symbolcodes, pNow->sCode.szCode);
            double weishu = pow(10.0, (int)pNow->sCode.cPriceUnit);
            char * symbolcode = pNow->sCode.szCode;
            double openprice = pNow->uiOpenPrice/weishu;
            double maxprice = pNow->uiMaxPrice/weishu;
            double minprice = pNow->uiMinPrice/weishu;
            double newprice = pNow->uiNewPrice/weishu;
            long long time = pNow->timeUpdate;
            SymbolContainer::iterator iter = symbolContainer.find(symbolcode);
            if (iter != symbolContainer.end()) {
                CSymbolParam * symbol = iter->second;
                symbol->fOpenPrice = openprice;
                symbol->fMaxPrice = maxprice;
                symbol->fMinPrice = minprice;
                //                if(symbol->flastPrice != symbol->fCurPrice)
                //                    symbol->flastPrice = symbol->fCurPrice; //设置上一口价格
                symbol->fCurPrice = newprice;
                symbol->fAskPrice = newprice +  getrealtips(symbol->dbBuypips, symbol->nDecimal);
                symbol->fBidPrice = newprice + getrealtips(symbol->dbSellpips, symbol->nDecimal);
                symbol->lastTick = time;
                setpricedirection(openprice, newprice, &symbol->nCurPriceDirection);
                printBusLogEx("quote_pankou_rsp %s  %f %f \n", symbolcode, maxprice, minprice);
                iCnt += 1;
            }
        }
        if(strlen(symbolcodes) > 0) {
            //更新浮动盈亏
            updateFPL();
            quote_change_end( iCount, symbolcodes);
        }
        return iCnt;
         */
        return 0;
    }
    
    int quote_kline_req(int klineType,char *productID)
    {
        return nhp_quote_kline_req(klineType, productID);
    }
    //个股分析
    int quote_stktrace_req(char *code)
    {
        return nhp_quote_stktrace_req(code);
    }
    
    int quote_reportData_req(char *codes)
    {
        return nhp_send_report_data_info_req(codes);
    }
    
    int trade_request_historyorder(char *startDate,char *endDate)
    {
        cJSON *root;
        char *out;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
//        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, "");
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_HISTORY_BEGINDATE, startDate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_HISTORY_ENDDATE, endDate);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_historyorder json:%s", out);
        int nSeq = nhp_trade_historyorder_req(out);
        free(out);
        return nSeq;
    }
    
    int trade_request_historyturnover(char *startDate,char *endDate)
    {
        cJSON *root;
        char *out;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
//        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, "");
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_HISTORY_BEGINDATE, startDate);
        cJSON_AddStringToObjectEx(root, FIX_TAG_INTPRTCL_HISTORY_ENDDATE, endDate);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_request_historyturnover json:%s", out);
        int nSeq = nhp_trade_historyturnover_req(out);
        free(out);
        return nSeq;
    }
    
    int trade_pretrade_req(char *securityID,char *orderType)
    {
        cJSON *root;
        char *out;
        root=cJSON_CreateObject();
        cJSON_AddStringToObjectEx(root, FIX_TAG_HEADER_SESSION, LoginedSession);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_SECURITYID, securityID);
        cJSON_AddStringToObjectEx(root, FIX_TAG_BODY_ORDERTYPE, orderType);
        out = cJSON_Print(root);
        cJSON_Delete(root);
        printBusLogEx("trade_pretrade_req json:%s", out);
        int nSeq = nhp_pretrade_req(out);
        free(out);
        return nSeq;
    }
    
    const char * getBankName()
    {
        return BankName;
    }
    
    void init_engine()
    {
        nhp_init();
        nhp_trade_events events;
        events.callback_trade_funds = &trade_request_funds;
        events.callback_trade_login = &trade_login;
        events.callback_trade_order = &trade_request_order;
        events.callback_trade_request_symbol = &trade_request_symbol;
        events.callback_get_symbol_num = &get_symbol_num;
        
        events.on_nhp_ca_kick_notify = &trade_kick_notify;
        events.on_nhp_trade_login_rsp = &trade_login_rsp;
        
        events.on_nhp_trade_open_rsp = &trade_open_order_rsp;
        events.on_nhp_trade_moneyhold_rsp = &trade_request_moneyhold_rsp;
        events.on_nhp_trade_order_rsp = &trade_request_order_rsp;
        events.on_nhp_trade_hisorder_rsp = &trade_request_hisorder_rsp;
        // culture
        events.on_nhp_quote_login_rsp = &quote_login_rsp;//行情服务登录应答
        events.on_nhp_quote_real = &quote_real_rsp;
        events.on_nhp_quote_change_end = &quote_change_end;
        events.callback_check_valid_quote_real_rsp = &check_valid_quote_real_rsp;
        
        events.on_nhp_quote_pankou = &quote_pankou_rsp;
        events.on_kline_data_recv_end = &quote_kline_data_recv_end;
        events.on_min_data_recv_end = &quote_min_data_recv_end;
        events.on_trace_data_recv_end = &quote_record_data_recv_end;
        
        events.on_nhp_quote_initMarket_rsp = &quote_initMarket_rsp;// update_otc_symbol_data_info
        events.on_zyq_symbol_data_info_end = &update_zyq_symbol_data_info;
        
        events.call_handler_inout_monery = &handler_inout_monery;
        events.call_handler_funds_flowing = &handler_funds_flowing;
        
        events.call_handler_inout_money_rsp = &handler_inout_money_rsp;
        
        /********************************2.0****************************************/
        
        events.call_handler_zyj_sumbit_order =  &handler_open_zyj_order_rsp;
        
        events.call_handler_zyj_symbol = &call_zyj_symbol_handler;
        
        //竞价查持仓单单请求
        events.call_handler_zyj_hold_order = &call_zyj_hold_order_handler;
        
        //竞价查委托单单请求
        events.call_handler_zyj_delegate_order = &call_zyj_delegate_order_handler;
        
        //竞价查成交单单请求
        events.call_handler_zyj_deal_order = &call_zyj_deal_order_handler;
        
        //交收单
        events.call_handler_zyj_settle_order = &call_zyj_settle_order_handler;
        
        //公告抬头查询
        events.call_handler_declare_title = &declare_title_rsp;
        
        //公告内容查询
        events.call_handler_declare_content = &declare_content_rsp;
        
        //公告rwy
        events.call_handler_declare_notify = &declare_notify;
        
        events.call_handler_modifypasswd = &handler_modifypasswd_rsp;
        
        //中远期
        events.call_handler_apply_delivery_intention = &handler_apply_delivery_intention;
        
        events.call_handler_query_apply_delivery_intention = &handler_query_apply_delivery_intention;
        
        events.call_trade_refresh = &handler_trade_refresh;
        
        events.call_handler_apply_delivery_sellte_rsp = &handler_apply_delivery_sellte_rsp;
        
        //仓库
        events.call_handler_sellte_notify = &handler_sellte_notify;
        events.call_handler_outstore_order_notify = &handler_outstore_order_notify;
        events.call_handler_apply_instore_notify = &handler_apply_instore_notify;
        events.call_handler_cang_order_notify = &handler_cang_order_notify;
        events.call_handler_apply_delivery_notify = &handler_apply_delivery_notify;
        events.call_handler_instore_order_notify = &handler_instore_order_notify;
        events.call_handler_cangorder_to_instore_notify = &handler_cangorder_to_instore_notify;
        
        events.call_handler_query_sellte_store_info = &handler_query_sellte_store_info_rsp;
        
        events.call_handler_query_in_store_goods_property = &handler_query_in_store_goods_property_rsp;
        
        events.call_handler_query_in_store_order = &handler_query_in_store_order_rsp;
        
        events.call_handler_query_apply_in_store = &handler_query_apply_in_store_rsp;
        
        events.call_handler_apply_in_store = &handler_apply_in_store_rsp;
        
        events.call_handler_query_cang = &handler_query_cang_rsp;
        
        events.call_handler_query_out_store_order = &handler_query_out_store_order_rsp;
        
        events.call_handler_query_cang_to_in_store_order = &handler_query_cang_to_in_store_order_rsp;
        
        events.call_handler_op_cang_to_in_store_order_response = &handler_op_cang_to_instore_order_response;
        events.call_handler_cang_to_in_store_order_response = &handler_cang_to_instore_order_response;
        
        events.call_handler_op_in_store_order_response = &handler_op_in_store_order_response;
        
        events.call_handler_openclose_market = &handler_openclose_market;
        events.call_handler_zyj_quote_real = &handler_zyj_quote_real_rsp;
        events.call_handler_zyj_quote_pankou = &handler_zyj_quote_pankou;
        events.call_handler_zyj_quote_pankou_detail = &handler_zyj_quote_pankou_detail;
        events.call_handler_zyq_min_data_recv_end = &handler_zyq_min_data_recv_end;
        events.call_handler_zyq_trace_data_recv_end = &handler_zyq_trace_data_recv_end;
        events.call_handler_zyq_kline_data_recv_end = &handler_zyq_kline_data_recv_end;
        events.call_handler_zyj_quote_real_detail = &handler_zyj_quote_real_detail;
        events.call_handler_zyj_complex_ranking = &handler_zyj_complex_ranking;
        
        events.callback_sign_status_notice = &handler_sign_status_notice;
        
        
        /********************************************************************
         文交所
         *******************************************************************/
        
        events.on_nhp_trade_request_symbol_rsp = &trade_request_symbol_rsp;
        //今日委托，成交
        events.on_nhp_trade_entrust_order_rsp = &trade_request_today_entrust_rsp;
        events.on_nhp_trade_deal_order_rsp = &trade_request_today_deal_rsp;
        events.on_nhp_trade_sharehold_rsp = &trade_request_sharehold_rsp;
        events.on_nhp_quote_hisKDataFirst_rsp = &queto_hisKDataFirst_rsp;
        events.on_nhp_quote_hisKDataCurDate_rsp = &quote_hisKDataCurDate_rsp;
        events.on_nhp_quote_stkDealData_rsp = &quote_stkDealDate_rsp;
        events.on_nhp_trade_historyorder_rsp = &trade_historyorder_rsp;
        events.on_nhp_trade_historyturnover_rsp = &trade_historyturnover_rsp;
        events.on_nhp_trade_pretrade_rsp = &trade_pretrade_rsp;
        events.on_nhp_trade_canceltrade_rsp = &trade_canceltrade_rsp;
        set_nhp_trade_events(events);
    }
}


