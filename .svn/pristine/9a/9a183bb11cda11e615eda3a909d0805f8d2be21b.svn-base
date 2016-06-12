#ifndef __NET_MGR_H__
#define __NET_MGR_H__

//#pragma once
//#include <unordered_map>
#ifdef __ANDROID__
#include <hash_map>
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

#include "NetSocket.h"
#include "Mutex.h"
#include "nt_crypt.h"

//using namespace stdext;
typedef struct tagSessionContext
{
	long nSid;
	Mutex* pMutexSession;
	CNetSocket* pNetSocket;
}SessionContext, *LPSessionContext;

typedef hash_map<long, LPSessionContext> THashMapSessionContext;
typedef hash_map<long, LPSessionContext>::iterator IterHashMapSessionContext;

class CNetMgr
{
public:
	CNetMgr(void);
	virtual ~CNetMgr(void);
protected:

	/*
     创建一个会话上下文

     返回值为会话的唯一标识

     */
	long CreateSessionContext(LPSessionContext& pSc);

	bool ReleaseSessionContext(long nSid);


	bool InsertMap(LPSessionContext pSc);

	inline bool GetSessionContext(long nSid, SessionContext& sc);

public:
	/*
     初始化内部必要的数据
     */
	int Init();
	int UnInit();

	//1.取得一个会话
	//CreateSession
	//返回会话的唯一标识
	int CreateSession();

    //设置socket包类型 0交易包结构 1行情包结构
    void SetSessionSocketPackType(int nSID, int type);

	//2.设置会话属性
	//SetSessionOption
	/*****************************************************************
     nCmd
     取值为

     *****************************************************************/
	bool SetSessionOption(int nSID, int nCmd, int nValue);

	bool GetSessionOption(int nSID, int nCmd, int& nValue);

	//3.增加服务信息
	//AddHost
	bool AddHost(int nSID, int nLayer, ServerPropertyData& pSpd);

	//3.1 服务总数
	//GetHostCount
	bool GetHostCount(int nSID, int nLayer, int& nCount);

	bool ClearHost(int nSID, int nLayer);

	//3.2 取服务的测试数据
	//GetHostCount
	bool GetServerPropertyData(int nSID, int nLayer, int nIndex, ServerPropertyData& pSpd);

	//=========================================
	//4.连接服务
	//StartSession
	void StartSession(int nSID);

	//5.断开会话连接
	//StopSession
	void StopSession(int nSID);

	//12.释放会话信息
	//ReleaseSession
	bool ReleaseSession(int nSID);
	//==================================================
	//6.登录
	//Login
	bool Login(int nSID, const string& strLoginId, int nLoginIdType, int nClientVersion, LPCSTR strPwd);

	// 设置行情推送标志
	bool SetQuoteFlag(int nSID, int iFlag);

	//7.发送数据
	//Send
	bool Send(int nSID, int nCmd, int nSeq, LPBYTE pData, int nLen);

	//8.退出
	//Logout
	bool Logout(int nSID, uint32_t nLen);

	//13.获取远程服务IP
	//GetPeerHost
	bool GetPeerHost(int nSID, HostProperty& pHP);

	//14.取得会话的连接状态
	//GetSessionStatus
	bool GetSessionStatus(int nSID, int& nStatus);
	////////////////////////////////////////////////////////////////////////////////
	//外部事件
	//9.一个业务包事件
	//OnRead
	//OnRead(int nSID, int nCmd, int nSeq, const CString& strData);
	//typedef bool (*PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
	void BindFirePackage(int nSID, PFFirePackage pfnFirePackage);

	// 行情包事件
	void BindQuotePackage(int nSID, PFFirePackage pfnQuotePackage);

	//10.会话连接成功
	//OnSessionStatus
	//OnSessionStatus();
	//typedef bool BindFireSessionStatus(int nFlag);
	void BindFireSessionStatus(int nSID, PFFireSessionStatus pfnFireSessionStatus);
	void BindNotifyEvent(int nSID, PFNotifyEvent pfnNotifyEvent);
	int ProcessEvent(int nSID);
	int NotifyNetState(int nSID, int net_state);
protected:

	Mutex m_MutexMap;
	THashMapSessionContext m_mapSessionContext;
	long m_nSid;

};


#endif // __NET_MGR_H__

// end of file
