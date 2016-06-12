//
//  vmsNetReachability.m
//  iVMS
//
//  Created by XiongXiong on 11-6-7.
//  Copyright 2011 Helecomm Daniel. All rights reserved.
//

#import "vmsNetReachability.h"

#import <arpa/inet.h>

@implementation vmsNetReachability

static Reachability * ratyWifi = nil;
static Reachability * raty3G = nil;


+(Reachability*)getRatyWifi
{
    return ratyWifi;
}

+(Reachability*)getRaty3G
{
    return raty3G;
}

+(void)startNetNotifier
{
    if (ratyWifi == nil) {
        ratyWifi = [Reachability reachabilityForLocalWiFi]; 
        [ratyWifi startNotifier];
    }
    
    if (raty3G == nil) {
        raty3G = [Reachability reachabilityForInternetConnection] ; 
        [raty3G startNotifier];
    }
}

+(BOOL)isWifiEnable
{
	NetworkStatus netstatus = [ratyWifi currentReachabilityStatus];
	if (netstatus != NotReachable) {
		return YES;
	} else {
		return NO;
	}
}

+(BOOL)is3GEnable
{
    
	NetworkStatus netstatus = [raty3G currentReachabilityStatus];
	if (netstatus != NotReachable) {
		return YES;
	}
	else {
		return NO;
	}
	
}

+(BOOL)isNetEnable
{
	if ([vmsNetReachability is3GEnable]==NO && [vmsNetReachability isWifiEnable] == NO) {
	//	vmsAlert * alert = [vmsAlert setup];
	//	[alert showSimpleAlertWithTitle:@"提示" message:@"网络不可用，请设置网络" cancelButtonTitle:@"确定" okButtonTitle:nil];
		return NO;
	}
	else {
		return YES;
	}
}

@end
