/*
 * $Id: test1.cpp 32 2010-10-10 13:46:12Z vintage $
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

		}
		else if(nRet == LWPR::SOCKET_RET_TIMEOUT)
		{
			printf("Receiver::DoReceiveNormalData timeout\n");
		}

		return nRet;
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
		opt.strIP = "192.168.187.128:39000";
		opt.pHandler = new Receiver();

		g_pNetServer = LWPR::NetServer::CreateNetServer(LWPR::NET_SERVER_ST, opt);

		g_pNetServer->ActivateService();

		g_pNetServer->Go();
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}
