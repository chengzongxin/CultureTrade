//
//  DataEngineWrapper.cpp
//  FT Trader
//
//  Created by SZFT1 on 14/11/25.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#include "DataEngineWrapper.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
    
    #include "DataEngine.h"
    
    DataEngine* mDataEngine;
    EngineEvents events;
    void instanceDataEngine(EngineEvents e) {
        if(nullptr == mDataEngine)
        mDataEngine = new DataEngine;
        events = e;
    }
    
    void process_event() {
        mDataEngine->process_event();
    }
    
    
#ifdef __cplusplus
};
#endif