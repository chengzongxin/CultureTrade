//
//  JZSelectView.h
//  FT Trader
//
//  Created by gaobo zhong on 12-5-29.
//  Copyright (c) 2012年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol UISelectViewDelegate;

@interface JZSelectView : UIActionSheet <UIPickerViewDelegate>
{
	int rows;
}

@property(strong, readwrite, nonatomic) id<UISelectViewDelegate> theDelegate;
-(id)initWithSelectDate:(NSString*)date;
-(id)initWithSelectData:(NSArray*)data;
-(id)initWithSelectTitle:(NSString*)title dataArray:(NSArray*)data;
-(id)initWithSelectTitle:(NSString*)title doneTitle:(NSString*)donetitle cancelTitle:(NSString*)cancletitle dataArray:(NSArray*)data;
-(void)selectRow:(NSInteger)row animated:(BOOL)animated;
-(void)selectItem:(NSString*)item animated:(BOOL)animated;
- (NSInteger)selectedRow;
-(void) selectDate:(NSDate*)date;
@end

@protocol UISelectViewDelegate <NSObject>
    -(void)dismissWithSelect:(id)sender didselectRow:(NSInteger)row;
-(void)dismissWithSelectDate:(id)sender date:(NSDate*)date cancle:(BOOL)cancle;
    -(void)onDidSelect:(id)selectView didSelectRow:(NSInteger)row data:(id)data;
@end







