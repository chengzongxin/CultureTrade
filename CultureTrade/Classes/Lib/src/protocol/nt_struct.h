#ifndef __NT_STRUCT_H__
#define __NT_STRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif


#pragma once


#include "nt_types.h"


//����Э��, ������Э��
//����
typedef struct {
	uint32_t dwTickCount;
} HeartBeatRequest;


//��Ӧ
typedef struct {
	uint32_t dwTickCount;
	union {
		uint32_t dwServerTime; //���ڿͻ��˷���������,��ת��Ӧ���ǵ�ǰʱ��
		uint32_t dwPayLoad; //������ת����������,ҵ���Ӧ���Ǹ������(�����ͳ����Ϣ,�򷵻�������(�ܰ���,bps, pps��),���򷵻ص�ǰ������)
	} ServerStatus;
} HeartBeatResponse;


//��¼Э��,����Э��
//����
//�������ݵ�����ʵ��
typedef struct {
	uint8_t  sLoginId[20];
	uint32_t dwClientTime;
	uint8_t  sMAC[6]; //����������ַ
	uint8_t  sReserved[2]; //����Э��
} PlainLoginReq;

typedef struct {
	uint32_t dwClientVersion; //��¼�汾��
	uint32_t dwLoginIdType; // �ʺ�����
	uint8_t  sLoginId[20]; //��¼�ʺ�
	uint32_t dwClientTime;
	uint32_t dwIp; //����ת�������ǿͻ��˵ı���ip��port,ת��������¼�����ʱ��,�滻���ⲿip��port
	uint32_t dwPort;
	//�����ⲿ���Ǽ��ܵ�����,��Կ������Ķ���MD5
	//PlainLoginReq stPlainLoginReq;
	uint8_t  sCipherData[0]; //sLoginID(20B) + dwClientTime(4B) + MAC(6B)...
} LoginRequest; // ��¼����


typedef struct {
	uint8_t  sCommKey[16];
	uint32_t dwUID;
	uint32_t dwIp;
	uint32_t dwGenerateTime;
	uint32_t dwExpireTime; // ʹ�õ������ʱ��,��ʱ��,����36000(s)(10Сʱ)
} LoginTicket;


//�������ݵ�����ʵ��
typedef struct {
	uint32_t dwUID;
	uint8_t  sCommKey[16]; //����ͨ�ŵļ�����Կ
	uint32_t dwClientTime;
	uint8_t  sTicket[sizeof(LoginTicket) + 4]; //��Ʊ���Ժ���չ����������ʹ��,Ŀǰ��û��ʹ��,�ͻ���ԭ�����漴��
	uint32_t dwExpireTime; //���ֶα���Ticket�Ĺ���ʱ��(��ʾ��ú����(��Ϊ��λ)),�ͻ������ڹ���ǰ���������µ�Ticket
	uint32_t dwLogLevel;
} PlainLoginResp;

//��Ӧ
typedef struct {
	int32_t  iRetCode; //0 : ��¼�ɹ�, -1 : �ʺŲ�����, -2 : �������, -3 : ����,���ʧ����ֻ��dwRetCode
	//UID��CommKey���ڵ�¼���񴫸�ת����ʱ��,��ת������Ϊ0,�Կͻ��˲��ɼ�
	//���RetCodeС��0,��û�����������
	uint32_t dwUID;
	uint8_t  sCommKey[16];

	//�����ⲿ���Ǽ��ܵ�,��Կ������Ķ���MD5,����¼�ɹ���,�����İ���ʹ��sCommKey���м���
	//PlainLoginResp stPlainLoginResp;
	uint8_t  sCipherData[0]; //UID(4B) + sCommKey(16B) + ClientTime(4B) + Ticket(36B) + ExpireTime(4B) + LogLevel(4B)
} LoginResponse; // ��¼��Ӧ��


//���ĵĵǳ�����
typedef struct {
	uint32_t dwUID;
} PlainLogoutReq;


//���ĵĵǳ�����
typedef struct {
	uint8_t sCipherData[0];
} LogoutRequest;


//��¼֪ͨ,��ת����λ�÷���
typedef struct {
	uint32_t dwSID; //�Ự��SID
	uint32_t dwUID; //�û���UID
	uint32_t dwLoginIdType; //��¼�ʺ�����
	uint8_t  sLoginId[20]; //��¼�ʺ�
	uint32_t dwCltIp; //�ͻ���ip
	uint16_t wCltPort; //�ͻ���port
	uint8_t  sReserved[2]; //�����ֶ�
} LoginNotify;


//�ǳ�֪ͨ,��ת����λ�÷���
typedef struct {
	uint32_t dwSID; //�Ự��SID
	uint32_t dwUID; //�û���UID
	uint32_t dwLoginIdType; //��¼�ʺ�����
} LogoutNotify;


// ���ĵ����û�����֪ͨ
typedef struct {
	uint32_t dwUID;
} PlainKickNotify;


//���ĵ����û�����֪ͨ
typedef struct {
	uint8_t sCipherData[0];
} KickNotify;


// ������֪ͨ
typedef struct {
	uint32_t dwUID;
	uint32_t dwLoginIdType;
} PasswdChangeNotify;


//Ͷ��֪ͨ��
typedef struct {
//	NtPkg    stNtPkg;
//	uint32_t adwUID[0];
	uint32_t dwUIDNum;
	uint32_t dwLoginIdType;
} MailNotifyPkg;


typedef struct {
	uint32_t dwIsPushQuote; // �Ƿ���������
} QuoteCtrl;


#ifdef __cplusplus
}
#endif

#endif // __NT_STRUCT_H__

// end of file

