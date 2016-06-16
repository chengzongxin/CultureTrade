#ifndef __NT_PACK_H__
#define __NT_PACK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "nt_types.h"


//����/���� ��������

// ppCur: ����/���뻺����
// pdwLeft: ������ʣ��ռ��С�����Ϊ0����ʾ���Ա߽���

int AddByte(uint8_t **ppCur, uint32_t *pdwLeft, uint8_t bValue);
int AddWord(uint8_t **ppCur, uint32_t *pdwLeft, uint16_t wValue);
int AddDWord(uint8_t **ppCur, uint32_t *pdwLeft, uint32_t dwValue);
int AddQWord(uint8_t **ppCur, uint32_t *pdwLeft, uint64_t qwValue);

//ֱ�ӽ�pBuf���ݷ��뻺����
int AddBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen);

//�Ƚ�����dwBufLen��2�ֽڱ������������Ȼ�����pBuf����
int AddWLenBuf(uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t *pBuf, uint32_t dwBufLen);

int GetByte(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pbValue);
int GetWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint16_t *pwValue);
int GetDWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint32_t *pdwValue);
int GetQWord(const uint8_t **ppCur, uint32_t *pdwLeft, uint64_t *pqwValue);

//ֱ�Ӵӻ�������ȡ���ݵ�pBuf
//���pBufΪ0����ֻ�Ǹı�pdwLeft������������
int GetBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint32_t dwBufLen);

// pwBufLen: pBuf��������ԭʼ���ȣ������ջ�ȡ���ݵ�ʵ�ʳ���
//�Ƚ���2�ֽڵ����ݳ��ȣ�����pwBufLen��Ȼ���ȡ���ݵ�pBuf
//���pBuf��pwBufLenΪ0����ֻ�Ƕ�ȡ���Ⱥ͸ı�pdwLeft������������
int GetWLenBuf(const uint8_t **ppCur, uint32_t *pdwLeft, uint8_t *pBuf, uint16_t *pwBufLen);

// ppBuf: ָ����������ָ�룬����GetWLenBuf���ڴ渴��
// pwBufLen: ppBuf��������ʵ�ʳ���
//�Ƚ���2�ֽڵ����ݳ��ȣ��������pwBufLen��Ȼ����������ָ�����ppBuf�����轫�ڴ渴�Ƴ���
int GetWLenBufPtr(const uint8_t **ppCur, uint32_t *pdwLeft, const uint8_t **ppBuf, uint16_t *pwBufLen);

//�����Ǳ����ֽ���汾

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

