//
//  TraderController.m
//  CultureTrade
//
//  Created by SZFT1 on 16/3/14.
//  Copyright © 2016年 cheng.zongxin. All rights reserved.
//

#import "TraderController.h"
#import "LoginController.h"
#import "UIBarButtonItem+helper.h"
#import "CombiInputText.h"

@interface TraderController () <UITableViewDelegate,UITableViewDataSource>
{
    NSMutableArray *_serverArray;
    CombiInputText *_textFiled;
}
@end

@implementation TraderController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self initialization];
    
    [self addTitle];
    
    [self addTableView];
    
    
}

- (void)initialization
{
    _serverArray = [NSMutableArray arrayWithObjects:@"192.168.0.194",
                                                    @"192.168.0.168",
                                                    @"120.25.224.254",
                                                    @"192.168.10.28",
                    nil];
}

- (void)addTitle
{
    self.navigationItem.leftBarButtonItem = [UIBarButtonItem itemWithIcon:@"back" highlightedIcon:nil target:self action:@selector(back)];
}

- (void)back
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (void)addTableView
{
    UITableView *tableView = [[UITableView alloc] init];
    tableView.frame = self.view.frame;
    tableView.delegate = self;
    tableView.dataSource = self;
    tableView.backgroundColor = COLOR_QUOTA_TABLE_BG;
    tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    [self.view addSubview:tableView];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _serverArray.count;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSArray *viewControllers = self.navigationController.viewControllers;
    for (UIViewController *vc in viewControllers) {
        if ([vc isKindOfClass:[LoginController class]]) {
            LoginController *loginVC = (LoginController *)vc;
            loginVC.tradeIP = _serverArray[indexPath.row];
            loginVC.quotaIP = _serverArray[indexPath.row];
            loginVC.title = _serverArray[indexPath.row];
        }
    }
    [self.navigationController popViewControllerAnimated:YES];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellID = @"ServerCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellID];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellID];
    }
    cell.textLabel.textColor = [UIColor whiteColor];
    cell.backgroundColor = COLOR_QUOTA_CELL_BG;
    cell.textLabel.text = _serverArray[indexPath.row];
    
    return cell;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 44)];
    
    CombiInputText *textField = [[CombiInputText alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width - 60, 44)];
    
    textField.leftLabel.text = @"服务器IP:";
    
    textField.keyboardType = UIKeyboardTypeDecimalPad;
    
    [view addSubview:textField];
    
    _textFiled = textField;
    
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    
    btn.frame = CGRectMake(self.view.frame.size.width - 60, 0, 60, 44);
    
    [btn addTarget:self action:@selector(clickComfirm) forControlEvents:UIControlEventTouchDown];
    
    [btn setTitle:@"确定" forState:UIControlStateNormal];
    
    [view addSubview:btn];
    
    return view;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 44;
}

- (void)clickComfirm
{
    NSArray *viewControllers = self.navigationController.viewControllers;
    for (UIViewController *vc in viewControllers) {
        if ([vc isKindOfClass:[LoginController class]]) {
            LoginController *loginVC = (LoginController *)vc;
            loginVC.tradeIP = _textFiled.text;
            loginVC.quotaIP = _textFiled.text;
            loginVC.title = _textFiled.text;
        }
    }
    [self.navigationController popViewControllerAnimated:YES];
}
@end
