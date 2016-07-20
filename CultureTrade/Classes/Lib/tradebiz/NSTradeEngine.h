//
//  NSTradeEngine.h
//  jzjyClient
//
//  Created by zengxw on 13-11-21.
//  Copyright (c) 2013年 MyCompany. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "tradedef.h"

//extern const unsigned int MINUTE1_HISK; //表明为一分钟线
//extern const unsigned int MINUTES5_HISK;//5分钟K线
//extern const unsigned int MINUTES10_HISK;//10分钟K线
//extern const unsigned int MINUTES15_HISK;//15分钟K线
//extern const unsigned int MINUTES30_HISK;//30分钟K线
//extern const unsigned int MINUTES60_HISK;//60分钟K线
//extern const unsigned int DAY_HISK;//日K线
//extern const unsigned int WEEK_HISK;//周K线
//extern const unsigned int MONTH_HISK;//月K线

extern NSString * const DELCARE_NOTIFY;
extern NSString * const REFRESH_NOTIFY;


#define           SUCCESS                         (0)
#define           DIRECTION_RISE                  (1)
#define           DIRECTION_FALL                  (-1)
#define           DIRECTION_DEFAULT               (0)
////止盈止损提示
//@interface NS_SL_TP_Hint : NSObject
//@property (nonatomic) NSString* price;
//@property (nonatomic) NSString* SlPrice;
//@property (nonatomic) NSString* TpPrice;
//@end



@protocol NSTradeEngineDelegate <NSObject>
@optional
-(void)on_net_status_rsp:(int)type nFlag:(int)nFlag;

//登录进度通知
-(void)trade_ui_login_progress_rsp:(int)progress max:(int)max_progress;

//踢用户下线
-(void)trade_ui_kick_notify;

//历史订单刷新通知
-(void)trade_ui_hisorder_rsp:(int)nSeq nRet:(int)nRet symbolcodes:(NSString *)ordercodes;

//持仓单刷新通知
-(void)trade_ui_holdorder_rsp:(int)nRet opencodes:(NSString *)opencodes limitcodes:(NSString*)limitcodes  closecodes:(NSString*) closecodes;

//资金刷新通知
-(void) trade_ui_moneyhold_rsp:(int) nRet;

//下单应答
-(void) trade_ui_openorder_rsp:(int)nSeq nRet:(int)nRet  orderticket:(NSString *)orderticket;

//汇总持仓视图 商品信息变化
-(void) trade_ui_openorder_stat_rsp:(int)nRet  symbolcode:(NSString*) symbolcode;

-(void)quote_ui_min_data_recv_notify:(int)nSeq symbol:(NSString*)symbolCode;
-(void)quote_ui_his_data_recv_notify:(int)nSeq cType:(char)cType symbol:(NSString*)symbolCode;
-(void)quote_ui_record_data_recv_notify:(int)nSeq symbol:(NSString*)symbolCode;

//刷新完成通知
-(void)trade_ui_refresh_complete:(int) nRet;

//实时行情
-(void)on_quote_real_notify:(NSString *)symbolcode price:(double)price second:(long long)second;
//
-(void)on_history_change_notify:(NSString *)symbolcode price:(int)price second:(long long)second;
//请求公告内容
-(void)on_delcare_content_rsp:(int) nSeq content:(NSString*) content;

-(void) on_delcare_notify:(int) annId;

-(void) on_modify_passwd_rsp:(int)nRet;

-(void) on_trade_apply_delivery_intention_rsp:(int)nSeq nRet:(int)nRet;

-(void) on_cang_to_in_store_order_response:(int)nSeq nRet:(int)nRet;

-(void) on_cang_order_response:(int)nSeq nRet:(int)nRet;


-(void) on_bargain_notify:(int) nRet;//交收单变更
-(void) on_sellte_notify:(int) nRet;//成交单变更
-(void) on_outstore_order_notify:(int) nRet;
-(void) on_apply_instore_notify:(int) nRet;
-(void) on_cang_order_notify:(int) nRet;
-(void) on_apply_delivery_notify:(int) nRet;
-(void) on_instore_order_notify:(int) nRet;
-(void) on_cangorder_to_instore_notify:(int) nRet;

-(void) on_op_in_store_order_response:(int)nSeq nRet:(int) nRet;

-(void) on_apply_in_store_rsp:(int)nSeq nRet:(int) nRet;

-(void) on_apply_delivery_sellte_rsp:(int)nSeq nRet:(int) nRet;

-(void) on_funds_flowing_rsp:(int) nSeq nRet:(int) nRet;

-(void) on_quote_detail_report_notify:(int) nSeq nRet:(int) nRet symbol:(NSString*)symbol;

-(void) on_quote_real_detail_notify:(NSString*)symbol;

-(void) on_inout_money_rsp:(int)nSeq nRet:(int)nRet data:(NSString *)data;

-(void) on_sign_change_notify:(int)status;


/*******************************************
                文交所
 ******************************************/
//登录应答
-(void)trade_login_rsp_to_ui:(int)nRet nType:(int)nType;

//商品信息变化 symbolcodes
-(void)trade_ui_symbol_change_notice:(int)nRet symbolcodes:(NSString *)symbolcodes;

// 今日委托
- (void)trade_ui_today_entrust_rsp:(NSString *)data;

// 今日成交
- (void)trade_ui_today_deal_rsp:(NSString *)data;

// 持仓查询
- (void)trade_ui_sharehold_rsp:(int)nRet sharehold:(NSString *)sharehold;

- (void)trade_ui_historyorder_rsp:(int)nSeq orders:(NSString *)orders;

- (void)trade_ui_historyturnover_rsp:(int)nSeq deals:(NSString *)deals;

- (void)trade_ui_pretrade_rsp:(int)nSeq pretrade:(NSString *)pretrades;

- (void)trade_ui_canceltrade_rsp:(int)nSeq canceltrades:(NSString *)canceltrades;

// 行情登陆
- (void)quote_ui_login_rsp:(int)nRet;

// K线回调
- (void)quote_ui_hisKDataFirst_rsp:(NSString *)data count:(int)count;

- (void)quote_ui_hisKDataCurDate_rsp:(NSString *)data count:(int)count;

- (void)quote_ui_stkDealData_rsp:(NSString *)data;

- (void)quote_ui_initMark_rsp:(NSString *)data len:(int)pLen;

- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len;

- (void)trade_ui_transferquery_rsp:(int)nSeq results:(NSString *)results;
// 历史委托


// 历史成交

@end

#import <Foundation/Foundation.h>

@interface NSTradeEngine : NSObject
{
    NSString *PATTER;
}
+(NSTradeEngine*)setup;
+(NSTradeEngine*) sharedInstance;

@property (weak, nonatomic) id<NSTradeEngineDelegate> delegate;



//请求登录 带地址信息
-(void)userloginwithnet:(NSString*)account password:(NSString*)password tradeip:(NSString*)tradeip tradeport:(int)tradeport quoteip:(NSString *)quoteip  quoteport:(int)quoteport;

- (void)loginquotewithnet:(NSString *)quoteip  quoteport:(int)quoteport;
//-(NSString*)getNSStringLimitHint:(NSString *)symbolcode ordertype:(char)ordertype;

-(int) trade_request_hisorder:(NSString *)startdate enddate:( NSString *)enddate;

//查历史单据 %04d-%02d-%02d
-(int) trade_request_zyj_hisorder:(NSString *)startdate enddate:( NSString *)enddate;



//-(int)trade_open_limit_order:(NSString *)symbolcode  shoushu:(double)shoushu  sl:(double)sl  tp:(double)tp  ordertype:(char)ordertype price:( double) price type:(char)type date:(NSString*)date;

// 查询单据 
-(void) trade_request_order;

-(int)sendTraceDataReq:(NSString*)codeName;
-(int)sendMinDataReq:(NSString*)codeName;
-(int)sendKLineDataReq:(NSString*)codeName kLineType:(int)type;
-(int)sendKLineDataReq:(NSString*)codeName kLineType:(int)nType kLineCount:(int)nReqCount;
-(int)sendDefaultKLineReq:(NSString*)codeName kLineType:(int)nType;
//-(void)sendSymbolDataInfo;

//-(void)sendInitMarket;
//


//修改持仓单
-(int) trade_modify_open_order:(NSString *)orderticket sl:(double)sl  tp:( double) tp;
//修改限价单
-(int)trade_modify_limit_order:(NSString *)orderticket price:( double) price  sl:( double) sl  tp:( double) tp;
//删除限价单
-(int)trade_cancel_limit_order:(NSString *)orderticket;
//平仓 2014.07.30 增加反手与偏离点差字段
-(int) trade_close_order:(NSString *)orderticket shoushu:( double) shoushu rqty:(double) rqty diff:(int)diff;

//撤消中远期单
-(int) trade_cancel_zyj_order:(NSString *)orderticket;
//开仓
-(int) trade_open_limit_zyj_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price;
//平今仓
-(int) trade_close_limit_zyj_day_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price;
//平昨仓
-(int) trade_close_limit_zyj_yesterday_order:(char)direction symbolcode:(NSString*)symbolcode qty:(double)shoushu price:(double)price;


-(BOOL)symbolDataForCode:(NSString*)symbolCode beginTime:(int*)pBeginTime totalTime:(int*)pTotalTime;

-(void) call_refresh_account_info;


-(NSString *)getUserAccount;

//用户状态
-(int) getUserStatus;

//用户名
-(NSString *)getUserName;

-(NSString *)getLoginUser;

-(NSString *)getSession;

//获取商品属性
//获取所有商品编码
-(NSString *)getAllSymbolCode;

-(NSString *)getNativeVersion;

//商品名
-(NSString *)getSymbolname:(NSString *)symbolcode;


-(char) getOrderType:(NSString *)ticket;
-(int) getSymbolType:(NSString *)symbolcode;
-(int) getTradeOrderType:(NSString *)ticket;

//下单类型
-(char) getOpenClose:(NSString *)ticket;

//买价
-(NSString*) getAskPrice:(NSString *)symbolcode;
//卖价
-(NSString*) getBidPrice:(NSString *)symbolcode;
//行情时间
-(long long)getLastTick:(NSString *)symbolcode;
//最高
-(NSString*) getHighPrice:(NSString *)symbolcode;
//最低
-(NSString*) getLowPrice:(NSString *)symbolcode;

//昨收价
-(NSString*) getYesterDayClosePrice:(NSString *)symbolcode;
//昨结价
-(NSString*) getSettlPrice:(NSString *)symbolcode;
//开盘价
-(NSString*) getOpenPrice:(NSString *)symbolcode;
//最新价
-(NSString*) getLastPrice:(NSString *)symbolcode;

//小数点
-(int) getDecimal:(NSString*) symbolcode;

//价格涨跌方向 -1跌 0平 1涨
-(int)getPriceDirection:(NSString *)symbolcode;

-(char)getExpirationType:(NSString*) ticket;
-(NSString*)getExpiration:(NSString*) ticket;

//获取订单属性
//汇总
//获取持仓单商品集合
-(NSString *)getHoldSymbolCodes;
//持仓商品的总买单量
-(NSString*) getHoldTotalBuyNum:(NSString *)symbolcode;
//持仓商品的总卖单量
-(NSString*) getHoldTotalSellNum:(NSString *)symbolcode;
//持仓商品的总利息
-(NSString*) getHoldTotalSumInterest:(NSString *)symbolcode;
//持仓商品的总佣金
-(NSString*)getHoldTotalCommision:(NSString *)symbolcode;
//持仓商品的总浮动盈亏
-(NSString*) getHoldTotalFpl:(NSString *)symbolcode;
//获取限价单商品集合
-(NSString *)getLimitSymbolCodes;
//限价商品的总买单量
-(NSString*) getLimitTotalBuyNum:(NSString *)symbolcode;
//限价商品的总卖单量
-(NSString*) getLimitTotalSellNum:(NSString *)symbolcode;
//限价商品的平均买价
-(NSString*)getLimitAverageBuyPrice:(NSString *)symbolcode;
//限价商品的平均卖价
-(NSString*) getLimitAverageSellPrice:(NSString *)symbolcode;

-(NSString*)getLimitTicketsBySymbol:(NSString*)symbolcode;
-(NSString*)getHoldTicketsBySymbol:(NSString*)symbolcode;

//获取所有持仓单单号
-(NSString *)getHoldOrderAllTickets;
//获取所有限价单单号
-(NSString *)getLimitOrderAllTickets;
//获取历史单据单号
-(NSString *) getHisOrderAllTickets;

//获取历史单据单号
-(NSString *) getHisZYJOrderAllTickets;

-(NSString *) getDeliveryMargin:(NSString*)ticket;

-(NSString *) getDeliveryPayment:(NSString*)ticket;

-(char) getIsClose: (NSString*)ticket;

-(NSString *) getOrderticket:(NSString*)ticket;
//属性

//商品状态
-(char)getOrderStatus:(NSString*)ticket;

//商品
-(NSString *)getSymbol:(NSString *)ticket;
//订单时间(挂单开仓时间)
-(NSString*)getOrderTime:(NSString *)ticket;
//子订单类型
-(char)getSubOrderType:(NSString *)ticket;
//订单价格(挂单开仓价格)
-(NSString*)getOrderPrice:(NSString *)ticket;
//订单数量(挂单手数)
-(NSString*) getOrderQty:(NSString *)ticket;
//止盈价格
-(NSString*)getSlPrice:(NSString *)ticket;
//止损价格
-(NSString*) getStopPx:(NSString *)ticket;
//买卖方向
-(char) getSide:(NSString *)ticket;

//开仓 开仓价格
-(NSString*) getExecuteprice:(NSString *)ticket;
//开仓 手数
-(NSString*) getCumQty:(NSString *)ticket;
//开仓 开仓时间
-(NSString*)getTransactTime:(NSString *)ticket;
//开仓 浮动盈亏（接收行情变化通知后要更新界面）
-(NSString*) getFPL:(NSString *)ticket;
-(int) getFPLDirection:(NSString *)ticket;
-(int)getTicketPriceDecimal:(NSString*)ticket;

//利息
-(NSString*)getSumInterest:(NSString *)ticket;

//开仓价格
-(NSString*) getOpenOrderPrice:(NSString *)ticket;

//佣金 (开仓手续费 + 平仓手续费)
-(NSString*) getCommision:(NSString *)ticket;

-(NSString*) getCloseCommision:(NSString *)ticket;

-(NSString*) getOpenCommision:(NSString *)ticket;
//平仓单号
-(NSString *)getCloseticket:(NSString *)ticket;
//平仓价格
-(NSString*) getCloseprice:(NSString *)ticket;
//平仓数量
-(NSString*) getCloseordersize:(NSString *)ticket;
//平仓时间
-(NSString*)getClosetime:(NSString *)ticket;
//盈亏
-(NSString*) getPL:(NSString *)ticket;

//获取资金属性

-(NSString*)getFreezeMargin;

//可用保证金
-(NSString*)getBalanceMargin;
//
-(NSString*)getTotalFPL;



//占用保证金
-(NSString*)getUsedMargin;

//余额 (可用 +　占用　＋ 冻结)
-(NSString*)getBalance;

-(NSString*) getTmpValue;

-(NSString*)getTotalClosePL;

-(NSString*) getRisk;

-(int) getRiskLevel;

-(NSString*)getHisorderAllPL;
//获取所有平仓单单号
-(NSString*)getCloseOrderAllTickets;
//获取当天平仓单的总盈亏
-(NSString*)getCloseorderAllPL;
//根据价格（INT）和商品代码 得到带有小数的商品价格
-(NSString*)getNormalPrice:(int)nPrice symbolNameCode:(NSString*)symbolNameCode;

-(NSString * ) getLoginedAccountName;
-(BOOL) isLogin;

//
-(double) getCurrPrice:(NSString *)symbolcode;

-(int) getMarketHint:(NSString*) symbolcode orderType:(char)sub_order_type hint:(SL_TP_Hint*)hint;
-(int) getLimitHint:(NSString*) symbolcode price:(double)price orderType:(char)sub_order_type hint:(SL_TP_Hint*)hint;


-(NSString*) getTicketByBargainId:(NSString*)ticket;
-(NSString*) getBargainId:(NSString*)ticket;

-(char) getIsYesDay:(NSString*)ticket;

-(NSString*) getHoldQty:(NSString*)ticket;

-(NSString*) getHoldPrice:(NSString*)ticket;
-(NSString*) getExecuteTime:(NSString*)ticket;

/*公告接口*/
-(int)trade_request_declare_title;
-(int)trade_request_declare_content:(int)aid;
-(int) getDeclareAnnouceSize;
-(NSString*) getDeclareAnnouceID;//公告ID
-(long long) getDeclareTime:(int) annouceID;
-(NSString*) getDelcare:(int) annouceID;//公告发布者
-(NSString*) getDelcareTitle:(int) annouceID;//公告标题
-(NSString*) getDelcareContent:(int) annouceID;//公告内容
-(char) getDelcareReadStatus:(int) annouceID;//阅读状态 1已阅读 2 未阅读


/* END*/

/*修改帐户信息*/
-(int) trade_request_modify_passwd:(NSString *)type oldpass:(NSString*)oldpass newpass:(NSString*)newpass;
/*END*/

-(NSDate*) getTimeIntervalSince1970:(long long) ticks;

-(int) getSymbolTraceDataCount:(NSString*) utfSymbol;
-(int) getSymbolTraceData:(NSString*) utfSymbol arr:(TraceUnit*) arr size:(int)size;
-(int) getZSSymbolTraceData:(NSString*) symbol data:(TrendData*)data size:(int)size;

-(int) getSymbolMinuteDataCount:(NSString*) utfSymbol;
-(int) getSymbolMinuteData:(NSString*) utfSymbol arr:(TraceUnit*) arr size:(int)size;

-(int) getZSSymbolMinuteData:(NSString*) symbol data:(TrendData*)data size:(int)size;

-(int) getSymbolKLineDataCount:(NSString*) utfSymbol nType:(char)nType;

-(int) getZSSymbolKLineData:(NSString*) utfSymbol nType:(char)nType data:(HistoryKData*)data size:(int)size;


-(BOOL) isZS;
-(BOOL) isZYJ;
-(BOOL) isStore;
-(BOOL) isZLStore;

//获取所有今天持仓历史单据
-(NSString *) getZYJToDayHistoryOrderAllTickets;

//交收单
-(NSString*) getSettleOrderAllTickets;
//获取成交单据
-(NSString*) getBarginOrderAllTickets;

-(NSString*) getCounterPartExecuteTicket:(NSString*)ticket;

-(NSString*) getDeferredCompensation:(NSString*)ticket;

-(NSString*) getSellCangId:(NSString*)ticket;

//买方货款
-(NSString*) getBuyPayOut:(NSString*)ticket;
-(NSString*) getDelegateTime:(NSString*)ticket;
-(NSString*) getMatchWeight:(NSString*)ticket;

-(NSString*)  getMatchDate:(NSString*)ticket;

-(NSString*) getDayStep:(NSString*)ticket;

-(NSString*) getMargin:(NSString*)ticket;

-(NSString*) getBargainCommision:(NSString*)ticket;

-(int) getSymbolShowOrder:(NSString*)symbolcode;

//仓库
-(int) query_apply_delivery_intention;
//提交提货意向
-(int) apply_delivery_intention:(NSString*)szMathId weight:(double)weight dateBegin:(NSString*)dateBegin dateEnd:(NSString*)dateEnd deliverymen:(NSString*)deliverymen deliveryIdCode:(NSString*)deliveryIdCode telphone:(NSString*)telphone address:(NSString*)address zipCode:(NSString*)zipCode invoice:(char)invoice invoicetitle:(NSString*)invoicetitle invCompayName:(NSString*)invCompayName invCompayAddress:(NSString*)invCompayAddress invCompayTelephone:(NSString*)invCompayTelephone invBankaccount:(NSString*)invBankaccount invBankDeposit:(NSString*)invBankDeposit taxregistryNumber:(NSString*)taxregistryNumber;

//重新提交提货意向
-(int) submit_delivery_intention:(NSString*)szMathId weight:(double)weight;

//修改提货意向
-(int) mod_delivery_intention:(NSString*)szMathId weight:(double)weight dateBegin:(NSString*)dateBegin dateEnd:(NSString*)dateEnd deliverymen:(NSString*)deliverymen deliveryIdCode:(NSString*)deliveryIdCode telphone:(NSString*)telphone address:(NSString*)address zipCode:(NSString*)zipCode invoice:(char)invoice invoicetitle:(NSString*)invoicetitle invCompayName:(NSString*)invCompayName invCompayAddress:(NSString*)invCompayAddress invCompayTelephone:(NSString*)invCompayTelephone invBankaccount:(NSString*)invBankaccount invBankDeposit:(NSString*)invBankDeposit taxregistryNumber:(NSString*)taxregistryNumber;

//提交“入库申请”请求
-(int) submit_apply_in_store_order:(NSString*)cid symbol:(NSString*)symbol aweight:(double)aweight unit:(double)unit idate:(NSString*)idate mdate:(NSString*)mdate lotnumber:(NSString*)lotnumber package:(NSString*)package factory:(NSString*)factory;

//修改“入库申请”请求
-(int) mod_apply_in_store_order:(NSString*)importid cid:(NSString*) cid symbol:(NSString*)symbol aweight:(double)aweight unit:(double)unit idate:(NSString*)idate mdate:(NSString*)mdate lotnumber:(NSString*)lotnumber package:(NSString*)package factory:(NSString*)factory;

//操作“入库申请”
-(int) trade_apply_in_store_order:(NSString*)importid;

//取消
-(int) trade_cancle_apply_in_store_order:(NSString*)importid;

-(NSString*) getDeliveryStoreIds;

-(NSString*) getStoreNameById:(NSString*)ids;

-(NSString*) getStoreFullNameById:(NSString*)ids;

-(NSString*) getStoreCodeById:(NSString*)ids;

-(NSString*) getStoreAddressById:(NSString*)ids;

-(NSString*) getStoreAuthenticatedPersonById:(NSString*)ids;
-(NSString*) getStoreKeeperById:(NSString*)ids;

-(NSString*) getStoreZipCodeById:(NSString*)ids;

-(NSString*) getStoreFaxById:(NSString*)ids;

-(NSString*) getStoreEmailById:(NSString*)ids;


-(NSString*) getOverPlusweight:(NSString*)iApplyid;//剩余重量

-(NSString*) getApplyByStoreId:(NSString*)iStoreid;

-(NSString*) getApplyInStoreAllApplyId;

-(NSString*) getInStoreAllApplyId;

-(NSString*) getOutStoreAllIds;

-(NSString*) getCangOrderAllIds;

-(NSString*) getCangToInStoreOrderAllIds;

-(NSString*) getBalanceWeight:(NSString*)ticket;

-(NSString*) getFreezeImportWeight:(NSString*)ticket;

-(NSString*) getFreezeCloseWeight:(NSString*)ticket;

-(NSString*) getFreezeWaitWeight:(NSString*)ticket;

-(NSString*) getCloseWeight:(NSString*)ticket;
    
-(char) getCangType:(NSString*)ticket;//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}

-(NSString*) getCangDate:(NSString*)ticket;

-(NSString*) getApplyDate:(NSString*)ticket;//申请时间
-(char) getCangStatus:(NSString*)ticket;

-(char) getExportStatus:(NSString*)ticket;// '出库状态'

-(NSString*) getExportdate:(NSString*)iApplyid; // '出库时间'

-(NSString*) getDeliverynumber:(NSString*)iApplyid;//提货码-

-(NSString*) getDeliveryMen:(NSString*)iApplyid;//提货人名称

-(NSString*) getIdCode:(NSString*)iApplyid;//提货人身份证

-(NSString*) getTelphone:(NSString*)iApplyid;//提货人电话号码

-(NSString*) getImportid:(NSString*)iApplyid; // '入库单ID',

-(NSString*) getWarehouseid:(NSString*)iApplyid;  // '交收仓库',

-(NSString*) getAccountgroupid:(NSString*)iApplyid;// '机构',

-(NSString*) getSymbolgroupid:(NSString*)iApplyid;// '品种名称',

-(NSString*) getInStoreSymbolcode:(NSString*)iApplyid;// '商品代码',

-(NSString*) getImportdate:(NSString*)iApplyid; // '入库时间',

-(char) getImportstatus:(NSString*)iApplyid;// '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',
-(char) getApplystatus:(NSString*)iApplyid;
-(NSString*) getRejectreason:(NSString*)iApplyid;// '拒绝原因',

-(NSString*) getCheckresult:(NSString*)iApplyid;// '检验结果',

-(NSString*) getApplyweight:(NSString*)iApplyid;// '申请重量',

-(NSString*) getImportweight:(NSString*)iApplyid;// '入库重量'

-(NSString*) getFreezeexportweight:(NSString*)iApplyid;

-(NSString*) getExportweight:(NSString*)iApplyid;

-(NSString*) getFreezecangweight:(NSString*)iApplyid;

-(NSString*) getCangweight:(NSString*)iApplyid;

-(NSString*) getUnitweight:(NSString*)iApplyid; // '每件重量',

-(NSString*) getQuantity:(NSString*)iApplyid; // '件数',

-(NSString*) getGoodsnumber:(NSString*)iApplyid;// '货号',

-(NSString*) getLot:(NSString*)iApplyid;// '批号',

-(NSString*) getMade:(NSString*)iApplyid;// '生产年月',

-(NSString*) getPackage:(NSString*)iApplyid;// '包装方式',

-(NSString*) getFactory:(NSString*)iApplyid;// '生产厂商',

-(NSString*) getWarehousetransactor:(NSString*)iApplyid;// '仓库经办人',

-(NSString*) getWarehousemanager:(NSString*)iApplyid;// '仓库负责人',

-(NSString*) getClienttransactor:(NSString*)iApplyid;// '客户经办人'

-(int) getAttributeNum:(NSString*)iApplyid;

-(NSString*) getAttributeIdByIndex:(NSString*)iApplyid idx:(int)index;

-(NSString*) getAttributeValueByIndex:(NSString*)iApplyid idx:(int)index;


-(NSString*) getApplyId:(NSString*)ids;

-(NSString*)  getApplyDeliveryIds;

-(NSString*)  getApplyDeliverymen:(NSString*)ticket; // '提货人',

-(NSString*) getApplyIdcode:(NSString*)ticket; // '身份证',

-(NSString*)  getApplyTelephone:(NSString*)ticket; // '联系电话',

-(NSString*)  getApplyAddress:(NSString*)ticket; // '联系地址',

-(NSString*)  getApplyZipcode:(NSString*)ticket; // '邮政编码',

-(NSString*) getApplyInvoicetitle:(NSString*)ticket; // '发票抬头',

-(NSString*)  getApplyCompanyname:(NSString*)ticket; // '公司名称',

-(NSString*)  getApplyCompanyaddress:(NSString*)ticket; // '公司地址',

-(NSString*)  getApplyCompanytelephone:(NSString*)ticket;// '公司电话',

-(NSString*)  getApplyBankaccount:(NSString*)ticket; // '银行账号',

-(NSString*)  getApplyBankdeposit:(NSString*)ticket; // '开户行',

-(NSString*)  getApplyTaxregistrynumber:(NSString*)ticket; // '税务登记号',

-(NSString*)  getApplyDeliverydatebegin:(NSString*)ticket; // '提货日期D5',

-(NSString*)  getApplyDeliverydateend:(NSString*)ticket; // '提货日期D6',

-(NSString*)  getApplyDeliverynumber:(NSString*)ticket; // '提货号',

-(NSString*)  getApplySymbolcode:(NSString*)ticket; // '商品代码',

-(NSString*)  getApplyWarehousefeedate:(NSString*)ticket; // '仓储费截止日期',

-(NSString*)  getApplyRejectreason:(NSString*)ticket;

-(NSString*)  getApplySettlementdate:(NSString*)ticket; // '交割日期',

-(NSString*) getApplyMatchId:(NSString*)ticket;

-(NSString*) getApplyWarehouseid:(NSString*)ticket; // '仓库ID',

-(NSString*) getApplyCangId:(NSString*)ticket;	// 仓单ID

-(NSString*) getApplyExecuteticket:(NSString*)ticket; // '成交单号',

-(NSString*) getApplyCounterpartyexecuteticket:(NSString*)ticket; // '对手单号',

-(NSString*) getApplyDeliveryweight:(NSString*)ticket; // '提货重量',

-(NSString*) getApplySettlementprirce:(NSString*)ticket; // '交割价格',

-(char) getApplyDeliverystatus:(NSString*)ticket; // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',

-(char) getApplyIsinvoice:(NSString*)ticket; // '是否需要发票',
    
-(char) getApplyIscancel:(NSString*)ticket; // '是否撤销',

-(int) trade_current_monery_flowing;
//查询资金流水
-(int) trade_monery_flowing:(NSString*) start end:(NSString*) end;
//提交 “仓单” 转入库单
-(int) trade_cang_to_instore_order:(NSString*)ticket weight:(double)weight;
//撤消 “仓单转入库单”
-(int) trade_cancel_cang_to_instore_order:(NSString*)ticket;
//提交 “仓单转入库单”
-(int) trade_submit_cang_to_instore_order:(NSString*)ticket;

//入库单转仓单
-(int) trade_instore_to_cang_order:(NSString*)ticket weight:(double)weight;

//入库单转出库单
-(int) trade_instore_to_exportstore_order:(NSString*)ticket deliveryName:(NSString*)deliveryName weight:(double)weight idCode:(NSString*)idCode telPhone:(NSString*)telPhone;

//提交买 "申报交收" 请求
-(int) trade_buy_apply_delivery_settle:(NSString*)symbolcode qty:(double)qty cancel:(BOOL)iscancel;

//提交卖 "申报交收" 请求
-(int) trade_sell_apply_delivery_settle:(NSString*)symbolcode qty:(double)qty cangid:(NSString*)cangid cancel:(BOOL)iscancel;


-(BOOL) isRefresh;

-(BOOL) add_balance:(NSString*) server port:(int) port;

-(void) clear_balance;

-(void) set_login_info:(NSString*)account password:(NSString*)pwd;

-(void) start_balance;


-(int) getZYQSymbolKLineData:(NSString*) symbol nType:(int)nType data:(HistoryKData*)data size:(int)size;


-(NSString*) getSymbolNewQty:(NSString*) symbol;

-(NSString*) getSymbolNewPrice:(NSString*) symbol;

-(int) getSymbolTurnoverDirec:(NSString*) symbol;

-(NSString*) getSymbolPriceRiseDown:(NSString*) symbol;

-(NSString*) getSymbolPriceRiseDownRate:(NSString*) symbol;

-(NSString*) getSymbolTotalTurnoverFunds:(NSString*) symbol;

-(NSString*) getSymbolOrderRate:(NSString*) symbol;

-(NSString*) getSymbolQtyRate:(NSString*) symbol;

-(NSString*) getSymbolTotalTurnoverQty:(NSString*) symbol;

-(NSString*) getSymbolBuyVolPrice:(NSString*) symbol;

-(NSString*) getSymbolBuyVolQty:(NSString*) symbol;

-(NSString*) getSymbolSellVolPrice:(NSString*) symbol;

-(NSString*) getSymbolSellVolQty:(NSString*) symbol;
-(int) request_zyq_pankou_detail:(NSString*) symbol;

-(int) getNowDataVolPirce:(NSString*) symbol data:(STUNowDataVolPirce*)stuNowDataVolPirce;
-(NSString*) getIntPriceBySymbol:(NSString *)symbolcode price:(int)price;
-(NSString*) getIntQty:(int)qty;
-(int) getZYJSymbolTraceData:(NSString*) symbol data:(STUTraceUnit_ZYQ*) stuTraceUnit_ZYQ size:(int) size;
-(int) getZYJSymbolSTKMineData:(NSString*) symbol data:(STUSTKMinUnit_ZYQ*) stuSTKMinUnit_ZYQ size:(int) size;



//
//-(int) trade_sign:(NSString*) tel bankName:(NSString*) bankName bankAccount:(NSString*) bankAccount idCode:(NSString*) idCode password:(NSString*) password phonePwd:(NSString*) phonePwd remark:(NSString*) remark;
//
//-(int) trade_unsign:(NSString*) tel bankName:(NSString*) bankName bankAccount:(NSString*) bankAccount idCode:(NSString*) idCode password:(NSString*) password phonePwd:(NSString*) phonePwd remark:(NSString*) remark;

-(NSString*) getFundFlowTickets;
-(NSString*) getFundFlowAccountAlias:(long long) ids;
-(NSString*) getFundFlowExecuteTime:(long long) ids;
-(NSString*) getFundFlowAccountId:(long long) ids;
-(char) getFundFlowOperatortype:(long long) ids;
-(NSString*) getFundFlowChangeFunds:(long long) ids;
-(NSString*) getFundFlowBalance:(long long) ids;
-(long long) getFundFlowAssociateticket:(long long) ids;


-(NSString*) getInOutMoneryids;
-(NSString*) getInOutMoneryApplyTime:(long long) ids;
-(NSString*) getInOutMoneryAuditTime:(long long) ids;
-(NSString*) getInOutMoneryRemark:(long long) ids;
-(char) getInOutMoneryExecutestatus:(long long) ids;
-(long long) getInOutMoneryId:(long long) ids;
-(char) getInOutMoneryOperatortype:(long long) ids;
-(NSString*) getInOutMoneryAmount:(long long) ids;

-(BOOL) isSignStatus;
-(BOOL) isSuportInMonery;
-(BOOL) isSuportOutMonery;

-(BOOL) isSuportSign;
-(BOOL) isSuportUnSign;
-(int) getBankPasswordType;


/*******************************************
                文交所
 ******************************************/
- (NSString *)getBankName;
- (float)getNewPrice:(int)code;

//请求商品信息
-(void) trade_request_symbol;
//请求盘口数据，行情列表根据初始化行情产生
- (void)requestPankouData;

//下市价单 2014.07.30 增加偏离点差字段
-(int)trade_open_market_order:(NSString *)symbolcode direction:(NSString *)direction price:(double)price shoushu:(int)shoushu;

// 今日委托
- (int)trade_request_today_entrustNS;

- (int)trade_request_today_dealNS;

- (int)trade_request_cancel_order:(NSString *)orderID;

- (int)trade_request_shareholdNS;

- (int)trade_request_moneyholdNS;

- (int)quote_kline_reqNS:(int)klineType productID:(NSString *)productID;

- (int)quote_stktrace_reqNS:(NSString *)productID;
// 刷新盘口
- (void)sendReportData:(NSString *)productIDs;
// 历史委托
- (int)trade_request_historyorder_fromDate:(NSString *)fromDate toDate:(NSString *)toDate;
// 历史成交
- (int)trade_request_historyturnover_fromDate:(NSString *)fromDate toDate:(NSString *)toDate;
// 银证转账
-(int) trade_inout_moneny_type:(int)type money:(double)money moneyPwd:(NSString *)moneyPwd bankPwd:(NSString *)bankPwd;

- (float)getPreClosePrice:(int)productID;

- (int)trade_request_transferquery;

- (int)trade_request_pretrade:(NSString *)securityID type:(NSString *)orderType;
@end