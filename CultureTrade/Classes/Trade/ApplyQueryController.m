//
//  ApplyQueryController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/19.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyQueryController.h"

@interface ApplyQueryController ()

@end

@implementation ApplyQueryController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"中签查询";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

//overwride
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    if (tableView == _leftTable) {
        return [[LeftApplyEntrustBanner alloc] initWithFrame:CGRectMake(0, 0, kLeftTableWith, kQuotaBannerHeight)];
    }else if (tableView == _mainTable){
        ApplyEntrustBanner *mainBanner = [[ApplyEntrustBanner alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width*kMainTableViewWithRatio, kQuotaBannerHeight)];
        [mainBanner setCount:7 textIndex:2901];
        return mainBanner;
    }else{
        return NULL;
    }
}
//overwrite
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    SymbolModel *symbol = GLOBAL.symbolArray[indexPath.row];
    if (tableView.tag == 0) {
        static NSString *cellID = @"LeftPurchaseCell";
        LeftApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[LeftApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithSymbol:symbol];
    }else{
        static NSString *cellID = @"PurchaseCell";
        ApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[ApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        cell.labelCount = 7;
        return [cell initWithSymbol:symbol];
    }
}

@end
