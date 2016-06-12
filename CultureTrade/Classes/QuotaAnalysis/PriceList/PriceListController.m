//
//  PriceListController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "PriceListController.h"
#import "PriceListCell.h"

@interface PriceListController () <UITableViewDataSource,UITableViewDelegate>

@end

@implementation PriceListController

- (void)viewDidLoad {
    [super viewDidLoad];
   
    UIImage *titleImage = [UIImage imageNamed:@"quotaAnalyze"];
    UIView *titleView = [[UIView alloc] initWithFrame:(CGRect){CGPointZero,titleImage.size}];
    titleView.backgroundColor = [UIColor colorWithPatternImage:titleImage];
    self.navigationItem.titleView = titleView;
    
    [self addBanner];
    
    [self addTable];
}

- (void)viewWillAppear:(BOOL)animated
{
    [self.view insertSubview:self.QuotaAnalysisController.guideView aboveSubview:self.view];
    [self.view insertSubview:self.QuotaAnalysisController.buttonListView aboveSubview:self.view];
}

- (void)addBanner
{
    CGFloat x = 0;
    CGFloat y = kQuotaGuideViewHeight + 5;
    CGFloat width = self.view.frame.size.width / 4;
    CGFloat height = 20;
    
    
    CGRect rect1 = CGRectMake(x, y, width, height);
    UILabel *price = [self settingLableWithFrame:rect1 Title:LocalizedStringByInt(1701)];
    [self.view addSubview:price];
    
    CGRect rect2 = CGRectMake(width, y, width, height);
    UILabel *dealPrice = [self settingLableWithFrame:rect2 Title:LocalizedStringByInt(1702)];
    [self.view addSubview:dealPrice];
    
    CGRect rect3 = CGRectMake(width * 2, y, width, height);
    UILabel *ratio = [self settingLableWithFrame:rect3 Title:LocalizedStringByInt(1703)];
    ratio.textAlignment = NSTextAlignmentJustified;
    [self.view addSubview:ratio];
    
    CGRect rect4 = CGRectMake(width * 3, y, width, height);
    UILabel *buyRatio = [self settingLableWithFrame:rect4 Title:LocalizedStringByInt(1704)];
    [self.view addSubview:buyRatio];
}

- (UILabel *)settingLableWithFrame:(CGRect)rect Title:(NSString *)title
{
    UILabel *label = [[UILabel alloc] initWithFrame:rect];
    label.text = title;
    label.textAlignment = NSTextAlignmentCenter;
    label.textColor = COLOR_GOLD_THEME;
    label.font = [UIFont systemFontOfSize:12];
    return label;
}

- (void)addTable
{
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, kQuotaGuideViewHeight + 30, self.view.frame.size.width, self.view.frame.size.height - kQuotaGuideViewHeight + 30) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.backgroundColor = COLOR(10, 10, 10, 1);
    _tableView.rowHeight = 28;
    _tableView.separatorColor = [UIColor blackColor];
    [self.view addSubview:_tableView];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 10;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentifer = @"dealDetail";
    PriceListCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifer];
    if (cell == nil) {
        cell = [[PriceListCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifer];
    }
    cell.price.text = @"8.30";
    cell.dealPrice.text = @"1034";
    
    CGFloat width = (indexPath.row + 5 )* 5;
    
    CGRect rect = cell.ratio.frame;
    cell.ratio.frame = CGRectMake(rect.origin.x, rect.origin.y, width , rect.size.height);
    
    cell.buyRatio.text = @"99.25";
    
    return cell;
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


@end
