//
//  AboutController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/4/27.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "AboutController.h"

@interface AboutController ()

@end

@implementation AboutController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"关于";
    
    [self addTextView];
}

- (void)addTextView
{
    self.textView = [[UITextView alloc] initWithFrame:CGRectMake(0, 20, ScreenSize.width, ScreenSize.height * 0.4)]; //初始化大小并自动释放
    
    self.textView.textColor = [UIColor whiteColor];//设置textview里面的字体颜色
    
    self.textView.font = [UIFont fontWithName:@"Arial" size:18.0];//设置字体名字和字体大小
    
    self.textView.delegate = self;//设置它的委托方法
    
    self.textView.backgroundColor = COLOR_ME_TEXTVIEW_BG;//设置它的背景颜色
    
    NSString *key = (NSString *)kCFBundleVersionKey;
    
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    
    NSMutableAttributedString *attrStr = [[NSMutableAttributedString alloc] initWithString:@"\n关于本软件\n欢迎您使用本软件及服务!\n软件版本号:   \n开发商：金智软件"];
    
    // setting all font
    NSRange allRange = NSMakeRange(0, attrStr.length);
    
    [attrStr addAttribute:NSForegroundColorAttributeName value:[UIColor whiteColor] range:allRange];
    
    [attrStr addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:13] range:allRange];
    
    // settiing version font
    NSRange range = NSMakeRange(26, version.length);
    
    [attrStr insertAttributedString:[[NSAttributedString alloc] initWithString:version] atIndex:range.location];

    [attrStr addAttribute:NSForegroundColorAttributeName value:COLOR_GOLD_THEME range:range];
    
    [attrStr addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:18] range:range];
    
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineHeightMultiple = 10.0f;
    paragraphStyle.maximumLineHeight = 10.0f;
    paragraphStyle.minimumLineHeight = 10.0f;
    paragraphStyle.lineSpacing = 25.0f;// 字体的行间距
    
    NSDictionary *attributes = @{NSParagraphStyleAttributeName:paragraphStyle
                                 };
   
    [attrStr addAttributes:attributes range:allRange];
    
    self.textView.attributedText = attrStr;//设置它显示的内容
    
    self.textView.returnKeyType = UIReturnKeyDefault;//返回键的类型
    
    self.textView.keyboardType = UIKeyboardTypeDefault;//键盘类型
    
    self.textView.scrollEnabled = YES;//是否可以拖动
    
    self.textView.autoresizingMask = UIViewAutoresizingFlexibleHeight;//自适应高度
    
    self.textView.userInteractionEnabled = NO;
    
    [self.view addSubview: self.textView];//加入到整个页面中
}

@end
