//
//  NSString+helper.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "NSString+helper.h"

@implementation NSString (helper)
- (NSString *)fileAppend:(NSString *)append
{
    // 1.1.获得文件拓展名
    NSString *ext = [self pathExtension];
    
    // 1.2.删除最后面的扩展名
    NSString *file = [self stringByDeletingPathExtension];
    
    // 1.3.拼接-568h@2x
    file = [file stringByAppendingString:append];
    
    // 1.4.拼接扩展名
    return [file stringByAppendingPathExtension:ext];
}

+ (NSString *)LocalizedStringByInt:(int)key
{
    NSString *keyStr = [NSString stringWithFormat:@"%d",key];
    return NSLocalizedString(keyStr, nil);
}

+ (NSString *)int2string:(int)intValue
{
    NSString *strValue = [NSString stringWithFormat:@"%d",intValue];
    return strValue;
}

+(NSString *)toNSString:(const char *) c {
    if(c != NULL && strlen(c) > 0) {
//        return [NSString stringWithCString:c encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGBK_95)];
        NSString *nsstring = [NSString stringWithCString:c encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000)];
        return nsstring;
    }
    return @"";
}

+ (NSString *)stringFromDate:(NSDate *)date
{
    NSDateFormatter *df = [[NSDateFormatter alloc] init];
    // yyyy-MM-dd HH:mm:ss zzz   大写HH为24小时制，zzz表示时区
    [df setDateFormat:@"yyyy-MM-dd"];
    
    return [df stringFromDate:date];
}

// 数值变化
+(NSString*)changePrice:(int)price{
    float newPrice = 0.0f;
    NSString *danwei = @"万";
    if (price<10000) {
        danwei = @"";
        return [[NSString alloc] initWithFormat:@"%d%@",price,danwei];
    }else if (price>=10000) {
        newPrice = price / 10000 ;
    }else if (price>=10000000) {
        newPrice = price / 10000000 ;
        danwei = @"千万";
    }else{
        newPrice = price / 100000000 ;
        danwei = @"亿";
    }
    return [[NSString alloc] initWithFormat:@"%.0f%@",newPrice,danwei];
}

+(NSString*)changePriceByFloat:(float)price{
    price = (double)price;
    float newPrice = 0.0f;
    NSString *danwei;
    float negative = 1;
    if (price < 0) {
        negative = -1;
        price = price * negative;
    }
    if (price<10000) {
        newPrice = price;
        danwei = @"";
    }else if (price>=10000 && price < 100000000) {
        newPrice = price / 10000;
        danwei = @"万";
    }else if (price>=100000000 && price < 1000000000000){
        newPrice = price / 100000000;
        danwei = @"亿";
    }else if (price>=1000000000000 && price < 10000000000000000){
        newPrice = price / 1000000000000;
        danwei = @"万亿";
    }else{
        newPrice = price / 10000000000000000;
        danwei = @"亿亿";
    }
    newPrice = newPrice * negative;
    return [[NSString alloc] initWithFormat:@"%.02f%@",newPrice,danwei];
}

//+(char *)toCString:(NSString*)s{
//    if(s != nil)
//    {
//    char *newchar = (char *)malloc(sizeof(char)*s.length + 1);
//    memset(newchar, 0, sizeof(char)+1);
//    strcpy(newchar, ([s cStringUsingEncoding: CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000)]));
////    free(newchar);
//    return newchar;
//        
//    }
//    return "";
//}

-(char*)toCString:(NSString*) s {
    if(s != nil)
    {
       char *cs = (char *)[s cStringUsingEncoding: CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000)];
       return cs;
    }else{
        return "";
    }
}

+ (NSString *)stringFormatFromFullDate:(NSString *)fullDate
{
    // 输入格式
    NSDateFormatter *inputFormat = [[NSDateFormatter alloc] init];
    [inputFormat setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *inputDate = [inputFormat dateFromString:fullDate];
    // 输出格式
    NSDateFormatter *outputFormat = [[NSDateFormatter alloc] init];
    [outputFormat setDateFormat:@"HH:mm:ss"];
    NSString *formatDate = [outputFormat stringFromDate:inputDate];
    return formatDate;
}
//- (NSString *)getConfigTextByKey:(int)key
//{
//    NSString *str = [NSString stringWithFormat:@"%d",key];
//    NSString *configText = [[NSDictionary dictionaryWithContentsOfFile:LocalizedJsonPath] objectForKey:str];
//    if (configText == nil) {
//        NSLog(@"configTextByKey:%d is %@ return localizedString.string",key,configText);
//        return [self getLocalizedStringByInt:key];
//    }
//    return configText;
//}

- (BOOL)isPureNumandCharacters:(NSString *)string
{
    string = [string stringByTrimmingCharactersInSet:[NSCharacterSet decimalDigitCharacterSet]];
    if(string.length == 0 || [string isEqualToString:@"."])
    {
        return YES;
    }
    return NO;
}

+(NSString *)stringFromDateDay:(NSDate *)date
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd"];
    NSString *destDateString = [dateFormatter stringFromDate:date];
    return destDateString;
}

- (int)confirmPasswordResult
{
    //数字条件
    NSRegularExpression *tNumRegularExpression = [NSRegularExpression regularExpressionWithPattern:@"[0-9]" options:NSRegularExpressionCaseInsensitive error:nil];
    //符合数字条件的有几个字符
    int tNumMatchCount = (int)[tNumRegularExpression numberOfMatchesInString:self
                                                                options:NSMatchingReportProgress
                                                                  range:NSMakeRange(0, self.length)];
    //英文字条件
    NSRegularExpression *tLetterRegularExpression = [NSRegularExpression regularExpressionWithPattern:@"[A-Za-z]" options:NSRegularExpressionCaseInsensitive error:nil];
    //符合英文字条件的有几个字符  www.it165.net
    int tLetterMatchCount = (int)[tLetterRegularExpression numberOfMatchesInString:self options:NSMatchingReportProgress range:NSMakeRange(0, self.length)];
    
    if (tNumMatchCount == self.length)
    {
        //全部符合数字，表示没有英文
        return ConfirmPasswordResult_PureNum;
    }
    else if (tLetterMatchCount == self.length)
    {
        //全不符合英文，表示没有数字
        return ConfirmPasswordResult_PureChar;
    }
    else if (tNumMatchCount + tLetterMatchCount == self.length)
    {
        //符合英文和符合数字条件的相加等于密码长度
        return ConfirmPasswordResult_CharNumComplex;
    }
    else
    {
        //可能包含标点符号的情况，或是包含非英文的文字，这里再依照需求详细判断想呈现的错误
        return ConfirmPasswordResult_SpecialSymbol;
    }
    return -1;
}


@end
