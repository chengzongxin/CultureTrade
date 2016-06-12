#ifndef __NT_CRYPT_H__
#define __NT_CRYPT_H__


#ifdef __cplusplus
extern "C" {
#endif
    
    
#include "nt_types.h"
    
    
#define CRYPT_KEY_LEN		16
    
    
    typedef enum {
        CA_NO_ENC = 0,
        CA_TEA = 1,
        CA_XXTEA,
        CA_RC4,
        CA_3DES,
        CA_AES, // 5
        CA_ASTEA,
        CA_RSA, // 暂不实现
    } CA_TYPE;
    
    
    
    int SymEncrypt(int32_t iAlgo, const void * pInBuf, uint32_t dwInBufLen,
                   const void * pKey, void * pOutBuf, uint32_t * pdwOutBufLen);
    
    int SymDecrypt(int32_t iAlgo, const void * pInBuf, uint32_t dwInBufLen,
                   const void * pKey, void * pOutBuf, uint32_t * pdwOutBufLen);
    
    
    
    uint32_t ASTeaEncryptLen(uint32_t dwInBufLen);
    
    
    int ASTeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);
    
    
    int ASTeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);
    
    
    /*
     //计算NtPkgHead的crc值
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
     */
    
#ifdef __cplusplus
}
#endif


#endif // __NT_CRYPT_H__

// end of file

