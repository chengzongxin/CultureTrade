//
//  ApplyPurchaseController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/13.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyPurchaseController.h"
#import "LeftPurchaseBanner.h"
#import "PurchaseBanner.h"
#import "LeftPurchaseCell.h"
#import "PurchaseCell.h"
#import "GlobalModel.h"
#import "ApplyPurchaseTradeController.h"
#define kLeftTableWith 70
#define kMainTableViewWithRatio 2
@interface ApplyPurchaseController () 
{
    UITableView *_leftTable;
    UITableView *_mainTable;
}
@end

@implementation ApplyPurchaseController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [NSTradeEngine sharedInstance].delegate = self;
    [[NSTradeEngine sharedInstance] trade_request_symbol];
    
    self.title = @"申购";
    
    [self addTableView];
}

- (void)addTableView
{
    CGRect leftRect = CGRectMake(0, 0, kLeftTableWith, self.view.frame.size.height - 64);
    UITableView *leftTable = [[UITableView alloc] initWithFrame:leftRect style:UITableViewStylePlain];
    leftTable.delegate = self;
    leftTable.dataSource = self;
    leftTable.bounces=NO;
    leftTable.rowHeight = KQuotaRowHeight;
    leftTable.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    leftTable.separatorColor = COLOR_CELL_SEPLITELINE;
    leftTable.backgroundColor = COLOR_QUOTA_TABLE_BG;
    [leftTable setShowsVerticalScrollIndicator:NO];
    [leftTable setShowsHorizontalScrollIndicator:NO];
    leftTable.tag = 0;
    _leftTable = leftTable;
    
    UIScrollView *leftScrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, kLeftTableWith, self.view.frame.size.height -64)];
    [leftScrollView addSubview:leftTable];
    leftScrollView.bounces=NO;
    leftScrollView.contentSize=CGSizeMake(kLeftTableWith,0);
    [self.view addSubview:leftScrollView];
    
    CGRect rect = CGRectMake(0, 0 ,self.view.frame.size.width*kMainTableViewWithRatio, self.view.frame.size.height -64);
    UITableView *mainTable = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    mainTable.delegate = self;
    mainTable.dataSource = self;
    mainTable.bounces=NO;
    mainTable.rowHeight = KQuotaRowHeight;
    mainTable.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    mainTable.separatorColor = COLOR_CELL_SEPLITELINE;
    mainTable.backgroundColor = COLOR_QUOTA_TABLE_BG;
    mainTable.separatorInset = UIEdgeInsetsMake(0, -2000, 0, -2000);
    mainTable.tag = 1;
    _mainTable = mainTable;
    
    UIScrollView *scrollView=[[UIScrollView alloc]initWithFrame:CGRectMake(kLeftTableWith, 0, self.view.frame.size.width-kLeftTableWith, self.view.frame.size.height -64)];
    [scrollView addSubview:mainTable];
    scrollView.bounces=NO;
    scrollView.contentSize=CGSizeMake(self.view.frame.size.width*kMainTableViewWithRatio,0);
    [self.view addSubview:scrollView];
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY= _mainTable.contentOffset.y;
    CGPoint leftOffsetY = _leftTable.contentOffset;
    leftOffsetY.y=offsetY;
    _leftTable.contentOffset=leftOffsetY;
    if(offsetY==0){
        _leftTable.contentOffset=CGPointZero;
    }
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    if (tableView == _leftTable) {
        return [[LeftPurchaseBanner alloc] initWithFrame:CGRectMake(0, 0, kLeftTableWith, kQuotaBannerHeight)];
    }else if (tableView == _mainTable){
        PurchaseBanner *mainBanner = [[PurchaseBanner alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width*kMainTableViewWithRatio, kQuotaBannerHeight)];
        mainBanner.count = 7;
        return mainBanner;
    }else{
        return NULL;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    SymbolModel *symbol = GLOBAL.symbolArray[indexPath.row];
    if (tableView.tag == 0) {
        static NSString *cellID = @"LeftPurchaseCell";
        LeftPurchaseCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[LeftPurchaseCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithSymbol:symbol];
    }else{
        static NSString *cellID = @"PurchaseCell";
        PurchaseCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[PurchaseCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        cell.labelCount = 7;
        return [cell initWithSymbol:symbol];
    }
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return kQuotaBannerHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return GLOBAL.symbolArray.count;
}

// 取消选中,选择cell
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    GLOBAL.symbolModel = GLOBAL.symbolArray[indexPath.row];
//    for (StockInfoNS *stockInfo in GLOBAL.stockInfoArray) { // 获取缓存initmarket中对应的stockinfo对象
//        if (stockInfo.m_uiCode == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
//            GLOBAL.stockInfo = stockInfo;
//            GLOBAL.sortUnit.m_uiPreClose = stockInfo.m_uiPrevClose;
//        }
//    }
//    for (SymbolModel *symbol in GLOBAL.symbolArray) {
//        if ([symbol.productID intValue] == GLOBAL.sortUnit.m_CodeInfo.m_uiCode ) {
//            GLOBAL.symbolModel = symbol;
//        }
//    }
        [_leftTable deselectRowAtIndexPath:indexPath animated:YES];
        [_mainTable deselectRowAtIndexPath:indexPath animated:YES];
    
    
//    [self tableView:self.leftTableView shouldHighlightRowAtIndexPath:indexPath];
    [self.navigationController pushViewController:[[ApplyPurchaseTradeController alloc] init] animated:NO];
}

- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}


- (void)trade_ui_symbol_change_notice:(int)nRet symbolcodes:(NSString *)symbolcodes
{
    MYLOGFUN;
    MYLog(@"%@",symbolcodes);
}

- (void)viewDidLayoutSubviews
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([_mainTable respondsToSelector:@selector(setSeparatorInset:)]) {
        [_mainTable setSeparatorInset:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_mainTable respondsToSelector:@selector(setLayoutMargins:)]) {
        [_mainTable setLayoutMargins:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_leftTable respondsToSelector:@selector(setSeparatorInset:)]) {
        [_leftTable setSeparatorInset:UIEdgeInsetsMake(0, _leftTable.frame.size.width*-3, 0, 0)];
    }
    
    if ([_leftTable respondsToSelector:@selector(setLayoutMargins:)]) {
        [_leftTable setLayoutMargins:UIEdgeInsetsMake(0, _leftTable.frame.size.width*-2, 0, 0)];
    }
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //设置TableView Separatorinset 分割线从边框顶端开始
    if ([cell respondsToSelector:@selector(setSeparatorInset:)]) {
        [cell setSeparatorInset:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-2, 0, 0)];
    }
    
    if ([cell respondsToSelector:@selector(setLayoutMargins:)]) {
        [cell setLayoutMargins:UIEdgeInsetsMake(0, _mainTable.frame.size.width*-2, 0, 0)];
    }
}

@end
