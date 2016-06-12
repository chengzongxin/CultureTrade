//
//  UIDeviceHardware.m
//  iVMS2
//
//  Created by  on 12-4-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "UIDeviceHardware.h"

#include <sys/types.h>

#include <sys/sysctl.h>

@implementation UIDeviceHardware

- (NSString *) platform{
    
    size_t size;
    
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    
    char *machine = (char *)malloc(size);
    
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    
    free(machine);
    
    return platform;
    
}

- (BOOL) hardwareGood
{
    NSString *platform = [self platform];
    
    if ([platform isEqualToString:@"iPhone1,1"])    return NO;
    
    if ([platform isEqualToString:@"iPhone1,2"])    return NO;
    
    if ([platform isEqualToString:@"iPhone2,1"])    return NO;
    
    if ([platform isEqualToString:@"iPhone3,1"])    return YES;
    
    if ([platform isEqualToString:@"iPod1,1"])      return NO;
    
    if ([platform isEqualToString:@"iPod2,1"])      return NO;
    
    if ([platform isEqualToString:@"iPod3,1"])      return NO;
    
    if ([platform isEqualToString:@"iPod4,1"])      return NO;
    
    if ([platform isEqualToString:@"iPad1,1"])      return YES;
    
    if ([platform isEqualToString:@"i386"])         return YES;
    
    return YES;
}

- (NSString *) platformString{
    
    NSString *platform = [self platform];
    
    if ([platform isEqualToString:@"iPhone1,1"])    return @"iPhone 1G";
    
    if ([platform isEqualToString:@"iPhone1,2"])    return @"iPhone 3G";
    
    if ([platform isEqualToString:@"iPhone2,1"])    return @"iPhone 3GS";
    
    if ([platform isEqualToString:@"iPhone3,1"])    return @"iPhone 4";
    
    if ([platform isEqualToString:@"iPod1,1"])      return @"iPod Touch 1G";
    
    if ([platform isEqualToString:@"iPod2,1"])      return @"iPod Touch 2G";
    
    if ([platform isEqualToString:@"iPod3,1"])      return @"iPod Touch 3G";
    
    if ([platform isEqualToString:@"iPod4,1"])      return @"iPod Touch 4G";
    
    if ([platform isEqualToString:@"iPad1,1"])      return @"iPad";
    
    if ([platform isEqualToString:@"i386"])         return @"Simulator";
    
    return platform;
    
}

@end
