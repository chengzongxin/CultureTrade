#include "nt_pack.h"
#include "nt_errno.h"
#include "nt_macro.h"


int AddByte(uint8_t **ppCur, uint32_t *pdwLeft, uint8_t bValue)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(bValue)) {
			return E_OVERFLOW;
		}
	}

	**(uint8_t **)ppCur = bValue;
	++ *ppCur;

	if (pdwLeft) {
		-- *pdwLeft;
	}

	return 0;
}


static int addWord(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(wValue)) {
			return E_OVERFLOW;
		}
	}

	if (iNetByteOrder) {
		wValue = htons(wValue);
	}

	memcpy(*ppCur, &wValue, sizeof(wValue));
	*ppCur += sizeof(wValue);

	if (pdwLeft) {
		*pdwLeft -= sizeof(wValue);
	}

	return 0;
}


static int addDWord(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(dwValue)) {
			return E_OVERFLOW;
		}
	}

	if (iNetByteOrder) {
		dwValue = htonl(dwValue);
	}
	
	memcpy(*ppCur, &dwValue, sizeof(dwValue));
	*ppCur += sizeof(dwValue);

	if (pdwLeft) {
		*pdwLeft -= sizeof(dwValue);
	}

	return 0;
}


static int addQWord(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(qwValue)) {
			return E_OVERFLOW;
		}
	}

	if (iNetByteOrder) {
		qwValue = htonll(qwValue);
	}
	
	memcpy(*ppCur, &qwValue, sizeof(qwValue));
	*ppCur += sizeof(qwValue);

	if (pdwLeft) {
		*pdwLeft -= sizeof(qwValue);
	}

	return 0;
}


int AddBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen)
{
	assert((ppCur && *ppCur));
	assert((pBuf));
	
	if (dwBufLen) {
		if (CK_U32_SOF(dwBufLen)) {
			return E_BAD_PARAM;
		}

		if (pdwLeft) {
			if (CK_U32_SOF(*pdwLeft)) {
				return E_BAD_PARAM;
			}

			if (*pdwLeft < dwBufLen) {
				return E_OVERFLOW;
			}
		}

		memcpy(*ppCur, pBuf, dwBufLen);
		*ppCur += dwBufLen;

		if (pdwLeft) {
			*pdwLeft -= dwBufLen;
		}
	}

	return 0;
}

static int addWLenBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen, int iNetByteOrder)
{
	assert((ppCur && *ppCur));
	assert((pBuf));

	if (CK_U32_SOF(dwBufLen)) {
		return E_BAD_PARAM;
	}
	
	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(uint16_t) + dwBufLen) {
			return E_OVERFLOW;
		}
	}

	CHECK_RET(addWord(ppCur,pdwLeft, (uint16_t)dwBufLen, iNetByteOrder));

	return AddBuf(ppCur, pdwLeft, pBuf, dwBufLen);
}


int GetByte(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pbValue)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(*pbValue)) {
			return E_UNDERFLOW;
		}
	}

	if (pbValue) {
		*pbValue = **(const uint8_t **)ppCur;
	}

	++ *ppCur;
	
	if(pdwLeft) {
		-- *pdwLeft;
	}

	return 0;
}


static int getWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(*pwValue)) {
			return E_UNDERFLOW;
		}
	}

	if (pwValue) {
		memcpy(pwValue, *ppCur, sizeof(*pwValue));
		if (iNetByteOrder) {
			*pwValue = htons(*pwValue);
		}
	}

	*ppCur += sizeof(*pwValue);
	if (pdwLeft) {
		*pdwLeft -= sizeof(*pwValue);
	}

	return 0;
}


static int getDWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}

		if (*pdwLeft < sizeof(*pdwValue)) {
			return E_UNDERFLOW;
		}
	}

	if (pdwValue) {
		memcpy(pdwValue, *ppCur, sizeof(*pdwValue));
		if (iNetByteOrder) {
			*pdwValue = htonl(*pdwValue);
		}
	}

	*ppCur += sizeof(*pdwValue);
	
	if (pdwLeft) {
		*pdwLeft -= sizeof(*pdwValue);
	}

	return 0;
}


static int getQWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue, int iNetByteOrder)
{
	assert((ppCur && *ppCur));

	if (pdwLeft) {
		if (CK_U32_SOF(*pdwLeft)) {
			return E_BAD_PARAM;
		}
		
		if (*pdwLeft < sizeof(*pqwValue)) {
			return E_UNDERFLOW;
		}
	}

	if (pqwValue) {
		memcpy(pqwValue, *ppCur, sizeof(*pqwValue));
		if (iNetByteOrder) {
			*pqwValue = htonll(*pqwValue);
		}
	}

	*ppCur += sizeof(*pqwValue);
	if (pdwLeft) {
		*pdwLeft -= sizeof(*pqwValue);
	}

	return 0;
}


int GetBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint32_t dwBufLen)
{
	assert((ppCur && *ppCur));

	if (dwBufLen) {
		if (CK_U32_SOF(dwBufLen)) {
			return E_BAD_PARAM;
		}

		if (pdwLeft) {
			if (CK_U32_SOF(*pdwLeft)) {
				return E_BAD_PARAM;
			}
			
			if (*pdwLeft < dwBufLen) {
				return E_UNDERFLOW;
			}
		}

		if(pBuf) {
			memcpy(pBuf, *ppCur, dwBufLen);
		}

		*ppCur += dwBufLen;

		if (pdwLeft) {
			*pdwLeft -= dwBufLen;
		}
	}

	return 0;
}



static int getWLenBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen, int iNetByteOrder)
{
	//assert((ppCur && *ppCur));

	const uint8_t *pTmp = *ppCur;
	const uint32_t dwTmp = pdwLeft ? *pdwLeft : 0;
	uint16_t wLen = 0;

	int iRet = getWord(ppCur, pdwLeft, &wLen, iNetByteOrder);
	if (0 == iRet) {
		if (!pwBufLen) {
			iRet = GetBuf(ppCur, pdwLeft, 0, wLen);
		} else if (wLen <= *pwBufLen) {
			iRet = GetBuf(ppCur, pdwLeft, pBuf, wLen);
		} else {
			iRet = E_OVERFLOW;
		}
	}

	if (0 == iRet) {
		if (pwBufLen) {
			*pwBufLen = wLen;
		}
	} else {
		*ppCur = pTmp;
		if (pdwLeft) {
			*pdwLeft = dwTmp;
		}
	}

	return iRet;
}


static int getWLenBufPtr(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen, int iNetByteOrder)
{
	//assert((ppCur && *ppCur));
	//assert((ppBuf && pwBufLen));

	const uint8_t * pTmp = *ppCur;
	const uint32_t dwTmp = (pdwLeft ? *pdwLeft : 0);
	uint16_t wLen = 0;

	int iRet = getWord(ppCur, pdwLeft, &wLen, iNetByteOrder);
	if (0 == iRet) {
		if (!pdwLeft || (pdwLeft && wLen <= *pdwLeft)) {
			*ppBuf = *ppCur;
			*ppCur += wLen;
			*pwBufLen = wLen;
			if (pdwLeft) {
				*pdwLeft -= wLen;
			}
		} else {
			iRet = E_OVERFLOW;
		}
	}

	if (0 != iRet) {
		*ppCur = pTmp;
		if (pdwLeft) {
			*pdwLeft = dwTmp;
		}
	}

	return iRet;
}


int AddWord(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue)
{
	return addWord(ppCur, pdwLeft, wValue, 1);
}


int AddDWord(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue)
{
	return addDWord(ppCur, pdwLeft, dwValue, 1);
}


int AddQWord(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue)
{
	return addQWord(ppCur, pdwLeft, qwValue, 1);
}


int AddWLenBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen)
{
	return addWLenBuf(ppCur, pdwLeft, pBuf, dwBufLen, 1);
}


int GetWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue)
{
	return getWord(ppCur, pdwLeft, pwValue, 1);
}


int GetDWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue)
{
	return getDWord(ppCur, pdwLeft, pdwValue, 1);
}


int GetQWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue)
{
	return getQWord(ppCur, pdwLeft, pqwValue, 1);
}


int GetWLenBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen)
{
	return getWLenBuf(ppCur, pdwLeft, pBuf, pwBufLen, 1);
}

int GetWLenBufPtr(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen)
{
	return getWLenBufPtr(ppCur, pdwLeft, ppBuf, pwBufLen, 1);
}


int AddWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue)
{
	return addWord(ppCur, pdwLeft, wValue, 0);
}


int AddDWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue)
{
	return addDWord(ppCur, pdwLeft, dwValue, 0);
}


int AddQWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue)
{
	return addQWord(ppCur, pdwLeft, qwValue, 0);
}


int AddWLenBufH(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen)
{
	return addWLenBuf(ppCur, pdwLeft, pBuf, dwBufLen, 0);
}


int GetWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue)
{
	return getWord(ppCur, pdwLeft, pwValue, 0);
}


int GetDWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue)
{
	return getDWord(ppCur, pdwLeft, pdwValue, 0);
}


int GetQWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue)
{
	return getQWord(ppCur, pdwLeft, pqwValue, 0);
}


int GetWLenBufH(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen)
{
	return getWLenBuf(ppCur, pdwLeft, pBuf, pwBufLen, 0);
}


int GetWLenBufPtrH(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen)
{
	return getWLenBufPtr(ppCur, pdwLeft, ppBuf, pwBufLen, 0);
}


// end of file

