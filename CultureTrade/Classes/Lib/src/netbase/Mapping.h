// Mapping.h: interface for the CMapping class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MAPPING_H__
#define __MAPPING_H__

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

#include "nt_types.h"

#define net_msg

class  CNetworkMessageMap
{
public:
	virtual bool ProcessNetworkMessage(uint32_t dwMsgMapID) = 0;
};

#define BEGIN_NETWORK_MSG_MAP(theClass) \
public: \
		bool ProcessNetworkMessage(uint32_t dwMsgMapID = 0) \
		{ \
			bool bRet = false; 

#define NETWORK_MESSAGE_HANDLER(msg, func) \
			if (msg & dwMsgMapID) \
				bRet = func(); 

#define END_NETWORK_MSG_MAP() \
		return bRet; \
	}


//////////////////////////////////////////////////////////////////////

#define app_msg



class  CApplicationMessageMap
{
public:
	virtual bool ProcessApplicationMessage(uint32_t dwMsgMapID, uint8_t * pData) = 0;
};

#define BEGIN_APP_MSG_MAP(theClass) \
public: \
		bool ProcessApplicationMessage(uint32_t dwMsgMapID = 0, uint8_t * pData = NULL) \
		{ \
			switch(dwMsgMapID) \
			{ \
			case 0:

#define APP_MESSAGE_HANDLER(msg, func) \
			case msg: \
			{ \
				func(pData); \
			} \
			break;


#define END_APP_MSG_MAP() \
			break; \
		default: \
			TRACE(_T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ASSERT(FALSE); \
			break; \
		} \
		return FALSE; \
	}

#endif // __MAPPING_H__

// end of file
