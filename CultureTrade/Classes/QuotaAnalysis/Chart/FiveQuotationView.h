//
//  FiveQuotationView.h
//  CultureTrade
//
//  Created by SZFT1 on 16/8/25.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FiveQuotationView : UIView
{
    UIFont *_font;
}
@property (nonatomic,retain) UIFont *font;

@property (nonatomic,retain) NSMutableArray *buyQuotation;
@property (nonatomic,retain) NSMutableArray *buyVolume;
@property (nonatomic,retain) NSMutableArray *sellQuotation;
@property (nonatomic,retain) NSMutableArray *sellVolume;

- (void)setBuyQuotation:(NSMutableArray *)buyQuotation;
- (void)setSellQuotation:(NSMutableArray *)sellQuotation;
- (void)setBuyVolume:(NSMutableArray *)buyVolume;
- (void)setSellVolume:(NSMutableArray *)sellVolume;
@end
