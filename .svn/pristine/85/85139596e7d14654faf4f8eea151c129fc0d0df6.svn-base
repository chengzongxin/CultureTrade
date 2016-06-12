//
//
//
///////////////////////////////////////////////////////////////////////
#ifndef __IND_H
#define __IND_H

#include "nethelper.h"

class CCalcInd
{
public:
	CCalcInd();
    
public:
	virtual void Calc(CSHisKUnitMA* pFDay, int nNum) = 0;

public:
	char m_cParamSize;
	short m_psParam[4];

	char m_cExpSize;
	int m_pnFirst[4];
};

class CCalcInd_MA : public CCalcInd
{
public:
    CCalcInd_MA();
public:
	virtual void Calc(CSHisKUnitMA* pFDay, int nNum);
    virtual void CalcMA(HistoryKUnitMA* pFDay, int nNum);
};

#endif
