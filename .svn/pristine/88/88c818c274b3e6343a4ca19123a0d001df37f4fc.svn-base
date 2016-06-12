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

	//不为NULL的话,则转序到目标buf,否则仍在原buf上转序
	pstNtPkgHead = pstToNtPkgHead ? pstToNtPkgHead : pstFromNtPkgHead;

	//起始符和版本号全部用默认值替换,后续由上层自己来决
	pstNtPkgHead->bStartFlag = NT_SFLAG;
	pstNtPkgHead->bVer = NT_VER;
	pstNtPkgHead->bEncryptFlag = pstFromNtPkgHead->bEncryptFlag;
	pstNtPkgHead->bFrag = pstFromNtPkgHead->bFrag;

	//以下这些转序都基于数据都在对齐的地址上
	pstNtPkgHead->wLen = htons(pstFromNtPkgHead->wLen);
	pstNtPkgHead->wCmd = htons(pstFromNtPkgHead->wCmd);
	pstNtPkgHead->wSeq = htons(pstFromNtPkgHead->wSeq);
	pstNtPkgHead->wCrc = 0; //先赋值为0,后面再重新计算
	pstNtPkgHead->dwSID = htonl(pstFromNtPkgHead->dwSID);
	memcpy(pstNtPkgHead->sReserved, pstFromNtPkgHead->sReserved, sizeof(pstToNtPkgHead->sReserved));

	//重新计算crc
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

	//不为NULL的话,则转序到目标buf,否则仍在原buf上转序
	pstNtPkgHead = pstToNtPkgHead ? pstToNtPkgHead : pstFromNtPkgHead;

	wCrc = pstFromNtPkgHead->wCrc;
	pstFromNtPkgHead->wCrc = 0;
	//计算并比较crc
	if (NtPkgHead2Crc16((const uint32_t *)pstFromNtPkgHead) != ntohs(wCrc)) {
		pstFromNtPkgHead->wCrc = wCrc;
		return E_ERR;
	}
	pstFromNtPkgHead->wCrc = wCrc;

	//起始符和版本号全部用默认值替换,后续由上层自己来决
	pstNtPkgHead->bStartFlag = pstFromNtPkgHead->bStartFlag;
	pstNtPkgHead->bVer = pstFromNtPkgHead->bVer;
	pstNtPkgHead->bEncryptFlag = pstFromNtPkgHead->bEncryptFlag;
	pstNtPkgHead->bFrag = pstFromNtPkgHead->bFrag;

	//以下这些转序都基于数据都在对齐的地址上
	pstNtPkgHead->wLen = ntohs(pstFromNtPkgHead->wLen);
	pstNtPkgHead->wCmd = ntohs(pstFromNtPkgHead->wCmd);
	pstNtPkgHead->wSeq = ntohs(pstFromNtPkgHead->wSeq);
	pstNtPkgHead->wCrc = ntohs(pstFromNtPkgHead->wCrc);
	pstNtPkgHead->dwSID = ntohl(pstFromNtPkgHead->dwSID);
	memcpy(pstNtPkgHead->sReserved, pstFromNtPkgHead->sReserved, sizeof(pstNtPkgHead->sReserved));

	return 0;
}


//打包
int EncodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen)
{
	int iRet;
	NtPkg * pstNtPkg;
	uint8_t * pbBody;
	uint8_t pbCipher[MAX_PKG_LEN];
	uint32_t dwCipherBodyLen;

	assert(pstFromNtPkg);

	//不为NULL的话,则转序到目标buf,否则仍在原buf上转序
	if (pstToNtPkg) {
		pstNtPkg = pstToNtPkg;
		pbBody = pstNtPkg->sBody; //指明加密后数据放在哪里
		dwCipherBodyLen = *pdwPkgLen - sizeof(NtPkgHead);
	} else {
		pstNtPkg = pstFromNtPkg;
		pbBody = pbCipher; //指明加密后数据放在哪里
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


//解包
int DecodeNtPkg(NtPkg * pstFromNtPkg, const uint8_t * pbCommKey, NtPkg * pstToNtPkg, uint32_t * pdwPkgLen)
{
	int iRet;
	NtPkg * pstNtPkg;
	uint8_t * pbBody;
	uint8_t pbPlain[MAX_PKG_LEN];
	uint32_t dwPlainBodyLen;

	assert(pstFromNtPkg);

	//不为NULL的话,则转序到目标buf,否则仍在原buf上转序
	if (pstToNtPkg) {
		pstNtPkg = pstToNtPkg;
		pbBody = pstNtPkg->sBody; //指明解密后数据放在哪里
		dwPlainBodyLen = *pdwPkgLen - sizeof(NtPkgHead);
	} else {
		pstNtPkg = pstFromNtPkg;
		pbBody = pbPlain; //指明解密后数据放在哪里
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


//打包
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
