//
//  DataEngine.cpp
//  FT Trader
//
//  Created by SZFT1 on 14/11/25.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#include "DataEngine.h"
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


int trade_session_sid;
int quote_session_sid;

//#define TRADE_SESSION_SID (1)
//#define QUOTE_SESSION_SID (2)

bool on_quote_session_trans_package_handle(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen){
    
    return 1;
}

bool on_trade_session_trans_package_handle(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen){
    
    return 1;
}

//网络状态回调
bool on_quote_session_status_callback(int nSid,int nFlag)
{
    printBusLogEx("on_nhp_session_status_callback_handle_quote nSid:%d nFlag:%d", nSid, nFlag);
//    quoteStuats = nFlag;
    //行情连接成功发登录包
    if (CST_CONNECTED == nFlag) {
//        Send(quote_session_sid, QUOTE_MSG_USERPWDVALID, 0, NULL, 0);
    }
//    netevents.nhp_net_status_rsp(QUOTE_TYPE, nFlag);
    return 1;
}

//网络状态回调
bool on_trade_session_status_callback(int nSid, int nFlag)
{
    printBusLogEx("on_nhp_session_status_callback_handle nSid:%d nFlag:%d", nSid, nFlag);
//    //交易
//    if (trade_session_sid == nSid) {
        if (CST_CONNECTED == nFlag) {
//            if (strlen(str_user) > 0 && strlen(str_pwd) > 0) {
//                tradeevents.callback_trade_login(str_user, str_pwd);
//            }
        }
//        netevents.nhp_net_status_rsp(TRADE_TYPE, nFlag);
//    }/Users/szft1/Desktop/Projects/CultureTrade/CultureTrade/Classes/Lib/ctradebiz/nethelper.c
    return 1;
}

//网络层有事件通知，切换会主线程来处理事件
bool on_nhp_recv_notify_event_quote(int nSid)
{
    //todo
    //netevents.on_nhp_recv_notify_event();
    return 1;
}
//网络层有事件通知，切换会主线程来处理事件
bool on_nhp_recv_notify_event(int nSid)
{
    //todo
    //netevents.on_nhp_recv_notify_event();
    return 1;
}

DataEngine::DataEngine() {
    
}

void DataEngine::initNet(char * tradeserver, int tradeport, char * quoteserver, int quoteport) {
    //建立交易链路
    trade_session_sid = CreateSession();
    AddHost(trade_session_sid, 1, tradeserver, tradeport);
    BindFirePackage(trade_session_sid, on_trade_session_trans_package_handle);
    BindFireSessionStatus(trade_session_sid, on_trade_session_status_callback);
    BindNotifyEvent(trade_session_sid, on_nhp_recv_notify_event);

    //建立行情链路
    quote_session_sid = CreateSession();
    SetSessionSocketPackType(quote_session_sid, 1);
    AddHost(quote_session_sid, 1, quoteserver, quoteport);
    BindFirePackage(quote_session_sid, on_quote_session_trans_package_handle);
    BindFireSessionStatus(quote_session_sid, on_quote_session_status_callback);
    BindNotifyEvent(quote_session_sid, on_nhp_recv_notify_event_quote);
}

//定时读取网络层数据
void DataEngine::process_event(){
    ProcessEvent(trade_session_sid);
    ProcessEvent(quote_session_sid);
}




