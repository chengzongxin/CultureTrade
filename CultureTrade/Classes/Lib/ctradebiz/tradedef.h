//
//  tradebiz.h
//  tradebiz
//
//  Created by SZFT1 on 14/12/31.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#ifndef tradebiz_tradebiz_h
#define tradebiz_tradebiz_h

#define ZS_TYPE               0x01  //做市商
#define ZYJ_TYPE              0x02  //中远期
#define STORE_TYPE            0x03  //仓库
#define ZYC_TYPE              0x04  //中立仓

#define TRADE_TYPE (1)
#define QUOTE_TYPE (2)

#define WIFI (10)

#define             KLINE_LENGHT           4096  //K线数据长度

#ifdef __cplusplus
extern "C"
{
#endif
    
//////////////金智行情//////////////////////
#pragma pack(1)
    
    /*         2.0  start            */
    
#define MARKET_NAME_ZYQ_LEN (10)
#define SYMBOL_CODE_ZYQ_LEN (10)
#define SYMBOL_NAME_ZYQ_LEN (10)
#define MAX_TRADE_TIME      (20)
#define  MAX_ZYQ_VOL_PRICE  (5)
    
    typedef struct _STUSymbolTradeTime_ZYQ
    {
        unsigned char m_ucTheDay;     // 是否是昨日, 0:今日, 1:昨日
        int m_nBeginTime;             // 交易节开始时间
        int m_nEndTime;               // 交易节结束时间
    }STUSymbolTradeTime_ZYQ;
    
    typedef struct _STUCommLenDirec
    {
        unsigned short m_lenDirec;
    }STUCommLenDirec;
    
    
    typedef struct _STUHisKIndexDirec
    {
        unsigned char m_ucHisType;  // K线裂隙
        int m_iHasNum;              // 已有数量
        int m_iReqNum;              // 请求数量
    }STUHisKIndexDirec;
    
    typedef struct _STUSymbolInfo_ZYQ
    {
        char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN+1];   // 商品代码
        char m_szSymbolName[SYMBOL_NAME_ZYQ_LEN+1];   // 商品名称
        unsigned short m_usTradeTimeIndex;              // 交易节索引 (用位表示)
        unsigned short m_usMarketType;                  // 市场类型
        unsigned char m_ucPriceUnit;                    // 价格小数位
        unsigned char m_ucTrade;                        // 是否交易 0：非交易  1：交易
        unsigned int m_uiTotalShares;                   // 流通股数(总股本数)
        long long m_iAgreeunit;                             // 4个小数
        //        STUSymbolInfo_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUSymbolInfo_ZYQ));
        //        }
    }STUSymbolInfo_ZYQ;
    
    
    typedef struct _STUMarketInfo_ZYQ
    {
        char m_szMarketName[MARKET_NAME_ZYQ_LEN + 1];   // 市场名称
        unsigned short m_usMarketType;                  // 市场类型
    }STUMarketInfo_ZYQ;
    
    
    typedef struct _STUInitMarketLenDirec_ZYQ
    {
        unsigned short m_usMarkets;
        unsigned short m_usTradeTimeNum;
        unsigned short m_usSymboNum;
        //        STUInitMarketLenDirec_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUInitMarketLenDirec_ZYQ));
        //        }
    }STUInitMarketLenDirec_ZYQ;
    
    typedef struct _STUSimpleSymbol_ZYQ
    {
        unsigned char m_ucPriceUnit;                    // 价格小数位
        unsigned short m_usMarketType;                  // 市场类型
        char m_szSymbolCode[SYMBOL_CODE_ZYQ_LEN + 1];   // 商品代码
    }STUSimpleSymbol_ZYQ;
    
    enum
    {
        RPDATA_ORDER_BY_SYMOBL,             // 商品代码
        RPDATA_ORDER_BY_NEWPRICE,           // 最新价
        RPDATA_ORDER_BY_RISEDOWM,           // 涨跌幅
        RPDATA_ORDER_BY_RISEDOWM_RATE,      // 涨跌比例
        RPDATA_ORDER_BY_TRUNOVER_QTY,       // 成交量
        RPDATA_ORDER_BY_TRUNOVER_FUNDS,     // 成交额
        RPDATA_ORDER_BY_QTY_RATE,           // 量比
        RPDATA_ORDER_BY_ORDER_RATE,         // 委比
        
        //---- 一下内部使用,请无请求, 请求也不会有回应
        RPDATA_ORDER_BY_5MIN_RISEDOWN_RATE, // 5分钟涨跌幅排名
        RPDATA_ORDER_BY_AMPLITUDE,          // 当日振幅排名
    };
    
    enum
    {
        REDATA_SEQ_TYPE_ASC =0,    // 正序 xiao -> da
        REDATA_SEQ_TYPE_DESC=1   // 逆序
    };
    
    enum
    {
        TURNOVER_DEIRC_B = 0,  // 买方成交
        TURNOVER_DEIRC_S = 1,  // 卖方成交
        TURNOVER_DEIRC_F = 2,  // 集合竞价或是其他成交
        TURNOVER_DEIRC_C = 3,  // 无成交
    };
    
#define MARKET_TYPE_ALL  0x0000   // 所有市场
    typedef  long long INT64;
    typedef struct _STUReportDataIndex_ZYQ
    {
        unsigned short m_usMarketType;              // 市场类型
        unsigned char  m_ucOrderBy;                 // 排序类型
        unsigned char  m_ucSeqType;                 // 排序(正序,逆序)
        unsigned short m_usBeginPos;                // 开始位置 ( 从1 开始)
        unsigned short m_usReqNum;                  // 请求个数
        unsigned short m_usReqDesSymoblNum;         // 指定请求个数; 如果是0,那么表示请求当前市场类型下面所有商品
        //        STUReportDataIndex_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUReportDataIndex_ZYQ));
        //        }
    }STUReportDataIndex_ZYQ;
    
    
    typedef struct _STUQuoteDateTime
    {
        int m_iTradeDay;                        // 交易日   YYYYMMDD
        INT64 m_iUpdateDataTime;                  // 更新时间 YYYYMMDDHHMMSS
    }STUQuoteDateTime;
    
    typedef struct _STUReportPublicData
    {
        unsigned int m_uiNewPrice;              //  最新价(当前价)
        unsigned int m_uiNewQty;                //  最新成交量(当前量) 2
        unsigned char m_chTurnoverDirec;        // 成交方向  0= B 1= S 2=平 3=无成交
        int m_iPriceRiseDown;                   // 涨跌幅
        int m_iPriceRiseDownRate;               // 涨跌比例 2
        unsigned int m_uiOpenPrice;             // 开盘价
        unsigned int m_uiMaxPrice;              // 最高价
        unsigned int m_uiMinPrice;              // 最低价
        unsigned int m_uiYesClosePrice;         // 昨收价
        unsigned int m_uiYesSetllPrice;         // 昨结价
        unsigned int m_uiTotalTurnoverQty;      // 总成交量
        INT64 m_iTotalTurnoverFunds;            // 总成交额
        int m_iQtyRate;                        // 量比
        int m_iOrderRate;                      // 委比
    }STUReportPublicData;
    
    typedef struct _STURealPublicDataPush
    {
        unsigned int m_uiNewPrice;          //  最新价(当前价)
        unsigned int m_uiNewQty;            //  最新成交量(当前量)
        unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平 3=无成交
        int m_iPriceRiseDown;               // 涨跌幅
        int m_iPriceRiseDownRate;           // 涨跌比例
        unsigned int m_uiTotalTurnoverQty;  // 总成交量
        INT64 m_iTotalTurnoverFunds;        // 总成交额
        int m_uiQtyRate;                    // 量比
        int m_uiOrderRate;                  // 委比
        
    }STURealPublicDataPush;
    
    typedef struct _STUCuaVolPrice
    {
        unsigned int m_uiPrice;
        unsigned int m_uiQty;
        //        STUCuaVolPrice()
        //        {
        //            m_uiPrice = 0;
        //            m_uiQty = 0;
        //        }
    }STUCuaVolPrice;
    
    typedef struct _STUNowDataVolPirce
    {
        STUCuaVolPrice m_buyVol;          // 买行情
        STUCuaVolPrice m_sellVol;         // 卖行情
    }STUNowDataVolPirce;
    
    typedef struct _STUReportDataResp_ZYQ
    {
        STUSimpleSymbol_ZYQ m_oSSymobl;     // 商品信息
        STUQuoteDateTime m_oTDateTime;
        STUReportPublicData m_oReportPublicData;
        STUNowDataVolPirce m_oQtyPriceV1;   // 一档行情
        //        STUReportDataResp_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUReportDataResp_ZYQ));
        //        }
    }STUReportDataResp_ZYQ;
    
    typedef struct _STURealDataPush_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STURealPublicDataPush m_oRealPublicData;
        STUNowDataVolPirce m_oQtyPriceV1;   // 一档行情
        
        //        STURealDataPush_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STURealDataPush_ZYQ));
        //        }
    }STURealDataPush_ZYQ;
    
    typedef struct _STURealDataDetailPush_Ext_ZYQ
    {
        int m_iOrderDiff;                      // 委差(有正副)
        unsigned int m_uiPOrderSaleQty;         // 内盘
        unsigned int m_uiPOrderBuyQty;          // 外盘
        
    }STURealDataDetailPush_Ext_ZYQ;
    
    typedef struct _STUReportDataDetailResp_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STUReportPublicData m_oReportPublicData;
        STURealDataDetailPush_Ext_ZYQ m_oExt;
        unsigned short m_usVolNum;              // 档位价格数量
        
        //        STUReportDataDetailResp_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUReportDataDetailResp_ZYQ));
        //        }
    }STUReportDataDetailResp_ZYQ;
    
    typedef struct _STURealDataDetailPush_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STURealPublicDataPush m_oRealPublicData;
        STURealDataDetailPush_Ext_ZYQ m_oExt;
        unsigned short m_usVolNum;              // 档位价格数量
        
        //        STURealDataDetailPush_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STURealDataDetailPush_ZYQ));
        //        }
        //
        //        void CopyTo(STURealDataPush_ZYQ& dst, const STUNowDataVolPirce& v1)
        //        {
        //            dst.m_oTDateTime = m_oTDateTime;
        //            dst.m_oRealPublicData = m_oRealPublicData;
        //            dst.m_oQtyPriceV1 = v1;   // 一档行情
        //        }
    }STURealDataDetailPush_ZYQ;
    
    typedef struct _STUHisKUnit_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        unsigned int m_uiOpenPrice;       // 开盘价
        unsigned int m_uiClosePrice;      // 收盘价
        unsigned int m_uiMaxPrice;        // 最高价
        unsigned int m_uiMinPrice;        // 最低价
        INT64 m_iTotalTurnoverFunds;      // 成交额
        unsigned int m_uiTrunoverQty;     // 成交量
        
        //        STUHisKUnit_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUHisKUnit_ZYQ));
        //        }
    }STUHisKUnit_ZYQ;
    
    typedef struct _STUSTKMinUnit_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        unsigned int m_uiTurnoverPrice;     // 成交价  涨跌 = 成交价 - 昨收
        unsigned int m_uiTrunoverFunds;     // 成交额 均价= 成交额/成交量
        unsigned int m_uiTrunoverQty;       // 成交量
        
        //        STUSTKMinUnit_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUSTKMinUnit_ZYQ));
        //        }
    }STUSTKMinUnit_ZYQ;
    
    
    
    typedef struct _STUTraceUnit_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        unsigned char m_chTurnoverDirec;             // 成交方向(B/S or up/down) 0= B 1= S 2=平 // TURNOVER_DEIRC_B  TURNOVER_DEIRC_S TURNOVER_DEIRC_F
        unsigned int m_uiPrice;             // 成交价格
        unsigned int m_uiQty;               // 成交数量
        
        //        STUTraceUnit_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUTraceUnit_ZYQ));
        //        }
    }STUTraceUnit_ZYQ;
    
    enum
    {
        COMPLEX_RANKING_TYPE_DAY_RISE_RATE = 1,     // 当日涨幅排名
        COMPLEX_RANKING_TYPE_DAY_DOWN_RATE,         // 当日跌幅排名
        COMPLEX_RANKING_TYPE_5MIN_RISE_RATE,        // 5分钟涨幅排名
        COMPLEX_RANKING_TYPE_5MIN_DOWN_RATE,        // 5分钟跌幅排名
        COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_TOP,    // 当日委比前几名
        COMPLEX_RANKING_TYPE_DAY_ORDER_RATE_BOTTOM, // 当日委比后几名
        COMPLEX_RANKING_TYPE_DAY_AMPLITUDE,         // 当日振幅排名
        COMPLEX_RANKING_TYPE_DAY_QTY_RATE,          // 当日量比排名
        COMPLEX_RANKING_TYPE_DAY_TRUN_FUNS,         // 当日成交金额排名
    };
    
    //
    typedef struct _STUComplexRankingHead_ZYQ
    {
        unsigned char m_ucRankingType;
        unsigned short m_usRankinkDataNum;
    }STUComplexRankingHead_ZYQ;
    
    typedef struct _STUComplexRankingData_ZYQ
    {
        STUSimpleSymbol_ZYQ m_oSSymobl;
        unsigned short m_usNewPrice;
        unsigned short m_usYesClose;
        unsigned short m_usYesSettl;
        INT64 m_iValue;
    } STUComplexRankingData_ZYQ;
    
    // 交易服务 <-> 行情服务  协议
    
    // 初始化市场
    // 功能号 INITMARKET_ZYQ =0x1a00;
    // 请求
    // 回应 SNormaHead + STUInitMarketLenDirec_ZYQ + STUMarketInfo_ZYQ[N] + STUSymbolTradeTime_ZYQ[N] + STUSymbolInfo_ZYQ[N];
    
    // 报价表
    // 功能号  REPORTDATA_DETAIL_ZYQ =0x02a0;
    // 请求
    // 回应 SNormaHead + STUNowDataDetail_ZYQ[N}
    
    // 实时行情 PUSH_REALDATA_DETAIL_ZYQ =0x0ba1;
    // 推送 SNormaHead + STUNowDataDetail_ZYQ(单个)
    
    // 开盘信号
    // 功能号 OPEN_MAKRKET_ZYQ =0x1afe;
    // 推送 SNormaHead + STUOpenQuoteInfo_ZYQ(单个)
    
    // 收盘信号
    // 功能号 CLOSE_MAKRKET_ZYQ =0x1aff;
    // 推送 SNormaHead + STUCloseQuoteInfo_ZYQ(单个)
    
    
    
    typedef struct _STUNowData_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STUSimpleSymbol_ZYQ m_oSymbolInfo;
        unsigned int m_uiYesClosePrice;     // 昨收价
        unsigned int m_uiYesSettlPrice;     // 昨结价
        unsigned int m_uiOpenPrice;         // 开盘价
        unsigned int m_uiMaxPrice;          // 最高价
        unsigned int m_uiMinPrice;          // 最低价
        unsigned int m_uiNewPrice;          // 最新价(当前价)
        unsigned int m_uiNewQty;            // 最新量
        unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平
        unsigned int m_uiQtyRate;           // 量比
        INT64 m_iTotalTrunoverQty;          // 总成交量
        INT64 m_iTotalTrunoverMoney;        // 总成交额
        INT64 m_iTotalTrunoverCount;        // 总成交笔数
        INT64 m_iTotalOrderQty;             // 订货量
        INT64 m_iTotalIssueQty;             // 总量
        INT64 m_iUnTrunoverBuy;             // 总未成交买量
        INT64 m_iUnTrunoverSell;            // 总未成交买量
        INT64 m_iVolTrunoverBuy;            // 总买(五档价格)成交量
        INT64 m_iVolTrunoverSell;           // 总卖(五档价格)成交量
        char m_cVolPriceCount;
        STUNowDataVolPirce m_arrVolPrice[MAX_ZYQ_VOL_PRICE];
        
        //        STUNowData_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUNowData_ZYQ));
        //        }
    }STUNowData_ZYQ;
    
    typedef struct _STUNowDataDetail_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STUSimpleSymbol_ZYQ m_oSymbol;
        unsigned int m_uiYesClosePrice;     // 昨收价
        unsigned int m_uiYesSettlPrice;     // 昨结价
        unsigned int m_uiOpenPrice;         // 开盘价
        unsigned int m_uiMaxPrice;          // 最高价
        unsigned int m_uiMinPrice;          // 最低价
        unsigned int m_uiNewPrice;          // 最新价(当前价)
        unsigned int m_uiNewQty;            // 最新量
        unsigned char m_chTurnoverDirec;    // 成交方向  0= B 1= S 2=平
        unsigned int m_uiSettlPrice;        // 结算价
        INT64 m_iBuyOpen;                   // 买订立
        INT64 m_iBuyClose;                  // 卖转让
        INT64 m_iSellOpen;                  // 卖订立
        INT64 m_iSellClose;                 // 买转让
        INT64 m_iTotalOrderCount;           // 总委托单数量
        INT64 m_iTotalTrunoverQty;          // 总成交量
        INT64 m_iTotalTrunoverMoney;        // 总成交额
        INT64 m_iTotalTrunoverCount;        // 总成交笔数
        INT64 m_iTotalOrderQty;             // 订货量
        INT64 m_iTotalIssueQty;            // 总量
        INT64 m_iUnTrunoverBuy;             // 总未成交买量
        INT64 m_iUnTrunoverSell;            // 总未成交买量
        INT64 m_iVolTrunoverBuy;            // 总买(五档价格)成交量
        INT64 m_iVolTrunoverSell;           // 总卖(五档价格)成交量
        char m_cVolPriceCount;
        STUNowDataVolPirce m_arrVolPrice[MAX_ZYQ_VOL_PRICE];
        
        //        STUNowDataDetail_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUNowDataDetail_ZYQ));
        //        }
    }STUNowDataDetail_ZYQ;
    
    // 开市命令
    typedef struct _STUOpenQuoteInfo_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STUSimpleSymbol_ZYQ m_oSymbol;
        unsigned int m_uiYesClosePrice;      // 昨收价
        unsigned int m_uiYesSettlPrice;      // 昨结价
        INT64  m_iTotalOrderQty;             // 订货量
        INT64  m_iTotalIssueQty;             // 总量
        
        //        STUOpenQuoteInfo_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUOpenQuoteInfo_ZYQ));
        //        }
    }STUOpenQuoteInfo_ZYQ;
    
    typedef struct _STUCloseQuoteInfo_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        int m_iNextTick;                        //下一交易日期
        
        STUSimpleSymbol_ZYQ m_oSymbol;
        unsigned int m_uiYesClosePrice;         // 昨收价
        unsigned int m_uiYesSettlPrice;         // 昨结价
        unsigned int m_uiOpenPrice;             // 开盘价
        unsigned int m_uiMaxPrice;              // 最高价
        unsigned int m_uiMinPrice;              // 最低价
        unsigned int m_uiClosePrice;            // 今天收盘价
        unsigned int m_uiSettlPrice;            // 今天结算价
        unsigned int m_uiQtyRate;               // 量比
        INT64 m_iTotalTrunoverQty;             // 总成交量
        INT64 m_iTotalTrunoverMoney;           // 总成交额
        INT64 m_iTotalTrunoverCount;           // 总成交笔数
        INT64 m_iVolTrunoverBuy;                // 总买(五档价格)成交量
        INT64 m_iVolTrunoverSell;               // 总卖(五档价格)成交量
        INT64 m_iTotalOrderQty;                // 订货量
        INT64 m_iTotalIssueQty;                // 总发行量
        
        //        STUCloseQuoteInfo_ZYQ()
        //        {
        //            memset(this, 0, sizeof(STUCloseQuoteInfo_ZYQ));
        //        }
    }STUCloseQuoteInfo_ZYQ;
    
    typedef struct _STUSettlQuoteInfo_ZYQ
    {
        STUQuoteDateTime m_oTDateTime;
        STUSimpleSymbol_ZYQ m_oSymbol;
        unsigned int m_uiCommPrice;
        INT64 m_iCommBuy;
        INT64 m_iCommSell;
        INT64 m_iCommTrunoverQty;
        unsigned int m_uiCommTrunoverCount;
        unsigned int m_uiNeutralPrice;
        INT64 m_iNeutralBuy;
        INT64 m_iNeutralSell;
        INT64 m_iNeutralTrunoverQty;
        unsigned int  m_uiNeturalTrunoverCount;
    }STUSettlQuoteInfo_ZYQ;
    
    /*         2.0  end */
    
//止盈止损提示
typedef struct _SL_TP_Hint
{
    char price[32];
    char SlPrice[32];
    char TpPrice[32];
}SL_TP_Hint;

//分时
typedef struct _TraceUnit
{
    unsigned int date;//日期
    unsigned int time;//时间
    unsigned int price;//价格
}TraceUnit;

typedef struct _HisKUnitMA
{
    long long date;//日期
    unsigned int time;//时间
    
    unsigned int openPrice;//开盘价
    unsigned int closePrice;//收
    
    unsigned int maxPrice;//高
    unsigned int minPrice;//低
    
    unsigned int mMA[4];
    unsigned int mMACD[3];
}HistoryKUnitMA;
    
    
typedef struct _HistroyKData
{
    unsigned int max;//高
    unsigned int min;//低
    unsigned int price;//低
    HistoryKUnitMA data[KLINE_LENGHT];
}HistoryKData;

typedef struct _TrendUnit
{
        unsigned int date;//日期
        unsigned int time;//时间
        unsigned int price;//价格
}TrendUnit;
    
typedef struct _TrendData
{
    unsigned int max;//高
    unsigned int min;//低
    TrendUnit data[1440];
}TrendData;
    

#pragma pack()

//enum OPT{
//    CAU_WHD_OP_CANCEL               = '1',  // 撤销
//    CAU_WHD_OP_COMMIT               = '2',  // 提交
//    CAU_WHD_OP_WAREHOUSE_APPLY      = '3',  // 仓库审批通过
//    CAU_WHD_OP_WAREHOUSE_REJECT     = '4',  // 仓库拒绝
//    CAU_WHD_OP_MARKET_APPLY         = '5',  // 市场审批通过
//    CAU_WHD_OP_MARKET_REJECT        = '6',  // 市场拒绝
//    CAU_WHD_OP_WAREHOUSE_PICKUP     = '7',  // 已提货
//    CAU_WHD_OP_DELIVERY_BUY_BREACHOFCONTRACT  = '8', // 买放违约
//    CAU_WHD_OP_DELIVERY_SELL_BREACHOFCONTRACT = '9', // 卖方违约
//    CAU_WHD_OP_DELIVERY_BOTH_BREACHOFCONTRACT = 'A', // 双方违约
//};

//    // 仓单转入库单申请
//    enum CANG_ENTR_STATUS
//    {
//        CANG_CANGTOIMPORT_INIT                  = '0' , // "录入"
//        CANG_CANGTOIMPORT_CANCEL                = '1' , // "撤销"
//        CANG_CANGTOIMPORT_WAIT_WAREHOUSE_APP    = '2' , // "等待仓库审核"
//        CANG_CANGTOIMPORT_WAREHOUSE_REJECT      = '3' , // "仓库驳回"
//        CANG_CANGTOIMPORT_COMPLETE              = '4'  // "完成"
//    };
//    
//    
//    enum CANG_STATUS
//    {
//        CANG_RECORD_STATUS_WAITAPP = '0', //等待仓库审核
//        CANG_RECORD_STATUS_REJECT = '1',  // 仓库驳回
//        CANG_RECORD_STATUS_COMPLETE = '2' // 完成
//    };
//    
//    enum IMPORT_APPLY_STATUS
//    {
//        IMPORT_APPLY_STATUS_INIT = '0', // 录入
//        IMPORT_APPLY_STATUS_CANCEL = '1', // 撤销
//        IMPORT_APPLY_STATUS_WAIT_MARKETAPP = '2', // 等待市场审核
//        IMPORT_APPLY_STATUS_MARKET_REJECT = '3', // 市场驳回
//        IMPORT_APPLY_STATUS_WAIT_WAREHOUSEAPP = '4', // 等待仓库审核
//        IMPORT_APPLY_STATUS_WAREHOUSE_REJECT = '5', // 仓库驳回
//        IMPORT_APPLY_STATUS_COMPLETE = '6'// 完成
//    };
    
    
    //单据状态
    enum ORDER_STATUS
    {
        ORDER_STATUS_UN_TURN_OVER = '1',	//'1'-未成交
        ORDER_STATUS_TURN_OVER = '2',		//'2'-已成交
        ORDER_STATUS_CANCEL = '3',			//'3'-已撤销
        ORDER_STATUS_PART_OVER = '4',		//'4'-部分成交
        ORDER_STATUS_TYPE_CLOSE = '5',		//'5'-全部成交
        ORDER_STATUS_PART_CANCEL = '6',		//'5'-部分撤销
    };
    //下单类型
    enum TRADE_TYPE_OPEN_CLOSE
    {
        TRADE_TYPE_ERASE ='C',			//‘C’ - 这个是给客户端删除持仓用个的
        TRADE_TYPE_OPEN ='1',		//‘1’ - 建仓
        TRADE_TYPE_CLOSE ='2',		//‘2’ - 平仓
        
        TRADE_TYPE_DELIVERY ='3',		//‘3’ - 交割
        TRADE_TYPE_CLOSE_TODAY='4',
        TRADE_TYPE_CLOSE_TESTERDAY='5'
    };
    
    const	unsigned short	HISKDATAFIRST=0x0500;   //第一次请求历史数据
    const	unsigned short	HISKDATA=0x0600;        //后续请求历史数据
    const   unsigned short  HISKDATACURDATE=0x1f00; //当天历史K线数据
    const   unsigned short  STKDEALDATA=0x0300;     //个股分析
    const   unsigned short  INITMARKET=0x1000;      //初始化市场
    const   unsigned short  REPORTDATA=0x0200;      //获取商品基本信息
    
    enum KLineType {
        MINUTE1_HISK        = (char)1,      //表明为一分钟线
        MINUTES5_HISK       = (char)5,      //5分钟K线
        MINUTES10_HISK      = (char)10,     //10分钟K线
        MINUTES15_HISK      = (char)15,     //15分钟K线
        MINUTES30_HISK      = (char)30,     //30分钟K线
        MINUTES60_HISK      = (char)60,     //60分钟K线
        DAY_HISK            = (char)61,     //日K线
        WEEK_HISK           = (char)62,     //周K线
        MONTH_HISK          = (char)63,     //月K线
    };
    
    const	unsigned short	SZ_Index		=0x1011;// 深圳指数
    const	unsigned short	SZ_Bond			=0x1012;// 深圳债券
    const	unsigned short	SZ_Fund			=0x1013;// 深圳基金
    const	unsigned short	SZ_AStock		=0x1014;// 深圳Ａ股
    const	unsigned short	SZ_BStock		=0x1015;// 深圳Ｂ股
    const	unsigned short	SZ_Other		=0x1016;// 深圳其他
    const   unsigned short	SZ_Right		=0x1017;// 深圳债券
    
    /////////yfj
    //是否昨仓（'1'-是, '2'-否）
    enum YESTERDAT_BIN_STATUS
    {
        STAT_YES='1',
        STAT_NO='2'
    };
    
    enum CANCELTYPEY
    {
        SELLTEMENT_AUTO_CANCEL_NO = '0',
        SELLTEMENT_AUTO_CANCEL_YES = '1'
    };
    enum OPER_TYPE
    {
        CAU_WHD_OP_CANCEL='1',
        CAU_WHD_OP_COMMIT='2'
    };
    //FIX_TAG_BODY_IS_SPECTION_AGENCY,	pRes[i].chIsCheckAgency,	"是否是检验机构"定义如下
    enum EIS_SPECTION_AGENCY
    {
        GCA_IS_CHECK_NO = '0',
        GCA_IS_CHECK_YES = '1'
    };
    //FIX_TAG_BODY_APPLY_STATUS, pRes[i].chApplystatus,	"状态" 定义入下
    enum IMPORT_APPLY_STATUS
    {
        IMPORT_APPLY_STATUS_INIT = '0', // 录入
        IMPORT_APPLY_STATUS_CANCEL = '1', // 撤销
        IMPORT_APPLY_STATUS_WAIT_MARKETAPP = '2', // 等待市场审核
        IMPORT_APPLY_STATUS_MARKET_REJECT = '3', // 市场驳回
        IMPORT_APPLY_STATUS_WAIT_WAREHOUSEAPP = '4', // 等待仓库审核
        IMPORT_APPLY_STATUS_WAREHOUSE_REJECT = '5', // 仓库驳回
        IMPORT_APPLY_STATUS_COMPLETE = '6'// 完成
    };
    
    //FIX_TAG_BODY_IS_IMPORT, pRes[i].chImportstatus,	"状态"定义入下
    enum IMPORT_APPLY_IMPORT_STATUS
    {
        IMPORT_APPLY_IMPORT_STATUS_WAIT_STORAGE		= '0', // 等待入库
        IMPORT_APPLY_IMPORT_STATUS_WAIT_APP			= '1', // 待审核
        IMPORT_APPLY_IMPORT_STATUS_STORAGE_COMPLETE = '2'  // 已入库
    };
    
    //FIX_TAG_BODY_EXPORT_STATUS,			pRes[i].chExportstatus,		"状态"定义如下
    enum EXPORT_STATUS
    {
        EXPORT_STATUS_WAIT_MARKETAPP = '0', // 等待市场审核
        EXPORT_STATUS_MARKET_REJECT = '1', // 市场驳回
        EXPORT_STATUS_WAIT_WAREHOUSEAPP = '2', // 等待仓库审核
        EXPORT_STATUS_WAREHOUSE_REJECT = '3', // 仓库驳回
        EXPORT_STATUS_WAIT_SHIP = '4' , // "待出库",
        EXPORT_STATUS_COMPELETE = '5'  // "完成"
    };
    enum ENTRACEOD_STATUS
    {
        //CANG_IMPORT_STATUS_WAIT_MARKET_APP      = '0', // 等待市场审核
        //CANG_IMPORT_STATUS_MARKET_REJECT        = '1',//市场拒绝
        //CANG_IMPORT_STATUS_WAIT_WHAREHOUSE_APP  = '2',//等待仓库审批
        //CANG_IMPORT_STATUS_WHAREHOUSE_REJECT    = '3',  // 仓库驳回
        //CANG_IMPORT_STATUS_COMPLETE             = '4', // 完成
        
        CANG_IMPORT_STATUS_WAIT_WHAREHOUSE_APP      = '0', // 待仓库审核
        CANG_IMPORT_STATUS_WHAREHOUSE_REJECT        = '1', // 仓库驳回
        CANG_IMPORT_STATUS_WAIT_MARKET_APP          = '2', // 等待市场审核
        CANG_IMPORT_STATUS_MARKET_REJECT            = '3', // 市场驳回
        CANG_IMPORT_STATUS_COMPLETE                 = '4' // 完成
    };
    
    enum CANG_STATUS
    {
        CANG_RECORD_STATUS_WAITAPP = '0', //等待仓库审核
        CANG_RECORD_STATUS_REJECT = '1',  // 仓库驳回
        CANG_RECORD_STATUS_COMPLETE = '2' // 完成
    };
    // 仓单转入库单申请
    enum CANG_ENTR_STATUS
    {
        CANG_CANGTOIMPORT_INIT                  = '0' , // "录入"
        CANG_CANGTOIMPORT_CANCEL                = '1' , // "撤销"
        CANG_CANGTOIMPORT_WAIT_WAREHOUSE_APP    = '2' , // "等待仓库审核"
        CANG_CANGTOIMPORT_WAREHOUSE_REJECT      = '3' , // "仓库驳回"
        CANG_CANGTOIMPORT_COMPLETE              = '4'  // "完成"
    };
    enum PICKORDER_STATUS                 //提货意向
    {
        //CANG_DELIVERY_STATUS_INIT                       = '0', // 录入
        //CANG_DELIVERY_STATUS_CANCEL                     = '1', // 撤销
        //CANG_DELIVERY_STATUS_WAIT_WAREHOUSE_APPLAY      = '2', // 待仓库审核
        //CANG_DELIVERY_STATUS_WAREHOUSE_REJECT           = '3', // 仓库退回
        //CANG_DELIVERY_STATUS_WAIT_MARKET_APPLAY         = '4', // 待市场审核
        //CANG_DELIVERY_STATUS_MARKET_REJECT              = '5', // 市场退回
        //CANG_DELIVERY_STATUS_WAIT_SHIP                  = '6', // 待发货(等等交收过程操作)
        //CANG_DELIVERY_STATUS_WAIT_PICKUP                = '7', // 待提货
        //CANG_DELIVERY_STATUS_PICKUPED                   = '8' // 已提货
        CANG_DELIVERY_STATUS_INIT                       = '0', // 录入
        CANG_DELIVERY_STATUS_CANCEL                     = '1', // 撤销
        CANG_DELIVERY_STATUS_WAIT_WAREHOUSE_APPLAY      = '2', // 待仓库审核
        CANG_DELIVERY_STATUS_WAREHOUSE_REJECT           = '3', // 仓库退回
        CANG_DELIVERY_STATUS_WAIT_MARKET_APPLAY         = '4', // 待市场审核
        CANG_DELIVERY_STATUS_MARKET_REJECT              = '5', // 市场退回
        // CANG_DELIVERY_STATUS_WAIT_SHIP                  = '6', // 待发货(等等交收过程操作)
        CANG_DELIVERY_STATUS_WAIT_PICKUP                = '6', // 待提货
        CANG_DELIVERY_STATUS_PICKUPED                   = '7', // 已提货
        CANG_DELIVERY_STATUS_FAIL                       = '8' // 作废
    };
    // 仓单信息
    enum CANG_TYPE
    {
        CANG_RECORE_TYPE_STD = '1',     // 标准仓单
        CANG_RECORE_TYPE_UNSTD = '2',   // 非标准仓单
        CANG_RECORE_TYPE_TMP = '3',     // 零时仓单
    };
    enum DELIVERY_STATUS_SPOT
    {
        CAU_DELIVERY_STATUS_MATCH                   = '1',  // 已配对
        CAU_DELIVERY_STATUS_READY_SETTLEMENT        = '2',  // 可交割
        CAU_DELIVERY_STATUS_BUY_DELIVERY_INTENTIONS = '3',  // 买方提交提货意向
        CAU_DELIVERY_STATUS_BUY_BREACHOFCONTRACT    = '4',  // 买方违约
        CAU_DELIVERY_STATUS_SELL_BREACHOFCONTRACT   = '5',  // 卖方违约
        CAU_DELIVERY_STATUS_BOTH_BREACHOFCONTRACT   = '6',  // 双方违约
        CAU_DELIVERY_STATUS_FINISH                  = '7'  // 交收完成
    };
    enum ACCESS_CONCTRL
    {
        ACCESS_MAKE_MARKET = '1', //做市
        ACCESS_LONG_TREM = '2', //中远期
        ACCESS_WAREHOUSE = '3', //仓库
        ACCESS_NEUTRAL_WAREHOUSE = '4'//中立仓
    };
    enum INOUT_MONERY_STATUS
    {
        //出入金执行状态
        YI_SHOU_LI='0',//已受理
        ZHI_FU_WAN_CHENG='1',//支付完成
        SYSTEM_REJECTION='2',//系统拒绝
        REVIEWED='3',//已审核
    };
    enum MONEY_IN_OUT_APPLY_TYPE
    {
        MONEY_IN_OUT_APPLY_TYPE_IN = '1',	 //入金
        MONEY_IN_OUT_APPLY_TYPE_OUT = '2',	 //出金
        MONEY_IN_OUT_APPLY_TYPE_SUB = '3',	 //红冲
        MONEY_IN_OUT_APPLY_TYPE_ADD = '4',	 //蓝补
        MONEY_IN_OUT_CONTRACT = '5', //签约
        MONEY_IN_OUT_STOP_CONTRACT = '6'	 //解约
        
    };
    
    //资金操作流水类型
    enum FUNDS_OPERATE_TYPE
    {
        FUNDS_OPERATE_TYPE_IN = '1',					//入金
        FUNDS_OPERATE_TYPE_OUT = '2',					//出金
        FUNDS_OPERATE_TYPE_PL = '3',					//盈亏
        FUNDS_OPERATE_TYPE_SWAP = '4',					//递延费
        FUNDS_OPERATE_TYPE_COMMISION = '5',				//佣金
        FUNDS_OPERATE_TYPE_CREDIT = '6',				//调整信用金
        FUNDS_OPERATE_TYPE_CUT_BACK = '7',				//平冲正
        FUNDS_OPERATE_TYPE_RECKON_PL = '8',				//无负债结算盈亏
        FUNDS_OPERATE_TYPE_REVERSED_PROCESSING = '9',	//冲正处理
        FUNDS_OPERATE_TYPE_COMMISION_PIPS = 'A',		//手续费点差
        FUNDS_OPERATE_TYPE_COMMISION_HZ = 'B',			//佣金结转
        FUNDS_OPERATE_TYPE_PL_HZ = 'C',					//盈亏结转
        FUNDS_OPERATE_TYPE_RECKON_PL_HZ = 'D',			//无负债结算盈亏结转
        FUNDS_OPERATE_TYPE_SWAP_HZ = 'E',                //递延费结转
        FUNDS_OPERATE_TYPE_SUB_FUND = 'F',                //红冲
        FUNDS_OPERATE_TYPE_ADD_FUND = 'G',                 //蓝补
        FUNDS_OPERATE_TYPE_DELIVERY_FUND = 'H',            //交割款
        FUNDS_OPERATE_TYPE_DELIVERY_COMMISION = 'I',        //交割费
        FUNDS_OPERATE_TYPE_OUT_FUND_COMMISION = 'J',        //出金手续费
        FUNDS_OPERATE_TYPE_OUT_FUND_COMMISION_HZ = 'K',      //出金手续费结转
        FUNDS_OPERATE_TYPE_DELIVERY_FUND_HZ = 'L',          //交割款结转
        FUNDS_OPERATE_TYPE_DELIVERY_COMMISION_HZ = 'M',		//交割费结转
        FUNDS_OPERATE_TYPE_MONEY_IN_OUT_ADJUEST_IN = 'N',	//调账入金
        FUNDS_OPERATE_TYPE_MONEY_IN_OUT_ADJUEST_OUT = 'O',	//调账出金
        FUNDS_OPERATE_TYPE_CHANGE_HZ = 'P',					//交易所手续费划转银行
        FUNDS_OPERATE_TYPE_INTEREST_SQ = 'Q',				//收取利息
        FUNDS_OPERATE_TYPE_INTEREST_ZF = 'R',				//支付利息
        
        FUNDS_OPERATE_TYPE_CAU_DELIVERY_MARGIN_BACK         = 'a',      // 交收保证金
        FUNDS_OPERATE_TYPE_CAU_DELIVERY_PAYMENT             = 'b',          // 交收货款
        FUNDS_OPERATE_TYPE_CAU_DELIVERY_MARGIN_PAYMENT      = 'c'   // 交收违约金
    };

#ifdef __cplusplus
}
#endif

#endif
