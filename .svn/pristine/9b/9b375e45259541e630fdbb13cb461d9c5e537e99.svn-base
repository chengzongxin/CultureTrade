//
//  BasicAlertView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^AlertViewBlock)(int buttonIndex);

typedef enum
{
    XYButtonStyleGray = 1,
    XYButtonStyleGreen = 2,
    XYButtonStyleDefault = XYButtonStyleGreen,
} XYButtonStyle;

@interface BasicAlertView : NSObject

@property (copy, nonatomic) NSString *title;
@property (copy, nonatomic) NSString *subtitle;
@property (copy, nonatomic) NSString *message;
@property (retain, nonatomic) NSArray *buttons;
@property (readonly, nonatomic) NSMutableArray *buttonsStyle;
@property (copy, nonatomic) AlertViewBlock blockAfterDismiss;

+(id)alertViewWithTitle:(NSString*)title
               subtitle:(NSString*)subtitle
                message:(NSString*)message
                buttons:(NSArray*)buttonTitles
           afterDismiss:(AlertViewBlock)block;

-(id)initWithTitle:(NSString*)title
          subtitle:(NSString*)subtitle
           message:(NSString*)message
           buttons:(NSArray*)buttonTitles
      afterDismiss:(AlertViewBlock)block;

-(void)setButtonStyle:(XYButtonStyle)style atIndex:(int)index;

-(void)show;
-(void)dismiss:(int)buttonIndex;

@end