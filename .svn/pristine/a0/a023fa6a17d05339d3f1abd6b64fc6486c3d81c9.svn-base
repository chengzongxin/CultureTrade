#include "NetMgr.h"
#include "nt_pack.h"
#include "nt_macro.h"
#include "nt_crypt.h"
#include "nt_md5.h"

CNetMgr::CNetMgr(void)
{
}

CNetMgr::~CNetMgr(void)
{
}


/*
¥¥Ω®“ª∏ˆª·ª∞…œœ¬Œƒ

∑µªÿ÷µŒ™ª·ª∞µƒŒ®“ª±Í ∂

*/
long CNetMgr::CreateSessionContext(LPSessionContext& pSc)
{
	long nSID;// = InterlockedIncrement(&m_nSid);
	Locker locker(m_MutexMap);
	nSID = ++m_nSid;
	pSc = new SessionContext;
	pSc->nSid = nSID;
    pSc->pNetSocket = new CNetSocket;
	pSc->pMutexSession = new Mutex;
	return nSID;
}

bool CNetMgr::ReleaseSessionContext(long nSid)
{	
	bool bRet = false;
	Locker locker(m_MutexMap);
	IterHashMapSessionContext iTer;
	iTer = m_mapSessionContext.find(nSid);
	if (iTer != m_mapSessionContext.end())
	{		
		delete iTer->second->pMutexSession;
		delete iTer->second->pNetSocket;
		iTer->second->pMutexSession = NULL;
		iTer->second->pNetSocket = NULL;
		delete iTer->second;
		iTer->second = NULL;
		m_mapSessionContext.erase(iTer);
		bRet = true;
	}
	return bRet;
}

bool CNetMgr::GetSessionContext(long nSid, SessionContext& sc)
{
	bool bRet = false;
	if(nSid > 0 )
	{
		IterHashMapSessionContext iTer;
		iTer = m_mapSessionContext.find(nSid);
		if (iTer != m_mapSessionContext.end())
		{
			sc = *iTer->second;
			bRet = true;
		}
	}
	return bRet;
}

bool CNetMgr::InsertMap(LPSessionContext pSc)
{
	bool bRet = false;
	Locker locker(m_MutexMap);
	IterHashMapSessionContext iTer;
	iTer = m_mapSessionContext.find(pSc->nSid);
	if (iTer == m_mapSessionContext.end())
	{//√ª”–
		m_mapSessionContext.insert(make_pair(pSc->nSid, pSc));
		bRet = true;
	}else
	{//”–£¨»œŒ™ «Õ¨“ª∏ˆ
		bRet = true;		
	}

	return bRet;
}
/*
≥ı ºªØƒ⁄≤ø±ÿ“™µƒ ˝æ›
*/
int CNetMgr::Init()
{
	CNetSocket::InitNet();
	return 0;
}
int CNetMgr::UnInit()
{
	//«Â≥˝√ª”–¥¶¿Ìµƒ ˝æ›
	IterHashMapSessionContext iTer = m_mapSessionContext.begin();
	for(; iTer != m_mapSessionContext.end(); iTer++)
	{
		if(NULL != iTer->second)
		{
			if(NULL != iTer->second->pNetSocket)
			{
				iTer->second->pNetSocket->Stop();
				delete iTer->second->pNetSocket;
			}

			if(NULL != iTer->second->pMutexSession)
			{					
				delete iTer->second->pMutexSession;
			}
			delete iTer->second;
		}
	}
	m_mapSessionContext.clear();

	CNetSocket::ClearNet();	
	return 0;
}

//1.»°µ√“ª∏ˆª·ª∞
//CreateSession
//∑µªÿª·ª∞µƒŒ®“ª±Í ∂
int CNetMgr::CreateSession()
{
	LPSessionContext pSc = NULL;
	int nSid = CreateSessionContext(pSc);
	if(nSid > 0 && pSc != NULL)
	{
		InsertMap(pSc);
	}
	return nSid;
}
void CNetMgr::SetSessionSocketPackType(int nSID, int type)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
        oSc.pNetSocket->SetPackType(type);
    }
}
//2.…Ë÷√ª·ª∞ Ù–‘
//SetSessionOption
/*****************************************************************
nCmd 
»°÷µŒ™
ReConnect = 1 // «∑Ò÷ÿ¡¨
ReConnectTryCount = 2 //‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
*****************************************************************/
bool CNetMgr::SetSessionOption(int nSID, int nCmd, int nValue)
{
	bool bRet = false;
	
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		switch(nCmd)
		{
		case SOC_ReConnect:// = 1 «∑Ò÷ÿ¡¨
			{
				//…Ë÷√ «∑Ò÷ÿ¡¨
				ReConnectProperty rcp;
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetReConnectProperty(rcp);
				rcp.bAllowAutoReConnect = nValue > 0 ? true : false;
				oSc.pNetSocket->SetReConnectProperty(rcp);
				bRet = true;
			}
			break;
		case SOC_ReConnectTryCount:// = 2 //‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
			{
				//‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
				ReConnectProperty rcp;
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetReConnectProperty(rcp);
				rcp.lReConnectCount = nValue;
				oSc.pNetSocket->SetReConnectProperty(rcp);
				bRet = true;
			}
			break;
		case SOC_WaitReConnect: 
			{
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->SetWaitReConnect(nValue);
			}
			break;
		case SOC_JudgeDisConnect: 
			{
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->SetJudgeDisConnect(nValue);
			}
			break;
		default:
			break;
		}
	}
	return bRet;
}

//2.»°ª·ª∞ Ù–‘
//GetSessionOption
/*****************************************************************
nCmd 
»°÷µŒ™
ReConnect = 1 // «∑Ò÷ÿ¡¨
ReConnectTryCount = 2 //‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
*****************************************************************/
bool CNetMgr::GetSessionOption(int nSID, int nCmd, int& nValue)
{
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		switch(nCmd)
		{
		case SOC_ReConnect:// = 1 «∑Ò÷ÿ¡¨
			{
				//…Ë÷√ «∑Ò÷ÿ¡¨
				ReConnectProperty rcp;
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetReConnectProperty(rcp);
				nValue  = rcp.bAllowAutoReConnect ? 1 : 0;
				bRet = true;
			}
			break;
		case SOC_ReConnectTryCount:// = 2 //‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
			{
				//‘⁄‘≠”–HOST…œ÷ÿ ‘∂‡…Ÿ¥Œ
				ReConnectProperty rcp;
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetReConnectProperty(rcp);
				nValue = rcp.lReConnectCount;
				bRet = true;
			}
			break;
		case SOC_WaitReConnect: 
			{
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetWaitReConnect(nValue);
			}
			break;
		case SOC_JudgeDisConnect: 
			{
				Locker locker(*oSc.pMutexSession);
				oSc.pNetSocket->GetJudgeDisConnect(nValue);
			}
			break;
		default:
			break;
		}
	}
	return bRet;
}

//3.‘ˆº”∑˛ŒÒ–≈œ¢
//AddHost
//1 ÷˜ 2 ±∏
bool CNetMgr::AddHost(int nSID, int nLayer, ServerPropertyData& spd)
{
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		if(HL_Pri == nLayer)
		{
			oSc.pNetSocket->AddPriServerPropertyData(spd);
			bRet = true;
		}
		else
		{
			oSc.pNetSocket->AddBakServerPropertyData(spd);
			bRet = true;
		}
	}
	return bRet;
}

//3.1 ∑˛ŒÒ◊‹ ˝

bool CNetMgr::ClearHost(int nSID, int nLayer)
{
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		if(HL_Pri == nLayer)
		{
			bRet = oSc.pNetSocket->ClearPriServerPropertyData();
		}
		else
		{
			bRet = oSc.pNetSocket->ClearBakServerPropertyData();
		}
	}
	return bRet;
}

//GetHostCount
bool CNetMgr::GetHostCount(int nSID, int nLayer, int& nCount)
{	
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		if(HL_Pri == nLayer)
		{
			nCount = oSc.pNetSocket->GetPriServerPropertyCount();
			bRet = true;
		}
		else
		{
			nCount = oSc.pNetSocket->GetBakServerPropertyCount();
			bRet = true;
		}
	}
	return bRet;
}

//3.2 »°∑˛ŒÒµƒ≤‚ ‘ ˝æ›
//GetHostCount
bool CNetMgr::GetServerPropertyData(int nSID, int nLayer, int nIndex, ServerPropertyData& pSpd)
{
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		if(HL_Pri == nLayer)
		{
			pSpd = oSc.pNetSocket->GetPriServerProperty(nIndex);
			bRet = true;
		}
		else if(HL_Bak == nLayer)
		{
			pSpd = oSc.pNetSocket->GetBakServerProperty(nIndex);
			bRet = true;
		}
	}
	return bRet;
}



//13.ªÒ»°‘∂≥Ã∑˛ŒÒIP
//GetPeerHost
bool CNetMgr::GetPeerHost(int nSID, HostProperty& hp)
{	
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->GetServerHost(hp.strIp, hp.nPort);
		bRet = true;
	}
	return bRet;
}

//14.»°µ√ª·ª∞µƒ¡¨Ω”◊¥Ã¨
//GetSessionStatus
bool CNetMgr::GetSessionStatus(int nSID, int& nStatus)
{
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		nStatus = oSc.pNetSocket->GetConnectStatusType();
		bRet = true;
	}
	return bRet;
}

//=========================================
//4.¡¨Ω”∑˛ŒÒ
//StartSession
void CNetMgr::StartSession(int nSID)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->Start();
	}
}

//5.∂œø™ª·ª∞¡¨Ω”
//StopSession
void CNetMgr::StopSession(int nSID)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->Stop();
	}
}

//12. Õ∑≈ª·ª∞–≈œ¢
//ReleaseSession
bool CNetMgr::ReleaseSession(int nSID)
{
	StopSession(nSID);
	return ReleaseSessionContext(nSID);
}


//==================================================
//6.µ«¬º
//Login
bool CNetMgr::Login(int nSID, const string& strLoginId, int nLoginIdType, int nClientVersion, LPCSTR strPwd)
{	
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		int nSeq = 1; //oSc.pNetSocket->GetNextSeq();
		int nLen;

		uint32_t dwCipherDataLen;
		uint8_t sKey[CRYPT_KEY_LEN];
		uint8_t szBuf[512] = {0};
        uint8_t* pbBuf = szBuf;
		LoginRequest * pstLoginRequest;
		pstLoginRequest = (LoginRequest *)pbBuf;

		PlainLoginReq stLoginRequest;

		if (0 != AddDWord(&pbBuf, 0, nClientVersion)) {
			return false;
		}

		if (0 != AddDWord(&pbBuf, 0, nLoginIdType)) {
			return false;
		}

		int nLoginId  = FIELD_SIZEOF(LoginRequest, sLoginId);
		strncpy((char*)pbBuf, strLoginId.c_str(), nLoginId - 1);
		pbBuf += nLoginId;
		if (AddDWord(&pbBuf, 0, (uint32_t)time(0))) {
			return false;
		}
		uint32_t dwIp = 1111;
		uint32_t dwPort = 123;
		if (AddDWord(&pbBuf, 0, dwIp)) {
			return false;
		}
		if (AddDWord(&pbBuf, 0, dwPort)) {
			return false;
		}
		nLen = pbBuf - szBuf;
		//PlainLoginReq
		memcpy(stLoginRequest.sLoginId, pstLoginRequest->sLoginId, sizeof(stLoginRequest.sLoginId));
		stLoginRequest.dwClientTime = pstLoginRequest->dwClientTime;
		//stLoginRequest.sMAC;

		Md5HashBuffer(sKey, strPwd, CRYPT_KEY_LEN);
		memcpy(oSc.pNetSocket->m_sKey, sKey, sizeof(sKey));
		dwCipherDataLen = sizeof(stLoginRequest) + 128;

		// Œ™¡ÀºÊ»› ÷ª˙∂À,π œ÷‘⁄º”√‹À„∑®≤…”√CA_ASTEA(‘≠¿¥ «CA_TEA,º”√‹«ø∂»“™∏¸∫√)
		int iRet = SymEncrypt(CA_ASTEA, &stLoginRequest, 
			sizeof(stLoginRequest), 
			sKey, pstLoginRequest->sCipherData, &dwCipherDataLen);
		if (0 != iRet) {
			return false;
		}
		nLen += dwCipherDataLen;
		bRet = oSc.pNetSocket->Send(BU_CA_LOGIN_USER, nSeq, szBuf, nLen);
	}

	return bRet;
}


bool CNetMgr::SetQuoteFlag(int nSID, int iFlag)
{
	uint8_t szBuf[512];
	uint8_t * pbBuf = szBuf;
	SessionContext oSc;
	bool bRet = false;

	if (GetSessionContext(nSID, oSc)) {
		Locker locker(*oSc.pMutexSession);

		if (0 != AddDWord(&pbBuf, 0, iFlag)) {
			return false;
		}

		bRet = oSc.pNetSocket->Send(BU_QUOTE_CTRL_QUOTE, 0, szBuf, pbBuf - szBuf);
	}

	return bRet;
}


//7.∑¢ÀÕ ˝æ›
//Send
bool CNetMgr::Send(int nSID, int nCmd, int nSeq, LPBYTE pData, int nLen)
{
	bool bRet = false;
	SessionContext oSc;
	HX_LOG("send data,sid=%d,cmd=0x%X,seq=%d,data_len=%d",nSID,nCmd,nSeq,nLen);
	if(GetSessionContext(nSID, oSc))
	{
        if (oSc.pNetSocket->m_nPackType == 1) {
            bRet = oSc.pNetSocket->Send(nCmd, nSeq, pData, nLen);
        } else if (oSc.pNetSocket->m_nPackType == 2) {
            bRet = oSc.pNetSocket->Send(nCmd, nSeq, pData, nLen);
        } else{
            Locker locker(*oSc.pMutexSession);
            
            uint32_t dwCipherDataLen = nLen + 1024;
            LPBYTE pEncryptData = new BYTE[dwCipherDataLen];
            
            /*
             uint8_t sKey[CRYPT_KEY_LEN];
             KeyContent kc;
             kc.SetKey(oSc.pNetSocket->m_sKey);
             kc.nUid = oSc.pNetSocket->m_dwUid;
             Md5HashBuffer(sKey, &kc, sizeof(KeyContent));
             */
            uint8_t sKey[CRYPT_KEY_LEN];
            memset(sKey, 0, CRYPT_KEY_LEN);
            Md5HashBuffer(sKey, ORIGINALPWD, strlen(ORIGINALPWD));
            
            // Œ™¡ÀºÊ»› ÷ª˙∂À,π œ÷‘⁄º”√‹À„∑®≤…”√CA_ASTEA(‘≠¿¥ «CA_TEA,º”√‹«ø∂»“™∏¸∫√)
            int iRet = SymEncrypt(CA_ASTEA, pData,
                                  nLen,
                                  sKey, pEncryptData, &dwCipherDataLen);
            if (0 != iRet) 
            {
                //TODO:writeLog	
                HX_LOG("send data,SymEncrypt err,ret=%d",iRet);
                return false;
            }
            bRet = oSc.pNetSocket->Send(nCmd, nSeq, pEncryptData, dwCipherDataLen);
        }
	}
	else
    {
        HX_LOG("GetSessionContext err, sid=%d",nSID);
    }
	return bRet;
}

//8.ÕÀ≥ˆ
//Logout
bool CNetMgr::Logout(int nSID, uint32_t nUid)
{	
	bool bRet = false;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		uint32_t nSeq = 2;
		uint32_t nLen = 0;
		uint32_t dwCipherDataLen = 512;
		uint8_t szBuf[512] = {0};
        uint8_t* pbBuf = szBuf;
		if (AddDWord(&pbBuf, 0, nUid)) {
			return false;
		}
		nLen = pbBuf - szBuf;
		
		uint8_t sKey[CRYPT_KEY_LEN];
		KeyContent kc;
		kc.SetKey(oSc.pNetSocket->m_sKey);
		kc.nUid = nUid;
		Md5HashBuffer(sKey, &kc, sizeof(KeyContent));

		// Œ™¡ÀºÊ»› ÷ª˙∂À,π œ÷‘⁄º”√‹À„∑®≤…”√CA_ASTEA(‘≠¿¥ «CA_TEA,º”√‹«ø∂»“™∏¸∫√)
		int iRet = SymEncrypt(CA_ASTEA, &nUid, 
			sizeof(nUid), 
			sKey, pbBuf, &dwCipherDataLen);
		if (0 != iRet) {
			return false;
		}
		nLen += dwCipherDataLen;

		bRet = oSc.pNetSocket->Send(BU_CA_LOGOUT_USER, nSeq, szBuf, nLen);
	}
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
//Õ‚≤ø ¬º˛
//9.“ª∏ˆ“µŒÒ∞¸ ¬º˛
//OnRead
//OnRead(int nSID, int nCmd, int nSeq, const CString& strData);
//typedef bool OnFirePackage(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
void CNetMgr::BindFirePackage(int nSID, PFFirePackage pfnFirePackage)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		//TODO:√ª”–ÕÍ≥…
		oSc.pNetSocket->BindFirePackage(nSID, pfnFirePackage);
	}
}


void CNetMgr::BindQuotePackage(int nSID, PFFirePackage pfnQuotePackage)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		//TODO:√ª”–ÕÍ≥…
		oSc.pNetSocket->BindQuotePackage(nSID, pfnQuotePackage);
	}
}


//11.ª·ª∞¡¨Ω”≥…π¶
//OnConnected
//OnConnected();
//typedef bool OnFireConnected(int nFlag);
void CNetMgr::BindFireSessionStatus(int nSID, PFFireSessionStatus pfnFireSessionStatus)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->BindFireSessionStatus(nSID, pfnFireSessionStatus);
	}
}
void CNetMgr::BindNotifyEvent(int nSID, PFNotifyEvent pfnNotifyEvent)
{
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->BindNotifyEvent(nSID, pfnNotifyEvent);
	}
}

int CNetMgr::ProcessEvent(int nSID)
{//return 0,suc;  1,no data to process, -1,no such session id
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		return oSc.pNetSocket->ProcessEvent(nSID);
	}
	return -1;
}
int CNetMgr::NotifyNetState(int nSID,int net_state)
{//return  0,suc; -1,no such session id;
	SessionContext oSc;
	if(GetSessionContext(nSID, oSc))
	{
		Locker locker(*oSc.pMutexSession);
		oSc.pNetSocket->NotifyNetState(net_state);
		return 0;
	}
	return -1;
}
// end of file
