//
//  SelectController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/23.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "SelectController.h"
#import "SelectCell.h"
#import "QuotaAnalysisController.h"
#import "QuotaionBanner.h"
#import "UIBarButtonItem+helper.h"
#import "NSTradeEngine.h"
#import "GlobalModel.h"
#import "SelectPrefer.h"

@interface SelectController ()<UITableViewDelegate,UITableViewDataSource,NSTradeEngineDelegate>
{
    NSMutableArray *_symbolArray;
    NSMutableArray *_sortUnitArray;
//    NSThread* mThrad;
    NSTimer *mTimer;
}
@end

@implementation SelectController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = COLOR_QUOTA_TABLE_BG;
    
    // 设置行情中心标题
    UIImage *titleImage = [UIImage imageNamed:@"title_select"];
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    self.navigationItem.titleView = titleView;
    
    [self addBanner];
    
    [self addTableView];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NSTradeEngine sharedInstance].delegate = self;
    [[NSTradeEngine sharedInstance] trade_request_symbol];
    
    if (GLOBAL.sortUnitArray != NULL) {
       _sortUnitArray = [self filterPerferIn:GLOBAL.sortUnitArray];
        [_tableView reloadData];
    }
    
    [self startTimer];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [NSTradeEngine sharedInstance].delegate = NULL;
    [self stopTimer];
}

- (void)startTimer
{
    if (mTimer == nil) { // 30秒刷一次
        mTimer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:10.0f target:self selector:@selector(loadProductListAndPankouData) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSDefaultRunLoopMode];
        
    }
}

- (void)stopTimer
{
    [mTimer invalidate];
    mTimer = nil;
    
}
- (void)loadProductListAndPankouData
{
    MYLOGFUN;
    [[NSTradeEngine sharedInstance] trade_request_symbol];
}


- (void)addBanner
{
    _banner = [[QuotaionBanner alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, kQuotaBannerHeight)];
    [self.view addSubview:_banner];
}

- (void)addTableView
{
    CGRect rect = CGRectMake(0, kQuotaBannerHeight ,self.view.frame.size.width, self.view.frame.size.height-2*64-10);
    _tableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.rowHeight = KQuotaRowHeight;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _tableView.separatorColor = COLOR_CELL_SEPLITELINE;
    _tableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    [self.view addSubview:_tableView];
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _sortUnitArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"selectcell";
    SelectCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[SelectCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
    }
    
    return [cell initWithSortUnit:(SortUnit *)[_sortUnitArray objectAtIndex:indexPath.row]];
}

// 取消选中,选择cell
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    GLOBAL.sortUnit = _sortUnitArray[indexPath.row];
    for (StockInfoNS *stockInfo in GLOBAL.stockInfoArray) { // 获取缓存initmarket中对应的stockinfo对象
        if (stockInfo.m_uiCode == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
            GLOBAL.stockInfo = stockInfo;
            GLOBAL.sortUnit.m_uiPreClose = stockInfo.m_uiPrevClose;
        }
    }
    for (SymbolModel *symbol in GLOBAL.symbolArray) {
        if ([symbol.productID intValue] == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
            GLOBAL.symbolModel = symbol;
        }
    }
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    [self presentViewController:[[QuotaAnalysisController alloc] init] animated:NO completion:nil];
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
/*
- (void)trade_ui_symbol_change_notice:(int)nRet symbolcodes:(NSString *)symbolcodes
{
    MYLOGFUN;
    NSData *data = [symbolcodes dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
    NSArray *array = [jsonDic objectForKey:@"SYMBOLS"];
    _symbolArray = [NSMutableArray array];
    for (NSDictionary *dict in array) {
        SymbolModel *s = [SymbolModel symbolWithDictionary:dict];
        NSDictionary *localDict = [SelectPrefer readFromFile];
        if (localDict == nil) { // 没有自选
            return;
        }else{      // 有自选
            if ([[localDict objectForKey:s.productID] boolValue])  [_symbolArray addObject:s];
    
        }
    }
    
    [self requestPankouData];
}

- (void)requestPankouData
{
    
    NSString *products = [NSString string];
    for (SymbolModel *s in _symbolArray) {
        if ([products  isEqual: @""]) {
            products = s.productID;
        }else{
            products = [NSString stringWithFormat:@"%@,%@",products,s.productID];
        }
    }
    
    [[NSTradeEngine sharedInstance] sendReportData:products];
    
}

- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len
{
    MYLog(@"盘口商品列表:[%d]",len);
    _sortUnitArray = [NSMutableArray array];
    NSArray *strArray = [data componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
    for (int i = 0; i<strArray.count; i++) {
        NSString *strLine = strArray[i];
        NSArray *fieldArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        SortUnit *sortUnit = [SortUnit sortUnitWithArray:fieldArr];
        if (sortUnit.m_CodeInfo.m_uiCode < 0) break;
        for (SymbolModel *symbol in _symbolArray) {
            if ([symbol.productID isEqualToString:[NSString stringWithFormat:@"%d",sortUnit.m_CodeInfo.m_uiCode]]) {
                sortUnit.productName = symbol.productName;
            }
        }
        for (StockInfoNS *s in GLOBAL.stockInfoArray) {
            if (s.m_uiCode == sortUnit.m_CodeInfo.m_uiCode) {
                sortUnit.m_uiPreClose = s.m_uiPrevClose;
            }
        }
        
        [_sortUnitArray addObject:sortUnit];
    }
    [_tableView reloadData];
}
*/

- (void)quote_ui_pankou_rsp:(NSString *)data len:(int)len
{
    _sortUnitArray = [self filterPerferIn:GLOBAL.sortUnitArray];
    [_tableView reloadData];
}

- (NSMutableArray *)filterPerferIn:(NSMutableArray *)sortUnitArray
{
    NSMutableArray *array = [NSMutableArray array];
    for (SortUnit *s in sortUnitArray) {
        NSDictionary *localDict = [SelectPrefer readFromFile];
        if (localDict == nil) { // 没有自选
            return sortUnitArray;
        }else{                  // 有自选
            NSString *key = [NSString stringWithFormat:@"%d",s.m_CodeInfo.m_uiCode];
            if ([[localDict objectForKey:key] boolValue])  [array addObject:s];
        }
    }
    return array;
}

@end
