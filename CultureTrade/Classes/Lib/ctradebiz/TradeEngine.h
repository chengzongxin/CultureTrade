//
//  TradeEngine.h
//  jxtMsgClient
//
//  Created by zengxw on 13-11-10.
//
//  http://116.255.219.47:8081/Admin//1001001.txt

//表示公共层版本
#define VERSION "2.0.1"

#ifndef jxtMsgClient_TradeEngine_h
#define jxtMsgClient_TradeEngine_h



#ifdef __cplusplus
extern "C"
{
#endif  

#include "tradedef.h"
#include "nethelper.h"
#include "JsonConvert.h"
    

    
typedef enum _CHECKORDER
{
    CHECK_SUCCESS = 0,					//成功
    CHECK_NOSYMBOL = -1,				//无此商品编码
    CHECK_SIZETOOSMALL = -2,            //交易数量小于最低限制
    CHECK_SIZETOOLARGE = -3,            //交易数量大于最大限制
    CHECK_TPSLERROR    = -4,            //止赢停损价设置不正确
    CHECK_TPTOOLOW     = -5,            //止赢价距离现价太近
    CHECK_SLTOOLOW     = -6,            //停损价距离现价太近
    CHECK_PRICETOONEAR = -7,            //输入的价格距离现价太近
    CHECK_ORDERCODEERROR = -8,          //无此单号
    CHECK_STEP_SIZE = -9,               //无此单号
    CHECK_FAIL = -10,                   //失败
    CHECK_RSIZETOOSMALL = -11,          //反手交易数量小于最低限制
    CHECK_RSIZETOOLARGE = -12,          //反手交易数量大于最大限制
    CHECK_MAXDEVIATION = -13,           //交易偏离点差大于最大限制
    CHECK_PRICE = -14,                  //交易偏离点差大于最大限制
    CHECK_PHONE_PARAMS_FAIL = -15,            //
    
    CHECK_PARAMS_FAIL = -8203
    
}CHECKORDER;
    

//extern int trading;
    
//界面层事件
typedef struct trade_ui_events
{
    //登录应答
    void (*trade_ui_login_rsp)(int type, int nRet);
    
    //登录进度通知
    void (*trade_ui_login_progress_rsp)(int progress, int max);
    
    //踢用户下线
    void (*trade_ui_kick_notify)();
    
    void(*call_funds_flowing)(int nSeq, int nRet);
    
    //历史订单刷新通知
    void(*trade_ui_hisorder_rsp)(int nSeq, int nRet, const char * ordercodes);
    
    //持仓单刷新通知
    void(*trade_ui_holdorder_rsp)(int nRet,const char * opencodes, const char * limitcodes, const char * closecodes);
    
    
    
    //下单应答
    void(*trade_ui_openorder_rsp)(int nSeq, int nRet, long long orderticket);
    
    //汇总持仓视图 商品信息变化
    void(*trade_ui_openorder_stat_rsp)(int nRet, const char * symbolcode);
    
    
    //告诉UI分时已经接收完成了
    void (*on_quote_minute_data_notify)(int nSeq, char* symbolCode);
    //告诉UI历史已经接收完成了
    void (*on_quote_history_data_notify)(int nSeq, char cType, char* symbolCode);
    //告诉UI分笔已经接收完成了
    void (*on_quote_trace_data_notify)(int nSeq, char* sybolCode);
    void (*on_quote_detail_report_notify)(int nSeq, int nRet, char* sybolCode);
    void (*on_quote_complex_ranking_notify)(int nSeq);
    void (*on_quote_real_detail_notify)(char* sybolCode);
    //2014.08.04刷新回调
    void (*trade_ui_refresh_complete)(int nRet);
    
    //实时行情
    void (*quote_real_notify)(char * symbolcode, double price, long long second);
    
    
    //2.0
    void (*history_change_notify)(char * symbolcode, int price, long long second);
    
//    void(*trade_ui_open_zyj_order_rsp)(int nSeq, int nRet, long long orderticket);
    
    
    
    void (*on_bargain_notify)(int nRet);//交收单
    
    void (*on_sellte_notify)(int nRet);
    void (*on_outstore_order_notify)(int nRet);
    void (*on_apply_instore_notify)(int nRet);
    void (*on_cang_order_notify)(int nRet);
    void (*on_apply_delivery_notify)(int nRet);
    void (*on_instore_order_notify)(int nRet);
    void (*on_cangorder_to_instore_notify)(int nRet);
        
    void (*delcare_title_rsp)(int nSeq);
    void (*delcare_content_rsp)(int nSeq, const char * szContent);
    void (*delcare_notify)(int annId);
    void (*modify_passwd_rsp)(int nRet);
    
    void (*apply_delivery_intention_rsp)(int nSeq, int nRet);
    
    void (*op_cang_to_in_store_order_response)(int nSeq, int nRet);
    void (*op_cang_order_response)(int nSeq, int nRet);
    void (*op_in_store_order_response)(int nSeq, int nRet);
    
    void (*apply_in_store_rsp)(int nSeq, int nRet);
    
    void (*on_apply_delivery_sellte_rsp)(int nSeq, int nRet);
    
    //20145.09.18修改 增加参数
    void (*on_inout_money_rsp)(int nSeq, int nRet,const char* json);
    //20145.09.18增加
    void (*on_sign_change_notify)(int status);
    
    
    /********************************************************
                            文交所
     ********************************************************/
    
    //商品信息变化 symbolcodes
    void(*trade_ui_symbol_change_notice)(int nRet, const char * symbolcodes);
    
    //今日委托
    void(*trade_ui_today_entrust_rsp)(const char *data);
    
    //今日成交
    void(*trade_ui_today_deal_rsp)(const char *data);
    
    //持仓份额
    void(*trade_ui_sharehold_rsp)(int nRet, const char *data);
    
    // 历史委托
    void(*trade_ui_historyorder_rsp)(int nSeq,char *json);
    
    //历史成交
    void(*trade_ui_historyturnover_rsp)(int nSeq,char *json);
    
    //预交易
    void(*trade_ui_pretrade_rsp)(int nSeq,char *json);
    
    // 行情登陆
    void(*quote_ui_login_rsp)(int nRet);
    
    // K线数据回调
    void(*quote_ui_hisKDataFirst_rsp)(unsigned char *data,int count);
    
    // 当日K线回调
    void(*quote_ui_hisKDataCurDate_rsp)(unsigned char *data,int count);
    
    // 实时数据回调
    void(*quote_ui_stkDealData_rsp)(unsigned char *data);
    
    // 初始化市场
    void(*quote_ui_initMarket_rsp)(const char *data,int pLen);
    
    // 盘口数据
    void(*quote_ui_pankou_rsp)(const char *data,int len);
    
    //资金刷新通知
    void(*trade_ui_moneyhold_rsp)(char *data);
    
    //转账查询
    void(*trade_ui_transferquery_rsp)(int nRet,const char *data);
    
    void(*trade_ui_canceltrade_rsp)(int nSeq,char *json);
}trade_ui_events;      //回调给NSTradeEngine
    
    void init_engine();
    
    int add_balance(char * server, int port);
    
    void clear_balance();
    
    void set_login_info(char * suser, char *  spwd);
    
    void start_balance();
    
    //分笔行情获取
    int nhp_send_quote_STK_trace_data_req(const char* szCode, int nszCodeSize);
    //分时行情获取
    int nhp_send_quote_STK_min_data_req(const char* szCode, int nszCodeSize);
    //k线行情
    int nhp_send_quote_his_data_req(const char* szCode, int nPeriodicType, int nNum, int nReqCount);
    
    void set_trade_ui_events(trade_ui_events events);

        //回收内存  或者清除缓存
    void recoveryMemory();
        
    //请求登录 带地址信息
    void userloginwithnet(char * account, char * password, char * tradeip, int tradeport, char * quoteip, int quoteport);

    void loginquotewithnet(char * quoteip, int quoteport);
    
    int isRefresh();
    
    //重新刷新
    int refresh_account_info();
    
    
    
    //查历史单据 %04d-%02d-%02d
    int trade_request_hisorder( char * startdate, char * enddate);
    
    //下中远期历史订单
    int trade_request_zyj_hisorder(const char * startdate, const char * enddate);

    //查持仓单
    int trade_request_order();
        
    //查资金
    int trade_request_funds();
    
    //查询出入金应答
    int trade_query_inout_monery();
    //资金流水
    int trade_monery_flowing(const char* start, const char* end);
    //当前资金流水
    int trade_current_monery_flowing();
    //下市价单
//    int trade_open_market_order(char * symbolcode, char direction, double price, double shoushu);
    int trade_open_market_order(char * symbolcode, int shoushu, double sl, double tp, char *direction, double price, int diff);
    
    int trade_open_orderex(char * symbolcode, char *direction, double price, int shoushu);

    //下限价单
//    int trade_open_limit_order(char * symbolcode, double shoushu, double sl, double tp, char ordertype, double price, char valdate_type, char * valdate);

    const char * getLimitHintToString(char * symbolcode, char sub_order_type);
    const char * getLimitHintToString2(char * symbolcode, double price, char sub_order_type);
        
    //修改持仓单
    int trade_modify_open_order(long long orderticket, double sl, double tp);

    //修改限价单
    int trade_modify_limit_order(long long orderticket, double price, double sl, double tp);

    //删除限价单
    int trade_cancel_limit_order(long long orderticket);

    //平仓
    int trade_close_order(long long orderticket, double shoushu, double rqty, int diff);
    
/*****************************START*******************************/
    //新加接口
    //中远期
    int trade_cancel_zyj_order(long long orderticket);
    
    //公告抬头查询
    int trade_request_declare_title();
    
    //公告内容查询
    int trade_request_declare_content(int titleId);
    
    
    double getSymbolNewQty(char * symbolcode);
    double getSymbolNewPrice(char * symbolcode);
    char getSymbolTurnoverDirec(char * symbolcode);
    double getSymbolPriceRiseDown(char * symbolcode);
    double getSymbolPriceRiseDownRate(char * symbolcode);
    double getSymbolTotalTurnoverFunds(char * symbolcode);
    double getSymbolOrderRate(char * symbolcode);
    double getSymbolQtyRate(char * symbolcode);
    
    double getSymbolTotalTurnoverQty(char * symbolcode);
    double getSymbolBuyVolPrice(char * symbolcode);
    double getSymbolBuyVolQty(char * symbolcode);
    double getSymbolSellVolPrice(char * symbolcode);
    double getSymbolSellVolQty(char * symbolcode);
    
    //公告属性
    int getDeclareAnnouceSize();
    const char* getDeclareAnnouceID();//公告ID
    long long getDeclareTime(int annouceID);
    const char* getDelcare(int annouceID);//公告发布者
    const char* getDelcareTitle(int annouceID);//公告标题
    const char* getDelcareContent(int annouceID);//公告内容
    char getDelcareReadStatus(int annouceID);//阅读状态 1已阅读 2 未阅读
    
    
    //帐户信息
    int trade_request_modify_passwd(const char *type, const char* oldpass, const char* newpass);
    
    //开仓
    int trade_open_limit_zyj_order(char direction, const char * symbolcode, double shoushu, double price);
    //平今仓
    int trade_close_limit_zyj_day_order(char direction, const char * symbolcode, double shoushu, double price);
    //平昨仓
    int trade_close_limit_zyj_yesterday_order(char direction, const char * symbolcode, double shoushu, double price);
    
/*****************************END*******************************/
    
    
    const char* getVersion();
    
//获取用户属性
    //用户账号
    const char * getUserAccount();
    //用户状态
    int    getUserStatus();
    //用户名
    const char * getUserName();
    // session
    const char * getSession();
    //用户名
    const char * getLoginUser();
    
//获取商品属性
    //获取所有商品编码
    const char * getAllSymbolCode();
    
    //商品类型 1、表示坐市商，2、表示中远期
    int getSymbolType(char * symbolcode);
    int getSymbolShowOrder(char * symbolcode);
    //步长
    int getSymbolStepSize(char * symbolcode);
    //商品名
	const char * getSymbolname(char * symbolcode);
	//买价
	double getAskPrice(char * symbolcode);
    //卖价
	double getBidPrice(char * symbolcode);
	//行情时间
	long long getLastTick(char * symbolcode);
	//最高
    double getHighPrice(char * symbolcode);
	//最低
	double getLowPrice(char * symbolcode);
    
    //昨收价
//    double getClosePrice(char * symbolcode);
    //昨结价
    double getSettlPrice(char * symbolcode);
    //开盘价
    double getOpenPrice(char * symbolcode);
    //最新价
    double getLastPrice(char * symbolcode);
	//价格涨跌方向 -1跌 0平 1涨
	int getPriceDirection(char * symbolcode);
    //价格小数点位数
    int getPriceDecimal(char * symbolcode);
    
//获取订单属性
    //汇总
    //获取持仓单商品集合
    const char * getHoldSymbolCodes();
    //获取持仓单商品名
    const char * getHoldSymbolName(char * symbolcode);
    //持仓商品的总买单量
    double getHoldTotalBuyNum(char * symbolcode);
    //持仓商品的总卖单量
    double getHoldTotalSellNum(char * symbolcode);
    //持仓商品的总利息
    double getHoldTotalSumInterest(char * symbolcode);
    //持仓商品的总佣金
    double getHoldTotalCommision(char * symbolcode);
    //持仓商品的总浮动盈亏
    double getHoldTotalFpl(char * symbolcode);
    //获取商品对应的持仓单号
    const char * getHoldTicketsBySymbol(char * symbolcode);
    //获取限价单商品集合
    const char * getLimitSymbolCodes();
    //获取限价单商品名
    const char * getLimitSymbolName(char * symbolcode);
    //限价商品的总买单量
    double getLimitTotalBuyNum(char * symbolcode);
    //限价商品的总卖单量
    double getLimitTotalSellNum(char * symbolcode);
    //限价商品的平均买价
    double getLimitAverageBuyPrice(char * symbolcode);
    //限价商品的平均卖价
    double getLimitAverageSellPrice(char * symbolcode);
    //获取商品对应的限价单号单号
    const char * getLimitTicketsBySymbol(char * symbolcode);
    
    //获取所有持仓单单号
    const char * getHoldOrderAllTickets();
    //获取所有限价单单号
    const char * getLimitOrderAllTickets();
    //获取所有平仓单单号
    const char * getCloseOrderAllTickets();
    //获取当天平仓单的总盈亏
    double getCloseorderAllPL();
    //获取历史单据单号
    const char * getHisOrderAllTickets();
    //获取历史单据单号
    const char * getHisZYJOrderAllTickets();
    
    //获取历史单据总盈亏
    double getHisorderAllPL();
    
    char getOrderType(char* ticket);
    
    int getTradeOrderType(char* ticket);
    
    char getIsClose(char* ticket);
    
    char getOpenClose(char* ticket);
    
    //属性
    //商品
	const char * getSymbol(char * ticket);
	//订单时间(挂单开仓时间)
	const char * getOrderTime(char * ticket);
    //成交时间
    const char * getExecuteTime(char * ticket);
	//子订单类型
    char getSubOrderType(char * ticket);
    //效力类型
    char getExpirationType(char * ticket);
    //效力时间
    const char* getExpiration(char * ticket);
    //订单状态
    char getOrderStatus(char * ticket);
	//订单价格(挂单开仓价格)
	double getOrderPrice(char * ticket);			//orderprice	订单价格	double	10,4
	//订单数量(挂单手数)
	double getOrderQty(char * ticket);
	//止盈价格
	double getSlPrice(char * ticket);		//止盈价格	double	10,4
	//止损价格
	double getStopPx(char * ticket);		//tpprice	止损价格	double	10,4
	//买卖方向
    char getSide(char * ticket);			//direction	买/卖	Char 	1
    
	//开仓 开仓价格
	double getExecuteprice(char * ticket);	//执行价格	double	10,4	订单执行的价格
	//开仓 手数
	double getCumQty(char * ticket);		//executesize	成交数量	double	20,4
	//开仓 开仓时间
	const char * getTransactTime(char * ticket);	//executetime	完成时间	Char 	24	订单完成时间
	//开仓 浮动盈亏（接收行情变化通知后要更新界面）
	double getFPL(char * ticket);
    double getMargin(char * ticket);
    //开仓价格
    double getOpenOrderPrice(char * ticket);
	//利息
	double getSumInterest(char * ticket);	//累计递延费	Double 	20,4
    
    double getBargainCommision(char * ticket);
	//佣金 (开仓手续费 + 平仓手续费)
	double getCommision(char * ticket);
    
    double getCloseCommision(char * ticket);
    
    double getOpenCommision(char * ticket);

    long long getOrderticket(char * ticket);
    
    double getDeliveryMargin(char * ticket);
    
    double getDeliveryPayment(char * ticket);
    
    long long getBargainId(char * ticket);
    
    long long getTicketByBargainId(char * ticket);
    char getIsYesDay(char * ticket);
    
    double getHoldQty(char * ticket);
    
    double getHoldPrice(char * ticket);
	//平仓单号
	const char * getCloseticket(char * ticket);
	//平仓价格
	double getCloseprice(char * ticket);
	//平仓数量
	double getCloseordersize(char * ticket);
	//平仓时间
	const char * getClosetime(char * ticket);
	//盈亏
	double getPL(char * ticket);
    //获取价格位数
    int getTicketPriceDecimal(char * ticket);
    
    //获取资金属性
    double getTotalClosePL();
    
    double getRisk();
    
    int getRiskLevel();
    
    double getTmpValue();
    //总浮动盈亏
    double getTotalFPL();
    
    //可用保证金
    double getBalanceMargin();
    //
    double getFreezeMargin();
    //占用保证金
    double getUsedMargin();
    
    //余额 (可用 +　占用　＋ 冻结)
    double getBalance();
        
    int getSymbolTraceDataCount(const char* utfSymbol);
    int getSymbolTraceData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize);
    int getZSSymbolTraceData(const char* utfSymbol, TrendData* trend, int nMaxSize);
    
    int getSymbolMinuteDataCount(const char* utfSymbol);
    int getSymbolMinuteData(const char* utfSymbol, CSTraceUnit* arr, int nMaxSize);
    int getZSSymbolMinuteData(const char* utfSymbol, TrendData* trend, int nMaxSize);
    
    int getSymbolKLineDataCount(const char* utfSymbol, char nType);
    
    
    int getZSSymbolKLineData(const char* utfSymbol, char nType, HistoryKData* data, int nMaxSize);
    int getSymbolKLineData(const char* utfSymbol, char nType, CSHisKUnitMA* arr, int nMaxSize);
    
    
    int getZYQSymbolKLineData(const char* utfSymbol, int nType, HistoryKData* histroy, int nMaxSize);
    
    void setYesterDayClosePrice(char* symbolCode, double fPrice);
    double getYesterDayClosePrice(char* symbolCode);
//    double getlastPrice(char* symbolCode);
    
    int get_symbol_data_min_begin_end_time(char* symbolCode, int* pBeginTime, int* pTotalTime);
    
    double getCurrPrice(char* symbolCode);
    
    int getLimitHint(char * symbolcode, double price, char sub_order_type, SL_TP_Hint* hint);
    int getMarketHint(char * symbolcode, char sub_order_type, SL_TP_Hint* hint);
    
    //2.0
    //获取帐户权限
    const char* getJurisdiction();
    
    //查竞价帐户商品信息请求
    int zyj_trade_request_symbol();
    //竞价查持仓单单请求
    int zyj_trade_hold_order();
    //查询交收单
    int zyj_trade_settle_order();
    //竞价查委托单单请求
    int zyj_trade_delegate_order();
    //竞价查成交单单请求
    int zyj_trade_deal_order();
    
//    int call_zyj_symbol_handler(int nSeq,const char * json);
//    //竞价查持仓单单请求
//    void call_zyj_hold_order_handler(int nSeq,const char * json);
//    //竞价查委托单单请求
//    void call_zyj_delegate_order_handler(int nSeq,const char * json);
//    //竞价查成交单单请求
//    void call_zyj_deal_order_handler(int nSeq,const char * json);
    
    //是否为做市商
    int getIsZS();
    
    //是否为中远期
    int getIsZYJ();
    
    //是否为仓库
    int getIsStore();
    
    //是否为中立仓
    int getIsZLStore();
    
    long long getCounterPartExecuteTicket(char* ticket);
    double getDeferredCompensation(char* ticket);
    const char* getSellCangId(char* ticket);
    //买方货款
    double getBuyPayOut(char* ticket);
    double getMatchWeight(char* ticket);
    
    const char* getDelegateTime(char* ticket);
    
    const char* getMatchDate(char* ticket);
    long getDayStep(char* ticket);
    
    //获取所有今天持仓历史单据
    const char * getZYJToDayHistoryOrderAllTickets();
    
    //获取所有交收单据
    const char * getSettleOrderAllTickets();
    
    //获取成交单据
    const char * getBarginOrderAllTickets();
    
    //查询提货意向
    int query_apply_delivery_intention();
    
    //提交提货意向
    int trade_apply_delivery_intention(const char* szMathId,double dbDeliveryweight,const char* szDeliveryDateBegin,const char* szDeliveryDateEnd,const char* szDeliverymen,const char* szDeliveryIdCode,const char* szTelphone,const char* szAddress,const char* szZipCode,char chIsInvoice,const char* szInvoicetitle,const char* szInvCompayName,const char* szInvCompayAddress,const char* szInvCompayTelephone,const char* szInvBankaccount,const char* szInvBankDeposit,const char* szTaxregistryNumber);
    
    //提交提货意向
    int trade_submit_delivery_intention(const char* szMathId,double dbDeliveryweight);
    
    //修改提货意向
    int mod_apply_delivery_intention(const char* szMathId,double dbDeliveryweight,const char* szDeliveryDateBegin,const char* szDeliveryDateEnd,const char* szDeliverymen,const char* szDeliveryIdCode,const char* szTelphone,const char* szAddress,const char* szZipCode,char chIsInvoice,const char* szInvoicetitle,const char* szInvCompayName,const char* szInvCompayAddress,const char* szInvCompayTelephone,const char* szInvBankaccount,const char* szInvBankDeposit,const char* szTaxregistryNumber);
    
    //提交“入库申请”请求
    int submit_apply_in_store_order(const char* cid, const char* symbol, double aweight, double unit, const char* idate, const char* mdate, const char* lotnumber, const char* package, const char* factory);
    
    //修改“入库申请”请求
    int mod_apply_in_store_order(const char* importid, const char* cid, const char* symbol, double aweight, double unit, const char* idate, const char* mdate, const char* lotnumber, const char* package, const char* factory);
    
    int trade_apply_in_store_order(const char * applyid);
    int trade_cancle_apply_in_store_order(const char * applyid);
    
    int query_in_store_goods_property();
    int query_sellt_store();
    int query_in_store_order();
    int query_apply_in_store();
    
    int trade_query_cang_to_instore_order();
    //查询仓单请求
    int trade_query_cang();
    
    //查询”出库单”请求
    int trade_query_out_store_order();
    
    const char* getDeliveryStoreIds();
    
    const char* getStoreNameById(long long id);
    
    const char* getStoreFullNameById(long long id);
    
    const char* getStoreCodeById(long long id);
    
    const char* getStoreAddressById(long long id);
    
    const char* getStoreAuthenticatedPersonById(long long id);
    const char* getStoreKeeperById(long long id);
    
    const char* getStoreZipCodeById(long long id);
    
    const char* getStoreFaxById(long long id);
    
    const char* getStoreEmailById(long long id);
    
    long long getApplyByStoreId(long long id);
    
    
    const char * getApplyInStoreAllApplyId();
    
    const char * getInStoreAllApplyId();
    
    const char * getOutStoreAllIds();
    
    const char * getCangOrderAllIds();
    
    const char * getCangToInStoreOrderAllIds();
    
    
    const char * getApplyDate(const char *ticket);//申请时间
    double getBalanceWeight(const char *ticket);
    double getFreezeImportWeight(const char *ticket);
    double getFreezeCloseWeight(const char *ticket);
    
    double getFreezeWaitWeight(const char *ticket);
    double getCloseWeight(const char *ticket);
    
    char getCangType(const char *ticket);//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}
    
    const char * getCangDate(const char *ticket);
    
    char getCangStatus(const char *ticket);
    char getExportStatus(const char *ticket);
    const char * getExportdate(const char *iApplyid); // '出库时间
    const char*  getDeliverynumber(const char *iApplyid);//提货码-
    
    const char*  getDeliveryMen(const char *iApplyid);//提货人名称

    const char*  getIdCode(const char *iApplyid);//提货人身份证
    
    const char*  getTelphone(const char *iApplyid);//提货人电话号码
    
    int getImportid(const char *iApplyid); // '入库单ID',
    
    long long getApplyid(const char *id);
    
    int getWarehouseid(const char *iApplyid);  // '交收仓库',
    
    int getAccountgroupid(const char *iApplyid);// '机构',
    
//    int getAccountid(const char *iApplyid);// '账户',
    
    int getSymbolgroupid(const char *iApplyid);// '品种名称',

    const char* getInStoreSymbolcode(const char *iApplyid);// '商品代码',
    
    const char* getImportdate(const char *iApplyid); // '入库时间',

    char getImportstatus(const char *iApplyid);// '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',

    char getApplystatus(const char *iApplyid);
    
    const char* getRejectreason(const char *iApplyid);// '拒绝原因',

    const char* getCheckresult(const char *iApplyid);// '检验结果',

    double getOverPlusweight(const char *iApplyid);//剩余重量
    
    double getApplyweight(const char *iApplyid);// '申请重量',

    double getImportweight(const char *iApplyid);// '入库重量'
    
    double getFreezeexportweight(const char *iApplyid);
    
    double getExportweight(const char *iApplyid);
    
    double getFreezecangweight(const char *iApplyid);
    
    double getCangweight(const char *iApplyid);

    double getUnitweight(const char *iApplyid); // '每件重量',
        
    int getQuantity(const char *iApplyid); // '件数',

    const char* getGoodsnumber(const char *iApplyid);// '货号',

    const char* getLot(const char *iApplyid);// '批号',

    const char* getMade(const char *iApplyid);// '生产年月',

    const char* getPackage(const char *iApplyid);// '包装方式',

    const char* getFactory(const char *iApplyid);// '生产厂商',
    
    const char* getWarehousetransactor(const char *iApplyid);// '仓库经办人',

    const char* getWarehousemanager(const char *iApplyid);// '仓库负责人',

    const char* getClienttransactor(const char *iApplyid);// '客户经办人'
    
    int getAttributeNum(const char *iApplyid);
    
    int getAttributeIdByIndex(const char *iApplyid, int index);
    
    const char* getAttributeValueByIndex(const char *iApplyid, int index);
    
    const char*  getApplyDeliveryIds(); // '提货id',
    const char*  getApplyDeliverymen(const char * ticket); // '提货人',
    const char*  getApplyIdcode(const char * ticket); // '身份证',
    const char*  getApplyTelephone(const char * ticket); // '联系电话',
    const char*  getApplyAddress(const char * ticket); // '联系地址',
    const char*  getApplyZipcode(const char * ticket); // '邮政编码',
    
    const char*  getApplyInvoicetitle(const char * ticket); // '发票抬头',
    const char*  getApplyCompanyname(const char * ticket); // '公司名称',
    const char*  getApplyCompanyaddress(const char * ticket); // '公司地址',
    const char*  getApplyCompanytelephone(const char * ticket); // '公司电话',
    const char*  getApplyBankaccount(const char * ticket); // '银行账号',
    const char*  getApplyBankdeposit(const char * ticket); // '开户行',
    const char*  getApplyTaxregistrynumber(const char * ticket); // '税务登记号',
    
    const char*  getApplyDeliverydatebegin(const char * ticket); // '提货日期D5',
    const char*  getApplyDeliverydateend(const char * ticket); // '提货日期D6',
    const char*  getApplyDeliverynumber(const char * ticket); // '提货号',
    const char*  getApplySymbolcode(const char * ticket); // '商品代码',
    const char*  getApplyWarehousefeedate(const char * ticket); // '仓储费截止日期',
    const char*  getApplyRejectreason(const char * ticket);
    const char*  getApplySettlementdate(const char * ticket); // '交割日期',
    
    long long getApplyDeliveryid(const char * ticket); // '提货单ID',
    int getApplyMatchId(const char * ticket);
    
    int getApplyWarehouseid(const char * ticket); // '仓库ID',
    int getApplyCangId(const char * ticket);	// 仓单ID
    int getApplyExecuteticket(const char * ticket); // '成交单号',
    int getApplyCounterpartyexecuteticket(const char * ticket); // '对手单号',
    double getApplyDeliveryweight(const char * ticket); // '提货重量',
    
    double getApplySettlementprirce(const char * ticket); // '交割价格',
    char getApplyDeliverystatus(const char * ticket); // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',
    
    char getApplyIsinvoice(const char * ticket); // '是否需要发票',
    char getApplyIscancel(const char * ticket); // '是否撤销',
    
    
    
    const char * getFundFlowTickets() ;
    const char * getFundFlowAccountAlias(long long ids);
    const char * getFundFlowExecuteTime(long long ids);
    long long getFundFlowAccountId(long long ids);
    char getFundFlowOperatortype(long long ids);
    double getFundFlowChangeFunds(long long ids);
    double getFundFlowBalance(long long ids);
    long long getFundFlowAssociateticket(long long ids);

    
    const char * getInOutMoneryids();
    const char * getInOutMoneryApplyTime(long long ids);
    const char * getInOutMoneryAuditTime(long long ids);
    const char * getInOutMoneryRemark(long long ids);
    char getInOutMoneryExecutestatus(long long ids);
    long long getInOutMoneryId(long long ids);
    char getInOutMoneryOperatortype(long long ids);
    double getInOutMoneryAmount(long long ids);
    
    int getSignStatus();
    int getSuportOutMonery();
    int getSuportInMonery();
    int getSuportSign();
    int getSuportUnSign();
    //操作仓单转入库单
    int trade_op_request_cang_to_instore_order(const char * ticket, double weight);
    
    //提交 “仓单转入库单”
    int trade_cang_to_instore_order(const char * ticket, double weight);
    //撤消 “仓单转入库单”
    int trade_cancel_cang_to_instore_order(const char * ticket);
    //提交 “仓单转入库单”
    int trade_submit_cang_to_instore_order(const char * ticket);
    
    //入库单转仓单
    int trade_instore_to_cang_order(const char *  ticket, double weight);
    
    //入库单转出库单
    int trade_instore_to_exportstore_order(const char *ticket, const char * deliveryName, double weight, const char *dCode, const char *telPhone);
    
    
    //提交买 "申报交收" 请求
    int trade_buy_apply_delivery_settle(const char * symbolcode, double qty, char iscancel);
    
    //提交卖 "申报交收" 请求
    int trade_sell_apply_delivery_settle(const char * symbolcode, double qty, const char* cangid, char iscancel);
    
    
    int check_tp(char * symbolcode,  char direction, double tp, double price);
    
    int check_sl(char * symbolcode,  char direction, double sl, double price);
    
    int trade_zyq_quote_report(STUReportDataIndex_ZYQ* indx, STUSimpleSymbol_ZYQ* symbols);
    
    //分笔行情获取
    int send_quote_STK_trace_data_req(const char* szCode, int nszCodeSize);
    //分时行情获取
    int send_quote_STK_min_data_req(const char* szCode, int nszCodeSize);
    //OTC历史
    int send_quote_his_data_req(const char* szCode, int nPeriodicType, int nNum, int nReqCount);
    
    void request_all_zyq__quote_pankou();
    
    int request_zyq_kline(char* symbolCode, int ktype, int hasNum, int reqCount);
    
    int request_zyq_pankou_detail(char* symbolCode);
    
    int getNowDataVolPirce(char * symbolcode, STUNowDataVolPirce* stuNowDataVolPirce);
    
    int getSymbolTraceData_ZYQ(const char* symbol, STUTraceUnit_ZYQ* stuTraceUnit_ZYQ, int iSize);
    int getSymbolMinData_ZYQ(const char* symbol, STUSTKMinUnit_ZYQ* stuSTKMinUnit_ZYQ, int iSize);
    
    int get_zyq_symbol_trade_begin_end_time(char* symbolCode, int* pBeginTime, int* m_nEndTime);
    
    
    int trade_in_monery(const char * password, double monery, const char * phonePwd, const char* remark, const char* info);
    
    int trade_out_monery(const char * password, double monery, const char * phonePwd, const char* remark, const char* info);
    
    
    int trade_sign(const char* tel,const char* bankName, const char* bankAccount,const char* idCode, const char * password, const char * phonePwd, const char* remark);
    
    int trade_unsign(const char* tel,const char* bankName, const char* bankAccount,const char* idCode, const char * password, const char * phonePwd, const char* remark);
    
    int getBankPasswordType();
    
    /************************************
                    文交所
     ************************************/
    
    //请求商品信息
    int trade_request_symbol();
    
    int trade_request_today_entrust();
    // 查今日成交请求
    int trade_request_today_deal();
    // 撤单
    int trade_request_cancel_order(char * orderID);
    // 持仓份额
    int trade_request_sharehold();
    // 资金持仓
    int trade_request_moneyhold();
    
    int quote_kline_req(int klineType,char *productID);
    
    int quote_stktrace_req(char *code);
    
    int quote_reportData_req(char *codes);
    // 历史委托
    int trade_request_historyorder(char *startDate,char *endDate);
    // 历史成交
    int trade_request_historyturnover(char *startDate,char *endDate);
    // 历史成交
    int trade_pretrade_req(char *securityID,char *orderType);
    // 出入金请求
    int trade_request_inout_money(int transferType,double money,const char * moneyPwd,const char *bankPwd);
    
    const char * getBankName();
    
    char* get_initmarket_productName(int symbolCode);
    int get_initmarket_preclose(int symbolCode);
//////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
    
#endif
