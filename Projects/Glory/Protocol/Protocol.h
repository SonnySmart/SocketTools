#ifndef __Protocol_H__
#define __Protocol_H__

#include "MobileClientKernel\Define.h"

#pragma pack(1)

#define VALIDATE					"11E9E77D-C628-4FD6-AC60-91A541DBD5C1"

#define MDM_MB_LOGON				100	//�㳡��¼

#define MDM_GR_LOGON				1									//��¼��Ϣ

#define SUB_MB_LOGON_ACCOUNTS		2	//�ʺŵ�¼
#define SUB_MB_LOGON_OTHERPLATFORM	4	//΢�ŵ�¼

#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼

#define LEN_MD5						33	//��������
#define LEN_ACCOUNTS				32	//�ʺų���
#define LEN_PASSWORD				33	//���볤��
#define LEN_MACHINE_ID				33	//���г���

#define SUB_MB_LOGON_SUCCESS		100	//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101	//��¼ʧ��
#define SUB_MB_UPDATE_NOTIFY		200	//������ʾ


//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

																		//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

																		//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonOtherPlatform
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbPlatformID;						//ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];			//�û�Uin
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//�û�ͷ��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szAgentID[LEN_ACCOUNTS];			//�����ʶ

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ֻ���¼
struct CMD_GR_LogonMobile
{
	//�汾��Ϣ
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwProcessVersion;					//���̰汾

	//��������
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomID;							//�Զ�ͷ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwSpreaderID;						//�ƹ�ID
	DWORD							dwExperience;						//������ֵ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//�û�ͷ��
	TCHAR							szAliPayAcccount[30];				//֧�����˻�
	TCHAR							szBinkID[20];						//���п��˻�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����

	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����	

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	BYTE							cbIsAgent;							//�����ʶ
	BYTE							cbMoorMachine;						//��������

	//Լս�����
	int								TodayAlmsCount;						//ÿ�յͱ�����ȡ����
	DWORD							dwLockServerID;						//��������
	DWORD							dwKindID;							//��Ϸ����
};

#pragma pack()


#endif // !__Protocol_H__

