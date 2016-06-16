#include "nt_crypt.h"
#include "nt_errno.h"
#include "nt_macro.h"

#include "nt_tea.h"
#include "nt_xxtea.h"
#include "nt_rc4.h"
#include "nt_3des.h"
#include "nt_aes.h"

#define DELTA		0x6e686274 //nhbt
// delta = (sqrt(5) - 1) / 2 * 2^32, (sqrt(5) - 1) / 2 �ǻƽ�ָ���ֵ.
#define ROUNDS		16
#define LOG_ROUNDS	4 // LOG_ROUNDS = log2(ROUNDS)
#define INIT_SUM	(uint32_t)(DELTA << LOG_ROUNDS)


void as_tea_encrypt(const uint8_t * pbInBuf, const uint32_t * k, uint8_t * pbOutBuf)
{
    uint32_t i, v0, v1, sum;
    
    v0 = ntohl(*(const uint32_t *)pbInBuf);
    v1 = ntohl(*(const uint32_t *)(pbInBuf + 4));
    sum = 0;
    
    for (i = 0; i < ROUNDS; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += DELTA;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
    }
    
    *(uint32_t *)pbOutBuf = htonl(v0);
    *(uint32_t *)(pbOutBuf + 4) = htonl(v1);
    
    /* now encrypted buf is TCP/IP-endian; */
    return;
}


void as_tea_decrypt(const uint8_t * pbInBuf, const uint32_t * k, uint8_t * pbOutBuf)
{
    uint32_t i, v0, v1, sum;
    
    v0 = ntohl(*(const uint32_t*)pbInBuf);
    v1 = ntohl(*(const uint32_t*)(pbInBuf + 4));
    sum = INIT_SUM;
    
    for (i = 0; i < ROUNDS; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
        sum -= DELTA;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    
    *(uint32_t *)pbOutBuf = htonl(v0);
    *(uint32_t *)(pbOutBuf + 4) = htonl(v1);
    
    /* now plain-text is TCP/IP-endian; */
    return;
}


uint32_t ASTeaEncryptLen(uint32_t dwInBufLen)
{
    uint32_t dwPadLen = dwInBufLen & 7;
    if (0 == dwPadLen) {
        return dwInBufLen + 8; // ��ǰ��4�ֽڱ�ʾ����,�����4�ֽ���У��
    }
    
    return dwInBufLen + 16 - dwPadLen;
}


int ASTeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
    uint32_t i, k[4], dwLen, dwCycle, dwOffset;
    uint8_t * pbData, * pbTemp;
    
    assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);
    
    dwLen = ASTeaEncryptLen(dwInBufLen);
    if (*pdwOutBufLen < dwLen || 0 == dwInBufLen) { // buffer���Ȳ�������û������
        return -1;
    }
    
    // ת����Կ
    for (i = 0; i < 4; i++) {
        /* now key is TCP/IP-endian; */
        k[i] = ntohl(*(const uint32_t *)(pbKey + i * 4));
    }
    
    pbData = (uint8_t *)malloc(dwLen); // ������ʱ�ڴ�
    
    // ���漸�д��������һ���°�(Len + Data + Crc + ...)
    pbTemp = pbData;
    *(uint32_t *)pbTemp = htonl(dwInBufLen); // ��ǰ��4���ֽڱ�����Ч���ݳ���(������)
    pbTemp += sizeof(uint32_t);
    memcpy(pbTemp, pbInBuf, dwInBufLen); // ��ʵ�ʵ�����׷���ں���
    pbTemp += dwInBufLen;
    *(uint32_t *)pbTemp = htonl(~dwInBufLen); // У��������(�ȳ�����򻯰汾,�����Ȱ�λȡ��)
    
    dwCycle =  dwLen >> 3; // ����8
    for (i = 0; i < dwCycle; i++) { // ÿ�ּ���8�ֽ�
        dwOffset = i << 3; // ����8
        as_tea_encrypt(pbData + dwOffset, k, pbOutBuf + dwOffset); // ÿ�μ���8�ֽ�
    }
    
    *pdwOutBufLen = dwLen;
    free(pbData); // �ͷ���ʱ�ڴ�
    
    return 0;
}


int ASTeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen)
{
    uint32_t i, k[4], dwLen, dwCycle, dwOffset;
    uint8_t * pbData, * pbTemp;
    
    assert(pbInBuf && pbKey && pbOutBuf && pdwOutBufLen);
    
    if (0 != (dwInBufLen & 7) || dwInBufLen <= 8) { // ���ȱ�����8���������Ҵ���8
        return -1;
    }
    
    for (i = 0; i < 4; i++) {
        /* now key is TCP/IP-endian; */
        k[i] = ntohl(*(const uint32_t *)(pbKey + i * 4));
    }
    
    pbData = (uint8_t *)malloc(dwInBufLen); // ������ʱ�ڴ�
    
    dwCycle =  dwInBufLen >> 3; // ����8
    for (i = 0; i < dwCycle; i++) { // ÿ�ּ���8�ֽ�
        dwOffset = i << 3; // ����8
        as_tea_decrypt(pbInBuf + dwOffset, k, pbData + dwOffset); // ÿ�μ���8�ֽ�
    }
    
    // ���漸�д����ǲ��(Len + Data + Crc + '\0'...)
    pbTemp = pbData;
    dwLen = ntohl(*(uint32_t *)pbTemp); // ǰ��4���ֽڱ�����Ч���ݳ���(������)
    pbTemp += sizeof(uint32_t);
    
    if (dwLen < dwInBufLen - 16 // �����Ǵ����
        || dwLen > dwInBufLen - 8) { // ȡ��У���벢������
        return -2; // ����ʧ��
    }
    
    if (~dwLen != ntohl(*(uint32_t *)(pbTemp + dwLen))) { // ȡ��У���벢������
        return -3; // ����ʧ��
    }
    
    memcpy(pbOutBuf, pbTemp, dwLen); // ��ʵ�ʵ����ݿ��������������
    *pdwOutBufLen = dwLen; /* init OutBufLen */
    free(pbData); // �ͷ���ʱ�ڴ�
    
    return 0;
}

int SymEncrypt(int32_t iAlgo, const void * pInBuf, uint32_t dwInBufLen,
               const void * pKey, void * pOutBuf, uint32_t * pdwOutBufLen)
{
    if (NULL == pInBuf || NULL == pOutBuf || NULL == pdwOutBufLen) {
        return E_BAD_PARAM;
    } else if (CK_U32_SOF(dwInBufLen) || 0 == dwInBufLen) {
        return E_CK_SOF;
    }
    
    if (CA_NO_ENC == iAlgo) { // pKey����ΪNULL
        *pdwOutBufLen = dwInBufLen;
        memcpy(pOutBuf, pInBuf, *pdwOutBufLen);
        return 0;
    } else {
        if (NULL == pKey) {
            return E_BAD_PARAM;
        }
        
        if (CA_TEA == iAlgo) {
            return TeaEncrypt((const uint8_t *)pInBuf, dwInBufLen,
                              (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
        } else if (CA_XXTEA == iAlgo) {
            return XXTeaEncrypt((const uint8_t *)pInBuf, dwInBufLen,
                                (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
        } else if (CA_RC4 == iAlgo) {
            return RC4((const uint8_t *)pInBuf, dwInBufLen,
                       (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
        } else if (CA_3DES == iAlgo) {
            return DesEncrypt((const uint8_t *)pInBuf, dwInBufLen,
                              (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
        } else if (CA_AES == iAlgo) {
            return AesEncrypt((const uint8_t *)pInBuf, dwInBufLen,
                              (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
        } else if (CA_ASTEA == iAlgo) {
            return ASTeaEncrypt((const uint8_t *)pInBuf, dwInBufLen,
                                (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
        }
    }
    
    return E_BAD_CRYPT_ALGO;
}

int SymDecrypt(int32_t iAlgo, const void * pInBuf, uint32_t dwInBufLen,
               const void * pKey, void * pOutBuf, uint32_t * pdwOutBufLen)
{
    if (NULL == pInBuf || NULL == pKey || NULL == pOutBuf || NULL == pdwOutBufLen) {
        return E_BAD_PARAM;
    } else if (CK_U32_SOF(dwInBufLen) || 0 == dwInBufLen) {
        return E_CK_SOF;
    }
    
    if (CA_NO_ENC == iAlgo) {
        *pdwOutBufLen = dwInBufLen;
        memcpy(pOutBuf, pInBuf, *pdwOutBufLen);
        return 0;
    } else if (CA_TEA == iAlgo) {
        return TeaDecrypt((const uint8_t *)pInBuf, dwInBufLen,
                          (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
    } else if (CA_XXTEA == iAlgo) {
        return XXTeaDecrypt((const uint8_t *)pInBuf, dwInBufLen,
                            (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
    } else if (CA_RC4 == iAlgo) {
        return RC4((const uint8_t *)pInBuf, dwInBufLen,
                   (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
    } else if (CA_3DES == iAlgo) {
        return DesDecrypt((const uint8_t *)pInBuf, dwInBufLen,
                          (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
    } else if (CA_AES == iAlgo) {
        return AesDecrypt((const uint8_t *)pInBuf, dwInBufLen,
                          (const uint8_t *)pKey, CRYPT_KEY_LEN, (uint8_t *)pOutBuf, pdwOutBufLen);
    } else if (CA_ASTEA == iAlgo) {
        return ASTeaDecrypt((const uint8_t *)pInBuf, dwInBufLen,
                            (const uint8_t *)pKey, (uint8_t *)pOutBuf, pdwOutBufLen);
    }
    
    return E_BAD_CRYPT_ALGO;
}


// end of file

