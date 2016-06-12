#include "nt_tea.h"


/*
	void TeaEncryptECB(uint8_t *pbInBuf, const uint32_t * k, uint8_t *pbOutBuf);
	void TeaDecryptECB(uint8_t *pbInBuf, onst uint32_t * k, uint8_t *pbOutBuf);
*/

#define DELTA		0x6e686274 //nhbt
// delta = (sqrt(5) - 1) / 2 * 2^32, (sqrt(5) - 1) / 2 是黄金分割点的值.
#define ROUNDS		16
#define LOG_ROUNDS	4 // LOG_ROUNDS = log2(ROUNDS)
#define INIT_SUM	(uint32_t)(DELTA << LOG_ROUNDS)



void tea_encrypt(uint32_t * v, const uint32_t * k)
{
	uint32_t i, v0, v1, sum; /* set up */

	v0 = v[0];
	v1 = v[1];
	sum = 0;

	for (i = 0; i < ROUNDS; i++) {
		sum += DELTA;
		v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
		v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
	}

	v[0] = v0;
	v[1] = v1;

	return;
}


void tea_decrypt(uint32_t * v, const uint32_t * k)
{
	uint32_t i, v0, v1, sum; /* set up */

	v0 = v[0];
	v1 = v[1];
	sum = INIT_SUM;

	for (i = 0; i < ROUNDS; i++) {
		v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
		v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
		sum -= DELTA;
	}

	v[0] = v0;
	v[1] = v1;

	return;
}


/* pbOutBuffer、pbInBuffer均为8Bytes, pbKey为16Bytes */
void TeaEncryptECB(const uint8_t * pbInBuf, const uint32_t * k, uint8_t * pbOutBuf)
{
	uint32_t i, v0, v1, sum;

	v0 = ntohl(*(const uint32_t *)pbInBuf);
	v1 = ntohl(*(const uint32_t *)(pbInBuf + 4));
	sum = 0;

	for (i = 0; i < ROUNDS; i++) {
		sum += DELTA;
		v0 += (v1 << 4) + (k[0] ^ v1) + (sum ^ (v1 >> 5)) + k[1];
		v1 += (v0 << 4) + (k[2] ^ v0) + (sum ^ (v0 >> 5)) + k[3];
	}

	*(uint32_t *)pbOutBuf = htonl(v0);
	*(uint32_t *)(pbOutBuf + 4) = htonl(v1);

	/* now encrypted buf is TCP/IP-endian; */
	return;
}


/* pbOutBuffer、pbInBuffer均为8Bytes, pbKey为16Bytes */
void TeaDecryptECB(const uint8_t * pbInBuf, const uint32_t * k, uint8_t * pbOutBuf)
{
	uint32_t i, v0, v1, sum;

	v0 = ntohl(*(const uint32_t*)pbInBuf);
	v1 = ntohl(*(const uint32_t*)(pbInBuf + 4));
	sum = INIT_SUM;

	for (i = 0; i < ROUNDS; i++) {
		v1 -= (v0 << 4) + (k[2] ^ v0) + (sum ^ (v0 >> 5)) + k[3]; 
		v0 -= (v1 << 4) + (k[0] ^ v1) + (sum ^ (v1 >> 5)) + k[1];
		sum -= DELTA;
	}

	*(uint32_t *)pbOutBuf = htonl(v0);
	*(uint32_t *)(pbOutBuf + 4) = htonl(v1);

	/* now plain-text is TCP/IP-endian; */
	return;
}


#define SALT_LEN		2
#define ZERO_LEN		7


/* ///////////////////////////////////////////////////////////////////////////////////////////// */

/* pbKey为16Bytes */
/*
	输入:dwInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8Bytes的倍数);
*/
/* TEA加密算法,CBC模式 */
/* 密文格式:PadLen(1Bytes) + Padding(var, 0-7Bytes) + Salt(2Bytes) + Body(var Bytes) + Zero(7Bytes) */
uint32_t TeaEncryptLen(uint32_t dwInBufLen)
{
	uint32_t dwPadlen, dwPadSaltBodyZeroLen; /* PadLen(1Bytes) + Salt + Body + Zero的长度 */

	/* 根据Body长度计算PadLen, 最小必需长度必需为8Bytes的整数倍 */
	dwPadSaltBodyZeroLen = dwInBufLen/* Body长度 */ + 1 + SALT_LEN + ZERO_LEN/* PadLen(1Bytes) + Salt(2Bytes) + Zero(7Bytes) */;
	if ((dwPadlen = dwPadSaltBodyZeroLen & 7)) {/* len = dwSaltBodyZeroLen % 8 */
		/* 模8余0需补0,余1补7,余2补6,...,余7补1 */
		dwPadlen = 8 - dwPadlen;
	}

	return dwPadSaltBodyZeroLen + dwPadlen;
}


/* pbKey为16Bytes */
/*
	输入:pbInBuf为需加密的明文部分(Body),dwInBufLen为pbInBuf长度;
	输出:pbOutBuf为密文格式,pbOutBufLen为pbOutBuf的长度是8Bytes的倍数;
*/
/* TEA加密算法,CBC模式 */
/*密文格式:PadLen(1Bytes) + Padding(var, 0-7Bytes) + Salt(2Bytes) + Body(var Bytes) + Zero(7Bytes) */
int TeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	uint32_t k[4], dwPadlen, dwPadSaltBodyZeroLen; /* PadLen(1Bytes) + Salt + Body + Zero的长度 */
	uint8_t src_buf[8], iv_plain[8], * iv_crypt;
	int src_i, i, j, iAlign;

	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	/* 根据Body长度计算PadLen,最小必需长度必需为8Bytes的整数倍 */
	dwPadSaltBodyZeroLen = dwInBufLen/* Body长度 */ + 1 + SALT_LEN + ZERO_LEN/*PadLen(1Bytes)+Salt(2Bytes)+Zero(7Bytes) */;
	if ((dwPadlen = dwPadSaltBodyZeroLen & 7)) {/* len = dwSaltBodyZeroLen % 8 */
		/* 模8余0需补0,余1补7,余2补6,...,余7补1 */
		dwPadlen = 8 - dwPadlen;
	}

	/* 增加输出buffer长度检查, *pbOutBufLen需预设为输出buffer的长度! */
	if (dwPadSaltBodyZeroLen + dwPadlen < 0 || dwPadSaltBodyZeroLen + dwPadlen > *pdwOutBufLen) {
		return -1;
	}

	if ((long)pbOutBuf & 3) {
		iAlign = 0;
	} else {
		iAlign = 1;
	}

	/* srand( (unsigned)time( NULL ) ); 初始化随机数 */
	/* 加密第一块数据(8Bytes),取前面10Bytes */
	src_buf[0] = (((uint8_t)rand()) & 0x0f8)/* 最低三位存PadLen,清零 */ | ((uint8_t)dwPadlen);
	src_i = 1; /* src_i指向src_buf下一个位置 */

	while (dwPadlen--) {
		src_buf[src_i++] = (uint8_t)rand(); /*Padding*/
	}

	/* come here, src_i must <= 8 */
	*(uint32_t *)iv_plain = 0;
	*(uint32_t *)(iv_plain + 4) = 0;

	iv_crypt = iv_plain; /* make zero iv */

	*pdwOutBufLen = 0; /* init OutBufLen */

	for (i = 0; i < 4; i++) {
		/* now key is TCP/IP-endian; */
		k[i] = ntohl(*(const uint32_t *)(pbKey + i * 4));
	}

	for (i = 1; i <= SALT_LEN;) {/* Salt(2Bytes) */
		if (src_i < 8) {
			src_buf[src_i++] = (uint8_t)rand();
			i++; /* i inc in here */
		}

		if (8 == src_i) {
			/* 8 == src_i */
			*(uint32_t *)src_buf ^= *(uint32_t *)iv_crypt; /* 加密前异或前8个Bytes的密文(iv_crypt指向的) */
			*(uint32_t *)(src_buf + 4) ^= *(uint32_t *)(iv_crypt + 4);

			/* pbOutBuffer、pbInBuffer均为8Bytes, pbKey为16Bytes */
			/* 加密 */
			TeaEncryptECB(src_buf, k, pbOutBuf);

			if (0 == iAlign) { //是否在对齐的地址上
				for (j = 0; j < 8; j++) {/* 加密后异或前8个Bytes的明文(iv_plain指向的) */
					pbOutBuf[j] ^= iv_plain[j];
				}
			} else {
				*(uint32_t *)pbOutBuf ^= *(uint32_t *)iv_plain;
				*(uint32_t *)(pbOutBuf + 4) ^= *(uint32_t *)(iv_plain + 4);
			}

			/* 保存当前的iv_plain */
			*(uint32_t *)iv_plain = *(uint32_t *)src_buf;
			*(uint32_t *)(iv_plain + 4) = *(uint32_t *)(src_buf + 4);

			/* 更新iv_crypt */
			src_i = 0;
			iv_crypt = pbOutBuf;
			*pdwOutBufLen += 8;
			pbOutBuf += 8;
		}
	}

	/* src_i指向src_buf下一个位置 */
	while (dwInBufLen) {
		if (src_i < 8) {
			src_buf[src_i++] = *(pbInBuf++);
			dwInBufLen--;
		}

		if (8 == src_i) {
			/* 8 == src_i */
			*(uint32_t *)src_buf ^= *(uint32_t *)iv_crypt; /* 加密前异或前8个Bytes的密文(iv_crypt指向的) */
			*(uint32_t *)(src_buf + 4) ^= *(uint32_t *)(iv_crypt + 4);

			/* pbOutBuffer、pbInBuffer均为8Bytes, pbKey为16Bytes */
			TeaEncryptECB(src_buf, k, pbOutBuf);

			if (0 == iAlign) { //是否在对齐的地址上
				for (j = 0; j < 8; j++) {/* 加密后异或前8个Bytes的明文(iv_plain指向的) */
					pbOutBuf[j] ^= iv_plain[j];
				}
			} else {
				*(uint32_t *)pbOutBuf ^= *(uint32_t *)iv_plain;
				*(uint32_t *)(pbOutBuf + 4) ^= *(uint32_t *)(iv_plain + 4);
			}

			/* 保存当前的iv_plain */
			*(uint32_t *)iv_plain = *(uint32_t *)src_buf;
			*(uint32_t *)(iv_plain + 4) = *(uint32_t *)(src_buf + 4);

			src_i = 0;
			iv_crypt = pbOutBuf;
			*pdwOutBufLen += 8;
			pbOutBuf += 8;
		}
	}

	/* src_i指向src_buf下一个位置 */

	for (i = 1; i <= ZERO_LEN;) {
		if (src_i<8) {
			src_buf[src_i++] = 0;
			i++; /* i inc in here */
		}

		if (8 == src_i) {
			/* 8 == src_i */
			*(uint32_t *)src_buf ^= *(uint32_t *)iv_crypt; /* 加密前异或前8个Bytes的密文(iv_crypt指向的) */
			*(uint32_t *)(src_buf + 4) ^= *(uint32_t *)(iv_crypt + 4);

			/* pbOutBuffer、pbInBuffer均为8Bytes, pbKey为16Bytes */
			TeaEncryptECB(src_buf, k, pbOutBuf);

			if (0 == iAlign) { //是否在对齐的地址上
				for (j = 0; j < 8; j++) {/* 加密后异或前8个Bytes的明文(iv_plain指向的) */
					pbOutBuf[j] ^= iv_plain[j];
				}
			} else {
				*(uint32_t *)pbOutBuf ^= *(uint32_t *)iv_plain;
				*(uint32_t *)(pbOutBuf + 4) ^= *(uint32_t *)(iv_plain + 4);
			}

			/* 保存当前的iv_plain */
			*(uint32_t *)iv_plain = *(uint32_t *)src_buf;
			*(uint32_t *)(iv_plain + 4) = *(uint32_t *)(src_buf + 4);

			src_i = 0;
			iv_crypt = pbOutBuf;
			*pdwOutBufLen += 8;
			pbOutBuf += 8;
		}
	}

	return 0;
}


/*pbKey为16Bytes*/
/*
	输入:pbInBuf为密文格式,dwInBufLen为pbInBuf的长度是8Bytes的倍数; *pbOutBufLen为接收缓冲区的长度
		特别注意*pbOutBufLen应预置接收缓冲区的长度!
	输出:pbOutBuf为明文(Body),pbOutBufLen为pbOutBuf的长度,至少应预留dwInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1Bytes) + Padding(var,0-7Bytes) + Salt(2Bytes) + Body(var Bytes) + Zero(7Bytes) */
int TeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	uint8_t dest_buf[8], zero_buf[8];
	const uint8_t * iv_pre_crypt, * iv_cur_crypt;
	int dest_i, i, j, iAlign;
	const uint8_t * pbInBufBoundary;
	uint32_t k[4], dwPadlen, dwPlainLen, dwBufPos = 0;

	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (dwInBufLen & 7 || dwInBufLen < 16) {
		return -1;
	}

	for (i = 0; i < 4; i++) {
		/* key is TCP/IP-endian; */
		k[i] = ntohl(*(const uint32_t*)(pbKey + i * 4));
	}

	TeaDecryptECB(pbInBuf, k, dest_buf);

	dwPadlen = dest_buf[0] & 0x7/* 只要最低三位 */;

	/* 密文格式:PadLen(1Bytes) + Padding(var,0-7Bytes) + Salt(2Bytes) + Body(var Bytes) + Zero(7Bytes) */
	i = dwInBufLen - 1/*PadLen(1Bytes) */ - dwPadlen - SALT_LEN - ZERO_LEN; /* 明文长度 */
	if ((int)*pdwOutBufLen < i || i < 0) {
		return -1;
	}

	if ((long)pbInBuf & 3) {
		iAlign = 0;
	} else {
		iAlign = 1;
	}

	*pdwOutBufLen = i;

	pbInBufBoundary = pbInBuf + dwInBufLen; /* 输入缓冲区的边界，下面不能pbInBu f>= pbInBufBoundary */

	*(uint32_t *)zero_buf = 0;
	*(uint32_t *)(zero_buf + 4) = 0;

	iv_pre_crypt = zero_buf;
	iv_cur_crypt = pbInBuf; /* init iv */

	pbInBuf += 8;
	dwBufPos += 8;

	dest_i = 1; /* dest_i指向dest_buf下一个位置 */

	/* 把Padding滤掉 */
	dest_i += dwPadlen;

	/* dest_i must <= 8 */

	/* 把Salt滤掉 */
	for (i = 1; i <= SALT_LEN;) {
		if (dest_i < 8) {
			dest_i++;
			i++;
		} else if (8 == dest_i) {
			/* 解开一个新的加密块 */
			/* 改变前一个加密块的指针 */
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pbInBuf; 

			if ((dwBufPos + 7) >= dwInBufLen) {
				return -1;
			}

			/* 异或前一块明文(在dest_buf[]中) */
			if (0 == iAlign) {
				for (j = 0; j < 8; j++) {
					dest_buf[j] ^= pbInBuf[j];
				}
			} else {
				*(uint32_t *)dest_buf ^= *(const uint32_t *)pbInBuf;
				*(uint32_t *)(dest_buf + 4) ^= *(const uint32_t *)(pbInBuf + 4);
			}

			/* 8 == dest_i */
			TeaDecryptECB(dest_buf, k, dest_buf);

			/* 在取出的时候才异或前一块密文(iv_pre_crypt) */
			pbInBuf += 8;
			dwBufPos += 8;

			dest_i = 0; /* dest_i指向dest_buf下一个位置 */
		}
	}

	/* 还原明文 */

	dwPlainLen = *pdwOutBufLen;
	while (dwPlainLen) {
		if (dest_i < 8) {
			*(pbOutBuf++) = dest_buf[dest_i] ^ iv_pre_crypt[dest_i];
			dest_i++;
			dwPlainLen--;
		} else if (8 == dest_i) {
			/*8 == dest_i */

			/* 改变前一个加密块的指针 */
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pbInBuf; 

			/*解开一个新的加密块*/
			if ((dwBufPos + 7) >= dwInBufLen) {
				return -1;
			}

			/* 异或前一块明文(在dest_buf[]中) */
			if (0 == iAlign) {
				for (j = 0; j < 8; j++) {
					dest_buf[j] ^= pbInBuf[j];
				}
			} else {
				*(uint32_t *)dest_buf ^= *(const uint32_t *)pbInBuf;
				*(uint32_t *)(dest_buf + 4) ^= *(const uint32_t *)(pbInBuf + 4);
			}

			TeaDecryptECB(dest_buf, k, dest_buf);

			/*在取出的时候才异或前一块密文(iv_pre_crypt)*/

			pbInBuf += 8;
			dwBufPos += 8;

			dest_i = 0; /* dest_i指向dest_buf下一个位置 */
		}
	}

	/* 校验Zero */
	for (i = 1;i <= ZERO_LEN;) {
		if (dest_i < 8) {
			if (dest_buf[dest_i] ^ iv_pre_crypt[dest_i]) {
				return -1;
			}

			dest_i++;
			i++;
		}
		else if (8 == dest_i) {
			/* 改变前一个加密块的指针 */
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pbInBuf; 

			/*解开一个新的加密块*/
			if ((dwBufPos + 7) >= dwInBufLen) {
				return -1;
			}

			/* 异或前一块明文(在dest_buf[]中) */
			if (0 == iAlign) {
				for (j = 0; j < 8; j++) {
					dest_buf[j] ^= pbInBuf[j];
				}
			} else {
				*(uint32_t *)dest_buf ^= *(const uint32_t *)pbInBuf;
				*(uint32_t *)(dest_buf + 4) ^= *(const uint32_t *)(pbInBuf + 4);
			}

			TeaDecryptECB(dest_buf, k, dest_buf);

			/*在取出的时候才异或前一块密文(iv_pre_crypt)*/

			pbInBuf += 8;
			dwBufPos += 8;
			dest_i = 0; /* dest_i指向dest_buf下一个位置 */
		}
	}

	return 0;
}


// end of file
