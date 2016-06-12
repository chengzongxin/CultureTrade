//
//  JsonConvert.h
//  FT Trader
//
//  Created by SZFT1 on 14/11/28.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//


#ifndef __FT_Trader__JsonConvert__
#define __FT_Trader__JsonConvert__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


//#include <string>
//#include "DataDef.h"
//using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif
    
    #include "cJSON.h"
   
    
    void cJSON_AddStringToObjectEx(cJSON * root, int tag, const char *content);

    void cJSON_AddDoubleToObjectEx(cJSON * root, int tag, double content);

    cJSON * cJSON_GetObjectItemEx(cJSON * root, int tag);

    void convert_declare_from_jsonnode(cJSON* order, char * fder);
    
    void convert_symbol_from_jsonnode(int symbolType, char* sysbolParam, cJSON* symbol);
    
    void convert_order_from_jsonnode(int orderType, cJSON* json, char* order);
    
    void convert_delivery_store_info_from_jsonnode(cJSON* cjson, const char* info);
    
    void convert_in_store_order_from_jsonnode(cJSON* cjson, const char* info);
    
    void convert_delivery_info_from_jsonnode(cJSON* cjson, const char* info);
    
        void convert_funds_flow_info_from_jsonnode(cJSON* cjson, const char* info);
        void convert_monery_inout_info_from_jsonnode(cJSON* cjson, const char* info);
#ifdef __cplusplus
}
#endif

#endif /* defined(__FT_Trader__JsonConvert__) */
