#pragma once

#include "config.h"
#include "nt_bu_typedef.h"
#include "nt_types.h"
//#include "winsock2.h"
#include <string>
#include "nt_crypt.h"
#include "nt_macro.h"
using namespace std;

//
//包格式定义定义
/*
 字段							长度		说明
 起始标记						1	 	标记一个包的开始
 版本号							1		仅针对交易类数据包
 加密方式						1		数据包加密算法
 片段							1       片段;
 长度							2		包的总长度
 包类型		     				2		包所属命令
 通讯包序号						2       异步模式下匹配往返包对的ID
 CRC                             2
 数据体		体头	会话ID		4		异步模式下匹配用户往返包对的ID
 保留值C							4	    保留值
 数据				*
 */
//包各域值定义
const uint8_t  PH_START_FLAG = 0xFF;

const uint32_t NET_PACKAGE_MAX = 64*1024;
enum enumPH_Version
{
	PH_VER_1 = 0x01//版本号为1
};
enum enumPH_Mode
{
	PH_MODE_1 = 0x01//加密方式为1
};

//包各域的长度
const int32_t PH_START_LEN     = 1;//起始标志字段长度
const int32_t PH_VER_TYPE_LEN  = 1;//仅针对交易类数据包长度
const int32_t PH_MODE_TYPE_LEN = 1;//数据包加密算法长度
const int32_t PH_FRAG_LEN	   = 1;//片段的长度
const int32_t PH_LEN_LEN	   = 2;//包长度字段长度
const int32_t PH_CMD_LEN       = 2;//数据包所属命令长度
const int32_t PH_SN_LEN        = 2;//包的序号长度
const int32_t PH_CRC_LEN       = 2;//包的CRC
const int32_t PH_SESSIONID_LEN = 4;//客户SessionId号
const int32_t PH_RESERVE_LEN  = 4;//保留值的长度

const int32_t PH_POSTION_OF_LEN = PH_START_LEN
+ PH_VER_TYPE_LEN
+ PH_MODE_TYPE_LEN
+ PH_FRAG_LEN;

const int32_t PH_Total_2_LEN = PH_POSTION_OF_LEN + PH_LEN_LEN;
const int32_t PH_POSTION_OF_CMD = PH_POSTION_OF_LEN
+ PH_LEN_LEN;

const int32_t PH_POSTION_OF_CRC = PH_POSTION_OF_CMD
+ PH_CMD_LEN
+ PH_SN_LEN;

const int32_t PH_POSTION_OF_SID = PH_POSTION_OF_CRC
+ PH_CRC_LEN;

const int32_t PH_HEAD_LEN		= PH_POSTION_OF_SID
+ PH_SESSIONID_LEN
+ PH_RESERVE_LEN;//  包头总长度


#ifdef HP_UNIX
#pragma pack 1
#elif defined(AIX)
#pragma options align=packed
#elif defined(_WIN32)
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif



typedef struct tagNtPkgHeadAlign{
    uint8_t  bStartFlag;
    uint8_t  bVer;
    uint8_t  bEncryptFlag;
    uint8_t  bFrag;
    uint16_t wLen;
    uint16_t wCmd;
    uint16_t wSeq;
    uint16_t wCrc;
    uint32_t dwSID;
	union HeadReserved
	{
		uint32_t dwReserved;
		struct
		{
			uint8_t  sReserved[4];
		}CharReserved;
		struct
		{
			uint16_t wTotal;
			uint16_t wCurSeq;
		}WordBuEx;
	}unHeadReserved;
	tagNtPkgHeadAlign()
	{
		bStartFlag = PH_START_FLAG;
		bVer = PH_VER_1;
		bEncryptFlag = PH_MODE_1;
		bFrag = 0;
		wLen = PH_HEAD_LEN;
		wCmd = 0;
		wSeq = 0;
		wCrc = 0;
		dwSID = 0;
		unHeadReserved.dwReserved = 0;
	}
} NtPkgHeadAlign; //主机版,按对齐地址排列


typedef struct {
    NtPkgHeadAlign pkgHead;
    uint16_t wTotal;
    uint16_t wCurSeq;
} NtPkgHeadBuEx; //用于大包处理的结构

#ifdef HP_UNIX
#pragma pack 4
#elif defined(AIX)
#pragma options align=reset
#elif defined(_WIN32)
#pragma pack(pop)
#else
#pragma pack()
#endif


typedef struct tagSessionPackageData
{
	HANDLE handle;
	uint32_t dwIfSID;
	NtPkgHeadAlign oNtPkgHead;
	uint8_t *pData;
	uint32_t nSize;
	void * pExt;
public:
	tagSessionPackageData()
	{
		handle = 0;
		pData= 0;
		nSize = 0;
		dwIfSID = 0;
		pExt =0;
	}
}SessionPackageData, *LPSessionPackageData;

class CNetBlockMsg
{
public:
	CNetBlockMsg(void){};
	virtual ~CNetBlockMsg(void){};
    
	//创建登录信息
	bool static CreateLoginPkg(int nSid, string& strLogin, PBYTE& pData, int& nLen)
	{
		NtPkgHeadAlign oNtPkg;
		oNtPkg.dwSID = nSid;
		oNtPkg.wCmd = BU_CA_LOGIN_USER;
		SET_ENCRYPT_FLAG(oNtPkg.bEncryptFlag, CA_TEA);
		//TODO:业务包的处理没有完成
        
		//打包处理
		CNetBlockMsg::EncodeNtPkgHead(&oNtPkg, pData, nLen);
		return true;
	}
    
	//创建心跳信息
	bool static CreateHearBeatPkg(int nSid, PBYTE& pData, int& nLen)
	{
		NtPkgHeadAlign oNtPkg;
		oNtPkg.dwSID = nSid;
		oNtPkg.wCmd = NET_SINGAL_HEARTBEAT;
		//打包处理
		CNetBlockMsg::EncodeNtPkgHead(&oNtPkg, pData, nLen);
		return true;
	}
    
    
	inline bool static IsCutFrag(int nLen)
	{
		if(nLen + PH_HEAD_LEN > NET_PACKAGE_MAX)
		{
			return true;
		}
		return false;
	}
    
	inline void static HostToNet(NtPkgHeadAlign& pkg)
	{
		pkg.wLen = htons(pkg.wLen);
		pkg.wCmd = htons(pkg.wCmd);
		pkg.wSeq = htons(pkg.wSeq);
		pkg.wCrc = htons(pkg.wCrc);
		pkg.dwSID = htonl(pkg.dwSID);
		pkg.unHeadReserved.dwReserved = htonl(pkg.unHeadReserved.dwReserved);
	}
    
	inline void static CRCHostToNet(NtPkgHeadAlign& pkg)
	{
		pkg.wCrc = htons(pkg.wCrc);
	}
    
	inline void static CRCNetToHost(NtPkgHeadAlign& pkg)
	{
		pkg.wCrc = ntohs(pkg.wCrc);
	}
    
    
	inline void static NetToHost(NtPkgHeadAlign& pkg)
	{
		pkg.wLen = ntohs(pkg.wLen);
		pkg.wCmd = ntohs(pkg.wCmd);
		pkg.wSeq = ntohs(pkg.wSeq);
		pkg.wCrc = ntohs(pkg.wCrc);
		pkg.dwSID = ntohl(pkg.dwSID);
		//pkg.unHeadReserved.dwReserved = ntohl(pkg.unHeadReserved.dwReserved);
	}
    
	inline static uint16_t NtPkgHead2Crc16(const uint32_t * pdwBuf)
	{
		uint32_t dwCrc = 0;
		dwCrc ^= *pdwBuf++;
		dwCrc ^= *pdwBuf++;
		dwCrc ^= *pdwBuf++;
		dwCrc ^= *pdwBuf++;
		dwCrc ^= *pdwBuf++;
		return (uint16_t)((dwCrc & 0xffff) ^ (dwCrc >> 16));
	}
    
    struct SPackHead
    {
        unsigned int nVersion;//包版本 默认1
        unsigned int nDatalen;//包长度
        unsigned int nReserved;//保留字段 默认0x0F0F0F0F
    };

    
	bool static EncodeNtPkgHead(NtPkgHeadAlign * pstNtPkgHeadAlign, PBYTE& pBuf, int& iLeftLen)
	{
        //重新计算出的新的Crc
        //封包+数据体长度
        //SPackHead oHead;
        //oHead.nVersion = 5;
        //oHead.nDatalen = PH_HEAD_LEN + iLeftLen;
        
        //int nDataLen = PH_HEAD_LEN + iLeftLen + sizeof(SPackHead);
        int nDataLen = PH_HEAD_LEN + iLeftLen;
        
        
        pstNtPkgHeadAlign->wLen = PH_HEAD_LEN + iLeftLen;
        pstNtPkgHeadAlign->wCrc = 0;
        if(NET_SINGAL_HEARTBEAT == pstNtPkgHeadAlign->wCmd)
        {
            pstNtPkgHeadAlign->bEncryptFlag = 0;
        }
        HostToNet(*pstNtPkgHeadAlign);
        pstNtPkgHeadAlign->wCrc = NtPkgHead2Crc16((const uint32_t *)pstNtPkgHeadAlign);
        CRCHostToNet(*pstNtPkgHeadAlign);
        
        PBYTE pEnData = new BYTE[nDataLen + 1];
        pEnData[nDataLen] = 0;
        
        //memcpy(pEnData + PH_HEAD_LEN + sizeof(SPackHead), pBuf, iLeftLen);
        memcpy(pEnData + PH_HEAD_LEN, pBuf, iLeftLen);
        
        iLeftLen = nDataLen;
        
        delete[] pBuf;
        pBuf = pEnData;
        
        int nPos = 0;
        
        /*
         memcpy(pEnData + nPos, &oHead.nVersion, sizeof(int));//起始标志字段长度
         nPos += sizeof(int);
         
         memcpy(pEnData + nPos, &oHead.nDatalen, sizeof(int));//起始标志字段长度
         nPos += sizeof(int);
         
         memcpy(pEnData + nPos, &oHead.nReserved, sizeof(int));//起始标志字段长度
         nPos += sizeof(int);
         */
        
        
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->bStartFlag),PH_START_LEN);//数据包加密算
        nPos += PH_START_LEN;
        
        
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->bVer),PH_VER_TYPE_LEN);//数据包加密算法长度
        
        nPos += PH_VER_TYPE_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->bEncryptFlag),PH_MODE_TYPE_LEN );//仅针对交易类数据包长度
        
        nPos += PH_MODE_TYPE_LEN;
        memcpy(pEnData + nPos, 	&(pstNtPkgHeadAlign->bFrag),PH_FRAG_LEN);//片段的长度
        
        nPos += PH_FRAG_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->wLen),PH_LEN_LEN);//包长度字段长度
        
        nPos += PH_LEN_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->wCmd),PH_CMD_LEN);//数据包所属命令长度
        
        nPos += PH_CMD_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->wSeq),PH_SN_LEN);//包的序号长度
        
        nPos += PH_SN_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->wCrc),PH_CRC_LEN);//包的CRC
        
        nPos += PH_CRC_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->dwSID),PH_SESSIONID_LEN);//客户SessionId号
        
        nPos += PH_SESSIONID_LEN;
        memcpy(pEnData + nPos, &(pstNtPkgHeadAlign->unHeadReserved.dwReserved),PH_RESERVE_LEN);//保留值的长度
        
        return true;
	}
    
	//int EncodeNtPkgHead(NtPkgHeadAlign * pstNtPkgHeadAlign, char ** ppBuf, int * piLeftLen)
	//{
	//	const NtPkgHead * pstNtPkgHead;
	//	char * pCrc;
    
	//	assert(pstNtPkgHeadAlign && ppBuf && *ppBuf);
    
	//	//封包
	//	pstNtPkgHead = (const NtPkgHead *)(*ppBuf);
	//	CHECK_RET(AddChar(ppBuf, piLeftLen, pstNtPkgHeadAlign->bStartFlag));
	//	CHECK_RET(AddWord(ppBuf, piLeftLen, pstNtPkgHeadAlign->wLen));
	//	CHECK_RET(AddChar(ppBuf, piLeftLen, pstNtPkgHeadAlign->bVer));
	//	CHECK_RET(AddChar(ppBuf, piLeftLen, pstNtPkgHeadAlign->bEncryptFlag));
	//	CHECK_RET(AddChar(ppBuf, piLeftLen, pstNtPkgHeadAlign->bReserved1));
	//	CHECK_RET(AddWord(ppBuf, piLeftLen, pstNtPkgHeadAlign->wCmd));
	//	CHECK_RET(AddWord(ppBuf, piLeftLen, pstNtPkgHeadAlign->dwSeq));
	//	pCrc = *ppBuf;
	//	pstNtPkgHeadAlign->wCrc = 0;
	//	CHECK_RET(AddWord(ppBuf, piLeftLen, pstNtPkgHeadAlign->wCrc)); //这个位置要重新计算
	//	CHECK_RET(AddDWord(ppBuf, piLeftLen, pstNtPkgHeadAlign->dwSID));
	//	if (piLeftLen && *piLeftLen < sizeof(pstNtPkgHeadAlign->sReserved2))
	//	{
	//		return -1;
	//	}
	//	memcpy(*ppBuf, pstNtPkgHeadAlign->sReserved2, sizeof(pstNtPkgHeadAlign->sReserved2));
	//	*ppBuf += sizeof(pstNtPkgHeadAlign->sReserved2);
	//	if (piLeftLen)
	//	{
	//		*piLeftLen -= sizeof(pstNtPkgHeadAlign->sReserved2);
	//	}
    
	//	//重新计算出的新的Crc
	//	AddWord(&pCrc, NULL, _20ToCrc16((const uint32_t *)pstNtPkgHead));
    
	//	return 0;
	//}
    
    
    
    
	//int DecodeNtPkgHead(char * pPkg, int nPkgLen, NtPkgHeadAlign * pstNtPkgHeadAlign)
	//{
	//	NtPkgHead * pstNtPkgHead;
	//	uint16_t wNCrc;
    
	//	assert(pPkg && pstNtPkgHeadAlign);
    
	//	pstNtPkgHead = (NtPkgHead *)pPkg;
    
	//	//解包
	//	CHECK_RET(GetChar(&pPkg, &nPkgLen, (char *)&pstNtPkgHeadAlign->bStartFlag));
	//	CHECK_RET(GetWord(&pPkg, &nPkgLen, &pstNtPkgHeadAlign->wLen));
	//	CHECK_RET(GetChar(&pPkg, &nPkgLen, (char *)&pstNtPkgHeadAlign->bVer));
	//	CHECK_RET(GetChar(&pPkg, &nPkgLen, (char *)&pstNtPkgHeadAlign->bEncryptFlag));
	//	CHECK_RET(GetChar(&pPkg, &nPkgLen, (char *)&pstNtPkgHeadAlign->bReserved1));
	//	CHECK_RET(GetWord(&pPkg, &nPkgLen, &pstNtPkgHeadAlign->wCmd));
	//	CHECK_RET(GetWord(&pPkg, &nPkgLen, &pstNtPkgHeadAlign->dwSeq));
	//	CHECK_RET(GetWord(&pPkg, &nPkgLen, &pstNtPkgHeadAlign->wCrc));
	//	CHECK_RET(GetDWord(&pPkg, &nPkgLen, (unsigned long *)&pstNtPkgHeadAlign->dwSID));
	//	if (nPkgLen < sizeof(pstNtPkgHeadAlign->sReserved2))
	//	{
	//		return -1;
	//	}
	//	memcpy(pstNtPkgHeadAlign->sReserved2, pPkg, sizeof(pstNtPkgHeadAlign->sReserved2));
	//	pPkg += sizeof(pstNtPkgHeadAlign->sReserved2);
	//	nPkgLen -= sizeof(pstNtPkgHeadAlign->sReserved2);
    
	//	//做Crc校验
	//	memset(pstNtPkgHead->sCrc, 0, sizeof(pstNtPkgHead->sCrc));
	//	if (pstNtPkgHeadAlign->wCrc != _20ToCrc16((const uint32_t *)pstNtPkgHead))
	//	{
	//		return -1;
	//	}
    
	//	//还原包头的Crc字段
	//	wNCrc = htons(pstNtPkgHeadAlign->wCrc);
	//	memcpy(pstNtPkgHead->sCrc, &wNCrc, sizeof(pstNtPkgHead->sCrc));
    
	//	return 0;
	//}
};

//////////////金智行情//////////////////////

const   unsigned short  USERPWDVALID        =0x8b00;//用户密码校验
const   unsigned int	SOFTWARE_VERSION    =0X1001;//当前软件版本号
const	unsigned short	HEARTBEAT           =0x8d00;//心跳命令

#pragma pack(1)
// 请求结构
struct stuRecvDataHead //balan请求结构
{
    char cMsgType;//1 为客户端请求，2为服务端数据
    char cGroupType;//ip分组
};



struct SCommPkgHead
{
	unsigned int nVersion;//包版本 默认1
	unsigned int nDatalen;//包长度
	unsigned int nReserved;//保留字段
};
struct	SNormaHead
{
	unsigned	short	m_usType;//响应类型，与请求数据包类型一致
	unsigned	short	m_usIndex;//请求索引，与请求数据包类型一致
	unsigned	int		m_usSize;//后续结构的个数
};

///////////////////CultureTrade start//////////////////////

struct PacketHead
{
    unsigned int m_uiDataLen;
    unsigned int m_uiReserved;
};

struct SCodeInfo
{
    unsigned	short	m_usMarketType;//市场标识
    unsigned	int		m_uiCode;//证券代码
};

struct	SCodeQuoteInfo
{
    unsigned	short	m_usMarketType;//市场标识
    unsigned	int		m_uiCode;//证券代码
};

struct	SRequestData
{
    unsigned	short	m_usType;               //请求类型
    unsigned	short	m_usMarketType;         //市场标识
    unsigned	short	m_usIndex;              //请求索引
    unsigned	short	m_usSize;               //请求证券总数,即	SCodeInfo结构的个数
    SRequestData()
    {
        memset(this,0,sizeof(SRequestData));
    }
//    SCodeQuoteInfo m_pstCode[0];
};

// 登陆返回
struct SUserPwdRes
{
    unsigned short m_usType;
    unsigned short m_usIndex;
    unsigned int m_uisize;
    //char* m_pcText; 返回错误时，后面直接接错误信息
};

struct	SHisKUnit
{
    unsigned	int		m_uiDate;
    unsigned	int		m_uiOpenPrice;
    unsigned	int		m_uiMaxPrice;
    unsigned	int		m_uiMinPrice;
    unsigned	int		m_uiClosePrice;
    unsigned	int		m_uiVolume;
    unsigned	int		m_uiSum;//成交金额
};

struct SHisKData    {
    SCodeInfo CodeInfo;         //股票信息
    unsigned short m_usSize;    //K线单元个数
//    SHisKUnit*  m_pstHisKUnit;  //单元数据指针
} ;

//HISKDATACURDATE、HISKDATAFIRST对应的数据结构：
struct	SDownData   {
    unsigned	short	m_usType; // HISKDATACURDATE
    unsigned	short	m_usIndex;//请求索引，与请求数据包类型一致
    unsigned	short	m_usSize;//结构SHisKData_New的个数
//    SHisKData*	m_psHisKData;  //以股票为单位数据体
};


//登陆请求//用户名和密码校验
struct SUserPwdDataReq
{
	char m_szUser[20];
	char m_szPwd[20];
	unsigned int m_uiSoftWareVersion;
	char m_szHardDiskInfo[17];
    char m_szNetWorkInfo[10];
};


//STKDEALDATA数据对应的结构：
//SDealData + SNowData->SVolPrice*SNowData->m_ucVolPrices + STraceUnit*m_usTraceUnits + SRealMinsUnit*m_usMinsUnits

//在本结构中，股票的SNowData包含了10个SVolPrice对?象，对应如下：
//卖5价-SVolPrice[0] 卖4价-SVolPrice[1] 卖3价-SVolPrice[2] 卖2价-SVolPrice[3] 卖1价-SVolPrice[4]
//买1价-SVolPrice[5] 买2价-SVolPrice[6] 买3价-SVolPrice[7] 买4价-SVolPrice[8] 买5价-SVolPrice[9]
//指数的StockNowData包含了2个SVolPrice对象，分别表示上涨家数，下跌家数，委买数，委卖数。
struct	SNowData
{
    unsigned	int	m_uiCode;
    unsigned	short	m_usTime;//距第一次开盘时间数
    unsigned	int	m_uiOpenPrice;//今开盘
    unsigned	int	m_uiMaxPrice;//最高价
    unsigned	int	m_uiMinPrice;//最低价
    unsigned	int	m_uiNewPrice;//最新价
    unsigned	int	m_uiAverage;//均价
    unsigned	int	m_uiVolume;//成交量总手
    unsigned	int	m_uiCurVol;//成交量现手
    unsigned	int	m_uiSum;//成交金额，期货中没有此项
    unsigned	int	m_uiBuyVol;//主买，对于指数为上涨家数
    unsigned	int	m_uiSellVol;//主卖，对于指数为下跌家数
    unsigned	char	m_ucVolPrices;//量价个数
    // SVolPrice*	m_pstVolPrice;//量价结构
};

struct	SVolPrice
{
    unsigned	int		m_uiPrice;//价格
    unsigned	int		m_uiVolume;//量
};

struct	SDealData
{
    unsigned	short	m_usType;//响应类型，STKDEALDATA
    unsigned	short	m_usIndex;//请求索引，与请求数据包类型一致
    SCodeInfo           m_usCode ;//请求股票信息
    unsigned	short	m_usTraceUnits;//STraceUnit结构的个数
    // STraceUnit*	m_pstTraceUnit;
    unsigned	short	m_usMinsUnits;//SRealMinsUnit结构的个数
    // SRealMinsUnit*	m_pstMinsUnit;
    SNowData	m_stNowData;
};


//交易品种成交分笔结构
struct	STraceUnit
{
    unsigned	short	m_usTime;     //距离开盘时间的分钟数
    unsigned	int	m_uiNewPrice; //最新价
    unsigned	int	m_uiVolume;   //成交量
    unsigned int           m_uiBuyVol;   //委买量
    unsigned int           m_uiSellVol;  //委卖量
    unsigned	int	m_uiBuyPrice; //委买价，对于指数则表示上涨家数
    unsigned	int	m_uiSellPrice;//委卖价，对于指数则表示下跌家数
    unsigned	int	m_uiSum;      //成交金额, 持仓量适用于期货市场
};

//当日证券成交数据，用于画价格线和成交量线
struct	SRealMinsUnit//以分钟为单位生成
{
    unsigned	short	m_usTime;//距离第一次开盘时间
    unsigned	int	m_uiNewPrice;
    unsigned	int	m_uiVolume;
    unsigned	int	m_uiKeepVol;//持仓量,适用于期货市场
    unsigned	int	m_uiAverage;//平均价格
    unsigned	int	m_uiBuyVol;//买盘数量
    unsigned	int	m_uiSellVol;//卖盘数量
};

//服务器返回INITMARKET命令，数据结构：
//SInitMarket + （SMarketInfo + SMarketInfo->m_usStocks * SStockInfo）+ ......  +  m_ucMarkets个（SMarketInfo + SMarketInfo->m_usStocks * SStockInfo）

//时间结构
struct	SPhase
{
    unsigned	short	m_usOpenTime;//开市时间
    unsigned	short	m_usCloseTime;//闭市时间
};

struct	SStockInfo
{
    char		m_acName[60];//名称
    char		m_acSpell[4];
    unsigned	int	m_uiCode;//代码
    unsigned	short	m_usMarketType;//市场标识
    unsigned	int	m_uiPrevClose;//昨收价
    unsigned	int 	m_ui5DayVol;//五日平均总手
    unsigned	int 	m_uiUpLimit;//涨停板限制,
    unsigned	int	m_uiDownLimit;//跌停板限制
    unsigned	int	m_uiShares;//流通股数
    unsigned int          m_uiReserves; // 库存量
    short		m_sAsset;//每股净资产
};

struct	SMarketInfo
{
    unsigned	short	m_usMarketType;// 请求市场标识
    unsigned    char    m_ucMarketName[13];//市场类型名，用于分类
    unsigned	int     m_uiTodayDate;	//今日日期
    unsigned	short	m_usStocks;		// 证券个数
    //SStockInfo*	m_pstStockInfo;	// 证券初始化信息
};


struct	SInitMarket
{
    unsigned	short	m_usType;		//命令类型
    unsigned	short	m_usMarketType;	//市场类型
    unsigned	int	m_uiTodayDate;	//今日日期
    unsigned	char	m_ucPhases;		//交易时间段
    SPhase		m_pstPhase[3];
    unsigned	char	m_ucMarkets;	//市场个数
    //	SMarketInfo*	m_pstMarket;	//市场信息
};

struct	SSortUnit
{
    SCodeInfo	m_CodeInfo;//代码值
    unsigned	int m_uiOpenPrice;//今开盘
    unsigned	int	m_uiMaxPrice;//最高价
    unsigned	int	m_uiMinPrice;//最低价
    unsigned	int	m_uiNewPrice;//最新价
    unsigned    int m_uiAverage;//均价
    unsigned	int	m_uiVolume;//成交量总手
    unsigned	int	m_uiCurVol;//成交量现手
    unsigned	int	m_uiSum;//成交金额
    unsigned	int	m_uiSellPrice;//委卖价
    unsigned	int	m_uiBuyPrice;//委买价
    unsigned	int	m_uiSellVol;//委卖量
    unsigned	int	m_uiBuyVol;//委买量
    int             m_iRatio;//委比
    SSortUnit()
    {
        memset(this,0,sizeof(SSortUnit));
    }
};

struct	SSortData
{
    unsigned	short	m_usType;//响应类型，与请求数据包类型一致
    unsigned	short	m_usIndex;//请求索引，与请求数据包类型一致
    unsigned	short	m_usSize;//结构SortUnit的个数
    unsigned	short	m_usTotal;//排序股票的总数目
    //		SSortUnit*	m_pstSortUnit;
};

///////////////////CultureTrade end//////////////////////
//登录返回
struct SUserPwdDataRes
{
	char 	m_cPassed;		//登陆是否成功 0 不成功，1成功
	int		m_nErrorData;	//错误信息长度
};

//struct SUserPwdRes
//{
//    unsigned short m_usType;
//    unsigned short m_usIndex;
//    unsigned int m_uisize;
//    char m_pcText[0];
//};

const	unsigned short	PUSH_REALDATA=0x0b00;//主动推送实时数据


struct StuRecvQuoteInfo
{
	SCodeInfo sCode;
	unsigned int uiPrice;
	time_t lastTick;
};

///////////////
//const	unsigned short	REPORTDATA                     =0x0200;//报价表
//const	unsigned short	REPORTDATA_ZYQ                 =0x02a0;//报价表

//实时数据
//struct	SNowData
//{
//	uint64_t	timeUpdate;
//	SCodeInfo	sCode;	  //产品代码
//	unsigned int uiOpenPrice;//今开盘
//	unsigned int uiMaxPrice;//最高价
//	unsigned int uiMinPrice;//最低价
//	unsigned int uiNewPrice;//最新价
//};

//const unsigned short STKMINUTEDATA=0x1100;//分时成交
//const unsigned short STKTRACEDATA= 0x0100;//分笔成交
//分笔数据
//struct STraceUnit
//{
//    unsigned int uiDate; //日期
//    unsigned int uiTime; //时间
//    unsigned int uiPrice; //价格
//};

//const unsigned short HISKDATA=0x0600;//历史K线数据
//struct SHisotyDataReq
//{
//    int nDataType;//历史K线种类
//    int nHasNum;//当前已有数量
//    int nAskNum;//请求数量
//};

typedef struct tagPackageData
{
	NtPkgHeadAlign oNtPkgHead;
    SCommPkgHead   oCommPkgHead;
	uint8_t *pData;
	int32_t nSize;
public:
	tagPackageData()
	{
		pData= NULL;
		nSize = 0;
	}
}PackageData, *LPPackageData;

typedef struct tagQuotePackageData
{
    NtPkgHeadAlign oNtPkgHead;
    PacketHead   oCommPkgHead;
    uint8_t *pData;
    int32_t nSize;
public:
    tagQuotePackageData()
    {
        pData= NULL;
        nSize = 0;
    }
}QuotePackageData, *LPQuotePackageData;

// 初始化市场
//const unsigned short INITMARKET                =0x1000;//OTC商品信息
//const unsigned short INIT_MARKET_ZYQ            =0x1a00;//中远期

//商品信息
struct StuSymbolData
{
	SCodeInfo sCode;
	char szName[20];
	char cTrade;//是否交易 0：非交易 1：交易
	int	 nBeginTime;//交易开始时间
	int  nTotalTradeTime;//总交易时间
};

#pragma pack()
