//
//  jxtnethelper.h
//  jxtMsgClient
//
//  Created by  on 13-2-17.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef jxtMsgClient_jxtnethelper_h
#define jxtMsgClient_jxtnethelper_h


//typedef enum enumConnectStatusType
//{	
//	CST_CONNECTING		= 0x1,//正在连接...
//	CST_CONNECTED		= 0x2,//已连接,正在登录...
//	CST_UNCONNECTED		= 0x3,//未连接
//	CST_RECONNECT		= 0x4,//正在重新连接...
//	CST_ERROR_CONNECT	= 0x5,//连接失败
//	CST_CLIENT_SENDDATA	= 0x6, //表示发送数据时关闭SOCKET
//	CST_CLIENT_ENUMNETWORKEVENTS	= 0x7, //表示EnumNetworkEvents出错关闭SOCKET
//	CST_CLIENT_JUDGE_CLOSE			= 0x8, //表示客户端在9s内无网络事件，关闭SOCKET
//	CST_SOCKET5_ERROR	= 0x9, //  表示使用SOCKET5代理服务登录
//	CST_SOCKET4_ERROR	= 0xA //  表示使用SOCKET4代理服务登录
//    
//}ConnectStatusType;
//
//typedef struct nhp_net_events
//{
//	//网络状态回调
//    /****
//     * 见上 ConnectStatusType
//     * 1、将对应的状态显示在UI上，如果是已连接显示
//     ****/
//	void(*nhp_net_status_rsp)(int type, int nFlag);
//	
//	//收到网络事件通知
//	void(*nhp_recv_notify_event)();
//	
//}nhp_net_events;
//
////设定网络事件回调
//void set_nhp_net_events(nhp_net_events events);
//
////收到网络事件通知后切换主线程处理事件
//void nhp_process_event();
//
////通知网络状态 0网络不可用 1网络可用
//void nhp_notify_net_state(int ntype,int state);


#endif
