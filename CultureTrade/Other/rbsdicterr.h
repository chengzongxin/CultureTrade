#ifndef __SSCC_RBSDICTERR_H__
#define __SSCC_RBSDICTERR_H__

#include "mytoolkit.h"


#define ECode_Ok                                0
#define ECode_FunctionId_NotExist               (-1001)         ///<  未知的功能号.
#define ECode_NotSupport_Error                  (-1002)         ///<  不支持的功能.
#define ECode_InputParam_EmptyOrError           (-1003)         ///<  输入参数为空或错误.
#define ECode_MultiSecuCompanyNotSupport        (-1004)         ///<  不支持多个证券公司.



#define ECode_NotTradeTimeRange                 (-1005)         ///<  不在交易时间范围.
#define ECode_ObjHasExisted                     (-1006)         ///<  对象已经存在.
#define ECode_ObjNotExisted                     (-1007)         ///<  对象不存在.
#define ECode_OperRightsError                   (-1008)         ///<  操作权限错误.
#define ECode_NoonForbiddenCancelTimeRange      (-1009)         ///<  中午不许撤单
#define ECode_SysSession_Invalid                (-1101)         ///<  系统会话无效.

#define ECode_SysEncrypt_Error                  (-1102)         ///<  系统加密错误.
#define ECode_SysDecrypt_Error                  (-1103)         ///<  系统解密错误.

#define ECode_SecuComp_NotExist                 (-2001)         ///<  未找到公司
#define ECode_SecuCompStatus_NotExistInDict     (-2002)         ///<  在字典中未找到公司状态信息
#define ECode_SecuCompStatus_NotEnable          (-2003)         ///<  公司状态信息不是Enable状态.

#define ECode_Branch_NotExist                   (-2005)         ///<  未找到营业部

#define ECode_Customer_IsEmpty					(-2006)			///<  客户号为空
#define ECode_Orig_Passwd_Error					(-2007)			////< 原始密码错误
#define ECode_Customer_NotExist                 (-2011)         ///<  未找到客户信息
#define ECode_CustomerStatus_NotExistInDict     (-2012)         ///<  在字典中未找到客户状态信息
#define ECode_CustomerStatus_NotEnable          (-2013)         ///<  客户状态信息不是Enable状态.

#define ECode_Cust_FundAcct_NotExist            (-2021)         ///<  未找到资金帐号.
#define ECode_Cust_FundAcctType_NotExist        (-2022)         ///<  对应资金帐号类别(普通或信用)不存在.
#define ECode_Cust_ShareHolder_NotExist         (-2023)         ///<  股东帐号找不到.
#define ECode_Market_NotExist                   (-2024)         ///<  市场找不到.

#define ECode_FundAcctStatus_NotExistInDict     (-2031)         ///<  在字典中未找到资金帐号状态信息
#define ECode_FundAcctStatus_NotEnable          (-2032)         ///<  资金帐号状态信息不是Enable状态.

#define ECode_ShareHolderStatus_NotExistInDict  (-2033)         ///<  在字典中未找到股东代码状态信息
#define ECode_ShareHolderStatus_NotEnable       (-2034)         ///<  股东代码状态信息不是Enable状态.

#define ECode_FundAsset_NotExist                (-2035)         ///<  未找到资金帐号.
#define ECode_FundAssetStatus_NotExistInDict    (-2036)         ///<  在字典中未找到资金资产状态信息
#define ECode_FundAssetStatus_NotEnable         (-2037)         ///<  资金资产状态信息不是Enable状态.
#define ECode_SubscriptQuery_DataError			(-2038)			///<配号查询日期错误
#define ECode_StkTemplate_NotExistInDict        (-2041)         ///<  没有在字典中找到证券模块.


#define ECode_CustLogin_Pwd_Error               (-3011)         ///<  客户登录密码错误.
#define ECode_CustSessionId_Invalid             (-3012)         ///<  客户SessionId无效.
#define ECode_CustIndex_Invalid                 (-3013)         ///<  客户下标无效.
#define ECode_Cust_NoLogin                      (-3014)         ///<  客户未登录.
#define ECode_CustLogin_ErrTooTimes_Error       (-3015)         ///<  客户登录错误次数太多.
#define ECode_CustLogin_Logined_Error			(-3016)         ///<  客户已登陆

#define ECode_SecuLimit_OpIsForbid              (-3020)         ///<  由于股东限制该操作被禁止.
#define ECode_FundLimit_OpIsForbid              (-3021)         ///<  由于资金限制该操作被禁止.
#define ECode_TrdRight_OpIsForbid               (-3022)         ///<  该交易权限不允许.
#define ECode_FundTrdCtrl_OpIsForbid            (-3023)         ///<  该交易限定不允许.
#define ECode_SecuRegFlag_OpIsForbid            (-3024)         ///<  该股东账户未指定交易

#define ECode_FundMoneyAsset_NotExist           (-3031)         ///<  资金货币资产不存在.


#define ECode_TradeType_Err                     (-4001)         ///<  交易类型错误.
#define ECode_StkCodeInMarket_NotExistOrPause   (-4002)         ///<  在指定市场的证券代码不存在或停盘.
#define ECode_StockIsNotOpenStatus_Error        (-4003)         ///<  证券不是正常开盘状态.
#define ECode_PriceIsBeyondRiseFallLimit_Error  (-4004)         ///<  价格超过涨跌停价.
#define ECode_QtyIsTooBigSmall_Error            (-4005)         ///<  数量太大或太小.
#define ECode_BuySaleQtyUnit_Error              (-4006)         ///<  买卖数量单位错误.
#define ECode_UnknowBuySaleType_Error           (-4007)         ///<  未知的买卖类型.
#define ECode_BuySaleInMktPriceAtUnlimitPriceStk_Error  (-4008) ///<  没有涨跌幅限制的股票不能做市价委托.
#define ECode_FundAssetNotEnough_Error          (-4009)         ///<  资金资产不足.
#define ECode_StkAssetNotEnoughOrNotExist_Error (-4010)         ///<  股份资产不足或不存在.
#define ECode_OrigOrderNotFound                 (-4011)         ///<  原始订单没有找到.
#define ECode_OrigOrderInfo_NotSameAs_Input     (-4012)         ///<  原始订单信息与输入信息不一致.
#define ECode_StatusNotAllowCancel              (-4013)         ///<  当前状态不允许撤单.
#define ECode_BuySaleTypeNotAllow_Error         (-4014)         ///<  当前买卖类型不允许.
#define ECode_PriceNotFixPrice_Error            (-4015)         ///<  价格不是系统定价错误.
#define ECode_WarrantExerDate_Error             (-4016)         ///<  权证行权日期错误.
#define ECode_Price_Error                       (-4017)         ///<  价格错误.
#define ECode_Trade_Too_Many_Holders			(-4018)			///<  当前交易人数超过系统设置
#define ECode_Trade_Req_NewStk_Forbiden			(-4019)			////< 该产品为非申购状态，禁止申购
#define ECode_Trade_Req_YSG_NewStk_Forbiden		(-4020)			////< 该产品已经申购，禁止申购
#define ECode_Trade_Sys_Close					(-4028)			///<  交易系统已关闭，禁止本次交易！
#define ECode_Trade_ProductClose				(-4029)			////  产品停止交易

#define ECode_Trade_HistoryQueryClose			(-4030)			////  交易期间不允许查询历史数据

#define ECode_Trade_Order_isValid				(-5002)				//单据无效
#define ECode_Command_isValid					(-5003)				//无效操作
#define ECode_Error_ApplyNew_NotCancel			(-5004)				//新股申购无法撤单

#define ECode_User_Margin_Not_Full				(-5013)				//用户保证金不足够
#define ECode_Database_Sql_Error				(-5014)				//数据库SQL语句出错

#define ECode_TradingDay_isValid				(-5018)				//交易日无效
#define ECode_Reckon_Price_Check_Error			(-5019)				//结算价格检查失败
#define ECode_TradingDay_Has_Expired			(-5020)				//交易日已过期

#define ECode_Database_Connect_Error			(-5024)				//数据库连接出错

#define ECode_In_Money_Less_than_Min_Error		(-5026)				//入金金额小于每笔最小入金金额
#define ECode_Market_Close						(-5027)				//市场关闭，禁止交易
#define ECode_Account_Have_Contract				(-5028)				//客户已签约，禁止重复
#define ECode_Account_Have_Stop_Contract		(-5029)				//客户已解约，禁止重复
#define ECode_Account_Contract_Not_Fit			(-5030)				//客户不符合签约条件(或解约)，请检查

#define ECode_Out_Money_Less_than_Min_Error		(-5032)				//出金金额小于每笔最小出金金额
#define ECode_Out_Money_Count_overflow_EveryDay (-5033)				//出入金超过每日可入出资金次数
#define ECode_Member_Status_Freeze_Not_Order	(-5034)				//会员冻结状态，禁止下单
#define ECode_Trade_Order_Have_Finish			(-5038)				//单据已经处理(审核、支付或作废), 请求重复
#define ECode_Trade_Pwd_Error					(-5039)				//交易密码不正确
#define ECode_Member_Have_Valid_Account			(-5040)				//会员下有有效的账户，禁止签约（或解约）
#define ECode_Member_Not_Contract				(-5041)				//会员未签约，禁止出入金
#define ECode_Money_Pwd_Error					(-5042)				//资金密码不正确
#define ECode_Not_Find_Member_Info				(-5043)				//找不到相关的会员信息
#define ECode_Not_Find_Operator_Info			(-5044)				//无此操作员
#define ECode_Symtem_Reckon_Running				(-5045)				//系统正在结算
#define ECode_Auto_Mode_Can_not_Manual_Reckon   (-5046)				//自动模式不能手动结算
#define ECode_Open_Market_Stop_Reckon		    (-5047)				//市场开市，禁止结算

#define ECode_In_Out_Money_Less_Zero_Error		(-5049)				//入出金金额无效
#define ECode_Payment_Interface_Not_Support		(-5050)				//三方支付通道不支持此接口
#define ECode_Customer_Not_Bind_Payment_Interface	(-5051)			//客户未绑定支付通道
#define ECode_Account_Not_Contract					(-5052)			//账户未签约，禁止出入金
#define ECode_Error_PayChannelCode				 (-5053)			//通道不存在或不正确
#define ECode_Error_BankMchNo					 (-5054)			//商户号不正确
#define ECode_SerialNumber_Not_Exist			 (-5055)			//流水号不能为空或不存在
#define ECode_SerialNumber_Exist				 (-5057)			//存在不同操作类型流水号
#define ECode_Account_Have_Cross_ValidateDate	 (-5058)			//账户已过期

#define ECode_Clear_File_Have_Created			 (-5061)			//当前交易日清算文件已经生成
#define ECode_Upload_ClearFile_Error			 (-5062)			//上传清算文件失败
#define ECode_Bank_Customer_No_Is_Not_Null		 (-5063)			//银行签约号不能为空
#define ECode_Customer_Status_Stop				 (-5064)			//账户已停止使用
#define ECode_Customer_Status_Freeze			 (-5065)			//账户冻结状态禁止此项操作
#define ECode_Not_In_Sign_Time					 (-5066)			//非出入金时间段，禁止签约、解约、出入金
#define ECode_BankAccount_Error					 (-5067)			//银行账户名或账户不正确
#define ECode_IdCode_Not_Exist					 (-5068)			//证件不存在或错误
#define ECode_Open_Market_Stop_Reload_System	 (-5069)			//市场开市状态，禁止加载系统
#define ECode_Not_In_In_Time					 (-5070)			//非入金时间段，禁止入金
#define ECode_Not_In_Out_Time					 (-5071)			//非出金时间段，禁止出金
#define ECode_Reckon_Get_Last_Avg_Price_Error	 (-5072)			//结算取最后10分钟均价失败
#define ECode_Out_Money_Overflow_Amount			 (-5073)			//出金超过每日可出资金

#define ECode_Limit_Cancel_Order                 (-5074)            //非撤单时间,禁止撤单
#define ECode_Sequence_Table_Failed              (-5075)            //读序列表失败
#endif


