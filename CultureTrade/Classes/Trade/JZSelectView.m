//
//  JZSelectView.m
//  FT Trader
//
//  Created by gaobo zhong on 12-5-29.
//  Copyright (c) 2012年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#import "JZSelectView.h"

#define kDuration 0.3
#define BARBUTTON(TITLE,STYLE,SELECTOR) [[UIBarButtonItem alloc]initWithTitle:TITLE style:STYLE target:self action:SELECTOR]
#define BARBUTTONSTYLE(STYLE) [[UIBarButtonItem alloc] initWithBarButtonSystemItem:STYLE target:nil action:nil]

@interface JZSelectView ()
{
    UIDatePicker* datePicker;
}
    @property (strong, readwrite, nonatomic) UIToolbar* toolBar;
    @property (strong, readwrite, nonatomic) UIView* view;
    @property (strong, readwrite, nonatomic) UILabel* contentview;
    @property (strong, readwrite, nonatomic) UIPickerView* pickerView;
    @property (strong, readwrite, nonatomic) NSArray *dataArray;
    @property (nonatomic)BOOL tipHidden;
    
@end

@implementation JZSelectView

float height = 284.0;
int currRow = 0;
int selectRow = 0;

-(id)init{
	self = [super init];
    if (self) {
	}
	return self;
}

-(id)initWithSelectData:(NSArray*)data{
	return [self initWithSelectTitle:@"                   " dataArray:data];
}

-(id)initWithSelectTitle:(NSString*)title dataArray:(NSArray*)data{
	return [self initWithSelectTitle:title doneTitle:@"完成" cancelTitle:@"取消" dataArray:data];
}

-(id)initWithSelectTitle:(NSString*)title doneTitle:(NSString*)donetitle cancelTitle:(NSString*)cancletitle dataArray:(NSArray*)data{
	self = [super init];
    if (self) {
		self.dataArray = data;
        rows = (int)[data count];
		int theight = height - 40;
		int btnnum = theight/50;
		for(int i=0; i<btnnum; i++) [self addButtonWithTitle:@" "];
		self.toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, 320, 44)];
		self.toolBar.barStyle = UIBarStyleDefault;
        UIBarButtonItem *fixedButton  = BARBUTTONSTYLE(UIBarButtonSystemItemFlexibleSpace);
		NSArray *array = [NSArray arrayWithObjects:
                          BARBUTTON(cancletitle, UIBarButtonItemStylePlain, @selector(docancel)),
                          fixedButton, 
                          BARBUTTON(title, UIBarButtonItemStylePlain, nil),
                          fixedButton, 
                          BARBUTTON(donetitle, UIBarButtonItemStyleDone, @selector(done)),
                          nil];
		[self.toolBar setItems: array];
		[self addSubview:self.toolBar];
		self.view = [[UIView alloc] initWithFrame:CGRectMake(0, 44, 320, height-44)];
		self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
		[self addSubview:self.view];
		self.pickerView = [[UIPickerView alloc] initWithFrame:CGRectMake(0,44, 320, 216) ];
		self.pickerView.delegate = self;
		self.pickerView.showsSelectionIndicator = YES;
		[self addSubview:self.pickerView];
		self.contentview = [[UILabel alloc] initWithFrame:CGRectMake(0.0, 130.0, 320.0, 40.0)];
		self.contentview.backgroundColor = [UIColor clearColor];
        self.contentview.textAlignment = NSTextAlignmentCenter;
        [self.contentview setHidden:YES];
		[self addSubview: self.contentview];
    }
    return self;
}


-(id)initWithSelectDate:(NSString*)date
{
    self = [super init];
    if (self) {
		int theight = height - 40;
		int btnnum = theight/50;
		for(int i=0; i<btnnum; i++) [self addButtonWithTitle:@" "];
		self.toolBar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, 320, 44)];
		self.toolBar.barStyle = UIBarStyleDefault;
		UIBarButtonItem *fixedButton  = BARBUTTONSTYLE(UIBarButtonSystemItemFlexibleSpace);
		NSArray *array = [NSArray arrayWithObjects:
                          BARBUTTON(@"取消", UIBarButtonItemStyleBordered, @selector(docancel)),
                          fixedButton,
                          BARBUTTON(@"选择日期", UIBarButtonItemStylePlain, nil),
                          fixedButton,
                          BARBUTTON(@"完成", UIBarButtonItemStyleDone, @selector(done)),
                          nil];
		[self.toolBar setItems: array];
		[self addSubview:self.toolBar];
		self.view = [[UIView alloc] initWithFrame:CGRectMake(0, 44, 320, height-44)];
		self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
		[self addSubview:self.view];

        datePicker = [[UIDatePicker alloc] initWithFrame:CGRectMake(0,44, 320, 216)];
        [datePicker setDatePickerMode:UIDatePickerModeDate];
//        datePicker date
        //创建时间格式化实例对象
//        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        //设置时间格式
//        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
        [self addSubview: datePicker];
    }
    return  self;
}

-(void) setTipHidden:(BOOL)tiphi{
    self.contentview.hidden = tiphi;
}

-(BOOL) isTipHidden{
    return self.contentview.isHidden;
}

-(void) selectRow:(NSInteger)row animated:(BOOL)animated{
	currRow = row;
	selectRow = row;
	[self.pickerView selectRow:row inComponent:0 animated:animated];
	[self pickerView:self.pickerView didSelectRow:row inComponent:0];
}

-(void) selectItem:(NSString*)item animated:(BOOL)animated{
	int row = [self.dataArray indexOfObject:item];
	[self selectRow:row animated:animated];
}

-(void) selectDate:(NSDate*)date{
    if(nil == datePicker) return;
    datePicker.date = date;
}

- (NSInteger)selectedRow{
	return [self.pickerView selectedRowInComponent:0];
}

-(void)done{
	[self dismissWithClickedButtonIndex:0 animated:YES];
    if(nil != datePicker && self.theDelegate && [self.theDelegate respondsToSelector:@selector(dismissWithSelectDate:date:cancle:)]){
        [self.theDelegate dismissWithSelectDate:self date:datePicker.date cancle:NO];
        return;
    }

    if(self.theDelegate && [self.theDelegate respondsToSelector:@selector(dismissWithSelect:didselectRow:)]) [self.theDelegate dismissWithSelect:self didselectRow:selectRow] ;
}

-(void)docancel{
    if(nil != datePicker && self.theDelegate && [self.theDelegate respondsToSelector:@selector(dismissWithSelectDate:date:cancle:)]){
        [self.theDelegate dismissWithSelectDate:self date:datePicker.date cancle:YES];
        [self dismissWithClickedButtonIndex:0 animated:YES];
        return;
    }
	[self selectRow:currRow animated:YES];
	[self done];
}

//-(void)dealloc{
//	[toolBar release];
//	[view release];
//	[contentview release];
//	[pickerView release];
//	[dataArray release];
//	[view release];
//	[super dealloc];
//}

//- (void)showInView:(UIView *) view
//{
//    CATransition *animation = [CATransition  animation];
//    animation.delegate = self;
//    animation.duration = kDuration;
//    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
//    animation.type = kCATransitionPush;
//    animation.subtype = kCATransitionFromTop;
//    [self setAlpha:1.0f];
//    [super showInView:view];
//    [self.layer addAnimation:animation forKey:@"DDLocateView"];
//    self.frame = CGRectMake(0, view.frame.size.height - self.frame.size.height, self.frame.size.width, self.frame.size.height);
//    [view addSubview:self];
//}

#pragma mark -
#pragma mark 

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView*)pickerView{
	return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
	return rows;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
	return [self.dataArray objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
    selectRow = row;
	if(self.theDelegate  && [self.theDelegate respondsToSelector:@selector(onDidSelect:didSelectRow:data:)])
        [self.theDelegate onDidSelect:self didSelectRow:row data:self.dataArray];
    if(!self.contentview.isHidden) self.contentview.text = [self.dataArray objectAtIndex:row];
}

@end