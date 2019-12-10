
#include "MobileClientKernel\ry_MD5.h"
#include "MobileClientKernel\LuaAssert.h"
#include "Protocol\Protocol.h"
#include "MobileClientKernel/Define.h"
#include "MobileClientKernel/packet.h"
#include "MobileClientKernel/Cipher.h"

static CMD_MB_LogonSuccess *g_CMD_MB_LogonSuccess = NULL;

void pushString(const char *szData, WORD wDstLen, void *out)
{
	do
	{
		if (!EMPTY_CHAR(szData))
		{
			size_t wSrcLen = strlen(szData);
			wchar_t *szTmp = new wchar_t[wDstLen + 1];
			memset(szTmp, 0, (wDstLen + 1) * 2);
			char* szSrc = new char[wSrcLen + 1];
			memset(szSrc, 0, wSrcLen + 1);
			memcpy(szSrc, szData, wSrcLen);
			ToServerString(szSrc, wSrcLen, (char*)szTmp, wDstLen * 2);
			CopyMemory((BYTE *)out, szTmp, wDstLen * 2);
			delete szTmp;
			delete szSrc;
		}
		else
		{
			ZeroMemory((BYTE *)out, wDstLen * 2);
		}
	} while (false);
}

CMD_MB_LogonAccounts phoneLoginData(const char *account, const char *pass)
{
	CMD_MB_LogonAccounts cmd;
	ZeroMemory(&cmd, sizeof(CMD_MB_LogonAccounts));
	cmd.wModuleID = INVALID_WORD;
	cmd.dwPlazaVersion = 101122049;
	cmd.cbDeviceType = 1;
	pushString(md5(string(pass)).c_str(), LEN_MD5, cmd.szPassword);
	pushString(account, LEN_ACCOUNTS, cmd.szAccounts);
	pushString("1575262880", LEN_MACHINE_ID, cmd.szMachineID);
	pushString("", LEN_MOBILE_PHONE, cmd.szMobilePhone);
	return cmd;
}

CMD_MB_LogonOtherPlatform wechatLoginData(const char *account, const char *nick)
{
	CMD_MB_LogonOtherPlatform cmd;
	ZeroMemory(&cmd, sizeof(CMD_MB_LogonAccounts));
	cmd.wModuleID = INVALID_WORD;
	cmd.dwPlazaVersion = 101122049;
	cmd.cbDeviceType = 1;
	cmd.cbPlatformID = 5;
	pushString("1575262880", LEN_MACHINE_ID, cmd.szMachineID);
	pushString(account, LEN_USER_UIN, cmd.szUserUin);
	pushString(nick, LEN_NICKNAME, cmd.szNickName);
	return cmd;
}

CMD_GR_LogonMobile* enterRoomData(WORD kind)
{
	CMD_GR_LogonMobile *cmd = new CMD_GR_LogonMobile;
	cmd->wGameID = kind;
	cmd->dwProcessVersion = 101122049;
	cmd->cbDeviceType = 0x10;
	cmd->wBehaviorFlags = 0x0011;
	cmd->wPageTableCount = 0xff;
	cmd->dwUserID = g_CMD_MB_LogonSuccess->dwUserID;
	pushString("1575262880", LEN_MACHINE_ID, cmd->szMachineID);
	CopyMemory(cmd->szPassword, g_CMD_MB_LogonSuccess->szDynamicPass, sizeof(cmd->szPassword));
	pushString("", LEN_MD5, cmd->szServerPasswd);
	return cmd;
}

size_t SendSocketData(void *buffer, unsigned short wMain, unsigned short wSub, const void* pData, unsigned short wDataSize)
{
	size_t result = 0;
	TCP_Buffer TcpBuffer;
	if (wDataSize < sizeof(TcpBuffer.cbBuffer))
	{
		memset(&TcpBuffer, 0, sizeof(TcpBuffer));
		TcpBuffer.Head.CommandInfo.wMainCmdID = wMain;
		TcpBuffer.Head.CommandInfo.wSubCmdID = wSub;
		TcpBuffer.Head.TCPInfo.wPacketSize = wDataSize + sizeof(TCP_Head);
		if (pData && wDataSize)
			memcpy(&TcpBuffer.cbBuffer, pData, wDataSize);
		CCipher::encryptBuffer(&TcpBuffer, (wDataSize + sizeof(TCP_Head)));
		CopyMemory(buffer, &TcpBuffer, TcpBuffer.Head.TCPInfo.wPacketSize);
		result = TcpBuffer.Head.TCPInfo.wPacketSize;
	}

	return result;
}

extern "C" __declspec(dllexport) void _OnReceive(void *buffer, unsigned int &len)
{
	CCipher::decryptBuffer(buffer, len);
	TCP_Buffer* pTcpBuffer = (TCP_Buffer*)buffer;
	if (pTcpBuffer->Head.CommandInfo.wMainCmdID || pTcpBuffer->Head.CommandInfo.wSubCmdID != SUB_KN_DETECT_SOCKET)
	{
		switch (pTcpBuffer->Head.CommandInfo.wMainCmdID)
		{
			case MDM_MB_LOGON:
			{
				if (pTcpBuffer->Head.CommandInfo.wSubCmdID == SUB_MB_LOGON_SUCCESS)
				{
					if (g_CMD_MB_LogonSuccess == NULL)
						g_CMD_MB_LogonSuccess = new CMD_MB_LogonSuccess;
					ZeroMemory(g_CMD_MB_LogonSuccess, sizeof(CMD_MB_LogonSuccess));
					CopyMemory(g_CMD_MB_LogonSuccess, pTcpBuffer->cbBuffer, sizeof(CMD_MB_LogonSuccess));
				}
			}
			break;
		}
	}
}

extern "C" __declspec(dllexport) void _SendValidate(void *buffer, unsigned int &len)
{
	TCHAR pValidate[128] = {};
	pushString(VALIDATE, 128, pValidate);
	len = SendSocketData(buffer, MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, pValidate, sizeof(pValidate));
}

extern "C" __declspec(dllexport) void _SendHeart(void *buffer, unsigned int &len)
{
	len = SendSocketData(buffer, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, NULL, 0);
}

extern "C" __declspec(dllexport) void _SendLogin(void *buffer, unsigned int &len)
{
	CMD_MB_LogonAccounts cmd = phoneLoginData("test004", "abc123456");
	len = SendSocketData(buffer, MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &cmd, sizeof(cmd));
}

extern "C" __declspec(dllexport) void _SendWeChatLogin(void *buffer, unsigned int &len)
{
	CMD_MB_LogonOtherPlatform cmd = wechatLoginData("oppNz1iQWCO8CEHxDXAQqFU0Q_qc", "Î´ÖªÊý");
	len = SendSocketData(buffer, MDM_MB_LOGON, SUB_MB_LOGON_OTHERPLATFORM, &cmd, sizeof(cmd));
}

extern "C" __declspec(dllexport) void _SendEnterRoom(void *buffer, unsigned int &len)
{
	CMD_GR_LogonMobile *cmd = enterRoomData(105);
	len = SendSocketData(buffer, MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, cmd, sizeof(CMD_GR_LogonMobile));
	delete cmd;
}
