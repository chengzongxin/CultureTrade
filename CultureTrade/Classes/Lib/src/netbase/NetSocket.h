#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

//#pragma once

#include "SafeQueue.h"

#include "config.h"
//#include <winsock2.h>
#include <string>
//#pragma comment(lib,"ws2_32.lib")
#include "Mapping.h"
#include "Buffer.h"
#include "Mutex.h"
#include "nt_NetBlockMsg.h"
//#include "httpProxySocket.h"
//#include "winSocketEx.h"
#include "ComStruct.h"
#include <vector>
#include <map>
#include "nt_crypt.h"
#include "ssl.h"
using namespace std;

// 接收缓冲区
#define MAX_PACKAGE_LEN 64*1024
#define ORIGINALPWD "zhangandzeng"
#define quote_queue_size  30
#define KREQKDATACOUNT  100  //请求K线个数
typedef struct tagReConnectProperty

{
	bool bAllowAutoReConnect ;
	long lReConnectCount;
	tagReConnectProperty()
	{
		clear();
	}
	void clear()
	{
		bAllowAutoReConnect = true;
		lReConnectCount = 0;
	}
    
}ReConnectProperty,*LPReConnectProperty;

//==============================================================
typedef struct tagProxyProperty
{
	string m_strIp;
	short m_nPort;
	string m_strName;
	string m_strPwd;
	long m_lProxyMode;
    
	tagProxyProperty()
	{
		clear();
	}
	void clear()
	{
		m_lProxyMode = 0;
		m_strIp = "";
		m_nPort = 0;
		m_strName = "";
		m_strPwd = "";
	}
    
	
	void FromProxyPropertyDll(ProxyPropertyDll ppd)
	{
		m_lProxyMode = ppd.nProxyMode ;
		m_strIp = ppd.szIp;
		m_nPort = ppd.nPort;
		m_strName = ppd.szName;
		m_strPwd = ppd.szPwd;
	}
    
	ProxyPropertyDll ToProxyPropertyDll()
	{
		ProxyPropertyDll ppd;
		ppd.nProxyMode = m_lProxyMode;
		ppd.nPort = m_nPort;
        
		strncpy(ppd.szIp, m_strIp.c_str(), m_strIp.length() > 16 ? 16 : m_strIp.length());
		strncpy(ppd.szName, m_strName.c_str(), m_strName.length() > 20 ? 20 : m_strName.length());
		strncpy(ppd.szPwd, m_strPwd.c_str(), m_strPwd.length() > 20 ? 20 : m_strPwd.length());
		return ppd;
	}
}ProxyProperty,*LPProxyProperty;

typedef struct tagHostProperty
{
	string strIp ;
	int nPort ;
	long lSpeed ;
	long lCurSum ;
	long lSum ;
	bool bIsNeedAnswer;
	bool IsValid;
public:
	tagHostProperty(char* str_ip,int port)
	{
		strIp = string(str_ip);
		nPort = port;
	}
	void clear()
	{
		strIp = "0.0.0.0" ;
		nPort = 0 ;
		lSpeed = LONG_MAX;
		lCurSum = 0;
		lSum = 1;
		bIsNeedAnswer = true ;
		IsValid = false ;
	}
	tagHostProperty()
	{
		clear();
	}
	
	void FromHostProperty(const HostPropertyDll & hpd)
	{
		strIp = hpd.szIp;
		nPort = hpd.nPort;
		lSpeed = hpd.nSpeed;
		lCurSum = hpd.lCurSum;
		lSum = hpd.lSum;
		bIsNeedAnswer = hpd.bIsNeedAnswer;
		IsValid = hpd.IsValid;
	}
    
	HostPropertyDll ToHostPropertyDll()
	{
		HostPropertyDll hpd;
		strncpy(hpd.szIp, strIp.c_str(), strIp.length() > 16 ? 16 : strIp.length());
		hpd.nPort = nPort;
		hpd.nSpeed = lSpeed;
		hpd.lCurSum = lCurSum;
		hpd.lSum = lSum;
		hpd.bIsNeedAnswer = bIsNeedAnswer;
		hpd.IsValid = IsValid;
		return hpd;
	}
}HostProperty,*LPHostProperty;

typedef struct tagServerPropertyData
{
	HostProperty hostProperty;
	ProxyProperty proxyProperty;
	long lIndex;
    
	tagServerPropertyData()
	{
		//clear();
		lIndex = -1;
	}
	tagServerPropertyData(char* str_ip,int port)
	{
		hostProperty = tagHostProperty(str_ip,port);
	}
	tagServerPropertyData(const ServerPropertyDataDll& spdd)
	{
		lIndex = spdd.lIndex;
		hostProperty.FromHostProperty(spdd.hostProperty);
		proxyProperty.FromProxyPropertyDll(spdd.proxyProperty);
	}
    
	ServerPropertyDataDll ToServerPropertyDataDll()
	{
		ServerPropertyDataDll spdd;
		spdd.hostProperty = hostProperty.ToHostPropertyDll();
		spdd.proxyProperty = proxyProperty.ToProxyPropertyDll();
		spdd.lIndex = lIndex;
		return spdd;
	}
}ServerPropertyData,*LPServerPropertyData;

typedef vector<HostProperty> ArrayHostProperty;
typedef vector<ServerPropertyData> ArrayServerPropertyData;

typedef struct tagKeyContent
{
	tagKeyContent()
	{
		memset(sKey, 0, CRYPT_KEY_LEN);
	}
	uint8_t sKey[CRYPT_KEY_LEN];
	uint32_t nUid;
    
	void SetKey(uint8_t* pKey)
	{
		memcpy(sKey, pKey, CRYPT_KEY_LEN);
	}
}KeyContent;


class CNetSocket:public CNetworkMessageMap
{
public:
	CNetSocket(void);
	virtual ~CNetSocket(void);
    
	BEGIN_NETWORK_MSG_MAP(CNetSocket)
    NETWORK_MESSAGE_HANDLER(FD_READ,  OnRead)
    NETWORK_MESSAGE_HANDLER(FD_CLOSE, OnClose)
    NETWORK_MESSAGE_HANDLER(FD_CONNECT, OnConnect)
	END_NETWORK_MSG_MAP()
    
	net_msg virtual bool OnRead();
	net_msg virtual bool OnClose();
	net_msg virtual bool OnConnect();
    
protected:
	//GET
	ConnectStatusType		m_connectStatusType;
    
	//SET
	//typedef bool (*PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
	PFFireSessionStatus m_pfFireSessionStatus;
	PFFirePackage       m_pfFireBlockMessage;
	PFFirePackage       m_pfQuoteBlockMessage;
	PFNotifyEvent m_pfNotifyEvent;
	
	ReConnectProperty		m_rcp; //连接方式信息
    
	long					m_curSocketReTryCnt;
	//SET GET
	ArrayServerPropertyData m_aryPriHosts;
	ArrayServerPropertyData m_aryBakHosts;
    
	//SET 服务与代理信息
	ServerPropertyData      m_spd;//服务数据
	bool m_bAllProxy;
	
	bool WaitForNextConnect(CNetSocket* pThis);
public:
	uint8_t m_sKey[CRYPT_KEY_LEN];
	uint32_t m_dwUid;
	uint8_t m_sCltKey[CRYPT_KEY_LEN]; // 客户端密钥,与服务端共享用来解密行情
    
    
	ConnectStatusType GetConnectStatusType() const;
	void SetConnectStatusType(int type);
    void GetServerHost(string& strIp, int& nPort);
    
    void SetPackType(int type);
    
	void SetReConnectProperty(ReConnectProperty& rcp);
	void GetReConnectProperty(ReConnectProperty& rcp);
    
	
	void SetWaitReConnect(const int& nWaitReConnect);
	void SetJudgeDisConnect(const int& nJudgeDisConnect);
	void GetWaitReConnect(int& nWaitReConnect);
	void GetJudgeDisConnect(int& nJudgeDisConnect);
    
	void SetAllProxy(const ProxyProperty& pp);
	void ClearAllProxy();
    
	bool HasHost(const ArrayServerPropertyData& aspd, const string& strIp, const long& nPort);
    
	bool AddPriServerPropertyData(ServerPropertyData& spd);
	bool AddBakServerPropertyData(ServerPropertyData& spd);
    
	bool ClearPriServerPropertyData();
	bool ClearBakServerPropertyData();
    
	int GetServerPropertyCount() const;
	int GetPriServerPropertyCount() const;
	int GetBakServerPropertyCount() const;
    
	ServerPropertyData GetPriServerProperty(int nIndex);
	ServerPropertyData GetBakServerProperty(int nIndex);
    
	int GetNextSeq(void);
	static bool SortFun(const LPPackageData p1, const LPPackageData p2);
	bool Connect();
	void ClearSocket();
	int main_loop(int timeout);
	bool is_should_exit(void);
	static int process_event(void*para);
	pthread_cond_t m_pthread_cond;
	pthread_mutex_t m_pthread_mutex;
	int m_is_start;
	int m_net_state;
    int                     m_nPackType;
    int                     m_nQuoteFlag;
    int                     m_nQuoteLoginFlag;
    
private:
    //这里不是一个好的设计思路 但这样做是在SOCKET层面定义的消息不太符合框架导致的，收到的包没有回业务上的商品ID！服务器发完这消息后，收到的任何消息都要将此删除。
    char*                   m_pstrSymbolID;
    short                   m_cKLinePeriodicType;
	uint32_t                m_nWaitReConnect;
	uint32_t                m_nJudgeDisConnect;
	WSAEVENT				m_hEvent;
	SOCKET					m_hSocket;
    SSL*                    m_hSSL;
	CBuffer					m_sendBuff;//发送BUFF
	CBuffer					m_recvBuff;//接收BUFF
    
	bool					m_bShouldExit;//是否退出
	HANDLE					m_hThread;
	HANDLE					m_hFireThread;
	HANDLE					m_hHeartBeatThread;
	UINT					m_dwThreadId;
	UINT					m_dwFireThreadId;
	UINT					m_dwHeartBeatThreadId;
	int                     m_nSid;
	int						m_nSeq;
	SafeQueue<LPPackageData> m_sqPackage;
    SafeQueue<LPQuotePackageData> m_sqQuotePackage;
	Mutex					m_MutexSendBuff;
    
    int                     m_nRecvKUnit;//是否接受过数据
    int                     m_nLastReqCode;//请求码
    
	typedef struct {
		DWORD dwTickCount; // 第一个包收到的时间,用来对十分钟超时的map进行清空
		vector<LPPackageData> vecPkg;
	} CutPkg;
	map<int, CutPkg> mapCutPkg;
	
	uint32_t m_send_heartbeat_time;
    uint32_t m_send_quote_login_time;
protected:
    
	void FireSessionStatus();
	bool SendData();
	bool SeletHost();
    
	int loop_read(void);
	int loop_write(void);
	int loop_heartbeat(void);
	int notify_event(void);
	//找到有效的包开头位置
	bool RemovDataUntilValidHeadFlag(CBuffer& recvBuff);
	
	typedef struct tagBuPkgBuffer
	{
		uint16_t wCmd;
		uint32_t dwSeq;
		uint16_t wTotal;
		uint16_t wCurSeq;
		PBYTE pData;
		uint32_t dwLen;
        
		uint16_t wCurReceiveNum;
	}BuPkgBuffer;
    
	typedef map<pair<uint16_t, uint32_t>, BuPkgBuffer> MAP_BuPkgBuffer;
    
	MAP_BuPkgBuffer m_mapBuPkgBuffer;
    
	int PushBizPkg(LPPackageData & lpPs);
	bool GetBizPkg();
    
	static unsigned int __stdcall SocketThreadProc(void * lParam);
	static unsigned int __stdcall FireDataThreadProc(void * lParam);
	static unsigned int __stdcall HeartBeatThreadProc(void * lParam);
    
	static unsigned int __stdcall CheckServerSpeedThread(void * lpVoid);
	static unsigned int __stdcall myGetServerIPListBySpeed(void * lpVoid);
public:
	static bool InitNet();
	static int ClearNet();
    
	void Stop();
	void Start();
	bool Send(PBYTE pData, int nLen);
	bool Send(int nCmd, int nSeq, PBYTE pData, int nLen);
    
    void BindFirePackage(int nSid, PFFirePackage pfFireBlockMessage);
	void BindQuotePackage(int nSID, PFFirePackage pfnQuotePackage);
    void BindFireSessionStatus(int nSid, PFFireSessionStatus pfFireSessionStatus);
	void BindNotifyEvent(int nSid, PFNotifyEvent pfNotifyEvent);
	int ProcessEvent(int nSid);
	int NotifyNetState(int net_state);
	////////////////////////////////////////////////////////////////////////////////////////
	static bool SetSocketOption(SOCKET socket);
	static bool ConnectByProxy(ServerPropertyData spd, SOCKET& socket,void * pthis);
	static long GetServerIPBySpeed(ArrayServerPropertyData& vtHi, bool bCheckForOne = false, long lTimeOut = 10000);
	static bool TranslateRemoteAddress(const char * lpszHost, int nPort, SOCKADDR_IN &sockAddr);
    
    void GenerateCheckSum32(char pOutBuf[4], const char* pInBuf, int iLen);
    int  SendQuotePack(const char* pBodyBuf, unsigned iBodyLen);
    int  SendBalancePack(const char* pBodyBuf, unsigned iBodyLen);
    
    int ProcessTradeData(void * lParam);
    int ProcessQuoteData(void * lParam);
    int ProcessBalaceData(void * lParam);
    int SZCodeToZSMarket(unsigned int uiCode);
    void UnicodeToUTF_8(char* pOut,wchar_t* pText);
};


#endif // __NET_SOCKET_H__

// end of file
