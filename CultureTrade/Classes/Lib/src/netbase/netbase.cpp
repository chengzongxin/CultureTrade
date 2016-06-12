// netbase.cpp : 定义 DLL 应用程序的导出函数。
//

#include "netbase.h"
#include "NetMgr.h"



CNetMgr sNetMgr;
//====================================
//1 初始化全局信息
//Init();
//NETBASE_API int Init()
int Init()
{
	return sNetMgr.Init();
}
//2 释放全局信息
//UnInit();
NETBASE_API int UnInit()
{
	return sNetMgr.UnInit();
}

//1.取得一个会话
//CreateSession
//返回会话的唯一标识
NETBASE_API int CreateSession()
{
	return sNetMgr.CreateSession();
}
NETBASE_API void SetSessionSocketPackType(int nSID, int type)
{
    return sNetMgr.SetSessionSocketPackType(nSID, type);
}

//2.设置会话属性
//SetSessionOption
/*****************************************************************
 nCmd
 取值为
 
 *****************************************************************/
NETBASE_API bool SetSessionOption(int nSID, int nCmd, int nValue)
{
	return sNetMgr.SetSessionOption(nSID, nCmd, nValue);
}

//NETBASE_API bool GetSessionOption(int nSID, int nCmd, int& nValue)
NETBASE_API bool GetSessionOption(int nSID, int nCmd, int* nValue)
{
	int value;
	bool ret;
	ret = sNetMgr.GetSessionOption(nSID, nCmd, value);
	if(nValue)
		*nValue = value;
	return ret;
}

//3.增加服务信息
//AddHost
//NETBASE_API bool AddHost(int nSID, int nLayer, LPServerPropertyDataDll pSpd)
NETBASE_API bool AddHost(int nSID, int nLayer, char* str_ip,int port)
{
	//ServerPropertyData spd(*pSpd);
	ServerPropertyData spd(str_ip,port);
	return sNetMgr.AddHost(nSID, nLayer, spd);
}

NETBASE_API bool ClearHost(int nSID, int nLayer)
{
	return sNetMgr.ClearHost(nSID, nLayer);
}


//3.1 服务总数
//GetHostCount
//NETBASE_API bool GetHostCount(int nSID, int nLayer, int& nCount)
NETBASE_API bool GetHostCount(int nSID, int nLayer, int* nCount)
{
	int count;
	bool ret;
	ret = sNetMgr.GetHostCount(nSID, nLayer, count);
	if(nCount)
		*nCount = count;
	return ret;
}

//3.2 取服务的测试数据
//GetHostCount
NETBASE_API bool GetServerPropertyData(int nSID, int nLayer, int nIndex, LPServerPropertyDataDll pSpd)
{
	ServerPropertyData spd;
	bool bRet = sNetMgr.GetServerPropertyData(nSID, nLayer, nIndex, spd);
	*pSpd = spd.ToServerPropertyDataDll();
	return bRet;
}

//=========================================
//4.连接服务
//StartSession
NETBASE_API void StartSession(int nSID)
{
	return sNetMgr.StartSession(nSID);
}

//5.断开会话连接
//StopSession
NETBASE_API void StopSession(int nSID)
{
	return sNetMgr.StopSession(nSID);
}

//12.释放会话信息
//ReleaseSession
NETBASE_API bool ReleaseSession(int nSID)
{
	return sNetMgr.ReleaseSession(nSID);
}
//==================================================
//6.登录
//Login
NETBASE_API bool Login(int nSID, LPCSTR szLoginId, int nLoginIdType, int nClientVersion, LPCSTR szPwd)
{
	//Login(int nSID, const CString& )
	return sNetMgr.Login(nSID, szLoginId, nLoginIdType, nClientVersion, szPwd);
}


NETBASE_API bool SetQuoteFlag(int nSID, int iFlag)
{
	return sNetMgr.SetQuoteFlag(nSID, iFlag);
}

//7.发送数据
//Send
NETBASE_API bool Send(int nSID, int nCmd, int nSeq, LPBYTE pData, int nLen)
{
	return sNetMgr.Send(nSID, nCmd, nSeq, pData, nLen);
}

//14.退出
//Logout
NETBASE_API bool Logout(int nSID, uint32_t nUid)
{
	return sNetMgr.Logout(nSID, nUid);
}
//13.获取远程服务IP
//GetPeerHost
NETBASE_API bool GetPeerHost(int nSID, LPHostPropertyDll pHP)
{
	HostProperty hp;
	bool bRet = sNetMgr.GetPeerHost(nSID, hp);
	*pHP = hp.ToHostPropertyDll();
	return bRet;
}

//14.取得会话的连接状态
//GetSessionStatus
//NETBASE_API bool GetSessionStatus(int nSID, int& nStatus)
NETBASE_API bool GetSessionStatus(int nSID, int* nStatus)
{
	int status;
	bool ret;
	ret = sNetMgr.GetSessionStatus(nSID, status);
	if(nStatus)
		*nStatus = status;
    return ret;
}

//18.一个业务包事件
//OnRead
//OnRead(int nSID, int nCmd, int nSeq, const CString& strData);
//typedef bool (*PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
void BindFirePackage(int nSID, PFFirePackage pfnFirePackage)
{
	return sNetMgr.BindFirePackage(nSID, pfnFirePackage);
}


void BindQuotePackage(int nSID, PFFirePackage pfnQuotePackage)
{
	return sNetMgr.BindQuotePackage(nSID, pfnQuotePackage);
}


//19.会话关闭
void BindFireSessionStatus(int nSID, PFFireSessionStatus pfnFireSessionStatus)
{
	return sNetMgr.BindFireSessionStatus(nSID, pfnFireSessionStatus);
}

void BindNotifyEvent(int nSID, PFNotifyEvent pfnNotifyEvent)
{
	return sNetMgr.BindNotifyEvent(nSID, pfnNotifyEvent);
}
int ProcessEvent(int nSID)
{//return 0,suc;  1,no data to process, -1,no such session id
	return sNetMgr.ProcessEvent(nSID);
}

int NotifyNetState(int nSID,int net_state)
{
	return sNetMgr.NotifyNetState(nSID,net_state);
}
// end of file
