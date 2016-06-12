//
//  AsyncURLConnection.m
//  CultureTrade
//
//  Created by SZFT1 on 15/10/15.
//  Copyright (c) 2015年 cheng.zongxin. All rights reserved.
//

#import "AsyncURLConnection.h"

@implementation ASyncURLConnection


+ (id)request:(NSString *)requestUrl completeBlock:(CompleteBlock_t)compleBlock errorBlock:(ErrorBlock_t)errorBlock;
{
    return [[self alloc] initWithRequest:requestUrl completeBlock:compleBlock errorBlock:errorBlock];
}

- (id)initWithRequest:(NSString *)requestUrl completeBlock:(CompleteBlock_t)compleBlock errorBlock:(ErrorBlock_t)errorBlock
{
    NSURL *url = [NSURL URLWithString:requestUrl];
    NSURLRequest *request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:0];
    
    if (self = [super initWithRequest:request delegate:self startImmediately:NO]) {
        data_ = [[NSMutableData alloc] init];
        completeBlock_ = [compleBlock copy];  //p = &function;
        errorBlock_ = [errorBlock copy];
        
        [self start];
    }
    
    return self;
}

#pragma mark- NSURLConnectionDataDelegate

-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    
    [data_ setLength:0];
}

-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [data_ appendData:data];
}

-(void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    completeBlock_(data_);  // *p(data)
}

-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    
    errorBlock_(error);
}


@end
