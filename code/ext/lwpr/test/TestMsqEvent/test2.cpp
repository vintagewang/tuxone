/*
 * $Id: test2.cpp 1 2010-09-30 14:35:38Z vintage $
 */
/*
 * 测试LWPR::EventMsqDispatch类是否能正常工作
 * 充当Client
 */
#include "IPCMSQ.h"
#include "Utility.h"
#include "SimpleProcessPool.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Test2 : public LWPR::SimpleProcessPool
{
public:
	Test2()
	{
	}

	~Test2()
	{
	}

	int DoChildWork(int argc, char** argv)
	{
		for(int i = 0; i < 10; i++)
		{
			LWPR::Buffer buf;
			buf.Size(128);

			LWPR::INT32 nMsgType = LWPR::Utility::GetPid();
			memcpy(buf.Inout(), &nMsgType, sizeof(nMsgType));

			strcpy(buf.Inout() + sizeof(LWPR::INT32), argv[3]);

			LWPR::IPCID_T nMsqId = atoi(argv[2]);

			LWPR::IPCMSQ_RET_TYPE_E nRet = LWPR::IPCMSQ::SendMsg(nMsqId, buf.Inout(), buf.Size());

			printf("LWPR::IPCMSQ::SendMsg %d\n", (int)nRet);

			sleep(1);
		}

		return 0;
	}
};


int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Useage: %s 进程数 对方消息队列ID 发送内容\n", argv[0]);
		exit(-1);
	}

	Test2 test2;
	test2.SetProcessNum(atoi(argv[1]));
	test2.Run(argc, argv);

	return 0;
}
