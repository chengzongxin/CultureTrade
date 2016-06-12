//
//  NSTradeBizUtils.m
//  FT Trader
//
//  Created by SZFT1 on 15/1/7.
//  Copyright (c) 2015年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#import "NSTradeBizUtils.h"
#import "tradebizUtils.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

//const NSString* PATTER = @"--";
static NSTradeBizUtils* instanceUtils = nil;

@implementation NSTradeBizUtils

+(NSTradeBizUtils*) sharedInstance {
    if (instanceUtils == nil) instanceUtils = [[NSTradeBizUtils alloc] init];
    return instanceUtils;
}

-(NSString*)toNSString:(const char *) c {
    if(c != NULL && strlen(c) > 0) {
//        NSString* str =  [NSString stringWithCString:c encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF8)];
        return [NSString stringWithCString:c encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000)];
    }
    return @"";
}


-(double) round:(double)f dec:(int)dec{
    return Rounding(f, dec);
}

-(BOOL) isPhone:(NSString*)str {
    const char* pstr = toCString(str);
    return isPhone(pstr) == 0 ? TRUE : FALSE;
}

-(BOOL) isShenFenzheng:(NSString*)str {
    const char* pstr = toCString(str);
    return isValidShenFenzheng(pstr) ? TRUE : FALSE;
}

-(NSString*)formatTime:(int)time ptt:(NSString*)ptt {
    NSString* str = [NSString stringWithFormat:@"%d", time];
    int size = str.length;
    return [NSString stringWithFormat:@"%@%@%@", [str substringToIndex: size - 2], ptt, [str substringFromIndex: size - 2]];
}

-(NSString*)formatDecimal:(double)price dot:(int)dot {
    NSString * strprice;
    if (dot == 0) {
        strprice = [NSString stringWithFormat:@"%d", (int)price];
    }  else if( 1 == dot) {
        strprice = [NSString stringWithFormat:@"%0.1f", price];
    } else if( 2 == dot) {
        strprice = [NSString stringWithFormat:@"%0.2f", price];
    } else if( 3 == dot) {
        strprice = [NSString stringWithFormat:@"%0.3f", price];
    } else if( 4 == dot) {
        strprice = [NSString stringWithFormat:@"%0.4f", price];
    } else {
        strprice = [NSString stringWithFormat:@"%0.5f", price];
    }
    return strprice;
}

-(NSString*)format:(int)number decimal:(int)dot {
    double d = number * 1.0 / pow(10, dot);
    return [self formatDecimal:d dot:dot];
}

- (NSString *)localIPAddress
{
    NSString *localIP = nil;
    struct ifaddrs *addrs;
    if (getifaddrs(&addrs)==0) {
        const struct ifaddrs *cursor = addrs;
        while (cursor != NULL) {
            if (cursor->ifa_addr->sa_family == AF_INET && (cursor->ifa_flags & IFF_LOOPBACK) == 0)
            {
                //NSString *name = [NSString stringWithUTF8String:cursor->ifa_name];
                //if ([name isEqualToString:@"en0"]) // Wi-Fi adapter
                {
                    localIP = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)cursor->ifa_addr)->sin_addr)];
                    break;
                }
            }
            cursor = cursor->ifa_next;
        }
        freeifaddrs(addrs);
    }
    return localIP;
}
@end
