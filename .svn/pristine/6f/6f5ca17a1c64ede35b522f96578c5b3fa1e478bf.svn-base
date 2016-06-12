//
//  FeedbackController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/4/27.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "FeedbackController.h"

@interface FeedbackController ()

@end

@implementation FeedbackController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"反馈";
    
    [self addTextView];
    
    [self addButton];
}

- (void)addTextView
{
    self.textView = [[UITextView alloc] initWithFrame:CGRectMake(0, 20, ScreenSize.width, ScreenSize.height * 0.4)]; //初始化大小并自动释放
    
    self.textView.textColor = [UIColor whiteColor];//设置textview里面的字体颜色
    
    self.textView.font = [UIFont fontWithName:@"Arial" size:18.0];//设置字体名字和字体大小
    
    self.textView.delegate = self;//设置它的委托方法
    
    self.textView.backgroundColor = COLOR_ME_TEXTVIEW_BG;//设置它的背景颜色
    
    self.textView.text = @"输入你的反馈......";//设置它显示的内容
    
    self.textView.returnKeyType = UIReturnKeyDefault;//返回键的类型
    
    self.textView.keyboardType = UIKeyboardTypeDefault;//键盘类型
    
    self.textView.scrollEnabled = YES;//是否可以拖动
    
    self.textView.autoresizingMask = UIViewAutoresizingFlexibleHeight;//自适应高度
    
    [self.view addSubview: self.textView];//加入到整个页面中
}

- (void)textViewDidBeginEditing:(UITextView *)textView {
    
    UIBarButtonItem *done = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(leaveEditMode)];
    
    self.navigationItem.rightBarButtonItem = done;
    
}

- (void)textViewDidEndEditing:(UITextView *)textView {
    
    self.navigationItem.rightBarButtonItem = nil;
    
}

- (void)leaveEditMode {
    
    [self.textView resignFirstResponder];
    
}

- (void)addButton
{
    CGSize size = [UIImage imageNamed:@"confirm"].size;
    CGFloat btnX = (self.view.frame.size.width - size.width * 2 - 10)/2;
    CGFloat btnY = CGRectGetMaxY(self.textView.frame) + 50;
    
    CGRect clearRect = (CGRect){{btnX,btnY},size};
    UIButton *clearBtn =  [self addButtonWithRect:clearRect Img:@"clear"];
    [clearBtn addTarget:self action:@selector(ClickClear) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:clearBtn];
    
    CGRect confirmRect = (CGRect){{CGRectGetMaxX(clearBtn.frame)+10,btnY},size};
    UIButton *confirmBtn =  [self addButtonWithRect:confirmRect Img:@"confirm"];
    [confirmBtn addTarget:self action:@selector(clickConfirm) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:confirmBtn];
    
}

- (UIButton *)addButtonWithRect:(CGRect)rect Img:(NSString *)img
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = rect;
    UIImage *Img = [UIImage imageNamed:img];
    [btn setImage:Img forState:UIControlStateNormal];
    return btn;
}

- (void)clickConfirm
{
    
}

@end
