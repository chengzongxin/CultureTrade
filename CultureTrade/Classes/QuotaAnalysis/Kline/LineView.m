//
//  LineView.m
//  CultureTrade
//
//  Created by SZFT1 on 15/11/25.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "LineView.h"
#import "KlineChart.h"
#import "NetManager.h"
#import "Stock.h"
#import "ChartPoint.h"
#import "VolumeChart.h"
#import "TipView.h"
#import "GlobalModel.h"
#import "Archiving.h"
#import "ScaleButton.h"
#define Scale_factory 0.8
#define kLeftTagNumber 6    // kLineChart DashLineNum
#define kLeftTagFont   9
#define kMATagFont     10
#define kTopMargin     5
#define kLeftMatgin    40
#define kMinStocksReq  30

@interface LineView ()
{
    UIView *_mainboxView; // k线图控件
    UIView *_bottomBoxView; // 成交量
    UIView *_moveOneView;   // 十字线
    UIView *_moveTwoView;
    TipView *_tipView;
    
    UIPinchGestureRecognizer *_pinchGesture; //捏合手势
    UILongPressGestureRecognizer *_longPressGestureRecognizer; //长按手势
    UIPanGestureRecognizer *_panGesture; //左右拖动
    CGPoint _currentPoint;    // 手指当前坐标
//    NSMutableArray *_offSetDelArr; // offset delete array
    
    NSMutableArray *_leftLabelArr;
    UILabel *_startDateLab;
    UILabel *_endDateLab;
    UILabel *_MA5;
    UILabel *_MA10;
    UILabel *_MA20;
    UILabel *_volMaxValueLab;
    
    NSMutableArray *_stockArray;   // 缓存所有股票数据
    NSMutableArray *_showStockArray;
    NSMutableArray *_willShowStockArray;
    NSMutableArray *_pointArray;   // 显示出来的坐标
    NSMutableArray *_showArray; // 正在显示的坐标数组
    
    int _selectedKLineType;
    
    KlineChart *_lineChart; // K线图
    VolumeChart *_volumeChart;  //成交量图
    ChartPoint *_chartPoint;
    NetManager *_netMgr;
    
    NSDate *_currentDate;
    BOOL isGetNewKLine;
    
    ScaleButton *_btnLeftMove;
    ScaleButton *_btnRightMove;
    ScaleButton *_btnBigScall;
    ScaleButton *_btnSmallScall;
}

@end

@implementation LineView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initialization];
    [self addView];
    [self loadCacheData];
    return self;
}

- (void)initialization
{
    _netMgr = [[NetManager alloc] init];
    _showStockArray = [NSMutableArray array];
    _willShowStockArray = [NSMutableArray array];
    _currentDate = [NSDate date];
    _chartPoint = [[ChartPoint alloc] init];
    self.font = [UIFont systemFontOfSize:kLeftTagFont];
    _leftLabelArr = [NSMutableArray array];
    //    self.lineWidth = 1.0f;
    finishUpdateBlock = ^(id self){
        [self updateInterface];
    };
    
}



#pragma mark -
#pragma mark 数据加载
- (void)loadCacheData
{
    if (_stockArray) {  // 取缓存
         //mainBox中待显示的个数
        int maxCount = _mainboxView.frame.size.width / (_chartPoint.kLineWidth + _chartPoint.intervalSpace);
        if (_stockArray.count <= maxCount) {
            _showStockArray = [NSMutableArray arrayWithArray:_stockArray];
            _willShowStockArray = [NSMutableArray array];
        }else{//定位显示坐标数组，和待显示坐标数组
            int firstPointInShow = (int)(_stockArray.count - maxCount);
            if (firstPointInShow <= 0) { // 坐标数小于show数目
                _showStockArray = _stockArray;
                _willShowStockArray = [NSMutableArray array];
            }else{
                _showStockArray = [NSMutableArray arrayWithArray:[_stockArray subarrayWithRange:NSMakeRange(firstPointInShow, maxCount-1)]];
                _willShowStockArray = [NSMutableArray arrayWithArray:[_stockArray subarrayWithRange:NSMakeRange(0, firstPointInShow - 1)]];
            }
        }
        TICK;
        _pointArray = [NSMutableArray arrayWithArray:[_chartPoint pointArrayTranslateByStockArray:_showStockArray innerMainFrame:_mainboxView.frame bottomFrame:_bottomBoxView.frame]]; // 换算坐标
        finishUpdateBlock(self);  //回调updateInterface
        TOCK;
    }else{            // 网络重新加载,初始加载日K
    }
}



- (void)loadHisKData:(NSString *)productID type:(int)type requestIndex:(int)index
{
    _selectedKLineType = (type - HISKDATA > 0)?(type - HISKDATA):(type - HISKDATAFIRST);
    [_netMgr loadHisKData:productID type:type requestIndex:index finish:^(NSMutableArray *stockArray) {
        _stockArray = stockArray; // cache 100 stock data
        
        _chartPoint.kLineWidth = _mainboxView.frame.size.width / _stockArray.count - _chartPoint.intervalSpace; // 让K线铺满图
        [self loadCacheData];
//        }
    } error:^(NSError *error) {
        MYLog(@"load stock data error = ");
        _netMgr.isSuccessLoad = NO;
    }];
}

//finishUpdateBlock回调updateInterface
- (void)updateInterface
{
    [self updateChartView];              // 必须先移除旧的view，加载新的view
    _lineChart.pointArray = _pointArray; // set方法重绘
    _volumeChart.pointArray = _pointArray;  // set方法重绘
    [self updateLabelText];                 // 更新新的view上的文本
};


#pragma mark -
#pragma mark 添加视图
- (void)addView
{
    if (_mainboxView==nil)
    {
        _mainboxView = [[UIView alloc] initWithFrame:CGRectMake(kLeftMatgin, kTopMargin, self.frame.size.width - kLeftMatgin ,self.frame.size.height * 0.6)];
        _mainboxView.backgroundColor = [UIColor colorWithHexString:@"#222222" withAlpha:1];
        _mainboxView.layer.borderColor = [UIColor colorWithHexString:@"#444444" withAlpha:1].CGColor;
        _mainboxView.layer.borderWidth = 0.5;
        _mainboxView.userInteractionEnabled = YES;
        [self addSubview:_mainboxView];
    // 添加手指捏合手势，放大或缩小k线图
    _pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(touchBoxAction:)];
    [_mainboxView addGestureRecognizer:_pinchGesture];
    // 添加长按手势，用于显示十字线
    _longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
    [_longPressGestureRecognizer addTarget:self action:@selector(gestureRecognizerHandle:)];
    [_longPressGestureRecognizer setMinimumPressDuration:0.3f];
    [_longPressGestureRecognizer setAllowableMovement:50.0];
    [_mainboxView addGestureRecognizer:_longPressGestureRecognizer];
        // left swipe
        _panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panBoxAction:)];
        [_mainboxView addGestureRecognizer:_panGesture];
    
    }


    // 画个成交量的框框
    if (_bottomBoxView==nil)
    {
        _bottomBoxView = [[UIView alloc] initWithFrame:CGRectMake(kLeftMatgin,CGRectGetMaxY(_mainboxView.frame)+20+kTopMargin,self.frame.size.width - kLeftMatgin, self.frame.size.height * 0.3 - 20)];
        _bottomBoxView.backgroundColor = [UIColor colorWithHexString:@"#222222" withAlpha:1];
        _bottomBoxView.layer.borderColor = [UIColor colorWithHexString:@"#444444" withAlpha:1].CGColor;
        _bottomBoxView.layer.borderWidth = 0.5;
        _bottomBoxView.userInteractionEnabled = YES;
        [self addSubview:_bottomBoxView];
    }

    
    // 左边标签
    int leftTagNum = kLeftTagNumber;
    CGFloat padRealValue = _mainboxView.frame.size.height / (leftTagNum - 1);
        _leftLabelArr = [NSMutableArray arrayWithCapacity:leftTagNum];
    CGFloat tagWidth = kLeftMatgin;
    CGFloat tagHeight = self.font.lineHeight;
    for (int i = 0; i < leftTagNum; i++) {
        CGFloat y = _mainboxView.frame.size.height - padRealValue * i;
        UILabel *leftTag = [[UILabel alloc] initWithFrame:CGRectMake(-10, y+kTopMargin, tagWidth, tagHeight)];
//        if (i == 0) {
//            leftTag.frame = CGRectMake(-10, y+5-tagHeight, tagWidth, tagHeight);
//        }
        leftTag.text = [[NSString alloc] initWithFormat:@"%.2f",00.00];
        leftTag.textColor = [UIColor colorWithHexString:@"#cccccc" withAlpha:1];
        leftTag.font = self.font;
        leftTag.textAlignment = NSTextAlignmentRight;
        leftTag.backgroundColor = [UIColor clearColor];
        [self addSubview:leftTag];
        [_leftLabelArr addObject:leftTag];
    }
    
//    UILabel *topTag = [_leftLabelArr lastObject];
//    topTag.frame = CGRectMake(-10, 5-tagHeight, tagWidth, tagHeight);
    
    // 添加平均线值显示
    CGRect mainFrame = _mainboxView.frame;
    
    CGFloat MAWidth = 60;
    CGFloat MAHeight = 20;
    
    // MA5 均线价格显示控件
    if (_MA5==nil) {
        _MA5 = [[UILabel alloc] initWithFrame:CGRectMake(mainFrame.origin.x + 5, -12, MAWidth, MAHeight)];
        _MA5.backgroundColor = [UIColor clearColor];
        _MA5.font = [UIFont systemFontOfSize:kMATagFont];
        _MA5.lineBreakMode = NSLineBreakByWordWrapping;
        _MA5.text = @"MA5:--";
        _MA5.textColor = [UIColor whiteColor];
//        [_MA5 sizeToFit];
        [self addSubview:_MA5];
    }
    
    
    // MA10 均线价格显示控件
    if (_MA10==nil) {
        _MA10 = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetMaxX(_MA5.frame) +10, -12, MAWidth, MAHeight)];
        _MA10.backgroundColor = [UIColor clearColor];
        _MA10.font = [UIFont systemFontOfSize:kMATagFont];
        _MA10.lineBreakMode = NSLineBreakByWordWrapping;
        _MA10.text = @"MA10:--";
        _MA10.textColor = [UIColor colorWithHexString:@"#FF9900" withAlpha:1];
//        [_MA10 sizeToFit];
        [self addSubview:_MA10];
    }
    
    
    // MA20 均线价格显示控件
    if (_MA20==nil) {
        _MA20 = [[UILabel alloc] initWithFrame:CGRectMake(CGRectGetMaxX(_MA10.frame) +10, -12, MAWidth, MAHeight)];
        _MA20.backgroundColor = [UIColor clearColor];
        _MA20.font = [UIFont systemFontOfSize:kMATagFont];
        _MA20.lineBreakMode = NSLineBreakByWordWrapping;
        _MA20.text = @"MA20:--";
        _MA20.textColor = [UIColor colorWithHexString:@"#FF00FF" withAlpha:1];
//        [_MA20 sizeToFit];
        [self addSubview:_MA20];
    }

    // 把显示开始结束日期放在成交量的底部左右两侧
    // 显示开始日期控件
    if (_startDateLab==nil)
    {
        _startDateLab = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.origin.x
                                                                 , _bottomBoxView.frame.origin.y+_bottomBoxView.frame.size.height
                                                                 , 50, 15)];
        _startDateLab.font = self.font;
        _startDateLab.text = @"--";
        _startDateLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _startDateLab.backgroundColor = [UIColor clearColor];
        [self addSubview:_startDateLab];
    }

    // 显示结束日期控件
    if (_endDateLab==nil)
    {
        _endDateLab = [[UILabel alloc] initWithFrame:CGRectMake(self.frame.size.width-55
                                                               , _startDateLab.frame.origin.y
                                                               , 50, 15)];
        _endDateLab.font = self.font;
        _endDateLab.text = @"--";
        _endDateLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _endDateLab.backgroundColor = [UIColor clearColor];
        _endDateLab.textAlignment = NSTextAlignmentRight;
        [self addSubview:_endDateLab];
    }
    
    // 显示成交量最大值
    if (_volMaxValueLab==nil) {
        _volMaxValueLab = [[UILabel alloc] initWithFrame:CGRectMake(5,_bottomBoxView.frame.origin.y + 5,30, self.font.lineHeight)];
        _volMaxValueLab.font = self.font;
        _volMaxValueLab.text = @"--";
        _volMaxValueLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _volMaxValueLab.backgroundColor = [UIColor clearColor];
        _volMaxValueLab.textAlignment = NSTextAlignmentLeft;
        [self addSubview:_volMaxValueLab];
    }
    
    // 长按时的十字线条
    _moveOneView = [[UIView alloc] initWithFrame:CGRectMake(kLeftMatgin, kTopMargin, self.frame.size.width, 0.5)];
    _moveOneView.backgroundColor = [UIColor whiteColor];
    _moveOneView.hidden = YES;
    [self addSubview:_moveOneView];
    
    _moveTwoView = [[UIView alloc] initWithFrame:CGRectMake(kLeftMatgin, kTopMargin, 0.5, CGRectGetMaxY(_bottomBoxView.frame) - kTopMargin)];
    _moveTwoView.backgroundColor = [UIColor whiteColor];
    _moveTwoView.hidden = YES;
    [self addSubview:_moveTwoView];
    
    _tipView = [[TipView alloc] initWithFrame:CGRectMake(_mainboxView.frame.size.width - 75, 0, 75, 95)];
    _tipView.hidden = YES;
    [_mainboxView addSubview:_tipView];
    
    _lineChart = [[KlineChart alloc] initWithFrame:(CGRect){CGPointZero,_mainboxView.frame.size}];
    [_mainboxView addSubview:_lineChart];
    
    _volumeChart = [[VolumeChart alloc] initWithFrame:(CGRect){CGPointZero,_bottomBoxView.frame.size}];
    [_bottomBoxView addSubview:_volumeChart];
    
    // scale button
    float leftMargin = (_mainboxView.frame.size.width - 20)/ 4;
    float interValue = _mainboxView.frame.size.width / 4;
    CGFloat y = CGRectGetMaxY(_mainboxView.frame) + 2;
    _btnLeftMove = [[ScaleButton alloc] init];
    _btnLeftMove.frame = CGRectMake(leftMargin, y, 20, 20);
    [_btnLeftMove setTitle:@"<-" forState:UIControlStateNormal];
    _btnLeftMove.tag = 3;
    [_btnLeftMove addTarget:self action:@selector(leftSlideStockCount:) forControlEvents:UIControlEventTouchDown];
    [self addSubview:_btnLeftMove];
    
    _btnRightMove = [[ScaleButton alloc] init];
    _btnRightMove.frame = CGRectMake(leftMargin + interValue , y, 20, 20);
    [_btnRightMove setTitle:@"->" forState:UIControlStateNormal];
    _btnRightMove.tag = 3;
    [_btnRightMove addTarget:self action:@selector(rightSlideStockCount:) forControlEvents:UIControlEventTouchDown];
    [self addSubview:_btnRightMove];
    
    _btnBigScall = [[ScaleButton alloc] init];
    _btnBigScall.frame = CGRectMake(leftMargin + interValue * 2, y, 20, 20);
    [_btnBigScall setTitle:@"+" forState:UIControlStateNormal];
    [_btnBigScall addTarget:self action:@selector(bigScale) forControlEvents:UIControlEventTouchDown];
    [self addSubview:_btnBigScall];
    
    _btnSmallScall = [[ScaleButton alloc] init];
    _btnSmallScall.frame = CGRectMake(leftMargin + interValue * 3, y, 20, 20);
    [_btnSmallScall setTitle:@"-" forState:UIControlStateNormal];
    [_btnSmallScall addTarget:self action:@selector(smallScale) forControlEvents:UIControlEventTouchDown];
    [self addSubview:_btnSmallScall];
    
    
}


#pragma mark -
#pragma mark 手指捏合动作
-(void)touchBoxAction:(UIPinchGestureRecognizer*)pGesture
{
    MYLog(@"状态：%li==%f",(long)_pinchGesture.state,pGesture.scale);
    if (pGesture.state==2) {
        if (pGesture.scale>1) {
            // 放大手势
            [self bigScale];
        }else{
            // 缩小手势
            [self smallScale];
        }
    }
    if (pGesture.state==3) {
    }
}

#pragma mark -
#pragma mark 左右拖动重绘K线
- (void)panBoxAction:(UIPanGestureRecognizer *)recognizer
{
    // start slide view
    if (recognizer.state == UIGestureRecognizerStateBegan) {
        _currentPoint = [recognizer translationInView:_mainboxView];
    }
    CGPoint newPoint;
    if (recognizer.state == UIGestureRecognizerStateChanged) {
        newPoint = [recognizer translationInView:_mainboxView];
        MYLog(@"newpoint = %f,%f",newPoint.x,newPoint.y);
    }
    
    // bug 会回弹到0.000，0.000
    // x++ finger right move,view right move
    if (newPoint.x > 1) { //
        int offset = (newPoint.x - _currentPoint.x) / (_chartPoint.kLineWidth + _chartPoint.intervalSpace); // 偏移了几个单位的klinewith 就是等于几个数组
        if (offset < 0) return;   // 用户拽到一般回拖的情况
        [self leftSlideStockCount:[NSNumber numberWithInt:offset]];
    }else if (newPoint.x < -1){    // x-- finger left move,view left move
                // 最新的点是按照正负增大
        int offset = (_currentPoint.x - newPoint.x) / (_chartPoint.kLineWidth + _chartPoint.intervalSpace);
        if (offset < 0) return;   // 用户拽到一般回拖的情况
        [self rightSlideStockCount:[NSNumber numberWithInt:offset]];
    }
    _currentPoint.x = newPoint.x;
    
    // end sliede view
    if (recognizer.state == UIGestureRecognizerStateEnded) {
        newPoint = [recognizer translationInView:_mainboxView];
    }
}
// leftSlide
- (void)leftSlideStockCount:(id)sender
{
    int offset = 0;
    if ([sender isKindOfClass:[UIButton class]]) {
        offset = (int)[(UIButton *)sender tag];
    }else{
        offset = [(NSNumber *)sender intValue];
    }
    // slide to the most left edge don't slide
    int maxCount = [_chartPoint numberOfStockInFrame:_mainboxView.frame];
    int leftStockCount = MIN((int)_willShowStockArray.count,maxCount);//排除小于0
    int time = MIN(offset, leftStockCount);
    for (int i = 0; i<time; i++) {
        [_showStockArray insertObject:[_willShowStockArray lastObject] atIndex:0];
        [_willShowStockArray removeLastObject];
    }
    
    _pointArray = [NSMutableArray arrayWithArray:[_chartPoint pointArrayTranslateByStockArray:_showStockArray innerMainFrame:_mainboxView.frame bottomFrame:_bottomBoxView.frame]];
    finishUpdateBlock(self);
    
    if (_willShowStockArray.count <=kMinStocksReq) {//加载数据 willshowstockarray <= 20
        
        NSDate *nowDate = [NSDate date];
        if ([nowDate timeIntervalSinceDate:_currentDate] < 0.1) {   // 拖动多次回调bug，定时器解决
            MYLog(@"%f",[nowDate timeIntervalSinceDate:_currentDate]);
            _currentDate = nowDate;
            return;
        }
        
        _currentDate = nowDate;
        if (_netMgr.isLoading == YES) return;//正在加载时不拖动K线图，不重新加载
        _netMgr.isLoading = YES;
        [_netMgr loadHisKData:GLOBAL.sortUnit.productID type:HISKDATA+_selectedKLineType requestIndex:(int)(_showStockArray.count+_willShowStockArray.count)  finish:^(NSMutableArray *stockArray) {
            _willShowStockArray = stockArray;
            _netMgr.isLoading = NO;
        } error:^(NSError *error) {
            MYLog(@"%@",error);
        }];
    }
}

// rightSlide
- (void)rightSlideStockCount:(id)sender
{
    int offset = 0;
    if ([sender isKindOfClass:[UIButton class]]) {
        offset = (int)[(UIButton *)sender tag];
    }else{
        offset = [(NSNumber *)sender intValue];
    }
    unsigned long time = MIN(offset, _showStockArray.count);
    
    // 移动到最右边时不再执行添加数组操作
    if (_showStockArray.count <= [_chartPoint numberOfStockInFrame:_mainboxView.frame]) return;
    
    for (int i = 0 ; i < time; i ++) {
        [_willShowStockArray addObject:[_showStockArray firstObject]];
        [_showStockArray removeObjectAtIndex:0];
    }
    
    _pointArray = [NSMutableArray arrayWithArray:[_chartPoint pointArrayTranslateByStockArray:_showStockArray innerMainFrame:_mainboxView.frame bottomFrame:_bottomBoxView.frame]];
    finishUpdateBlock(self);
}

#pragma mark -
#pragma mark 长按就开始生成十字线
-(void)gestureRecognizerHandle:(UILongPressGestureRecognizer*)longResture
{
    MYLog(@"%ld",(long)longResture.state);
   
    if (_pointArray.count == 0) return;  //没有数据不显示十字线和小视图
    if (longResture.state == UIGestureRecognizerStateBegan) {
        CGPoint currentPoint = [longResture locationInView:_mainboxView];
        MYLog(@"UIGestureRecognizerStateBegan = %@",NSStringFromCGPoint(currentPoint));
        
        // 手指是否越过边线
        
        CGRect oneFrame = _moveOneView.frame;
        oneFrame.origin.y = currentPoint.y;
        _moveOneView.frame = oneFrame;
        
        CGRect twoFrame = _moveTwoView.frame;
        twoFrame.origin.x = currentPoint.x + kLeftMatgin;
        _moveTwoView.frame = twoFrame;
        
        _moveOneView.hidden = NO;
        _moveTwoView.hidden = NO;
        _tipView.hidden = NO;
    }
    // 滑动过程中
    if (longResture.state == UIGestureRecognizerStateChanged) {
        CGPoint currentPoint = [longResture locationInView:_mainboxView];
        MYLog(@"UIGestureRecognizerStateChanged = %@",NSStringFromCGPoint(currentPoint));
        
        // 滑出边界
        CGRect mainFrame = _mainboxView.frame;
        CGFloat minX = mainFrame.origin.x - kLeftMatgin;
        CGFloat maxX = mainFrame.size.width + minX;
        CGFloat minY = mainFrame.origin.y;
        CGFloat maxY = mainFrame.size.height + minY;

        if (currentPoint.x < minX || currentPoint.x > maxX || currentPoint.y < minY || currentPoint.y > maxY) {
            _moveOneView.hidden = YES;
            _moveTwoView.hidden = YES;
            return;
        }

        // 定位stock
        int theNum = currentPoint.x / (_chartPoint.kLineWidth + _chartPoint.intervalSpace);
        if (theNum +1 > _pointArray.count) {
            theNum --;
        }
        
        // 画十字线
        CGRect oneFrame = _moveOneView.frame;
        oneFrame.origin.y = currentPoint.y;
        _moveOneView.frame = oneFrame;
        
        CGRect twoFrame = _moveTwoView.frame;
        twoFrame.origin.x = currentPoint.x + kLeftMatgin;
        _moveTwoView.frame = twoFrame;
        // 铺在最上层
//        [_moveOneView removeFromSuperview];
//        [_mainboxView addSubview:_moveOneView];
//        [_moveTwoView removeFromSuperview];
//        [_mainboxView addSubview:_moveTwoView];
        

        // 根据手指长按的位置更改tipView位置不被手指遮盖
        if (currentPoint.x < _tipView.frame.size.width){
            _tipView.frame = CGRectMake(_mainboxView.frame.size.width - 75, 0, 75, 95);
        }
        if (currentPoint.x > _tipView.frame.origin.x) {
            CGRect frame = _tipView.frame;
            frame.origin.x = 0;
            _tipView.frame = frame;
        }
        // 铺在最上层
        [_tipView removeFromSuperview];
        [_mainboxView addSubview:_tipView];
        
        if (theNum < _pointArray.count) { // 数组没那么长
        // 显示左上角数值视图
        ChartPoint *currentChartPoint = _pointArray[theNum];
        Stock *stock = currentChartPoint.stock;
        [self showTipViewWithChartPoint:stock];
        // 更新最大值和均线显示标签
        
        _MA5.text = [NSString stringWithFormat:@"MA5:%.2f",stock.MA5];
        _MA10.text = [NSString stringWithFormat:@"MA10:%.2f",stock.MA10];
        _MA20.text = [NSString stringWithFormat:@"MA20:%.2f",stock.MA20];
        }
    }
    
    if (longResture.state == UIGestureRecognizerStateEnded) {
        _moveOneView.hidden = YES;
        _moveTwoView.hidden = YES;
        _tipView.hidden = YES;
        _MA5.text = @"MA5:--";
        _MA10.text = @"MA10:--";
        _MA20.text = @"MA20:--";
    }
}

- (void)showTipViewWithChartPoint:(Stock *)s
{
    if (_selectedKLineType <= (char)60) { // 分线
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"MddHHmm"];
        NSDate *date = [dateFormatter dateFromString:s.date];
        [dateFormatter setDateFormat:@"MM/dd HH:mm"];
        _tipView.date.text = [NSString stringWithFormat:@"时间:%@",[dateFormatter stringFromDate:date]];
    }else{
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyyMMdd"];
        NSDate *date = [dateFormatter dateFromString:s.date];
        [dateFormatter setDateFormat:@"yyyy/MM/dd"];
        _tipView.date.text = [NSString stringWithFormat:@"时间:%@",[dateFormatter stringFromDate:date]];
    }
    [_tipView.date sizeToFit];
    
    _tipView.openPrice.text = [NSString stringWithFormat:@"开盘:%.2f",s.openPrice];
    _tipView.highestPrice.text = [NSString stringWithFormat:@"最高:%.2f",s.highestPrice];
    _tipView.lowestPrice.text = [NSString stringWithFormat:@"最低:%.2f",s.lowestPrice];
    _tipView.closePrice.text = [NSString stringWithFormat:@"收盘:%.2f",s.closePrice];
    _tipView.volume.text = [NSString stringWithFormat:@"成交量:%@",[NSString changePrice:s.volume]];
//    _tipView.volume.text = [NSString changePrice:s.volume];
//    _tipView.volume.text = [NSString stringWithFormat:@"成交量:%.2f",s.volume];
    _tipView.hidden = NO;
}

- (void)bigScale
{
    [self zoomKlineChartWith:1]; //++
}

- (void)smallScale
{
    //-- 当移动到最左边缩放会导致bug,先右移动，再缩放
    if (_showStockArray.count + 1 < _willShowStockArray.count) {
        _chartPoint.kLineWidth --;
        if (_chartPoint.kLineWidth < 1) {
            _chartPoint.kLineWidth = 1;
            return;
        }
        int count = [_chartPoint numberOfStockInFrame:_mainboxView.frame]; // 缩放补足右边空缺
        for (int i = 0 ; i < count; i ++) {
            if (_willShowStockArray.count == 0) return;
            [_showStockArray insertObject:[_willShowStockArray lastObject] atIndex:0];
            [_willShowStockArray removeLastObject];
        }
        _pointArray = [NSMutableArray arrayWithArray:[_chartPoint pointArrayTranslateByStockArray:_showStockArray innerMainFrame:_mainboxView.frame bottomFrame:_bottomBoxView.frame]];
        finishUpdateBlock(self);
    }
    
    
    [self zoomKlineChartWith:-1];
}

- (void)zoomKlineChartWith:(int)i
{
    _chartPoint.kLineWidth += i;
    if (_chartPoint.kLineWidth > 30){
        _chartPoint.kLineWidth = 30;
        return;
    }
    
    if (_chartPoint.kLineWidth < 1){
        _chartPoint.kLineWidth = 1;
        return;
    }

    [self loadCacheData];
}

- (void)updateChartView
{
    [_lineChart removeFromSuperview];
    _lineChart = [[KlineChart alloc] initWithFrame:(CGRect){CGPointZero,_mainboxView.frame.size}];
    _lineChart.kLineWidth = _chartPoint.kLineWidth;
    [_mainboxView addSubview:_lineChart];
    //    _chartPoint.intervalSpace = 0.5;
    
    [_volumeChart removeFromSuperview];
    _volumeChart = [[VolumeChart alloc] initWithFrame:(CGRect){CGPointZero,_bottomBoxView.frame.size}];
    _volumeChart.kLineWidth = _chartPoint.kLineWidth;
    [_bottomBoxView addSubview:_volumeChart];
}


- (void)updateLabelText
{
    ChartPoint *firstChartPoint = _pointArray.firstObject;
      // 最左边的在屏幕外
    ChartPoint *lastChartPoint = _pointArray.lastObject;
    
    //开始日期和结束日期
    NSString *startStr = firstChartPoint.stock.date;
    NSString *endStr = lastChartPoint.stock.date;
    
    if (_selectedKLineType <= (char)60) { // 分线
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"MddHHmm"];
        NSDate *startDate = [dateFormatter dateFromString:startStr];
        NSDate *endDate = [dateFormatter dateFromString:endStr];
        
        [dateFormatter setDateFormat:@"MM/dd HH:mm"];
        
        _startDateLab.text = [dateFormatter stringFromDate:startDate];
        _endDateLab.text = [dateFormatter stringFromDate:endDate];
    }else{
        NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateFormat:@"yyyyMMdd"];
        NSDate *startDate = [dateFormatter dateFromString:startStr];
        NSDate *endDate = [dateFormatter dateFromString:endStr];
        
        [dateFormatter setDateFormat:@"yyyy/MM/dd"];
        
        _startDateLab.text = [dateFormatter stringFromDate:startDate];
        _endDateLab.text = [dateFormatter stringFromDate:endDate];
    }
    [_startDateLab sizeToFit];
    [_endDateLab sizeToFit];
    
    // 左边tag
    
    float maxPrice = firstChartPoint.stock.maxPrice;
    float minPrice = firstChartPoint.stock.minPrice;
    float maxVolume = firstChartPoint.stock.maxVolume;
    for (int i = 0; i< _leftLabelArr.count; i ++) {
        UILabel *leftTag = _leftLabelArr[i];
        
        CGFloat intervalValue = (maxPrice - minPrice) / (kLeftTagNumber - 1);
        leftTag.text = [[NSString alloc] initWithFormat:@"%.2f",intervalValue*i+minPrice];
    }
    
    _volMaxValueLab.text = [NSString changePrice:maxVolume];
    
}

@end
