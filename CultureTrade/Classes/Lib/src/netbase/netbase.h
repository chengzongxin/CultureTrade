// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 NETBASE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// NETBASE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef __NETBASE_H__
#define __NETBASE_H__

//#include "ComStruct.h"
#include "nt_types.h"

//#define NETBASE_EXPORTS
#ifdef NETBASE_EXPORTS
#define NETBASE_API __declspec(dllexport)
#else
//#define NETBASE_API __declspec(dllimport)
#define NETBASE_API
#endif

#ifdef __cplusplus         // if used by C++ code
extern "C" {                  // we need to export the C interface
#else
    typedef char bool;
#endif
    typedef bool (*PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
    
    typedef bool (*PFFireSessionStatus)(int nSid,int nFlag);
    typedef bool (*PFNotifyEvent)(int nSid);
    
    //====================================
    //1 初始化全局信息
    //Init();
    NETBASE_API int Init();
    
    //2 释放全局信息
    //Init();
    NETBASE_API int UnInit();
    
    //3.取得一个会话
    //CreateSession
    //返回会话的唯一标识
    NETBASE_API int CreateSession();
    
    NETBASE_API void SetSessionSocketPackType(int nSID, int type);
    
    //4.设置会话属性
    //SetSessionOption
    /*****************************************************************
     nCmd
     取值为
     *****************************************************************/
    NETBASE_API bool SetSessionOption(int nSID, int nCmd, int nValue);
    
    //5 取会话属性
    //NETBASE_API bool GetSessionOption(int nSID, int nCmd, int& nValue);
    NETBASE_API bool GetSessionOption(int nSID, int nCmd, int* nValue);
    
    //6.增加服务信息
    //AddHost
    //NETBASE_API bool AddHost(int nSID, int nLayer, LPServerPropertyDataDll pSpd);
    /*
     * nLayer:1,主; 2,备
     */
    NETBASE_API bool AddHost(int nSID, int nLayer, char* str_ip,int port);
    
    NETBASE_API bool ClearHost(int nSID, int nLayer);
    
    //7 服务总数
    //GetHostCount
    //NETBASE_API bool GetHostCount(int nSID, int nLayer, int& nCount);
    NETBASE_API bool GetHostCount(int nSID, int nLayer, int* nCount);
    
    
    //8 取服务的测试数据
    //GetHostCount
    //NETBASE_API bool GetServerPropertyData(int nSID, int nLayer, int nIndex, LPServerPropertyDataDll pSpd);
    
    //=========================================
    //9.连接服务
    //StartSession
    NETBASE_API void StartSession(int nSID);
    
    //10.断开会话连接
    //StopSession
    NETBASE_API void StopSession(int nSID);
    
    //11.释放会话信息
    //ReleaseSession
    NETBASE_API bool ReleaseSession(int nSID);
    //==================================================
    //12.登录
    //Login
    /*
     uint32_t dwLoginIdType; // 帐号类型
     uint32_t dwClientVersion; //登录版本号
     uint8_t  sLoginId[20]; //登录帐号
     uint32_t dwClientTime;
     */
    NETBASE_API bool Login(int nSID, const char * szLoginId, int nLoginIdType, int nClientVersion, const char * szPwd);
    
    // 设置行情标志,为1表示需要服务端推送行情,为0表示不需要服务端推送行情
    NETBASE_API bool SetQuoteFlag(int nSID, int iFlag);
    
    
    //13.发送数据
    //Send
    NETBASE_API bool Send(int nSID, int nCmd, int nSeq, uint8_t * pData, int nLen);
    
    //14.退出
    //Logout
    NETBASE_API bool Logout(int nSID, uint32_t nUid);
    
    //15.获取远程服务IP
    //GetPeerHost
    //NETBASE_API bool GetPeerHost(int nSID, LPHostPropertyDll pHP);
    
    //16.取得会话的连接状态
    //GetSessionStatus
    //NETBASE_API bool GetSessionStatus(int nSID, int& nStatus);
    NETBASE_API bool GetSessionStatus(int nSID, int* nStatus);
    
    
    //18.一个业务包事件
    //OnRead
    //OnRead(int nSID, int nCmd, int nSeq, const CString& strData);
    //typedef bool (*PFFirePackage)(int nSid, int nCmd, int nSeq, LPBYTE pData, int nLen);
    NETBASE_API void BindFirePackage(int nSID, PFFirePackage pfnFirePackage);
    
    // 行情包事件
    NETBASE_API void BindQuotePackage(int nSID, PFFirePackage pfnQuotePackage);
    
    //19.会话关闭
    NETBASE_API void BindFireSessionStatus(int nSID, PFFireSessionStatus pfnFireSessionStatus);
    
    NETBASE_API void BindNotifyEvent(int nSID, PFNotifyEvent pfnNotifyEvent);
    
    /* ProcessEvent:
     * return:
     *     0,suc;  1,no data to process, -1,no such session id
     */
    NETBASE_API int ProcessEvent(int nSID);
    
    
    /* NotifyNetState:
     * return: 0,suc; -1, no such session id
     * para: 
     * net_state: 0,net unavariable; 1,net avariable
     */
    int NotifyNetState(int nSID,int net_state);
    
#ifdef __cplusplus
}
#endif


#endif // __NETBASE_H__

// end of file
