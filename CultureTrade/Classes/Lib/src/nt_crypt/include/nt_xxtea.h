#ifndef __NT_XXTEA_H__
#define __NT_XXTEA_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"


//º”√‹
int XXTeaEncrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);

//Ω‚√‹
int XXTeaDecrypt(const uint8_t * pbInBuf, uint32_t dwInBufLen, const uint8_t * pbKey, uint8_t * pbOutBuf, uint32_t * pdwOutBufLen);


#ifdef __cplusplus
}
#endif


#endif // __NT_XXTEA_H__

// end of file
