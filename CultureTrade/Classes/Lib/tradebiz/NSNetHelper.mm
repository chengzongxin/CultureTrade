//
//  vmsStartNet.m
//  iVMS2
//
//  Created by zengxw on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "NSNetHelper.h"
//#import "Debug.h"

#include "nethelper.h"
#import <arpa/inet.h>
#import <CFNetwork/CFNetwork.h>
#import <netinet/in.h>
#import <netdb.h>  
#import <SystemConfiguration/SystemConfiguration.h>
#import "vmsNetReachability.h"
#import "Reachability.h"

enum UserStatus {
    Default,
    Logined,
    Kill,
};
@interface NSNetHelper()
{
    UserStatus mUserStatus;
    NSThread* mThread;
}
@end
extern "C"
{
#include "jxtnethelper.h"
};

NSNetHelper * jxtstartnetInstance;
BOOL bStartNeted = NO;

//BOOL bProcessEventDisable = NO;
@implementation NSNetHelper

@synthesize delegate;
@synthesize bNetEnable;

int porcessEventCount = 0;

int netStatusFlag = 0;

static NSString *urlString = @"http://www.baidu.com/";

+(NSNetHelper*)setup
{
    if ( jxtstartnetInstance == NULL )
    {
        jxtstartnetInstance = [[NSNetHelper alloc] init];
        nhp_net_events events;
        events.nhp_net_status_rsp = &on_cbiz_net_status_rsp;
        events.nhp_recv_notify_event = &on_cbiz_recv_notify_event;
        set_nhp_net_events(events);
    }
    return jxtstartnetInstance;
}


-(void)initNet
{
    mUserStatus = Default;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(checkNetworkStatus:) name:kReachabilityChangedNotification object:nil];
    [self startNetNotifier];
    if ([vmsNetReachability isWifiEnable]) {
        nhp_notify_net_state(10, 1);
    } else if ([vmsNetReachability is3GEnable]) {
        nhp_notify_net_state(1, 1);
    }
    [NSTimer scheduledTimerWithTimeInterval:0.5f target:self selector:@selector(processEvent) userInfo:nil repeats:YES];
}

-(void)startNetNotifier
{
    [vmsNetReachability startNetNotifier];
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
    if(mUserStatus != Kill) nhp_notify_net_state([vmsNetReachability isWifiEnable] ? 10 : 1, 1);
}

-(BOOL)isWifiEnable {
    return [vmsNetReachability isWifiEnable];
}

-(BOOL)is3GEnable {
    return [vmsNetReachability is3GEnable];
}

-(BOOL)isNetEnable {
    return [vmsNetReachability isNetEnable];
}


- (void)checkNetworkStatus:(NSNotification *)notice
{
    Reachability* internetReach = ( Reachability * )notice.object;
    if ( [internetReach isEqual:[vmsNetReachability getRatyWifi ]] ) {
        if ([vmsNetReachability isWifiEnable]) {
            NSLog(@" get   wifi   enable   ");
            if ([NSNetHelper setup].delegate != nil
                && [[NSNetHelper setup].delegate respondsToSelector: @selector(on_net_status_rsp:nFlag:)]) {
                [[NSNetHelper setup].delegate on_net_status_rsp:10 nFlag:2];
            }
        } else {
            NSLog(@" get   wifi   disbale   ");
            if ([NSNetHelper setup].delegate != nil
                && [[NSNetHelper setup].delegate respondsToSelector: @selector(on_net_status_rsp:nFlag:)]) {
                [[NSNetHelper setup].delegate on_net_status_rsp:10 nFlag:1];
            }
        }
    }
    else if( [internetReach isEqual:[vmsNetReachability getRaty3G ]] )
    {
        if ([vmsNetReachability is3GEnable]) {
            NSLog(@" get   3g   enable   ");
            if ([NSNetHelper setup].delegate != nil
                && [[NSNetHelper setup].delegate respondsToSelector: @selector(on_net_status_rsp:nFlag:)]) {
                [[NSNetHelper setup].delegate on_net_status_rsp:9 nFlag:2];
            }
        } else {
            NSLog(@" get   3g   disbale   ");
            if ([NSNetHelper setup].delegate != nil
                && [[NSNetHelper setup].delegate respondsToSelector: @selector(on_net_status_rsp:nFlag:)]) {
                [[NSNetHelper setup].delegate on_net_status_rsp:9 nFlag:1];
            }
        }
    }
    else
    {
        NSLog(@" get unkown notice ");
    }
    if ([vmsNetReachability isNetEnable]) {
        [self checknettoactive];
    } else {
        nhp_notify_net_state(0, 0);
    }
}

-(void)checknettoactive
{
    http_get_data();
}

int http_get_data(void)
{
    @try {
        // NSString *urlString = @"http://www.baidu.com/";
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:urlString]
                                                               cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
                                                           timeoutInterval:5.0];
        [request setHTTPMethod:@"GET"];
        NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:jxtstartnetInstance startImmediately:YES];
        [connection scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        [connection start];
        NSLog(@" connection baidu.com  again ");
    }
    @catch (NSException *exception) {
        NSLog(@"%@", exception);
    }
    @finally {
    }
    nhp_notify_net_state(0, 0);
    return 0;
}

//网络状态回应
void on_cbiz_net_status_rsp(int type,int nFlag) {
    if ([NSNetHelper setup].delegate != nil
        && [[NSNetHelper setup].delegate respondsToSelector: @selector(on_net_status_rsp:nFlag:)]) {
        [[NSNetHelper setup].delegate on_net_status_rsp:type nFlag:nFlag];
    }
    //[[jxtUINetHelper setup] performSelectorOnMainThread:@selector(runNetStatusRsp:nFlag:) withObject:@[@type,@nFlag] waitUntilDone:NO];
}

void on_cbiz_recv_notify_event() {
    porcessEventCount++;
}

-(void) setLogined {
    mUserStatus = Logined;
}

//
-(void)processEvent
{
    //if (porcessEventCount > 0)
    {
        nhp_process_event();
        //porcessEventCount--;
    }
}

-(void) trade_ui_kick_notify{
    mUserStatus = Kill;
    nhp_notify_net_state(0, 0);
}


-(void) runProcessEvent{
    while (true) {
        [self processEvent];
        [NSThread sleepForTimeInterval:0.05f];
    }
}

-(NetState) getNetState {
    if([vmsNetReachability isWifiEnable]){
        return WiFi;
    }
    if([vmsNetReachability is3GEnable]){
        return G3;
    }
    return NoNetWork;
}

@end
