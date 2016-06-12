//
//  UITouchScrollView.h
//  CultureTrade
//
//  Created by SZFT1 on 16/2/26.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@protocol UIScrollViewTouchesDelegate
-(void)scrollViewTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event whichView:(id)scrollView;
@end


@interface UITouchScrollView : UIScrollView

@property(nonatomic,assign) id<UIScrollViewTouchesDelegate> touchesdelegate;

@end