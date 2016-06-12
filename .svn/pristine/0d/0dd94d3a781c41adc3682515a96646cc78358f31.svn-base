#ifndef __NT_MACRO_H__
#define __NT_MACRO_H__


#ifdef __cplusplus
extern "C" {
#endif


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAKEWORD
#define MAKEWORD(a, b) ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xff))) << 8))
#endif // MAKEWORD

#ifndef MAKELONG
#define MAKELONG(a, b)      ((uint32_t)(((uint16_t)(((uint32_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint32_t)(b)) & 0xffff))) << 16))
#endif // MAKELONG

#ifndef LOWORD
#define LOWORD(l) ((uint16_t)(((uint32_t)(l)) & 0xffff))
#endif // LOWORD

#ifndef HIWORD
#define HIWORD(l) ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))
#endif // HIWORD

#ifndef LOBYTE
#define LOBYTE(w) ((uint8_t)(((uint32_t)(w)) & 0xff))
#endif // LOBYTE

#ifndef HIBYTE
#define HIBYTE(w) ((uint8_t)((((uint32_t)(w)) >> 8) & 0xff))
#endif // HIBYTE


#ifndef LO4BIT
#define LO4BIT(w) ((uint8_t)(((uint32_t)(w)) & 0xf))
#endif // LOBYTE

#ifndef HI4BIT
#define HI4BIT(w) ((uint8_t)((((uint32_t)(w)) >> 4) & 0xf))
#endif // HI4BIT


#ifndef GET_ENCRYPT_FLAG
#define GET_ENCRYPT_FLAG LO4BIT
#endif // GET_ENCRYPT_FLAG

#ifndef GET_COMPRESS_FLAG
#define GET_COMPRESS_FLAG HI4BIT
#endif // GET_COMPRESS_FLAG


// 对四个字节设置低4bit
#ifndef SETLO4BIT
#define SETLO4BIT(w, v) \
	w = (uint32_t)((uint32_t)((((uint32_t)(w)) & 0xfffffff0) | ((uint32_t)(((uint32_t)(v)) & 0x0000000f))))
#endif // SETLO4BIT


// 对四个字节设置高4bit
#ifndef SETHI4BIT
#define SETHI4BIT(w, v) \
	w = (uint32_t)((uint32_t)((((uint32_t)(w)) & 0xffffff0f) | (uint32_t)(((uint32_t)(((uint32_t)(v)) & 0x0000000f)) << 4)))
#endif // SETHI4BIT


// 对一个字节设置低4bit
#ifndef SETBYLO4BIT
#define SETBYLO4BIT(w, v) \
	w = (uint8_t)((uint32_t)((((uint32_t)(w)) & 0xfffffff0) | ((uint32_t)(((uint32_t)(v)) & 0x0000000f))))
#endif // SETBYLO4BIT


// 对一个字节设置高4bit
#ifndef SETBYHI4BIT
#define SETBYHI4BIT(w, v) \
	w = (uint8_t)((uint32_t)((((uint32_t)(w)) & 0xffffff0f) | (uint32_t)(((uint32_t)(((uint32_t)(v)) & 0x0000000f)) << 4)))
#endif // SETBYHI4BIT

#ifndef SET_ENCRYPT_FLAG
#define SET_ENCRYPT_FLAG SETBYLO4BIT
#endif // SET_ENCRYPT_FLAG

#ifndef SET_COMPRESS_FLAG
#define SET_COMPRESS_FLAG SETBYHI4BIT
#endif // SET_COMPRESS_FLAG

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif // ARRAYSIZE


#ifndef OFFSETOF
#define OFFSETOF(type, member)	((uint32_t)&((type *)0)->member)
#endif //OFFSETOF


//CONTAINER_RECORD
#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member) \
	((type *)((uint8_t *)(ptr) - OFFSETOF(type, member)))
#endif //CONTAINER_OF


#ifndef FIELD_SIZEOF
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#endif // FIELD_SIZEOF


#ifndef MIN
#define MIN(_x, _y) (_x) < (_y) ? (_x) : (_y)
#endif // MIN

#ifndef MAX
#define MAX(_x, _y)	(_x) > (_y) ? (_x) : (_y)
#endif // MAX


// 检查无符号整数是否可能存在符号溢出
#ifndef CK_U32_SOF
#define CK_U32_SOF(u32) ((int32_t)(u32) < 0)
#endif // CK_U32_SOF

#ifndef CK_U64_SOF
#define CK_U64_SOF(u32) ((int64_t)(u32) < 0)
#endif // CK_U64_SOF


#ifndef CAT_TOKEN
#define CAT_TOKEN_1(t1,t2) t1##t2
#define CAT_TOKEN(t1,t2) CAT_TOKEN_1(t1,t2)
#endif // CAT_TOKEN

#ifndef COMPILE_ASSERT
#define COMPILE_ASSERT(x) \
	enum {CAT_TOKEN (comp_assert_at_line_, __LINE__) = 1 / !!(x) };
#endif // COMPILE_ASSERT

#ifndef CHECK_SIZE
#define CHECK_SIZE(type, size) \
	extern int sizeof_##type##_is_##size [!!(sizeof(type)==(size_t)size) - 1]
#endif //CHECK_SIZE

#ifndef CHECK_OFFSET
#define CHECK_OFFSET(type, member, value) \
	extern int offset_of_##member##_in_##type##_is_##value \
	[!!(offsetof(type,member)==((size_t)(value))) - 1]
#endif//CHECK_OFFSET

#define STRING(X) #X
#define STRVAL(X) STRING(X)
#define NT_UNUSED(x) ((x) = (x))

#define CHECK_RET(expr) \
	do{ \
		int iRet = (expr); \
		if(0 != iRet) \
			return iRet; \
	}while(0)

#define CHECK_RET1(expr, ret) \
	do{ \
		int iRet = (expr); \
		if(0 != iRet) \
			return ret; \
	}while(0)


//dwVal in [dwMin, dwMax]
#define RANGE32_IN(dwVal, dwMin, dwMax) \
	((uint32_t)dwMin <= (uint32_t)dwVal && (uint32_t)dwVal <= (uint32_t)dwMax)

//dwVal out [dwMin, dwMax]
#define RANGE32_OUT(dwVal, dwMin, dwMax) \
	((uint32_t)dwMin > (uint32_t)dwVal || (uint32_t)dwVal > (uint32_t)dwMax)

//wVal in [wMin, wMax]
#define RANGE16_IN(wVal, wMin, wMax) \
	((uint16_t)wMin <= (uint16_t)wVal && (uint16_t)wVal <= (uint16_t)wMax)

//wVal out [wMin, wMax]
#define RANGE16_OUT(wVal, wMin, wMax) \
	((uint16_t)wMin > (uint16_t)wVal || (uint16_t)wVal >= (uint16_t)wMax)



//#define DUMP_FUNC const char *
#define DUMP_INIT \
	static char __sDumpBuf[4096]; \
	char *__pCur = __sDumpBuf; \
	int __iLeft = sizeof(__sDumpBuf); \
	int __iRet = 0
#define DUMP_LINE(fmt, ...) \
	do { \
		if ((__iRet = snprintf(__pCur, __iLeft, fmt , ##__VA_ARGS__)) < 0) return __sDumpBuf; \
		else { __pCur += __iRet; __iLeft -= __iRet; } \
	} while (0)
#define DUMP_END return __sDumpBuf


#ifdef NDEBUG
#define DECLARE_ASSERT(x) \
	int CAT_TOKEN(___dummy, __LINE__) __attribute__((unused))
#else
#define DECLARE_ASSERT(x) \
	int CAT_TOKEN(___dummy, __LINE__) __attribute__((unused)) = ({assert(x); 0;})
#endif // DECLARE_ASSERT



#ifdef __NT_WIN__
#if 1
#define __BYTE_ORDER 1
#define __BIG_ENDIAN BIGENDIAN
#else
#define __BYTE_ORDER 0
#define __LITTLE_ENDIAN LITTLEENDIAN
#endif
#endif // __NT_WIN__


#ifndef htonll
#if __BYTE_ORDER == __BIG_ENDIAN // __LITTLE_ENDIAN
#define htonll(x) x
#else
#define htonll(x) \
	((x) << 56 | (x) >> 56 | \
	((x) & (uint64_t)0x000000000000ff00ULL) << 40 | \
	((x) & (uint64_t)0x0000000000ff0000ULL) << 24 | \
	((x) & (uint64_t)0x00000000ff000000ULL) << 8 | \
	((x) & (uint64_t)0x000000ff00000000ULL) >> 8 | \
	((x) & (uint64_t)0x0000ff0000000000ULL) >>24 | \
	((x) & (uint64_t)0x00ff000000000000ULL) >> 40)
#endif // __BIG_ENDIAN
#endif // htonll

#ifndef ntohll
#define ntohll htonll
#endif // ntohll


#ifdef __cplusplus
}
#endif


#endif // __NT_MACRO_H__

// end of file
