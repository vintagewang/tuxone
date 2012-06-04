/*
 * $Id: test1.cpp 1 2010-09-30 14:35:38Z vintage $
 */
/*
 * ²âÊÔ²Ù×÷ÏµÍ³API
 */
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	int a = alarm(30);
	cout << "a = " << a << endl;

	sleep(5);

	int b = alarm(90);
	cout << "b = " << b << endl;

	sleep(1000000);
	return 0;
}

