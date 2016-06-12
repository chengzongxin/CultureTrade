//
//  LineView.h
//  CultureTrade
//
//  Created by SZFT1 on 15/11/25.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void (^updateBlock)(id);

@interface LineView : UIView
{
    updateBlock finishUpdateBlock; // 定义一个block回调 更新界面
}
@property (nonatomic,retain) UIFont *font;
@property (nonatomic,assign) CGFloat lineWidth;

- (void)bigScale;
- (void)smallScale;
- (void)loadHisKData:(KLineType)type productID:(NSString *)productID;

//- (void)viewWillApper;
//- (void)viewWillDisapper;
@end
