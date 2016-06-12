//
//  storeUIView.h
//  qwlwStore
//
//  Created by zengxw on 13-5-22.
//  Copyright (c) 2013年 zengxw. All rights reserved.
//

@protocol StoreUIViewDelegate<NSObject>
-(BOOL)didTounchEndView;
@optional
-(void)didHitTest:(CGPoint)point;
@end

#define StoreUIVEIWTAG 1984

#import <UIKit/UIKit.h>

@interface storeUIView : UIView
{
    UIImageView *imgView;
}
@property (nonatomic, strong) id<StoreUIViewDelegate> delegate;

-(void)setHengImageBgView;
-(void)setShuImageBgView;

-(void)resetStoreViewFrame:(CGRect)frame;

@end
