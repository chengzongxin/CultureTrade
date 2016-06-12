#ifndef __NT_TEA_H__
#define __NT_TEA_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"


/* pKey为16byte */
/*
    输入:nInBufLen为需加密的明文部分(Body)长度;
    输出:返回为加密后的长度(是8byte的倍数);
*/
/* TEA加密算法,CBC模式 */
/* 密文格式:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
uint32_t TeaEncryptLen(uint32_t dwInBufLen);


/* pKey为16byte */
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/* TEA加密算法,CBC模式 */
/* 密文格式:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
int TeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


/* pKey为16byte */
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/* TEA解密算法,CBC模式 */
/* 密文格式:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
int TeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


#ifdef __cplusplus
}
#endif


#endif // __NT_TEA_H__

// end of file
