/*
 * $Id: test1.cpp 43 2010-10-16 15:09:55Z vintage $
 */
/*
 * ≤‚ ‘notify wait
 */
#include "Resource.h"
#include "Thread.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

class TestThread :  public LWPR::Thread, public LWPR::Resource
{
public:
	TestThread()
	{
		cout << __FUNCTION__ << endl;
		Start();
	}

	~TestThread()
	{
		cout << __FUNCTION__ << endl;
	}

	void Run()
	{
		cout << __FUNCTION__ << endl;
		while(true)
		{
//			Wait(6.3);
			Wait(0);
			//Wait();
			cout << "I am awake" << endl;
		}
	}

private:
};

int main(int argc, char *argv[])
{
	try
	{
		TestThread* t = new TestThread;

		int i = 3;
		while(true)
		{
			sleep(i++);

			t->Notify();
		}


		sleep(1000);
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}
