//
//  vmsNetReachability.h
//  iVMS
//
//  Created by XiongXiong on 11-6-7.
//  Copyright 2011 Helecomm Daniel. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Reachability.h"

@interface vmsNetReachability : NSObject {

}
+(BOOL)isWifiEnable;
+(BOOL)is3GEnable;
+(BOOL)isNetEnable;

+(void)startNetNotifier;
+(Reachability*)getRatyWifi;
+(Reachability*)getRaty3G;
@end
