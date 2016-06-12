#ifndef __NT_BU_TYPEDEF_H__
#define __NT_BU_TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif


#pragma once

#include "nt_types.h"


//从底层向上层依次分范围

//1.测试层
//   1.1  程序测试包
//2.程序服务层
//   2.1  程序监控报告类
//   2.2  程序监控控制类
//3.网络层
//   3.1  网络控制类
//   3.2  网络信号类
//4.业务网络层
//   4.1  业务控制类
//5.业务处理层
//   5.1  认证类
//        5.1.1  登录认证类
//   5.2  行情类
//        5.2.1  行情数据类
//        5.2.2  行情控制类
//   5.3  业务类
//        5.3.1  一般查询业务类
//        5.3.2  业务监控查询类
//        5.3.3  主动交易类
//        5.3.4  限价交易类
//        5.5.5  代理交易类
//        5.5.6  业务管理类
//        5.5.7  业务广播类
//   5.4  历史类 
//        5.4.1  行情历史数据类
//        5.4.2  交易历史数据类
//   5.5  新闻类
//        5.5.1  新闻相关类

//===========================================================
/*
	得到的定义为 
	enum {
		FLAG_START_name = start,
		FLAG_name_LEN   = end - start + 1,
		FLAG_END_name   = end,
	};

	const uint16_t FLAG_START_name = start;
	const uint16_t FLAG_name_LEN   = end - start + 1;
	const uint16_t FLAG_END_name   = end;
*/
#define DEF_FLAG_START_END_FOR_NAME(name, start, end) \
	enum { \
		FLAG_START_##name = ((uint16_t)start), \
		FLAG_END_##name   = ((uint16_t)end), \
		FLAG_LEN_##name   = (((uint16_t)end) - ((uint16_t)start) + 1), \
	}; \
/*
	const uint16_t FLAG_START_##name = ((uint16_t)start); \
	const uint16_t FLAG_END_##name   = ((uint16_t)end); \
	const uint16_t FLAG_LEN_##name   = (((uint16_t)end) - ((uint16_t)start) + 1);
*/

#define GET_FLAG_START_VALUE_FOR_NAME(name) \
	FLAG_START_##name

//1.测试层          1,1000 : 1000
//   1.1  程序测试包   1,1000
DEF_FLAG_START_END_FOR_NAME(TEST_PKG, 1, 1000)

//============================================
//2.程序服务层      1001,3000 : 2000
//   2.1  程序监控报告类  1001,1500
//   2.2  程序监控控制类  1501,2000
//FLAG_START_APP
//FLAG_END_APP
DEF_FLAG_START_END_FOR_NAME(APP, 1001, 3000)

DEF_FLAG_START_END_FOR_NAME(MONITOR_APP, 1001, 1500)
DEF_FLAG_START_END_FOR_NAME(MONITOR_APP_CTRL, 1501, 2000)

//=============================================
//3.网络层          3001,4000 : 1000
DEF_FLAG_START_END_FOR_NAME(NET, 3001, 4000)
//   3.1  网络控制类  3001,3100
DEF_FLAG_START_END_FOR_NAME(NET_CTRL, 3001, 3100)
//   3.2  网络信号类  3101,3200
DEF_FLAG_START_END_FOR_NAME(NET_SINGAL, 3101, 3200)
//心跳信号
//const uint16_t NET_SINGAL_HEARTBEAT = 3101;//心跳信号

enum {
	NET_SINGAL_HEARTBEAT = 3101,
};

//=============================================
//4.业务网络层      4001,5000 : 1000
//   4.1  业务控制类 4001,4100
DEF_FLAG_START_END_FOR_NAME(NETBU, 4001, 5000)
DEF_FLAG_START_END_FOR_NAME(NETBU_CTRL, 4001, 4100)

//=============================================
//5.业务处理层       10000,20000 :10000
DEF_FLAG_START_END_FOR_NAME(BU, 10000, 20000)
//   5.1  认证类     10001,10100 : 100
DEF_FLAG_START_END_FOR_NAME(BU_CA, 10001, 10100)
//        5.1.1  登录认证类  10001,10020
DEF_FLAG_START_END_FOR_NAME(BU_CA_LOGIN, 10001, 10020)


//const uint16_t BU_CA_LOGIN_USER = FLAG_START_BU_CA_LOGIN;//10001 登录功能号
enum {
	BU_CA_LOGIN_USER = 10001,
	BU_CA_LOGOUT_USER = 10002,
	BU_CA_LOGIN_NOTIFY = 10003,
	BU_CA_LOGOUT_NOTIFY = 10004,
	BU_CA_KICK_NOTIFY = 10005,
	BU_CA_PASSWD_NOTIFY = 10006,
};
//

//   5.2  行情类     10101,11000 
DEF_FLAG_START_END_FOR_NAME(BU_QUOTE, 10101, 11000)
//        5.2.1  行情数据类 10101,10600
DEF_FLAG_START_END_FOR_NAME(BU_QUOTE_DATA, 10101, 10600)

enum {
	BU_QUOTE_DATA_QUOTE = FLAG_START_BU_QUOTE_DATA,
	BU_QUOTE_DATA_GET_KLINE = BU_QUOTE_DATA_QUOTE + 1,
};

//        5.2.2  行情控制类 10601,10700
DEF_FLAG_START_END_FOR_NAME(BU_QUOTE_CTRL, 10601, 10700)

enum {
	BU_QUOTE_CTRL_QUOTE = FLAG_START_BU_QUOTE_CTRL,
};

//   5.3  业务类     11001,17000
DEF_FLAG_START_END_FOR_NAME(BU_BIZ, 11001, 17000)

//        5.3.1  一般查询业务类 11001,12000
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_QUERY, 11001, 12000)

enum {
	BU_BIZ_QUERY_AUDIT_MSG = 11982,
	BU_BIZ_QUERY_EXPERT_CHAT = 11986,
	BU_BIZ_QUERY_SECRET_CHAT = 11987,
	BU_BIZ_QUERY_NEWS_NOTICE = 11988,
};

//        5.3.2  业务监控查询类 12001,12200
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_MONITOR, 12001, 12200)

//        5.3.3  主动交易类   12201,13200
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_ACK_TRADER, 12201, 13200)

enum {
	BU_BIZ_MARKET_ORDER = 12201,
	BU_BIZ_CANCEL_LIMIT_ORDER = 12202,
};

//        5.3.4  限价交易类  13201,13700
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_LIMIT, 13201, 13700)

//        5.3.5  代理交易类  13701,14200
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_PROXY, 13701, 14200)

//        5.3.6  业务管理类 14201,15000
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_MGR, 14201, 15000)

enum {
	BU_BIZ_MGR_MODIFY_USERS_PSD = 12203,
	BU_BIZ_MGR_MODIFY_MGR_STATUS = 14209,
	BU_BIZ_MGR_RESET_MGR_PSD = 14212,
	BU_BIZ_MGR_MODIFY_MGR_PSD = 14215,
	BU_BIZ_MGR_MODIFY_USER_STATUS = 14218,
	BU_BIZ_MGR_RESET_USER_PSD = 14219,
	BU_BIZ_MGR_MODIFY_USER_GROUP = 14220,
	BU_BIZ_MGR_MODIFY_USER_PSD = 14221,
	BU_BIZ_MGR_SET_GROUP_MGR = 14246,
	BU_BIZ_MGR_SEND_NEWS_NOTICE = 14246,
};

//        5.3.7  业务广播类 15001,15500
DEF_FLAG_START_END_FOR_NAME(BU_BIZ_MSG, 15001, 15500)

enum {
	BU_BIZ_MSG_FORWARD_SECREC_CHAT = 15252,
	BU_BIZ_MSG_FORWARD_EXPERT_CHAT = 15253,
	BU_BIZ_MSG_FORWARD_NEWS_NOTICE = 15254,
	BU_BIZ_MSG_FORWARD_AUDIT_MSG = 15255,
	BU_BIZ_MSG_LIMIT_ORDER_MSG = 15256,
	BU_BIZ_MSG_SYSTEM_CUT_MSG = 15257,
	BU_BIZ_MSG_MARKET_STATUS_MSG = 15259,
};

//   5.4  历史类    17001,19000
DEF_FLAG_START_END_FOR_NAME(BU_HIS, 17001, 19000)

//        5.4.1  行情历史数据类 17001,18000
DEF_FLAG_START_END_FOR_NAME(BU_HIS_DATA, 17001, 18000)

//        5.4.2  交易历史数据类  18001,19000
DEF_FLAG_START_END_FOR_NAME(BU_HIS_TRADER, 18001, 19000)

//   5.5  新闻类    19001,20000
DEF_FLAG_START_END_FOR_NAME(BU_NEWS, 19001, 20000)

//        5.5.1  新闻相关类 19001,19600
DEF_FLAG_START_END_FOR_NAME(BU_NEWS_DATA, 19001, 19600)



//用户类型
typedef enum {
	UT_TRADE_USER, //交易用户
	UT_ADMIN, //管理员
	UT_ANY = 0xFFFFFFFF, // 任何类型的用户
} EUserType;

#ifdef __cplusplus
}
#endif

#endif // __NT_BU_TYPEDEF_H__

// end of file
