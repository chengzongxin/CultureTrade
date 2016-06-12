//
//  UIImage+helper.h
//  CultureTrade
//
//  Created by SZFT1 on 15/10/22.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (helper)
-(UIImage*)scaleToSize:(CGSize)size;
+(UIImage*)createImageWithColor:(UIColor*)color;
+(UIImage *)strechImageWithImg:(NSString *)imgStr;
#pragma mark 加载全屏的图片
+ (UIImage *)fullscrennImage:(NSString *)imgName;
// 旋转图片
+ (UIImage *)image:(UIImage *)image rotation:(UIImageOrientation)orientation;


//  图片压缩，bundle中只需要存在@3x.png的图片，@2x.png的图片会根据需要自动生成到Sandbox中
/**
 *  Get image with image name
 *
 *  @param name : Image name. A image named "logo@3x.png", inputing "logo" is necessary.
 *
 *  @return UIImage
 */
+ (UIImage *)imageWithName:(NSString *)name;
/* 使用方式
 *
 * logo_huodong     :bundle中后缀为@3x.png的图片名字，使用时无需加入任何后缀，直接传入名字即可
 * imageWithName    : 为UIImage的Category方法
 *
 * UIImage *image = [UIImage imageWithName:@"logo_huodong"];
 * UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake((CGRectGetWidth(self.view.bounds) - image.size.width) / 2.0, 50.0, image.size.width, image.size.height)];
 * [imageView setImage:image];
 * [self.view addSubview:imageView];
 */


@end




