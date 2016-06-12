//
//  BasicAlertView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BasicAlertView.h"
#import "BasicAlertManager.h"

@implementation BasicAlertView

+(id)alertViewWithTitle:(NSString*)title
               subtitle:(NSString*)subtitle
                message:(NSString*)message
                buttons:(NSArray*)buttonTitles
           afterDismiss:(AlertViewBlock)block
{
    return [[BasicAlertView alloc] initWithTitle:title subtitle:subtitle message:message buttons:buttonTitles afterDismiss:block];
}

-(id)initWithTitle:(NSString*)title
           subtitle:(NSString*)subtitle
           message:(NSString*)message
           buttons:(NSArray*)buttonTitles
      afterDismiss:(AlertViewBlock)block
{
    self = [self init];
    if(self)
    {
        self.title = title;
        self.subtitle = subtitle;
        self.message = message;
        self.buttons = buttonTitles;
        self.blockAfterDismiss = block;
    }
    return self;
}

-(void)setButtonStyle:(XYButtonStyle)style atIndex:(int)index
{
    if(index < [_buttonsStyle count])
        [_buttonsStyle replaceObjectAtIndex:index withObject:[NSNumber numberWithInt:style]];
}

-(void)setButtons:(NSArray *)buttons
{
    _buttons = buttons;
    _buttonsStyle = nil;
    _buttonsStyle = [NSMutableArray arrayWithCapacity:buttons.count];
    for (int i = 0; i < buttons.count; i++)
        [_buttonsStyle addObject:[NSNumber numberWithInt:XYButtonStyleDefault]];
}

-(void)show
{
    [[BasicAlertManager shareInstance] show:self];
}

-(void)dismiss:(int)buttonIndex
{
    [[BasicAlertManager shareInstance] dismiss:self button:buttonIndex];
}


@end
