//
//  nethelper.h
//  jxtMsgClient
//
//  Created by  on 13-1-27.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef jxtMsgClient_nethelper_h
#define jxtMsgClient_nethelper_h

#include "global.h"
#import "tradedef.h"
#ifdef __cplusplus
extern "C"
{
#endif
    
#define                                   USERPWDVALID               0x8b00 //用户密码校验
#define                                   PUSH_REALDATA              0x0b00//主动推送实时数据
//#define                                   REPORTDATA                 0x0200//报价表
#define                                   HISKDATA                   0x0600//历史K线数据
#define                                   STKMINUTEDATA              0x1100//分时成交
#define                                   STKTRACEDATA               0x0100//成交明细
#define                                   INITMARKET                 0x1000//商品信息
    
    
    ///---------------------目前PC客户端支持的协议
    
#define                                   INIT_MARKET_ZYQ             0x1a00//中远期
#define                                   CLOSE_MAKRKET_ZYQ		      0x1aff // 收市消息
#define                                   OPEN_MAKRKET_ZYQ		      0x1afe // 收市消息
    
#define                                   REPORTDATA_DETAIL_ZYQ       0x02a1
    
    // 请求K线数据
#define                                   HISKDATA_ZYQ                0x06a0
    
    // 请求分时走势
#define                                   STKMINUTEDATA_ZYQ           0x11a0
    
    // 成交明细
#define                                   STKTRACEDATA_ZYQ            0x01a0
    
#define                                   PUSH_REALDATA_ZYQ           0x0ba0//(报价)实时表推送
    //(图表详细)实时推送
#define                                   PUSH_REALDATA_DETAIL_ZYQ    0x0ba1
    // 实时推送报价表列表(注册改变)
#define                                   PUSH_REPORTDATA_LIST_ZYQ    0x0ba2
    
#define                                   REPORTDATA_ZYQ              0x02a0//报价表
    
    // 请求综合排名
#define                                   COMPLEX_RANKING_ZYQ         0x02a4
    
    // 推送综合排名
#define                                   PUSH_COMPLEX_RANKING_ZYQ    0x0ba3
//////////////金智行情//////////////////////
#pragma pack(1)

    
    // 回应结构
   typedef struct _stuCustomLoginInfo
    {//客户端发送信息
        char szTradeServer[16];
        short sTradePort;
        char szQuoteServer[16];
        short sQuotePort;
        
        char szTradeServer1[16];
        short sTradePort1;
        char szQuoteServer1[16];
        short sQuotePort1;
        
        char szTradeServer2[16];
        short sTradePort2;
        char szQuoteServer2[16];
        short sQuotePort2;
        
        char szTradeServer3[16];
        short sTradePort3;
        char szQuoteServer3[16];
        short sQuotePort3;
        
        char szTradeServer4[16];
        short sTradePort4;
        char szQuoteServer4[16];
        short sQuotePort4;
    }StuCustomLoginInfo;

    typedef struct _stuBalanceInfo {
        short idx;
        short rIdx;
        short mSize;
        //客户端发送信息
        char szServer[16];
        short sPort;
        
        char szServer1[16];
        short sPort1;
        
        char szServer2[16];
        short sPort2;
        
        char szServer3[16];
        short sPort3;
        
        char szServer4[16];
        short sPort4;
    } stuBalanceInfo;
    
////止盈止损提示
//typedef struct _SL_TP_Hint
//{
//    char price[32];
//    char SlPrice[32];
//    char TpPrice[32];
//}SL_TP_Hint;
    
typedef  struct _SCodeInfo
{
    char  szCode[11]; //名称
    char  cPriceUnit;//价格位数
}CSCodeInfo;
    
typedef struct _StuRecvQuoteInfo
{
    CSCodeInfo sCode;
    unsigned int uiPrice;
    uint64_t lastTick;
}CStuRecvQuoteInfo;

//分时
typedef struct _STraceUnit
{
    unsigned int uiDate;//日期
    unsigned int uiTime;//时间
    unsigned int uiPrice;//价格
}CSTraceUnit;
    
typedef struct _SHisotyDataReq
{
    int nDataType;//历史K线种类
    int nHasNum;  //当前已有数量
    int nAskNum;  //请求数量
}CSHisotyDataReq;
    
//K线数据结构
typedef struct _SHisKUnit{
    unsigned int uiDate;//日期
    unsigned int uiTime;//时间
    unsigned int uiOpenPrice;//开盘价
    unsigned int uiMaxPrice;//高
    unsigned int uiMinPrice;//低
    unsigned int uiClosePrice;//收
}CSHisKUnit;

///////////////
//实时数据
typedef struct	_SNowData
{
    uint64_t	timeUpdate;
    CSCodeInfo	sCode;	  //产品代码
    unsigned int uiOpenPrice;//今开盘
    unsigned int uiMaxPrice;//最高价
    unsigned int uiMinPrice;//最低价
    unsigned int uiNewPrice;//最新价
}CSNowData;


    
typedef struct _SHisKUnitMA
{
    unsigned int uiDate;//日期
    unsigned int uiTime;//时间
    unsigned int uiOpenPrice;//开盘价
    unsigned int uiMaxPrice;//高
    unsigned int uiMinPrice;//低
    unsigned int uiClosePrice;//收
    unsigned int m_uiMA[4];
}CSHisKUnitMA;
    
typedef struct _StuSymbolData
{
    CSCodeInfo sCode;
    char szName[20];
    char cTrade;//是否交易 0：非交易 1：交易
    int	 nBeginTime;//交易开始时间
    int  nTotalTradeTime;//总交易时间
}CStuSymbolData;

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

    
typedef struct _SUserPwdDataReq
{
    char m_szUser[20];
    char m_szPwd[64];
    unsigned int m_uiSoftWareVersion;
    char m_szHardDiskInfo[17];
}SUserPwdDataReq; //用户名和密码校验

    
    struct SHisotyDataReq
    {
        int nDataType;//历史K线种类
        int nHasNum;//当前已有数量
        int nAskNum;//请求数量
    };
    
    
    
    
    
#pragma pack()
    
//获取发包序号
int nhp_get_nseq();

//关闭网络 
void nhp_stop_session();
    
//
    void nhp_start_net();


    typedef enum enumConnectStatusType
    {
        CST_CONNECTING		= 0x1,//正在连接...
        CST_CONNECTED		= 0x2,//已连接,正在登录...
        CST_UNCONNECTED		= 0x3,//未连接
        CST_RECONNECT		= 0x4,//正在重新连接...
        CST_ERROR_CONNECT	= 0x5,//连接失败
        CST_CLIENT_SENDDATA	= 0x6, //表示发送数据时关闭SOCKET
        CST_CLIENT_ENUMNETWORKEVENTS	= 0x7, //表示EnumNetworkEvents出错关闭SOCKET
        CST_CLIENT_JUDGE_CLOSE			= 0x8, //表示客户端在9s内无网络事件，关闭SOCKET
        CST_SOCKET5_ERROR	= 0x9, //  表示使用SOCKET5代理服务登录
        CST_SOCKET4_ERROR	= 0xA //  表示使用SOCKET4代理服务登录
        
    }ConnectStatusType;
    
    typedef struct nhp_net_events
    {
        //网络状态回调
        /****
         * 见上 ConnectStatusType
         * 1、将对应的状态显示在UI上，如果是已连接显示
         ****/
        void(*nhp_net_status_rsp)(int type, int nFlag);
        
        //收到网络事件通知
        void(*nhp_recv_notify_event)();
        
    }nhp_net_events;
    

//业务事件
typedef struct nhp_trade_events
{
    //登录应答
    void(*on_nhp_trade_login_rsp)(char* json);
    
    //行情服务登录应答
    void(*on_nhp_quote_login_rsp)(int nRet);
    
    //登出应答
    //ret: 0 登出成功  <0 登出失败
    void(*on_nhp_loginout_rsp)(int ret);
    
    //收到踢当前用户下线
    void(*on_nhp_ca_kick_notify)();
    
    //下订单
    void(*on_nhp_trade_open_rsp)(int nSeq, char * json);
    
    //查资金
    void(*on_nhp_trade_moneyhold_rsp)(char * json);
    
    //查持仓单
    void(*on_nhp_trade_order_rsp)(char * json);
    
    //查历史单据
    void(*on_nhp_trade_hisorder_rsp)(int nSeq, char * json);
    
    

    //实时行情
    //void(*on_nhp_quote_real)(char * symbolcode, double price, long long time);
    void(*on_nhp_quote_real)(const char * symbolcode, int iCount);
    
    //盘面数据
    //int(*on_nhp_quote_pankou)(char * symbolcode, double openprice, double maxprice, double minprice, double newprice, long long time);
    int(*on_nhp_quote_pankou)(int nSeq, const char * pData, int iCount);
    
    //数据变化完毕
    void(*on_nhp_quote_change_end)(int count, char * symbolcodes);
    
    //是否是有效行情
    int(*callback_check_valid_quote_real_rsp)(char * symbolcode);
    
    int(*callback_get_symbol_num)();
    
    //回调实际登录
    void(*callback_trade_login)(char * account, char *password);
    
    //回调查资金
    int(*callback_trade_funds)();
    void (*callback_sign_status_notice)(int nSeq, const char* json);
    //回调查持仓单
    int(*callback_trade_order)();
    
    //回调查商品
    int(*callback_trade_request_symbol)();
    
//    void (*on_min_data_recv_end)(int nSeq, char* name, CSTraceUnit* array, int arrLen);
//    void (*on_trace_data_recv_end)(int nSeq, char* name, CSTraceUnit* array, int arrLen);//分笔线
//    void (*on_kline_data_recv_end)(int nSeq, char* name, CSTraceUnit* array, int arrLen);
    void (*on_min_data_recv_end)(int nSeq, char* pData, int arrLen);
    void (*on_trace_data_recv_end)(int nSeq, char* pData, int arrLen);//分笔线
    void (*on_kline_data_recv_end)(int nSeq, char* pData, int arrLen);
    
    
    void (*on_zyq_symbol_data_info_end)(unsigned char* pData, int count);
    //2.0
    //查竞价帐户商品信息请求
    int (*call_handler_zyj_symbol)(int nSeq,const char * json);
    //竞价查持仓单单请求
    void (*call_handler_zyj_hold_order)(int nSeq,const char * json);
    //交收单回调
    void (*call_handler_zyj_settle_order)(int nSeq,const char * json);
    
    //竞价查委托单单请求
    void (*call_handler_zyj_delegate_order)(int nSeq,const char * json);
    //竞价查成交单单请求
    void (*call_handler_zyj_deal_order)(int nSeq,const char * json);
    //
    void (*call_handler_zyj_sumbit_order)(int nSeq,const char * json);
    
    /******************************START*************************************/
    //新加接口
    //公告抬头查询
    void (*call_handler_declare_title)(int nSeq,const char * json);
    
    //公告内容查询
    void (*call_handler_declare_content)(int nSeq,const char * json);
    
    void (*call_handler_inout_monery)(int nSeq,const char * json);
    
    void (*call_handler_funds_flowing)(int nSeq,const char * json);
    
    //公告rwy
    void (*call_handler_declare_notify)(const char * json);
    
    void (*call_handler_modifypasswd)(int nSeq, const char * json);
    
    //中远期盘面数据
    int(*call_handler_zyj_quote_pankou)(int nSeq, const char* data, int count);
    void(*call_handler_zyj_quote_pankou_detail)(int nSeq, const char* data, int count);
    void(*call_handler_openclose_market)(int type, int status);
    void(*call_handler_zyq_kline_data_recv_end)(int nSeq, const char* pData, int iCount);
    void(*call_handler_zyq_min_data_recv_end)(int nSeq, const char* pData, int iCount);//请求分时走势

    void(*call_handler_zyq_trace_data_recv_end)(int nSeq, const char* pData, int iCount);//成交明细

    void(*call_handler_zyj_quote_real)(const  char* pData, int nLen);//(报价)实时表推送
    void(*call_handler_zyj_quote_real_detail)(const  char* pData, int nLen);//(图表详细)实时推送
    void(*call_handler_zyj_quote_real_list)(const char*pData, int nLen);//实时推送报价表列表(注册改变)
    void(*call_handler_zyj_complex_ranking)(int nSeq, const  char* pData, int nLen);//请求综合排名   推送综合排名

    
    //查询仓单请求
    void (*call_handler_apply_delivery_intention)(int nSeq, const char * json);
    
    void (*call_handler_query_apply_delivery_intention)(int nSeq, const char * json);
    
    void (*call_trade_refresh)(int n);
    
    
    void (*call_handler_sellte_notify)(const char * json);
    void (*call_handler_outstore_order_notify)(const char * json);
    void (*call_handler_apply_instore_notify)(const char * json);
    void (*call_handler_cang_order_notify)(const char * json);
    void (*call_handler_apply_delivery_notify)(const char * json);
    void (*call_handler_instore_order_notify)(const char * json);
    void (*call_handler_cangorder_to_instore_notify)(const char * json);

    
    //仓库
    void (*call_handler_query_sellte_store_info)(int nSeq, const char * json);
    
    //商品
    void (*call_handler_query_in_store_goods_property)(int nSeq, const char * json);
    
    void (*call_handler_query_in_store_order)(int nSeq, const char * json);
    
    void (*call_handler_query_apply_in_store)(int nSeq, const char * json);
    
    
    void (*call_handler_query_cang)(int nSeq, const char * json);
    
    //查询”出库单”请求
    void (*call_handler_query_out_store_order)(int nSeq, const char * json);
    
    void (*call_handler_apply_in_store)(int nSeq, const char * json);
    
    void (*call_handler_query_cang_to_in_store_order)(int nSeq, const char * json);
    
    void (*call_handler_op_cang_to_in_store_order_response)(int nSeq, const char * json);
    
    void (*call_handler_cang_to_in_store_order_response)(int nSeq, const char * json);
    
    //入库单操作
    void (*call_handler_op_in_store_order_response)(int nSeq, const char * json);
    
    //提交申报交收回调
    void (*call_handler_apply_delivery_sellte_rsp)(int nSeq, const char * json);
    
    void (*call_handler_inout_money_rsp)(int nSeq, const char * json);
    
    /**************************************************
                        文交所
     **************************************************/
    
    
    //查商品
    void(*on_nhp_trade_request_symbol_rsp)(char * json);
    
    //委托应答
    void(*on_nhp_trade_entrust_order_rsp)(char * json);
    
    //成交应答
    void(*on_nhp_trade_deal_order_rsp)(char *json);
    
    //持仓份额应答
    void(*on_nhp_trade_sharehold_rsp)(char *json);
    
    //历史委托
    void(*on_nhp_trade_historyorder_rsp)(int nSeq,char *json);
    
    //历史成交
    void(*on_nhp_trade_historyturnover_rsp)(int nSeq,char *json);
    
    // 首次历史K线回调
    void(*on_nhp_quote_hisKDataFirst_rsp)(int nCmd,unsigned char* data,int nLen);
    
    // 当日K线回调
    void(*on_nhp_quote_hisKDataCurDate_rsp)(int nCmd,unsigned char* data,int nLen);
    
    // 实时数据
    void(*on_nhp_quote_stkDealData_rsp)(unsigned char* data);
    
    // 初始化市场
    void (*on_nhp_quote_initMarket_rsp)(const char* pData, int count);
    
} nhp_trade_events; // 从nethelper回调
    
    //设定网络事件回调
    void set_nhp_net_events(nhp_net_events events);
    
    //收到网络事件通知后切换主线程处理事件
    void nhp_process_event();
    
    //通知网络状态 0网络不可用 1网络可用
    void nhp_notify_net_state(int ntype,int state);
    
    void nhp_refresh();
    void refresh_begin();
    void refresh_end();
    void set_nhp_trade_events(nhp_trade_events events);

    void nhp_request_login();
    
    int getTradeStauts();
    int getQuoteStauts();

    void nhp_init();
    
    int nhp_add_balance(char * server, int port);
    void nhp_clear_balance();
    void nhp_set_login_info(char * suser, char *  spwd);
    
    void nhp_start_balance();
    
    int isUpdate_ZYQ_Report();
    
    //初始化网路信息
    void nhp_init_net_login_info(char * tradeserver, int tradeport, char * quoteserver, int quoteport);
    
    void nhp_init_net_login_quote(char * quoteserver, int quoteport);
//    //分笔行情获取
//    int send_quote_STK_trace_data_req(const char* szCode, int nszCodeSize);
//    //分时行情获取
//    int send_quote_STK_min_data_req(const char* szCode, int nszCodeSize);
    //k线行情
//    int send_quote_his_data_req(const char* szCode, int nPeriodicType, int nNum, int nReqCount);

    int nhp_send_quote_req(int nCmd, const char* data, int size);
    
//    初始化市场
    int nhp_send_init_market_req();

    //请求报价表
    int nhp_send_zyj_report_data_info_req();

    //////////////////////////////金智手机交易客户端 START/////////////////////////////////
    /**************************************************
                            文交所
     **************************************************/
    //////////////////////////////文交所客户端 START/////////////////////////////////
    //登录
    int nhp_trade_login(const char * json_content);
    
    int nhp_trade_today_entrust(const char * json_content);
    // 查今日成交请求
    int nhp_trade_today_deal(const char * json_content);
    
    // 撤单
    int nhp_trade_cancel_order(const char * json);
    
    //下订单
    int nhp_trade_open(const char * json_content);
    
    //持仓份额
    int nhp_trade_sharehold_rsp(const char * json);
    
    // 资金持仓
    int nhp_trade_moneyhold_req(const char * json);
    
    // k线请求
    int nhp_quote_kline_req(int type, char *code);
    
    // 个股分析
    int nhp_quote_stktrace_req(char *code);
    
    //请求报价表
    int nhp_send_report_data_info_req(char *productIDs);
    
    int nhp_trade_historyorder_req(const char * json);
    // 历史成交
    int nhp_trade_historyturnover_req(const char * json);
    
//////////////////////////////文交所客户端 END  /////////////////////////////////
    //查资金
    int nhp_trade_funds(const char * json_content);
    
    //查询出入金应答
    int nhp_query_trade_inout_monery(const char * json_content);
    //当前资金流水
    int nhp_trade_current_monery_flowing(const char * json_content);
    //资金流水
    int nhp_trade_monery_flowing(const char * json_content);
    
    //查持仓单
    int nhp_trade_order(const char * json_content);
    //查历史单据
    int nhp_trade_hisorder(const char * json_content);
    //查商品
    int nhp_trade_request_symbol(const char * json_content);
    
    //公告抬头查询
    int nhp_trade_request_declare_title(const char * json_content);
    
    //公告内容查询
    int nhp_trade_request_declare_content(int mSeq, const char * json_content);
        

    extern int getSymbolMinuteData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize);
    extern int getSymbolTraceData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize);
//////////////////////////////金智手机交易客户端 END  /////////////////////////////////
        
//    int getPankouNoticeCount();
//    int getRealNoitceCount();

    const char * get_str_user();
    
    //2.0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    int nhp_send_zyq_quote_report(const char* pData, int pLen);
    
    //查竞价帐户商品信息请求
    int zyj_nhp_trade_request_symbol(const char * json);
    //竞价查持仓单单请求
    int zyj_nhp_trade_hold_order(const char * json);
    //查交收单
    int zyj_nhp_trade_settle_order(const char * json);
    //竞价查委托单单请求
    int zyj_nhp_trade_delegate_order(const char * json);
    //竞价查成交单单请求
    int zyj_nhp_trade_deal_order(const char * json);
    
    int nhp_trade_request_modify_passwd(const char * json);
    
    //查询提货意向
    int nhp_query_apply_delivery_intention(const char * json);
    
    //提交”提货意向“申请
    int nhp_apply_delivery_intention(const char * json);
    
    //操作提货意向
    int nhp_op_apply_delivery_intention(const char * json_content);
        
    //修改提货意向请求
    int nhp_mod_apply_delivery_intention(const char * json_content);
    
    
    //下中远期历史订单
    int nhp_zyj_trade_hisorder(const char * json_content);
    
    //下中远期订单
    int nhp_zyj_trade_open(const char * json_content);
    
    
    
    //获取”交收仓库信息”请求
    int nhp_query_sellt_store(const char * json_content);
    
    // 获取”商品属性”请求
    int nhp_query_in_store_goods_property(const char * json_content);
    
    //查询”入库单”请求
    int nhp_query_in_store_order(const char * json_content);
    
    //查询 “入库请求”请求
    int nhp_query_apply_in_store(const char * json_content);
    
    //提交“入库申请”请求
    int nhp_submit_apply_in_store(const char * json_content);
    //修改“入库申请”请求
    int nhp_mod_apply_in_store(const char * json_content);
    
    //操作“入库申请”
    int nhp_op_apply_in_store(const char * json_content);
    
    //查询仓单请求
    int nhp_query_cang_order(const char * json_content);
        
    //查询”出库单”请求
    int nhp_query_out_store_order(const char * json_content);
    
    //仓单转入库单请求
    int nhp_query_cang_to_instore_order(const char * json_content);
    
    //提交 “仓库转入库单”
    int nhp_cang_to_instore_order(const char * json_content);
    
    //操作 “仓库转入库单”
    int nhp_op_cang_to_instore_order(const char * json_content);
    //入库单转出库单” 请求
    int nhp_instore_to_exportstore_order(const char * json_content);
    //入库单转仓单请求
    int nhp_instore_to_cang_order(const char * json_content);
    
    //提交 "申报交收" 请求
    int nhp_apply_delivery_settle(const char * json_content);
    
    int isWifi();
    
    
    int nhp_trade_inout_monery(const char * json_content);
#ifdef __cplusplus
}
#endif

#endif
