/*
 * $Id: test1.cpp 69 2010-12-11 12:34:49Z vintage $
 */
/*
 * 测试LWPR::EventNetDispatch类是否能正常工作
 * 充当Server
 */
#include "Socket.h"
#include "Buffer.h"
#include "Utility.h"
#include "NetServer.h"
#include "Thread.h"
#include "Socket.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Receiver : public LWPR::NetServerHandler
{
public:
	/**
	 * 构造函数
	 */
	Receiver() {}

	/**
	 * 析构函数
	 */
	~Receiver() {}

	/**
	 * 读写socket
	 */
	LWPR::SOCKET_RET_TYPE_E DoReceiveNormalData(LWPR::SOCKET_FD_T fd)
	{
		printf("Receiver::DoReceiveNormalData -------------------------------------------------------------\n");
		printf("Receiver::DoReceiveNormalData Current thread id [%u] fd [%d]\n", LWPR::Thread::GetCurrentThreadId(), fd);

		printf("GetListenAddr = [%s]\n", GetNetServer()->GetListenAddr().c_str());
		LWPR::Buffer buf;

		LWPR::SOCKET_RET_TYPE_E nRet = LWPR::Socket::ReadSocket(fd, buf, 5, 6);
		if(nRet == LWPR::SOCKET_RET_FAILED)
		{
			printf("Receiver::DoReceiveNormalData error\n");
		}
		else if(nRet == LWPR::SOCKET_RET_OK)
		{
			printf("Receiver::DoReceiveNormalData Current thread id [%u] fd [%d] [%s]\n",
			       LWPR::Thread::GetCurrentThreadId(),
			       fd,
			       buf.Inout());

			bool result = GetNetServer()->WriteSocketPositive(fd, "office", 6);

			printf("Receiver::WriteSocketPositive %s\n", result ? "TRUE" : "FALSE");
		}
		else if(nRet == LWPR::SOCKET_RET_TIMEOUT)
		{
			printf("Receiver::DoReceiveNormalData timeout\n");
		}

		return nRet;
	}

	virtual void DoClientAmountUpdate(int amount, LWPR::SOCKET_FD_T fd, bool increase)
	{
		const char* desc = increase ? "increase a client fd" : "decrease a client fd";

		printf("%s: [%d], amount: [%d]\n", desc, fd, amount);
	}
};

LWPR::NetServer* g_pNetServer = NULL;

void DoSignalHandler(int signum)
{
	cout << "DoSignalHandler:" << signum << endl;

	if (g_pNetServer != NULL)
	{
		g_pNetServer->Shutdown();
	}
}

int main(int argc, char *argv[])
{
	try
	{
		// 捕获信号
		LWPR::Utility::RegisterSignal(SIGINT, DoSignalHandler);

		// 创建网络服务器
		LWPR::NET_SERVER_OPTION_T opt;
		opt.strAddr = "0.0.0.0:29000~39000";
		opt.pHandler = new Receiver();

		g_pNetServer = LWPR::NetServer::CreateNetServer(LWPR::NET_SERVER_MT, opt);

		g_pNetServer->ActivateService();

		g_pNetServer->Go();
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}
