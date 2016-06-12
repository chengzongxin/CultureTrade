#ifndef __NT_PROTOCOL_H__
#define __NT_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"



//起始标志
#define NT_SFLAG						0xFF
#define NT_VER							0x1
#define MAX_PKG_LEN						65536 // 64K(64 * 1024)

typedef struct {
	uint8_t  bStartFlag; //协议包起始标志,使用 NT_SFLAG
	uint8_t  bVer; //版本号,使用NT_VER
	uint8_t  bEncryptFlag; //加密标志(如果不加密,则为0,其值请参照nt_crypt.h中的枚举值)
	uint8_t  bFrag; //是否有包分片(分片由应用自己完成)
	uint16_t wLen; //总包长(包含协议头和数据)
	uint16_t wCmd; //命令号(消息号)
	uint16_t wSeq; //包的序列号,业务使用,转发不再关心(客户端顺序生成,同一个包的分片请使用同一个序列号)
	uint16_t wCrc; //Crc16校验码(参考nt_crypt.h里的NtPkgHead2Crc16方法)
	uint32_t dwSID; //会话ID,前端和后端分别使用不同的SID(客户端如果跟服务端有多个会话,请使用不同的会话id,此字段本客户端内不同会话唯一即可)
	uint8_t  sReserved[4]; //保留字段(一般用来作为回应包分片的信息存放)
} NtPkgHead; //协议包头



typedef struct {
	NtPkgHead stNtPkgHead;
	uint8_t   sBody[0];
} NtPkg; //协议包


#ifdef __cplusplus
}
#endif

#endif // __NT_PROTOCOL_H__

// end of file
