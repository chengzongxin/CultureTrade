#ifndef __COM_STRUCT_H__
#define __COM_STRUCT_H__

#include "nt_types.h"

/*
 #if _MSC_VER > 1000
 #pragma once
 #endif // _MSC_VER
 */

//#pragma warning(disable:4200)

typedef enum enumConnectStatusType {
	CST_CONNECTING		= 0x1,//正在连接
	CST_CONNECTED		= 0x2,//已连接
	CST_UNCONNECTED		= 0x3,//未连接
	CST_RECONNECT		= 0x4,//正在重新连接
	CST_ERROR_CONNECT	= 0x5,//连接失败
	CST_CLIENT_SENDDATA	= 0x6, //表示发送数据时关闭SOCKET
	CST_CLIENT_ENUMNETWORKEVENTS	= 0x7, //表示EnumNetworkEvents出错关闭SOCKET
	CST_CLIENT_JUDGE_CLOSE			= 0x8, //表示客户端在9s内无网络事件，关闭SOCKET
	CST_SOCKET5_ERROR	= 0x9, //  表示使用SOCKET5代理服务登录
	CST_SOCKET4_ERROR	= 0xA //  表示使用SOCKET4代理服务登录
} ConnectStatusType;


typedef enum enumProxyModeType {
	PMT_NOPROXY	= 0, //  表示不使用代理登录(默认为不使用代理登录)
	PMT_HTTP		= 1, //  表示使用HTTP代理服务登录
	PMT_SOCKET5	= 2, //  表示使用SOCKET5代理服务登录
	PMT_SOCKET4	= 3 //  表示使用SOCKET4代理服务登录
} ProxyModeType;


typedef enum enumHostLayer {
	HL_Pri = 1,//1 主
	HL_Bak = 2//2 备
} HostLayer;

typedef enum enumSessionOptionCmd {
	SOC_ReConnect = 1 ,//是否重连 取值 大于 0 表示自动重连 ，否则为不自动重连
	SOC_ReConnectTryCount = 2, //在原有HOST上重试多少次
	SOC_WaitReConnect = 3,//断开重新连接等待多少秒功能
	SOC_JudgeDisConnect = 4//判断网络主要断开功能
} SessionOptionCmd;


//心跳协议, 是明文协议
//请求
typedef struct {
	uint32_t dwTickCount;
} HeartBeatRequest;


//回应
typedef struct {
	uint32_t dwTickCount;
	union {
		uint32_t dwServerTime; //对于客户端发出的请求,中转回应的是当前时间
		uint32_t dwPayLoad; //对于中转发出的请求,业务回应的是负载情况(如果有统计信息,则返回总流量(总包量,bps, pps等),否则返回当前连接数)
	} ServerStatus;
} HeartBeatResponse;


//登录协议,密文协议
//请求
//加密数据的明文实体
typedef struct {
	uint8_t  sLoginId[20];
	uint32_t dwClientTime;
	uint8_t  sMAC[6];
	uint8_t  sReserved[2];
} PlainLoginReq;


typedef struct {
	uint32_t dwClientVersion; //登录版本号
	uint32_t dwLoginIdType; // 帐号类型
	uint8_t  sLoginId[20]; //登录帐号
	uint32_t dwClientTime;
	uint32_t dwIp;
	uint32_t dwPort;
	//下面这部分是加密的密文,密钥是密码的二次MD5
	//PlainLoginReq stPlainLoginReq;
	uint8_t  sCipherData[0]; //sLoginID(20B) + dwClientTime(4B) + MAC(6B)...
} LoginRequest;


typedef struct {
	uint8_t  sCommKey[16];
	uint32_t dwUID;
	uint32_t dwIp;
	uint32_t dwGenerateTime;
	uint32_t dwExpireTime; // 使用的是相对时间,是时长,例如36000(s)(10小时)
} LoginTicket;


//加密数据的明文实体
typedef struct {
	uint32_t dwUID;
	uint8_t  sCommKey[16]; //后续通信的加密密钥
	uint32_t dwServerTime;
	uint8_t  sTicket[sizeof(LoginTicket) + 4]; //此票据以后扩展供其它功能使用,目前暂没有使用,客户端原样保存即可
	uint32_t dwExpireTime; //此字段表明Ticket的过期时间(表示多久后过期(秒为单位)),客户端须在过期前重新请求新的Ticket
	uint32_t  dwLogLevel;
} PlainLoginResp;

//回应
typedef struct {
	int32_t iRetCode; //0 : 登录成功, -1 : 帐号不存在, -2 : 密码错误, -3 : 其它,如果失败则只有dwRetCode
	//UID和CommKey是在登录服务传给转发的时候,由转发进行剪切,对客户端不可见
	//如果RetCode不为0,则没有这两个字段
	uint32_t dwUID;
	uint8_t  sCommKey[16];//000
	
	//下面这部分是加密的,密钥是密码的二次MD5,当登录成功后,后续的包均使用sCommKey进行加密
	//PlainLoginResp stPlainLoginResp;
	uint8_t  sCipherData[0]; //UID(4B) + sCommKey(16B) + ServerTime(4B) + Ticket(36B) + ExpireTime(4B) + LogLevel(4B)
} LoginResponse;


typedef struct tagProxyPropertyDll {
	int nProxyMode;
	char szIp[16];//"255.255.255.255"
	int nPort;
	char szName[20];//20
	char szPwd[20];//20
    
	tagProxyPropertyDll()
	{
		clear();
	}
	void clear()
	{
		nProxyMode = 0;
		memset(szIp, 0, 16);
		nPort = 0;
		memset(szName, 0, 20);
		memset(szPwd, 0, 20);
	}
} ProxyPropertyDll, *LPProxyPropertyDll;

typedef struct tagHostPropertyDll
{
	char szIp[16];//"255.255.255.255"
	int nPort;
	int nSpeed;
	long lCurSum;
	long lSum;
	bool bIsNeedAnswer;
	bool IsValid;
    
public:
	void clear()
	{
		memset(szIp, 0, 16);
		nPort = 0;
		nSpeed = -1;
		lCurSum = -1;
		lSum = -1;
		bIsNeedAnswer = true ;
		IsValid = false ;
	}
    
	tagHostPropertyDll()
	{
		clear();
	}
} HostPropertyDll, *LPHostPropertyDll;


typedef struct tagServerPropertyDataDll {
	HostPropertyDll hostProperty;
	ProxyPropertyDll proxyProperty;
	long lIndex;
}ServerPropertyDataDll, *LPServerPropertyDataDll;

typedef bool (__stdcall *PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);

typedef bool (__stdcall *PFFireSessionStatus)(int nSid,int nFlag);
typedef bool (__stdcall *PFNotifyEvent)(int nSid);

#endif // __COM_STRUCT_H__

// end of file
