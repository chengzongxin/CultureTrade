//
//  ApplyQueryController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/7/19.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "ApplyQueryController.h"

@interface ApplyQueryController ()
{
    NSMutableArray *_applySuccessArray;
}
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

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _applySuccessArray.count;
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
    ApplyPurchaseSuccess *success = _applySuccessArray[indexPath.row];
    if (tableView.tag == 0) {
        static NSString *cellID = @"LeftApplyPurchaseSuccess";
        LeftApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[LeftApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
        }
        return [cell initWithSuccess:success];
    }else{
        static NSString *cellID = @"ApplyPurchaseSuccess";
        ApplyEntrustCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
        if (cell == nil) {
            cell = [[ApplyEntrustCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
            cell.labelCount = 7;
        }
        
        return [cell initWithSuccess:success];
    }
}

-(void)startRequest {
    
    [self showProgress:@"Loading..."];
    
    NSString *accountName = [[NSTradeEngine sharedInstance] getLoginedAccountName];
    
    NSString *session =[[NSTradeEngine sharedInstance] getSession];
    
    NSString *strURL = [NSString stringWithFormat:ENTRUSTSUCCESS,accountName,session,_startBtn.currentTitle,_endBtn.currentTitle];
    
    NSURL *url = [NSURL URLWithString:strURL];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    NSURLConnection *connection = [[NSURLConnection alloc]
                                   initWithRequest:request
                                   delegate:self];
    if (connection) {
        _datas = [NSMutableData new];
    }
}
#pragma mark- NSURLConnection 回调方法
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    [_datas appendData:data];
}
-(void) connection:(NSURLConnection *)connection didFailWithError: (NSError *)error {
    NSLog(@"%@",[error localizedDescription]);
}
- (void) connectionDidFinishLoading: (NSURLConnection*) connection {
    NSLog(@"请求完成…");
    NSDictionary* dict = [NSJSONSerialization JSONObjectWithData:_datas
                                                         options:NSJSONReadingAllowFragments error:nil];
    MYLog(@"connectionDidFinishLoading = %@",dict);
    
    _applySuccessArray = [NSMutableArray array];
    NSArray *entrusts = dict[@"data"];
    
    for (NSDictionary *subdict in entrusts) {
        ApplyPurchaseSuccess *entrust = [ApplyPurchaseSuccess applyPurchaseSuccessWithDictionary:subdict];
        [_applySuccessArray addObject:entrust];
    }
    
    [_leftTable reloadData];
    [_mainTable reloadData];
    
    [self closeProgressSuccess:@"Success!"];
}
@end
