//
//  UIDeviceHardware.h
//  iVMS2
//
//  Created by  on 12-4-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UIDeviceHardware : NSObject

- (NSString *) platform;
- (NSString *) platformString;
- (BOOL) hardwareGood;
@end
