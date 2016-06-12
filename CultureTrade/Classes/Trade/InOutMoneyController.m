//
//  InOutMoneyController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "InOutMoneyController.h"
#import "UIBarButtonItem+helper.h"
#import "TableViewWithBlock.h"
#import "SelectionCell.h"
#define kCellHeight 36
#define kTextFieldMargin 15
#define kCellHeight 36
#define kTextFieldMargin 15
#define TextFiledX 10
#define TextFiledY 5
#define TextFiledW ScreenSize.width - 2 * TextFiledX
#define TextFiledH 35
typedef enum : NSUInteger {
    isInMoney = 1,
    isOutMoney = 2,
} inoutMoneyType;
@interface InOutMoneyController ()
{
    NSMutableArray *_transferMoneyTypeArr;
    NSMutableArray *_transferBankNameArr;
    inoutMoneyType _type;
    int index;
    BOOL isOpened;
    TableViewWithBlock *_tableBlock;
}
@end

@implementation InOutMoneyController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self initalization];
    // 1.添加UIScrollView
    [self addScrollView];
    
    [self addTitleView];
    
    [self addTextFied];
    
    [self addButton];
    
    [self initalization];
//    [self addPickView];
    
    [self addDownList];
}

#pragma mark 自定义view
- (void)loadView
{
    UIImageView *imageView = [[UIImageView alloc] init];
    //    imageView.image = [UIImage fullscrennImage:@"new_feature_background.png"];
    /*
     以3.5inch为例（320x480）
     1> 当没有状态栏，applicationFrame的值{{0, 0}, {320, 480}}
     2> 当有状态栏，applicationFrame的值{{0, 20}, {320, 460}}
     */
    imageView.frame = [UIScreen mainScreen].applicationFrame;
    // 跟用户进行交互（这样才可以接收触摸事件）
    imageView.userInteractionEnabled = YES;
    self.view = imageView;
}

#pragma mark - UI界面初始化
#pragma mark 添加滚动视图
- (void)addScrollView
{
    UITouchScrollView *scroll = [[UITouchScrollView alloc] init];
    scroll.frame = self.view.bounds;
    scroll.showsHorizontalScrollIndicator = NO; // 隐藏水平滚动条
    CGSize size = scroll.frame.size;
    scroll.contentSize = CGSizeMake(size.width, size.height*1.2); // 内容尺寸
    //    scroll.pagingEnabled = YES; // 分页
    scroll.delegate = self;
    [self.view addSubview:scroll];
    _scroll = scroll;
    self.view = _scroll;
    _scroll.delegate = self;
    self.view.backgroundColor = COLOR_VC_BG;
}

- (void)initalization
{
    _transferMoneyTypeArr = [NSMutableArray arrayWithObjects:@"银行转交易所",@"交易所转银行", nil];
    NSString *bankName = [[NSTradeEngine setup] getBankName];
    _transferBankNameArr = [NSMutableArray arrayWithObject:bankName];
}

- (void)addTitleView
{
    UIImage *titleImage = [UIImage imageNamed:@"transfer"];
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    self.navigationItem.titleView = titleView;
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)addTextFied
{
    _transferMoneyType = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin, TextFiledW, TextFiledH)];
    _transferMoneyType.leftLabel.text = LocalizedStringByInt(2002);
    _transferMoneyType.style = CombiInputTextList;
    _transferMoneyType.delegate = self;
    [self.view addSubview:_transferMoneyType];
    
    _transferBank = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + TextFiledH + TextFiledY, TextFiledW, TextFiledH)];
    _transferBank.leftLabel.text = LocalizedStringByInt(2003);
    _transferBank.delegate = self;
    _transferBank.style = CombiInputTextList;
    _transferBank.text = (NSString *)[_transferBankNameArr objectAtIndex:0];
    [self.view addSubview:_transferBank];
    
    _transferMoney = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*2, TextFiledW, TextFiledH)];
    _transferMoney.delegate = self;
    _transferMoney.leftLabel.text = LocalizedStringByInt(2004);
    _transferMoney.keyboardType = UIKeyboardTypeDecimalPad;
    [self.view addSubview:_transferMoney];
    
    _fundPassword = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*3, TextFiledW, TextFiledH)];
    _fundPassword.delegate = self;
    [_fundPassword setSecureTextEntry:YES];
    _fundPassword.leftLabel.text = LocalizedStringByInt(2005);
    [self.view addSubview:_fundPassword];
    
    _bankPassword = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*4, TextFiledW, TextFiledH)];
    _bankPassword.delegate = self;
    [_bankPassword setSecureTextEntry:YES];
    _bankPassword.leftLabel.text = LocalizedStringByInt(2006);
    [self.view addSubview:_bankPassword];
}

- (void)addPickView
{
    self.transferMoneyTypePickView = [[UIPickerView alloc] initWithFrame:CGRectMake(10, 300, self.view.frame.size.width - 10, 162)];
    self.transferMoneyTypePickView.backgroundColor = [UIColor whiteColor];
    self.transferMoneyTypePickView.delegate = self;
    self.transferMoneyTypePickView.dataSource = self;
    self.transferMoneyTypePickView.hidden = YES;
    [self.view addSubview:self.transferMoneyTypePickView];
    [self.transferMoneyTypePickView reloadAllComponents];
    
    self.transferBankPickView = [[UIPickerView alloc] initWithFrame:CGRectMake(10, 300, self.view.frame.size.width - 10, 162)];
    self.transferBankPickView.backgroundColor = [UIColor whiteColor];
    self.transferBankPickView.delegate = self;
    self.transferBankPickView.dataSource = self;
    self.transferBankPickView.hidden = YES;
    [self.view addSubview:self.transferBankPickView];
    [self.transferBankPickView reloadAllComponents];
}

- (void)addButton
{
    CGSize size = [UIImage imageNamed:@"confirm"].size;
    CGFloat btnX = (self.view.frame.size.width - size.width * 2 - 10)/2;
    CGFloat btnY = CGRectGetMaxY(_bankPassword.frame)+30;
    
    CGRect clearRect = (CGRect){{btnX,btnY},size};
    UIButton *clearBtn =  [self addButtonWithRect:clearRect Img:@"clear"];
    [clearBtn addTarget:self action:@selector(ClickClear) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:clearBtn];
    
    CGRect confirmRect = (CGRect){{CGRectGetMaxX(clearBtn.frame)+10,btnY},size};
    UIButton *confirmBtn =  [self addButtonWithRect:confirmRect Img:@"confirm"];
    [confirmBtn addTarget:self action:@selector(confirmButtonClick) forControlEvents:UIControlEventTouchUpInside];
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



- (void)confirmButtonClick
{
    if (_transferMoneyType.text.length*_transferBank.text.length == 0) {
        showAlert(@"请输入完整信息");
        return;
    }
    
    for (int i = 0 ; i < _transferMoneyTypeArr.count; i++) {
        NSString *str = _transferMoneyTypeArr[i];
        if ([str isEqualToString:_transferMoneyType.text]) {
            if (i == 0) {
                _type = isInMoney;
            }else{
                _type = isOutMoney;
            }
        }
    }
    
    if (![_transferMoney.text isPureNumandCharacters:_transferMoney.text]) { // 输入的是否为纯数字
            showAlert(LocalizedStringByInt(1816));
            return;
        }
    [NSTradeEngine sharedInstance].delegate = self;
    [[NSTradeEngine setup] trade_inout_moneny_type:_type money:[_transferMoney.text doubleValue] moneyPwd:_fundPassword.text bankPwd:_bankPassword.text];
    [super showProgressByDelay:@"提交中..." delayInSeconds:10];
}

- (void)on_inout_money_rsp:(int)nSeq nRet:(int)nRet data:(NSString *)data
{
    if (nRet == 0) {
        [super closeProgressSuccess:@"已提交"];
    }else{
        [super closeProgress:LocalizedStringByInt(nRet)];
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    NSLog(@"view touch ended");
    [_transferMoneyType resignFirstResponder];
    [_transferBank resignFirstResponder];
    [_transferMoney resignFirstResponder];
    [_fundPassword resignFirstResponder];
    [_bankPassword resignFirstResponder];
}

/*
- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    if (textField == _transferMoneyType) {
        [textField endEditing:YES];
        [_transferMoneyTypePickView reloadAllComponents];
        _transferMoneyTypePickView.hidden = NO;
    }else if (textField == _transferBank){
        [textField endEditing:YES];
        [_transferBankPickView reloadAllComponents];
        _transferBankPickView.hidden = NO;
    }else {
        _transferMoneyTypePickView.hidden = YES;
        _transferBankPickView.hidden = YES;
    }
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{}
*/
#pragma mark -
#pragma mark pickView 代理
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    if (pickerView == _transferMoneyTypePickView) {
        return _transferMoneyTypeArr.count;
    }else if (pickerView == _transferBankPickView){
        return _transferBankNameArr.count;
    }else {
        return 0;
    }
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    return 20;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
    return self.view.frame.size.width - 20;
}

- (nullable NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    if (pickerView.tag == 0) {
        return _transferMoneyTypeArr[row];
    }else if (pickerView.tag == 1){
        return _transferBankNameArr[row];
    }else {
        return NULL;
    }
}
/*
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    MYLOGFUN;
    MYLog(@"%ld,%ld",(long)row,(long)component);
    if (pickerView == _transferMoneyTypePickView) {
        if (row == 0) {
            _type = isInMoney;
            _bankPassword.hidden = NO;
        }else{
            _bankPassword.hidden = YES;
            _type = isOutMoney;
        }
        _transferMoneyType.text = _transferMoneyTypeArr[row];
    }else if (pickerView.tag == 1){
        _transferBank.text =  _transferBankNameArr[row];
    }else {
    }
    pickerView.hidden = YES;
}
*/
- (void)addDownList
{
    index = 0;
    isOpened = NO;
    _tableBlock = [[TableViewWithBlock alloc] initWithFrame:CGRectMake(TextFiledX+100, CGRectGetMaxY(_transferMoneyType.frame), TextFiledW-100, 0)];
    [_tableBlock initTableViewDataSourceAndDelegate:^NSInteger(UITableView *tableView, NSInteger section) {
        if (index == 0) {
            return _transferMoneyTypeArr.count;
        }else{
            return _transferBankNameArr.count;
        }
    } setCellForIndexPathBlock:^UITableViewCell *(UITableView *tableView, NSIndexPath *indexPath) {
        if (index == 0) {
            SelectionCell *cell=[tableView dequeueReusableCellWithIdentifier:@"SelectionCell0"];
            if (!cell) {
                cell = [[SelectionCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"SelectionCell0"];
                [cell setSelectionStyle:UITableViewCellSelectionStyleGray];
            }
            
            NSString *str = _transferMoneyTypeArr[indexPath.row];
            [cell.lb setText:str];
            return cell;
        }else{
            SelectionCell *cell=[tableView dequeueReusableCellWithIdentifier:@"SelectionCell1"];
            if (!cell) {
                cell = [[SelectionCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"SelectionCell1"];
                [cell setSelectionStyle:UITableViewCellSelectionStyleGray];
            }
            
            NSString *str = _transferBankNameArr[indexPath.row];
            [cell.lb setText:str];
            return cell;
        }
    } setDidSelectRowBlock:^(UITableView *tableView, NSIndexPath *indexPath) {
        SelectionCell *cell=(SelectionCell*)[tableView cellForRowAtIndexPath:indexPath];
        if (index == 0) {
            _transferMoneyType.text=cell.lb.text;
            _bankPassword.hidden = indexPath.row;
        }else{
            _transferBank.text=cell.lb.text;
        }
        [self changeOpenStatus:nil];
    }];
    
    [self.view addSubview:_tableBlock];
}

- (void)changeOpenStatus:(id)sender {
    if (isOpened) {
        [UIView animateWithDuration:0.3 animations:^{
            UIImage *closeImage=[UIImage imageNamed:@"downList"];
            [_transferMoneyType.downListButton setImage:closeImage forState:UIControlStateNormal];
            
            CGRect frame=_tableBlock.frame;
            
            frame.size.height=0;
            [_tableBlock setFrame:frame];
            
        } completion:^(BOOL finished){
            isOpened=NO;
        }];
    }else{
        [UIView animateWithDuration:0.3 animations:^{
            UIImage *openImage=[UIImage imageNamed:@"downList"];
            [_transferMoneyType.downListButton setImage:openImage forState:UIControlStateNormal];
            
            CGRect frame=_tableBlock.frame;
            
            frame.size.height=90;
            [_tableBlock setFrame:frame];
        } completion:^(BOOL finished){
            isOpened=YES;
        }];
    }
    [_tableBlock reloadData];
}

#pragma mark -
#pragma mark UITextFieldDelegate

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    if (textField == _transferMoneyType) {
        [textField resignFirstResponder];
        // 改变位置
        CGRect frame = _tableBlock.frame;
        frame.origin.y = CGRectGetMaxY(_transferMoneyType.frame);
        _tableBlock.frame = frame;
        
        index = 0;
        [self changeOpenStatus:_transferMoneyType.downListButton];
    }
    
    if (textField == _transferBank) {
        [textField resignFirstResponder];
        // 改变位置
        CGRect frame = _tableBlock.frame;
        frame.origin.y = CGRectGetMaxY(_transferBank.frame);
        _tableBlock.frame = frame;
        
        index = 1;
        [self changeOpenStatus:_transferBank.downListButton];
    }
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [textField resignFirstResponder];
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    if (textField == _transferMoneyType || textField == _transferBank ) { // 点击之后的文本框点击下拉列表，后不隐藏虚拟键盘bug
        [_transferMoney resignFirstResponder];
        [_fundPassword resignFirstResponder];
        [_bankPassword resignFirstResponder];
        return YES;
    }
    return [super textFieldShouldBeginEditing:textField];
}
@end
