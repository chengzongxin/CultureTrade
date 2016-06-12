//
//  BaseTextField.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/4.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BaseTextField.h"

@implementation BaseTextField

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self settingTextField];
    return self;
}

- (void)settingTextField
{
    [self setTextColor:[UIColor whiteColor]];
    [self setAutocorrectionType: UITextAutocorrectionTypeNo]; // 关闭系统词汇联想
    [self setClearButtonMode:UITextFieldViewModeWhileEditing];//编辑时会出现个修改X
    [self setAutocapitalizationType:UITextAutocapitalizationTypeNone]; //关闭首字母大写
    [self setReturnKeyType:UIReturnKeyNext];//键盘下一步Next
    [self setHighlighted:YES];
    [self setBackground:[UIImage imageNamed:@"text_bg"]];
    //    [_account setValue:[UIColor colorWithHexString:@"#BC9842" withAlpha:1] forKeyPath:@"_placeholderLabel.textColor"];
    //    [_account setValue:[UIFont systemFontOfSize:10] forKeyPath:@"_placeholderLabel.font"];
}

////控制清除按钮的位置
//-(CGRect)clearButtonRectForBounds:(CGRect)bounds
//{
//    return CGRectMake(bounds.origin.x + bounds.size.width - 50, bounds.origin.y + bounds.size.height -20, 16, 16);
//}

//控制placeHolder的位置，左右缩20
-(CGRect)placeholderRectForBounds:(CGRect)bounds
{
    
    //    return CGRectInset(bounds, 20, 0);
    CGRect inset = CGRectMake(bounds.origin.x+30, bounds.origin.y + 8, bounds.size.width -10, bounds.size.height);//更好理解些
    return inset;
}
//控制显示文本的位置
-(CGRect)textRectForBounds:(CGRect)bounds
{
    //return CGRectInset(bounds, 50, 0);
    CGRect inset = CGRectMake(bounds.origin.x+30, bounds.origin.y, bounds.size.width -10, bounds.size.height);//更好理解些
    
    return inset;
    
}
//控制编辑文本的位置
-(CGRect)editingRectForBounds:(CGRect)bounds
{
    //return CGRectInset( bounds, 10 , 0 );
    
    CGRect inset = CGRectMake(bounds.origin.x +10, bounds.origin.y, bounds.size.width -10, bounds.size.height);
    return inset;
}
//控制左视图位置
- (CGRect)leftViewRectForBounds:(CGRect)bounds
{
    CGRect inset = CGRectMake(bounds.origin.x +10, bounds.origin.y, bounds.size.width-250, bounds.size.height);
    return inset;
    //return CGRectInset(bounds,50,0);
}

//控制placeHolder的颜色、字体
- (void)drawPlaceholderInRect:(CGRect)rect
{
    //CGContextRef context = UIGraphicsGetCurrentContext();
    //CGContextSetFillColorWithColor(context, [UIColor yellowColor].CGColor);
    //    [[UIColor colorWithHexString:@"#BC9842" withAlpha:1] setFill];
    
    //    [[self placeholder] drawInRect:rect withFont:[UIFont systemFontOfSize:10]];
    [[self placeholder] drawInRect:rect withAttributes:@{NSFontAttributeName : FONT_LOGIN_PLACEHOLDER_TEXT,
                                                         NSForegroundColorAttributeName : COLOR_LOGIN_PLACEHOLDER
                                                         }];
}

@end
