#ifndef __NT_PKG_H__
#define __NT_PKG_H__

#ifdef __cplusplus
extern "C" {
#endif


//应用类打包函数

#include "nt_types.h"
#include "nt_protocol.h"


typedef enum {
	NO_COMPRESS = 0,
	ZLIB = 1, 
	LZW, 
	LZSS, 
	SNAPPY, 
	LZO, 
	LIBLZF, 
	QUICKLZ, 
	FASTLZ = 8,
} COMPRESS_TYPE;


//计算NtPkgHead的crc值
uint16_t NtPkgHead2Crc16(const uint32_t * pdwBuf);

//打包函数,如果pstToNtPkgHead为NULL,则默认打包到pstFromNtPkgHead上
void EncodeNtPkgHead(NtPkgHead * pstFromNtPkgHead, NtPkgHead * pstToNtPkgHead);

//解包函数,如果pstToNtPkgHead为NULL,则默认解包到pstFromNtPkgHead上
int DecodeNtPkgHead(NtPkgHead * pstFromNtPkgHead, NtPkgHead * pstToNtPkgHead);

//打包
int EncodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen);

//解包
int DecodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen);

//打包
int EncodeNtPkgEx(uint8_t bEncryptFlag, uint16_t wCmd, uint16_t wSeq, uint32_t dwSID, const uint8_t * pbBody, uint32_t dwBodyLen, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen);

//解包
int DecodeNtPkgEx(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkgHead * pstNtPkgHead, uint8_t * pbBody, uint32_t * pdwBodyLen);


#ifdef __cplusplus
}
#endif

#endif // __NT_PKG_H__

// end of file
