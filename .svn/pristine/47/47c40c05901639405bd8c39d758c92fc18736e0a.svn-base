#include <math.h>
#include "Buffer.h"

//#pragma once



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBuffer::CBuffer()
{
	// Initial size
	m_nSize = 0;
	m_pPtr = m_pBase = NULL;
}

CBuffer::~CBuffer()
{
	if (NULL != m_pBase) {
		delete[] m_pBase;
		m_pBase = NULL;
	}
}


uint32_t CBuffer::GetMemSize() 
{
	return m_nSize;
}


int32_t CBuffer::GetBufferLen() 
{
	if (NULL == m_pBase) {
		return 0;
	}

	int nSize = (int)(m_pPtr - m_pBase);
	return nSize;
}


int CBuffer::Write(const uint8_t * pData, uint32_t nSize)
{
	if (nSize > 0) {
		ReAllocateBuffer(nSize + GetBufferLen());
		memcpy(m_pPtr, pData, nSize);
		m_pPtr += nSize;
	}

	return nSize;
}


uint32_t CBuffer::ReAllocateBuffer(uint32_t nRequestedSize)
{
	if (nRequestedSize <= GetMemSize()) {
		return 0;
	}

	// Allocate new size
	uint32_t nNewSize = (uint32_t)ceil(nRequestedSize / 1024.0) * 1024;
	// New Copy Data Over
	uint8_t * pNewBuffer = new uint8_t[nNewSize];
	uint32_t nBufferLen = GetBufferLen();
	memcpy(pNewBuffer, m_pBase, nBufferLen);

	if (NULL != m_pBase) {
		delete[] m_pBase;
		m_pBase = NULL;
	}

	m_pBase = pNewBuffer;
	m_pPtr = m_pBase + nBufferLen;
	m_nSize = nNewSize;

	return m_nSize;
}


uint32_t CBuffer::DeAllocateBuffer(uint32_t nRequestedSize)
{	
	uint32_t nBufferLen = GetBufferLen();
	if (nRequestedSize <= nBufferLen) {
		return 0;
	}

	// Allocate new size
	uint32_t nNewSize = (uint32_t) ceil(nRequestedSize / 1024.0) * 1024;
	if (nNewSize <= GetMemSize()) {
		return 0;
	}

	// New Copy Data Over
	uint8_t * pNewBuffer = new uint8_t[nNewSize];
	memcpy(pNewBuffer, m_pBase, nBufferLen);

	if(NULL != m_pBase) {
		delete[] m_pBase;
		m_pBase = NULL;
	}

	m_pBase = pNewBuffer;
	m_pPtr = m_pBase + nBufferLen;
	m_nSize = nNewSize;

	return m_nSize;
}


int CBuffer::Insert(const uint8_t * pData, uint32_t nSize)
{
	if(nSize > 0) {
		ReAllocateBuffer(nSize + GetBufferLen());

		memmove(m_pBase+nSize, m_pBase, GetBufferLen());
		memcpy(m_pBase, pData, nSize);

		m_pPtr += nSize;
	}

	return nSize;
}


uint32_t CBuffer::Read(uint8_t * pData, uint32_t nSize)
{
	if (nSize > 0) {
		if (nSize > GetMemSize()) {
			return 0;
		}

		if (nSize > GetBufferLen()) {
			nSize = GetBufferLen();
		}

		memcpy(pData, m_pBase, nSize);
		memmove(m_pBase, m_pBase + nSize, GetBufferLen() - nSize);
		m_pPtr -= nSize;

		DeAllocateBuffer(GetBufferLen());
	}

	return nSize;
}


uint32_t CBuffer::Delete(uint32_t nSize)
{	
	if (nSize > 0) {
		if (nSize > GetMemSize()) {
			return 0;
		}

		if (nSize > GetBufferLen()) {
			nSize = GetBufferLen();
		}

		memmove(m_pBase, m_pBase + nSize, GetBufferLen() - nSize);
		m_pPtr -= nSize;

		DeAllocateBuffer(GetBufferLen());
	}

	return nSize;
}


void CBuffer::ClearBuffer()
{
	// Force the buffer to be empty
	m_pPtr = m_pBase;
	DeAllocateBuffer(1024);
}


//////////////////////////////////////////////////////////////////////////////////////

int CBuffer::Scan(const uint8_t * pScan,uint32_t nPos)
{
	if (nPos > GetBufferLen()) {
		return -1;
	}

	uint8_t * pStr = (uint8_t *)strstr((char *)(m_pBase + nPos), (const char *)pScan);
	
	int nOffset = 0;

	if (NULL != pStr) {
		nOffset = (int)((pStr - m_pBase) + strlen((const char *)pScan));
	}

	return nOffset;
}


int CBuffer::Write(const string & strData)
{
	int nSize = strData.length();
	return Write((const uint8_t *)strData.c_str(), nSize);
}


int CBuffer::Insert(const string & strData)
{
	int nSize = strData.length();
	return Insert((const uint8_t *)strData.c_str(), nSize);
}

void CBuffer::Copy(CBuffer & buffer)
{
	if (this != &buffer ) {
		int nReSize = buffer.GetMemSize();
		int nSize = buffer.GetBufferLen();
		ClearBuffer();
		ReAllocateBuffer(nReSize);
		m_pPtr = m_pBase + nSize;
		memcpy(m_pBase, buffer.GetBuffer(), buffer.GetBufferLen());
	}

	return;
}


uint8_t * CBuffer::GetBuffer(uint32_t nPos)
{
	return m_pBase + nPos;
}


void CBuffer::FileWrite(const string & strFileName)
{
	return;
}

// end of file
