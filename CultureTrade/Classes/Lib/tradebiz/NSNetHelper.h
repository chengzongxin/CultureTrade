//
//  vmsStartNet.h
//  iVMS2
//
//  Created by zengxw on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
//#import "Debug.h"

typedef enum
{
    NoNetWork    = 0,
    WiFi     = 2,
    G3       = 1
} NetState;

typedef enum enumUIConnectStatusType
{	
	UI_CST_CONNECTING		          =0x1,//正在连接
	UI_CST_CONNECTED		          =0x2,//已连接
	UI_CST_UNCONNECTED		          =0x3,//未连接
	UI_CST_RECONNECT		          =0x4,//正在重新连接
	UI_CST_ERROR_CONNECT	          =0x5,//连接失败
	UI_CST_CLIENT_SENDDATA	          =0x6, //表示发送数据时关闭SOCKET
	UI_CST_CLIENT_ENUMNETWORKEVENTS	  =0x7, //表示EnumNetworkEvents出错关闭SOCKET
	UI_CST_CLIENT_JUDGE_CLOSE		  =0x8, //表示客户端在9s内无网络事件，关闭SOCKET
	UI_CST_SOCKET5_ERROR	          =0x9, //  表示使用SOCKET5代理服务登录
	UI_CST_SOCKET4_ERROR	          =0xA //  表示使用SOCKET4代理服务登录
} UIConnectStatusType;

@protocol NSNetHelperDelegate<NSObject>
//-(void)callbackNetStatusRsp:(int)flag;
@optional
-(void) on_net_status_rsp:(int)type nFlag:(int)nFlag;
@end

@interface NSNetHelper : NSObject {
	id<NSNetHelperDelegate> delegate;
    
    BOOL bNetEnable;
}

@property BOOL bNetEnable;

@property( strong, nonatomic) id<NSNetHelperDelegate> delegate;

//-(NetState) getNetState;
-(NetState) getNetState;
-(void) setLogined;
+(NSNetHelper*)setup;

-(void)initNet;

-(void) trade_ui_kick_notify;

//获取当前网络状态
//-(int)getNetStatus;

//-(void)setNetStatus:(int)state;

-(BOOL)isWifiEnable;

-(BOOL)is3GEnable;

-(BOOL)isNetEnable;
@end
