#ifndef __NT_PACK_H__
#define __NT_PACK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"


//编码/解码 函数集合

// ppCur: 编码/解码缓冲区
// pdwLeft: 缓冲区剩余空间大小，如果为0，表示忽略边界检查

int AddByte(uint8_t **ppCur, uint32_t *pdwLeft, uint8_t bValue);
int AddWord(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue);
int AddDWord(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue);
int AddQWord(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue);

//直接将pBuf数据放入缓冲区
int AddBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen);

//先将长度dwBufLen以2字节编码进缓冲区，然后放入pBuf数据
int AddWLenBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen);

int GetByte(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pbValue);
int GetWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue);
int GetDWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue);
int GetQWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue);

//直接从缓冲区读取数据到pBuf
//如果pBuf为0，则只是改变pdwLeft，不复制数据
int GetBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint32_t dwBufLen);

// pwBufLen: pBuf数据区的原始长度，和最终获取数据的实际长度
//先解码2字节的数据长度，存入pwBufLen，然后读取数据到pBuf
//如果pBuf或pwBufLen为0，则只是读取长度和改变pdwLeft，不复制数据
int GetWLenBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen);

// ppBuf: 指向数据区的指针，避免GetWLenBuf的内存复制
// pwBufLen: ppBuf数据区的实际长度
//先解码2字节的数据长度，存入存入pwBufLen，然后将数据区的指针存入ppBuf，无需将内存复制出来
int GetWLenBufPtr(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen);

//以下是本地字节序版本

#define AddByteH	AddByte
int AddWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue);
int AddDWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue);
int AddQWordH(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue);

#define AddBufH		AddBuf

int AddWLenBufH(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen);

#define GetByteH	GetByte
int GetWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue);
int GetDWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue);
int GetQWordH(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue);

#define GetBufH		GetBuf

int GetWLenBufH(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen);

int GetWLenBufPtrH(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen);


#ifdef __cplusplus
}
#endif

#endif // __NT_PACK_H__

// end of file

