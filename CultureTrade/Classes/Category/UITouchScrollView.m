//
//  UITouchScrollView.m
//  CultureTrade
//
//  Created by SZFT1 on 16/2/26.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "UITouchScrollView.h"

@implementation UITouchScrollView
@synthesize touchesdelegate=_touchesdelegate;
- (void) touchesEnded: (NSSet *) touches withEvent: (UIEvent *) event {
    
    if (!self.dragging) {
        //run at ios5 ,no effect;
        [self.nextResponder touchesEnded: touches withEvent:event];
        if (_touchesdelegate!=nil) {
            
            [_touchesdelegate scrollViewTouchesEnded:touches withEvent:event whichView:self];
        }
        NSLog(@"UITouchScrollView nextResponder touchesEnded");
    }
    [super touchesEnded: touches withEvent: event];
    
}
@end