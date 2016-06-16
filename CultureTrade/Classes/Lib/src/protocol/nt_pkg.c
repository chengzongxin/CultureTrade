#include "nt_pkg.h"
#include "nt_errno.h"
#include "nt_crypt.h"
#include "nt_macro.h"


uint16_t NtPkgHead2Crc16(const uint32_t * pdwBuf)
{
	uint32_t dwCrc = 0;
 
	dwCrc ^= *pdwBuf++;
	dwCrc ^= *pdwBuf++;
	dwCrc ^= *pdwBuf++;
	dwCrc ^= *pdwBuf++;
	dwCrc ^= *pdwBuf++;
 
    return (uint16_t)((dwCrc & 0xffff) ^ (dwCrc >> 16));
}


void EncodeNtPkgHead(NtPkgHead * pstFromNtPkgHead, NtPkgHead * pstToNtPkgHead)
{
	NtPkgHead * pstNtPkgHead;

	//��ΪNULL�Ļ�,��ת��Ŀ��buf,��������ԭbuf��ת��
	pstNtPkgHead = pstToNtPkgHead ? pstToNtPkgHead : pstFromNtPkgHead;

	//��ʼ���Ͱ汾��ȫ����Ĭ��ֵ�滻,�������ϲ��Լ�����
	pstNtPkgHead->bStartFlag = NT_SFLAG;
	pstNtPkgHead->bVer = NT_VER;
	pstNtPkgHead->bEncryptFlag = pstFromNtPkgHead->bEncryptFlag;
	pstNtPkgHead->bFrag = pstFromNtPkgHead->bFrag;

	//������Щת�򶼻������ݶ��ڶ���ĵ�ַ��
	pstNtPkgHead->wLen = htons(pstFromNtPkgHead->wLen);
	pstNtPkgHead->wCmd = htons(pstFromNtPkgHead->wCmd);
	pstNtPkgHead->wSeq = htons(pstFromNtPkgHead->wSeq);
	pstNtPkgHead->wCrc = 0; //�ȸ�ֵΪ0,���������¼���
	pstNtPkgHead->dwSID = htonl(pstFromNtPkgHead->dwSID);
	memcpy(pstNtPkgHead->sReserved, pstFromNtPkgHead->sReserved, sizeof(pstToNtPkgHead->sReserved));

	//���¼���crc
	pstNtPkgHead->wCrc = htons(NtPkgHead2Crc16((const uint32_t *)pstNtPkgHead));

	return;
}


int DecodeNtPkgHead(NtPkgHead * pstFromNtPkgHead, NtPkgHead * pstToNtPkgHead)
{
	NtPkgHead * pstNtPkgHead;
	uint16_t wCrc;

	if (NT_SFLAG != pstFromNtPkgHead->bStartFlag) {
		return E_ERR;
	}

	//��ΪNULL�Ļ�,��ת��Ŀ��buf,��������ԭbuf��ת��
	pstNtPkgHead = pstToNtPkgHead ? pstToNtPkgHead : pstFromNtPkgHead;

	wCrc = pstFromNtPkgHead->wCrc;
	pstFromNtPkgHead->wCrc = 0;
	//���㲢�Ƚ�crc
	if (NtPkgHead2Crc16((const uint32_t *)pstFromNtPkgHead) != ntohs(wCrc)) {
		pstFromNtPkgHead->wCrc = wCrc;
		return E_ERR;
	}
	pstFromNtPkgHead->wCrc = wCrc;

	//��ʼ���Ͱ汾��ȫ����Ĭ��ֵ�滻,�������ϲ��Լ�����
	pstNtPkgHead->bStartFlag = pstFromNtPkgHead->bStartFlag;
	pstNtPkgHead->bVer = pstFromNtPkgHead->bVer;
	pstNtPkgHead->bEncryptFlag = pstFromNtPkgHead->bEncryptFlag;
	pstNtPkgHead->bFrag = pstFromNtPkgHead->bFrag;

	//������Щת�򶼻������ݶ��ڶ���ĵ�ַ��
	pstNtPkgHead->wLen = ntohs(pstFromNtPkgHead->wLen);
	pstNtPkgHead->wCmd = ntohs(pstFromNtPkgHead->wCmd);
	pstNtPkgHead->wSeq = ntohs(pstFromNtPkgHead->wSeq);
	pstNtPkgHead->wCrc = ntohs(pstFromNtPkgHead->wCrc);
	pstNtPkgHead->dwSID = ntohl(pstFromNtPkgHead->dwSID);
	memcpy(pstNtPkgHead->sReserved, pstFromNtPkgHead->sReserved, sizeof(pstNtPkgHead->sReserved));

	return 0;
}


//���
int EncodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen)
{
	int iRet;
	NtPkg * pstNtPkg;
	uint8_t * pbBody;
	uint8_t pbCipher[MAX_PKG_LEN];
	uint32_t dwCipherBodyLen;

	assert(pstFromNtPkg);

	//��ΪNULL�Ļ�,��ת��Ŀ��buf,��������ԭbuf��ת��
	if (pstToNtPkg) {
		pstNtPkg = pstToNtPkg;
		pbBody = pstNtPkg->sBody; //ָ�����ܺ����ݷ�������
		dwCipherBodyLen = *pdwPkgLen - sizeof(NtPkgHead);
	} else {
		pstNtPkg = pstFromNtPkg;
		pbBody = pbCipher; //ָ�����ܺ����ݷ�������
		dwCipherBodyLen = sizeof(pbCipher) - sizeof(NtPkgHead);
	}

	iRet = SymEncrypt(GET_ENCRYPT_FLAG(pstFromNtPkg->stNtPkgHead.bEncryptFlag), 
		pstFromNtPkg->sBody, pstFromNtPkg->stNtPkgHead.wLen - sizeof(NtPkgHead), 
		pbCommKey, pbBody, &dwCipherBodyLen);
	if (0 != iRet) {
		return iRet;
	}

	if (pbBody != pstNtPkg->sBody) {
		memcpy(pstNtPkg->sBody, pbBody, dwCipherBodyLen);
	}

	pstFromNtPkg->stNtPkgHead.wLen = sizeof(NtPkgHead) + dwCipherBodyLen;
	if (pdwPkgLen) {
		*pdwPkgLen = pstFromNtPkg->stNtPkgHead.wLen;
	}
	EncodeNtPkgHead(&pstFromNtPkg->stNtPkgHead, &pstNtPkg->stNtPkgHead);

	return 0;
}


//���
int DecodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen)
{
	int iRet;
	NtPkg * pstNtPkg;
	uint8_t * pbBody;
	uint8_t pbPlain[MAX_PKG_LEN];
	uint32_t dwPlainBodyLen;

	assert(pstFromNtPkg);

	//��ΪNULL�Ļ�,��ת��Ŀ��buf,��������ԭbuf��ת��
	if (pstToNtPkg) {
		pstNtPkg = pstToNtPkg;
		pbBody = pstNtPkg->sBody; //ָ�����ܺ����ݷ�������
		dwPlainBodyLen = *pdwPkgLen - sizeof(NtPkgHead);
	} else {
		pstNtPkg = pstFromNtPkg;
		pbBody = pbPlain; //ָ�����ܺ����ݷ�������
		dwPlainBodyLen = sizeof(pbPlain) - sizeof(NtPkgHead);
	}

	iRet = DecodeNtPkgHead(&pstFromNtPkg->stNtPkgHead, &pstNtPkg->stNtPkgHead);
	if (0 != iRet) {
		return iRet;
	}

	iRet = SymDecrypt(GET_ENCRYPT_FLAG(pstNtPkg->stNtPkgHead.bEncryptFlag), 
		pstFromNtPkg->sBody, pstNtPkg->stNtPkgHead.wLen - sizeof(NtPkgHead), 
		pbCommKey, pbBody, &dwPlainBodyLen);
	if (0 != iRet) {
		return iRet;
	}

	if (pbBody != pstNtPkg->sBody) {
		memcpy(pstNtPkg->sBody, pbBody, dwPlainBodyLen);
	}

	pstNtPkg->stNtPkgHead.wLen = sizeof(NtPkgHead) + dwPlainBodyLen;
	if (pdwPkgLen) {
		*pdwPkgLen = pstNtPkg->stNtPkgHead.wLen;
	}

	return 0;
}


//���
int EncodeNtPkgEx(uint8_t bEncryptFlag, uint16_t wCmd, uint16_t wSeq, uint32_t dwSID, const uint8_t * pbBody, uint32_t dwBodyLen, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen)
{
	int iRet;
	uint32_t dwCipherBodyLen = MAX_PKG_LEN - sizeof(NtPkgHead);
	NtPkgHead * pstNtPkgHead = &pstToNtPkg->stNtPkgHead;

	pstNtPkgHead->bEncryptFlag = bEncryptFlag;
	pstNtPkgHead->bFrag = 0;
	pstNtPkgHead->wCmd = wCmd;
	pstNtPkgHead->wSeq = wSeq;
	pstNtPkgHead->dwSID = dwSID;
	memset(pstNtPkgHead->sReserved, 0, sizeof(pstNtPkgHead->sReserved));

	iRet = SymEncrypt(GET_ENCRYPT_FLAG(pstNtPkgHead->bEncryptFlag), pbBody, dwBodyLen, 
		pbCommKey, pstToNtPkg->sBody, &dwCipherBodyLen);
	if (0 != iRet) {
		return iRet;
	}

	pstToNtPkg->stNtPkgHead.wLen = sizeof(NtPkgHead) + dwCipherBodyLen;
	if (pdwPkgLen) {
		*pdwPkgLen = pstToNtPkg->stNtPkgHead.wLen;
	}
	EncodeNtPkgHead(&pstToNtPkg->stNtPkgHead, NULL);

	return 0;
}


int DecodeNtPkgEx(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkgHead * pstNtPkgHead, uint8_t * pbBody, uint32_t * pdwBodyLen)
{
	int iRet;

	iRet = DecodeNtPkgHead(&pstFromNtPkg->stNtPkgHead, pstNtPkgHead);
	if (0 != iRet) {
		return iRet;
	}

	iRet = SymDecrypt(GET_ENCRYPT_FLAG(pstNtPkgHead->bEncryptFlag), 
		pstFromNtPkg->sBody, pstNtPkgHead->wLen - sizeof(NtPkgHead), 
		pbCommKey, pbBody, pdwBodyLen);
	if (0 != iRet) {
		return iRet;
	}

	return 0;
}


// end of file
