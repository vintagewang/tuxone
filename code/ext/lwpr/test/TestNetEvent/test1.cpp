/*
 * $Id: test1.cpp 16 2010-10-05 02:52:15Z vintage $
 */
/*
 * 测试LWPR::EventNetDispatch类是否能正常工作
 * 充当Server
 */
#include "Socket.h"
#include "Buffer.h"
#include "Utility.h"
#include "EventNetDispatch.h"
#include "Thread.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Receiver : public LWPR::EventNetHandler
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

int main(int argc, char *argv[])
{
	try
	{
		LWPR::EVENT_NET_OPTION_T opt;
		opt.nMinPort = 33333;
		opt.nMaxPort = 33344;
		opt.strIP = "192.168.187.128";
		opt.nThreadCount = 30;
		opt.pHandler = new Receiver();

		LWPR::EventNetDispatch disp(opt);

		disp.Start();

		disp.Reap();

		printf("-------------------------------------------------\n");
		sleep(100);
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	printf("=====================================================\n");

	return 0;
}
