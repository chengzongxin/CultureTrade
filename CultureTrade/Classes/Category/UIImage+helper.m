//
//  UIImage+helper.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "UIImage+helper.h"
#define BASE_IMG_WIDTH  (1242)
#define BASE_IMG_HEIGHT (2208)

#define SUFFIX_3X_PNG   @"@3x.png"
#define SUFFIX_2X_PNG   @"@2x.png"

@implementation UIImage (helper)

-(UIImage*)scaleToSize:(CGSize)size
{
    // 创建一个bitmap的context
    // 并把它设置成为当前正在使用的context
    //Determine whether the screen is retina
    if([[UIScreen mainScreen] scale] == 2.0){
        UIGraphicsBeginImageContextWithOptions(size, NO, 2.0);
    }else{
        UIGraphicsBeginImageContext(size);
    }
    // 绘制改变大小的图片
    [self drawInRect:CGRectMake(0, 0, size.width, size.height)];
    // 从当前context中创建一个改变大小后的图片
    UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    // 使当前的context出堆栈
    UIGraphicsEndImageContext();
    // 返回新的改变大小后的图片
    return scaledImage;
}

// 根据传入的uicolor返回一个uiimage，只需要在外部指定frame即可，在这里传入cgrect没有效果
+(UIImage*) createImageWithColor:(UIColor*)color
{
    CGRect rect=CGRectMake(0.0f, 0.0f, 1.0f, 1.0f);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);
    UIImage *theImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return theImage;
}

+(UIImage *)strechImageWithImg:(NSString *)imgStr
{
    UIImage *img = [UIImage imageNamed:imgStr];
    img = [img stretchableImageWithLeftCapWidth:img.size.width/2 topCapHeight:img.size.height/2];
    return img;
}

#pragma mark 加载全屏的图片
// new_feature_1.png
+ (UIImage *)fullscrennImage:(NSString *)imgName
{
    // 1.如果是iPhone5，对文件名特殊处理
    if (IS_IPHONE5) {
        // 1.1.获得文件拓展名
        NSString *ext = [imgName pathExtension];
        
        // 1.2.删除最后面的扩展名
        imgName = [imgName stringByDeletingPathExtension];
        
        // 1.3.拼接-568h@2x
        imgName = [imgName stringByAppendingString:@"-568h@2x"];
        
        // 1.4.拼接扩展名
        imgName = [imgName stringByAppendingPathExtension:ext];
    }
    
    // 2.加载图片
    return [self imageNamed:imgName];
}

+ (UIImage *)image:(UIImage *)image rotation:(UIImageOrientation)orientation
{
    long double rotate = 0.0;
    CGRect rect;
    float translateX = 0;
    float translateY = 0;
    float scaleX = 1.0;
    float scaleY = 1.0;
    
    switch (orientation) {
        case UIImageOrientationLeft:
            rotate = M_PI_2;
            rect = CGRectMake(0, 0, image.size.height, image.size.width);
            translateX = 0;
            translateY = -rect.size.width;
            scaleY = rect.size.width/rect.size.height;
            scaleX = rect.size.height/rect.size.width;
            break;
        case UIImageOrientationRight:
            rotate = 3 * M_PI_2;
            rect = CGRectMake(0, 0, image.size.height, image.size.width);
            translateX = -rect.size.height;
            translateY = 0;
            scaleY = rect.size.width/rect.size.height;
            scaleX = rect.size.height/rect.size.width;
            break;
        case UIImageOrientationDown:
            rotate = M_PI;
            rect = CGRectMake(0, 0, image.size.width, image.size.height);
            translateX = -rect.size.width;
            translateY = -rect.size.height;
            break;
        default:
            rotate = 0.0;
            rect = CGRectMake(0, 0, image.size.width, image.size.height);
            translateX = 0;
            translateY = 0;
            break;
    }
    
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    //做CTM变换
    CGContextTranslateCTM(context, 0.0, rect.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextRotateCTM(context, rotate);
    CGContextTranslateCTM(context, translateX, translateY);
    
    CGContextScaleCTM(context, scaleX, scaleY);
    //绘制图片
    CGContextDrawImage(context, CGRectMake(0, 0, rect.size.width, rect.size.height), image.CGImage);
    
    UIImage *newPic = UIGraphicsGetImageFromCurrentImageContext();
    
    return newPic;
}



// 获取图片
+ (UIImage *)imageWithName:(NSString *)name
{
    if([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        return [UIImage imageNamed:name];
    }
    else
    {
        NSAssert(NO == [name hasSuffix:SUFFIX_3X_PNG], @"File name suffix @3x.png is unnecessary!");
        
        NSString *srcImgName = [name stringByAppendingString:SUFFIX_3X_PNG];
        
        NSString *desImgName = [UIImage desImageName:[UIImage getImageName:name] withExt:SUFFIX_2X_PNG];
        
        // Path
        NSString *cacheDir = [NSHomeDirectory() stringByAppendingPathComponent:@"/Library/Caches/Resources/images/"];
        
        // 判断存放图片的文件夹是否存在,不存在则创建对应文件夹
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        BOOL isDir = NO;
        BOOL isDirExist = [fileManager fileExistsAtPath:cacheDir isDirectory:&isDir];
        if(!(isDirExist && isDir))
        {
            NSError *error = nil;
            BOOL bCreateDir = [fileManager createDirectoryAtPath:cacheDir withIntermediateDirectories:YES attributes:nil error:&error];
            
            if(!bCreateDir)
            {
                NSLog(@"Create Directory Failed! : %@", error.description);
                
                return nil;
            }
        }
        
        NSString *desPath = [cacheDir stringByAppendingPathComponent:desImgName];
        
        UIImage *desImage = [UIImage imageWithContentsOfFile:desPath];
        
        if(!desImage)
        {
            // 如果从Cache中没有取到图片
            if([UIImage compressImage:srcImgName compressionQuality:0.8 desPath:desPath])
            {
                // 压缩成功
                desImage = [UIImage imageWithContentsOfFile:desPath];
            }
            else
            {
                desImage = nil;
            }
        }
        
        return desImage;
    }
}

// 压缩图片
+ (BOOL)compressImage:(NSString *)name compressionQuality:(CGFloat)compressionQuality desPath:(NSString *)desPath
{
    if(name == nil || desPath == nil)
    {
        return NO;
    }
    
    UIImage *srcImage = [UIImage imageNamed:name];
    
    if(srcImage == nil)
    {
        return NO;
    }
    
    // 根据屏幕尺寸设置图片尺寸
    UIScreen *mainScreen = [UIScreen mainScreen];
    
    // Default is Portrait or PortraitUpsideDown
    CGSize baseImgSize = CGSizeMake(BASE_IMG_WIDTH, BASE_IMG_HEIGHT);
    UIDeviceOrientation orientation = [UIDevice currentDevice].orientation;
    if(orientation == UIDeviceOrientationLandscapeLeft || orientation == UIDeviceOrientationLandscapeRight)
    {
        // LandscapeLeft or LandscapeRight
        baseImgSize = CGSizeMake(BASE_IMG_HEIGHT, BASE_IMG_WIDTH);
    }
    
    CGSize newSize = CGSizeMake(srcImage.size.width * srcImage.scale * (CGRectGetWidth(mainScreen.bounds) * mainScreen.scale) / baseImgSize.width, srcImage.size.height * srcImage.scale * (CGRectGetHeight(mainScreen.bounds) * mainScreen.scale) / baseImgSize.height);
    
    // Reset image
    UIImage *newImage = [UIImage imageWithImage:srcImage scaleToSize:newSize];
    
    // 对图片进行压缩
    NSData *imageData = UIImageJPEGRepresentation(newImage, compressionQuality);
    
    // 保存新图片
    return [imageData writeToFile:desPath atomically:YES];
}

// 重设图片的全名(带扩展名)
+ (NSString *)desImageName:(NSString *)srcImageName withExt:(NSString *)extName
{
    return [srcImageName stringByAppendingString:extName];
}

// 获取图片的名字(不带扩展名)
+ (NSString *)getImageName:(NSString *)srcImage
{
    if(srcImage)
    {
        NSArray *tempArray = [srcImage componentsSeparatedByString:@"."];
        
        if(tempArray)
        {
            // 有.分割的文件名
            if([tempArray count] > 1)
            {
                NSString *extName = [tempArray lastObject];
                
                // 严格判断文件的扩展名
                if([extName isEqualToString:@"png"] || [extName isEqualToString:@"jpg"] || [extName isEqualToString:@"jpeg"])
                {
                    return [srcImage substringWithRange:NSMakeRange(0, srcImage.length - (extName.length + 1))];
                }
                else
                {
                    return nil;
                }
            }
            else
            {
                return srcImage;
            }
        }
        else
        {
            return srcImage;
        }
    }
    return nil;
}

// 对图片尺寸进行重新设置
+ (UIImage *)imageWithImage:(UIImage*)image scaleToSize:(CGSize)newSize
{
    UIGraphicsBeginImageContext(newSize);
    
    [image drawInRect:CGRectMake(0.0, 0.0, newSize.width, newSize.height)];
    
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    return newImage;
}

@end
