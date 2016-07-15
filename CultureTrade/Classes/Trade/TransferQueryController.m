//
//  TransferQueryController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/16.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TransferQueryController.h"
#import "TransferQueryCell.h"
#import "UIBarButtonItem+helper.h"
#define kBannerHeight 20
#define kCellHeight 35
@interface TransferQueryController ()
{
    NSMutableArray *_transferArray;
}
@end

@implementation TransferQueryController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addTitleBanner];
    
    [self addTableView];
}

- (void)addTitleBanner
{
    self.title = LocalizedStringByInt(2400);
    
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
    
    _titleBanner = [[TitleBanner alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, kBannerHeight)];
    _titleBanner.bannerTitles = @[@"类型",@"时间",@"金额",@"状态"];
    [self.view addSubview:_titleBanner];
}

- (void)addTableView
{
    CGRect rect = CGRectMake(0, CGRectGetMaxY(_titleBanner.frame) + 5,self.view.frame.size.width, self.view.frame.size.height - 20 - 5  - kBannerHeight - 64);
    _tableView = [[UITableView alloc] initWithFrame:rect style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    _tableView.separatorColor = COLOR_TRANSFER_QUERY_CELL_SEPLITELINE;
    _tableView.backgroundColor = COLOR_TRANSFER_QUERY_TABLE_BG;
    _tableView.bounces = YES;
    [self.view addSubview:_tableView];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    _transferArray = [NSMutableArray array];
    
    [NSTradeEngine sharedInstance].delegate = self;
    [[NSTradeEngine sharedInstance] trade_request_transferquery];
}

- (void)trade_ui_transferquery_rsp:(int)nSeq results:(NSString *)results
{
    if (results == nil) return;
//    [self closeProgressSuccess:@"Success!"];
    _transferArray = [NSMutableArray array];
    NSError *error;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[results dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    NSArray *jsonArray = [str2dict objectForKey:@"INTPRTCL"];
    for (NSDictionary *dict in jsonArray) {
        TransferQuery *transferQuery = [TransferQuery transferQueryWithDictionary:dict];
        [_transferArray addObject:transferQuery];
    }
    [_tableView reloadData];
}
#pragma mark - UITableView Delegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return kCellHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _transferArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"TransferQueryID";
    TransferQueryCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[TransferQueryCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellID];
    }
    
    TransferQuery *transferQuery = _transferArray[indexPath.row];
    // type
    cell.stockName.text = transferQuery.operatorType;
    if ([transferQuery.operatorType isEqualToString:@"1"]) {
        cell.stockName.text = @"入金";
    }else{
        cell.stockName.text = @"出金";
    }
    // time
    NSArray *filedArr = [transferQuery.time componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@" "]];
    if (filedArr.count == 2) cell.entrustPrice.text = [NSString stringWithFormat:@"%@\n%@",filedArr[0],filedArr[1]];
    // money
    cell.entrustDeal.text = [NSString changePriceByFloat:[transferQuery.money doubleValue]];
    
    
    // status 
    if ([transferQuery.performState isEqualToString:@"2"]){
        cell.status.text = @"系统拒绝";
    }else if ([transferQuery.performState isEqualToString:@"1"]){
        cell.status.text = @"支付完成";
    }else if ([transferQuery.performState isEqualToString:@"0"]){
        cell.status.text = @"已受理";
    }
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
