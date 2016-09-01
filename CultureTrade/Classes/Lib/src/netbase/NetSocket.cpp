#include <assert.h>
#include<algorithm>
#include <fstream>
#include "NetSocket.h"
#include "nt_bu_typedef.h"
#include "nt_md5.h"
#include "nt_macro.h"
#include "nt_pack.h"
#include "nt_pkg.h"
#include "global.h"
#include "jxt_log.h"

//#include "nt_debug.h"
//#include "zlib.h"

using namespace std;
//using namespace NTMsgStruct;

//int readcount = 0;
int closesocket(SOCKET soc)
{
	if(soc < 0)
		return -1;
	return close((int)soc);
}
#define SD_BOTH	2


int hx_soc_set_nonblock(int sock)
{//return 0:suc; -1:err
	int opt;
	opt = fcntl(sock, F_GETFL, 0);
	if(opt == -1 || fcntl(sock, F_SETFL, opt | O_NONBLOCK) == -1){
		return -1;
	}
	return 0;
}

long GetTickCount()
{
	//clock_t clock(void);
	//CLOCKS_PER_SEC
	return clock();
}
//typedef struct _QuoteReqMap
//{
//    char symbol[32];//商品
//    char cType;//类型
//}QuoteReqMap;
//
//typedef std::map<int, QuoteReqMap> Req_Seq_Container; //保存序列号
//Req_Seq_Container quote_Req_Seq_Container;

CNetSocket::CNetSocket(void)
{
	m_nWaitReConnect = 2;
	m_nJudgeDisConnect = 9;
	m_nSid = -1;
	m_nSeq = 0;
	m_pfFireSessionStatus = NULL;
	m_pfFireBlockMessage = NULL;
	m_pfQuoteBlockMessage = NULL;
	m_bShouldExit = false;
	m_connectStatusType = CST_UNCONNECTED;
	m_hSocket = NULL;
	m_hEvent = NULL;
	m_hThread = NULL;
	m_hFireThread = NULL;
	m_hHeartBeatThread = NULL;
	m_curSocketReTryCnt = -1;
	m_bAllProxy = false;
    m_nPackType = 0;
    m_nQuoteFlag = 0;
    m_nQuoteLoginFlag = 0;
	const char * sCltKey = "0123456789abcdef";
	Md5HashBuffer(m_sCltKey, sCltKey, strlen(sCltKey));
	Md5HashBuffer(m_sCltKey, m_sCltKey, sizeof(m_sCltKey));

	memset(&m_pthread_cond,0,sizeof(m_pthread_cond));
	memset(&m_pthread_mutex,0,sizeof(m_pthread_mutex));

    m_pstrSymbolID = NULL;
	m_is_start = 0;
    
    m_nRecvKUnit = 0;
    m_nLastReqCode = 0;
    m_hSSL = NULL;
    m_send_heartbeat_time = 0;
	//m_pthread_cond = PTHREAD_COND_INITIALIZER;
	//m_pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
}

CNetSocket::~CNetSocket(void)
{
	ClearSocket();
    delete m_pstrSymbolID;
#if 0
	if( m_hHeartBeatThread != NULL )
	{
		CloseHandle(m_hHeartBeatThread);
		m_hHeartBeatThread = NULL;
	}

	if( m_hFireThread != NULL )
	{
		CloseHandle(m_hFireThread);
		m_hFireThread = NULL;
	}
	if( m_hThread != NULL )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
#endif

}

bool CNetSocket::InitNet(void)
{
#ifdef _WIN32
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}
#endif
	return true;
}
int CNetSocket::ClearNet()
{
#ifdef _WIN32
	return ::WSACleanup();
#endif
	return 0;
}

ConnectStatusType CNetSocket::GetConnectStatusType() const
{
	return m_connectStatusType;
}
void CNetSocket::SetConnectStatusType(int type)
{
    m_connectStatusType = (ConnectStatusType)type;
}

void CNetSocket::GetServerHost(string& strIp, int& nPort)
{
	//服务器的HOST信息
	strIp = m_spd.hostProperty.strIp;
	nPort = m_spd.hostProperty.nPort;
}

void CNetSocket::SetWaitReConnect(const int& nWaitReConnect)
{
	m_nWaitReConnect = nWaitReConnect;
}

void CNetSocket::SetJudgeDisConnect(const int& nJudgeDisConnect)
{
	m_nJudgeDisConnect = nJudgeDisConnect;
}
void CNetSocket::GetWaitReConnect(int& nWaitReConnect)
{
	nWaitReConnect = m_nWaitReConnect;
}

void CNetSocket::GetJudgeDisConnect(int& nJudgeDisConnect)
{
	nJudgeDisConnect = m_nJudgeDisConnect;
}
void CNetSocket::SetPackType(int type)
{
    m_nPackType = type;
}
void CNetSocket::SetReConnectProperty(ReConnectProperty& rcp)
{
	m_rcp = rcp;
}
void CNetSocket::GetReConnectProperty(ReConnectProperty& rcp)
{
	rcp = m_rcp;
}

bool CNetSocket::HasHost(const ArrayServerPropertyData& aspd,const string& strIp, const long& nPort)
{
	bool bRet = false ;
	ArrayServerPropertyData::const_iterator iterCur = aspd.begin();
	ArrayServerPropertyData::const_iterator iterEnd = aspd.end();

	for( ; iterCur != iterEnd ; iterCur ++ )
	{
		if (iterCur->hostProperty.strIp == strIp && iterCur->hostProperty.nPort == nPort )
		{
			bRet = true ;
			break ;
		}
	}
	return bRet ;
}

void CNetSocket::SetAllProxy(const ProxyProperty& pp)
{
	m_bAllProxy = true;
	m_spd.proxyProperty = pp;
}

void CNetSocket::ClearAllProxy()
{
	m_bAllProxy = false;
	m_spd.proxyProperty.clear();
}
bool CNetSocket::AddPriServerPropertyData(ServerPropertyData& spd)
{
	bool bRet = false;
	if (!HasHost(m_aryPriHosts,spd.hostProperty.strIp, spd.hostProperty.nPort))
	{
		if(m_bAllProxy)
		{
			spd.proxyProperty = m_spd.proxyProperty;
		}
		m_aryPriHosts.push_back(spd);
		bRet = true;
	}
	return bRet;
}

bool CNetSocket::AddBakServerPropertyData(ServerPropertyData& spd)
{
	bool bRet = false;
	if (!HasHost(m_aryBakHosts,spd.hostProperty.strIp, spd.hostProperty.nPort))
	{
		if(m_bAllProxy)
		{
			spd.proxyProperty = m_spd.proxyProperty;
		}
		m_aryBakHosts.push_back(spd);
		bRet = true;
	}
	return bRet;
}

bool CNetSocket::ClearPriServerPropertyData()
{
	m_aryPriHosts.clear();
	if(m_aryBakHosts.size()<=0)
	{
		m_bAllProxy =false;
	}
	return true;
}
bool CNetSocket::ClearBakServerPropertyData()
{
	m_aryBakHosts.clear();
	if(m_aryPriHosts.size()<=0)
	{
		m_bAllProxy =false;
	}
	return true;
}

int CNetSocket::GetServerPropertyCount() const
{
	return GetPriServerPropertyCount() + GetBakServerPropertyCount();
}

int CNetSocket::GetPriServerPropertyCount() const
{
	return m_aryPriHosts.size();
}

int CNetSocket::GetBakServerPropertyCount() const
{
	return m_aryBakHosts.size();
}

ServerPropertyData CNetSocket::GetPriServerProperty(int nIndex)
{
	ServerPropertyData spd;
	if(nIndex >=0 && nIndex < GetPriServerPropertyCount())
	{
		spd = m_aryPriHosts[nIndex];
	}
	return spd;
}
ServerPropertyData CNetSocket::GetBakServerProperty(int nIndex)
{
	ServerPropertyData spd;
	if(nIndex >=0 && nIndex < GetBakServerPropertyCount())
	{
		spd = m_aryBakHosts[nIndex];
	}
	return spd;
}
/******************************************************
 Bind网络包处理方法
 *******************************************************/

void CNetSocket::BindFirePackage(int nSid, PFFirePackage pfFireBlockMessage)
{
	m_nSid = nSid;
	m_pfFireBlockMessage = pfFireBlockMessage;
}

void CNetSocket::BindQuotePackage(int nSid, PFFirePackage pfQuoteBlockMessage)
{
	m_nSid = nSid;
	m_pfQuoteBlockMessage = pfQuoteBlockMessage;
}

void CNetSocket::BindFireSessionStatus(int nSid, PFFireSessionStatus pfFireSessionStatus)
{
	m_nSid = nSid;
	m_pfFireSessionStatus = pfFireSessionStatus;
}
void CNetSocket::BindNotifyEvent(int nSid, PFNotifyEvent pfNotifyEvent)
{
	m_nSid = nSid;
	m_pfNotifyEvent = pfNotifyEvent;
}

/******************************************************
 选择服务HOST信息
 *******************************************************/
bool CNetSocket::SeletHost()
{
	if( m_rcp.lReConnectCount >= 0 )
	{
		if(m_curSocketReTryCnt <= 0)//重新选IP
		{
			long lFastIndex = GetServerIPBySpeed(m_aryPriHosts, true);
			if( lFastIndex < 0 )
			{
				lFastIndex = GetServerIPBySpeed(m_aryBakHosts, true);
				if( lFastIndex < 0 )
				{
					return false;
				}
				//取得最快的HOST
				m_spd = m_aryBakHosts[lFastIndex];
			}else
			{
				//取得最快的HOST
				m_spd = m_aryPriHosts[lFastIndex];
			}

			m_curSocketReTryCnt = m_rcp.lReConnectCount ;
		}
		if(  m_curSocketReTryCnt > 0 )
		{
			m_curSocketReTryCnt -- ;
		}
	}
	return true;
}

/******************************************************
 清除 收发Buffer，关闭SOCKET及关联的Event
 *******************************************************/
void CNetSocket::ClearSocket()
{
	m_sendBuff.ClearBuffer();
	m_recvBuff.ClearBuffer();
#if 0
	if (m_hEvent)
	{
		WSACloseEvent(m_hEvent);
		m_hEvent = NULL;
	}
#endif
	if (m_hSocket)
	{
		shutdown(m_hSocket, SD_BOTH);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	m_connectStatusType = CST_UNCONNECTED;
}

/******************************************************
 设置发送超时6秒,接收超时6秒
 *******************************************************/
bool CNetSocket::SetSocketOption(SOCKET socket)
{
	int nRecvBuf = MAX_PACKAGE_LEN;
	setsockopt(socket,SOL_SOCKET,SO_RCVBUF, (const char*)&nRecvBuf,sizeof(int));

	int nSendBuf = MAX_PACKAGE_LEN;
	setsockopt(socket,SOL_SOCKET,SO_SNDBUF, (const char*)&nSendBuf,sizeof(int));
	struct timeval TimeOut={8,0};//3
	//int TimeOut=6000; //设置发送超时8秒,接收超时8秒
	if(setsockopt(socket,SOL_SOCKET,SO_SNDTIMEO,&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		return false;

	if(setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		return false;

	linger sLinger;
	sLinger.l_onoff = 0; // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
	sLinger.l_linger = 0; // (容许逗留的时间为0秒)
	if (setsockopt(socket,SOL_SOCKET,SO_LINGER,&sLinger,sizeof(linger))==SOCKET_ERROR)
		return false;

	return true;
}

/*******************************
 连接服务
 ********************************/
bool CNetSocket::Connect()
{
	HX_LOG("m_connectStatusType=%d",m_connectStatusType);
	//表示正连接则不要再发起连接功能
	if(m_connectStatusType == CST_CONNECTING)
	{
		return false;
	}

	//正在连接，初始化连接相关的信息
	ClearSocket();
	m_connectStatusType = CST_CONNECTING;
	//创建SOKCET
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	HX_LOG("m_hSocket=%d",m_hSocket);
	if (m_hSocket == INVALID_SOCKET)
	{
		m_connectStatusType = CST_UNCONNECTED;
		return false;
	}

	//是否要重新选择IP
	if(!SeletHost())
	{
		//没有可用的服务
		m_connectStatusType = CST_ERROR_CONNECT;
		FireSessionStatus();
		return false;
	}
	//设置SOCKET相关接收与发送BUFFER
	if(!SetSocketOption(m_hSocket))
	{
		m_connectStatusType = CST_UNCONNECTED;
		return false;
	}
	//处理是否需要代理连接信息
	if(!ConnectByProxy(m_spd, m_hSocket,this))
	{
		m_connectStatusType = CST_ERROR_CONNECT;
		FireSessionStatus();
		return false;
	}
	else
		OnConnect();
#if 0 //xiongxiong, csligongxiong@qq.com
	//与SOCKET关联的事件
	m_hEvent = WSACreateEvent();
	if (m_hEvent == WSA_INVALID_EVENT)
	{
		m_connectStatusType = CST_UNCONNECTED;
		return false;
	}

	//设置关注的事件
	int nRet = WSAEventSelect(m_hSocket,
                              m_hEvent,
                              FD_CONNECT|FD_CLOSE|FD_READ);
	if (nRet == SOCKET_ERROR)
	{
		m_connectStatusType = CST_UNCONNECTED;
		return false;
	}
#endif
	return true;
}

//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=(int)str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            
            result.push_back(s);
            i=(int)(pos+pattern.size()-1);
        }
    }
    return result;
}

bool CNetSocket::Send(int nCmd, int nSeq, PBYTE pData, int nLen)
{
	bool bRet = false;

    if (m_nPackType == 0) {
        NtPkgHeadAlign oNtPkg;
        oNtPkg.dwSID = m_nSid;
        oNtPkg.wCmd = nCmd;
        oNtPkg.wSeq = nSeq;
        if (BU_QUOTE_CTRL_QUOTE == nCmd) { // 设置行情标志不需要加密
            SET_ENCRYPT_FLAG(oNtPkg.bEncryptFlag, CA_NO_ENC);
        } else {
            // 为了兼容手机端,故现在加密算法采用CA_ASTEA(原来是CA_TEA,加密强度要更好)
            SET_ENCRYPT_FLAG(oNtPkg.bEncryptFlag, CA_ASTEA);
        }

        //COPY要发送的数据
        PBYTE pSendData = new BYTE[nLen];
        memcpy(pSendData, pData, nLen);

        //此处考虑是不是大包要不要分包发送处理
        if(!CNetBlockMsg::IsCutFrag(nLen))
        {
            //只要打一个包处理
            CNetBlockMsg::EncodeNtPkgHead(&oNtPkg, pSendData, nLen);

            //ofstream logfile("login.log", ios::binary | ios::app);
            //logfile.write((const char*)pSendData, nLen);


            //HX_LOG("write data to send buffer");
            bRet = Send(pSendData, nLen);
        }else
        {
            HX_LOG("package need cut,not support");
            //TODO:要打多个包处理
        }

        delete[] pSendData;
    }
    else if( m_nPackType == 2) {
//        PBYTE pSendData = new BYTE[nLen];
//        memcpy(pSendData, pData, nLen);
        //HX_LOG("write data to send buffer");
        switch(nCmd){
            case 0x01: {
                int nDataLen = sizeof(stuRecvDataHead);
                char* pDataQuote = new char[nDataLen];
                memset(pDataQuote, 0, nDataLen);
                stuRecvDataHead* pHead = (stuRecvDataHead*)(pDataQuote);
                pHead->cGroupType = 1;
                pHead->cMsgType = 1;
    //            memcpy(pDataQuote, (const void *)&pHead, nDataLen);
                memcpy(pDataQuote, pHead, nDataLen);
    //            bRet = Send((PBYTE)pDataQuote, nLen);            
                
                SendBalancePack(pDataQuote, nDataLen);
                delete[] pDataQuote;
//                int nDataLen = nLen + 1;
//                char* pDataQuote = new char[nLen + 1];
//                memset(pDataQuote, 0, nDataLen);
//                memcpy((void *)pDataQuote, (char*)pData, nLen);
//                SendQuotePack(pDataQuote, nDataLen);
//                delete[] pDataQuote;
                break;
            }
        default:
            Send(pData, nLen);
        }
    }//TODO:发送行情包
    else if( m_nPackType == 1) {

        switch (nCmd)
        {
            case USERPWDVALID:
            {
                int nDataLen;
                nDataLen = sizeof(SRequestData)+sizeof(SUserPwdDataReq);
                char* pDataQuote = new char[nDataLen];
                SRequestData* pHead = (SRequestData*)(pDataQuote);
                pHead->m_usType = USERPWDVALID;
                pHead->m_usIndex = 1;
                pHead->m_usSize = 0;
                
                SUserPwdDataReq* pUserPwd = (SUserPwdDataReq*)(pHead+1);
                /*
                 #ifdef __OFF_REAL_QUOTE__
                 strcpy(pUserPwd->m_szUser, "android");
                 #else
                 memset(pUserPwd->m_szUser, 0, sizeof(pUserPwd->m_szUser));
                 #endif
                 */
                
                char *user = new char[20];
                char *pwd = new char[20];
                memcpy(user, "", 20);
                memcpy(pwd, "", 20);
                memcpy(pUserPwd->m_szUser, user, sizeof(pUserPwd->m_szUser));
                memcpy(pUserPwd->m_szPwd, pwd, sizeof(pUserPwd->m_szPwd));
                pUserPwd->m_uiSoftWareVersion = SOFTWARE_VERSION;
                memset(pUserPwd->m_szHardDiskInfo, 0, sizeof(pUserPwd->m_szHardDiskInfo));
                printBusLogEx("USERPWDVALID  QuoteUser= [%s] QuotePwd= [%s]\n",pUserPwd->m_szUser, pUserPwd->m_szPwd);
                SendQuotePack(pDataQuote, nDataLen);
                delete []user;
                delete []pwd;
                delete []pDataQuote;
                m_send_quote_login_time = (UInt32)time(NULL);
                m_nQuoteLoginFlag = 0;
            }
                break;
            case HEARTBEAT:
            {
                int nDataLen = sizeof(SRequestData);
                char* pData = new char[nDataLen];
                for(int i = 0; i < nDataLen; i ++)
                    pData[i] = 0;
                SRequestData* pReq = (SRequestData*)(pData);
                pReq->m_usType = HEARTBEAT;
                pReq->m_usSize = 0;
                SendQuotePack(pData, nDataLen);
                delete []pData;
            }
                break;
            case HISKDATAFIRST + MINUTE1_HISK:
            case HISKDATAFIRST + MINUTES5_HISK:
            case HISKDATAFIRST + MINUTES10_HISK:
            case HISKDATAFIRST + MINUTES15_HISK:
            case HISKDATAFIRST + MINUTES30_HISK:
            case HISKDATAFIRST + MINUTES60_HISK:
            case HISKDATAFIRST + DAY_HISK:
            case HISKDATAFIRST + WEEK_HISK:
            case HISKDATAFIRST + MONTH_HISK:
            case HISKDATA + MINUTE1_HISK:
            case HISKDATA + MINUTES5_HISK:
            case HISKDATA + MINUTES10_HISK:
            case HISKDATA + MINUTES15_HISK:
            case HISKDATA + MINUTES30_HISK:
            case HISKDATA + MINUTES60_HISK:
            case HISKDATA + DAY_HISK:
            case HISKDATA + WEEK_HISK:
            case HISKDATA + MONTH_HISK:
            {
                int nBufferSize;
                nBufferSize = sizeof(SRequestData)+sizeof(SCodeInfo)*3;
                char* pBuffer = new char[nBufferSize];
                memset(pBuffer, 0, nBufferSize);
                
                int code = atoi((char*)pData);  // 藏品代码
                int markType = code/1000;
                
                SRequestData* pReq  = (SRequestData*)pBuffer;
                
                pReq->m_usType = nCmd;
                pReq->m_usMarketType = markType;    //市场类型
                pReq->m_usIndex = nCmd;     //在服务器返回数据的时候会把这个请求索引再传回来，以达到请求和答复一 一对应
                pReq->m_usSize = 3;
                
                //第一个SCodeInfo
                SCodeInfo *pCodeInfo = (SCodeInfo*)(pBuffer+sizeof(SRequestData));
                pCodeInfo->m_uiCode = code;                  //藏品代码
                pCodeInfo->m_usMarketType = markType;        //藏品市场类型
                //第二个SCodeInfo
                pCodeInfo++;
                pCodeInfo->m_uiCode = nLen;       //现有历史数据数量
                //第三个SCodeInfo
                pCodeInfo++;
                pCodeInfo->m_uiCode = KREQKDATACOUNT;     //想要得到的历史数据数量
                
                SendQuotePack(pBuffer, nBufferSize);
                delete []pBuffer;
            }
                break;
            case STKDEALDATA:
            {
                int nBufferSize;
                nBufferSize = sizeof(SRequestData)+sizeof(SCodeInfo);
                char* pBuffer = new char[nBufferSize];
                memset(pBuffer, 0, nBufferSize);
                
                int code = atoi((char*)pData);  // 藏品代码
                int markType = code/1000;
                
                SRequestData* pReq  = (SRequestData*)pBuffer;
                pReq->m_usType = STKDEALDATA;
                pReq->m_usMarketType = markType;    //市场类型
                pReq->m_usIndex = nSeq;     //在服务器返回数据的时候会把这个请求索引再传回来，以达到请求和答复一 一对应
                pReq->m_usSize = 1;
                
                //第一个SCodeInfo
                SCodeInfo *pCodeInfo = (SCodeInfo*)(pBuffer+sizeof(SRequestData));
                pCodeInfo->m_uiCode = code;                  //藏品代码
                pCodeInfo->m_usMarketType = markType;        //藏品市场类型
                
                SendQuotePack(pBuffer, nBufferSize);
                delete []pBuffer;
            }
                break;
            case INITMARKET:
                {
                    int nDataLen = sizeof(SRequestData);
                    char* pData = new char[nDataLen];
                    memset(pData, 0, nDataLen);
//                    for(int i = 0; i < nDataLen; i ++)
//                        pData[i] = 0;
                    SRequestData* pRes = (SRequestData*)(pData);
                    pRes->m_usType = INITMARKET;
                    pRes->m_usIndex = nSeq;
                    SendQuotePack(pData, nDataLen);
                    delete []pData;
                    printBusLogEx("INITMARKET is sending!\n");
                }
                break;
            case REPORTDATA:
                {
                    
                    printBusLogEx("REPORTDATA is sending!\n");
                    char *str = (char *)pData;
                    if (str == NULL) return false;
                    std::vector<std::string> result=split(str,",");
                    
                    int nBufferSize = sizeof(SRequestData)+sizeof(SCodeInfo)*(int)result.size();
                    char* pBuffer = new char[nBufferSize];
                    memset(pBuffer, 0xc, nBufferSize);
                    
                    SRequestData* pReq = (SRequestData*)(pBuffer);
                    pReq->m_usType = REPORTDATA;
                    pReq->m_usIndex = nSeq;
                    pReq->m_usSize = (int)result.size();
                    
                    //第一个SCodeInfo
                    int index = 0;
                    for(vector<string>::iterator it  = result.begin(); it != result.end(); ++it)
                    {
                        printBusLogEx("请求藏品代码 %s",(*it).c_str());
                        SCodeInfo codeInfo;
                        memset(&codeInfo, 0, sizeof(SCodeInfo));
                        codeInfo.m_uiCode = atoi((*it).c_str());
                        codeInfo.m_usMarketType = atoi((*it).c_str())/1000; //SZCodeToZSMarket(atoi((*it).c_str()));
                        memcpy(pBuffer+sizeof(SRequestData)+sizeof(SCodeInfo)*index,&codeInfo,sizeof(SCodeInfo));
                        index++;

//                        pCodeInfo->m_uiCode = atoi((*it).c_str());                                 //藏品代码
//                        pCodeInfo->m_usMarketType = SZCodeToZSMarket(atoi((*it).c_str()));          //藏品市场类型
//                        pCodeInfo++;
                    }
                    
                    SendQuotePack(pBuffer, nBufferSize);
//                    printBusLogEx("REPORTDATA has send!\n");
                    delete []pBuffer;
                    
                }
                break;
//            case REPORTDATA_ZYQ:
//            {
//                printBusLogEx("REPORTDATA_ZYQ\n");
//                int nDataLen = sizeof(SNormaHead) + nLen;
//                char* pDataQuote = new char[nDataLen + 1];
//                SNormaHead  tRequest;
//                tRequest.m_usIndex = 2;
//                tRequest.m_usSize = nLen;
//                tRequest.m_usType = nCmd;
//
//                memset(pDataQuote, 0, nDataLen);
//                memcpy(pDataQuote, (const void *)&tRequest, sizeof(SNormaHead));
//                memcpy((void *)(pDataQuote + sizeof(SNormaHead)), (char*)pData, nLen);
//                
//                SendQuotePack(pDataQuote, nDataLen);
//                delete[] pDataQuote;
////                SendQuotePack((char*)&tRequest,sizeof(SNormaHead));
//            }
//                break;
//            case STKTRACEDATA:
//                {
//                    //对于此处保存char的字符串 如果收到消息了 则将此字符串删除
//                    //没有收到任何消息而新发了消息 不作处理的！ 直到收到消息后才进行处理的
//                    printBusLogEx("STKTRACEDATA\n");
//                    if(pData && nLen > 0)
//                    {
//                        char* strSymbolID = (char*)pData;
//                        //quote_Req_Seq_Container.insert(pair<int,string>(nSeq, strSymbolID));
//
//                        int nDataLen = sizeof(SNormaHead) + sizeof(SCodeInfo);
//                        char* pDataQuote = new char[nDataLen + 1];
//                        for(int i = 0; i < (nDataLen + 1); i++)
//                            pDataQuote[i] = 0;
//                        SNormaHead* pHead = (SNormaHead*)(pDataQuote);
//                        pHead->m_usIndex = nSeq;
//                        pHead->m_usSize = 0;
//                        pHead->m_usType = STKTRACEDATA;
//                        SCodeInfo* pCodeInfo = (SCodeInfo*)(pHead+1);
//                        memcpy(pCodeInfo->szCode, pData, nLen);
//                        if(nLen >= sizeof(pCodeInfo->szCode))
//                            pCodeInfo->szCode[sizeof(pCodeInfo->szCode) - 1] = 0;
//                        else
//                            pCodeInfo->szCode[nLen] = 0;
//
//                        pCodeInfo->cPriceUnit = 0;
//                        
//                        #ifdef ___DEBUG_SEND___
//                        char str[8];
//                        myPrintf("--------------------------------------\n");
//                        for(int i = 0; i < nDataLen; i ++)
//                        {
//                            BYTE data = pDataQuote[i];
//                            sprintf(str, "%x ", data);
//                            myPrintf("%s", str);
//                            if( (i != 0)
//                               && (0== (i % 16)))
//                                myPrintf("\n");
//                        }
//                        myPrintf("\n--------------------------------------\n");
//                        #endif
//                        
////                        QuoteReqMap his;
////                        strcpy(his.symbol, strSymbolID);
////                        quote_Req_Seq_Container.insert(pair<int,QuoteReqMap>(nSeq, his));
//                        SendQuotePack(pDataQuote, nDataLen);
//                        delete []pDataQuote;
//                    }
//                }
//                break;
//            case STKMINUTEDATA:
//                {
//                    //对于此处保存char的字符串 如果收到消息了 则将此字符串删除
//                    //没有收到任何消息而新发了消息 不作处理的！ 直到收到消息后才进行处理的
//                    printBusLogEx("STKMINUTEDATA\n");
//                    if(pData && nLen > 0)
//                    {
//                        char* strSymbolID = (char*)pData;
//                        //quote_Req_Seq_Container.insert(pair<int,string>(nSeq, strSymbolID));
//
//                        int nDataLen = sizeof(SNormaHead) + sizeof(SCodeInfo);
//                        char* pDataQuote = new char[nDataLen + 1];
//                        for(int i = 0; i < (nDataLen + 1); i++)
//                            pDataQuote[i] = 0;
//                        SNormaHead* pHead = (SNormaHead*)(pDataQuote);
//                        pHead->m_usIndex = nSeq;
//                        pHead->m_usSize = 0;
//                        pHead->m_usType = STKMINUTEDATA;
//
//                        SCodeInfo* pCodeInfo = (SCodeInfo*)(pHead+1);
//                        memcpy(pCodeInfo->szCode, pData, nLen);
//                        if(nLen >= sizeof(pCodeInfo->szCode))
//                            pCodeInfo->szCode[sizeof(pCodeInfo->szCode) - 1] = 0;
//                        else
//                            pCodeInfo->szCode[nLen] = 0;
//
//                        pCodeInfo->cPriceUnit = 0;
//                        
//#ifdef ___DEBUG_SEND___
//                        char str[8];
//                        myPrintf("--------------------------------------\n");
//                        for(int i = 0; i < nDataLen; i ++)
//                        {
//                            BYTE data = pDataQuote[i];
//                            sprintf(str, "%x ", data);
//                            myPrintf("%s", str);
//                            if( (i != 0)
//                               && (0== (i % 16)))
//                                myPrintf("\n");
//                        }
//                        myPrintf("\n--------------------------------------\n");
//#endif
////                        QuoteReqMap his;
////                        strcpy(his.symbol, strSymbolID);
////                        quote_Req_Seq_Container.insert(pair<int,QuoteReqMap>(nSeq, his));
//                        SendQuotePack(pDataQuote, nDataLen);
//                        delete []pDataQuote;
//                    }
//                }
//                break;
//            case HISKDATA:
//                {
//                    if(pData && nLen > 0)
//                    {
//                        //这里参数也用数组的形式传了，方法已经定死了，只能用这种方法来传 保证兼容原来的代码 或者使用全局量
//                        int k = 0;
//                        char* strSymbolID = (char*)pData;
//                        char* strPeriodic = NULL;
//                        char* strCount = NULL;
//                        char* strReqCount = NULL;
//                        int j = 0;
//                        for(k = 0; k < nLen; k ++)
//                        {
//                            if('|' == pData[k]) {
//                                pData[k] = 0;
//                                j ++;
//                                if(1 == j)
//                                    strPeriodic = (char*)&(pData[k + 1]);
//                                else if(2 == j)
//                                    strCount = (char*)&(pData[k + 1]);
//                                else if(3 == j)
//                                    strReqCount = (char*)&(pData[k + 1]);
//                            }
//                        }
//                        
//                        //quote_Req_Seq_Container.insert(pair<int,string>(nSeq, strSymbolID));
//                        
//                        int nLen = sizeof(SNormaHead) + sizeof(SHisotyDataReq) + sizeof(SCodeInfo);
//                        char* pDataQuote = new char[nLen + 1];
//                        for(int i = 0; i < (nLen + 1); i++)
//                            pDataQuote[i] = 0;
//                        
//                        SNormaHead* pHead = (SNormaHead*)pDataQuote;
//                        pHead->m_usIndex = nSeq;
//                        pHead->m_usSize = 0;
//                        pHead->m_usType = HISKDATA;
//                        
//                        printBusLogEx("REQ_HIS_DATA seq=%d symbol=%s cType=%d Count=%s ReqCount=%s\n", nSeq, strSymbolID, atoi(strPeriodic), strCount, strReqCount);
//                        
//                        SHisotyDataReq* pHisDataReq = (SHisotyDataReq*)(pHead + 1);
//                        m_cKLinePeriodicType = atoi(strPeriodic);
//                        pHisDataReq->nDataType = m_cKLinePeriodicType;
//                        pHisDataReq->nHasNum = atoi(strCount);
//                        pHisDataReq->nAskNum = atoi(strReqCount);
//                        if(pHisDataReq->nAskNum <= 0 && 0 != pHisDataReq->nHasNum) {
//                            pHisDataReq->nHasNum = 0;
//                            pHisDataReq->nAskNum = 100;
//                        }
//                        
//                        SCodeInfo* pCodeInfo = (SCodeInfo*)(pHisDataReq + 1);
//                        strcpy(pCodeInfo->szCode, strSymbolID);
//                        pCodeInfo->cPriceUnit = 0;
//                        
////                        QuoteReqMap his;
////                        his.cType =m_cKLinePeriodicType;
////                        strcpy(his.symbol, strSymbolID);
////                        quote_Req_Seq_Container.insert(pair<int,QuoteReqMap>(nSeq, his));
//
//                        SendQuotePack(pDataQuote, nLen);
//                    }
//                }
//                break;
//            case INITMARKET:
//            case INIT_MARKET_ZYQ:
//                {
//                    printBusLogEx("INITMARKET\n");
//                    int nDataLen = sizeof(SNormaHead);
//                    char* pData = new char[nDataLen];
//                    for(int i = 0; i < nDataLen; i ++)
//                        pData[i] = 0;
//                    SNormaHead* pHead = (SNormaHead*)(pData);
//                    pHead->m_usIndex = 2;
//                    pHead->m_usSize = 0;
//                    pHead->m_usType = nCmd;
//                    SendQuotePack(pData, nDataLen);
//                    delete []pData;
//                }
//                break;
            default:
                if(nCmd > 0){
                    printBusLogEx("Send Quote Server pCmd=[%d] pLen=[%d]\n", nCmd, nLen);
                    int nDataLen = sizeof(SNormaHead) + nLen;
                    char* pDataQuote = new char[nDataLen + 1];
                    SNormaHead  tRequest;
                    tRequest.m_usIndex = nSeq;
                    tRequest.m_usSize = nLen;
                    tRequest.m_usType = nCmd;
                    memset(pDataQuote, 0, nDataLen);
                    memcpy(pDataQuote, (const void *)&tRequest, sizeof(SNormaHead));
                    if(nLen > 0) memcpy((void *)(pDataQuote + sizeof(SNormaHead)), (char*)pData, nLen);
                    SendQuotePack(pDataQuote, nDataLen);
                    delete[] pDataQuote;
                } else {
                    SendQuotePack((char*)pData, nLen);
                }
                break;
        }
    }
	return bRet;
}




/*******************************
 发送数据
 ********************************/
bool CNetSocket::Send(PBYTE pData, int nLen)
{
	Locker locker(m_MutexSendBuff);
#ifdef ___DEBUG_SEND___
    myPrintf("lock check\n");
    char str[8];
    myPrintf("--------------------------------------\n");
    for(int i = 0; i < nLen; i ++)
    {
        BYTE data = pData[i];
        sprintf(str, "%d ", (char)data);
        myPrintf("%s", str);
        if( (i != 0)
           && (0== (i % 16)))
            myPrintf("\n");
    }

    myPrintf("\n--------------------------------------\n");
    #endif
	m_sendBuff.Write(pData, nLen);
	//return SendData();
	return true;
}

bool CNetSocket::SendData()
{ //return true,send suc; false, send err
	Locker locker(m_MutexSendBuff);
	int nSendDataLen = m_sendBuff.GetBufferLen();
	PBYTE pData = m_sendBuff.GetBuffer();
	int nBytesSent = 0;
	//HX_LOG("write,m_hSocket=%d,m_connectStatusType=%d",m_hSocket,m_connectStatusType);
	if(m_connectStatusType == CST_CONNECTED)
	{
		while (nBytesSent < nSendDataLen)
		{
			int nBytes;
            
#ifdef ___DEBUG_SEND___
            char str[8];
            for(int i = 0; i < nSendDataLen - nBytesSent; i ++)
            {
                BYTE data = pData[i];
                sprintf(str, "0x%x", data);
                printf(" %s", str);
            }
            printf("\n");
#endif
//            PacketHead* pHead = (PacketHead*)pData;
//            SRequestData* pReq = (SRequestData*)(pData+sizeof(PacketHead));
//            SCodeInfo* pCode = (SCodeInfo*)(pData+sizeof(PacketHead)+sizeof(SRequestData));
//            pCode++;
//            pCode++;
//            pCode++;
//            SUserPwdDataReq* pUser = (SUserPwdDataReq*)(pData+sizeof(PacketHead)+sizeof(SRequestData));
//            int headlen = sizeof(PacketHead);
//            int reqlen = sizeof(SRequestData);
//            int userlen = sizeof(SUserPwdDataReq);
            
            
#if SSL_ENABLE
            if (this->m_nPackType == 0) {
                nBytes = SSL_write(m_hSSL, (const char *)pData , nSendDataLen - nBytesSent);
            }else{
                nBytes = (int)write(m_hSocket,(const char *)pData , nSendDataLen - nBytesSent);
            }
#else
            nBytes = (int)(write(m_hSocket,(const char *)pData , nSendDataLen - nBytesSent));
#endif
            
#if DEBUG
//            printBusLogEx("send ==> m_nSid=[%d] nBytes[%d]", m_nSid, nBytes);
            if (m_nPackType == 0) {//trade
                unsigned int cmd;
                memcpy(&cmd, pData+6, sizeof(unsigned short));
                printBusLogEx("Trade Send ==> cmd = [%d],m_hSocket = [%d],m_nSid = [%d],nBytes = [%d]\n",HTONS(cmd), m_hSocket,m_nSid, nBytes);
            }else if (m_nPackType == 1){//quote
                unsigned short cmd;
                memcpy(&cmd, pData+sizeof(PacketHead), sizeof(unsigned short));
                printBusLogEx("Quote Send ==> cmd = [%x],m_hSocket = [%d],m_nSid = [%d],nBytes = [%d]\n",cmd, m_hSocket, m_nSid, nBytes);
            }else if (m_nPackType == 2){//balance
                unsigned short cmd;
                memcpy(&cmd, pData+sizeof(SCommPkgHead), sizeof(unsigned short));
                printBusLogEx("Balan Send ==> cmd = [%d],m_hSocket = [%d],m_nSid = [%d],nBytes = [%d]\n",cmd, m_hSocket, m_nSid, nBytes);
            }
#endif
			if(nBytes > 0 )
            {
                nBytesSent += nBytes;
                m_sendBuff.Delete(nBytes);
            }
			else
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    printBusLogEx("write wouldblock,errno=%d,sock=%d",errno, m_hSocket);
                    return true;
                }
                else
                {
                    printBusLogEx("write err,errno=%d,sock=%d",errno, m_hSocket);
                    return false;
                }
            }
		}
	}
	return true ;
}

//找到有效的包开头位置
bool CNetSocket::RemovDataUntilValidHeadFlag(CBuffer& recvBuff)
{
	bool bRet = false;
	int nPos = 0;
	int nLen = recvBuff.GetBufferLen();
	for (; nPos < nLen; ++nPos)
	{
		uint8_t bStartFlag = 0;
		memcpy(&bStartFlag, recvBuff.GetBuffer(nPos), PH_START_LEN);

		if (bStartFlag == PH_START_FLAG)
		{
			bRet = true;
			break;
		}
	}
	if(!bRet)
	{
		--nPos;
	}
	//移去无效数据
	recvBuff.Delete(nPos);
	return bRet;
}
void DebugHex(unsigned char* buff, int count)
{
    int i,j,c;
    int printnext=1;
    int offset = 0;
    
    if (count<=0)
        return;
    
    if (count > 1000)
    {
        // debug_printf("buffer count=%d, the max is 1000", count);
        count = 1000;
    }
    
    char* log_buf = new char[1024*100];
    memset(log_buf, 0, 1024*100);
    
    if (count % 16)
    {
        c = count + (16 - count % 16);
    }
    else
    {
        c = count;
    }
    // offset += sprintf(g_log_buf + offset, "line count %d, c %d \n", count, c);
    for (i = 0; i < c; i++)
    {
        if (printnext)
        {
            printnext--;
            offset += sprintf(log_buf + offset, "%.4x ",i&0xffff);
        }
        
        if (i < count)
        {
            offset += sprintf(log_buf + offset, "%3.2x", buff[i]&0xff);
        }
        else
        {
            offset += sprintf(log_buf + offset, "   ");
        }
        if (!((i+1)%8))
        {
            if ((i+1)%16)
            {
                offset += sprintf(log_buf + offset, "%s", " -");
            }
            else
            {
                offset += sprintf(log_buf + offset,  "%s", "   ");
                for (j = i-15; j <= i; j++)
                {
                    if (j < count)
                    {
                        if ( (buff[j]&0xff) >= 0x20 &&
                            (buff[j]&0xff) <= 0x7e)
                        {
                            // 0x25 -> '%' 0x5c -> '\'
                            if ((buff[j]&0xff) == 0x25 || (buff[j]&0xff) == 0x5c)
                            {
                                offset += sprintf(log_buf + offset, "%s", ".");
                            }
                            else
                            {
                                offset += sprintf(log_buf + offset, "%c", buff[j]&0xff);
                            }
                        }
                        else
                        {
                            offset += sprintf(log_buf + offset, "%s", ".");
                        }
                    }
                    else
                    {
                        offset += sprintf(log_buf + offset, "%s", " ");
                    }
                }
                offset += sprintf(log_buf + offset, "%s", "\n");
                printnext=1;
            }
        }
    }
    printBusLogEx("%s\n", log_buf);
    delete [] log_buf;
}
void HexDump(char *buf, int len) {
    int i,j,k;
    char binstr[len*2];
    for (i=0;i<len;i++) {
        if (0==(i%16)) {
            sprintf(binstr,"%04x -",i);
            sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
        } else if (15==(i%16)) {
            sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
            sprintf(binstr,"%s  ",binstr);
            for (j=i-15;j<=i;j++) {
                sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
            }
            printf("%s\n",binstr);
        } else {
            sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
        }
    }
    if (0!=(i%16)) {
        k=16-(i%16);
        for (j=0;j<k;j++) {
            sprintf(binstr,"%s   ",binstr);
        }
        sprintf(binstr,"%s  ",binstr);
        k=16-k;
        for (j=i-k;j<i;j++) {
            sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
        }
        printBusLogEx("%s\n", binstr);
    }
}

/*******************************
        发生了读取事件
 ********************************/
bool CNetSocket::OnRead() {
    //true,read suc;  false, read err
	int read_len;
	int dwSize = 0;
    dwSize = 64*1024;
    
	if(m_hSocket == INVALID_SOCKET)
		return false;
    
    BYTE* pData = new BYTE[dwSize+2];
#if SSL_ENABLE
    if (this->m_nPackType == 0) {
        read_len = SSL_read(m_hSSL, pData, dwSize);
    }else{
        read_len = (int)read(m_hSocket, pData, dwSize);
    }
#else
    read_len = (int)read(m_hSocket, pData, dwSize);
#endif

    
#if DEBUG
    //    if(m_nPackType)
    //        HX_LOG("Quote read,m_hSocket=%d len=%d",m_hSocket, read_len);
    //    else
    //        HX_LOG("Trade read,m_hSocket=%d len=%d",m_hSocket, read_len);
    if(m_nPackType == 0) {
        unsigned short cmd;
        memcpy(&cmd, (void*)(pData + 6), sizeof(unsigned short));
        printBusLogEx("Trade Read ==> cmd = [%d],m_hSocket = [%d],byte = [%d]",HTONS(cmd),m_hSocket,read_len);
    }else if (m_nPackType == 1) {
        unsigned short cmd;
        memcpy(&cmd, (void*)(pData + sizeof(PacketHead)), sizeof(unsigned short));
        printBusLogEx("Quote Read ==> cmd = [%x],m_hSocket = [%d],byte = [%d]",cmd,m_hSocket,read_len);
    }else if (m_nPackType == 2) {
        unsigned short cmd;
        memcpy(&cmd, (void*)(pData + sizeof(SCommPkgHead)), sizeof(unsigned short));
        printBusLogEx("Balan Read ==> cmd = [%d],m_hSocket = [%d],byte = [%d]",cmd,m_hSocket,read_len);
    }else {HX_LOG("%d",m_nPackType);}
#endif
    
	if(read_len < 0) {
        delete[] pData;
        if((errno == EAGAIN || errno == EWOULDBLOCK)) {
            HX_LOG("read wouldblock,sock=%d",m_hSocket);
            return true;
        } else  //err
        {
            delete m_pstrSymbolID;
            m_pstrSymbolID = NULL;
            HX_LOG("read err,errno=%d,sock=%d",errno,m_hSocket);
            return false;
        }
    } else if(read_len == 0)  // CONN_LOST; /* EOF */
    {
        delete m_pstrSymbolID;
        m_pstrSymbolID = NULL;
        HX_LOG("read err,errno=%d,sock=%d",errno,m_hSocket);
        delete[] pData;
        return false;
    } else {
        m_recvBuff.Write(pData, read_len);
        delete[] pData;
        pData = NULL;
    }

    //交易包
    if (m_nPackType == 0) {
        while (m_recvBuff.GetBufferLen() > PH_HEAD_LEN) {
            bool bWaitForAllPackage = false;
            //查找合法的数据包
            for(int nPos = 0; nPos < m_recvBuff.GetBufferLen(); ++nPos)
            {
                int nSize = 0;
                int nBufferLen = m_recvBuff.GetBufferLen();
                if( nBufferLen < PH_HEAD_LEN)
                {//等待数据整合
                    bWaitForAllPackage = true;
                    break;
                }

                //找到有效的包开头位置
                RemovDataUntilValidHeadFlag(m_recvBuff);
                nBufferLen = m_recvBuff.GetBufferLen();

                if( nBufferLen < PH_HEAD_LEN)
                {//等待数据整合
                    bWaitForAllPackage = true;
                    break;
                }

                //取包头信息
                NtPkgHeadAlign oNtPkgHead;
                memcpy(&oNtPkgHead, m_recvBuff.GetBuffer(nPos), PH_HEAD_LEN);

                uint16_t wCrc = ntohs(oNtPkgHead.wCrc);
                oNtPkgHead.wCrc = 0;
                if (wCrc != NtPkgHead2Crc16((const uint32_t *)&oNtPkgHead)) {
                    continue; // 非法包
                }
                oNtPkgHead.wCrc = htons(wCrc);

                //oNtPkgHead
                CNetBlockMsg::NetToHost(oNtPkgHead);
                if (oNtPkgHead.bStartFlag == PH_START_FLAG)
                {
                    nSize = oNtPkgHead.wLen;
                    //如果是无效的包头
                    if ( nSize > NET_PACKAGE_MAX || nSize <= 0)
                    {
                        //移去无效数据
                        m_recvBuff.Delete(1);

                        //初始再找标识信息
                        nPos = -1;
                        nSize = 0;
                    }
                    else
                    {//有效处理
                        if( nSize > nBufferLen)
                        {//等待一个完整的数据包
                            bWaitForAllPackage = true;
                            break;
                        }

                        //处理一个现有的完整包,包括包头信息
                        //nSize -= PH_HEAD_LEN;
                        PBYTE pTmpData = new BYTE[nSize + 1];
                        memset(pTmpData, 0, nSize + 1);
                        m_recvBuff.Read(pTmpData, nSize);
                        pTmpData[nSize] =  NULL;

                        HX_LOG("oNtPkgHead.wCmd=0x%X,(NET_SINGAL_HEARTBEAT=0x%X\n)",oNtPkgHead.wCmd,NET_SINGAL_HEARTBEAT);

                        //如果是心跳包则，直接不处理
                        if (NET_SINGAL_HEARTBEAT == oNtPkgHead.wCmd
                            || BU_QUOTE_CTRL_QUOTE == oNtPkgHead.wCmd) {//

                            HX_LOG("+++++++++++++++++++++change heart+++++++++++++++++\n)");
                            delete []pTmpData;
                            //处理下一个包
                            nPos = -1;
                            continue;
                        }

                        LPPackageData lpData = new PackageData;
                        lpData->oNtPkgHead = oNtPkgHead;
                        lpData->pData = pTmpData;
                        lpData->nSize = nSize;
                        m_sqPackage.push(lpData);
                        notify_event();

                        nPos = -1;
                    }
                }
            }

            if( bWaitForAllPackage)
            {
                break;
            }
        }
    }
    else if( m_nPackType == 1 )
    {
        //TODO: 行情包头解开之后的处理
        while(m_recvBuff.GetBufferLen() >= sizeof(PacketHead))
        {
            bool bWaitForAllPackage = false;
            //查找合法的数据包
            //            myPrintf("----------------------START------------------------------\n");
            for(int nPos = 0; nPos < m_recvBuff.GetBufferLen(); ++nPos)
            {
                //                myPrintf("(begin data) recv [%d]\n", m_recvBuff.GetBufferLen());
                int nSize = 0;
                int nBufferLen = m_recvBuff.GetBufferLen();
                //                myPrintf("recv buff len=[%d]\n", m_recvBuff.GetBufferLen());
                if(nBufferLen < sizeof(PacketHead))
                {//等待数据整合
                    bWaitForAllPackage = true;
                    printBusLogEx("bWaitForAllPackage recv [%d]\n", m_recvBuff.GetBufferLen());
                    break;
                }
                
                //找到有效的包开头位置
                //RemovDataUntilValidHeadFlag(m_recvBuff);
                //                nBufferLen = m_recvBuff.GetBufferLen();
                //                if( nBufferLen < PH_HEAD_LEN)
                //                {//等待数据整合
                //                    bWaitForAllPackage = true;
                //                    break;
                //                }
                //HX_LOG("recv quote thread\r\n ");
                //取包头信息
                PacketHead oCommPkgHead;
                memcpy(&oCommPkgHead, m_recvBuff.GetBuffer(nPos), sizeof(PacketHead));
                
                
                nSize = ntohl(oCommPkgHead.m_uiDataLen);
                    //                    printBusLogEx("整包长度=[%d]=[0x%x]\n", nSize, nSize);
                    //                    myPrintf("Pkg len=[%d] recv buff len=[%d]\n", nSize, m_recvBuff.GetBufferLen());
                    //                    myPrintf"\n oCommPkgHead.nDatalen [%d] oCommPkgHead.nVersion[%d]\n", oCommPkgHead.nDatalen, oCommPkgHead.nVersion);
                    //如果是无效的包头
                if ( nSize > NET_PACKAGE_MAX || nSize < 0)
                {
                    //移去无效数据
                    m_recvBuff.Delete(1);
                    //初始再找标识信息
                    nPos = -1;
                    m_connectStatusType = CST_UNCONNECTED;
                    printBusLogEx("超过设置包长度=[%d]=[0x%x]！ 正在关闭socket...............", nSize, nSize);
                    return 1;
                }
                else
                {//有效处理
                    if( nSize + sizeof(PacketHead) > nBufferLen)
                    {//等待一个完整的数据包
                        //                            myPrintf("bWaitForAllPackage recv [%d]\n", m_recvBuff.GetBufferLen());
                        bWaitForAllPackage = true;
                        break;
                    }
                    else if(nSize == 0)
                    {
                        m_recvBuff.Delete(sizeof(PacketHead));
                        nPos = -1;
                        //                            myPrintf("nSize=0 continue recvDataLen[%d]\n", m_recvBuff.GetBufferLen());
                        continue;
                    }
                    //处理一个现有的完整包,包括包头信息
                    //nSize -= PH_HEAD_LEN;
                    int nTmpDataSize = nSize + sizeof(PacketHead) + 1;
                    PBYTE pTmpData = new BYTE[nTmpDataSize];
                    //                        for(int i = 0; i < nTmpDataSize; i ++)
                    //                            pTmpData[i] = 0;
                    memset(pTmpData, 0, nTmpDataSize);
                    
                    m_recvBuff.Read(pTmpData, nSize + sizeof(PacketHead));
                    
                    //                        myPrintf("cur recv recvDataLen[%d]\n", m_recvBuff.GetBufferLen());
                    //                        DebugHex((unsigned char*)pTmpData, nTmpDataSize);
                    //如果是心跳包则，直接不处理
                    if (nSize <= 3) {//
                        if (m_nQuoteLoginFlag == 0) { //如果没有登录
                            Send(USERPWDVALID, 0, NULL, 0);
                        }
                        delete []pTmpData;
                        nPos = -1;
                        printBusLogEx("行情服务心跳数据!\n");
                        continue;
                    }
                    SRequestData oNormalHead = *(SRequestData*)(pTmpData + sizeof(PacketHead));
                    
                    //队列长度大于30后，丢实时行情
                    if (PUSH_REALDATA == oNormalHead.m_usType && m_sqQuotePackage.size() > quote_queue_size) {
                        delete []pTmpData;
                    } else{
                        LPQuotePackageData lpQuoteData = new QuotePackageData;
                        memset(lpQuoteData, 0, sizeof(PackageData));
                        lpQuoteData->oCommPkgHead = oCommPkgHead;
                        lpQuoteData->pData = pTmpData;
                        lpQuoteData->nSize = nTmpDataSize;
                        m_sqQuotePackage.push(lpQuoteData);
                        
                        
                        notify_event();
                    }
                    nPos = -1;
                
                }//如果是无效的包头
                //                    else {
                //                        
                //                    }
            }//if (oCommPkgHead.nVersion == 1)
                
            if( bWaitForAllPackage)
            {
                //                myPrintf("bWaitForAllPackage [%d]\n", m_recvBuff.GetBufferLen());
                break;
            }

        }//for(int nPos = 0; nPos < m_recvBuff.GetBufferLen(); ++nPos)

    }
    else if (m_nPackType == 2)
    {
        //TODO: balance包头解开之后的处理
        while(m_recvBuff.GetBufferLen() >= sizeof(SCommPkgHead)) {
            bool bWaitForAllPackage = false;
            //查找合法的数据包
            for(int nPos = 0; nPos < m_recvBuff.GetBufferLen(); ++nPos) {
                int nSize = 0;
                int nBufferLen = m_recvBuff.GetBufferLen();
                if(nBufferLen < sizeof(SCommPkgHead)) {//等待数据整合
                    bWaitForAllPackage = true;
                    printBusLogEx("bWaitForAllPackage recv [%d]\n", m_recvBuff.GetBufferLen());
                    break;
                }
                //取包头信息
                SCommPkgHead oCommPkgHead;
                memcpy(&oCommPkgHead, m_recvBuff.GetBuffer(nPos), sizeof(SCommPkgHead));
                if (oCommPkgHead.nVersion == 1) {
                    nSize = ntohl(oCommPkgHead.nDatalen);
                    //如果是无效的包头
                    if ( nSize > NET_PACKAGE_MAX || nSize < 0) {
                        //移去无效数据
                        m_recvBuff.Delete(1);
                        //初始再找标识信息
                        nPos = -1;
                        m_connectStatusType = CST_UNCONNECTED;
                        printBusLogEx("超过设置包长度=[%d]=[0x%x]！ 正在关闭socket...............", nSize, nSize);
                        return 1;
                    }  else  {//有效处理
                        if( nSize + sizeof(SCommPkgHead) > nBufferLen) {//等待一个完整的数据包
                            //                            myPrintf("bWaitForAllPackage recv [%d]\n", m_recvBuff.GetBufferLen());
                            bWaitForAllPackage = true;
                            break;
                        } else if(nSize == 0) {
                            m_recvBuff.Delete(sizeof(SCommPkgHead));
                            nPos = -1;
                            //                            myPrintf("nSize=0 continue recvDataLen[%d]\n", m_recvBuff.GetBufferLen());
                            continue;
                        }
                        //处理一个现有的完整包,包括包头信息
                        //nSize -= PH_HEAD_LEN;
                        int nTmpDataSize = nSize + sizeof(SCommPkgHead) + 1;
                        PBYTE pTmpData = new BYTE[nTmpDataSize];
                        memset(pTmpData, 0, nTmpDataSize);
                        
                        m_recvBuff.Read(pTmpData, nSize + sizeof(SCommPkgHead));

                        if (nSize <= 3) {
                            delete []pTmpData;
                            nPos = -1;
                            printBusLogEx("Balan服务心跳数据!\n");
                            continue;
                        }
                        
                        LPPackageData lpData = new PackageData;
                        memset(lpData, 0, sizeof(PackageData));
                        lpData->oCommPkgHead = oCommPkgHead;
                        lpData->pData = pTmpData;
                        lpData->nSize = nTmpDataSize;
                        m_sqPackage.push(lpData);
                        notify_event();
                        nPos = -1;
                    }
                }
            }
            if( bWaitForAllPackage) break;
        }
    }
	return true;
}



void CNetSocket::FireSessionStatus()
{
	//TODO:发生了关闭事件 or  connet
	//try
	{
		if(NULL != m_pfFireSessionStatus)
		{
			m_pfFireSessionStatus(m_nSid, m_connectStatusType);
		}
	}
	//catch(...){}
}
/*******************************
 发生了关闭事件或主动关闭了SOCKET
 ********************************/
bool CNetSocket::OnClose()
{
	m_connectStatusType = CST_UNCONNECTED;
	ClearSocket();
	FireSessionStatus();
	return false;
}
/*******************************
 发生了连接成功事件
 ********************************/
bool CNetSocket::OnConnect()
{
	m_connectStatusType = CST_CONNECTED;
	//TODO:发生了连接成功事件
	FireSessionStatus();
	return false;
}

int CNetSocket::loop_read()
{ //return: 0,suc; <0, err
	if(OnRead())
		return 0;
	else
		return -1;
}
int CNetSocket::loop_write(void)
{//return: 0,suc; <0, err

	if(SendData())
		return 0;
	else
		return -1;
}

#define HX_HEARTBEAT_INTERVAL		5	// second
int CNetSocket::loop_heartbeat(void)
{
	uint8_t szBuf[sizeof(HeartBeatRequest)];
	uint8_t * pbBuf;
	if((time(NULL) - m_send_heartbeat_time > HX_HEARTBEAT_INTERVAL) &&  CST_CONNECTED == GetConnectStatusType())
    {
        if (m_nPackType == 0) {
            pbBuf = szBuf;
            // 心跳
            if (0 != AddDWord(&pbBuf, 0, GetTickCount())) {
                return -1;
            }

            // 可能要加锁访问
            Send(NET_SINGAL_HEARTBEAT, /*pThis->GetNextSeq()*/0, szBuf, sizeof(HeartBeatRequest));
        }
        else if (m_nPackType == 1){
            if (m_nQuoteLoginFlag == 1)
            Send(HEARTBEAT, 0, NULL, 0);
        }

        m_send_heartbeat_time = time(NULL);
        //HX_LOG("send heartbeat time=%u",m_send_heartbeat_time);
    }
    /*
    if (m_nPackType == 1) {
        if (m_nQuoteLoginFlag == 0) {
            if((time(NULL) - m_send_quote_login_time > 1) &&  CST_CONNECTED == GetConnectStatusType())
            {
                Send(35584, 0, NULL, 0);
            }
        }
    }
     */
	return 0;
}

int CNetSocket::main_loop(int timeout) //timeout: ms
{
    struct timeval local_timeout;
	fd_set readfds, writefds;
	int fdcount;
	int rc;

	if(m_hSocket == INVALID_SOCKET) return  -1;

	FD_ZERO(&readfds);
	FD_SET(m_hSocket, &readfds);
	FD_ZERO(&writefds);
	if(m_sendBuff.GetBufferLen() > 0){
		FD_SET(m_hSocket, &writefds);
	}
	if(timeout >= 0){
		local_timeout.tv_sec = timeout/1000;
		local_timeout.tv_usec = (timeout-local_timeout.tv_sec*1000)*1000;
	}else{
		local_timeout.tv_sec = 1;
		local_timeout.tv_usec = 0;
	}

	fdcount = select(m_hSocket+1, &readfds, &writefds, NULL, &local_timeout);
	if(fdcount == -1){
		HX_LOG("select err,fdcount=%d,errno=%d",fdcount,errno);
		return -3; // FIXME what error to return?
	}
    else{
		if(FD_ISSET(m_hSocket, &readfds)){
			rc = loop_read();
			if(rc){
				HX_LOG("loop_read err,ret=%d,errno=%d",rc,errno);
				OnClose();
				return rc;
			}
		}
		if(FD_ISSET(m_hSocket, &writefds)){
			rc = loop_write();
			if(rc){
				HX_LOG("loop_write err,ret=%d,errno=%d",rc,errno);
				OnClose();
				return rc;
			}
		}
	}
	loop_heartbeat();
	return 0;
}

bool CNetSocket::is_should_exit(void)
{
	return m_bShouldExit;
}

typedef enum te_net_state
{
	NET_STATE_UNAVARIABLE,
	NET_STATE_AVARIABLE
}te_net_state;

//TODO: Main Loop
void* net_thread_run(void* para)
{
	CNetSocket* net_soc = (CNetSocket*)para;
	HX_LOG("net_soc->is_should_exit=%d,",net_soc->is_should_exit());
    //int count = 0;
	while( !net_soc->is_should_exit() )
    {
        switch(net_soc->m_net_state)
        {
            case NET_STATE_AVARIABLE:
                if(net_soc->GetConnectStatusType() != CST_CONNECTED)
                {
                    HX_LOG("net_soc->main_loopnet_soc->Connect()");
                    if(!net_soc->Connect())
                    {
                        sleep(3);
                    }
                }
                break;
            case NET_STATE_UNAVARIABLE:
                if(net_soc->GetConnectStatusType() == CST_CONNECTED)
                {
                    net_soc->OnClose();
                }
                break;
            default:
                break;
        }
        if(net_soc->GetConnectStatusType()  == CST_CONNECTED)
            net_soc->main_loop(2000);

        usleep(200);
    }
	net_soc->OnClose();
	net_soc->m_is_start = 0;
	//net_soc->ClearSocket();
	return NULL;
}
/*
 pthread_cond_t g_pthread_cond = PTHREAD_COND_INITIALIZER;
 pthread_mutex_t g_pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
 */
int CNetSocket::notify_event(void)
{
	if(m_pfNotifyEvent)
    {
        return m_pfNotifyEvent(m_nSid);
    }
#ifdef EVENT_THREAD_SUPPORT
    pthread_mutex_lock(&m_pthread_mutex);
    pthread_cond_signal(&m_pthread_cond);
    pthread_mutex_unlock(&m_pthread_mutex);
#endif
    return 0;
}
int CNetSocket::ProcessEvent(int nSid)
{
	return process_event(this);
}
int CNetSocket::NotifyNetState(int net_state)
{
	return m_net_state = net_state;
}
/*
 int process_event(void*para)
 {
 CNetSocket * net_soc = (CNetSocket*)para;
 return net_soc->FireDataThreadProc(para);
 }*/
void* event_thread_run(void* para)
{
    int ret;
    CNetSocket* net_soc = (CNetSocket*)para;
    while(1)
    {
        pthread_mutex_lock(&net_soc->m_pthread_mutex);
        pthread_cond_wait(&net_soc->m_pthread_cond,&net_soc->m_pthread_mutex); //unlock->wait->lock
        pthread_mutex_unlock(&net_soc->m_pthread_mutex);
        while((ret = CNetSocket::process_event(para))==0);
    }
    return NULL;
}
void CNetSocket::Start()
{
	pthread_t net_thread;
	int ret;
	m_bShouldExit = false;
	if(m_is_start)
		return;
	ret = pthread_create(&net_thread,NULL,net_thread_run,this);
    if(ret != 0)
    {
        return;
    }
#ifdef EVENT_THREAD_SUPPORT
	ret = pthread_create(&net_thread,NULL,event_thread_run,this);
    if(ret != 0)
    {
        return;
    }
#endif
	m_is_start = 1;
}

void CNetSocket::Stop()
{
	m_bShouldExit = true;
}

#if 0
/*******************************
 启动监听SOCKET线程
 ********************************/
void CNetSocket::Start()
{
	Stop();
	m_bShouldExit = false;

	if( m_hHeartBeatThread != NULL )
	{
		CloseHandle(m_hHeartBeatThread);
		m_hHeartBeatThread = NULL;
	}

	if( m_hFireThread != NULL )
	{
		CloseHandle(m_hFireThread);
		m_hFireThread = NULL;
	}

	if( m_hThread != NULL )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_hHeartBeatThread=
    (HANDLE)_beginthreadex(NULL,	// Security
                           0,								// Stack size - use default
                           HeartBeatThreadProc,     			// Thread fn entry point
                           (void*) this,
                           CREATE_SUSPENDED,				// Init flag
                           &m_dwHeartBeatThreadId);			// Thread address

	m_hFireThread=
    (HANDLE)_beginthreadex(NULL,	// Security
                           0,								// Stack size - use default
                           FireDataThreadProc,     			// Thread fn entry point
                           (void*) this,
                           CREATE_SUSPENDED,				// Init flag
                           &m_dwFireThreadId);					// Thread address


	m_hThread=
    (HANDLE)_beginthreadex(NULL,	// Security
                           0,								// Stack size - use default
                           SocketThreadProc,     			// Thread fn entry point
                           (void*) this,
                           CREATE_SUSPENDED,				// Init flag
                           &m_dwThreadId);					// Thread address

	ResumeThread(m_hHeartBeatThread);
	ResumeThread(m_hFireThread);
	ResumeThread(m_hThread);
}
/*******************************
 关闭监听SOCKET线程
 ********************************/
void CNetSocket::Stop()
{
	m_bShouldExit = true;
	//如果是正处于连接状态，可能的情况是 因为连接超时时间为6S，需要等待这个时间后才返回。

	WaitForSingleObject(m_hHeartBeatThread, 200);
	TerminateThread(m_hHeartBeatThread,1);
	WaitForSingleObject(m_hFireThread, 200);
	TerminateThread(m_hFireThread,1);
	WaitForSingleObject(m_hThread, 200);
	TerminateThread(m_hThread,1);
	//1
	//WaitForSingleObject(m_hThread, INFINITE);
	//2
	////DWORD dwWaitCnt = 0;

	////while(WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 100))
	////{
	////	MSG msg;
	////	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	////	{
	////		TranslateMessage(&msg);
	////		DispatchMessage(&msg);
	////	}
	////	++ dwWaitCnt;
	////	if(dwWaitCnt > 60)
	////	{
	////		TerminateThread(m_hThread,1);
	////		break;
	////	}
	////}

	m_connectStatusType = CST_UNCONNECTED;

	if (NULL != m_hHeartBeatThread)
	{
		CloseHandle(m_hHeartBeatThread);
		m_hHeartBeatThread = NULL;
	}

	if (NULL != m_hFireThread)
	{
		CloseHandle(m_hFireThread);
		m_hFireThread = NULL;
	}

	if (NULL != m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	ClearSocket();
}


unsigned int CNetSocket::HeartBeatThreadProc(LPVOID lParam)
{
	uint8_t szBuf[sizeof(HeartBeatRequest)];
	uint8_t * pbBuf;
	CNetSocket* pThis = reinterpret_cast<CNetSocket*>(lParam);
	time_t tLastHeartBeat = time(NULL);

	while(!pThis->m_bShouldExit)
	{
		if (time(NULL) > (tLastHeartBeat + 5) && CST_CONNECTED == pThis->GetConnectStatusType()) // 5秒钟一次心跳
		{
			pbBuf = szBuf;
			// 心跳
			if (0 != AddDWord(&pbBuf, 0, GetTickCount())) {
				return false;
			}

			// 可能要加锁访问
			pThis->Send(NET_SINGAL_HEARTBEAT, /*pThis->GetNextSeq()*/0, szBuf, sizeof(HeartBeatRequest));
			tLastHeartBeat = time(NULL);
		}

		Sleep(200);
	}

	return 0;
}
#endif

/*
 bool ZLIBUnCompress(BYTE ** pOutput, uint32_t * pdwOutLen, const BYTE * pInput, uint32_t dwInLen)
 {
 int iRet;
 uint32_t dwBufLen = (dwInLen << 1); // 默认直接取输入缓冲区的2倍
 BYTE * pBuf = NULL;

 assert(pOutput && pdwOutLen && pInput && dwInLen > 0);

 do {
 delete []pBuf;

 dwBufLen <<= 1; // 每次缓冲区翻倍,第一次直接取了输入缓冲区的4倍大小
 // 预留一个包头的长度,以减少组装包头带来的数据复制
 pBuf = new BYTE[sizeof(NtPkgHeadAlign) + dwBufLen + 1];

 iRet = uncompress((Bytef *)pBuf + sizeof(NtPkgHeadAlign), (uLongf *)&dwBufLen, (const Bytef *)pInput, (uLong)dwInLen);
 } while (Z_BUF_ERROR == iRet);

 if (Z_OK == iRet) {
 *pOutput = pBuf; // 返回分配的缓冲区,前20个字节预留给包头
 *pdwOutLen = dwBufLen; // 返回实际解压的数据长度
 return true;
 }

 return false;
 }
 */


bool ZLIBUnCompress(BYTE ** pOutput, uint32_t * pdwOutLen, const BYTE * pInput, uint32_t dwInLen)
{
#if 0
	int iRet;
	uint32_t dwBufLen;
	BYTE * pBuf;

	assert(pOutput && pdwOutLen && pInput && dwInLen > sizeof(uint32_t));

	memcpy(&dwBufLen, pInput, sizeof(uint32_t)); // 取出真实数据的长度
	dwBufLen = ntohl(dwBufLen);

	// 指向实际压缩后的数据
	pInput += sizeof(uint32_t);
	dwInLen -= sizeof(uint32_t);

	assert(dwInLen == dwBufLen);

	// 分配缓冲区
	pBuf = new BYTE[sizeof(NtPkgHeadAlign) + dwBufLen + 1];

	iRet = uncompress((Bytef *)pBuf + sizeof(NtPkgHeadAlign), (uLongf *)&dwBufLen, (const Bytef *)pInput, (uLong)dwInLen);
	if (Z_OK != iRet) {
		delete []pBuf;
		return false;
	}

	*pOutput = pBuf; // 返回分配的缓冲区,前20个字节预留给包头
	*pdwOutLen = dwBufLen; // 返回实际解压的数据长度
#endif
	return true;
}



void CNetSocket::UnicodeToUTF_8(char* pOut,wchar_t* pText)

{
    
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    
    char* pchar = (char *)pText;
    
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    
    return;  
    
}

unsigned int CNetSocket::FireDataThreadProc(LPVOID lParam)
{
	return 0;
}


int CNetSocket::process_event(void*lParam)
{
    //return 0,suc;  1,no data to process
    CNetSocket* pThis = reinterpret_cast<CNetSocket*>(lParam);
    //HX_LOG("process data,m_bShouldExit=%d",pThis->m_bShouldExit);
    //while(!pThis->m_bShouldExit)
    if(!pThis->m_bShouldExit)
    {
        if (pThis->m_nPackType == 0){
            return pThis->ProcessTradeData(pThis);
        } else if (pThis->m_nPackType == 1){
            return pThis->ProcessQuoteData(pThis);
        } else if (pThis->m_nPackType == 2){
            return pThis->ProcessBalaceData(pThis);
        }
    }
    return 0;
}

int CNetSocket::ProcessTradeData(void *lParam)
{
    CNetSocket* pThis = reinterpret_cast<CNetSocket*>(lParam);
    LPPackageData lpPs = NULL;
    bool bRet = pThis->m_sqPackage.pop(lpPs);
    if (!bRet)
    {
        //pThis->m_sqPackage.wait();//Sleep(10);
        //HX_LOG("no package to process");
        return 1;
    }
    if(NULL != pThis->m_pfFireBlockMessage)
    {
        //TODO:包头解开之后的处理
        if(BU_CA_LOGIN_USER == lpPs->oNtPkgHead.wCmd) {
            //保存服务返回的Key
            //pThis->m_sKey
            if(lpPs->nSize >= sizeof(LoginResponse)) {
                LoginResponse* pstLoginResponse = (LoginResponse*)(lpPs->pData + sizeof(NtPkgHeadAlign));
                
                pstLoginResponse->iRetCode = ntohl(pstLoginResponse->iRetCode);
                pstLoginResponse->dwUID = ntohl(pstLoginResponse->dwUID);
                
                if(pstLoginResponse->iRetCode >= 0)
                {
                    PlainLoginResp stPLR;
                    uint32_t dwCipherDataLen = sizeof(PlainLoginResp);
                    int iRet = SymDecrypt(GET_ENCRYPT_FLAG(lpPs->oNtPkgHead.bEncryptFlag), pstLoginResponse->sCipherData,
                                          lpPs->nSize - sizeof(NtPkgHeadAlign) - sizeof(LoginResponse),
                                          pThis->m_sKey, &stPLR, &dwCipherDataLen);
                    if (0 != iRet)
                    {
                        //TODO:writeLog
                        //continue;
                        HX_LOG("login rsp,SymDecrypt err,ret=%d",iRet);
                        return 0;
                    }
                    stPLR.dwUID = ntohl(stPLR.dwUID);
                    if(stPLR.dwUID == pstLoginResponse->dwUID)
                    {
                        pThis->m_dwUid = stPLR.dwUID;
                        memcpy(pThis->m_sKey, stPLR.sCommKey, sizeof(pThis->m_sKey));
                        HX_LOG("login suc,uid=0x%X",stPLR.dwUID);
                        //成功数据体
                    }else
                    {
                        //TODO:writeLog
                        //continue;
                        HX_LOG("login rsp,dwUID not match");
                        return 0;
                    }
                }
                else
                {
                    HX_LOG("login failed,retcode=%d",pstLoginResponse->iRetCode);
                }
                pThis->m_pfFireBlockMessage(pThis->m_nSid, lpPs->oNtPkgHead.wCmd, lpPs->oNtPkgHead.wSeq, lpPs->pData, sizeof(NtPkgHeadAlign) + sizeof(LoginResponse));
            }
            else
            {
                HX_LOG("login rsp,(lpPs->nSize < sizeof(LoginResponse))");
                //TODO:writeLog
            }
        } else {
            //if(lpPs->oNtPkgHead.wCmd >= FLAG_START_BU_QUOTE_DATA
            //   && lpPs->oNtPkgHead.wCmd <= FLAG_END_BU_BIZ)
            if (pThis->m_nPackType == 0)
            {
                {
                    //数据解密
                    uint8_t * pKey;
                    
                    if (BU_QUOTE_DATA_QUOTE == lpPs->oNtPkgHead.wCmd) {
                        pKey = pThis->m_sCltKey;
                    } else {
                        /*
                         uint8_t sKey[CRYPT_KEY_LEN];
                         KeyContent kc;
                         kc.SetKey(pThis->m_sKey);
                         kc.nUid = pThis->m_dwUid;
                         Md5HashBuffer(sKey, &kc, sizeof(KeyContent));
                         */
                        uint8_t sKey[CRYPT_KEY_LEN];
                        memset(sKey, 0, CRYPT_KEY_LEN);
                        Md5HashBuffer(sKey, ORIGINALPWD, strlen(ORIGINALPWD));
                        
                        pKey = sKey;
                    }
                    
                    uint32_t dwDecryptDataLen = lpPs->nSize + 1024;
                    LPBYTE pDecryptData = new BYTE[dwDecryptDataLen];
                    //lpPs->oNtPkgHead.bEncryptFlag
                    int iRet = SymDecrypt(GET_ENCRYPT_FLAG(lpPs->oNtPkgHead.bEncryptFlag), lpPs->pData + sizeof(lpPs->oNtPkgHead),
                                          lpPs->nSize - sizeof(lpPs->oNtPkgHead), pKey, pDecryptData + sizeof(lpPs->oNtPkgHead), &dwDecryptDataLen);
                    if (0 != iRet)
                    {
                        //TODO:writeLog
                        //continue;
                        HX_LOG("SymDecrypt err,oNtPkgHead.wCmd=0x%X",lpPs->oNtPkgHead.wCmd);
                        return 0;
                    }
                    
                    delete []lpPs->pData;
                    
                    // 注意,写合包业务的时候,要先合包再进行解压缩
                    //合并成完整的业务包
                    if (lpPs->oNtPkgHead.bFrag) {
                        lpPs->pData = pDecryptData; // 前20个字节被保留
                        lpPs->nSize = dwDecryptDataLen; // 只含实际的数据长度,不包括包头
                        // 等待所有分包都收完
                        if (pThis->PushBizPkg(lpPs) <= 0) {
                            //continue;
                            HX_LOG("PushBizPkg<=0");
                            return 0;
                        } else {
                            pDecryptData = lpPs->pData;
                            dwDecryptDataLen = lpPs->nSize;
                        }
                    }
                    
                    int iCompressFlag = GET_COMPRESS_FLAG(lpPs->oNtPkgHead.bEncryptFlag);
                    // 如果有压缩标志则进行解压缩
                    if (ZLIB == iCompressFlag) {
                        LPBYTE pUncompressData;
                        uint32_t dwUncompressLen;
                        
                        bool bRet = ZLIBUnCompress(&pUncompressData, &dwUncompressLen, pDecryptData + sizeof(lpPs->oNtPkgHead), dwDecryptDataLen);
                        if (!bRet) {
                            // 解压缩失败
                            //TODO:writeLog
                            //continue;
                            HX_LOG("ZLIBUnCompress err,ret=%d",bRet);
                            return 0;
                        }
                        
                        // 释放原来解密的缓冲区,指向解压分配的缓冲区
                        delete []pDecryptData;
                        pDecryptData = pUncompressData;
                        dwDecryptDataLen = dwUncompressLen;
                    } else if (NO_COMPRESS != iCompressFlag) {
                        // 还不支持该种解压算法
                        //TODO:writeLog
                        //continue;
                        HX_LOG("compress flag not support,flag=0x%X",iCompressFlag);
                        return 0;
                    }
                    
                    pDecryptData[sizeof(lpPs->oNtPkgHead) + dwDecryptDataLen] = '\0';
                    lpPs->pData = pDecryptData;
                    memcpy(lpPs->pData, &lpPs->oNtPkgHead, sizeof(lpPs->oNtPkgHead));
                    lpPs->nSize = sizeof(lpPs->oNtPkgHead) + dwDecryptDataLen;
                }
                if (BU_QUOTE_DATA_QUOTE == lpPs->oNtPkgHead.wCmd && NULL != pThis->m_pfQuoteBlockMessage) {
                    pThis->m_pfQuoteBlockMessage(pThis->m_nSid, lpPs->oNtPkgHead.wCmd, lpPs->oNtPkgHead.wSeq, lpPs->pData, lpPs->nSize);
                } else {
                    pThis->m_pfFireBlockMessage(pThis->m_nSid, lpPs->oNtPkgHead.wCmd, lpPs->oNtPkgHead.wSeq, lpPs->pData, lpPs->nSize);
                }
            }
    
        }
    }
    delete[] lpPs->pData;
    lpPs->pData = NULL;
    delete lpPs;
    lpPs = NULL;
    
    return 0;
}

//TODO:处理行情包
int CNetSocket::ProcessQuoteData(void *lParam)
{
    CNetSocket* pThis = reinterpret_cast<CNetSocket*>(lParam);
    LPQuotePackageData lpQuotePs = NULL;
    bool bRet = pThis->m_sqQuotePackage.pop(lpQuotePs);
    if (!bRet)
    {
        //pThis->m_sqPackage.wait();//Sleep(10);
        //HX_LOG("no package to process");
        return 1;
    }
    
    unsigned short rspPkg;
    memcpy(&rspPkg, (void*)(lpQuotePs->pData + sizeof(PacketHead)), sizeof(unsigned short));
    
    switch (rspPkg) {
        case USERPWDVALID:
            {
                pThis->Send(INITMARKET, m_nSeq+1, NULL, 0);
                printBusLogEx("quote login response!");
                pThis->m_pfFireBlockMessage(pThis->m_nSid, USERPWDVALID, 0, NULL, 1);
                pThis->m_nQuoteLoginFlag = 1;
                
            }
            break;
        case HEARTBEAT:
            printBusLogEx("quote heartbeat response!");
            break;
        case REPORTDATA:
            {
                SSortData sortPkg;
                memcpy(&sortPkg, (void*)(lpQuotePs->pData + sizeof(PacketHead)), sizeof(SSortData));
                int allDataLen = 1000*sortPkg.m_usSize+100;
                char *allData = new char[allDataLen];
                memset((char *)allData, 0, sizeof(allData));
                int pos = sizeof(PacketHead)+sizeof(SSortData);
                SSortUnit *pUnit = (SSortUnit*)(lpQuotePs->pData+pos);
                for (int i = 0; i<sortPkg.m_usSize; i++) {
                    if (pUnit->m_CodeInfo.m_uiCode < 1) {
                        printBusLogEx("异常数据 error");
                        return false;
                    }
//                    printBusLogEx("商品基本信息 [%d]th:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",i,pUnit->m_CodeInfo.m_uiCode,pUnit->m_CodeInfo.m_usMarketType,pUnit->m_uiOpenPrice,pUnit->m_uiMaxPrice,pUnit->m_uiMinPrice,pUnit->m_uiNewPrice,pUnit->m_uiAverage,pUnit->m_uiVolume,pUnit->m_uiCurVol,pUnit->m_uiSum,pUnit->m_uiSellPrice,pUnit->m_uiBuyPrice,pUnit->m_uiSellVol,pUnit->m_uiBuyVol,pUnit->m_iRatio);
                    char strData[1000];
                    sprintf(strData,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",pUnit->m_CodeInfo.m_uiCode,pUnit->m_CodeInfo.m_usMarketType,pUnit->m_uiOpenPrice,pUnit->m_uiMaxPrice,pUnit->m_uiMinPrice,pUnit->m_uiNewPrice,pUnit->m_uiAverage,pUnit->m_uiVolume,pUnit->m_uiCurVol,pUnit->m_uiSum,pUnit->m_uiSellPrice,pUnit->m_uiBuyPrice,pUnit->m_uiSellVol,pUnit->m_uiBuyVol,pUnit->m_iRatio);
                     strcat(allData, strData);
                    pUnit++;
                }
                pThis->m_pfFireBlockMessage(pThis->m_nSid,REPORTDATA,sortPkg.m_usIndex,(PBYTE)allData,lpQuotePs->nSize - pos);
                delete [] allData;
            }
            break;
            
        case INITMARKET:
            {
                SInitMarket initPkg;
                int pos = sizeof(PacketHead);
                memcpy(&initPkg, (void*)(lpQuotePs->pData + pos), sizeof(SInitMarket));
                
//                pThis->m_pfFireBlockMessage(pThis->m_nSid,INITMARKET,NULL,(PBYTE)(lpQuotePs+pos),lpQuotePs->nSize - pos);
                
                SInitMarket* pInit = (SInitMarket*)(lpQuotePs->pData+pos);
//                printBusLogEx("%d,%d,%d,%s,%s",pInit->m_usType,pInit->m_usMarketType,pInit->m_uiTodayDate,pInit->m_ucPhases,pInit->m_ucMarkets);	//市场个数);

                pos += sizeof(SInitMarket);
                // 第一个market
                SMarketInfo *pMarket = (SMarketInfo*)(lpQuotePs->pData+pos);
                
                int allDataLen = 1000*pMarket->m_usStocks*pInit->m_ucMarkets;
                char *allStockName = new char[allDataLen];
                char *allStockData = new char[allDataLen];
                for (int i = 0 ; i < pInit->m_ucMarkets; i++) {
                    
                    pos += sizeof(SMarketInfo);
                    
                    SStockInfo *pStock = (SStockInfo*)(lpQuotePs->pData+pos);
                    
                    for (int j = 0; j < pMarket->m_usStocks; j++) {
//                        printf("=====InitMarket=====\n%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s\n",pStock->m_uiCode,pStock->m_usMarketType,pStock->m_uiPrevClose,pStock->m_ui5DayVol,pStock->m_uiUpLimit,pStock->m_uiDownLimit,pStock->m_uiShares,pStock->m_uiReserves,pStock->m_sAsset,pStock->m_acName,pStock->m_acSpell);
                        char stockName[2000];
                        char stockData[2000];
                        
                        sprintf(stockName,"%s\n",pStock->m_acName);
                        strcat(allStockName, stockName);
                        
                        sprintf(stockData,"%d,%d,%d,%d,%d,%d,%d,%d,%d\n",pStock->m_uiCode,pStock->m_usMarketType,pStock->m_uiPrevClose,pStock->m_ui5DayVol,pStock->m_uiUpLimit,pStock->m_uiDownLimit,pStock->m_uiShares,pStock->m_uiReserves,pStock->m_sAsset);
                        strcat(allStockData, stockData);
                        
                        pStock++;
                    }
                    pos += sizeof(SStockInfo)*pMarket->m_usStocks;
                    pMarket = (SMarketInfo*)(lpQuotePs->pData+pos);
                }
                pThis->m_pfFireBlockMessage(pThis->m_nSid, pInit->m_usType, m_nSeq, (PBYTE)allStockName, lpQuotePs->nSize - pos);
                pThis->m_pfFireBlockMessage(pThis->m_nSid, pInit->m_usType, m_nSeq, (PBYTE)allStockData, lpQuotePs->nSize - pos);
                delete [] allStockName;
                delete [] allStockData;
                
            }
            break;
        case HISKDATAFIRST + MINUTE1_HISK:
        case HISKDATAFIRST + MINUTES5_HISK:
        case HISKDATAFIRST + MINUTES10_HISK:
        case HISKDATAFIRST + MINUTES15_HISK:
        case HISKDATAFIRST + MINUTES30_HISK:
        case HISKDATAFIRST + MINUTES60_HISK:
        case HISKDATAFIRST + DAY_HISK:
        case HISKDATAFIRST + WEEK_HISK:
        case HISKDATAFIRST + MONTH_HISK:
        case HISKDATA + MINUTE1_HISK:
        case HISKDATA + MINUTES5_HISK:
        case HISKDATA + MINUTES10_HISK:
        case HISKDATA + MINUTES15_HISK:
        case HISKDATA + MINUTES30_HISK:
        case HISKDATA + MINUTES60_HISK:
        case HISKDATA + DAY_HISK:
        case HISKDATA + WEEK_HISK:
        case HISKDATA + MONTH_HISK:
            {
                SDownData downData;
                memcpy(&downData, (void*)(lpQuotePs->pData + sizeof(PacketHead)), sizeof(SDownData));
                
                SDownData* pDown = (SDownData*)(lpQuotePs->pData+sizeof(PacketHead));
                SHisKData* pHisK = (SHisKData*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData));
                if (!pDown) {
                    return 1;
                }
                if (pDown->m_usSize<=0) { // 没有数据
                    printBusLogEx("NO SHisKData count : %d",pDown->m_usSize);
                     if (downData.m_usType - HISKDATA > 0) {//后续请
                             pThis->m_pfFireBlockMessage(pThis->m_nSid, downData.m_usType - HISKDATA, downData.m_usIndex, NULL, 0);
                         }else{
                             pThis->m_pfFireBlockMessage(pThis->m_nSid, downData.m_usType - HISKDATAFIRST, downData.m_usIndex, NULL, 0);
                         }
                         break;
                }
                printBusLogEx("SHisKData count : %d",pDown->m_usSize);
                
                
                if (pHisK->m_usSize > KREQKDATACOUNT) {
                    printBusLogEx("SHisKUnit count : %d",pHisK->m_usSize);
                    printBusLogEx("SHisKUnit count Error!!!!!");
                    break;
                }
                printBusLogEx("SHisKUnit count : %d",pHisK->m_usSize);
                
                // first.
                //        SHisKUnit* pKUnit = (SHisKUnit*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData)+sizeof(SHisKData));
                SHisKUnit* pKUnit = NULL;
                int allKDataLen = 200*pHisK->m_usSize;
                char *allHisKData = new char[allKDataLen];
                for (int i = 0 ;i< pHisK->m_usSize ;i++)
                {
                    char hisKData[200];
                    pKUnit = (SHisKUnit*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData)+sizeof(SHisKData)+sizeof(SHisKUnit)*i);
                    printBusLogEx("The %dth HisKUnit:---%d,%d,%d,%d,%d,%d,%d,%d\n", i,pKUnit->m_uiDate,pKUnit->m_uiOpenPrice,pKUnit->m_uiMaxPrice,pKUnit->m_uiMinPrice,pKUnit->m_uiClosePrice,pKUnit->m_uiSum,pKUnit->m_uiVolume);
                    sprintf(hisKData,"%d,%d,%d,%d,%d,%d,%d\n",pKUnit->m_uiDate,pKUnit->m_uiOpenPrice,pKUnit->m_uiMaxPrice,pKUnit->m_uiMinPrice,pKUnit->m_uiClosePrice,pKUnit->m_uiSum,pKUnit->m_uiVolume);
                    strcat(allHisKData, hisKData);
                    pKUnit++;
                }
                
                
                if (m_nLastReqCode == downData.m_usType - HISKDATA +pHisK->CodeInfo.m_uiCode*100) {//后续请求
                    m_nLastReqCode = pHisK->CodeInfo.m_uiCode*100 + downData.m_usType - HISKDATA;
                    pThis->m_pfFireBlockMessage(pThis->m_nSid, downData.m_usType - HISKDATA, downData.m_usIndex, (PBYTE)allHisKData, lpQuotePs->nSize - sizeof(PacketHead)-sizeof(SDownData)-sizeof(SHisKData));
//                    m_nRecvKUnit += pHisK->m_usSize;
//                    if (pHisK->m_usSize < KREQKDATACOUNT) m_nRecvKUnit = 0; // 没有后续
                }else{//新的请求
                    m_nLastReqCode = pHisK->CodeInfo.m_uiCode*100 + downData.m_usType - HISKDATAFIRST;
                    pThis->m_pfFireBlockMessage(pThis->m_nSid, downData.m_usType - HISKDATAFIRST, downData.m_usIndex, (PBYTE)allHisKData, lpQuotePs->nSize - sizeof(PacketHead)-sizeof(SDownData)-sizeof(SHisKData));
//                    m_nRecvKUnit += pHisK->m_usSize;
                }
                delete [] allHisKData;
            }
            
            break;
        case STKDEALDATA:  // 个股分析
            {
                SDealData dealData;
                memcpy(&dealData, (void*)(lpQuotePs->pData + sizeof(PacketHead)), sizeof(SDealData));
                int allDealLen = sizeof(SNowData)+sizeof(SVolPrice)*10+sizeof(STraceUnit)*242*12+sizeof(SRealMinsUnit)+1000;
                char *allDealData = new char[allDealLen];
                int pos = 0;
                pos += sizeof(PacketHead);
                SDealData* pDeal = (SDealData*)(lpQuotePs->pData+pos);
                
                char nowData[200];
                printBusLogEx("nowData - > %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",pDeal->m_stNowData.m_uiCode,pDeal->m_stNowData.m_usTime,pDeal->m_stNowData.m_uiOpenPrice,pDeal->m_stNowData.m_uiMaxPrice,pDeal->m_stNowData.m_uiMinPrice,pDeal->m_stNowData.m_uiNewPrice,pDeal->m_stNowData.m_uiAverage,pDeal->m_stNowData.m_uiVolume,pDeal->m_stNowData.m_uiCurVol,pDeal->m_stNowData.m_uiSum,pDeal->m_stNowData.m_uiBuyVol,pDeal->m_stNowData.m_uiSellVol,pDeal->m_stNowData.m_ucVolPrices);
                sprintf(nowData,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",pDeal->m_stNowData.m_uiCode,pDeal->m_stNowData.m_usTime,pDeal->m_stNowData.m_uiOpenPrice,pDeal->m_stNowData.m_uiMaxPrice,pDeal->m_stNowData.m_uiMinPrice,pDeal->m_stNowData.m_uiNewPrice,pDeal->m_stNowData.m_uiAverage,pDeal->m_stNowData.m_uiVolume,pDeal->m_stNowData.m_uiCurVol,pDeal->m_stNowData.m_uiSum,pDeal->m_stNowData.m_uiBuyVol,pDeal->m_stNowData.m_uiSellVol,pDeal->m_stNowData.m_ucVolPrices);
                strcat(allDealData, nowData);
                
                printBusLogEx("allDealData  nowData = %s",allDealData);
                
                pos += sizeof(SDealData);
                SVolPrice *pVolPrice = (SVolPrice*)(lpQuotePs->pData+pos);
                //
                for (int i = 0; i < pDeal->m_stNowData.m_ucVolPrices; i++) {
//                    printBusLogEx("==============[%d]==============",i);
//                    printBusLogEx("SVolPrice     = %d,%d\n",pVolPrice->m_uiPrice,pVolPrice->m_uiVolume);
                    char strData[200];
                    sprintf(strData,"%d,%d\n",pVolPrice->m_uiPrice,pVolPrice->m_uiVolume);
//                    printBusLogEx("strData       = %s",strData);
                    strcat(allDealData, strData);
//                    printBusLogEx("allDealData   = %s",allDealData);
                    pVolPrice++;
                }
                
                pos += pDeal->m_stNowData.m_ucVolPrices*sizeof(SVolPrice);
                
                STraceUnit* pTrace = (STraceUnit*)(lpQuotePs->pData+pos);
                for (int i = 0; i<pDeal->m_usTraceUnits; i++) {
                    printBusLogEx("个股分析 实时数据:%d,%d,%d,%d,%d,%d,%d,%d\n",pTrace->m_usTime,pTrace->m_uiNewPrice,pTrace->m_uiVolume,pTrace->m_uiBuyVol,pTrace->m_uiSellPrice,pTrace->m_uiBuyPrice,pTrace->m_uiSellPrice,pTrace->m_uiSum);
                    char strData[200];
                    sprintf(strData,"%d,%d,%d,%d,%d,%d,%d,%d\n",pTrace->m_usTime,pTrace->m_uiNewPrice,pTrace->m_uiVolume,pTrace->m_uiBuyVol,pTrace->m_uiSellPrice,pTrace->m_uiBuyPrice,pTrace->m_uiSellPrice,pTrace->m_uiSum);
                    strcat(allDealData, strData);
                    pTrace++;
                }
                
                pos += pDeal->m_usTraceUnits*sizeof(STraceUnit);
                
                SRealMinsUnit *pReal = (SRealMinsUnit*)(lpQuotePs->pData+pos);
                for (int i = 0; i<pDeal->m_usMinsUnits; i++) {
                    //            printBusLogEx(allDealData,"%d,%d,%d,%d,%d,%d,%d\n",pReal->m_usTime,pReal->m_uiNewPrice,pReal->m_uiVolume,pReal->m_uiKeepVol,pReal->m_uiAverage,pReal->m_uiBuyVol,pReal->m_uiSellVol);
                    char strData[1024];
                    sprintf(strData,"%d,%d,%d,%d,%d,%d,%d\n",pReal->m_usTime,pReal->m_uiNewPrice,pReal->m_uiVolume,pReal->m_uiKeepVol,pReal->m_uiAverage,pReal->m_uiBuyVol,pReal->m_uiSellVol);
                    strcat(allDealData, strData);
                    pReal++;
                }
                
                pThis->m_pfFireBlockMessage(pThis->m_nSid, dealData.m_usType, dealData.m_usIndex, (PBYTE)allDealData, lpQuotePs->nSize - sizeof(PacketHead));
                delete [] allDealData;
            }
            break;
        case HISKDATACURDATE:
            {
                // 当日K线
                SDownData* pDown = (SDownData*)(lpQuotePs->pData+sizeof(PacketHead));
                printBusLogEx("SDownData count : %d",pDown->m_usSize);
                if (!pDown) {
                    return 1;
                }
                SHisKData* pHisK = (SHisKData*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData));
                if (pHisK->m_usSize > KREQKDATACOUNT*5) {
                    printBusLogEx("SHisKData count : %d",pHisK->m_usSize);
                    printBusLogEx("SHisKData count Error!!!!!");
                    return 0;
                }
                printBusLogEx("SHisKData count : %d",pHisK->m_usSize);
                
                // first.
                //        SHisKUnit* pKUnit = (SHisKUnit*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData)+sizeof(SHisKData));
                SHisKUnit* pKUnit = NULL;
                int allKDataLen = 200*pHisK->m_usSize;
                char *allHisKData = new char[allKDataLen];
                for (int i = 0 ;i< pHisK->m_usSize ;i++)
                {
                    char hisKData[200];
                    pKUnit = (SHisKUnit*)(lpQuotePs->pData+sizeof(PacketHead)+sizeof(SDownData)+sizeof(SHisKData)+sizeof(SHisKUnit)*i);
                    printBusLogEx("The HISKDATACURDATE %dth HisKUnit:---%d,%d,%d,%d,%d,%d,%d,%d\n", i,pKUnit->m_uiDate,pKUnit->m_uiOpenPrice,pKUnit->m_uiMaxPrice,pKUnit->m_uiMinPrice,pKUnit->m_uiClosePrice,pKUnit->m_uiSum,pKUnit->m_uiVolume);
                    sprintf(hisKData,"%d,%d,%d,%d,%d,%d,%d\n",pKUnit->m_uiDate,pKUnit->m_uiOpenPrice,pKUnit->m_uiMaxPrice,pKUnit->m_uiMinPrice,pKUnit->m_uiClosePrice,pKUnit->m_uiSum,pKUnit->m_uiVolume);
                    strcat(allHisKData, hisKData);
                    pKUnit++;
                }
                
                //新的请求
//                m_nRecvKUnit += pHisK->m_usSize;
                pThis->m_pfFireBlockMessage(pThis->m_nSid,pDown->m_usType, pDown->m_usIndex-HISKDATAFIRST, (PBYTE)allHisKData, lpQuotePs->nSize - sizeof(PacketHead)-sizeof(SDownData)-sizeof(SHisKData));
                delete [] allHisKData;
                
            }
            break;
        default:
            break;
    }
    
    delete[] lpQuotePs->pData;
    lpQuotePs->pData = NULL;
    delete lpQuotePs;
    lpQuotePs = NULL;
    return 0;
}



int CNetSocket::ProcessBalaceData(void *lParam)
{
    CNetSocket* pThis = reinterpret_cast<CNetSocket*>(lParam);
    LPPackageData lpPs = NULL;
    bool bRet = pThis->m_sqPackage.pop(lpPs);
    if (!bRet)
    {
        //pThis->m_sqPackage.wait();//Sleep(10);
        //HX_LOG("no package to process");
        return 1;
    }
    int nHeadLen = sizeof(SCommPkgHead);
    pThis->m_pfFireBlockMessage(pThis->m_nSid, 0, 0, (uint8_t *)(lpPs->pData + nHeadLen), lpPs->nSize - nHeadLen);
    delete[] lpPs->pData;
    lpPs->pData = NULL;
    delete lpPs;
    lpPs = NULL;
    
    return 0;
}

int CNetSocket::SZCodeToZSMarket(unsigned int uiCode)
{
    unsigned short usMarketType = 0;
    
    // 通过代码值得出市场类型值
    int nFirstKey;
    nFirstKey = uiCode/1000;
    switch(nFirstKey)
    {
        case 100:   // 指数
            usMarketType = SZ_AStock;;
            break;
        case 602:   // 邮票
            usMarketType = SZ_Bond;;
            break;
        case 605:  //  邮资封片
            usMarketType = SZ_Right;
            break;
        case 903:  //  电话卡
            usMarketType = SZ_Fund;
            break;
        case 701:  //  钱币
            usMarketType = SZ_BStock;
            break;
        default:
            usMarketType = SZ_Other;
            //return -1;//错误代码
    }
    
    return usMarketType;
}

bool CNetSocket::SortFun(const LPPackageData p1, const LPPackageData p2)
{
	int wSeq1 = p1->oNtPkgHead.unHeadReserved.WordBuEx.wCurSeq;
	int wSeq2 = p2->oNtPkgHead.unHeadReserved.WordBuEx.wCurSeq;
    
	return wSeq1 < wSeq2 ? true : false;
}


int CNetSocket::PushBizPkg(LPPackageData & lpPs)
{
	// 将 wCmd 和 wSeq 组合起来当作key使用
	int iKey = (int)(lpPs->oNtPkgHead.wCmd << 16) | (int)(lpPs->oNtPkgHead.wSeq & 0x0000ffff);

	// 在收到第一个包的时候记录下时间
	if (0 == mapCutPkg[iKey].vecPkg.size()) {
		mapCutPkg[iKey].dwTickCount = GetTickCount();
	}

	mapCutPkg[iKey].vecPkg.push_back(lpPs);

	// 遍历一下map,看是否有超时的包,要清除
	map<int, CutPkg>::iterator iterMap;
	vector<LPPackageData>::iterator iter;
	for (iterMap = mapCutPkg.begin(); iterMap != mapCutPkg.end();) {
		if ((iterMap->second.dwTickCount + 600 * 1000) > GetTickCount()) { // 10分钟
			iterMap++;
			continue;
		}

		// 超时的分包
		for (iter = iterMap->second.vecPkg.begin(); iter != iterMap->second.vecPkg.end(); iter++)
		{
			// 释放数据
			delete [](*iter)->pData;
			delete (*iter);
		}

		iterMap->second.vecPkg.clear();
		mapCutPkg.erase(iterMap++);
	}

	if (mapCutPkg[iKey].vecPkg.size() < lpPs->oNtPkgHead.unHeadReserved.WordBuEx.wTotal) {
		return 0;
	}

	// 所有分包都已收完,排好序后进行组包
	sort(mapCutPkg[iKey].vecPkg.begin(), mapCutPkg[iKey].vecPkg.end(), SortFun);

	uint32_t dwTotalLen = 0;
	for (iter = mapCutPkg[iKey].vecPkg.begin(); iter != mapCutPkg[iKey].vecPkg.end(); iter++)
	{
		dwTotalLen += (*iter)->nSize;
	}

	uint8_t * pBuffer = new uint8_t[sizeof(lpPs->oNtPkgHead) + dwTotalLen + 1]; // 预留包头长度
	uint8_t * pData = pBuffer + sizeof(lpPs->oNtPkgHead); // 预留包头长度

	for (iter = mapCutPkg[iKey].vecPkg.begin(); iter != mapCutPkg[iKey].vecPkg.end(); iter++)
	{
		// vector里面的包的指针因为预留了20个字节的包头,所以拷贝实际数据时要偏移20, 长度则是实际的数据长度
		memcpy(pData, (*iter)->pData + sizeof((*iter)->oNtPkgHead), (*iter)->nSize);
		pData += (*iter)->nSize;

		// 释放数据
		delete [](*iter)->pData;
		if ((*iter) != lpPs) { // 刚刚最后push进来的那个不需要释放
			delete (*iter);
		}
	}

	// 包全部组完,清空缓存的数据
	mapCutPkg[iKey].vecPkg.clear();
	mapCutPkg.erase(iKey);

	lpPs->oNtPkgHead.bFrag = 0;
	lpPs->oNtPkgHead.wLen = sizeof(lpPs->oNtPkgHead) + dwTotalLen; // 这个长度很有可能大于64K,则这个值无效
	lpPs->oNtPkgHead.unHeadReserved.dwReserved = 0;
	lpPs->pData = pBuffer;
	lpPs->nSize = dwTotalLen; // 只包含实际的数据长度


	return 1;
}

/******************************************************
 从域名中取得IP信息
 *******************************************************/
bool CNetSocket::TranslateRemoteAddress(const char * lpszHost, int nPort, SOCKADDR_IN &sockAddr)
{
	assert(lpszHost != NULL);

	memset(&sockAddr,0,sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszHost);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszHost);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			//WSASetLastError(WSAEINVAL);
			return false;
		}
	}

	sockAddr.sin_port = htons((uint16_t)nPort);
	return true;
}


/******************************************************
 如果配置了代理则能过代理连接服务
 *******************************************************/
bool CNetSocket::ConnectByProxy(ServerPropertyData spd, SOCKET &socket, void *pthis)
{
    CNetSocket * pThis = (CNetSocket *)pthis;
		{
			int ret;
			int conret;
			SOCKADDR_IN		saServer;
			if(!TranslateRemoteAddress((const char *)spd.hostProperty.strIp.c_str(),spd.hostProperty.nPort, saServer))
			{
				return false;
			}
			HX_LOG("connect to %s:%d,sock=%d",(const char *)spd.hostProperty.strIp.c_str(),spd.hostProperty.nPort,socket);
#if SSL_ENABLE
            if (pThis->m_nPackType != 0)
            {
                ret = hx_soc_set_nonblock((int)socket);
                if(ret == -1)  return false;
            }
#else
            ret = hx_soc_set_nonblock((int)socket);
            if(ret == -1)  return false;
#endif
			if((conret = connect(socket, (struct sockaddr *)&saServer, sizeof(saServer))) == -1)
            {
                if(errno != EINPROGRESS){
                    HX_LOG("connect err, errno=%d",errno);
                    return false;
                }else{
                    struct timeval timeout = {5, 0};
                    fd_set wfd;
                    FD_ZERO(&wfd);
                    FD_SET(socket, &wfd);
                    ret = select(socket + 1, NULL, &wfd, NULL, &timeout);
                    if(ret>0 && FD_ISSET(socket,&wfd))
                    {
                        int error=0;
                        socklen_t len = sizeof(error);
                        if (getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 ||error != 0)
                        {
                            HX_LOG("connect failed,error=%d",error);
                            return false;
                        }
                    }else{
                        HX_LOG("connect failed,ret=%d",ret);
                        return false;
                    }
                }
            }
#if SSL_ENABLE
            if (pThis->m_nPackType != 0) return true;
            ////////////
            // 初始化 //
            ////////////
            SSL_CTX* ctx;
            SSL_METHOD *meth;
            
            SSL_load_error_strings();
            SSLeay_add_ssl_algorithms();
            meth = (SSL_METHOD *)SSLv23_client_method();
            
            ctx = SSL_CTX_new(meth);
            if (!ctx) {
                HX_LOG("ssl connect failed,ret=%d",errno);
//                        ERR_print_errors_fp(stderr);
                //        std::cout << "SSL_CTX_new error." << std::endl;
                return -1;
            }

            /////////////////////////////////////
            // TCP连接已经建立，执行Client SSL //
            /////////////////////////////////////
            SSL*     ssl;
            X509*    server_certificate;
            char*    str;
            
            ssl = SSL_new(ctx);
            if (ssl == NULL) {
                HX_LOG("connet error,errno = [%d]",errno);
                return -1;
            }
            SSL_set_fd(ssl, socket);
            ret = SSL_connect(ssl);
            if (ret == -1) {
                HX_LOG("connet error,errno = [%d]",errno);
                return -1;
            }
            
            
            
            // 接下来的获取密码和获取服务器端证书的两部是可选的，不会影响数据交换
            
            // 获取cipher
            HX_LOG("SSL connection using: %s",SSL_get_cipher(ssl));
            
            // 获取服务器端的证书
            server_certificate = SSL_get_peer_certificate(ssl);
            if (server_certificate != NULL) {
                HX_LOG("Server certificate:");
                
                str = X509_NAME_oneline(X509_get_subject_name(server_certificate), 0, 0);
                if (str == NULL) {
                    HX_LOG("X509_NAME_oneline error.");
                }
                else {
                    HX_LOG("subject: %s",str);
                    OPENSSL_free(str);
                }
                
                str = X509_NAME_oneline(X509_get_issuer_name(server_certificate), 0, 0);
                if (str == NULL) {
                    HX_LOG("X509_NAME_oneline error.");
                }
                else {
                    HX_LOG("issuer: %s",str);
                    OPENSSL_free(str);
                }
                
                X509_free(server_certificate);
            }
            else {
                HX_LOG("Server does not have certificate. we sould Esc!");
                return -1;
            }
            pThis->m_hSSL = ssl;
#endif
            
			HX_LOG("connect to %s:%d,suc,socket=%d,conret=%d",
                   (const char *)spd.hostProperty.strIp.c_str(),spd.hostProperty.nPort,
                   socket,conret);

			//OnConnect();
		}
	return true;
}

long CNetSocket::GetServerIPBySpeed(ArrayServerPropertyData& aryHPD, bool bCheckForOne, long lTimeOut)
{
	int index = -1;
	int i;
	int size;
	SOCKET skt = INVALID_SOCKET;
	long begin;
	long end;
	size = aryHPD.size();
	if(size <= 0)
		return index;
	if(size == 1)
		return 0;
	for(i = 0; i < size; i++)
    {

        skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (skt == INVALID_SOCKET)
            return -2;
        begin = GetTickCount();
        HX_LOG("connect to host[%d]",i);
        if(!ConnectByProxy(aryHPD[i], skt,NULL))
        {
            aryHPD[i].hostProperty.lSpeed = LONG_MAX;
            aryHPD[i].hostProperty.IsValid = false;
        }
        else
        {
            end = GetTickCount();
            aryHPD[i].hostProperty.lSpeed = (end -begin);
            aryHPD[i].hostProperty.IsValid = true;
        }
        close(skt);
    }
	long min_speed = LONG_MAX;
	for(i = 0; i < size; i++)
    {
        if(aryHPD[i].hostProperty.lSpeed < min_speed)
        {
            index = i;
            min_speed = aryHPD[i].hostProperty.lSpeed;
        }
    }
	return index;
}


int CNetSocket::GetNextSeq(void)
{
	return ++m_nSeq;
}


/////////////////zxw add quote start //////////////////////////////

void  CNetSocket::GenerateCheckSum32(char pOutBuf[4], const char* pInBuf, int iLen)
{
    uint64_t  ui64Chksum = 0; ///  32位累加和.
    const char* pPos = pInBuf;
    const char* pPosEnd = pInBuf+iLen;
    for(int i=0; i<iLen/4; ++i)
    {
        unsigned char ucChar0 = (unsigned char)(*pPos);
        unsigned char ucChar1 = (unsigned char)(*(pPos+1));
        unsigned char ucChar2 = (unsigned char)(*(pPos+2));
        unsigned char ucChar3 = (unsigned char)(*(pPos+3));
        ui64Chksum += (ucChar0*0x01000000+ucChar1*0x010000+ucChar2*0x0100+ucChar3);

        while(ui64Chksum>0xFFFFFFFF) /// 高32位不是0.
        {
            ui64Chksum = (ui64Chksum%0x0100000000) /*低32位*/ + (ui64Chksum/0x0100000000) /*高32位*/;
        }
        pPos += 4;
    }
    if(iLen%4!=0)  /// 如果总字节数不是4的倍数.
    {
        unsigned char ucChar0 = 0;
        unsigned char ucChar1 = 0;
        unsigned char ucChar2 = 0;
        unsigned char ucChar3 = 0;

        if(pPos<=pPosEnd) { ucChar0 = (unsigned char)(*pPos); ++pPos; }
        if(pPos<=pPosEnd) { ucChar1 = (unsigned char)(*pPos); ++pPos; }
        if(pPos<=pPosEnd) { ucChar2 = (unsigned char)(*pPos); ++pPos; }
        if(pPos<=pPosEnd) { ucChar3 = (unsigned char)(*pPos); ++pPos; }

        ui64Chksum += (ucChar0*0x01000000+ucChar1*0x010000+ucChar2*0x0100+ucChar3);

        while(ui64Chksum>0xFFFFFFFF) /// 高32位不是0.
        {
            ui64Chksum = (ui64Chksum%0x0100000000) /*低32位*/ + (ui64Chksum/0x0100000000) /*高32位*/;
        }
    }
    unsigned int uiChkSum = (unsigned int)ui64Chksum;
    uiChkSum = ~uiChkSum;  /// 取反得到校验和，取反的目的是全部相加后等于0.
    pOutBuf[3] = (unsigned char)(uiChkSum%0x0100);  /// 低位
    pOutBuf[2] = (unsigned char)((uiChkSum%0x010000)/0x0100);
    pOutBuf[1] = (unsigned char)((uiChkSum%0x01000000)/0x010000);
    pOutBuf[0] = (unsigned char)((uiChkSum%0x0100000000)/0x01000000);  /// 高位
    return ;
}

int CNetSocket::SendQuotePack(const char* pBodyBuf,  unsigned int iBodyLen)
{
    int ret;
    int iHeadLen = sizeof(PacketHead);
    int iPackLen = iBodyLen + iHeadLen;
    char *sendData = new char[iPackLen];
    
    PacketHead* oPackHead = (PacketHead*)sendData;
    oPackHead->m_uiDataLen = iBodyLen;
    oPackHead->m_uiReserved = 0x0c0a0a0a;
    oPackHead->m_uiDataLen = HTONL(oPackHead->m_uiDataLen);
    oPackHead->m_uiReserved = HTONL(oPackHead->m_uiReserved);
    memcpy(sendData + sizeof(PacketHead), pBodyBuf, iBodyLen);
    ret = Send((PBYTE)sendData, iPackLen);
    delete []sendData;
    return ret;
}

int CNetSocket::SendBalancePack(const char* pBodyBuf,  unsigned int iBodyLen)
{
    int ret;
    int iHeadLen = sizeof(SCommPkgHead);
    int iPackLen = iBodyLen + iHeadLen;
    char *sendData = new char[iPackLen];
    
    SCommPkgHead* oPackHead;
    oPackHead = (SCommPkgHead*)sendData;
    oPackHead->nDatalen = iBodyLen;
    oPackHead->nReserved = 0x0c0a0a0a;
    oPackHead->nDatalen = HTONL(oPackHead->nDatalen);
    oPackHead->nReserved = HTONL(oPackHead->nReserved);
    memcpy(sendData + sizeof(SCommPkgHead), pBodyBuf, iBodyLen);
    ret = Send((PBYTE)sendData, iPackLen);
    delete []sendData;
    return ret;
}

