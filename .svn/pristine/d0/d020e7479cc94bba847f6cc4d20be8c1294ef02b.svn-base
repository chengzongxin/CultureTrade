#include "nt_rc4.h"


#define USE_LONG_RC4

/* The scheduled RC4 key */
typedef struct {
	uint32_t state[256];
	uint32_t x, y;
} RC4KEY;


void rc4ExpandKey(RC4KEY * rc4, const uint8_t * pbKey, uint32_t dwKeyLen)
{
	uint32_t x, sx, y = 0, dwKeyPos = 0;

	rc4->x = rc4->y = 0;
	for (x = 0; x < 256; x++) {
		rc4->state[x] = x;
	}

	for (x = 0; x < 256; x++) {
		sx = rc4->state[x];
		y += sx + pbKey[dwKeyPos];
		y &= 0xFF;
		rc4->state[x] = rc4->state[y];
		rc4->state[y] = sx;

		if (++dwKeyPos == dwKeyLen) {
			dwKeyPos = 0;
		}
	}

	return;
}


void rc4Crypt(RC4KEY * rc4, uint8_t * pbData, uint32_t dwLen)
{
	uint32_t sx, sy;

	while (dwLen--) {
		rc4->x++;
		rc4->x &= 0xFF;

		sx = rc4->state[rc4->x];
		rc4->y += sx;
		rc4->y &= 0xFF;

		sy = rc4->state[rc4->y];
		rc4->state[rc4->y] = sx;
		rc4->state[rc4->x] = sy;

		*pbData++ ^= rc4->state[(uint8_t)(sx + sy)];
	}

	return;
}


//加解密均使用此函数
int RC4(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	RC4KEY rc4;

	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (0 == dwInBufLen || *pdwOutBufLen < dwInBufLen) {
		return -1;
	}

	*pdwOutBufLen = dwInBufLen;
	memcpy(pbOutBuf, pbInBuf, *pdwOutBufLen);

	rc4ExpandKey(&rc4, pbKey, dwKeyLen);
	rc4Crypt(&rc4, pbOutBuf, *pdwOutBufLen);

	return 0;
}


