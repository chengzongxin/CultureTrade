#ifndef __NT_TEA_H__
#define __NT_TEA_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"


/* pKeyΪ16byte */
/*
    ����:nInBufLenΪ����ܵ����Ĳ���(Body)����;
    ���:����Ϊ���ܺ�ĳ���(��8byte�ı���);
*/
/* TEA�����㷨,CBCģʽ */
/* ���ĸ�ʽ:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
uint32_t TeaEncryptLen(uint32_t dwInBufLen);


/* pKeyΪ16byte */
/*
	����:pInBufΪ����ܵ����Ĳ���(Body),nInBufLenΪpInBuf����;
	���:pOutBufΪ���ĸ�ʽ,pOutBufLenΪpOutBuf�ĳ�����8byte�ı���,����ӦԤ��nInBufLen+17;
*/
/* TEA�����㷨,CBCģʽ */
/* ���ĸ�ʽ:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
int TeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


/* pKeyΪ16byte */
/*
	����:pInBufΪ���ĸ�ʽ,nInBufLenΪpInBuf�ĳ�����8byte�ı���; *pOutBufLenΪ���ջ������ĳ���
		�ر�ע��*pOutBufLenӦԤ�ý��ջ������ĳ���!
	���:pOutBufΪ����(Body),pOutBufLenΪpOutBuf�ĳ���,����ӦԤ��nInBufLen-10;
	����ֵ:�����ʽ��ȷ����TRUE;
*/
/* TEA�����㷨,CBCģʽ */
/* ���ĸ�ʽ:PadLen(1byte) + Padding(var, 0-7byte) + Salt(2byte) + Body(var byte) + Zero(7byte) */
int TeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


#ifdef __cplusplus
}
#endif


#endif // __NT_TEA_H__

// end of file
