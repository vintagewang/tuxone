/**
 * Copyright 2012 Wangxr, vintage.wang@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <lwpr.h>

using std::cout;
using std::cerr;
using std::endl;

static int run(int argc, char** argv)
{
	for(int i = 0; i < atoi(argv[1]); i++)
	{
		LWPR::SOCKET_FD_T fd = LWPR::Socket::ConnectRemoteHost(argv[2]);
		cout << "create remote connection " << i << "\t" << fd << endl;
		LWPR::Thread::Sleep(1);
	}

	LWPR::Thread::Sleep(1000);

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(LWPR::Exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(std::exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
