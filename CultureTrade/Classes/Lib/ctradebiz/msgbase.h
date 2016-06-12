#ifndef MSGBASE_20120227_HT
#define MSGBASE_20120227_HT


//最小数量
const double DB_MIN_ORDER_SIZE =  0.00001;
const double DB_MIN_PRICE_SIZE =  0.000001;
const unsigned int AMOUNT_ROUND = 4;			//金额精度

//下单标志
enum TRADE_FLAG
{
	TRADE_FLAG_NEW='1',				//‘1’-  新建单
	TRADE_FLAG_MODIFY_LIMIT,		// '2' - 修改限价单
	TRADE_FLAG_MODIFY_SL_TP,		// '3' -（修改止损，获利价）
	TRADE_FLAG_CANCEL,  			//‘4’ - 撤销单
	TRADE_FLAG_LIMIG_STOP_BARGIN,	// '5' -- 撮合成交
	TRADE_FLAG_TP_BARGIN,			// '6' -- 获利成交
	TRADE_FLAG_SL_BARGIN,			// '7' -- 止损成交
	TRADE_FLAG_SO,					// '8' -- 系统斩仓指令
	TRADE_FLAG_GROUP				// '9' -- 批量新单
};

//订单类型
enum ORDER_TYPE
{
	ORDER_TYPE_MARKET ='1',		//‘1’ - 市价单
	ORDER_TYPE_LIMIT='2'			// '2’ - 限价单;
};

//手续计提方式
enum COMMISION_TYPE
{
	COMMISION_TYPE_FIXED = '1',	//'1' -- 固定
	COMMISION_TYPE_RATIO = '2'	//'2' -- 比率
};

//单据获利方向
enum ORDER_LIMIT_STOP_TP_SL
{
	ORDER_LIMIT = '1',
	ORDER_STOP = '2',
	ORDER_TP = '3',
	ORDER_SL = '4'
};

//保证金模式
enum MARGIN_MODE
{
	MARGIN_TYPE_FIXED = '1',		//‘1’ -- 固定
	MARGIN_TYPE_RATIO ='2'		//'2' -- 比率
};



//子订单类型
enum SUB_ORDER_TYPE
{
	SUB_ORDER_TYPE_BUY ='1',				//'1' -buy
	SUB_ORDER_TYPE_SELL,					//'2' - sell
	SUB_ORDER_TYPE_BUY_LIMIT,				//'3' - buy limit
	SUB_ORDER_TYPE_BUY_STOP,				//'4' - buy stop
	SUB_ORDER_TYPE_SELL_LIMIT,				//'5' - sell limit
	SUB_ORDER_TYPE_SELL_STOP				//'6' - sell stop
};

//归档类型
enum FILL_POLLCY_TYPE
{
	FILL_POLLCY_TYPE_ALL_OR_NONE = '1',		// ‘1’ – all or none
	FILL_POLLCY_TYPE_CANCEL_REMAIN,			// ‘2’ – cancel remain
	FILL_POLLCY_TYPE_RETURN_REMAIN			// ‘3’ – return remain
};

//效力类型
enum VALIDATE_TYPE
{
	VALIDATE_TYPE_GTC = '1',	 //'1' - GTC (截止日期、时间)
	VALIDATE_TYPE_DAY = '2',			 //'2' - day 当日有效
	VALIDATE_TYPE_FOREVER='3'		 //'3' - 永久有效
};

//买卖方向
enum DIRECTION_TYPE
{
	DIRECTION_TYPE_BUY ='b',    //  ‘b’- buy
	DIRECTION_TYPE_SELL ='s'	//  ‘s’ -selll
};

//买单开仓 权限
enum BUY_OPEN_RIGHT
{
	BUY_OPEN_RIGHT_YES = '1',	//'1'-是
	BUY_OPEN_RIGHT_NO = '2'		//'2'-否
};



//下单操作类型
enum ORDER_OPERATOR_TYPE
{
	ORDER_OPERATOR_TYPE_NORMAL = '1',			//'1'-普通下单
	ORDER_OPERATOR_TYPE_RIGHT = '2',			//'2'-代客下单(有给价权限)
	ORDER_OPERATOR_TYPE_NOT_RIGHT = '3',		//'3'-代客下单(无给价权限)
	ORDER_OPERATOR_TYPE_OPERATE = '4',			//'4'-操作员下单
	ORDER_OPERATOR_TYPE_RISK_CLOSE = '5',		//'5'-系统斩仓
	ORDER_OPERATOR_TYPE_MATCH_LIMIT_STOP = '6',	//'6'-限价停损成交
	ORDER_OPERATOR_TYPE_MATCH_TP = '7',			//'7'-获利平仓
	ORDER_OPERATOR_TYPE_MATCH_SL = '8',			//'8'-止损平仓
	ORDER_OPERATOR_TYPE_RECKON_CLOSE = '9', 	//'9'-结算斩仓
	ORDER_OPERATOR_TYPE_RECKON_LOCK = 0x3130,//'10',		//'10'-结算锁仓
	ORDER_OPERATOR_TYPE_RECKON_CANCEL = 0x3131//'11'	//'11'-结算撤单
    
};

//资金操作流水类型
//enum FUNDS_OPERATE_TYPE
//{
//	FUNDS_OPERATE_TYPE_IN = '1',					//'1'-入金
//	FUNDS_OPERATE_TYPE_OUT = '2',					//'2'-出金
//	FUNDS_OPERATE_TYPE_PL = '3',					//'3'-盈亏
//	FUNDS_OPERATE_TYPE_SWAP = '4',					//'4'-递延费
//	FUNDS_OPERATE_TYPE_COMMISION = '5',				//'5'-佣金
//	FUNDS_OPERATE_TYPE_CREDIT = '6',				//'6'-调整信用金
//	FUNDS_OPERATE_TYPE_CUT_BACK = '7',				//'7'-强平冲证
//	FUNDS_OPERATE_TYPE_RECKON_PL = '8',				//'8'-无负债结算盈亏
//	FUNDS_OPERATE_TYPE_REVERSED_PROCESSING = '9',	//'9'-冲正处理
//    
//};



//开收市状态
enum SYMBOL_OPEN_CLOSE_STATUS
{
	SYMBOL_OPEN_CLOSE_STATUS_OPEN = '1',		//开市状态
	SYMBOL_OPEN_CLOSE_STATUS_CLOSE = '2'		//收市状态
};

//订单交易权限
enum SYMBOL_TRADE_RIGHT
{
	SYMBOL_RIGHT_STOP = '1',			//禁用
	SYMBOL_RIGHT_ONLY_BUY = '2',		//仅买入
	SYMBOL_RIGHT_ONLY_SELL = '3',		//仅卖出
	SYMBOL_RIGHT_ONLY_CLOSE = '4',		//仅平仓
	SYMBOL_RIGHT_ALL = '5'				//全部交易
};


//保证金类型
enum MARGIN_TYPE
{
	MARGIN_TYPE_DAY_TRADE = '1',		//即市（占用）保证金
	MARGIN_TYPE_OVER_NIGHT = '2',		//过市（过夜）保证金
	MARGIN_TYPE_FREEZE = '3',			//即市冻结预付款
	MARGIN_TYPE_LOCK = '4'			   //锁仓预付款
};


//价格状态
enum PRICE_STATUS
{
	PRICE_STATUS_REAL = '1',		//实时价
	PRICE_STATUS_HISTORY = '2',		//历史价
	PRICE_STATUS_CLEAR = '3'		//清盘价
};


//商品报价模式
enum SYMBOL_PRICE_MODE
{
	SYMBOL_DIRECT_QUOTATION = '1',		//直接报价
	SYMBOL_INDIRECT_QUOTATION = '2',	//间接报价
	SYMBOL_CROSS_RATES = '3'			//交叉汇率
};

//风险率计算公式
enum RISK_CALC_TYPE
{
	RISK_CALC_TYPE_ONE ='1',		//风险率计算方式 1-占用/净值
	RISK_CALC_TYPE_TWO ='2',		//风险率计算方式 2-净值/占用',
};


//交易是否支持单处理模式
enum TRADE_ORDER_MODE
{
	TRADE_SUPPORT_MODE_LOCK_ORDER = '1',		 //锁单模式（双向单，锁保证金）
	TRADE_ORDER_MODE_DOUBLE = '2',				//双向单
	TRADE_ORDER_MODE_HEDGE = '3',				//对冲模式(单向单)
};

//平仓顺序模式
enum CLOSE_SORT_MODE
{
	CLOSE_SORT_MODE_QUEUE = '1',		 //先进先平
	CLOSE_SORT_MODE_STACK = '2',		 //后进先平
	CLOSE_SORT_MODE_PROFIT = '3',		 //盈利优先
	CLOSE_SORT_MODE_LOSS   = '4'		 //亏损优先
};

//负债模式
enum DEBT_CLEAR_MODE
{
	DEBT_CLEAR_MODE_HAVE ='1',		//有负债
	DEBT_CLEAR_MODE_NO ='2'			//无负债
};

//序列号类型
enum SEQUENCE_NUMBER
{
	SN_ORDERID = 1,					//序列号 -- 普通定单
	SN_SESSIONID = 2				//序列号 -- 会话SESSIONID
};


//有效无效标志
enum ISVALID_FLAG
{
	VAILD_FLAG = '1',			//有效标志
	INVALID_FLAG = '0'				//无效标志
};


//enum CANCELTYPEY
//{
//    SELLTEMENT_AUTO_CANCEL_NO = '0',
//    SELLTEMENT_AUTO_CANCEL_YES = '1'
//};

#endif
