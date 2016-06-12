#ifndef __NT_STRUCT_H__
#define __NT_STRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif


#pragma once


#include "nt_types.h"


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
	uint8_t  sMAC[6]; //物理网卡地址
	uint8_t  sReserved[2]; //补齐协议
} PlainLoginReq;

typedef struct {
	uint32_t dwClientVersion; //登录版本号
	uint32_t dwLoginIdType; // 帐号类型
	uint8_t  sLoginId[20]; //登录帐号
	uint32_t dwClientTime;
	uint32_t dwIp; //在中转看到的是客户端的本机ip和port,转发传给登录服务的时候,替换成外部ip和port
	uint32_t dwPort;
	//下面这部分是加密的密文,密钥是密码的二次MD5
	//PlainLoginReq stPlainLoginReq;
	uint8_t  sCipherData[0]; //sLoginID(20B) + dwClientTime(4B) + MAC(6B)...
} LoginRequest; // 登录请求


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
	uint32_t dwClientTime;
	uint8_t  sTicket[sizeof(LoginTicket) + 4]; //此票据以后扩展供其它功能使用,目前暂没有使用,客户端原样保存即可
	uint32_t dwExpireTime; //此字段表明Ticket的过期时间(表示多久后过期(秒为单位)),客户端须在过期前重新请求新的Ticket
	uint32_t dwLogLevel;
} PlainLoginResp;

//回应
typedef struct {
	int32_t  iRetCode; //0 : 登录成功, -1 : 帐号不存在, -2 : 密码错误, -3 : 其它,如果失败则只有dwRetCode
	//UID和CommKey是在登录服务传给转发的时候,由转发设置为0,对客户端不可见
	//如果RetCode小于0,则没有下面的数据
	uint32_t dwUID;
	uint8_t  sCommKey[16];

	//下面这部分是加密的,密钥是密码的二次MD5,当登录成功后,后续的包均使用sCommKey进行加密
	//PlainLoginResp stPlainLoginResp;
	uint8_t  sCipherData[0]; //UID(4B) + sCommKey(16B) + ClientTime(4B) + Ticket(36B) + ExpireTime(4B) + LogLevel(4B)
} LoginResponse; // 登录回应包


//明文的登出请求
typedef struct {
	uint32_t dwUID;
} PlainLogoutReq;


//密文的登出请求
typedef struct {
	uint8_t sCipherData[0];
} LogoutRequest;


//登录通知,中转发给位置服务
typedef struct {
	uint32_t dwSID; //会话的SID
	uint32_t dwUID; //用户的UID
	uint32_t dwLoginIdType; //登录帐号类型
	uint8_t  sLoginId[20]; //登录帐号
	uint32_t dwCltIp; //客户端ip
	uint16_t wCltPort; //客户端port
	uint8_t  sReserved[2]; //保留字段
} LoginNotify;


//登出通知,中转发给位置服务
typedef struct {
	uint32_t dwSID; //会话的SID
	uint32_t dwUID; //用户的UID
	uint32_t dwLoginIdType; //登录帐号类型
} LogoutNotify;


// 明文的踢用户下线通知
typedef struct {
	uint32_t dwUID;
} PlainKickNotify;


//密文的踢用户下线通知
typedef struct {
	uint8_t sCipherData[0];
} KickNotify;


// 密码变更通知
typedef struct {
	uint32_t dwUID;
	uint32_t dwLoginIdType;
} PasswdChangeNotify;


//投递通知包
typedef struct {
//	NtPkg    stNtPkg;
//	uint32_t adwUID[0];
	uint32_t dwUIDNum;
	uint32_t dwLoginIdType;
} MailNotifyPkg;


typedef struct {
	uint32_t dwIsPushQuote; // 是否推送行情
} QuoteCtrl;


#ifdef __cplusplus
}
#endif

#endif // __NT_STRUCT_H__

// end of file

