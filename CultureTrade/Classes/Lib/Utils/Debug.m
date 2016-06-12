//
//  Debug.m
//  iVMS
//
//  Created by XiongXiong on 11-5-30.
//  Copyright 2011 Helecomm Daniel. All rights reserved.
//

#import "Debug.h"



@implementation NSData (NSDataExtensions) 
- (BOOL) appendToFile: (NSString *) path atomically: (BOOL) flag 
{ 
    NSFileHandle* fh = [NSFileHandle fileHandleForWritingAtPath: path]; 
	if(fh == nil) {
		[[NSFileManager defaultManager] createFileAtPath:path contents:nil attributes:nil];
		fh = [NSFileHandle fileHandleForWritingAtPath:path];
	} 
    if(fh) 
		@try 
	{ 
		[fh seekToEndOfFile]; 
		[fh writeData: self];    
		[fh closeFile];    
		return YES; 
	} 
	@catch(id error) 
	{
	} 
    return NO; 
} 
@end 

#ifdef __VMS_DEBUG__

#define LOGTOFILE_MODE

void vms_log_prompt (NSString *format, ...) { 
    if (format == nil) { 
        printf("nil\n"); 
        return; 
    } 
    va_list argList; 
    va_start(argList, format); 
    NSMutableString *s = [[NSMutableString alloc] initWithFormat:format 
													   arguments:argList];
	/*
	 [s replaceOccurrencesOfString: @"%%" 
	 withString: @"%%%%" 
	 options: 0 
	 range: NSMakeRange(0, [s length])]; 
	 */
	//#ifdef LOGTOFILE_MODE 
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *logPath = [documentsDirectory stringByAppendingPathComponent:@"log.txt"];
	
    NSData *dataToWrite = [s dataUsingEncoding: NSUTF8StringEncoding]; 
    //[dataToWrite writeToFile: logPath atomically: YES]; 
	[dataToWrite appendToFile:logPath atomically:YES];
	//#else 
    printf("%s\n", [s UTF8String]);
	//#endif 
    va_end(argList); 
}

#endif