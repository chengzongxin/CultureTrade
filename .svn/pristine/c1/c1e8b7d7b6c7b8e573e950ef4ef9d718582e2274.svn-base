#include "nt_3des.h"


#define ENCRYPT 1
#define DECRYPT 0

// 16 sub keys
typedef uint8_t (* PSubKey)[16][48];


typedef struct {
	uint8_t sSubKey[3][16][48]; // 16圈子密钥
	uint32_t dwIs3Des; // 3次DES标志
} des_context;


// Permuted Choice 1 (PC-1)
static const uint8_t PC1_Table[56] =
{
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 
	21, 13,  5, 28, 20, 12,  4
};


// Left Shifts
static const uint8_t LOOP_Table[16] =
{
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};


// Permuted Choice 2 (PC-2)
static const uint8_t PC2_Table[48] =
{
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};


// Initial Permutation (IP)
static const uint8_t IP_Table[64] =
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};


// Expansion (E)
static const uint8_t E_Table[48] =
{
	32,  1,  2,  3,  4,  5, 
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13, 
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 
	28, 29, 30, 31, 32,  1
};


// The (in)famous S-boxes
const static uint8_t S_Box[8][4][16] =
{
	{
		// S1 
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
	},

	{
		// S2 
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
	},

	{
		// S3 
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
	},

	{
		// S4 
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
	},

	{
		// S5 
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
	},

	{
		// S6 
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
	},

	{
		// S7 
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
	},

	{
		// S8 
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
	}
};


// Permutation P
static const uint8_t P_Table[32] =
{
	16,  7, 20, 21, 
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

// Final Permutation (IP**-1)
static const uint8_t IPR_Table[64] =
{
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};



// -- xor --
static void Xor(uint8_t * InA, const uint8_t * InB, uint32_t dwLen) // 异或
{
	uint32_t i;

	for (i = 0; i < dwLen; i++) {
		InA[i] ^= InB[i];
	}

	return;
}


// -- Perform one or two circular left shifts --
static void RotateL(uint8_t * In, uint32_t dwLen, uint32_t dwLoop) // 循环左移
{
	uint8_t sTmp[256]; // Sample:

	// loop = 2
	memcpy(sTmp, In, dwLoop);    // In = 12345678 sTmp = 12
	memcpy(In, In + dwLoop, dwLen - dwLoop); // In = 345678
	memcpy(In + dwLen - dwLoop, sTmp, dwLoop); // In = 34567812

	return;
}


// Sample:
// In = [0x01]
// Out = [0x01] [0x00] [0x00] [0x00] [0x00] [0x00] [0x00] [0x00]
static void ByteToBit(uint8_t * Out, const uint8_t * In, uint32_t dwBits) // 字节组转换成位组
{
	uint32_t i;

	for (i = 0; i < dwBits; i++) {
		// In[i]的第N位右移N位并和0x01按位"与"运算(N=1~8)
		Out[i] = (In[i >> 3] >> (i & 7)) & 1;
	}

	return;
}


static void BitToByte(uint8_t * Out, const uint8_t * In, uint32_t dwBits) // 位组转换成字节组
{
	uint32_t i;

	memset(Out, 0, (dwBits + 7) >> 3);
	for (i = 0; i < dwBits; i++) {
		Out[i >> 3] |= In[i] << (i & 7);
	}

	return;
}


// -- Perform the permutation --
static void Transform(uint8_t * Out, uint8_t * In, const uint8_t * Table, uint32_t dwLen) // 变换
{
	uint32_t i;
	uint8_t sTmp[256];

	for (i = 0; i < dwLen; i++) {
		sTmp[i] = In[Table[i] - 1];
	}

	memcpy(Out, sTmp, dwLen);

	return;
}


static void S_func(uint8_t Out[32], const uint8_t In[48]) // S 盒代替
{
	uint8_t j, m, n;

	//2.4.3 Break E(R[i-1]) xor K[i] into eight 6-bit blocks. Bits 1-6 are B[1]... 
	for(j = 0; j < 8; j++, In += 6,Out += 4) {
		// 2.4.4.1 Take the 1st and 6th bits of B[j] together as a 2-bit value (call it m) indicating the row in S[j] to look in for the substitution. 
		m = (In[0] << 1) + In[5];

		// 2.4.4.2 Take the 2nd through 5th bits of B[j] together as a 4-bit value (call it n) indicating the column in S[j] to find the substitution.
		n = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];

		// 2.4.4.3 Replace B[j] with S[j][m][n]. 
		ByteToBit(Out, &S_Box[j][m][n], 4);
     }

	 return;
}


static void F_func(uint8_t In[32], const uint8_t Ki[48]) // f 函数
{
	uint8_t MR[48];

	// 2.4.1 Expand the 32-bit R[i-1] into 48 bits according to the bit-selection function below. 
	Transform(MR, In, E_Table, 48);

	// 2.4.2 Exclusive-or E(R[i-1]) with K[i]. 
	Xor(MR, Ki, 48);

	// 2.4.4 Substitute the values found in the S-boxes for all B[j]. Start with j = 1. All values in the S-boxes should be considered 4 bits wide. 
	S_func(In, MR);

	// 2.4.5 Permute the concatenation of B[1] through B[8] as indicated below. 
	Transform(In, In, P_Table, 32);

	return;
}


// == 1. Process the key ==
void DesSetSubKey(PSubKey pSubKey, const uint8_t Key[8])
{
	uint32_t i;
	uint8_t K[64], * KL = &K[0], * KR = &K[28];

	ByteToBit(K, Key, 64);

	Transform(K, K, PC1_Table, 56);

	// 1.2.3 Calculate the 16 sub keys. Start with i = 1. 
	for (i = 0; i < 16; i++) {
		// 1.2.3.1 Perform one or two circular left shifts on both C[i-1] and D[i-1] to get C[i] and D[i], respectively. 
		RotateL(KL, 28, LOOP_Table[i]);
		RotateL(KR, 28, LOOP_Table[i]);

		// 1.2.3.2 Permute the concatenation C[i]D[i] as indicated below. This will yield K[i], which is 48 bits long.
		Transform((*pSubKey)[i], K, PC2_Table, 48);
     }

	 return;
}


void DesSetKey(des_context * ctx,  const uint8_t * pbKey, uint32_t dwKeySize)
{
	uint64_t K3;

	DesSetSubKey(&ctx->sSubKey[0], pbKey);
	ctx->dwIs3Des = 16 == dwKeySize ? 1 : 0;

	if (ctx->dwIs3Des) {
		DesSetSubKey(&ctx->sSubKey[1], pbKey + 8);
		//第三轮可以与第一轮相同,我们现在取不相同的,前8字节与后8字节异或取反得到第三轮密钥
		K3 = ~(*(const uint64_t *)pbKey ^ *(const uint64_t *)(pbKey + 8));
		DesSetSubKey(&ctx->sSubKey[2], (const uint8_t *)&K3);
	}
}


// == 2. process a 64-bit data block ==
void DesProcessBlock(const uint8_t In[8], const PSubKey pSubKey, uint8_t Out[8], int32_t iIsEncrypt)
{
	int i;
	uint8_t M[64], sTmp[32], *Li = &M[0], *Ri = &M[32];

     // 2.1 Get a 64-bit data block. If the block is shorter than 64 bits, it should be padded as appropriate for the application. 
     ByteToBit(M, In, 64);
     // 2.2 Perform the following permutation on the data block. 
     Transform(M, M, IP_Table, 64);

     //2.3 Split the block into two halves. The first 32 bits are called L[0], and the last 32 bits are called R[0]. 
	 if (iIsEncrypt) {
		// 2.4 Apply the 16 sub keys to the data block. Start with i = 1. 
		for (i = 0; i < 16; i++) {
			memcpy(sTmp, Ri, 32);
			// R[i] = L[i-1] xor f(R[i-1], K[i])
			F_func(Ri, (*pSubKey)[i]);
			// 2.4.6 Exclusive-or the resulting value with L[i-1].
			// R[I]=P XOR L[I-1]
			Xor(Ri, Li, 32);
			// L[i] = R[i-1]
			memcpy(Li, sTmp, 32);
		} //2.4.8 Loop back to 2.4.1 until K[16] has been applied. 
	} else {
		for (i = 15; i >= 0; i--) {
			memcpy(sTmp, Li, 32);
			F_func(Li, (*pSubKey)[i]);
			Xor(Li, Ri, 32);
			memcpy(Ri, sTmp, 32);
		}
	}

	// 2.5 Perform the following permutation on the block R[16]L[16]. (Note that block R precedes block L this time.) 
	Transform(M, M, IPR_Table, 64);
	BitToByte(Out, M, 64);

	return;
}


// process data
void DesProcessData(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, int32_t iIsEncrypt)
{
	uint32_t i, j;
	des_context ctx;

	//输入必须8字节对齐
	dwInBufLen = (dwInBufLen + 7) & 0xfffffff8;
	DesSetKey(&ctx, pbKey, dwKeyLen);

	if (0 == ctx.dwIs3Des) {
		for (i = 0, j = dwInBufLen >> 3; i < j; ++i, pbOutBuf += 8, pbInBuf += 8) {
			DesProcessBlock(pbInBuf, &ctx.sSubKey[0], pbOutBuf, iIsEncrypt);
		}
	} else {
		//3次des加密 E(D(E(Plain, K0), K1), K2),解密 D(E(D(Cipher, K0), K1), K2)
		for (i = 0, j = dwInBufLen >> 3; i < j; ++i, pbOutBuf += 8, pbInBuf += 8) {
			DesProcessBlock(pbInBuf, &ctx.sSubKey[0], pbOutBuf, iIsEncrypt);
			DesProcessBlock(pbInBuf, &ctx.sSubKey[1], pbOutBuf, !iIsEncrypt);
			DesProcessBlock(pbInBuf, &ctx.sSubKey[2], pbOutBuf, iIsEncrypt);
		}
	}

	return;
}


// == 2. encrypt data ==
int DesEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (0 == dwInBufLen || (dwInBufLen & 7) || *pdwOutBufLen < dwInBufLen) { //长度不是8的倍数
		return -1;
	}

	*pdwOutBufLen = dwInBufLen;
	DesProcessData(pbInBuf, dwInBufLen, pbKey, dwKeyLen, pbOutBuf, 1);

	return 0;
}


// == 2. decrypt data ==
int DesDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint32_t dwKeyLen, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
	assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);

	if (0 == dwInBufLen || (dwInBufLen & 7) || *pdwOutBufLen < dwInBufLen) { //长度不是8的倍数
		return -1;
	}

	*pdwOutBufLen = dwInBufLen;
	DesProcessData(pbInBuf, dwInBufLen, pbKey, dwKeyLen, pbOutBuf, 0);

	return 0;
}

// end of file
