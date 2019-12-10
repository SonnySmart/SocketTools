#ifndef __Protocol_H__
#define __Protocol_H__

#include "MobileClientKernel\Define.h"

#pragma pack(1)

#define VALIDATE					"11E9E77D-C628-4FD6-AC60-91A541DBD5C1"

#define MDM_MB_LOGON				100	//广场登录

#define MDM_GR_LOGON				1									//登录信息

#define SUB_MB_LOGON_ACCOUNTS		2	//帐号登录
#define SUB_MB_LOGON_OTHERPLATFORM	4	//微信登录

#define SUB_GR_LOGON_MOBILE			2									//手机登录

#define LEN_MD5						33	//加密密码
#define LEN_ACCOUNTS				32	//帐号长度
#define LEN_PASSWORD				33	//密码长度
#define LEN_MACHINE_ID				33	//序列长度

#define SUB_MB_LOGON_SUCCESS		100	//登录成功
#define SUB_MB_LOGON_FAILURE		101	//登录失败
#define SUB_MB_UPDATE_NOTIFY		200	//升级提示


//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

																		//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

																		//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonOtherPlatform
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	BYTE							cbGender;							//用户性别
	BYTE							cbPlatformID;						//平台编号
	TCHAR							szUserUin[LEN_USER_UIN];			//用户Uin
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//用户头像
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szAgentID[LEN_ACCOUNTS];			//代理标识

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//手机登录
struct CMD_GR_LogonMobile
{
	//版本信息
	WORD							wGameID;							//游戏标识
	DWORD							dwProcessVersion;					//进程版本

	//桌子区域
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //房间密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwCustomID;							//自定头像
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwSpreaderID;						//推广ID
	DWORD							dwExperience;						//经验数值
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//用户头像
	TCHAR							szAliPayAcccount[30];				//支付宝账户
	TCHAR							szBinkID[20];						//银行卡账户
	TCHAR							szDynamicPass[LEN_PASSWORD];		//动态密码

	//财富信息
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行	

	//扩展信息
	BYTE							cbInsureEnabled;					//使能标识
	BYTE							cbIsAgent;							//代理标识
	BYTE							cbMoorMachine;						//锁定机器

	//约战房相关
	int								TodayAlmsCount;						//每日低保已领取次数
	DWORD							dwLockServerID;						//锁定房间
	DWORD							dwKindID;							//游戏类型
};

#pragma pack()


#endif // !__Protocol_H__

