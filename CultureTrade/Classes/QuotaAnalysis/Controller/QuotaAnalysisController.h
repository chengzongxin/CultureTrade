//
//  QuotaAnalysisController.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "DockController.h"
#import "GuideView.h"
#import "ButtonListView.h"

@protocol QuotaAnalysisControllerDelegate <ButtonListViewDelegate>

@end


@interface QuotaAnalysisController : DockController
{
    GuideView *_guideView;
}
@property (nonatomic,retain) GuideView *guideView;
@property (nonatomic,retain) ButtonListView *buttonListView;
@property (nonatomic,weak) id<QuotaAnalysisControllerDelegate> delegate;
@end
