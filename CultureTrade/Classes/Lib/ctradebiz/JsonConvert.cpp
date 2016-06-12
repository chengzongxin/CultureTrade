//
//  JsonConvert.cpp
//  FT Trader
//
//  Created by SZFT1 on 14/11/28.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#include "JsonConvert.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#include "CalcInd.h"
#include "DataDef.h"

#include "global.h"
#include "nethelper.h"
#include "jxt_log.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
    
#include "cJSON.h"
#include "global.h"
#include "nethelper.h"
#include "tagdef.h"
#include "msgbase.h"
    
    
    ////////////////////
    void cJSON_AddStringToObjectEx(cJSON * root, int tag, const char *content)
    {
        char tmp[6];
        memset(tmp, 0, 6);
        sprintf(tmp, "%d", tag);
        cJSON_AddStringToObject(root, tmp, content);
    }
    
    void cJSON_AddDoubleToObjectEx(cJSON * root, int tag, double content)
    {
        char tmp[6];
        memset(tmp, 0, 6);
        sprintf(tmp, "%d", tag);
        cJSON_AddNumberToObject(root, tmp, content);
    }
    
    cJSON * cJSON_GetObjectItemEx(cJSON * root, int tag)
    {
        char tmp[6];
        memset(tmp, 0, 6);
        sprintf(tmp, "%d", tag);
        return cJSON_GetObjectItem(root, tmp);
    }
    
    ///////////////////下单//////////////////////
    void convert_declare_from_jsonnode(cJSON* order, char * forder)
    {
        CDeclareTitleDesc * findOrder = (CDeclareTitleDesc *)forder;
        cJSON* jNode = NULL;
        //CFindOrderRes findOrder;
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLARETIME);//公告时间
        if(NULL != jNode) findOrder->tTime =  atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLAREID);//公告编号
        if(NULL != jNode) findOrder->nAnnouceID = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLAREPUBLICER);//公告发布者
        if(NULL != jNode) findOrder->szDelcare = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLARETITLE);//公告标题
        if(NULL != jNode) findOrder->szTitle = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLARECONTENT);//公告内容
        if(NULL != jNode) findOrder->szContent = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DECLAREREADSTATUS);//阅读状态 1已阅读 2 未阅读
        if(NULL != jNode) findOrder->cReadStatus = jNode->valuestring[0];
    }
    
    //解析单据
    void convert_order_from_jsonnode(int orderType, cJSON* order, char* forder){
        CFindOrderRes * findOrder = (CFindOrderRes *)forder;
        
        cJSON* jNode = NULL;
        //CFindOrderRes findOrder;
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ACCTID);
        if(NULL != jNode) findOrder->iAccountID =  atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CUMQTY);
        if(NULL != jNode) findOrder->dbCumQty = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERQTY);
        if(NULL != jNode) findOrder->dbOrderQty = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDSTATUS);
        if(NULL != jNode) findOrder->chOrdStatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERTYPE);
        if(NULL != jNode) findOrder->chOrderType = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SIDE);
        if(NULL != jNode) findOrder->chSide = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SECURITYID);
        if(NULL != jNode) findOrder->ssSymbol = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_TEXT);
        if(NULL != jNode) findOrder->ssText = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_TIMEFORCE);
        if(NULL != jNode) findOrder->ssTransactTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SLPRICE);
        if(NULL != jNode) findOrder->dbSlPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLIENTPRICE);
        if(NULL != jNode) findOrder->dbCurrentPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SUBORDERTYPE);
        if(NULL != jNode) findOrder->chSubOrderType = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_FILLPOLLCY);
        if(NULL != jNode) findOrder->chFillPollcy = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_VALIDATETYPE);
        if(NULL != jNode) findOrder->chExpiration = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPERATORTYPE);
        if(NULL != jNode) findOrder->chOperatorType = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ACCOUNTALIAS);
        if(NULL != jNode) findOrder->ssAccountAlias = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_INSTRUMENTID);
        if(NULL != jNode)findOrder->ssInstrumentID = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_STOPLIMITPRICE);
        if(NULL != jNode) findOrder->dbStopLimitPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_EXECUTEPRICE);
        if(NULL != jNode) findOrder->dbExecuteprice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SUMINTEREST);
        if(NULL != jNode) findOrder->dbSumInterest = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_INTERESTDAYS);
        if(NULL != jNode) findOrder->dbInterestDays = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_MARGIN);
        if(NULL != jNode) findOrder->dbMargin = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPERATORALIAS);
        if(NULL != jNode) findOrder->ssOperatorAlias = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERTIME);
        if(NULL != jNode) findOrder->ssOrderTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSETICKET);
        if(NULL != jNode) findOrder->ssCloseticket = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSEORDERSIZE);
        if(NULL != jNode) findOrder->dbCloseordersize = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSEPRICE);
        if(NULL != jNode) findOrder->dbCloseprice =atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSEMARGINCURRECYPRICE);
        if(NULL != jNode) findOrder->dbCloseMarginCurrecyPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_PAYMENTCURRENCYPRICE);
        if(NULL != jNode) findOrder->dbPaymentCurrencyPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSEACCOUNTCURRENCYRATE);
        if(NULL != jNode) findOrder->dbCloseAccountCurrencyRate = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLSETIME);
        if(NULL != jNode) findOrder->ssClosetime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPENCLOSEPL);
        if(NULL != jNode) findOrder->dbOpenClosePl = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSECOMMISION);
        if(NULL != jNode) findOrder->dbCloseCommision = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ISCLOSE);//下单类型
        if(NULL != jNode) findOrder->chIsClose = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERTICKET);
        if(NULL != jNode) findOrder->iOrderticket = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_PRICE);
        if(NULL != jNode) findOrder->dbPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_STOPPX);
        if(NULL != jNode) findOrder->dbStopPx = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERINTEREST);
        if(NULL != jNode) findOrder->dbInterest = atof(jNode->valuestring);
        
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_CLOSE_TEXT);
        if(NULL != jNode) findOrder->ssCloseText = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_ORDERPL);
        if(NULL != jNode) findOrder->dbPl = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPENCLOSE);//下单类型
        if(NULL != jNode) findOrder->chOpenClose = (jNode->valuestring)[0];
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPENCOMMISION);
        if(NULL != jNode) findOrder->dbOpenCommision = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_EXPIRATIONDATE);
        if(NULL != jNode) findOrder->ssExpirationdate = jNode->valuestring;
        
        //
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_EXECUTE_DATE);
        if(NULL != jNode) findOrder->ssExecuteTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_BARGAIN_ID);
        if(NULL != jNode) findOrder->iBargainId =atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_BARGAIN_COMMISION);
        if(NULL != jNode) findOrder->dbBargainCommision = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_INTPRTCL_USERNAME);
        if(NULL != jNode) findOrder->ssAccountName = jNode->valuestring;
        
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_INTPRTCL_USERNAME);
        if(NULL != jNode) findOrder->ssAccountName = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_IS_YESDAY);
        if(NULL != jNode) findOrder->chYesday = jNode->valuestring[0];
        
        //后面增加2.0
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_MinDecimal);//小数位数
        if(NULL != jNode) findOrder->nDecimal = atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_HOLDER_SIZE);
        if(NULL != jNode) findOrder->dbHolderSize = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_HOLDER_PRICE);
        if(NULL != jNode) findOrder->dbHolderPrice = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_IS_OPENCLOSE);//下单类型
        if(NULL != jNode) findOrder->chIsClose = jNode->valuestring[0];
        
        
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_PARTYORDERID);//对手单号
        if(NULL != jNode) findOrder->iCounterPartExecuteTicket = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DEF_COMPENSATION);//交收延期手续费
        if(NULL != jNode) findOrder->dbDeferredCompensation = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_SELLCANG_ID);//卖方仓库ID
        if(NULL != jNode) findOrder->ssSellCangId = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_BUY_PAYOUT);//买方货款
        if(NULL != jNode) findOrder->dbBuyPayOut = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_MATH_WEIGHT);//交收重量
        if(NULL != jNode) findOrder->dbMatchWeight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_MATCH_DATE);//配对日期
        if(NULL != jNode) findOrder->ssMatchDate = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DAY_STEP);//交收到第几日
        if(NULL != jNode) findOrder->iDayStep = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAX_BODY_ORDER_TIME);//委托时间
        if(NULL != jNode) findOrder->ssDelegateTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DELIVERY_MARGIN);//交收冻结保证金
        if(NULL != jNode) findOrder->dbDeliveryMargin = atof(jNode->valuestring);
        
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_DELIVERY_PAYMENT);//交收贷款
        if(NULL != jNode) findOrder->dbDeliveryPayment = atof(jNode->valuestring);
        
        
        jNode = cJSON_GetObjectItemEx(order, FIX_TAG_BODY_OPENORDERPRICE);
        if(NULL != jNode) findOrder->dbOpenPrice = atof(jNode->valuestring);
        
        
        findOrder->orderType = orderType;
    }
    
    void convert_symbol_from_jsonnode(int symbolType, char * cJson, cJSON* symbol){
        CSymbolParam * sysbolParam = (CSymbolParam *)cJson;
        cJSON* jNode;
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_INTPRTCL_PRODUCTNAME);
        if(NULL != jNode) sysbolParam->ssSymbolname = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_BASECURRENCY);
        if(NULL != jNode) sysbolParam->ssBasecurrency = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MARGINCURRENCY);
        if(NULL != jNode) sysbolParam->ssMargincurrency = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_PAYMENTCURRENCY);
        if(NULL != jNode) sysbolParam->ssPaymentcurrency = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_RECKONCURRENCY);
        if(NULL != jNode) sysbolParam->ssReckoncurrency = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_QUOTAYIONTYPE);
        if(NULL != jNode) sysbolParam->ssQuotationType = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_AGREEUNIT);
        if(NULL != jNode) sysbolParam->dbAgreeunit = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MINPERSIZE);
        if(NULL != jNode)  sysbolParam->dbMinpersize = atof(jNode->valuestring);// jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MAXPERSIZE);
        if(NULL != jNode) sysbolParam->dbMaxpersize = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_PENDINGMINPERSIZE);
        if(NULL != jNode) sysbolParam->dbPendingminpersize = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_STEPSIZE);
        if(NULL != jNode) sysbolParam->dbStepsize = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MAXBUYPOSITION);
        if(NULL != jNode)  sysbolParam->dbMaxBuyPosition = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MAXSELLPOSITION);
        if(NULL != jNode) sysbolParam->dbMaxSellPosition = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MAXDEVIATIONPIPS);
        if(NULL != jNode) sysbolParam->dbMaxdeviationpips = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_LIMITORSTOPPIPS);
        if(NULL != jNode) sysbolParam->dbLimitorstoppips = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_BUYPIPS);
        if(NULL != jNode) sysbolParam->dbBuypips = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_SELLPIPS);
        if(NULL != jNode) sysbolParam->dbSellpips = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_TRADERIGHT);
        if(NULL != jNode) sysbolParam->chTraderight = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_ACCOUNTRECKONRATE);
        if(NULL != jNode) sysbolParam->dbAccountReckonRate = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MARGINTYPE);
        if(NULL != jNode) sysbolParam->chMargintype = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_ORGINMARGIN);
        if(NULL != jNode) sysbolParam->dbOrginmargin = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_FREEZEMARGIN);
        if(NULL != jNode) sysbolParam->dbFreezemargin = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_SLPIPS);
        if(NULL != jNode) sysbolParam->dbSlpips = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_TPPIPS);
        if(NULL != jNode) sysbolParam->dbTppips = atof(jNode->valuestring);
        
        //后面增加2.0
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MinDecimal);//小数位数
        if(NULL != jNode) sysbolParam->nDecimal = atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_MINCHANGE_UNIT);//订单大小
        if(NULL != jNode) sysbolParam-> dbMinchangeunit=atof(jNode->valuestring);// jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_QUOTEMINUNIT);//报价最小单位（订单价位）
        if(NULL != jNode) sysbolParam-> dbQuoteminunit= atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_SPECIES);//品种类型 1-外汇2-现货3-递延4-期货
        if(NULL != jNode) sysbolParam-> chSpecies= (char)atoi(jNode->valuestring);//(char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_MAXQUOTE_TIMEOUT);//最高报价延时，设置行情报价超时
        if(NULL != jNode) sysbolParam-> iMaxquoteTimeOut=atoi(jNode->valuestring);//// (char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_EXECTYPE);//执行方式 1–即时 2–询价 3–内部市场 4–交易所
        if(NULL != jNode) sysbolParam-> chExectype= (char)atoi(jNode->valuestring);//(char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DEF_MARGINSETTING);//保证金默认设置
        if(NULL != jNode) sysbolParam-> chDefaultmarginsetting= (char)atoi(jNode->valuestring);//(char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MARGINTYPE);//保证金计提方式 1-比例2-固定
        if(NULL != jNode) sysbolParam->chMargintype = (char)atoi(jNode->valuestring);//(char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_ORGINMARGIN);//初始保证金
        if(NULL != jNode) sysbolParam->dbOrginmargin = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_FREEZEMARGIN);//即市冻结保证金
        if(NULL != jNode) sysbolParam->dbFreezemargin = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_LOCK_MARGIN);//交易数量限制锁仓保证金设置
        if(NULL != jNode) sysbolParam->dbLockmargin = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DEF_SIZESETTING);//交易数量限制默认设置
        if(NULL != jNode) sysbolParam->chDefaultsizesetting = (char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_MINPERSIZE);//最小可交易数量
        if(NULL != jNode) sysbolParam->dbMinpersize = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_MAX_DOUBLEPOSITION);//最大双边订货量
        if(NULL != jNode) sysbolParam->dbMaxDoublePosition = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_BASE_PRICE);//基准价格
        if(NULL != jNode) sysbolParam->dbBasePrice = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DEF_TRADESETTING);//默认交易设置
        if(NULL != jNode) sysbolParam-> chDefaultradesetting = (char)atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_TRADERIGHT);//交易权限 1 - 禁用 2- 仅买入 3 - 仅卖出 4-仅平仓 5- 全部交易 '
        if(NULL != jNode) sysbolParam-> iTraderight= atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_FILLPOLLCY);//归档
        if(NULL != jNode) sysbolParam->chFillpollcy = (char)atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_VALIDATETYPE);//效力类型 1-GTC (截止日期、时间) 2 - day 当日有效  3 - 永久有效
        if(NULL != jNode) sysbolParam-> chExpiration= (char)atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DEF_COMMISIONSETTING);//手续默认设置
        if(NULL != jNode) sysbolParam-> chDefaultcommisionsetting= (char)atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_COMMISIONTYPE);//手续计提方式  '1' -- 固定 ‘2’ -- 比例
        if(NULL != jNode) sysbolParam->chCommisiontype = (char)atoi(jNode->valuestring);//jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_CLOSE_TODAYCOMMISION);//平今仓手续费
        if(NULL != jNode) sysbolParam->dbCloseTodayCommision = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol,FIX_TAX_BODY_OPEN_COMMISION );//建仓手续费
        if(NULL != jNode) sysbolParam->dbOpencommision = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_CLOSE_HISTORYCOMMISION);//平昨仓手续费
        if(NULL != jNode) sysbolParam->dbCloseHistoryCommision = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_TRADE_COMMISIONADD);//交易员手续附加
        if(NULL != jNode) sysbolParam->dbTradecommisionadd = atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DLY_BUYCOMMISION);//交收手续费类型
        if(NULL != jNode) sysbolParam->chDeliverycommisiontype= atoi(jNode->valuestring);//(char)jNode->valueint;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAX_BODY_DLY_SELLCOMMISION);//交收卖手续费
        if(NULL != jNode) sysbolParam->dbDeliverySellcommision= atof(jNode->valuestring);//jNode->valuedouble;
        
        jNode = cJSON_GetObjectItemEx(symbol, FIX_TAG_BODY_SHOWORDER);//显示顺序
        if(NULL != jNode)  sysbolParam->iShowOrder = atoi(jNode->valuestring);
        
        sysbolParam->nDecimal = abs((int)log10(sysbolParam->dbQuoteminunit));
        
        if(sysbolParam->nDecimal<0) sysbolParam->nDecimal=0;
        
        sysbolParam->symbolType = symbolType;
    }
    
    void convert_delivery_store_info_from_jsonnode(cJSON* cjson, const char* info) {
        COutCAUDeliveryWarehouseInfo* delivery = (COutCAUDeliveryWarehouseInfo*)info;
        
        cJSON* jNode;
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->iWareHouseId = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_NAME);
        if(NULL != jNode) delivery->szWareHouseName = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->szWareHouseFullName = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->szWareHouseCode = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->szAddress = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->szKeeper = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->szAuthenticatedPerson = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->ssZipCode = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->ssFax = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->ssEmail = jNode->valuestring;
    }
    
    void convert_delivery_info_from_jsonnode(cJSON* cjson, const char* info) {
        CDeliveryInfo* delivery = (CDeliveryInfo*)info;
        
        cJSON* jNode;
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_ID);
        if(NULL != jNode) delivery->iDeliveryid = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_GROUPID);
        if(NULL != jNode) delivery->iAccountgroupid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_ACCOUNTID);
        if(NULL != jNode) delivery->iAccountid = atoi(jNode->valuestring); // '客户ID',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_MATCH_ID);
        if(NULL != jNode) delivery->iMatchId = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_SYMBOL_GROUPID);
        if(NULL != jNode) delivery->iSymbolgroupid = atoi(jNode->valuestring); // '品种',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_SYMBOL);
        if(NULL != jNode) delivery->ssSymbolcode = jNode->valuestring; // '商品代码',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);
        if(NULL != jNode) delivery->iWarehouseid = atoi(jNode->valuestring); // '仓库ID',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_ID);
        if(NULL != jNode) delivery->iCangId = atoi(jNode->valuestring);	// 仓单ID
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_BARGAIN_ID);
        if(NULL != jNode) delivery->iExecuteticket = atoi(jNode->valuestring); // '成交单号',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_PARTYORDERID);
        if(NULL != jNode) delivery->iCounterpartyexecuteticket = atoi(jNode->valuestring); // '对手单号',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_WEIGHT);
        if(NULL != jNode) delivery->dbDeliveryweight = atof(jNode->valuestring); // '提货重量',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_DATE);
        if(NULL != jNode) delivery->ssSettlementdate = jNode->valuestring; // '交割日期',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_SETTLEMENTPRICE);
        if(NULL != jNode) delivery->dbSettlementprirce = atof(jNode->valuestring); // '交割价格',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_FEEDATE);
        if(NULL != jNode) delivery->ssWarehousefeedate = jNode->valuestring; // '仓储费截止日期',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_STATUS);
        if(NULL != jNode) delivery->chDeliverystatus = jNode->valuestring[0]; // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_REJECTREASON);
        if(NULL != jNode) delivery->ssRejectreason = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IS_INVOICE);
        if(NULL != jNode) delivery->chIsinvoice = jNode->valuestring[0]; // '是否需要发票',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_ISCANCEL);
        if(NULL != jNode) delivery->chIscancel = jNode->valuestring[0]; // '是否撤销',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_MEN);
        if(NULL != jNode) delivery->ssDeliverymen = jNode->valuestring; // '提货人',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IDCARD);
        if(NULL != jNode) delivery->ssIdcode = jNode->valuestring; // '身份证',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_TEL);
        if(NULL != jNode) delivery->ssTelephone = jNode->valuestring; // '联系电话',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ADDRESS);
        if(NULL != jNode) delivery->ssAddress = jNode->valuestring; // '联系地址',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ZIPCODE);
        if(NULL != jNode) delivery->ssZipcode = jNode->valuestring; // '邮政编码',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_INVOICE_TITLE);
        if(NULL != jNode) delivery->ssInvoicetitle = jNode->valuestring; // '发票抬头',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_COMPAY_NAME);
        if(NULL != jNode) delivery->ssCompanyname = jNode->valuestring; // '公司名称',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_COMPAY_ADDRESS);
        if(NULL != jNode) delivery->ssCompanyaddress = jNode->valuestring; // '公司地址',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_COMPAY_TEL);
        if(NULL != jNode) delivery->ssCompanytelephone = jNode->valuestring; // '公司电话',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_INTPRTCL_BANKCARDNO);
        if(NULL != jNode) delivery->ssBankaccount = jNode->valuestring; // '银行账号',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_INTPRTCL_BANKNAME);
        if(NULL != jNode) delivery->ssBankdeposit = jNode->valuestring; // '开户行',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_TAXREGISTRY_NUMBER);
        if(NULL != jNode) delivery->ssTaxregistrynumber = jNode->valuestring; // '税务登记号',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_DATE_BEGIN);
        if(NULL != jNode) delivery->ssDeliverydatebegin = jNode->valuestring; // '提货日期D5',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_DATE_END);
        if(NULL != jNode) delivery->ssDeliverydateend = jNode->valuestring; // '提货日期D6',
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_NUMBER);
        if(NULL != jNode) delivery->ssDeliverynumber = jNode->valuestring; // '提货号',
    }
    
    void convert_in_store_order_from_jsonnode(cJSON* cjson, const char* info) {
        CStoreOrderInfo* cobj = (CStoreOrderInfo*)info;
        cJSON* jNode;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_TO_IMPORT_APPLY_ID);//仓单ID
        if(NULL != jNode) cobj->iCangToImportId = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_ID);//仓单ID
        if(NULL != jNode) cobj->iCangid = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXPORT_ID);//出库单ID
        if(NULL != jNode) cobj->iExportid = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IMPORT_ID);//入库单ID
        if(NULL != jNode) cobj->iImportid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSEID);//交收仓库
        if(NULL != jNode) cobj->iWarehouseid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_GROUPID);//机构
        if(NULL != jNode) cobj->iAccountgroupid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_ACCOUNTID);//客户ID
        if(NULL != jNode) cobj->iAccountid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_APPLY_IMPORT_ID);//入库申请单ID
        if(NULL != jNode) cobj->iApplyid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_SYMBOL_GROUPID);//品种名称
        if(NULL != jNode) cobj->iSymbolgroupid = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_SECURITYID);//商品代码
        if(NULL != jNode) cobj->szSymbolcode = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_SYMBOL);//商品代码
        if(NULL != jNode) cobj->szSymbolcode = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IMPORT_DATE);//入库时间
        if(NULL != jNode) cobj->szImportdate = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_APPLY_STATUS);//
        if(NULL != jNode) cobj->chApplystatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IMPORT_STATUS);//状态
        if(NULL != jNode) cobj->chImportstatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_REJECTREASON);//拒绝原因
        if(NULL != jNode) cobj->szRejectreason = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_CHECKRESULT);//检验结果
        if(NULL != jNode) cobj->szCheckresult = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_APPLY_WEIGHT);//申请重量
        if(NULL != jNode) cobj->dbApplyweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IMPORT_WEIGHT);//入库重量
        if(NULL != jNode) cobj->dbImportweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FREEZE_EXPOERT_WEIGHT);//冻结出库重量
        if(NULL != jNode) cobj->dbFreezeexportweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXPOERT_WEIGHT);//出库重量
        if(NULL != jNode) cobj->dbExportweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FREEZE_CANG_WEIGHT);//冻结转仓单重量
        if(NULL != jNode) cobj->dbFreezecangweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_WEIGHT);//转仓单重量
        if(NULL != jNode) cobj->dbCangweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_UNIT_WEIGHT);//每件重量
        if(NULL != jNode) cobj->dbUnitweight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_QUANTITY);//件数
        if(NULL != jNode) cobj->iQuantity = atoi(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_GOODS_NUMBER);//货号
        if(NULL != jNode) cobj->szGoodsnumber = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_LOT_NUMBER);//批号
        if(NULL != jNode) cobj->szLot = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_PACKAGE);//包装方式
        if(NULL != jNode) cobj->szPackage = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_MADE_DATE);//生产年月
        if(NULL != jNode) cobj->szMade = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FACTORY);//生产厂商
        if(NULL != jNode) cobj->szFactory = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_TRA);//仓库经办人
        if(NULL != jNode) cobj->szWarehousetransactor = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WAREHOUSE_MANAGER);//仓库负责人
        if(NULL != jNode) cobj->szWarehousemanager = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CUST_TRA);//客户经办人
        if(NULL != jNode) cobj->szClienttransactor = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_NUMBER);//提货码
        if(NULL != jNode) cobj->ssDeliverynumber = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_DELIVERY_MEN);//提货人名称
        if(NULL != jNode) cobj->ssDeliveryMen = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_IDCARD);//提货人身份证
        if(NULL != jNode) cobj->ssIdCode = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_TEL);//提货人电话号码
        if(NULL != jNode) cobj->ssTelphone = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXPORT_DATE);//出库时间
        if(NULL != jNode) cobj->ssExportdate = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXPORT_STATUS);//出库状态
        if(NULL != jNode) cobj->chExportStatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_TYPE);//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}
        if(NULL != jNode) cobj->chCangType = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_DATE);//仓单日期
        if(NULL != jNode) cobj->ssCangDate = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CANG_STATUS);//状态;{"from":"json","0","等待仓库审核","1":"仓库驳回","2":"完成"}
        if(NULL != jNode) cobj->chCangStatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FREEZE_IMPORT_WEIGHT);//冻结转入库重量
        if(NULL != jNode) cobj->dbFreezeImportWeight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FREEZE_WAREHOUSE_DELIVERYWEIGHT);//冻结交收重量
        if(NULL != jNode) cobj->dbFreezeCloseWeight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_WARIT_WAREHOUSE_DELIVERYWEIGHT);//待交收交收重量
        if(NULL != jNode) cobj->dbFreezeWaitWeight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_FINISH_WAREHOUSE_DELIVERYWEIGHT);//已交收交收重量
        if(NULL != jNode) cobj->dbCloseWeight = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_APPLY_DATE);//已交收交收重量
        if(NULL != jNode) cobj->ssApplyDate = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_SYMOBL_ATT_NUM);/*, "属性个数"*/
        if(NULL != jNode) {
            cobj->iAttributeNum = atoi(jNode->valuestring);
            for(int i = 0;i < cobj->iAttributeNum; i++ ) {
                COUTSymbolAttributeValue attributeValue;
                jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ATTR_ID);//属性ID
                if(NULL != jNode) attributeValue.iAttributeId = atoi(jNode->valuestring);
                
                jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ATT_VALUE);//属性值
                if(NULL != jNode) attributeValue.szAttvalue = jNode->valuestring;
                
                cobj->arrAttributArrValue[i] = attributeValue;
            }
        }
    }
    
    
    void convert_funds_flow_info_from_jsonnode(cJSON* cjson, const char* info) {
        CFundsFlowRes* cobj = (CFundsFlowRes*)info;
        cJSON* jNode;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ACCTID);
        if(NULL != jNode) cobj->iAccountId = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ACCOUNTALIAS);
        if(NULL != jNode) cobj->ssAccountAlias = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXECUTEDATE);
        if(NULL != jNode) cobj->ssExecuteTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_CHANGE);
        if(NULL != jNode) cobj->dbChangeFunds = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_BALANCE);
        if(NULL != jNode) cobj->dbBalance = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ASSOCIATETICKET);
        if(NULL != jNode) cobj->iAssociateticket = atoll(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_OPERATORTYPE);
        if(NULL != jNode) cobj->chOperatortype = jNode->valuestring[0];
    }
    
    void convert_monery_inout_info_from_jsonnode(cJSON* cjson, const char* info) {
        CMoneryInOutRes* cobj = (CMoneryInOutRes*)info;
        cJSON* jNode;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_ID);
        if(NULL != jNode) cobj->iId = atoll(jNode->valuestring);
        
        if(cobj->iId == 0) {
            jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_MONEYAPPLYSERINALNO);
            if(NULL != jNode) cobj->iId = atoll(jNode->valuestring);
        }
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_INTPRTCL_MONEY);
        if(NULL != jNode) cobj->dbAmount = atof(jNode->valuestring);
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_TIME);
        if(NULL != jNode) cobj->ssApplyTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_VERIFICATIONTIME);
        if(NULL != jNode) cobj->ssAuditTime = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_EXECUTETYPE);
        if(NULL != jNode) cobj->chExecutestatus = jNode->valuestring[0];
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_TEXT);
        if(NULL != jNode) cobj->ssRemark = jNode->valuestring;
        
        jNode = cJSON_GetObjectItemEx(cjson, FIX_TAG_BODY_OPERATORTYPE);
        if(NULL != jNode) cobj->chOperatortype = jNode->valuestring[0];
    }
    
#ifdef __cplusplus
};
#endif