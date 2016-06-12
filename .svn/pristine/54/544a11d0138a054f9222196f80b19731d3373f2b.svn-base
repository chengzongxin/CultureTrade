
/* @file            DataLibary.h
 * @author   	姚吉祥
 * @date        	2011-07-14
 * @version  		1.1
 * @brief       	数据“库”
 *
 * @history
 * @note
 *
 **/


#ifndef __SSCC__DATALIBARY_H__
#define __SSCC__DATALIBARY_H__

#include "DataDef.h"
#include <vector>
#include <map>
#include <list>
#include <string>
#include "struct.h"

using namespace std;


//wzm-2012-start
typedef CMap<CString, CString&,CSymbolParam*, CSymbolParam* > SymbolParam;
//wzm-2012-end
class CDataLibary
{
public:
	CDataLibary(){};
	~CDataLibary(){};
public:
	/// 当前用户信息{
	string								m_ssServerIP;
	unsigned short						m_nServerPort;
	//备份服务器
	string								m_ssServerIP1;
	unsigned short						m_nServerPort1;
	string								m_ssUserID;
	string								m_ssUserName;
	string								m_ssLastLoginTime;//上次登录时间
	string								m_ssPassword;
	string								m_ssUserStatus;
	string								m_ssBankID;
	string                              m_ssSession;		///请求用Session
	string								m_ssNeedChangePwd;	//是否需要变更密码
	string								m_ssAccountType;	//账户类型
	string								m_ssAccountID;		//帐户ID
	/// 当前用户信息}
    
    
	vector<CTradeOrder>					m_vecOrders;//委托返回
	vector<COrderTurnOver>				m_vecTurnOver;//成交信息
	vector<CProductInfo>				m_vecProducts;//产品信息
	vector<CSwingOrder>					m_vecSwing;	//摇号返回
	vector<CTransferFormOders>			m_vesTrans;//转账返回
	vector<SProductBalanceResponse>		m_vecShare;		//证券持仓
	SMoneyBalanceResponse				m_moneyBalance;	//资金持仓
	int									m_nLoginResult;									/// 登录结果
	string								m_ssLoginErrMsg;									/// 登录错误码
	CPreTradeData						m_PreTradeData;		///最大可买可卖
    
	//wzm-2012-start
	COrderRes                           m_HoldOrder;//订单返回-持仓单
	CFundsRes                           m_Funds;//资金返回
	vector<CFindOrderRes>				m_vecHoldOrder;//订单查询返回-持仓单
	//vector<CFindOrderRes>				m_vecCloseOrder;//订单查询返回-平仓单
	vector<CFindOrderRes>				m_vecLimitOrder;//订单查询返回-限价单
	vector<CFindOrderRes>				m_vecHistoryOrder;//订单查询返回-历史
	SymbolParam                         m_mapSymbolParam;//商品信息
	vector<CFundsLogRes>				m_vecFundsLog;//资金流水返回
	CLocalSetup                         m_LocalSetup;//本地设置
	Language                            m_mapLanguage;//语言资源
	string								m_ssAlertContent;//提示内容
	COutMoneyInOutApplyRes              m_OutMoneyInOutApply;//出入金申请返回
	vector<CDeclareTitle>               m_vecDeclareTitle; //公告标题
    
	vector<CMoneyInOutList>             m_vecMoneyInOutList;//查询出入金
    
	string                              m_ssOrderErrorInfo;//订单错误信息
	string                              m_ssAllowOutFund;//可出金额
	//wzm-2012-end
};

#endif
