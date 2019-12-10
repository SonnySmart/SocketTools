#include <iostream>
#include <SocketTools/ClientSocket.h>
#include <SocketTools/SocketManager.h>
#include <stdio.h>
#include <thread>

//长度定义
#define SOCKET_TCP_BUFFER			16384								//网络缓冲

extern "C" __declspec(dllimport) void _OnReceive(void *buffer, unsigned int &len);
extern "C" __declspec(dllimport) void _SendValidate(void *buffer, unsigned int &len);
extern "C" __declspec(dllimport) void _SendHeart(void *buffer, unsigned int &len);
extern "C" __declspec(dllimport) void _SendLogin(void *buffer, unsigned int &len);
extern "C" __declspec(dllimport) void _SendWeChatLogin(void *buffer, unsigned int &len);
extern "C" __declspec(dllimport) void _SendEnterRoom(void *buffer, unsigned int &len);

bool SocketSend(socket_tools::ClientSocket *socket, const char *buf, int len);

void OnHeart(socket_tools::ClientSocket *socket)
{
	char sendbuf[1024] = { 0 };
	unsigned int len = 0;
	while (true)
	{
		memset(sendbuf, 0, sizeof(sendbuf));
		_SendHeart(sendbuf, len);
		SocketSend(socket, sendbuf, len);
		std::this_thread::sleep_for(std::chrono::milliseconds(15000));		
	}
}

void OnReceive(socket_tools::ClientSocket *socket)
{
	BYTE reciveBuffer[SOCKET_TCP_BUFFER];
	while (true)
	{
		ZeroMemory(reciveBuffer, SOCKET_TCP_BUFFER);
		size_t len = socket->Receive(reciveBuffer, SOCKET_TCP_BUFFER);
		if (len > 0)
		{
			//printf("数据接收成功[%p][%d]", reciveBuffer, len);
			_OnReceive(reciveBuffer, len);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool SocketSend(socket_tools::ClientSocket *socket, const char *buf, int len)
{
	int nAllSentDataSize;
	int nNeedSentDataSize;
	int nAllSentDataSize_BeforeCurSend;
	int CurSentDataSize;

	bool bResult;

	bResult = false;

	nAllSentDataSize = 0;
	if (socket->SocketIsValid())
	{
		nNeedSentDataSize = len;
		if (len > 0)
		{
			while (TRUE)
			{
				nAllSentDataSize_BeforeCurSend = nAllSentDataSize;
				CurSentDataSize = socket->Send((const uint8_t*)&buf[nAllSentDataSize], nNeedSentDataSize - nAllSentDataSize);
				if (CurSentDataSize < 1)
					break;
				nAllSentDataSize = CurSentDataSize + nAllSentDataSize_BeforeCurSend;
				nNeedSentDataSize = len;
				if (CurSentDataSize + nAllSentDataSize_BeforeCurSend >= len)
					goto _RESULT_;
			}
			bResult = false;
		}
		else
		{
		_RESULT_:
			bResult = nAllSentDataSize == nNeedSentDataSize;
		}
	}
	return bResult;
}

int __cdecl main(int argc, char **argv)
{
	constexpr auto defaultBuflen = 1024;
	constexpr auto defaultServ = "47.94.109.177";
	//constexpr auto defaultServ = "127.0.0.1";
	constexpr auto defaultPort = "8600";
	constexpr auto gamePort = "7100";

	char sendbuf[defaultBuflen] = "This is a test";
	char recvbuf[defaultBuflen];
	unsigned int len = defaultBuflen;
	memset(recvbuf, 0, sizeof(recvbuf));

	//if (argc != 2)
	//{
	//	std::cout << "usage: " << argv[0] << " server-name" << std::endl;
	//	return 1;
	//}

	using namespace socket_tools;

	SocketManager socketManager;

	ClientSocket socket(
		SocketMode::Stream,
		Protocol::TCP,
		AddressFamily::Unspecified,
		defaultServ,
		defaultPort);

	ClientSocket socket2(
		SocketMode::Stream,
		Protocol::TCP,
		AddressFamily::Unspecified,
		defaultServ,
		gamePort);

	if (socket.SocketIsValid())
	{
		printf("[%s][%s]连接成功.\n", defaultServ, defaultPort);
	}
	if (socket2.SocketIsValid())
	{
		printf("[%s][%s]连接成功.\n", defaultServ, gamePort);
	}

	memset(sendbuf, 0, sizeof(sendbuf));
	_SendValidate(sendbuf, len);
	SocketSend(&socket, sendbuf, len);

	std::thread t = std::thread(&OnHeart, &socket);
	t.detach();
	std::thread t1 = std::thread(&OnHeart, &socket2);
	t1.detach();
	std::thread t2 = std::thread(&OnReceive, &socket);
	t2.detach();

	memset(recvbuf, 0, sizeof(recvbuf));
	printf("按任意键开始发送登录大厅\n");
	gets_s(recvbuf);

	memset(sendbuf, 0, sizeof(sendbuf));
	_SendLogin(sendbuf, len);
	//_SendWeChatLogin(sendbuf, len);
	SocketSend(&socket, sendbuf, len);

	memset(recvbuf, 0, sizeof(recvbuf));
	printf("按任意键开始发送登录房间\n");
	gets_s(recvbuf);

	memset(sendbuf, 0, sizeof(sendbuf));
	_SendEnterRoom(sendbuf, len);
	SocketSend(&socket2, sendbuf, len);

	while (recvbuf[0] != 'q')
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		printf("按q退出\n");
		gets_s(recvbuf);
	}

	socket.Shutdown();
	socket2.Shutdown();

	return 0;
}