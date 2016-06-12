//
//
//
//////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "nethelper.h"
#include "CalcInd.h"

#define EPS (1e-5)
#define LogTrace 

#define ZeroMemory(a,b) memset(a,0,b)

//////////////////////////////////////////////////////////////////////
CCalcInd::CCalcInd()
{
	m_cParamSize = 0;
	ZeroMemory(m_psParam, 6*sizeof(short));

	m_cExpSize = 0;
	ZeroMemory(m_pnFirst, 6*sizeof(int));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCalcInd_MA::CCalcInd_MA() : CCalcInd()
{
	m_psParam[0] = 5;
	m_psParam[1] = 10;
	m_psParam[2] = 30;
	m_psParam[3] = 60;
    
    m_cParamSize = 4;
}

void CCalcInd_MA::Calc(CSHisKUnitMA* pFDay, int nNum)
{
//	printf("%s\n", __FUNCTION__);
	m_cExpSize = m_cParamSize;

	if (pFDay == NULL || nNum <= 0) return;
	for (int c=0; c < m_cParamSize; c++)
	{
		short sParam = m_psParam[c];
		if (sParam<1)
			sParam = 1;

		m_pnFirst[c] = int(sParam-1);

		for (int i=0; i<nNum; i++)
		{
			if (i<m_pnFirst[c])
				pFDay[i].m_uiMA[c] = 0;
			else if (sParam<2)
				pFDay[i].m_uiMA[c] = 0;
			else
			{
				pFDay[i].m_uiMA[c] = 0;
				for (int j=i-sParam+1; j<=i; j++)
					pFDay[i].m_uiMA[c] += pFDay[j].uiClosePrice;
				pFDay[i].m_uiMA[c] /= sParam;
//				printf("MA pFDay T=[%d] m_uiMA[%d]=[%d]\n", pFDay[i].uiTime, c, pFDay[i].m_uiMA[c]);
			}
		}
	}
}

void CCalcInd_MA::CalcMA(HistoryKUnitMA* pFDay, int nNum)
{
//    printf("%s\n", __FUNCTION__);
    m_cExpSize = m_cParamSize;
    
    if (pFDay == NULL || nNum <= 0) return;
    for (int c=0; c < m_cParamSize; c++)
    {
        short sParam = m_psParam[c];
        if (sParam<1)
            sParam = 1;
        m_pnFirst[c] = int(sParam-1);
        for (int i=0; i<nNum; i++)
        {
            if (i<m_pnFirst[c])
                pFDay[i].mMA[c] = 0;
            else if (sParam<2)
                pFDay[i].mMA[c] = 0;
            else
            {
                pFDay[i].mMA[c] = 0;
                for (int j=i-sParam+1; j<=i; j++)
                    pFDay[i].mMA[c] += pFDay[j].closePrice;
                pFDay[i].mMA[c] /= sParam;
                //				printf("MA pFDay T=[%d] m_uiMA[%d]=[%d]\n", pFDay[i].uiTime, c, pFDay[i].m_uiMA[c]);
            }
        }
    }
    
//    for (int m = 0; m < m_cParamSize; m++) {
//        for (int i=0; i<nNum; i++) {
//            if (i >= m_psParam[m] - 1) {
//                int close = 0;
//                for (int n = i; n >= (i - m_psParam[m] + 1); n--) {
//                    close += pFDay[n].closePrice;
//                }
//                close = close/m_psParam[m];
//                pFDay[i].mMA[m] = close;
//            }
//        }
//    }
}
