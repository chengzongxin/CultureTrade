//
//  DealDetailController.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/30.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "DealDetailController.h"
#import "GuideView.h"
#import "DealDetailCell.h"

@interface DealDetailController () <UITableViewDataSource,UITableViewDelegate>
@end

@implementation DealDetailController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSMutableArray *array = [NSMutableArray arrayWithObjects:@"123",@"456",@"789", nil];
    [array removeObjectAtIndex:0];
    MYLog(@"%@",array[0]);
    
    
    self.title = LocalizedStringByInt(1501);
    
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
    UILabel *fenbi = [self settingLableWithFrame:rect1 Title:LocalizedStringByInt(1601)];
    [self.view addSubview:fenbi];
    
    CGRect rect2 = CGRectMake(width, y, width, height);
    UILabel *price = [self settingLableWithFrame:rect2 Title:LocalizedStringByInt(1602)];
    [self.view addSubview:price];
    
    CGFloat widthSmall = self.view.frame.size.width / 6;
    CGRect rect3 = CGRectMake(width * 2, y, widthSmall, height);
    UILabel *xianshou = [self settingLableWithFrame:rect3 Title:LocalizedStringByInt(1603)];
    [self.view addSubview:xianshou];
    
    CGRect rect4 = CGRectMake(CGRectGetMaxX(xianshou.frame), y, widthSmall, height);
    UILabel *open = [self settingLableWithFrame:rect4 Title:LocalizedStringByInt(1604)];
    [self.view addSubview:open];
    
    CGRect rect5 = CGRectMake(CGRectGetMaxX(open.frame), y, widthSmall, height);
    UILabel *ping = [self settingLableWithFrame:rect5 Title:LocalizedStringByInt(1605)];
    [self.view addSubview:ping];
    
    
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
    DealDetailCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifer];
    if (cell == nil) {
        cell = [[DealDetailCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifer];
    }
    cell.fenbi.text = @"15:30:50";
    cell.price.text = @"4644";
    cell.xianshou.text = @"4";
    cell.open.text = @"0";
    cell.ping.text = @"4";
    
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
