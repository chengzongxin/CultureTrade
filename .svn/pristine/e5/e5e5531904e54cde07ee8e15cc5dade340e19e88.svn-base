
#ifndef __DATADICT_H__
#define __DATADICT_H__

#include <string>
#include "ErrorDef.h"
#include <vector>
/////////////////////////////////  Marcos  /////////////////////////////////
using namespace std;

// 份额产品查询时
#define ALLFUNDCOMPANY_ID	"-1"


// 所有的数据定义都将从此类进行派生
class CBaseInfo
{
public:
	CBaseInfo() {};
	~CBaseInfo() {};
    
public:
	// 合法性检查（包括完整性），若返回NULL则表示检查通过，否则返回错误提示字符串
	virtual const char* ValidityCheck(){return NULL;};
};

// 订单简要信息
class COrder : public CBaseInfo
{
public:
	string szProductID;//产品编号
	string szOrderInfo;//市价委托
	string fOrderPrice;//委托价格
	string lVolume;//委托数量
};

//wzm-2012-start
//订单信息请求结构
class COrderMm : public CBaseInfo
{
public:
	string szAccountId;		//账户ID
	string szTradeFlag;		//下单标志
	string szOpenorClose;	//下单类型
	string szOrderTicket;	//订单号
	string szContractId;	//商品
	string lOrderSize;		//手数
	string fTpPrice;		//止损价
	string fSlPrice;		//获利价
	string szOrderType;		//单据类型
	string lDifferPips;		//允许偏离点差
	string fOrderPrice;		//委托价格
	string fClientPrice;	//客户端报价
	string szSuborderType;	//交易类型
	string szFillPollcy;	//归档
	string szValidateType;	//效力类型
	string szValidateDate;	//截止日期时间
	string szDirection;		//买卖方向
	string szOperatorType;	//操作类型
	string szRemarkets;		//备注
	string lOrderSize1;		//反手建仓手数
};

//订单应答
class COrderRes : public CBaseInfo
{
public:
	string ssRetCode;		//返回码
	string ssOrderID;		//订单号
	string ssSymbol;		//商品
	string ssPrice;			//价格
};

//资金应答
class CFundsRes : public CBaseInfo
{
public:
    CFundsRes(){
        chRiskcalctype = '2';	//"风险率计算方式"
        dbStopOutLevel = 0.0;	//"自动斩仓风险率"
        dbMarginCallLevel = 0.0;//"追加保证金风险率"
        
        dbBalanceMargin = 0.0;	//可用预付款	double	20,4
        dbUsedMargin = 0.0;	//占用预付款	double	20,4
        dbFreezeMargin = 0.0;	//冻结预付款	double	20,4
        dbLockMargin = 0.0;	//锁仓预付款	double	20,4
        dbCredit = 0.0;		//信用金	double	20,4
        dbCommision = 0.0;		//累计佣金	double	20,4
        dbPl = 0.0;			//盈亏	double	20,4
        dbInterest = 0.0;		//递延费	double	20,4
        dbIn = 0.0;			//入金	double	20,4
        dbOut = 0.0;			//出金	double	20,4
        dbAdjustment = 0.0;	//交易调整	double	20,4
        dbOpenbalance = 0.0;	//期初余额	double	20,4
        
        dbLastmargin = 0.0;	//上日剩余
        dbFpl = 0.0;			//浮动盈亏
        dbSumposition = 0.0;	//累计头寸
        
        dbFPL = 0.0;
        dbClosePL = 0.0;
        dbTmpValue = 0.0;//净值
        dbRisk = 0.0;
        iLevel = 0;
        
        chTradeOrderMode = '\0';//"交易单据处理模式");
        
        chNodebtclear = '\0';//"结算模式");
        
        dbMininmounteverytime = 0.0;//"每次最小入金金额");
        
        dbInoutcommision = 0.0;//"出入金手续费");
        chInoutcommisiontype = '\0';
    }
    string ssCurrency;		//货币
    string ssSymbolRuleGroupName;//商品规则组名称
    
    char chRiskcalctype;	//"风险率计算方式"
    double dbStopOutLevel;	//"自动斩仓风险率"
    double dbMarginCallLevel;//"追加保证金风险率"
    
    double dbBalanceMargin;	//可用预付款	double	20,4
    double dbUsedMargin;	//占用预付款	double	20,4
    double dbFreezeMargin;	//冻结预付款	double	20,4
    double dbLockMargin;	//锁仓预付款	double	20,4
    double dbCredit;		//信用金	double	20,4
    double dbCommision;		//累计佣金	double	20,4
    double dbPl;			//盈亏	double	20,4
    double dbInterest;		//递延费	double	20,4
    double dbIn;			//入金	double	20,4
    double dbOut;			//出金	double	20,4
    double dbAdjustment;	//交易调整	double	20,4
    double dbOpenbalance;	//期初余额	double	20,4
    
    double dbLastmargin;	//上日剩余
    double dbFpl;			//浮动盈亏
    double dbSumposition;	//累计头寸
    
    double dbFPL;
    double dbClosePL;
    double dbTmpValue;//净值
    double dbRisk;
    int iLevel;
    
    char chTradeOrderMode;//"交易单据处理模式");
    
    char chNodebtclear;//"结算模式");
    
    double dbMininmounteverytime;//"每次最小入金金额");
    
    double dbInoutcommision;//"出入金手续费");
    char chInoutcommisiontype;//出入金手续费类型
};

class CMoneyHolder : public CBaseInfo{
public:
    CMoneyHolder(){
        dAccountID = 0.0;
        dAccountBalance = 0.0;
        dAccountFreezeMoney = 0.0;
        dClearDate = 0.0;
        dTotalBalance = 0.0;
        dCanOut = 0.0;
        dOtherFreeze = 0.0;
    }
    ~CMoneyHolder(){}
    double dAccountID;
    double dAccountBalance;
    double dAccountFreezeMoney;
    double dClearDate;
    double dTotalBalance;
    double dCanOut;
    double dOtherFreeze;
};

class CPersonalInfo: public CBaseInfo {
public:
    string personBankName;
};

class CMoneryInOutRes : public CBaseInfo {
public:
    CMoneryInOutRes(){
        iId = 0;
    }
    ~CMoneryInOutRes(){}
    string ssApplyTime;                 //请求时间
    string ssAuditTime;                 //审核时间
    string ssRemark;                    //备注
    char chExecutestatus;               //执行状态0=未处理,1=已处理,2=作废',
    long long iId;                      //ID
    char chOperatortype;                //操作类型
    double dbAmount;					//入出金金额
};

//查资金流水
//class CFundsLogRes : public CBaseInfo
//{
//public:
//	string ssRetCode;		//int	int
//	string ssAccountID;		//accountid	账户ID	int
//	string ssAccountAlias;	//账户	char	128
//	string ssExecuteDate;	//执行日期
//	string ssChange;		//发生金额	double
//	string ssBalance;		//余额	double
//	string ssOperatorType;	//操作类型			1=盈亏;2=资金入金;3=出入金调整;4=仓息划拔;5=扣除佣金;6=资金出金',
//	string ssOperatorAlias;	//操作员	char	128
//	string ssAssociateTicket;//关联单号
//};

class CFundsFlowRes : public CBaseInfo {
public:
    CFundsFlowRes(){
        iAccountId = 0;
        iAssociateticket=0;
        dbChangeFunds=0.0;
        dbBalance=0.0;
    }
    ~CFundsFlowRes(){}
    string ssAccountAlias;                 //请求时间
    string ssExecuteTime;                 //审核时间
    long long iAccountId;                      //ID
    char chOperatortype;                //操作类型
    double dbChangeFunds;					//入出金金额
    double dbBalance;
    long long iAssociateticket;
};

//查单据应答
class CFindOrderRes : public CBaseInfo
{
public:
    CFindOrderRes()
    {
        fFpl = 0.0;
        iAccountID=0;
        iOrderticket=0;
        
        dbDeliveryFreezeMargin=0.0;
        dbDeliveryPayOut=0.0;
        chDeliverytype=0.0;
        chOpenorclose='\0';
        dbPrice=0.0;
        dbSlPrice=0.0;
        dbStopPx=0.0;
        dbStopLimitPrice=0.0;
        dbCurrentPrice=0.0;
        chSide='\0';
        chFillPollcy='\0';
        dbOrderQty=0.0;
        dbCumQty=0.0;
        dbExecuteprice=0.0;
        dbInterest=0.0;
        dbSumInterest=0.0;
        dbInterestDays=0;
        dbMargin=0.0;
        chOperatorType='\0';
        dbCloseordersize=0.0;
        dbCloseprice=0.0;
        dbCloseMarginCurrecyPrice=0.0;
        dbPaymentCurrencyPrice=0.0;
        dbCloseAccountCurrencyRate=0.0;
        dbOpenClosePl=0.0;
        dbCloseCommision=0.0;
        dbOpenCommision=0.0;
        dbPl=0.0;
        fFpl=0.0;
        chOrdStatus='\0';
        chIsClose='\0';
        chOrderType='\0';
        chSubOrderType='\0';
        chExpiration='\0';
        orderType=0;
        chYesday='\0';
        nDecimal=0;
        dbHolderSize=0.0;
        dbHolderPrice=0.0;
        dbOpenPrice=0.0;
    }
    ~CFindOrderRes(){}
	string ssRetCode;		//返回码	double	20,4
	string ssAccountAlias;	//账户	char	128
	string ssInstrumentID;	//合约月份	Int
	string ssSymbol;		//Instrumentcode	合约代码	char	20
	string ssOrderTime;		//Ordertime	订单时间	char	24	下订单的时间
    string ssExecuteTime;	//Ordertime	成没交时间
	string ssTransactTime;	//executetime	完成时间	Char 	24	订单完成时间
	string ssText;			//remarks	备注	char	128
	string ssCloseText;     //remarks	平仓备注	char	128
	string ssOperatorAlias;	//操作员代码	char	128
	string ssCloseOperatorAlias;//平仓操作员代码
    
	string ssCloseticket;	//平仓单号
	string ssClosetime;		//平仓时间
	string ssExpirationdate;//截止时间
    /*2.0 start*/
    string ssAccountName;//客户名称
    
    
    string ssDelegateTime;//单据委托时间
    
    // FIX_TAG_BODY_BARGAIN_COMMISION
    
    long long iBargainId;//成交单号
    long iAccountID;		//账户ID	int	16
    long long iOrderticket;		//订单号	Int
    double dbDeliveryFreezeMargin;			//	交收冻结保证金
    double dbDeliveryPayOut;				//	交收货款
    char chDeliverytype;				//交收类型（1- 普通交收，2- 中立仓交收）';
    char chOpenorclose;					//  下单类型	Char	1	‘1’ - 建仓 ‘2’ - 平仓 ‘3’-交收 '4'-平今仓 '5'-平昨仓
    /*2.0 end*/
    
    double dbOpenPrice; //开仓价格
    double dbPrice;			//orderprice	订单价格	double	10,4
    double dbSlPrice;		//止损价格	double	10,4
    double dbStopPx;		//tpprice	止盈价格 	double	10,4
    double dbStopLimitPrice;//停损触发价	double	10,4
    double dbCurrentPrice;	//当前行情价格	double	10,4

    char chSide;			//direction	买/卖	Char 	1
    char chFillPollcy;	//归档	Char 	1	'归档 ‘1’ - 全部或无 ‘2’- 剩余撤销  ‘3’- 剩余返回
    double dbOrderQty;		//ordersize	订单数量	double	20,4
    double dbCumQty;		//executesize	成交数量	double	20,4
    double dbExecuteprice;	//执行价格	double	10,4	订单执行的价格
    double dbInterest;		//递延费	Double 	20,4
    double dbSumInterest;	//累计递延费	Double 	20,4  (利息)
    int dbInterestDays;	//计息天数	Int
    //    //string ssOrdStatus;		//Status	状态	Char	1	处理状态 1未成交  2已成交  3已撤单
    double dbMargin;		//单据保证金	double	20,4
    char chOperatorType;	//操作类型	Char	1	1帐户2代客下单3系统斩仓4结算锁仓斩仓

    double dbCloseordersize;//平仓数量
    double dbCloseprice;	//平仓价格
    double dbCloseMarginCurrecyPrice;//保证金货币价格
    double dbPaymentCurrencyPrice;//计价货币兑换结算货币价格
    double dbCloseAccountCurrencyRate;//当前结算货币到账户汇率转换价格

    double dbOpenClosePl;	//建仓到平仓盈亏
    double dbCloseCommision;//平仓手续费
    double dbOpenCommision;	//	开仓手续费
    double dbBargainCommision;	//	成交手续费
    
    double dbPl;			//盈亏
    double fFpl;
    char chOrdStatus;		//Status	状态	Char	1	处理状态 1未成交  2已成交  3已撤单
    char chIsClose;		//是否平仓
    char chOrderType;		//ordertype	订单类型	Char	1	1-市价 2- 限价
    char chSubOrderType;	//子订单类型	char 	1	'子定单类型', 1 - buy ,2 - sell ,3 - buy limit 4 - buy stop 5 - sell limit 6 - sell stop
    char chExpiration;	//效力类型	expiration	expiration	1-GTC (截止日期、时间) 2 - day 当日有效  3 - 永久有效
    
    char chOpenClose; //下单类型;
    int orderType;//1、表示做市商 2、表示中远期
    char chYesday;
    int nDecimal;
    
    double dbHolderSize;
    double dbHolderPrice;
    
    
    
    long long iCounterPartExecuteTicket;
    double dbDeferredCompensation;
    string ssSellCangId;
    //买方货款
    double dbBuyPayOut;
    double dbMatchWeight;
    string ssMatchDate;
    long iDayStep;
    double dbDeliveryMargin;//交收冻结保证金
    double dbDeliveryPayment;//交收贷款
};



//商品定义SYMBOL_PARAM,*LPSYMBOL_PARAM
class CSymbolParam : public CBaseInfo
{
public:
    CSymbolParam()
    {
        fCurPrice = 0.0;
        fBidPrice = 0.0;
        fAskPrice = 0.0;
        lastTick = 0;
        fOpenPrice = 0.0;
        fMaxPrice = 0.0;
        fMinPrice = 0.0;
        fClosePrice = 0.0;
        //涨跌方向 -1跌 0平 1涨
        nCurPriceDirection = 0;
        fYesterdayClosePrice=0;
        dbAgreeunit=0.0;			//合约单位
        
        chIstrade='\0';
        dbMinpersize=0.0;		//最小可交易数量
        dbMaxpersize=0.0;		//最大可交易数量
        dbPendingminpersize=0.0;	//挂单最小可交易数量
        dbStepsize=0.0;			//交易数量步长
        dbMaxBuyPosition=0.0;	//最大买持仓数量
        dbMaxSellPosition=0.0;	//最大卖持仓数量限制
        dbTppips=0.0;			//止盈下单点差
        dbSlpips=0.0;			//止损下单点差
        dbMaxdeviationpips=0.0;	//最大偏离点差
        dbLimitorstoppips=0.0;	//限价停止点差
        dbBuypips=0.0;			//报价买价点差
        dbSellpips=0.0;			//报价卖价点差
        chTraderight='\0';
        chOpenCloseStatus='\0';	//开收市状态 :'1' 开市,'2' 休市
        nDecimal=0;				//行情小数位数
        dbAccountReckonRate=0.0;			////对应特定账户结算汇率==》账户汇率转换

        fCurPrice=-1.0;  //最新价格
        fBidPrice=-1.0;//卖价
        fAskPrice=-1.0;//买价

        fOpenPrice=-1.0;//开盘价
        fMaxPrice=-1.0;//最高价
        fMinPrice=-1.0;//最低价
        fClosePrice=-1.0;//收盘价
        fSettlPrice=-1.0;//昨结
        fYesterdayClosePrice=-1.0;//昨收
        flastPrice=-1.0; //上一次的价格
        nCurPriceDirection=0;
        lastTick=0;
        
        //2.0
        dbMinchangeunit=0.0;
        dbQuoteminunit=0.0;							//报价最小单位（订单价位）
        chSpecies='\0';									//品种类型 1-外汇2-现货3-递延4-期货
        iMaxquoteTimeOut=0;							//最高报价延时，设置行情报价超时
        chExectype='\0';								//执行方式 1–即时 2–询价 3–内部市场 4–交易所
        chDefaultmarginsetting='\0';					//保证金默认设置
        chMargintype='\0';								//保证金计提方式 1-比例2-固定
        dbOrginmargin=0.0;							//初始保证金
        dbFreezemargin=0.0;							//即市冻结保证金
        dbLockmargin=0.0;							//锁仓保证金
        chDefaultsizesetting='\0';						//交易数量限制默认设置
        dbMaxDoublePosition=0.0;						//最大双边订货量
        dbBasePrice=0.0;								//基准价格
        chDefaultradesetting='\0';						//默认交易设置
        iTraderight=0;								//交易权限 1 - 禁用 2- 仅买入 3 - 仅卖出 4-仅平仓 5- 全部交易 '
        chFillpollcy='\0';								//归档 ‘1’ - 全部或无 ‘2’- 剩余撤销  ‘3’- 剩余返回
        chExpiration='\0';								//1-GTC (截止日期、时间) 2 - day 当日有效  3 - 永久有效
        chDefaultcommisionsetting='\0';					//手续默认设置
        chCommisiontype='\0';							//手续计提方式  '1' -- 固定 ‘2’ -- 比例
        dbOpencommision=0.0;							//建仓手续费
        dbCloseTodayCommision=0.0;					//平今仓手续费
        dbCloseHistoryCommision=0.0;					//平昨仓手续费
        dbTradecommisionadd=0.0;						//交易员手续附加
        chDeliverycommisiontype='\0';					//交收手续费类型
        dbDeliveryBuycommision=0.0;					//交收买手续费
        dbDeliverySellcommision=0.0;
        iShowOrder=0;									//显示顺序
        symbolType=0;
    }
    ~CSymbolParam()
    {
        
    }

	string ssSymbolgroupid;		//商品组ID
	string ssSymbolid;			//合约月份Id
	string ssSymbolcode;		//合约代码
	string ssExtraquotecode;	//合约月份外部行情代码
	string ssSymbolname;		//商品名
	//string ssBegintime;		//有效开始时间
	//string ssEndtime;			//有效结束时间
	string ssBasecurrency;		//基准货币
	string ssMargincurrency;	//保证金货币
	string ssPaymentcurrency;	//计价货币
	string ssReckoncurrency;	//结算货币
	string ssQuotationType;		//报价类型
    double dbAgreeunit;			//合约单位

//	string ssAgreeunit;			//合约单位
//	string ssMinchangeunit;		//订单大小（最小变动单位）
//	double ssQuoteminunit;		//报价最小单位（订单价位）
	//char chSpecies;			//品种类型 1-外汇2-现货3-递延4-期货
	//long iMaxquoteTimeOut;	//最高报价延时，设置行情报价超时
	//char chExectype;			//执行方式 1–即时 2–询价 3–内部市场 4–交易所
    char chIstrade;			//是否交易品种 1交易品种 2非交易品种
	//char chBuyopen;			//买单开仓 1-是2-否
	//char chDefaultswapsettings;//递延默认设置
	//long iThreedayswap;		//三天收息
	//char chSwaptype;			//递延计提方式 1 固定值 2 按天比例 3 按年比例
	//double dbSellswap;		//卖出递延
	//double dbBuyswap;			//买进递延
	//char chDefaultmarginsetting;//保证金默认设置
//	char chMargintype;		//保证金计提方式 1-比例 2-固定
//	double dbOrginmargin;		//初始保证金
//	//double dbDayTradeMargin;	//即市（占用）保证金
//	//double dbOverNightMargin;	//过市（过夜）保证金
//	double dbFreezemargin;		//即市冻结预付款
	//string ssLockmargin;		//锁仓预付款
	//char chDefaultsizesetting;//交易数量限制默认设置
	double dbMinpersize;		//最小可交易数量
	double dbMaxpersize;		//最大可交易数量
	double dbPendingminpersize;	//挂单最小可交易数量
	double dbStepsize;			//交易数量步长
	double dbMaxBuyPosition;	//最大买持仓数量
	double dbMaxSellPosition;	//最大卖持仓数量限制
	//char chDefaulpipssetting;	//默认点差设置
	double dbTppips;			//止盈下单点差
	double dbSlpips;			//止损下单点差
	double dbMaxdeviationpips;	//最大偏离点差
	//long iFreezepips;			//冻结点差
	double dbLimitorstoppips;	//限价停止点差
	double dbBuypips;			//报价买价点差
	double dbSellpips;			//报价卖价点差
	//char chDefaultradesetting;//默认交易设置
	char chTraderight;		//交易权限 1 - 禁用 2- 仅买入 3 - 仅卖出 4-仅平仓 5- 全部交易 '
	//char chFillpollcy;		//归档 ‘1’ - 全部或无 ‘2’- 剩余撤销  ‘3’- 剩余返回
//	string ssExpiration;		//1-GTC (截止日期、时间) 2 - day 当日有效  3 - 永久有效
	//char chDefaultcommisionsetting;//手续默认设置
	//char chCommisiontype;		//手续计提方式  '1' -- 固定 ‘2’ -- 比例
	//double dbOpencommision;	//建仓手续费
	//double dbClosecommision;	//平仓手续费
	//double dbTradecommisionadd;//交易员手续附加
	char chOpenCloseStatus;	//开收市状态 :'1' 开市,'2' 休市
	int nDecimal;				//行情小数位数
	double dbAccountReckonRate;			////对应特定账户结算汇率==》账户汇率转换
//    int nSortNum;
    
    double fCurPrice;  //最新价格
	double fBidPrice;
	double fAskPrice;
	long long lastTick;
    
    double fOpenPrice;
    double fMaxPrice;
    double fMinPrice;
    double fClosePrice;//收盘价
    double fSettlPrice;//昨结
    
    double fYesterdayClosePrice;//昨收
    
    double flastPrice; //上一次的价格
    
    //涨跌方向 -1跌 0平 1涨
    int     nCurPriceDirection;
    
    
    
    //2.0
    double dbMinchangeunit;
    
//#define FIX_TAX_BODY_MINCHANGE_UNIT        1221    // "订单大小"
    double dbQuoteminunit;							//报价最小单位（订单价位）
//#define FIX_TAG_BODY_QUOTEMINUNIT			8068	//报价最小单位
    char chSpecies;									//品种类型 1-外汇2-现货3-递延4-期货
//#define FIX_TAX_BODY_SPECIES               1222    //品种类型
    int iMaxquoteTimeOut;							//最高报价延时，设置行情报价超时
//#define FIX_TAX_BODY_MAXQUOTE_TIMEOUT      1223    //最高报价延时
    char chExectype;								//执行方式 1–即时 2–询价 3–内部市场 4–交易所
//#define FIX_TAX_BODY_EXECTYPE              1224    //执行方式
    char chDefaultmarginsetting;					//保证金默认设置
//#define FIX_TAX_BODY_DEF_MARGINSETTING     1225    //保证金默认设置
    char chMargintype;								//保证金计提方式 1-比例2-固定
//#define FIX_TAG_BODY_MARGINTYPE             8103    //保证金类型
    double dbOrginmargin;							//初始保证金
//#define FIX_TAG_BODY_ORGINMARGIN            8104    //初始保证金
    double dbFreezemargin;							//即市冻结保证金
//#define FIX_TAG_BODY_FREEZEMARGIN           8037    //冻结保证金
    double dbLockmargin;							//锁仓保证金
//#define FIX_TAX_BODY_LOCK_MARGIN           1226    //锁仓保证金
    char chDefaultsizesetting;						//交易数量限制默认设置
//#define FIX_TAX_BODY_DEF_SIZESETTING       1227    //交易数量限制默认设置

    double dbMaxDoublePosition;						//最大双边订货量
//#define FIX_TAX_BODY_MAX_DOUBLEPOSITION    1228    //最大双边订货量
    double dbBasePrice;								//基准价格
//#define FIX_TAX_BODY_BASE_PRICE            1229    //基准价格
    char chDefaultradesetting;						//默认交易设置
//#define FIX_TAX_BODY_DEF_TRADESETTING      1230    //默认交易设置
    int iTraderight;								//交易权限 1 - 禁用 2- 仅买入 3 - 仅卖出 4-仅平仓 5- 全部交易 '
//#define FIX_TAG_BODY_TRADERIGHT				8079	//交易权限
    char chFillpollcy;								//归档 ‘1’ - 全部或无 ‘2’- 剩余撤销  ‘3’- 剩余返回
//#define FIX_TAG_BODY_FILLPOLLCY             8029    //归档
    char chExpiration;								//1-GTC (截止日期、时间) 2 - day 当日有效  3 - 永久有效
//#define FIX_TAG_BODY_VALIDATETYPE           8030    //效力类型
    char chDefaultcommisionsetting;					//手续默认设置
//#define FIX_TAX_BODY_DEF_COMMISIONSETTING  1231    //手续默认设置
    char chCommisiontype;							//手续计提方式  '1' -- 固定 ‘2’ -- 比例
//#define FIX_TAX_BODY_COMMISIONTYPE         1232    //手续计提方式
    double dbOpencommision;							//建仓手续费
//#define FIX_TAX_BODY_OPEN_COMMISION        1233    //建仓手续费
    double dbCloseTodayCommision;					//平今仓手续费
//#define FIX_TAX_BODY_CLOSE_TODAYCOMMISION  1234    // 平今仓手续费
    double dbCloseHistoryCommision;					//平昨仓手续费
//#define FIX_TAX_BODY_CLOSE_HISTORYCOMMISION 1239   //平昨仓手续费
    double dbTradecommisionadd;						//交易员手续附加
//#define FIX_TAX_BODY_TRADE_COMMISIONADD    1235    //交易员手续附加
    char chDeliverycommisiontype;					//交收手续费类型
//#define FIX_TAX_BODY_DLY_COMMISIONTYPE     1236    //交收手续费类型
    double  dbDeliveryBuycommision;					//交收买手续费
//#define FIX_TAX_BODY_DLY_BUYCOMMISION      1237    //交收买手续费
    double  dbDeliverySellcommision;				//交收卖手续费
//#define FIX_TAX_BODY_DLY_SELLCOMMISION     1238    //交收卖手续费
    int iShowOrder;									//显示顺序
//#define FIX_TAG_BODY_SHOWORDER				8138	//显示顺序
   
    int symbolType;//商品类型
    
};
//查询出入金OUT_MONEY_IN_OUT, *LPOUT_MONEY_IN_OUT;
class CMoneyInOutList : public CBaseInfo
{
public:
	string ssId;				//ID
	string ssOperatortype;		//操作类型
	string ssAmount;			//入出金金额
	string ssApplyTime;			//请求时间
	string ssAuditTime;			//审核时间
	string ssExecutestatus;		//执行状态0=未处理,1=已处理,2=作废',
	string ssRemark;			//备注
};

class CLocalSetup : public CBaseInfo
{
public:
	string ssDefaultSymbol;		//默认商品 0:自动选择 1:上次使用的 2：商品
	string ssDefaultVolume;		//默认手数 0:上次使用的 1：手数
	string ssDefaultDeviation;	//默认偏差 0:上次使用的 1:偏差
    
	string ssLastUsedSymbol;	//最后使用的商品
	string ssLastUsedVolume;	//最后使用的量
	string ssLastUsedDeviation;	//最后使用的偏差
    
	string ssSetupSymbol;		//设置的商品
	string ssSetupVolume;		//设置的量
	string ssSetupDeviation;	//设置的偏离点差
};

class CAlertSetup : public CBaseInfo
{
public:
	int iNo;			//序号
	string ssUsed;			//启用/关闭
	string ssAlertType;		//报警类型
	string ssSymbol;		//商品
	string ssCondition;		//条件
	string ssPrice;			//价格
	string ssExec;			//执行
	string ssTime;			//时间
	string ssRepeatTimes;	//次数
	int    nTimes;//已经重复次数
};

//#pragma pack(1)

struct SCodeInfo
{
	char  szCode[11]; //名称
	char  cPriceUnit;//价格位数
	bool operator == (const SCodeInfo& right)
	{
		if(0 == memcmp(this,&right,sizeof(*this)))
			return true;
		return false;
	}
};

//struct	SQuoteData
//{//实时行情
//	unsigned	int 	m_uiTime;//时间
//	SCodeInfo			sCode;	  //产品代码
//	unsigned	int		m_uiNewPrice;//最新价
//	unsigned	int		m_uiBidPrice;//卖价
//	unsigned	int		m_uiAskPrice;//买价
//
//	SQuoteData()
//	{
//		clear();
//	}
//	void clear()
//	{
//		m_uiTime = 0;
//		m_uiNewPrice = 0;
//		m_uiBidPrice = 0;
//		m_uiAskPrice = 0;
//	}
//};
//#pragma pack()
//struct	SNowData
//{
//	__time32_t	timeUpdate;
//	SCodeInfo	sCode;	  //产品代码
//	unsigned int uiOpenPrice;//今开盘
//	unsigned int uiMaxPrice;//最高价
//	unsigned int uiMinPrice;//最低价
//	unsigned int uiNewPrice;//最新价
//};

//行情信息
struct StuRecvQuoteInfo
{
	SCodeInfo sCode;
	unsigned int uiPrice;
	time_t lastTick;
};

class CQuickOrderData:public CBaseInfo
{
public:
	int nMode;
	string ssCode;
	double dbPrice;
};

class COutMoneyInOutApply:public CBaseInfo
{
public:
	string ssAccountId;								//帐号ID
	string ssOperatortype;							//操作类型
	string ssAmount;								//入出金金额
	string ssTelephone;								//申请人电话
	string ssBankName;								//开户行
	string ssBankAccount;							//开户账户
	string ssIdCode;								//身份证号
	string ssExecutestatus;							//执行状态0=未处理,1=已处理,2=作废',
	string ssApplyOperatorId;						//申请人ID
	string ssMoneyPwd;
	string ssRemarks;
};

class COutMoneyInOutApplyRes:public CBaseInfo
{
public:
	string ssId;									//申请流水号
	string ssApplyTime;								//申请接受时间
};

////公告抬头
//class CDeclareTitle:public CBaseInfo
//{
//public:
//	string	ssAnnouceID;//公告ID
//	string  ssDelcare;//公告发布者
//	string  ssTime;//公告时间
//	string  ssTitle;//公告标题
//	string  ssReadStatus;//阅读状态 1已阅读 2 未阅读
//};
//wzm-2012-end

class CPreTradeData:public CBaseInfo
{
public:
	string	 nMaxBuyVol;//最大可买量
	string  nMinBuyVol;//最小可买量
	string  nMaxSellVol;//最大可卖量
	string  nMinSelVol;//最小可卖量
	string  dCurrentBalance;//当前资金账户可用数
	string	nProductBalance;	//当前该产品份额可用数
};



class CTradeOrder
{
public:
	CTradeOrder() {};
	~CTradeOrder() {};
	COrder sOrder;
	string  szProductName;
	string		nStatus;//当前状态   1 :已提交 2 成交 3 部分成交 4主动撤单  5交易所拒绝 6已冻结 7 其他
	string		nTurnoverNum;//已成交数量
	string		nCancelNum;//已撤单数量
	string		szContractNo; //委托合同号
	string		nOrderDate;//委托日期
	string		nOrderTime;//委托时间
};
// 份额持仓信息


// 份额持仓信息

class CSwingOrder : public CBaseInfo  // 份额持仓信息
{
public:
	string szProductID;		//代码g
	string szProductName;	//产品名称
	string nChosedDate;		//配号日期
	string ssStartChosed;	//起始配号
	string ssEndChosed;		//结束配号
	string ssAskNum;		//申购数量
	string ssAskPrice;		//中签价格
	string nChosedNum;		//中签数量
    string nChonsedType;	//配号类别 0为配号 1中签
};


class CTransferForm:public CBaseInfo  //转账信息
{
public:
    string  szAcctPwd;//银行转账密码
    string  cDirection;//I出金 O入金
    string  dMoney; //转帐金额
    
};

//转账明细
class CTransferFormOders:public CBaseInfo  //转账信息
{
public:
	string  szAcctID; //资金账户
	string  nTradeDate; //成交日期
	string  dOrderMoney; //转账金额
	string  cDirection;//I出金 O入金
	string  szRemark;//摘要
	string  szBankSerialNo;//银行流水号
	string  dCurrentBalance;//当时资金余额
};

// 成交信息
class COrderTurnOver : public CBaseInfo
{
public:
    string	szProductCode;//产品编号
	string	szProductName;//产品名称
	string	szInterestsCode;//权益人编号
	string	szInteresteName;//权益人名称
	string	szContractCode;//合同编号
	string	szOrderType; //买卖方向
	string	lAccount;			//成交量
	string	fTurnoverPrice;		//成交价格
	string	dMoney;				//总金额
	string	nTradeDate;			//成交日期	yyyymmdd
	string	nTradeTime;			//成交时间	hhmmss
	string	szCustomID;			//投资人编号
	string	dCommion;			//交易佣金
};

//class CProductInfo : public CBaseInfo			//产品信息
//{
//public:
//    string szProductID;//产品编号
//    string szProductName;//产品名称
//    string	dTotalPrice;	//权益总价
//    string	nTotalVolume;	//总份数
//    string	fSinglePrice;	//每份金额
//    string	nReserveVolume;	//保留份数
//    string	nPublicVolume;	//发行份数
//    string	nMinimumAsk;	//最低申购手数
//    string	fPercent;		//申购率
//    string	nTradeType;//交易方式
//    string	szPublicUserID;//发布产品人的编号
//    string	cProductState;//	状态
//    string	nSignDate;//	挂牌日期
//    string	nPublicDate;//	发行日期
//    string	nDestoryDate;//	截止日期
//    string	lAskVolume;//	已申购份数
//};


#define PRODUCT_NAME_LEN 32

// 新的商品列表结构体
struct SProductInfo			//产品信息
{
    char szProductID[7];//产品编号
    char szProductName[PRODUCT_NAME_LEN+1];//产品名称
    double	dTotalPrice;	//权益总价
    long	nTotalVolume;	//总份数
    float	fSinglePrice;	//每份金额
    long	nReserveVolume;	//保留份数
    long	nPublicVolume;	//发行份数
    long	nMinimumAsk;	//最低申购手数
    long    nMaxnumAsk;    //最大申购手数
    float	fPercent;		//申购率
    int		nTradeType;//交易方式
    char	szPublicUserID[20];//发布产品人的编号
    char	cProductState;//	状态
    char	szSignDate[11];//	挂牌日期
    char	szPublicDate[11];//	发行日期
    char	szDestoryDate[11];//	截止日期
    long	lAskVolume;//	已申购份数
    SProductInfo()
    {
        memset(this,0,sizeof(SProductInfo));
    }
};


class CProductBalanceClt
{
public:
	string  lBalance;	//权益余额
	string  szProductID;//	产品编码
	string  nBigKind;	//产品类别
	string  szProductBigName;//产品类别名称
	string	nSmallKind;//产品子类别
	string  szSmallName;//	产品子类名称
	string  szProductName;//产品名称
	string  nType;	//产品类型
	string  nLockedVolume;//锁定数量
};

typedef vector<string> STRING_VECTOR;
typedef std::map<int, STRING_VECTOR> FIXRECORDSET;

/***************************START*********************************************/
//公告抬头
class CDeclareTitleDesc
{
public:
    long long tTime;//公告时间
//    #define FIX_TAG_BODY_DECLARETIME            8113    //公告时间
    int	   nAnnouceID;//公告ID
//    #define FIX_TAG_BODY_DECLAREID              8110    //公告编号
    string   szDelcare;//公告发布者
//    #define FIX_TAG_BODY_DECLAREPUBLICER        8111    //公告发布者
    string   szTitle;//公告标题
    string   szContent;//公告内容
//    #define FIX_TAG_BODY_DECLARETITLE           8112    //公告标题
    char   cReadStatus;//阅读状态 1已阅读 2 未阅读
//    #define FIX_TAG_BODY_DECLAREREADSTATUS      8109    //阅读状态
};


//交收仓库信息
class COutCAUDeliveryWarehouseInfo
{
public:
    COutCAUDeliveryWarehouseInfo(){
        iWareHouseId=0;
    }
    long long iWareHouseId;
    string  szWareHouseName;
    string   szWareHouseFullName;
    string   szWareHouseCode;
    string   szAddress;
    string   szAuthenticatedPerson;
    string   szKeeper;
    string   ssZipCode;
    string   ssFax;
    string   ssEmail;
};

class COUTSymbolAttributeValue
{
public:
    int iAttributeId;
    string szAttvalue; // zhouhaihua ?? 长度?
};

#define   S_SYMBOL_ATTRIBUTE_MAX_LEN 25


class CDeliveryInfo
{
public:
    CDeliveryInfo(){
        iDeliveryid=0; // '提货单ID',
        iAccountgroupid=0; // '机构ID',
        iAccountid=0; // '客户ID',
        iMatchId=0;
        iSymbolgroupid=0; // '品种',
        
        iWarehouseid=0; // '仓库ID',
        iCangId=0;	// 仓单ID
        iExecuteticket=0; // '成交单号',
        iCounterpartyexecuteticket=0; // '对手单号',
        dbDeliveryweight=0.0; // '提货重量',
        
        dbSettlementprirce=0.0; // '交割价格',
        chDeliverystatus='\0'; // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',
        
        chIsinvoice='\0'; // '是否需要发票',
        chIscancel='\0'; // '是否撤销',
    }
    string ssDeliverymen; // '提货人',
    string ssIdcode; // '身份证',
    string ssTelephone; // '联系电话',
    string ssAddress; // '联系地址',
    string ssZipcode; // '邮政编码',
    
    string ssInvoicetitle; // '发票抬头',
    string ssCompanyname; // '公司名称',
    string ssCompanyaddress; // '公司地址',
    string ssCompanytelephone; // '公司电话',
    string ssBankaccount; // '银行账号',
    string ssBankdeposit; // '开户行',
    string ssTaxregistrynumber; // '税务登记号',
    
    string ssDeliverydatebegin; // '提货日期D5',
    string ssDeliverydateend; // '提货日期D6',
    string ssDeliverynumber; // '提货号',
    string ssSymbolcode; // '商品代码',
    string ssWarehousefeedate; // '仓储费截止日期',
    string ssRejectreason;
    string ssSettlementdate; // '交割日期',
    
    long long iDeliveryid; // '提货单ID',
    int iAccountgroupid; // '机构ID',
    int iAccountid; // '客户ID',
    int iMatchId;
    int iSymbolgroupid; // '品种',
    
    int iWarehouseid; // '仓库ID',
    int iCangId;	// 仓单ID
    int iExecuteticket; // '成交单号',
    int iCounterpartyexecuteticket; // '对手单号',
    double dbDeliveryweight; // '提货重量',
    
    double dbSettlementprirce; // '交割价格',
    char chDeliverystatus; // '提货状态;{"from":"json","0":"录入","1":"撤销","2":"待仓库发货","3":"待提货“,"4“:"已提货"}',

    char chIsinvoice; // '是否需要发票',
    char chIscancel; // '是否撤销',
};

class CStoreOrderInfo
{
public:
    CStoreOrderInfo() {
        iCangid=0;
        iImportid=0; // '入库单ID',
        iWarehouseid=0; // '交收仓库',
        iAccountgroupid=0; // '机构',
        iAccountid=0; // '账户',
        iApplyid=0; // '入库申请单ID',
        iSymbolgroupid=0; // '品种名称',
        chImportstatus='\0'; // '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',
        dbApplyweight=0.0; // '申请重量',
        dbImportweight=0.0; // '入库重量',
        dbFreezeexportweight=0.0;
        dbExportweight=0.0;
        dbFreezecangweight=0.0;
        dbCangweight=0.0;
        dbUnitweight=0.0; // '每件重量',
        iQuantity=0; // '件数',
        iAttributeNum='\0';
        chExportStatus='\0';        
        iCangToImportId=0; // '仓单ID',
        iCangid=0; // '仓单ID',
        iExportid=0; //'出库单ID',
        dbFreezeWaitWeight=0.0;
    }
    string szGoodsnumber ; // '货号',
    string szLot; // '批号',
    string szMade; // '生产年月',
    string szPackage; // '包装方式',
    string szFactory; // '生产厂商',
    
    string szWarehousetransactor; // '仓库经办人',
    string szWarehousemanager; // '仓库负责人',
    string szClienttransactor; // '客户经办人',

    string ssDeliverynumber;//提货码-
    string ssDeliveryMen;//提货人名称
    string ssIdCode;//提货人身份证
    string ssTelphone;//提货人电话号码

    string szSymbolcode ; // '商品代码',
    string szImportdate; // '入库时间',
    string ssExportdate;//出库时间
    //    char szApplydate[S_DATE_TIMER_LEN +1];	//'申请日期',
    //    char chImportstatus; // '状态;{"from":"json","0“,"录入","1":"撤销","2":"等待市场审核“,"3":"市场驳回“,"4":"等待仓库审核“,"5":"仓库驳回“,"6":"完成"}',

    string szRejectreason ; // '拒绝原因',
    string szCheckresult; // '检验结果',
    
    string ssApplyDate;//申请时间
    
    string ssCangDate;//仓单时间
    
    long long iCangToImportId; // '仓单ID',
    long long iCangid; // '仓单ID',
    long long iExportid; //'出库单ID',
    int iImportid; // '入库单ID',
    int iWarehouseid; // '交收仓库',
    int iAccountgroupid; // '机构',
    int iAccountid; // '账户',
    long long iApplyid ; // '入库申请单ID',
    int iSymbolgroupid; // '品种名称',
    
    double dbApplyweight; // '申请重量',
    double dbImportweight; // '入库重量',
    double dbFreezeexportweight;
    double dbExportweight;
    double dbFreezecangweight;
    double dbCangweight;
    double dbUnitweight; // '每件重量',
    int iQuantity; // '件数',
    
    char chApplystatus;		//'状态;{"from":"json","0","录入","1":"撤销","2":"等待市场审核","3":"市场驳回","4":"等待仓库审核","5":"仓库驳回","6":"完成"}',
    char chImportstatus;					// '状态;{"from":"json","0","等待入库","1":"已入库"}',
    
    char chExportStatus;
    
    
    char chCangType;//仓单类型;{"form":"json","1":"标准仓单","2":"非标准仓单","3":"临时仓单"}
    
    char chCangStatus;//
    
    double dbFreezeImportWeight;
    double dbFreezeCloseWeight;
    
    double dbFreezeWaitWeight;
    double dbCloseWeight;
    // 商品属性
    int iAttributeNum;
    COUTSymbolAttributeValue arrAttributArrValue[S_SYMBOL_ATTRIBUTE_MAX_LEN];
};

#endif