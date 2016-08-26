//
//  TimeLineView.m
//  CultureTrade
//
//  Created by SZFT1 on 16/1/5.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "TimeLineView.h"
#import "TipView.h"
#import "TimeLineChart.h"
#import "VolumeChart.h"
#import "GlobalModel.h"
#import "DataModel.h"
#import "TimeChartPoint.h"
#import "TimeVolumeChart.h"
#import "GuideView.h"
#import "ChartController.h"

#define Scale_factory 0.8
#define kPointCount 242

@interface TimeLineView () 
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
    
    NSMutableArray *_leftLabelArr;
    UILabel *_startDateLab;  // 5个时间点
    UILabel *_startDateLab1;
    UILabel *_startDateLab2;
    UILabel *_endDateLab;
    UILabel *_endDateLab1;
    UILabel *_volMaxValueLab;
    
    TimeLineChart *_timeLineChart; // K线图
    TimeVolumeChart *_timeVolumeChart;  //成交量图
    TimeChartPoint *_timeChartPoint;  // 点坐标对象
    
    NSMutableArray *_realMinsUnitArr;
    NSMutableArray *_volPriceArr;
    NSMutableArray *_traceUnitArr;
    
    NSMutableArray *_realPointArr;
    
    NSTimer *mTimer;
    NSThread* mThread;
}
@end

@implementation TimeLineView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    [self initialization];
    [self addView];
    return self;
}

- (void)initialization
{
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    _realMinsUnitArr = [NSMutableArray array];
    _volPriceArr = [NSMutableArray array];
    _traceUnitArr = [NSMutableArray array];
    _realPointArr = [NSMutableArray array];
    _timeChartPoint = [[TimeChartPoint alloc] init];
    self.font = [UIFont systemFontOfSize:9];
    _leftLabelArr = [NSMutableArray array];
    //    self.lineWidth = 1.0f;
    finishUpdateBlock = ^(id self){
        dispatch_async(dispatch_get_main_queue(), ^{
            // 更UI
            [self updateInterface];
        });
    };
    
}

- (void)viewWillApper
{
    [self startTimer];
}

- (void)viewWillDisapper
{
    [self stopTimer];
}

- (void)startTimer // 30秒刷一次
{
    if (mTimer == nil) { // 30秒刷一次
        mTimer = [[NSTimer alloc] initWithFireDate:[NSDate date] interval:5.0f target:self selector:@selector(processEvent) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSDefaultRunLoopMode];
        
    }
}

- (void)stopTimer
{
    [mTimer invalidate];
    mTimer = nil;
    
}

- (void)processEvent
{
    MYLOGFUN;
    NSThread* mThrad = [[NSThread alloc] initWithTarget:self selector:@selector(loadCacheData) object:nil];
    [mThrad start];
}

- (void)isFirstUpdate
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSString *fileName = LocalTimeLineDirectory(GLOBAL.sortUnit.productID);
    if([fm fileExistsAtPath:fileName] == YES)
    {
        NSLog(@"File is exist!");
    }else{
        NSError *error;
        BOOL success = [fm createDirectoryAtPath:fileName withIntermediateDirectories:YES attributes:nil error:&error];  // 如果没有中间文件夹直接创建
        if (!success) {
            NSLog(@"%@",error);
        }else{
            NSLog(@"%@",[fm contentsOfDirectoryAtPath:fileName error:nil]);
        }
    }
}

- (void)loadCacheData
{
    MYLog(@"%@",HomeDocument);
    // read local,1.check file exist,2.read file
    [self isFirstUpdate];
    [self processDataOnBackground:[NSString stringWithContentsOfFile:LocalTimeLineFile(GLOBAL.sortUnit.productID) encoding:NSUTF8StringEncoding error:nil]];
    if ([NSTradeEngine sharedInstance].delegate != self) {
        [NSTradeEngine sharedInstance].delegate = self;
    }
    
    [[NSTradeEngine sharedInstance] quote_stktrace_reqNS:[NSString stringWithFormat:@"%d",GLOBAL.sortUnit.m_CodeInfo.m_uiCode]];

}



- (void)quote_ui_stkDealData_rsp:(NSString *)data
{
    MYLog(@"--------UI stkDealData--------- %@",data);
    if (![data hasPrefix:GLOBAL.sortUnit.productID]) { // 两个商品来回切换导致平铺错误
        MYLog(@"--------UI stkDealData  data---------%@",data);
        MYLog(@"--------UI stkDealData  productID---------%@",GLOBAL.sortUnit.productID);
        return;
    }
    mThread = [[NSThread alloc] initWithTarget:self selector:@selector(processDataOnBackground:) object:data];
    [mThread start];
    
    // cache local
    [data writeToFile:LocalTimeLineFile(GLOBAL.sortUnit.productID) atomically:YES encoding:NSUTF8StringEncoding error:nil];
}

- (void)processDataOnBackground:(NSString *)data
{
    [self processData:data];
    [mThread cancel];
    finishUpdateBlock(self);
}

- (void)processData:(NSString *)data
{
    GLOBAL.volPriceArray = [NSMutableArray array];
    NSMutableArray *strDataArray = [NSMutableArray arrayWithArray:[data componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]]];
    for (int i = 0; i < strDataArray.count; i++)
    {
        NSString *strLine = [strDataArray objectAtIndex:i];
        //判断是否有空置
        NSArray *filedArr = [strLine componentsSeparatedByCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@","]];
        if (filedArr.count == 13) {   // 实时行情数据
            NowDataNS *now = [[NowDataNS alloc] init];
            now.m_uiCode  = [filedArr[0] intValue];  //
            now.m_usTime = [filedArr[1] intValue];  //距第一次开盘时间数
            now.m_uiOpenPrice = [filedArr[2] floatValue]*0.001;  //今开盘
            now.m_uiMaxPrice = [filedArr[3] floatValue]*0.001;  //最高价
            now.m_uiMinPrice = [filedArr[4] floatValue]*0.001;  //最低价
            now.m_uiNewPrice = [filedArr[5] floatValue]*0.001;  //最新价
            now.m_uiAverage = [filedArr[6] floatValue]*0.001;  //均价
            now.m_uiVolume = [filedArr[7] intValue];  //成交量总手
            now.m_uiCurVol = [filedArr[8] intValue];  //成交量现手
            now.m_uiSum = [filedArr[9] intValue];  //成交金额，期货中没有此项
            now.m_uiBuyVol = [filedArr[10] intValue];  //主买，对于指数为上涨家数
            now.m_uiSellVol = [filedArr[11] intValue];  //主卖，对于指数为下跌家数
            now.m_ucVolPrices = [filedArr[12] intValue];  //量价个数
            [self updateGuideViewWithnowData:now];
            GLOBAL.nowData = now;               // 存入缓存
        }else if (filedArr.count == 2){  // 买1卖1，10个
            _volPriceArr = [NSMutableArray array];
            VolPriceNS *vol = [[VolPriceNS alloc] init];
            vol.m_uiPrice = [filedArr[0] floatValue];
            vol.m_uiVolume = [filedArr[1] intValue];
            [_volPriceArr addObject:vol];
            [GLOBAL.volPriceArray addObject:vol];
        }else if (filedArr.count == 8){ // 分笔结构
            
            TraceUnitNS *trace = [[TraceUnitNS alloc] init];
            trace.m_usTime = [filedArr[0] intValue];     //距离开盘时间的分钟数
            trace.m_uiNewPrice = [filedArr[1] intValue]; //最新价
            trace.m_uiVolume = [filedArr[2] intValue];   //成交量
            trace.m_uiBuyVol = [filedArr[3] intValue];   //委买量
            trace.m_uiSellVol = [filedArr[4] intValue];  //委卖量
            trace.m_uiBuyPrice = [filedArr[5] intValue]; //委买价，对于指数则表示上涨家数
            trace.m_uiSellPrice = [filedArr[6] intValue];//委卖价，对于指数则表示下跌家数
            trace.m_uiSum = [filedArr[7] intValue];      //成交金额, 持仓量适用于期货市场
            [_traceUnitArr addObject:trace];
        } else if (filedArr.count == 7) { // 分时走线图
            
            RealMinsUnitNS *real = [[RealMinsUnitNS alloc] init];
            real.m_usTime = [filedArr[0] intValue];     //距离第一次开盘时间
            real.m_uiNewPrice = [filedArr[1] intValue];
            real.m_uiVolume = [filedArr[2] intValue];
            real.m_uiKeepVol = [filedArr[3] intValue];   //持仓量,适用于期货市场
            real.m_uiAverage = [filedArr[4] intValue];  //平均价格
            real.m_uiBuyVol = [filedArr[5] intValue]; //买盘数量
            real.m_uiSellVol = [filedArr[6] intValue];//卖盘数量
            [_realMinsUnitArr addObject:real];
        }
    }
    
    _realPointArr = [_timeChartPoint pointArrayTranslateByRealArray:_realMinsUnitArr innerMainFrame:_mainboxView.frame bottomFrame:_bottomBoxView.frame];
    if ([_delegate respondsToSelector:@selector(fiveQuotation_rsp:)]) {
        [_delegate fiveQuotation_rsp:GLOBAL.volPriceArray];
    }
}


#pragma mark -
#pragma mark 添加视图
- (void)addView
{
    if (_mainboxView==nil)
    {
        _mainboxView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width ,self.frame.size.height * 0.6)];
        _mainboxView.backgroundColor = [UIColor colorWithHexString:@"#222222" withAlpha:1];
        _mainboxView.layer.borderColor = [UIColor colorWithHexString:@"#444444" withAlpha:1].CGColor;
        _mainboxView.layer.borderWidth = 0.5;
        _mainboxView.userInteractionEnabled = YES;
        [self addSubview:_mainboxView];
        // 添加手指捏合手势，放大或缩小k线图
//        _pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(touchBoxAction:)];
//        [_mainboxView addGestureRecognizer:_pinchGesture];
        // 添加长按手势，用于显示十字线
        _longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
        [_longPressGestureRecognizer addTarget:self action:@selector(gestureRecognizerHandle:)];
        [_longPressGestureRecognizer setMinimumPressDuration:0.3f];
        [_longPressGestureRecognizer setAllowableMovement:50.0];
        [_mainboxView addGestureRecognizer:_longPressGestureRecognizer];
        // left swipe
//        _panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panBoxAction:)];
//        [_mainboxView addGestureRecognizer:_panGesture];
        
    }
    
    
    // 画个成交量的框框
    if (_bottomBoxView==nil)
    {
        _bottomBoxView = [[UIView alloc] initWithFrame:CGRectMake(0,_mainboxView.frame.size.height+10,self.frame.size.width, self.frame.size.height * 0.3)];
        _bottomBoxView.backgroundColor = [UIColor colorWithHexString:@"#222222" withAlpha:1];
        _bottomBoxView.layer.borderColor = [UIColor colorWithHexString:@"#444444" withAlpha:1].CGColor;
        _bottomBoxView.layer.borderWidth = 0.5;
        _bottomBoxView.userInteractionEnabled = YES;
        [_mainboxView addSubview:_bottomBoxView];
    }

    
    // 左边标签
    CGFloat padRealValue = _mainboxView.frame.size.height / 6;
    _leftLabelArr = [NSMutableArray arrayWithCapacity:7];
    for (int i = 0; i < 7; i++) {
        CGFloat y = _mainboxView.frame.size.height-padRealValue * i;
        UILabel *leftTag = [[UILabel alloc] initWithFrame:CGRectMake(-10, y-10, 38, 30)];
        leftTag.text = [[NSString alloc] initWithFormat:@"%.2f",00.00];
        leftTag.textColor = [UIColor colorWithHexString:@"#cccccc" withAlpha:1];
        leftTag.font = self.font;
        leftTag.textAlignment = NSTextAlignmentRight;
        leftTag.backgroundColor = [UIColor clearColor];
        [self addSubview:leftTag];
        [_leftLabelArr addObject:leftTag];
    }
    
    
    // 添加平均线值显示
//    CGRect mainFrame = _mainboxView.frame;
    
    // 把显示开始结束日期放在成交量的底部左右两侧
    // 显示开始日期控件
    if (_startDateLab==nil)
    {
        _startDateLab = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.origin.x
                                                                  , _bottomBoxView.frame.origin.y+_bottomBoxView.frame.size.height
                                                                  , 50
                                                                  , 15)];
        _startDateLab.font = self.font;
        _startDateLab.text = @"09:30";
        _startDateLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _startDateLab.backgroundColor = [UIColor clearColor];
        [_mainboxView addSubview:_startDateLab];
    }
    
    if (_startDateLab1==nil)
    {
        _startDateLab1 = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.size.width*0.25-25
                                                                  , _bottomBoxView.frame.origin.y+_bottomBoxView.frame.size.height
                                                                  , 50
                                                                  , 15)];
        _startDateLab1.font = self.font;
        _startDateLab1.text = @"10:30";
        _startDateLab1.textAlignment = NSTextAlignmentCenter;
        _startDateLab1.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _startDateLab1.backgroundColor = [UIColor clearColor];
        [_mainboxView addSubview:_startDateLab1];
    }
    
    if (_startDateLab2==nil)
    {
        _startDateLab2 = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.size.width*0.5-25
                                                                   , _bottomBoxView.frame.origin.y+_bottomBoxView.frame.size.height
                                                                   , 50
                                                                   , 15)];
        _startDateLab2.font = self.font;
        _startDateLab2.text = @"11:30";
        _startDateLab2.textAlignment = NSTextAlignmentCenter;
        _startDateLab2.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _startDateLab2.backgroundColor = [UIColor clearColor];
        [_mainboxView addSubview:_startDateLab2];
    }
    
    // 显示结束日期控件
    if (_endDateLab1==nil)
    {
        _endDateLab1 = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.size.width*0.75-35
                                                                , _startDateLab.frame.origin.y
                                                                , 50
                                                                , 15)];
        _endDateLab1.font = self.font;
        _endDateLab1.text = @"14:00";
        _endDateLab1.textAlignment = NSTextAlignmentCenter;
        _endDateLab1.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _endDateLab1.backgroundColor = [UIColor clearColor];
        _endDateLab1.textAlignment = NSTextAlignmentRight;
        [_mainboxView addSubview:_endDateLab1];
    }
    
    // 显示结束日期控件
    if (_endDateLab==nil)
    {
        _endDateLab = [[UILabel alloc] initWithFrame:CGRectMake(_bottomBoxView.frame.size.width-50
                                                                , _startDateLab.frame.origin.y
                                                                , 50
                                                                , 15)];
        _endDateLab.font = self.font;
        _endDateLab.text = @"15:00";
        _endDateLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _endDateLab.backgroundColor = [UIColor clearColor];
        _endDateLab.textAlignment = NSTextAlignmentRight;
        [_mainboxView addSubview:_endDateLab];
    }
    
    // 显示成交量最大值
    if (_volMaxValueLab==nil) {
        _volMaxValueLab = [[UILabel alloc] initWithFrame:CGRectMake(5,_mainboxView.frame.size.height + self.font.lineHeight,30, self.font.lineHeight)];
        _volMaxValueLab.font = self.font;
        _volMaxValueLab.text = @"--";
        _volMaxValueLab.textColor = [UIColor colorWithHexString:@"CCCCCC" withAlpha:1];
        _volMaxValueLab.backgroundColor = [UIColor clearColor];
        _volMaxValueLab.textAlignment = NSTextAlignmentLeft;
        [self addSubview:_volMaxValueLab];
    }
    
    // 长按时的十字线条
    _moveOneView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, 0.5)];
    _moveOneView.backgroundColor = [UIColor whiteColor];
    _moveOneView.hidden = YES;
    [self addSubview:_moveOneView];
    
    _moveTwoView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0.5, CGRectGetMaxY(_bottomBoxView.frame))];
    _moveTwoView.backgroundColor = [UIColor whiteColor];
    _moveTwoView.hidden = YES;
    [self addSubview:_moveTwoView];
    
    _tipView = [[TipView alloc] initWithFrame:CGRectMake(self.frame.size.width - 65, 0, 60, 75)];
    _tipView.layer.borderWidth = 0.5f;
    _tipView.layer.borderColor = [UIColor whiteColor].CGColor;
    _tipView.hidden = YES;
    [self addSubview:_tipView];
    
    _timeLineChart = [[TimeLineChart alloc] initWithFrame:(CGRect){CGPointZero,_mainboxView.frame.size}];
    [_mainboxView addSubview:_timeLineChart];
    
    _timeVolumeChart = [[TimeVolumeChart alloc] initWithFrame:(CGRect){CGPointZero,_bottomBoxView.frame.size}];
    [_bottomBoxView addSubview:_timeVolumeChart];
}


#pragma mark 长按就开始生成十字线
-(void)gestureRecognizerHandle:(UILongPressGestureRecognizer*)longResture
{
    MYLog(@"%ld",(long)longResture.state);
    
    if (_realPointArr.count == 0) return;  // 没有数据不加载十字线和框图
    
    if (longResture.state == UIGestureRecognizerStateBegan) {
        CGPoint currentPoint = [longResture locationInView:_mainboxView];
        MYLog(@"UIGestureRecognizerStateBegan = %@",NSStringFromCGPoint(currentPoint));
        
        // 手指是否越过边线
        
        CGRect oneFrame = _moveOneView.frame;
        oneFrame.origin.y = currentPoint.y;
        _moveOneView.frame = oneFrame;
        
        CGRect twoFrame = _moveTwoView.frame;
        twoFrame.origin.x = currentPoint.x;
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
        CGFloat minX = mainFrame.origin.x;
        CGFloat maxX = mainFrame.size.width + minX;
        CGFloat minY = mainFrame.origin.y;
        CGFloat maxY = mainFrame.size.height + minY;
        
        if (currentPoint.x < minX || currentPoint.x > maxX || currentPoint.y < minY || currentPoint.y > maxY) {
            _moveOneView.hidden = YES;
            _moveTwoView.hidden = YES;
            return;
        }
        
        
        float oneUnit = _mainboxView.frame.size.width / kPointCount;
        // 定位stock
        int theNum = currentPoint.x / oneUnit;
        if (theNum +1 > _realPointArr.count) {
            theNum --;
        }
        
        // 画十字线
        CGRect oneFrame = _moveOneView.frame;
        oneFrame.origin.y = currentPoint.y;
        _moveOneView.frame = oneFrame;
        
        CGRect twoFrame = _moveTwoView.frame;
        twoFrame.origin.x = currentPoint.x;
        _moveTwoView.frame = twoFrame;
        
        // 根据手指长按的位置更改tipView位置不被手指遮盖
        if (currentPoint.x < _tipView.frame.size.width){
            _tipView.frame = CGRectMake(self.frame.size.width - 65, 0, 60, 75);
        }
        if (currentPoint.x > _tipView.frame.origin.x) {
            CGRect frame = _tipView.frame;
            frame.origin.x = 0;
            _tipView.frame = frame;
        }
        
        if (theNum < _realPointArr.count) { // 数组没那么长
            // 显示左上角数值视图
            MYLog(@"theNum stock  = %d",theNum);
            TimeChartPoint *currentChartPoint = _realPointArr[theNum];
            RealMinsUnitNS *realMin = currentChartPoint.realMin;
            [self showTipViewWithChartPoint:realMin];
            // 更新最大值和均线显示标签
//            _tipView.volume.text = [NSString changePrice:realMin.m_uiVolume];
        }
    }
    
    if (longResture.state == UIGestureRecognizerStateEnded) {
        CGPoint currentPoint = [longResture locationInView:_mainboxView];
        MYLog(@"UIGestureRecognizerStateEnded = %@",NSStringFromCGPoint(currentPoint));
        _moveOneView.hidden = YES;
        _moveTwoView.hidden = YES;
        _tipView.hidden = YES;
    }
}

- (void)showTipViewWithChartPoint:(RealMinsUnitNS *)realMin
{
    int time = realMin.m_usTime + 570;
    int hour = time/60;
    int minutes = time%60;
    
    _tipView.date.text = [NSString stringWithFormat:@"时间:%02d:%02d",hour,minutes];
    _tipView.openPrice.text = [NSString stringWithFormat:@"成交:%.2f",realMin.m_uiNewPrice*0.001];
    _tipView.highestPrice.text = [NSString stringWithFormat:@"涨跌:%.2f",realMin.m_uiNewPrice*0.001 - GLOBAL.sortUnit.m_uiPreClose];
    _tipView.lowestPrice.text = [NSString stringWithFormat:@"均价:%.2f",realMin.m_uiAverage*0.001];
    _tipView.closePrice.text = [NSString stringWithFormat:@"成交量:%@",[NSString changePrice:realMin.m_uiVolume]];
    _tipView.hidden = NO;
}


//finishUpdateBlock回调updateInterface
- (void)updateInterface
{
    [self updateChartView];              // 必须先移除旧的view，加载新的view
    _timeLineChart.pointArray = _realPointArr; // set方法重绘
    _timeVolumeChart.pointArray = _realPointArr;  // set方法重绘
    [self updateLabelText];                 // 更新新的view上的文本
};

- (void)updateChartView
{
    [_timeLineChart removeFromSuperview];
    _timeLineChart = [[TimeLineChart alloc] initWithFrame:(CGRect){CGPointZero,_mainboxView.frame.size}];
//    _lineChart.kLineWidth = _chartPoint.kLineWidth;
    [_mainboxView addSubview:_timeLineChart];
    //    _chartPoint.intervalSpace = 0.5;
    
    [_timeVolumeChart removeFromSuperview];
    _timeVolumeChart = [[TimeVolumeChart alloc] initWithFrame:(CGRect){CGPointZero,_bottomBoxView.frame.size}];
//    _volumeChart.kLineWidth = _chartPoint.kLineWidth;
    [_bottomBoxView addSubview:_timeVolumeChart];
}

- (void)updateLabelText
{
//    TimeChartPoint *firstChartPoint = _realPointArr.firstObject;
//    // 最左边的在屏幕外
//    TimeChartPoint *lastChartPoint = _realPointArr.lastObject;
//
//    //开始日期和结束日期
//    _startDateLab.text = [NSString stringWithFormat:@"%d",firstChartPoint.realMin.m_usTime];
//    _endDateLab.text = [NSString stringWithFormat:@"%d",lastChartPoint.realMin.m_usTime];
    // 左边tag
    
//    float maxPrice = GLOBAL.nowData.m_uiMaxPrice;
//    float minPrice = GLOBAL.nowData.m_uiMinPrice;
    
    // 第一次进入页面时会更新这个
//    float maxPrice = GLOBAL.nowData.m_uiMaxPrice;
//    float minPrice = GLOBAL.nowData.m_uiMinPrice;
    float maxPrice = _timeChartPoint.maxPrice;
    float minPrice = _timeChartPoint.minPrice;
//    float interValue = maxPrice - minPrice;
//    maxPrice += interValue*0.1;
//    minPrice -= interValue*0.1;
//    float maxVolume = firstChartPoint.stock.maxVolume;
    for (int i = 0; i< _leftLabelArr.count; i ++) {
        UILabel *leftTag = _leftLabelArr[i];
        
        CGFloat intervalValue = (maxPrice - minPrice) / 6;
        leftTag.text = [[NSString alloc] initWithFormat:@"%.2f",(intervalValue*i+minPrice)];
    }
    
    _volMaxValueLab.text = [NSString changePrice:_timeChartPoint.maxVolume];
    
}

- (void)updateGuideViewWithnowData:(NowDataNS *)nowData
{
    GuideView *guideView = [[(ChartController *)[[self superview] nextResponder] QuotaAnalysisController] guideView];
    float preClose = GLOBAL.stockInfo.m_uiPrevClose;
    
    guideView.yesterdayClose.text =         [NSString stringWithFormat:@"昨收: %.2f",preClose];
    if (nowData.m_uiNewPrice != 0){
        guideView.price.text =                  [NSString stringWithFormat:@"%.2f",(float)nowData.m_uiNewPrice];
        guideView.increase.text =               [NSString stringWithFormat:@"%.2f",(float)(nowData.m_uiNewPrice - preClose)];
        
        if (nowData.m_uiNewPrice < preClose) {
            guideView.increasePercent.text =    [NSString stringWithFormat:@"-%.2f%%",(float)((1 - nowData.m_uiNewPrice/preClose)*100.0)];
        }else{
            guideView.increasePercent.text =    [NSString stringWithFormat:@"%.2f%%",(float)((nowData.m_uiNewPrice/preClose - 1)*100.0)];
        }
    
        guideView.todayOpen.text =              [NSString stringWithFormat:@"今开: %.2f",(float)nowData.m_uiOpenPrice];
        
        guideView.highest.text =                [NSString stringWithFormat:@"最高: %.2f",(float)nowData.m_uiMaxPrice];
        guideView.lowest.text =                 [NSString stringWithFormat:@"最低: %.2f",(float)nowData.m_uiMinPrice];
        guideView.dealVolume.text =             [NSString stringWithFormat:@"成交量: %d",nowData.m_uiVolume];
        guideView.dealTurnover.text =           [NSString stringWithFormat:@"成交额: %d",nowData.m_uiSum];
    }
    
    [guideView fillColor];
}

- (void)trade_login_rsp_to_ui:(int)nRet nType:(int)nType
{
    if ([_delegate respondsToSelector:@selector(login_rsp_ui:type:)]) {
        [_delegate login_rsp_ui:nRet type:nType];
    }
}



@end
