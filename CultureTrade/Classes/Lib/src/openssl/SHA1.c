/* from valgrind tests */
/* ================ sha1.c ================ */
/*
 SHA-1 in C
 By Steve Reid <steve@edmweb.com>
 100% Public Domain
 Test Vectors (from FIPS PUB 180-1)
 "abc"
 A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 A million repetitions of "a"
 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
 */
/* #define LITTLE_ENDIAN * This should be #define'd already, if true. */
/* #define SHA1HANDSOFF * Copies data before messing with it. */

#include "SHA1.h"

#define SHA1HANDSOFF


/* ================ end of sha1.h ================ */
//#include <endian.h>

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) |(rol(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#error "Endianness not defined!"
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] ^block->l[(i+2)&15]^block->l[i&15],1))
/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);
/* Hash a single 512-bit block. This is the core of the algorithm. */
void SHA1Transform(u_int32_t state[5], const unsigned char buffer[64])
{
    u_int32_t a, b, c, d, e;
    typedef union {
        unsigned char c[64];
        u_int32_t l[16];
    } CHAR64LONG16;
#ifdef SHA1HANDSOFF
    CHAR64LONG16 block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, 64);
#else
    /* The following had better never be used because it causes the
     * pointer-to-const buffer to be cast into a pointer to non-const.
     * And the result is written through.  I threw a "const" in, hoping
     * this will cause a diagnostic.
     */
    CHAR64LONG16* block = (const CHAR64LONG16*)buffer;
#endif
    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
    memset(block, 0, sizeof(block));
#endif
}

/* SHA1Init - Initialize new context */
void SHA1Init(SHA1_CTX* context)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
}

/* Run your data through this. */
void SHA1Update(SHA1_CTX* context, const unsigned char* data, u_int32_t len)
{
    u_int32_t i;
    u_int32_t j;
    j = context->count[0];
    if ((context->count[0] += len << 3) < j)
        context->count[1]++;
    context->count[1] += (len>>29);
    j = (j >> 3) & 63;
    if ((j + len) > 63) {
        memcpy(&context->buffer[j], data, (i = 64-j));
        SHA1Transform(context->state, context->buffer);
        for ( ; i + 63 < len; i += 64) {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}

/* Add padding and return the message digest. */
void SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
    unsigned i;
    unsigned char finalcount[8];
    unsigned char c;
#if 0	/* untested "improvement" by DHR */
    /* Convert context->count to a sequence of bytes
     * in finalcount.  Second element first, but
     * big-endian order within element.
     * But we do it all backwards.
     */
    unsigned char *fcp = &finalcount[8];
    for (i = 0; i < 2; i++)
    {
        u_int32_t t = context->count[i];
        int j;
        for (j = 0; j < 4; t >>= 8, j++)
            *--fcp = (unsigned char) t
            }
#else
    for (i = 0; i < 8; i++) {
        finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                         >> ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
    }
#endif
    c = 0200;
    SHA1Update(context, &c, 1);
    while ((context->count[0] & 504) != 448) {
        c = 0000;
        SHA1Update(context, &c, 1);
    }
    SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform() */
    for (i = 0; i < 20; i++) {
        digest[i] = (unsigned char)
        ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }
    /* Wipe variables */
    memset(context, 0, sizeof(*context));
    memset(&finalcount, 0, sizeof(finalcount));
}


//
//  SHA.c
//  SHA1
//
//  Created by SZFT1 on 14/12/8.
//  Copyright (c) 2014年 Shenzhen Jinzhi Software Co., Ltd. All rights reserved.
//
//
//#include "SHA.h"
//
//
//
//
///*
// 
// SHA-1   in   C
// 
// By   Steve   Reid   <steve@edmweb.com>
// 
// 100%   Public   Domain
// 
// 
// Test   Vectors   (from   FIPS   PUB   180-1)
// 
// "abc "
// 
// A9993E36   4706816A   BA3E2571   7850C26C   9CD0D89D
// 
// "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq "
// 
// 84983E44   1C3BD26E   BAAE4AA1   F95129E5   E54670F1
// 
// A   million   repetitions   of   "a "
// 
// 34AA973C   D4C4DAA4   F61EEB2B   DBAD2731   6534016F
// 
// */
//
//
///*   #define   LITTLE_ENDIAN   *   This   should   be   #define 'd   if   true.   */
//
///*   #define   SHA1HANDSOFF   *   Copies   data   before   messing   with   it.   */
//
//
//#include   <stdio.h>
//#include   <string.h>
//
//typedef   struct  {
//    
//    unsigned   long   state[5];
//    
//    unsigned   long   count[2];
//    
//    unsigned   char   buffer[64];
//    
//}   SHA1_CTX;
//
//
//void   SHA1Transform(unsigned   long   state[5],   unsigned   char   buffer[64]);
//
//void   SHA1Init(SHA1_CTX*   context);
//
//void   SHA1Update(SHA1_CTX*   context,   unsigned   char*   data,   unsigned   int   len);
//
//void   SHA1Final(unsigned   char   digest[20],   SHA1_CTX*   context);
//
//
//#define   rol(value,   bits)   (((value)   <<   (bits))   |   ((value)   >>   (32   -   (bits))))
//
//
///*   blk0()   and   blk()   perform   the   initial   expand.   */
//
///*   I   got   the   idea   of   expanding   during   the   round   function   from   SSLeay   */
//
//#ifdef   LITTLE_ENDIAN
//
//#define   blk0(i)   (block-> l[i]   =   (rol(block-> l[i],24)&0xFF00FF00)|(rol(block-> l[i],8)&0x00FF00FF))
//
//#else
//
//#define   blk0(i)   block-> l[i]
//
//#endif
//
//#define   blk(i)   (block-> l[i&15]   =   rol(block-> l[(i+13)&15]^block-> l[(i+8)&15]^block-> l[(i+2)&15]^block-> l[i&15],1))
//
//
///*   (R0+R1),   R2,   R3,   R4   are   the   different   operations   used   in   SHA1   */
//
//#define   R0(v,w,x,y,z,i)   z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
//
//#define   R1(v,w,x,y,z,i)   z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
//
//#define   R2(v,w,x,y,z,i)   z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
//
//#define   R3(v,w,x,y,z,i)   z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
//
//#define   R4(v,w,x,y,z,i)   z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);
//
//
///*   Hash   a   single   512-bit   block.   This   is   the   core   of   the   algorithm.   */
//
//
//void   SHA1Transform(unsigned   long   state[5],   unsigned   char   buffer[64])
//
//{
//    
//    unsigned   long   a,   b,   c,   d,   e;
//    
//    typedef   union   {
//        
//        unsigned   char   c[64];
//        
//        unsigned   long   l[16];
//        
//    }   CHAR64LONG16;
//    
//    CHAR64LONG16*   block;
//    
//#ifdef   SHA1HANDSOFF
//    
//    static   unsigned   char   workspace[64];
//    
//    block   =   (CHAR64LONG16*)workspace;
//    
//    memcpy(block,   buffer,   64);
//    
//#else
//    
//    block   =   (CHAR64LONG16*)buffer;
//    
//#endif
//    
//    /*   Copy   context-> state[]   to   working   vars   */
//    
//    a   =   state[0];
//    
//    b   =   state[1];
//    
//    c   =   state[2];
//    
//    d   =   state[3];
//    
//    e   =   state[4];
//    
//    /*   4   rounds   of   20   operations   each.   Loop   unrolled.   */
//    
//    R0(a,b,c,d,e,   0);   R0(e,a,b,c,d,   1);   R0(d,e,a,b,c,   2);   R0(c,d,e,a,b,   3);
//    
//    R0(b,c,d,e,a,   4);   R0(a,b,c,d,e,   5);   R0(e,a,b,c,d,   6);   R0(d,e,a,b,c,   7);
//    
//    R0(c,d,e,a,b,   8);   R0(b,c,d,e,a,   9);   R0(a,b,c,d,e,10);   R0(e,a,b,c,d,11);
//    
//    R0(d,e,a,b,c,12);   R0(c,d,e,a,b,13);   R0(b,c,d,e,a,14);   R0(a,b,c,d,e,15);
//    
//    R1(e,a,b,c,d,16);   R1(d,e,a,b,c,17);   R1(c,d,e,a,b,18);   R1(b,c,d,e,a,19);
//    
//    R2(a,b,c,d,e,20);   R2(e,a,b,c,d,21);   R2(d,e,a,b,c,22);   R2(c,d,e,a,b,23);
//    
//    R2(b,c,d,e,a,24);   R2(a,b,c,d,e,25);   R2(e,a,b,c,d,26);   R2(d,e,a,b,c,27);
//    
//    R2(c,d,e,a,b,28);   R2(b,c,d,e,a,29);   R2(a,b,c,d,e,30);   R2(e,a,b,c,d,31);
//    
//    R2(d,e,a,b,c,32);   R2(c,d,e,a,b,33);   R2(b,c,d,e,a,34);   R2(a,b,c,d,e,35);
//    
//    R2(e,a,b,c,d,36);   R2(d,e,a,b,c,37);   R2(c,d,e,a,b,38);   R2(b,c,d,e,a,39);
//    
//    R3(a,b,c,d,e,40);   R3(e,a,b,c,d,41);   R3(d,e,a,b,c,42);   R3(c,d,e,a,b,43);
//    
//    R3(b,c,d,e,a,44);   R3(a,b,c,d,e,45);   R3(e,a,b,c,d,46);   R3(d,e,a,b,c,47);
//    
//    R3(c,d,e,a,b,48);   R3(b,c,d,e,a,49);   R3(a,b,c,d,e,50);   R3(e,a,b,c,d,51);
//    
//    R3(d,e,a,b,c,52);   R3(c,d,e,a,b,53);   R3(b,c,d,e,a,54);   R3(a,b,c,d,e,55);
//    
//    R3(e,a,b,c,d,56);   R3(d,e,a,b,c,57);   R3(c,d,e,a,b,58);   R3(b,c,d,e,a,59);
//    
//    R4(a,b,c,d,e,60);   R4(e,a,b,c,d,61);   R4(d,e,a,b,c,62);   R4(c,d,e,a,b,63);
//    
//    R4(b,c,d,e,a,64);   R4(a,b,c,d,e,65);   R4(e,a,b,c,d,66);   R4(d,e,a,b,c,67);
//    
//    R4(c,d,e,a,b,68);   R4(b,c,d,e,a,69);   R4(a,b,c,d,e,70);   R4(e,a,b,c,d,71);
//    
//    R4(d,e,a,b,c,72);   R4(c,d,e,a,b,73);   R4(b,c,d,e,a,74);   R4(a,b,c,d,e,75);
//    
//    R4(e,a,b,c,d,76);   R4(d,e,a,b,c,77);   R4(c,d,e,a,b,78);   R4(b,c,d,e,a,79);
//    
//    /*   Add   the   working   vars   back   into   context.state[]   */
//    
//    state[0]   +=   a;
//    
//    state[1]   +=   b;
//    
//    state[2]   +=   c;
//    
//    state[3]   +=   d;
//    
//    state[4]   +=   e;
//    
//    /*   Wipe   variables   */
//    
//    a   =   b   =   c   =   d   =   e   =   0;
//    
//}
///*   SHA1Init   -   Initialize   new   context   */
//
//
//void   SHA1Init(SHA1_CTX*   context)
//
//{
//    
//    /*   SHA1   initialization   constants   */
//    
//    context-> state[0]   =   0x67452301;
//    
//    context-> state[1]   =   0xEFCDAB89;
//    
//    context-> state[2]   =   0x98BADCFE;
//    
//    context-> state[3]   =   0x10325476;
//    
//    context-> state[4]   =   0xC3D2E1F0;
//    
//    context-> count[0]   =   context-> count[1]   =   0;
//    
//}
//
//
///*   Run   your   data   through   this.   */
//
//
//void   SHA1Update(SHA1_CTX*   context,   unsigned   char*   data,   unsigned   int   len)
//
//{
//    
//    unsigned   int   i,   j;
//    
//    
//    j   =   (context-> count[0]   >>   3)   &   63;
//    
//    if   ((context-> count[0]   +=   len   <<   3)   <   (len   <<   3))   context-> count[1]++;
//    
//    context-> count[1]   +=   (len   >>   29);
//    
//    if   ((j   +   len)   >   63)   {
//        
//        memcpy(&context-> buffer[j],   data,   (i   =   64-j));
//        
//        SHA1Transform(context-> state,   context-> buffer);
//        
//        for   (   ;   i   +   63   <   len;   i   +=   64)   {
//            
//            SHA1Transform(context-> state,   &data[i]);
//            
//        }
//        
//        j   =   0;
//        
//    }
//    
//    else   i   =   0;
//    
//    memcpy(&context-> buffer[j],   &data[i],   len   -   i);
//    
//}
//
//
///*   Add   padding   and   return   the   message   digest.   */
//
//
//void   SHA1Final(unsigned   char   digest[20],   SHA1_CTX*   context)
//
//{
//    
//    unsigned   long   i,   j;
//    
//    unsigned   char   finalcount[8];
//    
//    
//    for   (i   =   0;   i   <   8;   i++)   {
//        
//        finalcount[i]   =   (unsigned   char)((context-> count[(i   >=   4   ?   0   :   1)]
//                                               
//                                               >>   ((3-(i   &   3))   *   8)   )   &   255);     /*   Endian   independent   */
//        
//    }
//    
//    SHA1Update(context,   (unsigned   char   *) "\200 ",   1);
//    
//    while   ((context-> count[0]   &   504)   !=   448)   {
//        
//        SHA1Update(context,   (unsigned   char   *) "\0 ",   1);
//        
//    }
//    
//    SHA1Update(context,   finalcount,   8);     /*   Should   cause   a   SHA1Transform()   */
//    
//    for   (i   =   0;   i   <   20;   i++)   {
//        
//        digest[i]   =   (unsigned   char)
//        
//        ((context-> state[i>> 2]   >>   ((3-(i   &   3))   *   8)   )   &   255);
//        
//    }
//    
//    /*   Wipe   variables   */
//    
//    i   =   j   =   0;
//    
//    memset(context-> buffer,   0,   64);
//    
//    memset(context-> state,   0,   20);
//    
//    memset(context-> count,   0,   8);
//    
//    memset(&finalcount,   0,   8);
//    
//#ifdef   SHA1HANDSOFF     /*   make   SHA1Transform   overwrite   it 's   own   static   vars   */
//    
//    SHA1Transform(context-> state,   context-> buffer);
//    
//#endif
//    
//}


/*************************************************************/


//int   main(int   argc,   char**   argv)
//{
//    int   i,   j;
//    SHA1_CTX   context;
//    unsigned   char   digest[20],   buffer[16384];
//    FILE*   file;
//    if   (argc   >   2)   {
//        puts( "Public   domain   SHA-1   implementation   -   by   Steve   Reid   <steve@edmweb.com> ");
//        puts( "Produces   the   SHA-1   hash   of   a   file,   or   stdin   if   no   file   is   specified. ");
//        exit(0);
//    }
//    if   (argc   <   2)   {
//        file   =   stdin;
//    } else   {
//        if   (!(file   =   fopen(argv[1],   "rb ")))   {
//            fputs( "Unable   to   open   file. ",   stderr);
//            exit(-1);
//        }
//    }
//    
//    SHA1Init(&context);
//    
//    while   (!feof(file))   {     /*   note:   what   if   ferror(file)   */
//        
//        i   =   fread(buffer,   1,   16384,   file);
//        
//        SHA1Update(&context,   buffer,   i);
//        
//    }
//    
//    SHA1Final(digest,   &context);
//    
//    fclose(file);
//    
//    for   (i   =   0;   i   <   5;   i++)   {
//        
//        for   (j   =   0;   j   <   4;   j++)   {
//            
//            printf( "%02X ",   digest[i*4+j]);
//            
//        }
//        
//        putchar(' ');
//        
//    }
//    
//    putchar('\n');
//    
//    exit(0);
//    
//}


//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <errno.h>
//
//#undef BIG_ENDIAN_HOST
//typedef unsigned int u32;
//
///****************
// * Rotate a 32 bit integer by n bytes
// */
//#if defined(__GNUC__) && defined(__i386__)
//static inline u32
//rol( u32 x, int n)
//{
//    __asm__("roll %%cl,%0"
//            :"=r" (x)
//            :"0" (x),"c" (n));
//    return x;
//}
//#else
//#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )
//#endif
//
//
//typedef struct {
//    u32  h0,h1,h2,h3,h4;
//    u32  nblocks;
//    unsigned char buf[64];
//    int  count;
//} SHA1_CONTEXT;
//
//
//
//void
//sha1_init(SHA1_CONTEXT *hd )
//{
//    hd->h0 = 0x67452301;
//    hd->h1 = 0xefcdab89;
//    hd->h2 = 0x98badcfe;
//    hd->h3 = 0x10325476;
//    hd->h4 = 0xc3d2e1f0;
//    hd->nblocks = 0;
//    hd->count = 0;
//}
//
//
///****************
// * Transform the message X which consists of 16 32-bit-words
// */
//static void
//transform( SHA1_CONTEXT *hd, unsigned char *data )
//{
//    u32 a,b,c,d,e,tm;
//    u32 x[16];
//    
//    /* get values from the chaining vars */
//    a = hd->h0;
//    b = hd->h1;
//    c = hd->h2;
//    d = hd->h3;
//    e = hd->h4;
//    
//#ifdef BIG_ENDIAN_HOST
//    memcpy( x, data, 64 );
//#else
//    {
//        int i;
//        unsigned char *p2;
//        for(i=0, p2=(unsigned char*)x; i < 16; i++, p2 += 4 )
//        {
//            p2[3] = *data++;
//            p2[2] = *data++;
//            p2[1] = *data++;
//            p2[0] = *data++;
//        }
//    }
//#endif
//    
//    
//#define K1  0x5A827999L
//#define K2  0x6ED9EBA1L
//#define K3  0x8F1BBCDCL
//#define K4  0xCA62C1D6L
//#define F1(x,y,z)   ( z ^ ( x & ( y ^ z ) ) )
//#define F2(x,y,z)   ( x ^ y ^ z )
//#define F3(x,y,z)   ( ( x & y ) | ( z & ( x | y ) ) )
//#define F4(x,y,z)   ( x ^ y ^ z )
//    
//    
//#define M(i) ( tm =   x[i&0x0f] ^ x[(i-14)&0x0f] \
//^ x[(i-8)&0x0f] ^ x[(i-3)&0x0f] \
//, (x[i&0x0f] = rol(tm,1)) )
//    
//#define R(a,b,c,d,e,f,k,m)  do { e += rol( a, 5 )     \
//+ f( b, c, d )  \
//+ k	      \
//+ m;	      \
//b = rol( b, 30 );    \
//} while(0)
//    R( a, b, c, d, e, F1, K1, x[ 0] );
//    R( e, a, b, c, d, F1, K1, x[ 1] );
//    R( d, e, a, b, c, F1, K1, x[ 2] );
//    R( c, d, e, a, b, F1, K1, x[ 3] );
//    R( b, c, d, e, a, F1, K1, x[ 4] );
//    R( a, b, c, d, e, F1, K1, x[ 5] );
//    R( e, a, b, c, d, F1, K1, x[ 6] );
//    R( d, e, a, b, c, F1, K1, x[ 7] );
//    R( c, d, e, a, b, F1, K1, x[ 8] );
//    R( b, c, d, e, a, F1, K1, x[ 9] );
//    R( a, b, c, d, e, F1, K1, x[10] );
//    R( e, a, b, c, d, F1, K1, x[11] );
//    R( d, e, a, b, c, F1, K1, x[12] );
//    R( c, d, e, a, b, F1, K1, x[13] );
//    R( b, c, d, e, a, F1, K1, x[14] );
//    R( a, b, c, d, e, F1, K1, x[15] );
//    R( e, a, b, c, d, F1, K1, M(16) );
//    R( d, e, a, b, c, F1, K1, M(17) );
//    R( c, d, e, a, b, F1, K1, M(18) );
//    R( b, c, d, e, a, F1, K1, M(19) );
//    R( a, b, c, d, e, F2, K2, M(20) );
//    R( e, a, b, c, d, F2, K2, M(21) );
//    R( d, e, a, b, c, F2, K2, M(22) );
//    R( c, d, e, a, b, F2, K2, M(23) );
//    R( b, c, d, e, a, F2, K2, M(24) );
//    R( a, b, c, d, e, F2, K2, M(25) );
//    R( e, a, b, c, d, F2, K2, M(26) );
//    R( d, e, a, b, c, F2, K2, M(27) );
//    R( c, d, e, a, b, F2, K2, M(28) );
//    R( b, c, d, e, a, F2, K2, M(29) );
//    R( a, b, c, d, e, F2, K2, M(30) );
//    R( e, a, b, c, d, F2, K2, M(31) );
//    R( d, e, a, b, c, F2, K2, M(32) );
//    R( c, d, e, a, b, F2, K2, M(33) );
//    R( b, c, d, e, a, F2, K2, M(34) );
//    R( a, b, c, d, e, F2, K2, M(35) );
//    R( e, a, b, c, d, F2, K2, M(36) );
//    R( d, e, a, b, c, F2, K2, M(37) );
//    R( c, d, e, a, b, F2, K2, M(38) );
//    R( b, c, d, e, a, F2, K2, M(39) );
//    R( a, b, c, d, e, F3, K3, M(40) );
//    R( e, a, b, c, d, F3, K3, M(41) );
//    R( d, e, a, b, c, F3, K3, M(42) );
//    R( c, d, e, a, b, F3, K3, M(43) );
//    R( b, c, d, e, a, F3, K3, M(44) );
//    R( a, b, c, d, e, F3, K3, M(45) );
//    R( e, a, b, c, d, F3, K3, M(46) );
//    R( d, e, a, b, c, F3, K3, M(47) );
//    R( c, d, e, a, b, F3, K3, M(48) );
//    R( b, c, d, e, a, F3, K3, M(49) );
//    R( a, b, c, d, e, F3, K3, M(50) );
//    R( e, a, b, c, d, F3, K3, M(51) );
//    R( d, e, a, b, c, F3, K3, M(52) );
//    R( c, d, e, a, b, F3, K3, M(53) );
//    R( b, c, d, e, a, F3, K3, M(54) );
//    R( a, b, c, d, e, F3, K3, M(55) );
//    R( e, a, b, c, d, F3, K3, M(56) );
//    R( d, e, a, b, c, F3, K3, M(57) );
//    R( c, d, e, a, b, F3, K3, M(58) );
//    R( b, c, d, e, a, F3, K3, M(59) );
//    R( a, b, c, d, e, F4, K4, M(60) );
//    R( e, a, b, c, d, F4, K4, M(61) );
//    R( d, e, a, b, c, F4, K4, M(62) );
//    R( c, d, e, a, b, F4, K4, M(63) );
//    R( b, c, d, e, a, F4, K4, M(64) );
//    R( a, b, c, d, e, F4, K4, M(65) );
//    R( e, a, b, c, d, F4, K4, M(66) );
//    R( d, e, a, b, c, F4, K4, M(67) );
//    R( c, d, e, a, b, F4, K4, M(68) );
//    R( b, c, d, e, a, F4, K4, M(69) );
//    R( a, b, c, d, e, F4, K4, M(70) );
//    R( e, a, b, c, d, F4, K4, M(71) );
//    R( d, e, a, b, c, F4, K4, M(72) );
//    R( c, d, e, a, b, F4, K4, M(73) );
//    R( b, c, d, e, a, F4, K4, M(74) );
//    R( a, b, c, d, e, F4, K4, M(75) );
//    R( e, a, b, c, d, F4, K4, M(76) );
//    R( d, e, a, b, c, F4, K4, M(77) );
//    R( c, d, e, a, b, F4, K4, M(78) );
//    R( b, c, d, e, a, F4, K4, M(79) );
//    
//    /* Update chaining vars */
//    hd->h0 += a;
//    hd->h1 += b;
//    hd->h2 += c;
//    hd->h3 += d;
//    hd->h4 += e;
//}
//
//
///* Update the message digest with the contents
// * of INBUF with length INLEN.
// */
//static void
//sha1_write( SHA1_CONTEXT *hd, unsigned char *inbuf, size_t inlen)
//{
//    if( hd->count == 64 ) { /* flush the buffer */
//        transform( hd, hd->buf );
//        hd->count = 0;
//        hd->nblocks++;
//    }
//    if( !inbuf )
//        return;
//    if( hd->count ) {
//        for( ; inlen && hd->count < 64; inlen-- )
//            hd->buf[hd->count++] = *inbuf++;
//        sha1_write( hd, NULL, 0 );
//        if( !inlen )
//            return;
//    }
//    
//    while( inlen >= 64 ) {
//        transform( hd, inbuf );
//        hd->count = 0;
//        hd->nblocks++;
//        inlen -= 64;
//        inbuf += 64;
//    }
//    for( ; inlen && hd->count < 64; inlen-- )
//        hd->buf[hd->count++] = *inbuf++;
//}
//
//
///* The routine final terminates the computation and
// * returns the digest.
// * The handle is prepared for a new cycle, but adding bytes to the
// * handle will the destroy the returned buffer.
// * Returns: 20 bytes representing the digest.
// */
//
//static void
//sha1_final(SHA1_CONTEXT *hd)
//{
//    u32 t, msb, lsb;
//    unsigned char *p;
//    
//    sha1_write(hd, NULL, 0); /* flush */;
//    
//    t = hd->nblocks;
//    /* multiply by 64 to make a byte count */
//    lsb = t << 6;
//    msb = t >> 26;
//    /* add the count */
//    t = lsb;
//    if( (lsb += hd->count) < t )
//        msb++;
//    /* multiply by 8 to make a bit count */
//    t = lsb;
//    lsb <<= 3;
//    msb <<= 3;
//    msb |= t >> 29;
//    
//    if( hd->count < 56 ) { /* enough room */
//        hd->buf[hd->count++] = 0x80; /* pad */
//        while( hd->count < 56 )
//            hd->buf[hd->count++] = 0;  /* pad */
//    }
//    else { /* need one extra block */
//        hd->buf[hd->count++] = 0x80; /* pad character */
//        while( hd->count < 64 )
//            hd->buf[hd->count++] = 0;
//        sha1_write(hd, NULL, 0);  /* flush */;
//        memset(hd->buf, 0, 56 ); /* fill next block with zeroes */
//    }
//    /* append the 64 bit count */
//    hd->buf[56] = msb >> 24;
//    hd->buf[57] = msb >> 16;
//    hd->buf[58] = msb >>  8;
//    hd->buf[59] = msb	   ;
//    hd->buf[60] = lsb >> 24;
//    hd->buf[61] = lsb >> 16;
//    hd->buf[62] = lsb >>  8;
//    hd->buf[63] = lsb	   ;
//    transform( hd, hd->buf );
//    
//    p = hd->buf;
//#ifdef BIG_ENDIAN_HOST
//#define X(a) do { *(u32*)p = hd->h##a ; p += 4; } while(0)
//#else /* little endian */
//#define X(a) do { *p++ = hd->h##a >> 24; *p++ = hd->h##a >> 16;	 \
//*p++ = hd->h##a >> 8; *p++ = hd->h##a; } while(0)
//#endif
//    X(0);
//    X(1);
//    X(2);
//    X(3);
//    X(4);
//#undef X
//}