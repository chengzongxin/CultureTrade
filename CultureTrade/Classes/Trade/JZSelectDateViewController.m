//
//  JZSelectDateViewController.m
//  FT Trader
//
//  Created by SZFT1 on 14-9-25.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#import "JZSelectDateViewController.h"
#import "UIBarButtonItem+helper.h"

@interface JZSelectDateViewController ()
{
    UIButton* sl_hint_lbl;
    NSDate* startDate;
    NSDate* endDate;
    UIButton*  startdate_view ;
    UIButton*  enddate_view ;
    #if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
    UIAlertController* alert;
    UIDatePicker* startDatePicker;
    UIDatePicker* endDatePicker;
    #endif
}
@end

@implementation JZSelectDateViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    
    startDate = [NSDate dateWithTimeIntervalSince1970:([[NSDate date] timeIntervalSince1970] - 86400*2)];
    endDate = [NSDate dateWithTimeIntervalSince1970:([[NSDate date] timeIntervalSince1970] - 86400)];
    
    
    self.navigationItem.rightBarButtonItem = [UIBarButtonItem itemWithIcon:@"search" highlightedIcon:nil target:self action:@selector(done:)];
    
   int top = 0;
//    UIColor* borderColor = [UIColor colorWithRed:209.0f/255.0f green:209.0f/255.0f blue:209.0f/255.0f alpha:1.0f];//[UIColor grayColor];
    int _width = App_Frame_Width;
    UIView* _sl_tp_view = [[UIView alloc] initWithFrame:CGRectMake(0, top, _width, 81)];
    _sl_tp_view.backgroundColor = app_item_bg;
//   UIView* _sl_tp_view = [[UIView alloc] initWithFrame:CGRectMake(5, top, _width, 81)];
//    _sl_tp_view.layer.cornerRadius = 8.0;
//    _sl_tp_view.layer.borderWidth = 1;
//    _sl_tp_view.layer.borderColor = borderColor.CGColor;
    
    UIView *_sp_view = [[UIView alloc] initWithFrame:CGRectMake(0, 40, _width, 1)];
    _sp_view.backgroundColor = app_item_separator_color;
    [_sl_tp_view addSubview:_sp_view];
    
    
    UIView* start = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 40, _width)];
    
    int title_width = 80;
    int _sl_width = 100;
    UILabel* sllbl = [[UILabel alloc] initWithFrame:CGRectMake(4, 1, title_width, 36)];
    sllbl.text = LocalizedStringByInt(8058);
    sllbl.textColor = app_title_text_color;
    
    [start addSubview:sllbl];
    
    UILabel* tplbl = [[UILabel alloc] initWithFrame:CGRectMake(4, 43, title_width, 36)];
    tplbl.text = LocalizedStringByInt(8059);
    tplbl.textColor = app_title_text_color;
    [start addSubview:tplbl];

//    UIImageView *arr_image = [[UIImageView alloc] initWithFrame:CGRectMake(title_width + ( _width - _sl_width - 10), 18, 18, 11)];
//    [arr_image setImage:[UIImage imageNamed:@"down_arrow"]];
//    arr_image.tag = 0;
//    arr_image.userInteractionEnabled = YES;
//    [start addSubview:arr_image];

//    sl_hint_lbl = [[UIButton alloc] initWithFrame:CGRectMake(title_width + 1 , 1, _width - title_width, 36)];
////    sl_hint_lbl.
////    sl_hint_lbl.font = [UIFont systemFontOfSize:16.0];
////    sl_hint_lbl.textColor = [UIColor redColor];
////    sl_hint_lbl.text= [JZGlobal stringFromDateDay:startDate];
//    [sl_hint_lbl setTitle:[JZGlobal stringFromDateDay:startDate] forState:UIControlStateNormal];
////    sl_hint_lbl.backgroundColor=[UIColor redColor];
//    [sl_hint_lbl addTarget:self action:@selector(sl) forControlEvents:UIControlEventTouchUpInside];
//    [sl_hint_lbl setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
//    [start addSubview:sl_hint_lbl];
    
    [_sl_tp_view addSubview:start];
    
    
    
    UILabel* tp_hint_lbl = [[UILabel alloc] initWithFrame:CGRectMake(title_width + 1, 43, _width - _sl_width, 36)];
    tp_hint_lbl.font = [UIFont systemFontOfSize:14.0];
    tp_hint_lbl.textColor = [UIColor redColor];
    //    tp_hint_lbl.text=@"asdsa";
    [_sl_tp_view addSubview:tp_hint_lbl];
    
    [self.view addSubview:_sl_tp_view];
    
    
    startdate_view = [[UIButton alloc] initWithFrame:CGRectMake(title_width + 1 , top + 2, _width - title_width, 36)];
    [startdate_view setTitle:[NSString stringFromDateDay:startDate] forState:UIControlStateNormal];
    [startdate_view addTarget:self action:@selector(select_start_date:) forControlEvents:UIControlEventTouchUpInside];
    [startdate_view setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
    [self.view addSubview:startdate_view];
    
    UIImageView *arr_image = [[UIImageView alloc] initWithFrame:CGRectMake(title_width + ( _width - _sl_width - 10), top + 16, 18, 11)];
    [arr_image setImage:[UIImage imageNamed:@"down_arrow"]];
    arr_image.tag = 0;
    arr_image.userInteractionEnabled = YES;
    [self.view addSubview:arr_image];
    
    UITapGestureRecognizer *singleTap1 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(select_start_date:)];
    [arr_image addGestureRecognizer:singleTap1];

    
    enddate_view = [[UIButton alloc] initWithFrame:CGRectMake(title_width + 1 , top + 2 + 40, _width - title_width, 36)];
    [enddate_view setTitle:[NSString stringFromDateDay:endDate] forState:UIControlStateNormal];
    [enddate_view addTarget:self action:@selector(select_end_date:) forControlEvents:UIControlEventTouchUpInside];
    [enddate_view setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
    [self.view addSubview:enddate_view];
    
    UIImageView *arr_image2 = [[UIImageView alloc] initWithFrame:CGRectMake(title_width + ( _width - _sl_width - 10), top + 16 + 40, 18, 11)];
    [arr_image2 setImage:[UIImage imageNamed:@"down_arrow"]];
    arr_image2.tag = 1;
    arr_image2.userInteractionEnabled = YES;
    
    UITapGestureRecognizer *singleTap2 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(select_end_date:)];
    [arr_image addGestureRecognizer:singleTap2];
    
    [self.view addSubview:arr_image2];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

-(void) initAlertController:(NSInteger)tag {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
    alert=nil;
    alert = [UIAlertController alertControllerWithTitle:nil
                                                message:@"\n\n\n\n\n\n\n\n\n\n\n"// change UIAlertController height
                                         preferredStyle:UIAlertControllerStyleActionSheet];
    UIToolbar *toolView = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, alert.view.frame.size.width-16, 40)];
    toolView.backgroundColor = [UIColor blackColor]; //set it's background
    toolView.barStyle = UIBarStyleBlackTranslucent;
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 160, 40)];
    titleLabel.backgroundColor = [UIColor clearColor];
    titleLabel.font = [UIFont systemFontOfSize:14];
    titleLabel.textColor = [UIColor whiteColor];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    //    titleLabel.text = title;
    UIBarButtonItem *bbtTitle = [[UIBarButtonItem alloc] initWithCustomView:titleLabel];
    UIBarButtonItem *bbtSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
    UIBarButtonItem *bbtOK = [[UIBarButtonItem alloc] initWithTitle:LocalizedStringByInt(8027) style:UIBarButtonItemStyleDone target:self action:@selector(OKWithPicker:)];
    bbtOK.tag = tag;
    bbtOK.width = 60.f;
    UIBarButtonItem *bbtCancel = [[UIBarButtonItem alloc] initWithTitle:LocalizedStringByInt(8014) style:UIBarButtonItemStylePlain target:self action:@selector(CancleWithPicker:)];
    bbtCancel.width = 60.f;
    bbtCancel.tag = tag;
    toolView.items = [NSArray arrayWithObjects:bbtCancel,bbtSpace,bbtTitle,bbtSpace,bbtOK, nil];
    
    [alert.view addSubview:toolView];
    
    //    datePicker = [[UIDatePicker alloc] initWithFrame:CGRectMake(0,44, 320, 216)];
    //    [datePicker setDatePickerMode:UIDatePickerModeDate];
    //
    //
    //
    //    //        datePicker date
    //    //创建时间格式化实例对象
    //    //        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    //    //设置时间格式
    //    //        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm"];
    //
    //
    //    [alert.view addSubview:datePicker];
    //    [self presentViewController:alert animated:NO completion:nil];
#endif
}

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
-(void) CancleWithPicker:(id) sender {
    int tag = (int)((UIBarButtonItem*)sender).tag;
    switch (tag) {
        case 100:
            break;
        default:
            break;
    }
    endDatePicker = nil;
    startDatePicker =  nil;
    [alert dismissViewControllerAnimated:YES completion:nil];
}

-(void) OKWithPicker:(id) sender{
    int tag = (int)((UIBarButtonItem*)sender).tag;
    switch (tag) {
        case 100:
            startDate = startDatePicker.date;
            [startdate_view setTitle:[NSString stringFromDateDay:startDate] forState:UIControlStateNormal];
            startdate_view.titleLabel.text = [NSString stringFromDateDay:startDate];
            startDatePicker = nil;
            break;
        case 101:
            endDate = endDatePicker.date;
            [enddate_view setTitle:[NSString stringFromDateDay:endDate] forState:UIControlStateNormal];
            enddate_view.titleLabel.text = [NSString stringFromDateDay:endDate];
            endDatePicker = nil;
            break;
        default:
            break;
    }
    
    [alert dismissViewControllerAnimated:YES completion:nil];
}
#endif
-(void)done:(id)sender
{
    NSDateComponents *scomponents = [[NSCalendar currentCalendar] components:NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear fromDate:[NSDate date]];
    NSDateComponents *ecomponents = [[NSCalendar currentCalendar] components:NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear fromDate:endDate];
    int sdate = (int)(scomponents.year * 10000 + scomponents.month*100 + scomponents.day);
    int edate = (int)(ecomponents.year * 10000 + ecomponents.month*100 + ecomponents.day);
    if(sdate <= edate){
        showAlert(LocalizedStringByInt(9048));
        return;
    }
    scomponents = [[NSCalendar currentCalendar] components:NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear fromDate:startDate];
    sdate = (int)(scomponents.year * 10000 + scomponents.month*100 + scomponents.day);
    if(sdate > edate){
        showAlert(LocalizedStringByInt(8053));
        return;
    }
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    if (self.requestType == isHistoryOrder) {
        [[NSTradeEngine setup] trade_request_historyorder_fromDate:[formatter stringFromDate:startDate] toDate:[formatter stringFromDate:endDate]];
    }else if(self.requestType == isHistoryTurnOver){
        [[NSTradeEngine setup] trade_request_historyturnover_fromDate:[formatter stringFromDate:startDate] toDate:[formatter stringFromDate:endDate]];
    }
    
    [self.navigationController popViewControllerAnimated:YES];
}


-(void) select_start_date:(id)sender
{
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
    [self initAlertController:100];
    startDatePicker = [[UIDatePicker alloc] initWithFrame:CGRectMake(0,44, 320, 216)];
    startDatePicker.backgroundColor = [UIColor whiteColor];
    [startDatePicker setDatePickerMode:UIDatePickerModeDate];
    startDatePicker.date = startDate;
    [alert.view addSubview:startDatePicker];
    [self presentViewController:alert animated:NO completion:nil];
#endif
} else {
    JZSelectView * select = [[JZSelectView alloc] initWithSelectDate:@""];
    select.tag = 0;
    [select selectDate:startDate];
    select.theDelegate = self;
    [select showInView:self.view];
}
}

-(void) select_end_date:(id)sender
{ if([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
    [self initAlertController:101];
    endDatePicker = [[UIDatePicker alloc] initWithFrame:CGRectMake(0,44, 320, 216)];
    endDatePicker.backgroundColor = [UIColor whiteColor];
    [endDatePicker setDatePickerMode:UIDatePickerModeDate];
    endDatePicker.date = endDate;

    [alert.view addSubview:endDatePicker];
    [self presentViewController:alert animated:NO completion:nil];
#endif
} else {
    JZSelectView * select = [[JZSelectView alloc] initWithSelectDate:@""];
    select.tag = 1;
    [select selectDate:endDate];
    select.theDelegate = self;
    [select showInView:self.view];
}
}

-(void)dismissWithSelectDate:(id)sender date:(NSDate*)date cancle:(BOOL)cancle
{
    if(cancle) return;
    int tag =(int)((JZSelectView*)sender).tag;
    if(tag == 0){
        startDate = date;
        [startdate_view setTitle:[NSString stringFromDateDay:startDate] forState:UIControlStateNormal];
    } else {
        endDate = date;
        [enddate_view setTitle:[NSString stringFromDateDay:endDate] forState:UIControlStateNormal];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
