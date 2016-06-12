//
//  tradebizUtils.c
//  FT Trader
//
//  Created by SZFT1 on 15/1/6.
//  Copyright (c) 2015年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#include "tradebizUtils.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>

#ifndef __ANDROID__
	#include <regex.h>
#endif

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
    
    int itobs(int n,char * ps){
        int i;
        static int size = 8 * sizeof(int);
        for(i=size-1;i>=0;i--,n >>=1)
            ps[i]=(01 & n) + '0';
        ps[size]='\0';
        return 1;
    }
void longlong2String(char* szValue, int iSize, long long value){
	memset(szValue, 0, iSize);
	sprintf(szValue, "%lld", value);
}
void double2String(char* szValue, int iSize,double value, int decimal) {
	memset(szValue, 0, iSize);
	switch(decimal) {
		case 1: sprintf(szValue, "%.1f", value); break;
		case 2: sprintf(szValue, "%.2f", value); break;
		case 3: sprintf(szValue, "%.3f", value); break;
		case 4: sprintf(szValue, "%.4f", value); break;
		case 5: sprintf(szValue, "%.5f", value); break;
		case 6: sprintf(szValue, "%.6f", value); break;
		case 7: sprintf(szValue, "%.7f", value); break;
		case 8: sprintf(szValue, "%.8f", value); break;
		case 9: sprintf(szValue, "%.9f", value); break;
		case 10: sprintf(szValue, "%.10f", value); break;
		default: sprintf(szValue, "%.f", value); break;
	}
}
    
    int isPhone(const char* str)
    {
    	#ifndef __ANDROID__
        // 正则表示式提取
        const char* pattern_date = "(([0-9]{11})|((400|800)([0-9\\-]{7,10})|(([0-9]{4}|[0-9]{3})(-| )?)?([0-9]{7,8})((-| |转)*([0-9]{1,4}))?))";
        int z = 0;
        regex_t reg_date, reg_time;
        regmatch_t pm_date[1], pm_time[1];
        regcomp(&reg_date, pattern_date, REG_EXTENDED);
        z = regexec(&reg_date, str, 1, pm_date, 0);
        regfree(&reg_date);
        if(0 != z) {
            return -1;
        }
        #endif
        return 0;  
    }
    
    
///4舍5入函数
double Rounding(double f, int lDec)
{
    int lDirection = (f >= 0) ? 1 : (-1);
    long long lReturn = (long long)( (lDirection * f) * pow((double)10 , (double)(lDec + 1)) + 5) /10;
    return (double)lReturn / pow((double)10, (double)lDec) * lDirection;
}

bool isValidShenFenzheng(const char* str)
{
    bool bRet = false;
    int iAllCount = 0;
    int iIndex = 0;
    char ids[20];
    strcpy(ids, str);
    int iANum[] = {1, 0, 'X', 9, 8, 7, 6, 5, 4, 3, 2};
    do {
        if (str == NULL || strlen(str) != 18)break;
        for (int i = 0; i < 17; i++)
        {
            char tmp[2];
            memset( tmp, 0, sizeof(tmp));
            tmp[0] = ids[i];
            iAllCount += (atoi(tmp)*((int)pow((double)(2.0),(double)(17-i))%11));
        }
        iIndex = iAllCount%11;
        
        char tmp[2];
        memset( tmp, 0, sizeof(tmp));
        tmp[0] = ids[17];
        if ((iIndex == 2 && ids[17] != 'X') !=0 || (iIndex != 2 && iANum[iIndex] != atoi(tmp)))break;
        bRet = true;
    } while (0);
    return bRet;
}
#ifdef __cplusplus
}
#endif