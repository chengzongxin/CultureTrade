//
//  storeUIView.m
//  qwlwStore
//
//  Created by zengxw on 13-5-22.
//  Copyright (c) 2013年 zengxw. All rights reserved.
//

#import "storeUIView.h"

@implementation storeUIView

@synthesize delegate = _delegate;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        imgView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        [self addSubview:imgView];
        if (frame.size.width > 768.0) {
            [self setHengImageBgView];
        }
        else{
            [self setShuImageBgView];
        }
    }
    return self;
}

-(void)setHengImageBgView
{
    //imgView.image = [UIImage imageNamed:@"IPAD-heng.png"];
}
-(void)setShuImageBgView
{
    //imgView.image = [UIImage imageNamed:@"IPAD-shu.png"];
}

-(void)resetStoreViewFrame:(CGRect)frame
{
    self.frame = frame;
    imgView.frame = CGRectMake(0, 0, frame.size.width, frame.size.height);
}
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *result = [super hitTest:point withEvent:event];
    [self endEditing:YES];
    
    if (self.delegate != nil
        && [self.delegate respondsToSelector:@selector(didHitTest:)]) {
        [self.delegate didHitTest:point];
    }
    
    return result;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"begin");
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.delegate != nil) {
        [self.delegate didTounchEndView];
    }
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
