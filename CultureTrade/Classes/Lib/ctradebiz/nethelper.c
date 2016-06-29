//
//  nethelper.c
//  jxtMsgClient
//
//  Created by  on 13-1-27.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "nethelper.h"
#include "jxtnethelper.h"
#include "common.h"
#include "jxt_log.h"
//#include "global.h"
#include "netbase.h"
#include "nt_types.h"

#include "tagdef.h"
#include "TradeEngine.h"

int nSeq = 0;

const int PKGHEADERLEN = 20;


char str_user[USERACCLEN];
char str_pwd[USERACCLEN];
int  n_pwd_md5;

nhp_net_events netevents;
nhp_trade_events tradeevents;
callback_logic callbacklogics;

int nSID = -1;
void clear_suser_spwd();

int nQuoteSID = -1;


int nBalanceSID = -1;

int nPankouNoticeCount = 0;
int nRealNoitceCount = 0;

int nQuoteLogined = 0;
int nQuoteReqPanKou = 0;


int tradeStuats=-1;
int quoteStuats=-1;
int net_type = 0;





void nhp_stopnet();
void nhp_init_net_balance(char * server, int port);
void exceptional_handling(char * json);
//int getPankouNoticeCount()
//{
//    return nPankouNoticeCount;
//}
//int getRealNoitceCount()
//{
//    return nRealNoitceCount;
//}

//int miPan=1;

//做市商=》盘面更新商品
int hasZsSymbol = 0;
int update_zs_report = 0;
int request_zs_report_count = 1;

//是否正在连接
int connection_ing = 0;

//中远期=》盘面更新商品
int hasZyjSymbol = 0;
int update_zyj_report = 0;
int request_zyj_report_count = 1;
int kick_notify = 0;

stuBalanceInfo mBalanceInfo;
StuCustomLoginInfo loginInfo;
int useBalance = 0;
int useLoginInfoIdx = 0;
int useLoginNext();
int use_start_balance_next();
int isUpdate_ZYQ_Report(){
    return update_zs_report;
}
//请求报价牌
//void report_price(){
//    nhp_send_report_data_info_req();
//    nhp_send_zyj_report_data_info_req();
//}

void refresh_begin(){
    hasZsSymbol = 0;
    update_zs_report=0;
    hasZyjSymbol = 0;
    update_zyj_report = 0;
}
void refresh_end(){}

int is_refresh = 0;
void nhp_refresh() {
    is_refresh = 1;
}

int nhp_send_zyq_quote_report(const char* pData, int pLen){
    int nSeq = nhp_get_nseq();
    Send(nQuoteSID, REPORTDATA_ZYQ, nSeq, (LPBYTE)pData, pLen);
    printBusLogEx("pData = [%s]", pData);
    return nSeq;
}

//void sendZSQuoteReport(){
//    Send(nQuoteSID, REPORTDATA, 0, NULL, 0);
//}

//TODO:行情回调
bool on_nhp_session_trans_package_callback_handle_quote(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen){
//    if (nSid != nQuoteSID) return 1;
    printBusLogEx("on_nhp_session_trans_package_callback_handle_quote  nSid: [%d] nCmd: [%0x] nSeq: [%d] nSize: [%d]\n", nSid, nCmd, nSeq, nLen);
    switch (nCmd){
        case USERPWDVALID: {
            update_zs_report = 0;
            update_zyj_report = 0;
            if (nLen == 1) {
                printBusLogEx("quote server login success");
                nQuoteLogined = 1;
                nhp_send_init_market_req();
                tradeevents.on_nhp_quote_login_rsp(1);
            } else {
                tradeevents.on_nhp_quote_login_rsp(0);
                printBusLogEx("quote server login failed");
            }
            break;
        }
        case MINUTE1_HISK:        //= (char)1,   //表明为一分钟线
        case MINUTES5_HISK:       //= (char)5,   //5分钟K线
        case MINUTES10_HISK:      //= (char)10,  //10分钟K线
        case MINUTES15_HISK:      //= (char)15,  //15分钟K线
        case MINUTES30_HISK:      //= (char)30,  //30分钟K线
        case MINUTES60_HISK:      //= (char)60,  //60分钟K线
        case DAY_HISK:            //= (char)61, //日K线
        case WEEK_HISK:           //= (char)62, //周K线
        case MONTH_HISK:          //= (char)63, //月K线
        {
            tradeevents.on_nhp_quote_hisKDataFirst_rsp(nCmd,pData,nLen);
            break;
        }
        case HISKDATACURDATE:  // 1f00
        {
            tradeevents.on_nhp_quote_hisKDataCurDate_rsp(nCmd,pData,nLen);
            break;
        }
        case STKDEALDATA:     // 0300
        {
            tradeevents.on_nhp_quote_stkDealData_rsp(pData);
            break;
        }
        case INITMARKET: {
            tradeevents.on_nhp_quote_initMarket_rsp((const char*)pData, nLen);
            break;
        }
        case PUSH_REALDATA: {
            if(hasZsSymbol > 0){
                if(update_zs_report > 0){
                    request_zs_report_count = 0;
                    tradeevents.on_nhp_quote_real((char*)pData, nLen);
                } else {
                    request_zs_report_count += 1;
                    if (request_zs_report_count % 10 == 0 && nQuoteLogined == 1) {
//                        sendZSQuoteReport();
                        request_zs_report_count = 0;
                    }
                }
            }
            break;
        }
        case REPORTDATA: {
            tradeevents.on_nhp_quote_pankou(nSeq, (char*)pData, nLen);
            printBusLogEx("做市商盘面数据更新了【】 商品");
            break;
        }
        case STKMINUTEDATA:{
//            int nSymbolIDLen = strlen((const char*)pData);
//            char* pSymbolID = (char*)pData;
//            int iCount = nLen;
//            printBusLogEx("_STKMINUTEDATA : %d", iCount);
//            tradeevents.on_min_data_recv_end(nSeq, pSymbolID, (CSTraceUnit*)(pData + nSymbolIDLen + 1), iCount);
            tradeevents.on_min_data_recv_end(nSeq, (char*)pData, nLen);
            break;
        }
        case  STKTRACEDATA:{
//            int nSymbolIDLen = strlen((const char*)pData);
//            char* pSymbolID = (char*)pData;
//            int iCount = nLen;
//            printBusLogEx("_STKTRACEUTEDATA : %d", iCount);
//            tradeevents.on_trace_data_recv_end(nSeq, pSymbolID, (CSTraceUnit*)(pData + nSymbolIDLen + 1), iCount);
            tradeevents.on_trace_data_recv_end(nSeq, (char*)pData, nLen);
            break;
        }
        case HISKDATA: {
//            int nSymbolIDLen = strlen((const char*)pData);
//            char* pSymbolID = (char*)pData;
//            int iCount = nLen;
//            printBusLogEx("_HISKDATA : %d", iCount);
//            tradeevents.on_kline_data_recv_end(nSeq, pSymbolID, (CSHisKUnit*)(pData + nSymbolIDLen + 1), iCount);
           tradeevents.on_kline_data_recv_end(nSeq, (char*)pData, nLen);
            break;
        }
        
        case INIT_MARKET_ZYQ:{
            tradeevents.on_zyq_symbol_data_info_end(pData, nLen);
            break;
        }
        case OPEN_MAKRKET_ZYQ:{// 开市消息
            tradeevents.call_handler_openclose_market(ZYJ_TYPE, 1);
            break;
        }
        case CLOSE_MAKRKET_ZYQ :{// 收市消息
            tradeevents.call_handler_openclose_market(ZYJ_TYPE, 0);
            break;
        }
        case REPORTDATA_ZYQ :{//报价表
            update_zyj_report = tradeevents.call_handler_zyj_quote_pankou(nSeq, (char*)pData, nLen);
            printBusLogEx("中远期盘面数据更新了【%d】 商品", update_zyj_report);
            break;
        }
        case REPORTDATA_DETAIL_ZYQ: {//报价表详细
            tradeevents.call_handler_zyj_quote_pankou_detail(nSeq, (char*)pData, nLen);
            break;
        }
        case HISKDATA_ZYQ: {//请求K线数据
            tradeevents.call_handler_zyq_kline_data_recv_end(nSeq, (const char*)(pData), nLen);
            break;
        }
        case STKMINUTEDATA_ZYQ: {//请求分时走势
            tradeevents.call_handler_zyq_min_data_recv_end(nSeq, (const char*)(pData), nLen);
            break;
        }
        case STKTRACEDATA_ZYQ: {//成交明细
            tradeevents.call_handler_zyq_trace_data_recv_end(nSeq, (const char*)(pData), nLen);
            break;
        }
        case PUSH_REALDATA_ZYQ: {//(报价)实时表推送
            tradeevents.call_handler_zyj_quote_real((const char*)pData, nLen);
            break;
        }
        case PUSH_REALDATA_DETAIL_ZYQ: {//(图表详细)实时推送
            tradeevents.call_handler_zyj_quote_real_detail((const char*)pData, nLen);
            break;
        }
        case PUSH_REPORTDATA_LIST_ZYQ: {//实时推送报价表列表(注册改变)
//            tradeevents.on_nhp_zyj_quote_real((char*)pData, nLen);
            break;
        }
        case COMPLEX_RANKING_ZYQ: //请求综合排名
        case PUSH_COMPLEX_RANKING_ZYQ: {//推送综合排名
            tradeevents.call_handler_zyj_complex_ranking(nSeq, (const char*)pData, nLen);
            break;
        }
    }
    return 1;
}


bool on_nhp_session_trans_package_callback_handle(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen) {
//    if(nSid != nSID) return 1;
    char * json;
    json = (char *)ty_malloc(nLen - PKGHEADERLEN + 1);
    memcpy((void *)json, (const void *) (pData + PKGHEADERLEN ), nLen - PKGHEADERLEN);
    printBusLogEx("on_nhp_session_trans_package_callback_handle nSid: [%d] nCmd: [%d] nSeq: [%d] nSize: [%d]\n", nSid, nCmd, nSeq, nLen);
    printBusLogEx("on_nhp_session_trans_package_callback_handle pData = \n%s",json);
    ////////////////////////trade rsp///////////////////////////
    
    //登录应答
    if (nCmd == atoi(FIX_MSGTYPE_TRADELOGIN_RES)) {
        hasZyjSymbol = 0;
        hasZsSymbol = 0;
        tradeevents.on_nhp_trade_login_rsp(json);
    } else if (nCmd == atoi(FIX_MSGTYPE_ACCCOUNT_RELOGIN)) {//重新登录
        
    }else if (nCmd == atoi(FIX_MSGTYPE_QUERY_TODAYORDER_RES)) {//今日委托应答
        tradeevents.on_nhp_trade_entrust_order_rsp(json);
    }else if (nCmd == atoi(FIX_MSGTYPE_QUERY_TODAYTURNOVER_RES)) {//今日成交应答
        tradeevents.on_nhp_trade_deal_order_rsp(json);
    }else if (nCmd == atoi(FIX_MSGTYPE_TRADE_RES)) {//下单应答
        tradeevents.on_nhp_trade_open_rsp(nSeq, json);
    }else if (nCmd == atoi(FIX_MSGTYPE_SHAREHOLD_RES)) {//1022 份额持仓
        tradeevents.on_nhp_trade_sharehold_rsp(json);
    }else if (nCmd == atoi(FIX_MSGTYPE_MONEYHOLD_RES)) {//资金查询应答
        tradeevents.on_nhp_trade_moneyhold_rsp(json);
    } else if (nCmd == atoi(FIX_MSGTYPE_ORDER_RES)) {//单据查询应答0
        tradeevents.on_nhp_trade_order_rsp(json);
    } else if (nCmd == atoi(FIX_MSGTYPE_HISTORYREPORT_RES)) {//历史单据查询应答
        tradeevents.on_nhp_trade_hisorder_rsp(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QUERY_PRODUCT_RES)){//商品信息应答,行情
        tradeevents.on_nhp_trade_request_symbol_rsp(json);
        update_zs_report = 0;
        hasZsSymbol = 1;
    } else if (nCmd == atoi(FIX_MSGTYPE_SYMBOL_CHANGE_NOTICE)) {//商品信息变更通知
        tradeevents.callback_trade_request_symbol();
    } else if (nCmd == atoi(FIX_MSGTYPE_ACCOUNT_FUND_CHANGE_NOTICE)) {//账户资金变更通知
        tradeevents.callback_trade_funds();
    } else if (nCmd == atoi(FIX_MSGTYPE_ACCOUNT_FUND_AND_ORDER_NOTICE)) {//账户资金、单据变更通知
        tradeevents.callback_trade_funds();
        tradeevents.callback_trade_order();
    } else if (nCmd == atoi(FIX_MSGTYPE_MONEY_IN_OUT_AUDIT_NOTICE))  {//出入金审核通知
    }  else if (nCmd == atoi(FIX_MSGTYPE_ACCOUNT_SIGN_STATUS_NOTICE))  {//出入金审核通知
        tradeevents.callback_sign_status_notice(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_ACCCOUNT_KILL_LOGOUT)) {//用户被踢下线
        kick_notify=1;
        nhp_stop_session();
//        tradeevents.on_nhp_ca_kick_notify(NULL);
    } else if (nCmd == atoi(FIX_MSGTYPE_QRY_CAU_SYMBOL_RES)) {
        update_zyj_report = 0;
        hasZyjSymbol = tradeevents.call_handler_zyj_symbol(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QRY_CAU_ORDER_RES)) {//竞价查委托单单响应
        tradeevents. call_handler_zyj_delegate_order(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QRY_CAU_ORDER_BARGAIN_RES)) {//竞价查成交单单响应
        tradeevents.call_handler_zyj_deal_order(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QRY_CAU_ORDER_POSITION_RES)) {//竞价查持仓单单响应
        tradeevents.call_handler_zyj_hold_order (nSeq, json);
    }  else if (nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_SETTLE_RES)) {//竞价查交收单单响应
        tradeevents.call_handler_zyj_settle_order (nSeq, json);
    }  else if (nCmd == atoi(FIX_MSGTYPE_CAU_TRADE_RES) ||
              nCmd == atoi(FIX_MSGTYPE_CAU_TRADE_BARGAIN_RES) ||
              nCmd == atoi(FIX_MSGTYPE_CAU_CANCLE_ORDER_RES)) {//竞价查成交单单请求
        tradeevents.call_handler_zyj_sumbit_order(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_DECLARE) || nCmd ==  MSGTYPE_DECLARE_INFO_NOTICE) {//公告推送
        tradeevents.call_handler_declare_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_QRY_DECLARETITLE_RES)){///查询公告抬头应答
        tradeevents.call_handler_declare_title( nSeq, json);
        //公告rwy
    } else if(nCmd == atoi(FIX_MSGTYPE_QRY_DECLARECONTENT_RES)){///查询公告内容应答
        //公告内容查询
        tradeevents.call_handler_declare_content(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_QRY_MONEY_IN_OUT_RES)) {//查询出入金应答
        tradeevents.call_handler_inout_monery(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_FUNDSLOG_RES) || nCmd == atoi(FIX_MSGTYPE_QRY_FUNDSLOG_RES)) {//资金流水 当前资金流水 应答
        tradeevents.call_handler_funds_flowing(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_MODIFYPWD_RES)) {//修改密码
        tradeevents.call_handler_modifypasswd(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_BILLOFLADING_RES)) {//查询提货意向应答
        tradeevents.call_handler_query_apply_delivery_intention(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_BILLOFLADING_ORDER_RES)) {//提交提货应答
        tradeevents.call_handler_apply_delivery_intention(nSeq, json);
    }else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_SELLT_WAREHOUSE_RES)) {//获取”交收仓库信息”应答
        tradeevents.call_handler_query_sellte_store_info(nSeq,  json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_GOODSATTR_RES)) {//获取”商品属性”应答
        tradeevents.call_handler_query_in_store_goods_property(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_IMPORT_APPLY_RES)) {//请求“入库申请”请求
        tradeevents.call_handler_apply_in_store(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_IMPORT_APPLY_RES)) {//查询 “入库请求”应答
        tradeevents.call_handler_query_apply_in_store(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_IMPORT_RES)) {//查询”入库单”应答
        tradeevents.call_handler_query_in_store_order(nSeq,  json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_SETTLE_STATUS)) {//“交收”状态推送
        tradeevents.call_handler_sellte_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_EXPORT_STATUS)) {//“出库单”状态推送
        tradeevents.call_handler_outstore_order_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_IMPORT_APPLY_STATUS)) {//“入库申请”状态系统推送
        tradeevents.call_handler_apply_instore_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_CANG_STATUS)) {//“仓单”状态推送
        tradeevents.call_handler_cang_order_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_BILLOFLADING_ORDER_REQ)) {//提货意向”状态推送
        tradeevents.call_handler_apply_delivery_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_IMPORT_STATUS)) {//推送 “入库单”
        tradeevents.call_handler_instore_order_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_PUSH_CANG_IMPORT_STATUS)) {//推送 “仓单转入库单”
        tradeevents.call_handler_cangorder_to_instore_notify(json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_CANG_RES)) {//查询”仓单”
        tradeevents.call_handler_query_cang(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_EXPORT_RES)) {//查询”出库单”应答
        tradeevents.call_handler_query_out_store_order(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_QRY_CANG_TO_IMPORT_RES)) {//"仓单转入库单"应答
        tradeevents.call_handler_query_cang_to_in_store_order(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_OP_CANG_TO_IMPORT_RES)) {//操作"仓单转入库单" 应答
        tradeevents.call_handler_op_cang_to_in_store_order_response(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_CANG_TO_IMPORT_RES)) { //提交"仓单转入库单"应答
        tradeevents.call_handler_cang_to_in_store_order_response(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_OP_BILLOFLADING_RES)) {//操作提货意向应答
        tradeevents.call_handler_apply_delivery_intention(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_MOD_BILLOFLADING_RES)) {//修改提货意向应答
        tradeevents.call_handler_apply_delivery_intention(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_MOD_IMPORT_APPLY_RES)) {//修改“入库申请”
        tradeevents.call_handler_apply_in_store(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_OP_IMPORT_APPLY_RES)) {//操作”入库申请”应答
        tradeevents.call_handler_apply_in_store(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_OP_CANG_RES)) {  //操作"仓单"
        
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_IMPORT_TO_EXPORT_RES)) {  //入库单转出库单” 应答
        tradeevents.call_handler_op_in_store_order_response(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_WAREHOUSE_IMPORT_TO_CANG_RES)) {  //入库单转仓单应答
        tradeevents.call_handler_op_in_store_order_response(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_QRY_CAU_HIS_ORDER_POSITION_RES)) {  //竞价查历史持仓单应答
        tradeevents.on_nhp_trade_hisorder_rsp(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_CAU_CANCLE_DELIVERY_ORDER_RES) ||//提交申报交收响应
              nCmd == atoi(FIX_MSGTYPE_CAU_DELIVERY_BARGAIN_TRADE_RES) ||
            nCmd == atoi(FIX_MSGTYPE_CAU_DELIVERY_TRADE_RES) ||
            nCmd == atoi(FIX_MSGTYPE_CAU_RECKON)) {
        tradeevents.call_handler_apply_delivery_sellte_rsp(nSeq, json);
    } else if(nCmd == atoi(FIX_MSGTYPE_MONEYINOUTAPPLY_RES)) {      //出入金申请应答
        tradeevents.call_handler_inout_money_rsp(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QUERY_HISTORYORDER_RES)){
        tradeevents.on_nhp_trade_historyorder_rsp(nSeq, json);
    } else if (nCmd == atoi(FIX_MSGTYPE_QUERY_HISTORYTURNOVER_RES)){
        tradeevents.on_nhp_trade_historyturnover_rsp(nSeq,json);
    } else if (nCmd == atoi(FIX_MSGTYPE_PRETRADE_RES)){
        tradeevents.on_nhp_trade_pretrade_rsp(nSeq,json);
    }
    
    //异常处理
    exceptional_handling(json);
    
    ty_mfree((void **)&json);
    return 1;
}

// 异常处理
void exceptional_handling(char * json){
    
    cJSON *root;
    
    cJSON *retcode;
    root=cJSON_Parse(json);
    int nRet=-1;
    if (root){
        retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE_JSON);
        if (retcode != NULL) return;
        
        retcode = cJSON_GetObjectItemEx(root, FIX_TAG_INTPRTCL_RSPSCODE);
        
        //正常
        if (NULL != retcode && (nRet = atoi(retcode->valuestring)) == FIX_MSGTYPE_RES_OK) return;
        //异常
        int retValue =atoi(retcode->valuestring);
        
        switch (retValue) {
            case -1101: // 系统会话无效
            {
                nhp_notify_net_state(0, 0);
                netevents.nhp_net_status_rsp(TRADE_TYPE, 0);
                sleep(5);
                nhp_notify_net_state(1, 1);
            }
                break;
            case -3016: // RELOGIN
            {
                nhp_notify_net_state(0, 0);
                netevents.nhp_net_status_rsp(TRADE_TYPE, 0);
                sleep(5);
                nhp_notify_net_state(1, 1);
            }
                break;
            default:
                break;
        }
    }
    cJSON_Delete(root);
}



void set_nhp_trade_events(nhp_trade_events events) {
    tradeevents = events;
}

void login_on_quote() {
    printBusLogEx("login_on_quote");
    if(nQuoteLogined != 0) return;
    LPBYTE account = new BYTE[40];
    memset(account, 0, 40);
    memcpy(account, str_user, 20);
    memcpy(&account[20], str_pwd, 20);
    Send(nQuoteSID, USERPWDVALID, 0, account, 0);
    delete []account;
}

int getTradeStauts(){return tradeStuats;}

int getQuoteStauts(){return quoteStuats;}

//网络状态回调
bool on_nhp_session_status_callback_handle_quote(int nSid,int nFlag)
{
    if(kick_notify) return 1;
    printBusLogEx("on_nhp_session_status_callback_handle_quote nSid:%d nFlag:%d", nSid, nFlag);
    quoteStuats = nFlag;
    //行情连接成功发登录包
    if (CST_CONNECTED == nFlag) {
        nQuoteLogined=0;
        login_on_quote();
        nQuoteReqPanKou = 0;
    } else {
        nQuoteReqPanKou = 1;
    }
    netevents.nhp_net_status_rsp(QUOTE_TYPE, nFlag);
//    trading = 0;
    return 1;
}

//登录
void nhp_request_login(){
    refresh_begin();
    tradeevents.callback_trade_login(str_user, str_pwd);
}

//网络状态回调
bool on_nhp_session_status_callback_handle(int nSid,int nFlag)
{
    if(kick_notify) return 1;
    tradeStuats = nFlag;
    printBusLogEx("on_nhp_session_status_callback_handle nSid:%d nFlag:%d", nSid, nFlag);
    //交易
    if (nSid == nSID) {
        if (CST_CONNECTED == nFlag) {
            if (strlen(str_user) > 0 && strlen(str_pwd) > 0) {
                nhp_request_login();
                tradeevents.callback_trade_login(str_user, str_pwd);
            }else{
                char *user[USERACCLEN] = {0};
                char *pwd[USERACCLEN] = {0};
                callbacklogics.cache_user_pwd(user,pwd);
                if (strlen(*user) > 0 && strlen(*pwd) > 0)
                {
                    refresh_begin();
                    tradeevents.callback_trade_login(*user, *pwd);
                }
            }
        } else {
            if(useBalance && useLoginInfoIdx < 5){//重连5次
                useLoginInfoIdx+=1;
                if(useLoginNext()) return 1;
            }
            if(1 == connection_ing) nhp_stopnet();
        }
        netevents.nhp_net_status_rsp(TRADE_TYPE, nFlag);
        connection_ing = 0;
    }
    return 1;
}

void set_callback_logics(callback_logic callbacklogic)
{
    callbacklogics = callbacklogic;
}

//网络层有事件通知，切换会主线程来处理事件
bool on_nhp_recv_notify_event_quote_new(int nSid)
{
    //todo
    //netevents.on_nhp_recv_notify_event();
    return 1;
}

//网络层有事件通知，切换会主线程来处理事件
bool on_nhp_recv_notify_event_new(int nSid)
{
    //todo
    //netevents.on_nhp_recv_notify_event_new();
    return 1;
}

uint64_t processcount  = 0;
//
void nhp_process_event()
{
    if(kick_notify) return;
    if(nBalanceSID>0) {
        ProcessEvent(nBalanceSID);
    }
    if(is_refresh == 1){//如果是刷新时
        tradeevents.call_trade_refresh(0);
        is_refresh = 0;
    }
    if (processcount % 2 == 0) {
        ProcessEvent(nSID);
    }
    ProcessEvent(nQuoteSID);    
    processcount++;
}

//设定网络状态相关的回调
void set_nhp_net_events(nhp_net_events events)
{
    netevents = events;
}

void nhp_init_net(char * tradeserver, int tradeport, char * quoteserver, int quoteport)
{
    
    //TODO: 网络初始化
    nSID = CreateSession();
    SetSessionSocketPackType(nSID, 0);
	AddHost(nSID, 1, tradeserver, tradeport);
    BindFirePackage(nSID, on_nhp_session_trans_package_callback_handle);
	BindFireSessionStatus(nSID, on_nhp_session_status_callback_handle);
    BindNotifyEvent(nSID, on_nhp_recv_notify_event_new);

    
    //建立行情链路
    nQuoteSID = CreateSession();
    SetSessionSocketPackType(nQuoteSID, 1);
    AddHost(nQuoteSID, 1, quoteserver, quoteport);

    BindFirePackage(nQuoteSID, on_nhp_session_trans_package_callback_handle_quote);
	BindFireSessionStatus(nQuoteSID, on_nhp_session_status_callback_handle_quote);
    BindNotifyEvent(nQuoteSID, on_nhp_recv_notify_event_quote_new);
 
    printBusLogEx("nhp_init_net ==>  nSID=[%d] sever=[%s] port=[%d] nQuoteSID=[%d] sever=[%s] port=[%d]", nSID, tradeserver, tradeport, nQuoteSID, quoteserver, quoteport);
    
}

/*
void nhp_add_servers_by_clear()
{
    int preids[64];
    int idscount = 64;
    int i;

    memset((void *)preids, 0, sizeof(int) * idscount);

    if (nSID > 0)
    {

        ClearHost(nSID, 1);
        ClearHost(nQuoteSID, 1);


        getAllServerIds(preids, &idscount);

        for (i = 0; i < idscount; i++)//for (i = idscount - 1; i >= 0; i--)
        {
            AddHost(nSID, 1, getServerIpById(preids[i]),
                    getServerPortById(preids[i]));

            printBusLogEx("AddHost SID:%d ip:%s port:%d", nSID,
                      getServerIpById(preids[i]),
                      getServerPortById(preids[i]));
        }

    }
}
*/
void nhp_start_net()
{
    if (nSID > -1) {
        NotifyNetState(nSID, 1);
        StartSession(nSID);
        printBusLogEx("StartSession SID:%d", nSID);
    }

    if (nQuoteSID > -1) {
        NotifyNetState(nQuoteSID, 1);
        StartSession(nQuoteSID);
        printBusLogEx("StartSession QuoteSID:%d", nQuoteSID);
    }
}

int isWifi(){
    return net_type;
}

void nhp_notify_net_state(int ntype, int state)
{
    net_type = ntype;
    int s = NotifyNetState(nSID, state);
    s = NotifyNetState(nQuoteSID, state);
}

void nhp_stopnet()
{
    if (nSID > -1 || nQuoteSID > -1) {
        nhp_stop_session();
    }
}

void clear_suser_spwd()
{
    memset(str_user, 0, USERACCLEN);
    memset(str_pwd, 0, USERACCLEN);
}

void cache_suser_spwd(char * suser, char * spwd)
{
    clear_suser_spwd();
    strcpy(str_user, suser);
    strcpy(str_pwd, spwd);
}

void reset_spwd_cache(char *new_pwd)
{
    memset(str_pwd, 0, USERACCLEN);
    strcpy(str_pwd, new_pwd);
    n_pwd_md5 = 0;
}

//网络层有事件通知，切换会主线程来处理事件
static bool on_nhp_recv_balance_notify_event(int nSid)
{
    //todo
    //netevents.on_nhp_recv_notify_event();
    return 1;
}

bool on_nhp_session_balance_status_callback_handle(int nSid,int nFlag){
    printBusLogEx("on_nhp_session_balance_status_callback_handle %d", nFlag);
    //行情连接成功发登录包
    if (CST_CONNECTED == nFlag) {
        Send(nBalanceSID, 0x01, 1, NULL, 0);        
//        int nDataLen = sizeof(stuRecvDataHead);
//        char* pDataQuote = new char[nDataLen];
//        memset(pDataQuote, 0, nDataLen);
//        stuRecvDataHead* pHead = (stuRecvDataHead*)(pDataQuote);
//        pHead->cGroupType = 1;
//        pHead->cMsgType = 1;
////            memcpy(pDataQuote, (const void *)&pHead, nDataLen);
//        memcpy(pDataQuote, pHead, nDataLen);
////            bRet = Send((PBYTE)pDataQuote, nLen);
//
//        SendQuotePack(pDataQuote, nDataLen);
//        delete[] pDataQuote;
    } else {
        if(CST_UNCONNECTED != nFlag) {
            if(mBalanceInfo.rIdx >= mBalanceInfo.mSize || !use_start_balance_next()){
                mBalanceInfo.rIdx = 0;
                nhp_stop_session();
                netevents.nhp_net_status_rsp(TRADE_TYPE, nFlag);
            }
        }
    }
    return 1;
}

int useLoginNext() {
    switch (useLoginInfoIdx) {
        case 0: {
//            if(strlen(loginInfo.szTradeServer) == 0 || strlen(loginInfo.szQuoteServer) == 0) return 0;
            if(strlen(loginInfo.szTradeServer) == 0) return 0;
            nhp_init_net_login_info(loginInfo.szTradeServer, loginInfo.sTradePort, loginInfo.szQuoteServer,loginInfo.sQuotePort);
            return 1;
        }
        case 1: {
//            if(strlen(loginInfo.szTradeServer1) == 0 || strlen(loginInfo.szQuoteServer1) == 0) return 0;
            if(strlen(loginInfo.szTradeServer1) == 0 ) return 0;
            nhp_init_net_login_info(loginInfo.szTradeServer1, loginInfo.sTradePort1, loginInfo.szQuoteServer1, loginInfo.sQuotePort1);
            return 1;
        }
        case 2: {
//            if(strlen(loginInfo.szTradeServer2) == 0 || strlen(loginInfo.szQuoteServer2) == 0) return 0;
            if(strlen(loginInfo.szTradeServer2) == 0) return 0;
            nhp_init_net_login_info(loginInfo.szTradeServer2, loginInfo.sTradePort2, loginInfo.szQuoteServer2, loginInfo.sQuotePort2);
            return 1;
        }
        case 3: {
//            if(strlen(loginInfo.szTradeServer3) == 0 || strlen(loginInfo.szQuoteServer3) == 0) return 0;
            if(strlen(loginInfo.szTradeServer3) == 0) return 0;
            nhp_init_net_login_info(loginInfo.szTradeServer3, loginInfo.sTradePort3, loginInfo.szQuoteServer3, loginInfo.sQuotePort3);
            return 1;
        }
        case 4: {
//            if(strlen(loginInfo.szTradeServer4) == 0 || strlen(loginInfo.szQuoteServer4) == 0) return 0;
            if(strlen(loginInfo.szTradeServer4) == 0) return 0;
            nhp_init_net_login_info(loginInfo.szTradeServer4, loginInfo.sTradePort4, loginInfo.szQuoteServer4, loginInfo.sQuotePort4);
            return 1;
        }
        default: break;
    }
    return 0;
}

bool on_nhp_session_balance_callback_handle(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen) {
    printBusLogEx("on_nhp_session_balance_callback_handle nCmd=%d  nLen=%d", nCmd, nLen);
    int iCount = nLen / sizeof(StuCustomLoginInfo);
    if(iCount > 0) {
        nhp_stop_session();
        memset(&loginInfo, 0, sizeof(loginInfo));
        memcpy(&loginInfo, (char*)pData, sizeof(StuCustomLoginInfo));
        
        printBusLogEx("1.tHost=%s tPort=%d  qHost=%s qPort=%d", loginInfo.szTradeServer, loginInfo.sTradePort, loginInfo.szQuoteServer,loginInfo.sQuotePort);
        printBusLogEx("2.tHost=%s tPort=%d  qHost=%s qPort=%d", loginInfo.szTradeServer1, loginInfo.sTradePort1, loginInfo.szQuoteServer1,loginInfo.sQuotePort1);
        printBusLogEx("3.tHost=%s tPort=%d  qHost=%s qPort=%d", loginInfo.szTradeServer2, loginInfo.sTradePort2, loginInfo.szQuoteServer2,loginInfo.sQuotePort2);
        printBusLogEx("4.tHost=%s tPort=%d  qHost=%s qPort=%d", loginInfo.szTradeServer3, loginInfo.sTradePort3, loginInfo.szQuoteServer3,loginInfo.sQuotePort3);
        printBusLogEx("5.tHost=%s tPort=%d  qHost=%s qPort=%d", loginInfo.szTradeServer4, loginInfo.sTradePort4, loginInfo.szQuoteServer4,loginInfo.sQuotePort4);
        
        useLoginInfoIdx = 0;
        useLoginNext();
    }
    return 1;
}

void nhp_init_net_balance(char * server, int port) {
    nhp_stop_session();
    nBalanceSID = CreateSession();
    SetSessionSocketPackType(nBalanceSID, 2);
    
    AddHost(nBalanceSID, 1, server, port);
    BindFirePackage(nBalanceSID, on_nhp_session_balance_callback_handle);
    BindFireSessionStatus(nBalanceSID, on_nhp_session_balance_status_callback_handle);
    BindNotifyEvent(nBalanceSID, on_nhp_recv_balance_notify_event);

    NotifyNetState(nBalanceSID, 1);
    StartSession(nBalanceSID);
    printBusLogEx("nhp_init_net_balance ==> StartSession nBalanceSID=[%d] sever=[%s] port=[%d]", nBalanceSID, server, port);
}

void nhp_clear_balance(){
    memset(&mBalanceInfo, 0, sizeof(stuBalanceInfo));
    mBalanceInfo.mSize = 0;
    mBalanceInfo.rIdx = 0;
    mBalanceInfo.idx  = 0;
}

int nhp_add_balance(char * server, int port){
    if(mBalanceInfo.mSize > 5) return 0;
    switch (mBalanceInfo.idx) {
        case 0: {
            mBalanceInfo.idx = 1;
            mBalanceInfo.mSize = 1;
            strcpy(mBalanceInfo.szServer, server);
            mBalanceInfo.sPort = port;
            break;
        }
        case 1: {
            mBalanceInfo.idx = 2;
            mBalanceInfo.mSize = 2;
            strcpy(mBalanceInfo.szServer1, server);
            mBalanceInfo.sPort1 = port;
            break;
        }
        case 2: {
            mBalanceInfo.idx = 3;
            mBalanceInfo.mSize = 3;
            strcpy(mBalanceInfo.szServer2, server);
            mBalanceInfo.sPort2 = port;
            break;
        }
        case 3: {
            mBalanceInfo.idx = 4;
            mBalanceInfo.mSize = 4;
            strcpy(mBalanceInfo.szServer3, server);
            mBalanceInfo.sPort3 = port;
            break;
        }
        case 4: {
            mBalanceInfo.mSize = 5;
            strcpy(mBalanceInfo.szServer4, server);
            mBalanceInfo.sPort4 = port;
            break;
        }
        default:
            break;
    }
    return 1;
}

void nhp_set_login_info(char * suser, char *  spwd){
    cache_suser_spwd(suser, spwd);
}

int use_start_balance_next(){
    switch (mBalanceInfo.rIdx) {
        case 0: {
            if(strlen(mBalanceInfo.szServer) == 0) return 0;
            nhp_init_net_balance(mBalanceInfo.szServer, mBalanceInfo.sPort);
            mBalanceInfo.rIdx = 1;
            return 1;
        }
        case 1: {
            if(strlen(mBalanceInfo.szServer1) == 0) return 0;
            nhp_init_net_balance(mBalanceInfo.szServer1, mBalanceInfo.sPort1);
            mBalanceInfo.rIdx = 2;
            return 1;
        }
        case 2: {
            if(strlen(mBalanceInfo.szServer2) == 0) return 0;
            nhp_init_net_balance(mBalanceInfo.szServer2, mBalanceInfo.sPort2);
            mBalanceInfo.rIdx = 3;
            return 1;
        }
        case 3: {
            if(strlen(mBalanceInfo.szServer3) == 0) return 0;
            nhp_init_net_balance(mBalanceInfo.szServer3, mBalanceInfo.sPort3);
            mBalanceInfo.rIdx = 4;
            return 1;
        }
        case 4: {
            if(strlen(mBalanceInfo.szServer4) == 0) return 0;
            nhp_init_net_balance(mBalanceInfo.szServer4, mBalanceInfo.sPort4);
            mBalanceInfo.rIdx = 0;
            return 1;
        }
        default:
            break;
    }
    return 0;
}

void nhp_start_balance(){
    mBalanceInfo.rIdx = 0;
    mBalanceInfo.idx = 0;
    useBalance = 1;
    use_start_balance_next();
}

//登录
void nhp_init_net_login_info(char * tradeserver, int tradeport, char * quoteserver, int quoteport)
{
//    nhp_init_net_balance("192.168.0.186", 1988);
    nhp_stop_session();
    hasZyjSymbol = 0;
    hasZsSymbol = 0;
    connection_ing = 1;
    
#if BALANCE_ENABLE
    nhp_init_net(loginInfo.szTradeServer, loginInfo.sTradePort, loginInfo.szQuoteServer, loginInfo.sQuotePort);
#else
    nhp_init_net(tradeserver, tradeport, quoteserver, quoteport);
#endif
    //缓存帐号密码 连接成功后登录
//        cache_suser_spwd(suser, spwd);
    //启动网络
    nhp_start_net();
}


void nhp_init_net_login_quote(char * quoteserver, int quoteport)
{
    nhp_stop_session();
    connection_ing = 1;
    
    //建立行情链路
    nQuoteSID = CreateSession();
    SetSessionSocketPackType(nQuoteSID, 1);
    AddHost(nQuoteSID, 1, quoteserver, quoteport);
    
    BindFirePackage(nQuoteSID, on_nhp_session_trans_package_callback_handle_quote);
    BindFireSessionStatus(nQuoteSID, on_nhp_session_status_callback_handle_quote);
    BindNotifyEvent(nQuoteSID, on_nhp_recv_notify_event_quote_new);
    printBusLogEx("nhp_init_net ==>  nSID=[%d] nQuoteSID=[%d] sever=[%s] port=[%d]", nSID, nQuoteSID, quoteserver, quoteport);
    
    //启动网络
    if (nQuoteSID > -1) {
        NotifyNetState(nQuoteSID, 1);
        StartSession(nQuoteSID);
        printBusLogEx("StartSession QuoteSID:%d", nQuoteSID);
    }
}

void nhp_stop_session()
{
    StopSession(nBalanceSID);
    nBalanceSID = -1;
    
    StopSession(nSID);
    nSID = 0;
    StopSession(nQuoteSID);
    nQuoteSID = 0;
}


int nhp_get_nseq()
{
    if(nSeq > 65565) nSeq = 1;
    return nSeq+=1;
}

void SendRequest(int nCmd, int nseq, const char * json_content)
{
    Send(nSID, nCmd, nseq, (LPBYTE)json_content, (int)strlen(json_content));
}

void SendRequestEx(const char* strCmd, int nseq, const char * json_content)
{
    Send(nSID, atoi(strCmd),nseq,(LPBYTE)json_content,(int)strlen(json_content));
}

//////////////////////////////金智手机行情请求/////////////////////////////////
////分笔
//int send_quote_STK_trace_data_req(const char* szCode, int nSize)
//{
//    int mSeq = nhp_get_nseq();
//    Send(nQuoteSID, _STKTRACEDATA, mSeq, (LPBYTE)szCode, nSize);
//    return mSeq;
//}
//
////分时
//int send_quote_STK_min_data_req(const char* szCode, int nSize)
//{
//    int mSeq = nhp_get_nseq();
//    Send(nQuoteSID, _STKMINUTEDATA, mSeq, (LPBYTE)szCode, nSize);
//    return mSeq;
//}

////K线请求
//int send_quote_his_data_req(const char* szCode, int nPeriodicType, int nNum, int nReqCount)
//{
//    //格式 : 名字 + | + 周期 + | + 当前数据个数
//    static char temp[128];
//    int dataSize = 0;
//    sprintf(temp, "%s|%d|%d|%d|", szCode, nPeriodicType, nNum, nReqCount);
//    dataSize = strlen(temp);
//    int mSeq = nhp_get_nseq();
//    Send(nQuoteSID, _HISKDATA, mSeq, (LPBYTE)temp, dataSize);
//    return mSeq;
//}

int nhp_send_quote_req(int nCmd, const char* data, int size){
    int mSeq = nhp_get_nseq();
    Send(nQuoteSID, nCmd, mSeq, (LPBYTE)data, size);
    return mSeq;
}





int nhp_send_zyj_report_data_info_req()
{
    int id = nhp_get_nseq();
    Send(nQuoteSID, REPORTDATA_ZYQ, 0, NULL, 0);
    return id;
}

//////////////////////////////金智手机交易客户端 START/////////////////////////////////

int nhp_trade_inout_monery(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_MONEYINOUTAPPLY_REQ, mSeq, json_content);
    return mSeq;
}

//登录
int nhp_trade_login(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_TRADELOGIN_REQ, mSeq, json_content);
    return mSeq;
}





//查资金
int nhp_trade_funds(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_FUNDS_REQ, mSeq, json_content);
    return mSeq;
}

//查询出入金应答
int nhp_query_trade_inout_monery(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_MONEY_IN_OUT_REQ, mSeq, json_content);
    return mSeq;
}
//资金流水
int nhp_trade_monery_flowing(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_FUNDSLOG_REQ, mSeq, json_content);
    return mSeq;
}

//查当前资金流水请求
int nhp_trade_current_monery_flowing(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_FUNDSLOG_REQ, mSeq, json_content);
    return mSeq;
}

//查持仓单
int nhp_trade_order(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_ORDER_REQ, mSeq, json_content);
    return mSeq;
}

//查历史单据
int nhp_trade_hisorder(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_HISTORYREPORT_REQ, mSeq, json_content);
    return mSeq;
}



//公告抬头查询
int nhp_trade_request_declare_title(const char * json_content){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_DECLARETITLE_REQ, mSeq, json_content);
    return mSeq;

}

//公告内容查询
int nhp_trade_request_declare_content(int mSeq1, const char * json_content){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_DECLARECONTENT_REQ, mSeq, json_content);
    return mSeq;

}

//修改密码
int nhp_trade_request_modify_passwd(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_MODIFYPWD_REQ, mSeq, json);
    return mSeq;
}

//竞价查历史持仓单单请求
int nhp_zyj_trade_hisorder(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_CAU_HIS_ORDER_POSITION_REQ, mSeq, json_content);
    return mSeq;
}

//下中远期订单
int nhp_zyj_trade_open(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_CAU_TRADE_REQ, mSeq, json_content);
    return mSeq;
}

//查询提货意向
int nhp_query_apply_delivery_intention(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_BILLOFLADING_REQ, mSeq, json_content);
    return mSeq;
}

//“提货意向”申请
int nhp_apply_delivery_intention(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_BILLOFLADING_ORDER_REQ, mSeq, json_content);
    return mSeq;
}

//操作"提货意向请求"
int nhp_op_apply_delivery_intention(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_OP_BILLOFLADING_REQ, mSeq, json_content);
    return mSeq;
}

//修改提货意向请求
int nhp_mod_apply_delivery_intention(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_MOD_BILLOFLADING_REQ, mSeq, json_content);
    return mSeq;
}

//获取”交收仓库信息”请求
int nhp_query_sellt_store(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_SELLT_WAREHOUSE_REQ, mSeq, json_content);
    return mSeq;
}

//获取”商品属性”请求
int nhp_query_in_store_goods_property(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_GOODSATTR_REQ, mSeq, json_content);
    return mSeq;
}

//查询”入库单”请求
int nhp_query_in_store_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_IMPORT_REQ, mSeq, json_content);
    return mSeq;
}

//入库单转出库单” 请求
int nhp_instore_to_exportstore_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_IMPORT_TO_EXPORT_REQ, mSeq, json_content);
    return mSeq;
}

//入库单转仓单请求
int nhp_instore_to_cang_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_IMPORT_TO_CANG_REQ, mSeq, json_content);
    return mSeq;
}


//查询 “入库请求”请求
int nhp_query_apply_in_store(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_IMPORT_APPLY_REQ, mSeq, json_content);
    return mSeq;
}

//请求“入库申请”请求
int nhp_submit_apply_in_store(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_IMPORT_APPLY_REQ, mSeq, json_content);
    return mSeq;
}

//修改“入库申请”
int nhp_mod_apply_in_store(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_MOD_IMPORT_APPLY_REQ, mSeq, json_content);
    return mSeq;
}

//操作“入库申请”
int nhp_op_apply_in_store(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_OP_IMPORT_APPLY_REQ, mSeq, json_content);
    return mSeq;
}

//查询仓单请求
int nhp_query_cang_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_CANG_REQ, mSeq, json_content);
    return mSeq;
}

//操作"仓单"
int nhp_op_cang_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_OP_CANG_REQ, mSeq, json_content);
    return mSeq;
}

//查询”出库单”请求
int nhp_query_out_store_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_EXPORT_REQ, mSeq, json_content);
    return mSeq;
}

//仓单转入库单请求
int nhp_query_cang_to_instore_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_CANG_TO_IMPORT_REQ, mSeq, json_content);
    return mSeq;
}

//操作"仓单转入库单"
int nhp_op_cang_to_instore_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_OP_CANG_TO_IMPORT_REQ, mSeq, json_content);
    return mSeq;
}

//提交 "仓单转入库单" 请求
int nhp_cang_to_instore_order(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_CANG_TO_IMPORT_REQ, mSeq, json_content);
    return mSeq;
}


//提交 "申报交收" 请求
int nhp_apply_delivery_settle(const char * json_content) {
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_CAU_DELIVERY_TRADE_REQ, mSeq, json_content);
    return mSeq;
}

//////////////////////////////金智手机交易客户端 END  /////////////////////////////////
const char * get_str_user()
{
    return str_user;
}

const char * get_str_pwd()
{
    return str_pwd;
}

int  get_n_pwd_md5()
{
    return n_pwd_md5;
}


//2.0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int zyj_nhp_trade_request_symbol(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_CAU_SYMBOL_REQ, mSeq, json);
    return mSeq;
}

int zyj_nhp_trade_hold_order(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_CAU_ORDER_POSITION_REQ, mSeq, json);
    return mSeq;
}

int zyj_nhp_trade_settle_order(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_WAREHOUSE_QRY_SETTLE_REQ, mSeq, json);
    return mSeq;
}

int zyj_nhp_trade_delegate_order(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_CAU_ORDER_REQ, mSeq, json);
    return mSeq;
}

int zyj_nhp_trade_deal_order(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QRY_CAU_ORDER_BARGAIN_REQ, mSeq, json);
    return mSeq;
}

void nhp_init() {
    
}

/*******************************
            文交所
 *******************************/
//请求商品
int nhp_trade_request_symbol(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    //    SendRequestEx(FIX_MSGTYPE_SYMBOL_REQ, mSeq, json_content);
    SendRequestEx(FIX_MSGTYPE_QUERY_PRODUCT_REQ, mSeq, json_content);
    return mSeq;
}

//下订单
int nhp_trade_open(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_TRADE_REQ, mSeq, json_content);
    return mSeq;
}

// 查今日委托请求
int nhp_trade_today_entrust(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QUERY_TODAYORDER_REQ, mSeq, json_content);
    return mSeq;
}

// 查今日成交请求
int nhp_trade_today_deal(const char * json_content)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QUERY_TODAYTURNOVER_REQ, mSeq, json_content);
    return mSeq;
}

// 撤单
int nhp_trade_cancel_order(const char * json){
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_CANCELTRADE_REQ, mSeq, json);
    return mSeq;
}

int nhp_trade_sharehold_rsp(const char * json)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_SHAREHOLD_REQ, mSeq, json);
    return mSeq;
}

// 资金持仓
int nhp_trade_moneyhold_req(const char * json)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_MONEYHOLD_REQ, mSeq, json);
    return mSeq;
}

// K线数据请求+个股分析
int nhp_quote_kline_req(int type, char *code){
    int mSeq = nhp_get_nseq();
    Send(nQuoteSID, type, mSeq, (LPBYTE)code, 0);
    return mSeq;
}

int nhp_quote_stktrace_req(char *code){
    int mSeq = nhp_get_nseq();
    Send(nQuoteSID, STKDEALDATA, mSeq, (LPBYTE)code, 0);
    return mSeq;
}

//初始化市场
int nhp_send_init_market_req()
{
    int id = nhp_get_nseq();
    Send(nQuoteSID, INITMARKET, id, NULL, 0);//初始化中远期
    //    Send(nQuoteSID, INIT_MARKET_ZYQ, id, NULL, 0);
    return id;
}

//请求盘面数据
int nhp_send_report_data_info_req(char *productIDs)
{

    int id = nhp_get_nseq();
    Send(nQuoteSID, REPORTDATA, id, (LPBYTE)productIDs, 0);
    return id;

}

int nhp_trade_historyorder_req(const char * json)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QUERY_HISTORYORDER_REQ, mSeq, json);
    return mSeq;
}
int nhp_trade_historyturnover_req(const char *json)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_QUERY_HISTORYTURNOVER_REQ, mSeq, json);
    return mSeq;
}

int nhp_pretrade_req(const char *json)
{
    int mSeq = nhp_get_nseq();
    SendRequestEx(FIX_MSGTYPE_PRETRADE_REQ, mSeq, json);
    return mSeq;
}