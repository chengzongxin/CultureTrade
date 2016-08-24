//
//  TimeLineView.h
//  CultureTrade
//
//  Created by SZFT1 on 16/1/5.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>
typedef void (^updateBlock)(id);

@protocol TimeLineViewDelegate <NSObject>
@optional
- (void)login_rsp_ui:(int)nRet type:(int)nType;

@end

@interface TimeLineView : UIView <NSTradeEngineDelegate>
{
    updateBlock finishUpdateBlock; // 定义一个block回调 更新界面
}

@property (nonatomic,weak) id<TimeLineViewDelegate> delegate;

@property (nonatomic,retain) UIFont *font;
@property (nonatomic,assign) CGFloat lineWidth;

- (void)viewWillApper;
- (void)viewWillDisapper;

@end
