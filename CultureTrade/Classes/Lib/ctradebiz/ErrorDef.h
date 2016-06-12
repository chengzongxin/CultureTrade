
#ifndef __ERRORDEF_H__
#define __ERRORDEF_H__

#include <map>
#include <string>


#define FTS_LOGIN_SUCCESS 		1

#define FTS_LOGIN_TIMEOUT 		-1
#define FTS_LOGIN_USERDISABLED	-2
#define FTS_OTHER_ERROR			-3

#define GLOBAL_RETCODE_SUCCESS	"0000"

// 数据库操作错误
#define GLOBAL_RETCODE_DBERROR	"8000"

// 当一些错误的请求出现的时候，我们最通常的对付方法就是告诉TA：对不起你权限不够。
#define GLOBAL_RETCODE_NOPERMISSION "7000"

// 系统操作员错误
#define OPTER_ERROR_EXIST		"1001"
#define OPTER_ERROR_NOTEXIST	"1002"
#define OPTER_ERROR_SAMENAME	"1003"

// 产品错误
#define PRODUCT_ERROR_EXIST		"1011"
#define PRODUCT_ERROR_NOTEXIST	"1012"
#define PRODUCT_ERROR_SAMENAME	"1013"

// 份额公司错误
#define FUNDCOMPANY_ERROR_EXIST		"1021"
#define FUNDCOMPANY_ERROR_NOTEXIST	"1022"

// 交易账号错误
#define TRDACCT_ERROR_EXIST			"1031"
#define TRDACCT_ERROR_NOTEXIST		"1032"

// 订单错误
#define ORDER_ERROR_CHECKDATAERROR	"2011"



//if (ssRetCode == GLOBAL_RETCODE_SUCCESS)	return "操作成功。";
//if (ssRetCode == GLOBAL_RETCODE_DBERROR)	return "数据库操作错误：";
//if (ssRetCode == OPTER_ERROR_EXIST)			return "操作员已存在。";
//if (ssRetCode == OPTER_ERROR_NOTEXIST)		return "操作员不存在。";
//if (ssRetCode == PRODUCT_ERROR_EXIST)			return "投资产品已存在。";
//if (ssRetCode == PRODUCT_ERROR_NOTEXIST)		return "投资产品不存在。";
//if (ssRetCode == FUNDCOMPANY_ERROR_EXIST)			return "该份额公司已存在。";
//if (ssRetCode == FUNDCOMPANY_ERROR_NOTEXIST)		return "该份额公司不存在。";
//if (ssRetCode == TRDACCT_ERROR_EXIST)			return "该交易账号已存在。";
//if (ssRetCode == TRDACCT_ERROR_NOTEXIST)		return "该交易账号不存在。";
//if (ssRetCode == GLOBAL_RETCODE_NOPERMISSION)		return "用户当前权限不支持本操作。";
//if (ssRetCode == ORDER_ERROR_CHECKDATAERROR)		return "交易申请单复核数据出错。";



#endif