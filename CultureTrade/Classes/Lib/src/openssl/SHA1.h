//
//  SHA.h
//  SHA1
//
//  Created by SZFT1 on 14/12/8.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//

#ifndef __SHA1__SHA__
#define __SHA1__SHA__

#ifdef __cplusplus
extern "C"
{
#endif
    
#include <stdio.h>
#include <string.h>
#include <sys/types.h>	/* for u_int*_t */
/* ================ sha1.h ================ */
/*
 SHA-1 in C
 By Steve Reid <steve@edmweb.com>
 100% Public Domain
 */
typedef struct {
    u_int32_t state[5];
    u_int32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void SHA1Transform(u_int32_t state[5], const unsigned char buffer[64]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, const unsigned char* data, u_int32_t len);
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#endif /* defined(__SHA1__SHA__) */


#ifdef __cplusplus
}
#endif