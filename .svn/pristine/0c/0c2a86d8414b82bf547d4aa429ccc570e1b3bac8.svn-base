//
//  DataEngineWrapper.h
//  FT Trader
//
//  Created by SZFT1 on 14/11/25.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#ifndef FT_Trader_DataEngineWrapper_h
#define FT_Trader_DataEngineWrapper_h

#ifdef __cplusplus
extern "C" {
#endif
    
    //界面层事件
    typedef struct EngineEvents {
        //登录应答
        void (*login_res)(int type, int nRet);
        
        //登录进度通知
//        void (*login_progress_res)(int progress);
//        
//        //踢用户下线
//        void (*trade_ui_kick_notify)();
//        
//        //商品信息变化 symbolcodes
//        void(*trade_ui_symbol_change_notice)(int nRet, const char * symbolcodes);
//        
//        //历史订单刷新通知
//        void(*trade_ui_hisorder_rsp)( int nRet, const char * ordercodes);
//        
//        //持仓单刷新通知
//        void(*trade_ui_holdorder_rsp)(int nRet,const char * opencodes, const char * limitcodes, const char * closecodes);
//        
//        //资金刷新通知
//        void(*trade_ui_funds_change_rsp)(int nRet);
//        
//        //下单应答
//        void(*trade_ui_openorder_rsp)(int nRet, const char * orderticket, const char * symbolcode , double orderprice);
//        
//        //汇总持仓视图 商品信息变化
//        void(*trade_ui_openorder_stat_rsp)(int nRet, const char * symbolcode);
//        
//        //告诉UI分时已经接收完成了
//        void (*quote_ui_min_data_recv_notify)(int nSeq, char* symbolCode);
//        
//        //告诉UI历史已经接收完成了
//        void (*quote_ui_his_data_recv_notify)(int nSeq, char cType, char* symbolCode);
//        //告诉UI分笔已经接收完成了
//        void (*quote_ui_record_data_recv_notify)(int nSeq, char* sybolCode);
//        
//        //2014.08.04刷新回调
//        void (*trade_ui_refresh_complete)();
//        
//        //实时行情
//        void (*quote_real_notify)(char * symbolcode, double price, long long second);
        
        //2.0
    } EngineEvents;
    
    //初始化
    void instanceDataEngine(EngineEvents events);
    void process_event();
    
#ifdef __cplusplus
};
#endif

#endif
