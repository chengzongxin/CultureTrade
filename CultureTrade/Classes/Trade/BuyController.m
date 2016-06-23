//
//  BuyController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/12/4.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "BuyController.h"
#import "UIBarButtonItem+helper.h"
#import "BaseTextField.h"
#import "BuyInCell.h"
#import "GlobalModel.h"
#import "SelectionCell.h"
#import "UIImage+helper.h"
#define kCellHeight 36
#define kTextFieldMargin 15
#define TextFiledX 10
#define TextFiledY 5
#define TextFiledW ScreenSize.width - 2 * TextFiledX
#define TextFiledH 35
@interface BuyController ()

@end

@implementation BuyController

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

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 1.添加UIScrollView
    [self addScrollView];
    
    [self addTitleView];
    
    [self addTextFied];
    
    [self addTableView];
    
    [self addButton];
    
    [self addDownList];
    
    [self fillData];
    
    [self requestPreTrade];
}

- (void)addTitleView
{
    UIImage *titleImage = [UIImage imageNamed:@"buyIn"];
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    self.navigationItem.titleView = titleView;
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
}

-(void) hideKeyboard {
    [_productID resignFirstResponder];
    [_productName resignFirstResponder];
    [_quoteType resignFirstResponder];
    [_buyInPrice resignFirstResponder];
    [_buyInNumber resignFirstResponder];
    
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)addTextFied
{
    _productID = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin, TextFiledW, TextFiledH)];
    _productID.delegate = self;
    _productID.leftLabel.text = LocalizedStringByInt(1801);
    _productID.font = FONT_BUYIN_TEXTFIELD;
    _productID.style = CombiInputTextList;
    [self.view addSubview:_productID];
    [_productID.downListButton addTarget:self action:@selector(changeOpenStatus:) forControlEvents:UIControlEventTouchUpInside];
    
    _productName = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + TextFiledH + TextFiledY, TextFiledW, TextFiledH)];
    _productName.delegate = self;
    _productName.leftLabel.text = LocalizedStringByInt(1802);
    _productName.font = FONT_BUYIN_TEXTFIELD;
    _productName.style = CombiInputTextList;
    [self.view addSubview:_productName];
    [_productName.downListButton addTarget:self action:@selector(changeOpenStatus:) forControlEvents:UIControlEventTouchUpInside];
    
    _buyInPrice = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*2, TextFiledW, TextFiledH)];
    _buyInPrice.delegate = self;
    _buyInPrice.leftLabel.text = LocalizedStringByInt(1804);
    _buyInPrice.font = FONT_BUYIN_TEXTFIELD;
    _buyInPrice.keyboardType = UIKeyboardTypeDecimalPad;
    [self.view addSubview:_buyInPrice];
    
    _buyInNumber = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*3, TextFiledW, TextFiledH)];
    _buyInNumber.delegate = self;
    _buyInNumber.leftLabel.text = LocalizedStringByInt(1805);
    _buyInNumber.font = FONT_BUYIN_TEXTFIELD;
    _buyInNumber.keyboardType = UIKeyboardTypeNumberPad;
    [self.view addSubview:_buyInNumber];
    
    _maxCanBuy = [[CombiInputText alloc] initWithFrame:CGRectMake(TextFiledX, kTextFieldMargin + (TextFiledH  + TextFiledY)*4, TextFiledW, TextFiledH)];
    _maxCanBuy.leftLabel.text = LocalizedStringByInt(1806);
    _maxCanBuy.font = FONT_BUYIN_TEXTFIELD;
    _maxCanBuy.userInteractionEnabled = NO;
    [self.view addSubview:_maxCanBuy];
}

- (void)addDownList
{
    index = 0;
    isOpened = NO;
    _tableBlock = [[TableViewWithBlock alloc] initWithFrame:CGRectMake(TextFiledX+100, CGRectGetMaxY(_productID.frame), TextFiledW-100, 0)];
    [_tableBlock initTableViewDataSourceAndDelegate:^NSInteger(UITableView *tableView, NSInteger section) {
        return GLOBAL.sortUnitArray.count;
    } setCellForIndexPathBlock:^UITableViewCell *(UITableView *tableView, NSIndexPath *indexPath) {
        if (index == 0) {
            SelectionCell *cell=[tableView dequeueReusableCellWithIdentifier:@"SelectionCell0"];
            if (!cell) {
                cell = [[SelectionCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"SelectionCell0"];
                [cell setSelectionStyle:UITableViewCellSelectionStyleGray];
            }
            SortUnit *sortUnit = GLOBAL.sortUnitArray[indexPath.row];
            [cell.lb setText:sortUnit.productID];
            return cell;
        }else{
            SelectionCell *cell=[tableView dequeueReusableCellWithIdentifier:@"SelectionCell1"];
            if (!cell) {
                cell = [[SelectionCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"SelectionCell1"];
                [cell setSelectionStyle:UITableViewCellSelectionStyleGray];
            }
            SortUnit *sortUnit = GLOBAL.sortUnitArray[indexPath.row];
            [cell.lb setText:sortUnit.productName];
            return cell;
        }
    } setDidSelectRowBlock:^(UITableView *tableView, NSIndexPath *indexPath) {
        SelectionCell *cell=(SelectionCell*)[tableView cellForRowAtIndexPath:indexPath];
        cell.lb.textColor = COLOR_TABLEBLOCK_CELL_SELECTED_TEXT;
        if (index == 0) {
            _productID.text=cell.lb.text;
            [_productID.downListButton sendActionsForControlEvents:UIControlEventTouchUpInside];
        }else{
            _productName.text=cell.lb.text;
            [_productName.downListButton sendActionsForControlEvents:UIControlEventTouchUpInside];
        }
        if ([NSTradeEngine sharedInstance].delegate != self)
        {
            [NSTradeEngine sharedInstance].delegate = self;
        }
        GLOBAL.sortUnit = GLOBAL.sortUnitArray[indexPath.row];
        [self requestPreTrade];
        [self fillData];
        [self loadData];
    }];
    
    [self.view addSubview:_tableBlock];
}

- (void)requestPreTrade
{
    [[NSTradeEngine sharedInstance] trade_request_pretrade:[NSString stringWithFormat:@"%d",GLOBAL.sortUnit.m_CodeInfo.m_uiCode] type:@"0B"];
}

- (void)changeOpenStatus:(id)sender {
    if (isOpened) {
        [UIView animateWithDuration:0.3 animations:^{
            UIImage *closeImage=[UIImage imageNamed:@"downList"];
            [_productID.downListButton setImage:closeImage forState:UIControlStateNormal];
            
            CGRect frame=_tableBlock.frame;
            
            frame.size.height=0;
            [_tableBlock setFrame:frame];
            
        } completion:^(BOOL finished){
            isOpened=NO;
        }];
    }else{
        [UIView animateWithDuration:0.3 animations:^{
            UIImage *openImage=[UIImage imageNamed:@"downList"];
            [_productName.downListButton setImage:openImage forState:UIControlStateNormal];
            
            CGRect frame=_tableBlock.frame;
            
            frame.size.height=300;
            [_tableBlock setFrame:frame];
        } completion:^(BOOL finished){
            isOpened=YES;
        }];
    }
    [_tableBlock reloadData];
}


- (void)fillData
{
    int code = GLOBAL.sortUnit.m_CodeInfo.m_uiCode;
    NSString *name = GLOBAL.sortUnit.productName;
    float preClose = [[NSTradeEngine sharedInstance] getPreClosePrice:GLOBAL.sortUnit.m_CodeInfo.m_uiCode];
    
    _productID.text = [NSString stringWithFormat:@"%d",GLOBAL.sortUnit.m_CodeInfo.m_uiCode];
    _productName.text = [NSString stringWithFormat:@"%@",GLOBAL.sortUnit.productName];
    _buyInPrice.text = [NSString stringWithFormat:@"%0.2f",preClose];
    
    if (code == 0) _productID.text = @"";
    if (name  == nil || name == 0) _productName.text = @"";
    if (preClose == 0) _buyInPrice.text = @"";
}

#pragma mark -
#pragma mark UITextFieldDelegate

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    if (textField == _productID) {
        [textField resignFirstResponder];
        // 改变位置
        CGRect frame = _tableBlock.frame;
        frame.origin.y = CGRectGetMaxY(_productID.frame);
        _tableBlock.frame = frame;
        
        index = 0;
        [self changeOpenStatus:_productID.downListButton];
    }
    
    if (textField == _productName) {
        [textField resignFirstResponder];
        // 改变位置
        CGRect frame = _tableBlock.frame;
        frame.origin.y = CGRectGetMaxY(_productName.frame);
        _tableBlock.frame = frame;
        
        index = 1;
        [self changeOpenStatus:_productName.downListButton];
    }
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [textField resignFirstResponder];
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    if (textField == _productID || textField == _productName ) { // 点击之后的文本框点击下拉列表，后不隐藏虚拟键盘bug
        [_buyInPrice resignFirstResponder];
        [_buyInNumber resignFirstResponder];
        [_maxCanBuy resignFirstResponder];
        return YES;
    }
    return [super textFieldShouldBeginEditing:textField];
}

- (void)clickDownListButton
{
    MYLOGFUN;
}

- (void)addTableView
{
    CGRect rect = CGRectMake(0, CGRectGetMaxY(_maxCanBuy.frame) + 15 ,self.view.frame.size.width, kCellHeight * 5);
    _tableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.rowHeight = SIZE_BUYIN_CELL_HEIGHT;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _tableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _tableView.backgroundColor = COLOR_BUYIN_TABLE_BG;
    _tableView.scrollEnabled = NO;
    [self.view addSubview:_tableView];
}


#pragma mark - UITableView Delegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return kCellHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 5;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"buyCellID";
    BuyInCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[BuyInCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    
    if (GLOBAL.volPriceArray.count < 10)
    {
        [GLOBAL.volPriceArray removeAllObjects];
        for (int i = 0; i < 10; i ++) {
            VolPriceNS *v = [[VolPriceNS alloc] init];
            v.m_uiPrice = 0;
            v.m_uiVolume = 0;
            [GLOBAL.volPriceArray addObject:v];
        }
    }
    VolPriceNS *sellVol = GLOBAL.volPriceArray[4 - indexPath.row];
    VolPriceNS *buyVol = GLOBAL.volPriceArray[indexPath.row + 5];
    cell.buy.text = [NSString stringWithFormat:@"买%d:",(int)(indexPath.row+1)];
    cell.buyPrice.text = [NSString stringWithFormat:@"%0.2f",(float)buyVol.m_uiPrice*0.001];
    cell.buyShouShu.text = [NSString stringWithFormat:@"%d",buyVol.m_uiVolume];
    
    cell.sell.text = [NSString stringWithFormat:@"卖%d:",(int)(indexPath.row+1)];
    cell.sellPrice.text = [NSString stringWithFormat:@"%0.2f",(float)sellVol.m_uiPrice*0.001];
    cell.sellShouShu.text = [NSString stringWithFormat:@"%d",sellVol.m_uiVolume];
    
    return cell;
    
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)viewDidLayoutSubviews
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([self.tableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [self.tableView setSeparatorInset:UIEdgeInsetsMake(0, 0, 0, 0)];
    }
    
    if ([self.tableView respondsToSelector:@selector(setLayoutMargins:)]) {
        [self.tableView setLayoutMargins:UIEdgeInsetsMake(0, 0, 0, 0)];
    }
}

-(void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([cell respondsToSelector:@selector(setSeparatorInset:)]) {
        [cell setSeparatorInset:UIEdgeInsetsZero];
    }
    
    if ([cell respondsToSelector:@selector(setLayoutMargins:)]) {
        [cell setLayoutMargins:UIEdgeInsetsZero];
    }
}


- (void)addButton
{
    CGSize size = [UIImage imageNamed:@"confirm"].size;
    CGFloat btnX = (self.view.frame.size.width - size.width * 2 - 10)/2;
    CGFloat btnY = CGRectGetMaxY(_tableView.frame)+30;
    
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

- (void)ClickClear
{
    [super ClickClear];
    [NSTradeEngine setup].delegate = nil;
    [GLOBAL.volPriceArray removeAllObjects];
    for (int i = 0; i < 10; i ++) {
        VolPriceNS *v = [[VolPriceNS alloc] init];
        v.m_uiPrice = 0;
        v.m_uiVolume = 0;
        [GLOBAL.volPriceArray addObject:v];
    }
    [_tableView reloadData];
}

- (void)confirmButtonClick
{
    [self textFieldDidEndEditing:_buyInPrice];
    [self textFieldDidEndEditing:_buyInNumber];
    
    if ([_productName.text containsString:@"指数"]) {
        showDetailAlert(@"下单失败", @"指数不可交易");
        return;
    }
    
    NSString *priceStr = _buyInPrice.text;
    NSString *shoushuStr = _buyInNumber.text;
    NSString *productID = _productID.text;
    double price = 0.0f;
    int shoushu = 0;
    
    if (!(priceStr.length && shoushuStr.length)) {// 请输入买入价格和买入数量
        showAlert(LocalizedStringByInt(1814));
        return;
    }
    
    if ([priceStr isPureNumandCharacters:priceStr] && [shoushuStr isPureNumandCharacters:shoushuStr]) { // 输入的是否为纯数字
        price = [priceStr doubleValue];
        shoushu = [shoushuStr intValue];
    }else{  //请输入纯数字
        showAlert(LocalizedStringByInt(1816));
        return;
    }
    
    if (!productID.length) {
        showAlert(LocalizedStringByInt(1817));
        return;
    }
    if ([NSTradeEngine sharedInstance].delegate != self)
    {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    [[NSTradeEngine sharedInstance] trade_open_market_order:productID direction:'b' price:price  shoushu:shoushu];
    [super showProgressByDelay:@"Loading..." delayInSeconds:10];
}


- (void)trade_ui_openorder_rsp:(int)nSeq nRet:(int)nRet orderticket:(NSString *)orderticket{
    
    [self stopTimer];
    [super dissmissProgress];
    if (nRet > 0) {
        BasicAlertView *alertView = [BasicAlertView alertViewWithTitle:@"下单成功"
                                                              subtitle:@""
                                                               message:nil
                                                               buttons:[NSArray arrayWithObject:@"confirm"]
                                                          afterDismiss:^(int buttonIndex) {
                                                              MYLog(@"%d",buttonIndex);
                                                              [self startTimer];
                                                          }];
        [alertView show];
    }else{
        BasicAlertView *alertView = [BasicAlertView alertViewWithTitle:LocalizedStringByInt(nRet)
                                                              subtitle:@""
                                                               message:nil
                                                               buttons:[NSArray arrayWithObject:@"confirm"]
                                                          afterDismiss:^(int buttonIndex) {
                                                              MYLog(@"%d",buttonIndex);
                                                              [self startTimer];
                                                          }];
        [alertView show];
    }
    
}

- (void)trade_ui_pretrade_rsp:(int)nSeq pretrade:(NSString *)pretrades
{
    NSData *data = [pretrades dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    _maxCanBuy.text = jsonDic[@"9801"];
}

#pragma mark touches event
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    NSLog(@"view touch began");
}
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    NSLog(@"view touch ended");
    [_buyInPrice resignFirstResponder];
    [_buyInNumber resignFirstResponder];
}
-(void)scrollViewTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event whichView:(id)scrollView{
    NSLog(@"scrollView  touch ended");
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
    [self startTimer];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self hideKeyboard];
    [self stopTimer];
}

- (void)startTimer // 30秒刷一次
{
    if (mTimer == nil) { // 30秒刷一次
        mTimer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:1.0f target:self selector:@selector(processEvent) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSDefaultRunLoopMode];
        
    }
}

- (void)stopTimer
{
    [mTimer invalidate];
    mTimer = nil;
    
}

- (void)processEvent
{
    NSThread* mThrad = [[NSThread alloc] initWithTarget:self selector:@selector(loadData) object:nil];
    [mThrad start];
}


- (void)loadData
{
    if (_productID.text.length > 0) [[NSTradeEngine sharedInstance] quote_stktrace_reqNS:_productID.text];
}

- (void)quote_ui_stkDealData_rsp:(NSString *)data
{
    MYLog(@"--------UI stkDealData--------- %@",data);
    mThread = [[NSThread alloc] initWithTarget:self selector:@selector(processDataOnBackground:) object:data];
    [mThread start];
    
}

- (void)processDataOnBackground:(NSString *)data
{
    GLOBAL.volPriceArray = [NSMutableArray array];
    NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[data componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    for (int i = 0; i < strDataArray.count; i++)
    {
        NSString *strLine = [strDataArray objectAtIndex:i];
        //判断是否有空置
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        if (filedArr.count == 2){  // 买1卖1，10个
            
            VolPriceNS *vol = [[VolPriceNS alloc] init];
            vol.m_uiPrice = [filedArr[0] intValue];
            vol.m_uiVolume = [filedArr[1] intValue];
            [GLOBAL.volPriceArray addObject:vol];
        }
    }
    [_tableView reloadData];
}

@end
