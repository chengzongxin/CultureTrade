//
//  BasicAlertManager.h
//  CultureTrade
//
//  Created by SZFT1 on 15/12/9.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BasicAlertView.h"

@interface BasicAlertManager : NSObject
{
    NSMutableArray *_alertViewQueue;
    id _currentAlertView;
    
    UIImageView *_alertView;
    UIView *_blackBG;
    UILabel *_loadingLabel;
    
    NSTimer *_loadingTimer;
    
    Boolean _isDismissing;
}

singleton_interface(BasicAlertManager)

+(BasicAlertManager *)sharedAlertViewManager;

-(BasicAlertView *)showAlertView:(NSString*)message;
//-(XYLoadingView*)showLoadingView:(NSString*)message;

-(void)show:(id)entity;


-(void)dismiss:(id)entity;
-(void)dismiss:(id)entity button:(int)buttonIndex;
-(void)dismissLoadingView:(id)entity withFailedMessage:(NSString*)message;
-(void)cleanupAllViews;
 
@end
