/*
 * $Id: test2.cpp 1 2010-09-30 14:35:38Z vintage $
 */
/*
 * 测试LWPR::Mutex类是否能正常工作
 */
#include "Thread.h"
#include "Synchronized.h"
#include <iostream>
#include <stdio.h>

using namespace std;

LWPR::Mutex g_Mutex;
int i = 0;

class ThreadTest : public LWPR::Thread
{
public:
	ThreadTest()
	{
		this->Start();
	}

	~ThreadTest()
	{

	}

	void Run()
	{
		while(true)
		{
			LWPR::Synchronized syn(g_Mutex);
			printf("线程[%ld] 正在运行。。。 %d\n", GetId(), i++);
			Sleep(0.1);
		}
	}

private:
};

int main(int argc, char *argv[])
{
	ThreadTest t[10];

	sleep(1000000000);
	return 0;
}
