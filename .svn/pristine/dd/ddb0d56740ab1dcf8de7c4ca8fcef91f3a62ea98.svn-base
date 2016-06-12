#include "nt_xxtea.h"


#define XXTEA_MX	(((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (pdwKey[(p & 3) ^ e] ^ z))
#define XXTEA_DELTA	0x6e686274 //nhbt


//加密
int XXTeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	uint32_t e, p, q, sum, y, z, * pdwOutBuf;
	const uint32_t * pdwKey;

	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (0 == dwInBufLen || (dwInBufLen & 3) || *pdwOutBufLen < dwInBufLen) { //必须是4字节大小的倍数
		return -1;
	}

	*pdwOutBufLen = dwInBufLen;
	memcpy(pbOutBuf, pbInBuf, *pdwOutBufLen);

	pdwKey = (const uint32_t *)pbKey;
	pdwOutBuf = (uint32_t *)pbOutBuf;
	dwInBufLen >>= 2; //除以4

	q = 6 + 52 / dwInBufLen;
	sum = 0;
	y = pdwOutBuf[0];
	z = pdwOutBuf[dwInBufLen - 1];

	while (q-- > 0) {
		sum += XXTEA_DELTA;
		e = (sum >> 2) & 3;

		for (p = 0; p < dwInBufLen - 1; p++) {
			y = pdwOutBuf[p + 1];
			z = pdwOutBuf[p] += XXTEA_MX;
		}

		y = pdwOutBuf[0];
		z = pdwOutBuf[dwInBufLen - 1] += XXTEA_MX;
	}

	return 0;
}


//解密
int XXTeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	uint32_t e, p, q, sum, y, z, * pdwOutBuf;
	const uint32_t * pdwKey;

	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (0 == dwInBufLen || (dwInBufLen & 3) || *pdwOutBufLen < dwInBufLen) { //必须是4字节大小的倍数
		return -1;
	}

	*pdwOutBufLen = dwInBufLen;
	memcpy(pbOutBuf, pbInBuf, *pdwOutBufLen);

	pdwKey = (const uint32_t *)pbKey;
	pdwOutBuf = (uint32_t *)pbOutBuf;
	dwInBufLen >>= 2; //除以4

	q = 6 + 52 / dwInBufLen;
	sum = q * XXTEA_DELTA;
	y = pdwOutBuf[0];
	z = pdwOutBuf[dwInBufLen - 1];

	while (0 != sum) {
		e = (sum >> 2) & 3;
		for (p = dwInBufLen - 1; p > 0 ; p--) {
			z = pdwOutBuf[p - 1];
				y = pdwOutBuf[p] -= XXTEA_MX;
		}

		z = pdwOutBuf[dwInBufLen - 1];
		y = pdwOutBuf[0] -= XXTEA_MX;
		sum -= XXTEA_DELTA;
	}

	return 0;
}


// end of file
