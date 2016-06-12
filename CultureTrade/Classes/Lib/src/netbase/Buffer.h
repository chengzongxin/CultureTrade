#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>

#include "nt_types.h"

using namespace std;

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER
*/

class CBuffer  
{
// Attributes
protected:
	uint8_t * m_pBase;
	uint8_t * m_pPtr;
	uint32_t m_nSize;

// Methods
protected:
	uint32_t ReAllocateBuffer(uint32_t nRequestedSize);
	uint32_t DeAllocateBuffer(uint32_t nRequestedSize);
	uint32_t GetMemSize();
public:
	void ClearBuffer();
	uint32_t Delete(uint32_t nSize);
	uint32_t Read(uint8_t * pData, uint32_t nSize);
	int Write(const uint8_t * pData, uint32_t nSize);
	int Write(const string & strData);
	int32_t GetBufferLen();
	int Scan(const uint8_t * pScan,uint32_t nPos);
	int Insert(const uint8_t * pData, uint32_t nSize);
	int Insert(const string & strData);

	void Copy(CBuffer & buffer);	

	uint8_t * GetBuffer(uint32_t nPos = 0);

	CBuffer();
	virtual ~CBuffer();

	void FileWrite(const string& strFileName);

};

#endif // __BUFFER_H__

// end of file
