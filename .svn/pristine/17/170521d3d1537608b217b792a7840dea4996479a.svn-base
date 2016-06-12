//
//  Archiving.m
//  CultureTrade
//
//  Created by SZFT1 on 16/2/2.
//  Copyright (c) 2016年 cheng.zongxin. All rights reserved.
//

#import "Archiving.h"
#import <objc/runtime.h>

@implementation Archiving

//解档
- (id)initWithCoder:(NSCoder *)decoder
{
    if (self = [super init]) {
        unsigned int count = 0;
        //获取类中所有成员变量名
        Ivar *ivar = class_copyIvarList([Archiving class], &count);
        for (int i = 0; i<count; i++) {
            Ivar iva = ivar[i];
            const char *name = ivar_getName(iva);
            NSString *strName = [NSString stringWithUTF8String:name];
            //进行解档取值
            id value = [decoder decodeObjectForKey:strName];
            //利用KVC对属性赋值
            [self setValue:value forKey:strName];
        }
        free(ivar);
    }
    return self;
}
//归档
- (void)encodeWithCoder:(NSCoder *)encoder
{
    unsigned int count;
    Ivar *ivar = class_copyIvarList([Archiving class], &count);
    for (int i=0; i<count; i++) {
        Ivar iv = ivar[i];
        const char *name = ivar_getName(iv);
        NSString *strName = [NSString stringWithUTF8String:name];
        //利用KVC取值
        id value = [self valueForKey:strName];
        [encoder encodeObject:value forKey:strName];
    }
    free(ivar);
}

/*
- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super init];
    if (self) {
        self.date = [aDecoder decodeObjectForKey:@"date"];
        self.highestPrice = [[aDecoder decodeObjectForKey:@"courseTeacher"] floatValue];
        self.lowestPrice = [[aDecoder decodeObjectForKey:@"lowestPrice"] floatValue];
        self.openPrice =  [[aDecoder decodeObjectForKey:@"openPrice"] floatValue];
        self.closePrice = [[aDecoder decodeObjectForKey:@"closePrice"] floatValue];
        self.maxPrice = [[aDecoder decodeObjectForKey:@"maxPrice"] floatValue];
        self.minPrice = [[aDecoder decodeObjectForKey:@"minPrice"] floatValue];
        self.volume =  [[aDecoder decodeObjectForKey:@"volume"] floatValue];
        self.maxVolume = [[aDecoder decodeObjectForKey:@"maxVolume"] floatValue];
        self.minVolume = [[aDecoder decodeObjectForKey:@"minVolume"] floatValue];
        self.MA5 = [[aDecoder decodeObjectForKey:@"MA5"] floatValue];
        self.MA10 =  [[aDecoder decodeObjectForKey:@"MA10"] floatValue];
        self.MA20 = [[aDecoder decodeObjectForKey:@"MA20"] floatValue];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    [aCoder encodeObject:self.date forKey:@"date"];
    [aCoder encodeFloat:self.highestPrice forKey:@"highestPrice"];
    [aCoder encodeFloat:self.lowestPrice forKey:@"lowestPrice"];
    [aCoder encodeFloat:self.openPrice forKey:@"openPrice"];
    [aCoder encodeFloat:self.closePrice forKey:@"closePrice"];
    [aCoder encodeFloat:self.maxPrice forKey:@"maxPrice"];
    [aCoder encodeFloat:self.minPrice forKey:@"minPrice"];
    [aCoder encodeFloat:self.volume forKey:@"volume"];
    [aCoder encodeFloat:self.maxVolume forKey:@"maxVolume"];
    [aCoder encodeFloat:self.minVolume forKey:@"minVolume"];
    [aCoder encodeFloat:self.MA5 forKey:@"MA5"];
    [aCoder encodeFloat:self.MA10 forKey:@"MA10"];
    [aCoder encodeFloat:self.MA20 forKey:@"MA20"];
    
}
*/
// 获得保存文件的路径
- (NSString *)filePath
{
    return [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingPathComponent:@"course.plist"];
}

// 调用NSKeyedArchived类的类方法：- (void)archiveRootObject:toFile:写入文件
- (void)writeToFile:(NSString *)path saveArray:(NSArray *)array
{
    [NSKeyedArchiver archiveRootObject:array toFile:path];
}

// 调用NSKeyedUnarchiver类的类方法:- (id)unarchiveObjectWithFile:读文件
- (NSArray *)readFromFile:(NSString *)path
{
    return [NSKeyedUnarchiver unarchiveObjectWithFile:path];
}

@end
