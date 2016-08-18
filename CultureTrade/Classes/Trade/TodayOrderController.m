//
//  TodayOrderController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/8/18.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TodayOrderController.h"

@implementation TodayOrderController

- (void)viewDidLoad {
    [super viewDidLoad];
    
}

- (void)addTitleView
{
    self.title = @"今日委托";
}

- (void)trade_ui_today_entrust_rsp:(NSString *)data
{
    [self closeProgressSuccess:@"Success!"];
    _orderArray = [NSMutableArray array];
    NSError *error = nil;
    NSDictionary *str2dict = [NSJSONSerialization JSONObjectWithData:[data dataUsingEncoding:NSUTF8StringEncoding]
                                                             options:NSJSONReadingMutableContainers
                                                               error:&error];
    NSMutableArray *array = [str2dict objectForKey:@"ORDER"];
    NSMutableArray *extraArray = [NSMutableArray arrayWithArray:array];
    
    if ([str2dict allValues].count == 1) {
        if ([[[str2dict allValues] objectAtIndex:0] intValue] == -100001) {
            showAlert(LocalizedStringByInt(-100001));
            return;
        }
    }
    _orderArray = [NSMutableArray arrayWithArray:extraArray];
    [_tableView reloadData];
    
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_tableView deselectRowAtIndexPath:indexPath animated:YES];
}

@end
