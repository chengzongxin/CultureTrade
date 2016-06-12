#ifndef __NT_ERRNO_H__
#define __NT_ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
 * 错误号定义
 * 错误号为同一个整数空间, 无重复定义, 方便定位错误
 * int型函数返回值一般定义, 0表示成功, 负数表示失败, 其值为E_XXXX, E_XXXX即为错误号
 * 各模块可选择定义自己特别的错误号, 也可以选择简单使用通用的return E_ERR
 */

// generic
#define E_ERR				(-1)
#define E_BAD_PARAM			(-2)
#define E_CK_SOF			(-3)
#define E_OVERFLOW			(-4)
#define E_UNDERFLOW			(-5)
#define E_NOT_INIT			(-6)
#define E_IO_FAIL			(-7)
#define E_EMPTY				(-8)
#define E_FULL				(-9)
#define E_NOT_FOUND			(-10)
#define	E_INCONSISTENT		(-11)
#define E_TRUNC             (-12)
#define	E_DUPLICATE			(-13)
#define E_CONN_FAIL			(-14)
#define E_SEL_GRADE			(-15)
#define E_TIMEOUT			(-16)
#define E_REINIT			(-17) //重复初始化
#define E_CONNRESET			(-18)

// crypt
#define E_BAD_CRYPT_ALGO	(-101)
#define E_CRYPT_FAIL		(-102)

// state
#define E_STATE_INVALID		(-201)
#define E_STATE_REDEFINE	(-202)
#define E_STATE_UNDEFINE	(-203)
#define E_STATE_MISMATCH	(-204)
#define E_NO_HANDLER		(-205)

// lock
#define E_LOCK_BUSY			(-301)


#ifdef __cplusplus
}
#endif

#endif // __NT_ERRNO_H__


// end of file
