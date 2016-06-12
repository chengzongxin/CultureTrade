//
//  tradebizUtils.h
//  FT Trader
//
//  Created by SZFT1 on 15/1/6.
//  Copyright (c) 2015年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#ifndef __FT_Trader__tradebizUtils__
#define __FT_Trader__tradebizUtils__


#ifdef __cplusplus
extern "C"
{
#endif
    int itobs(int n,char * ps);
void longlong2String(char* szValue,int iSize, long long value);

void double2String(char* szValue, int iSize,double value, int decimal);
    int isPhone(const char* str);
    double Rounding(double f, int lDec);
    bool isValidShenFenzheng(const char* str);
    
#ifdef __cplusplus
}
#endif
#endif /* defined(__FT_Trader__tradebizUtils__) */
