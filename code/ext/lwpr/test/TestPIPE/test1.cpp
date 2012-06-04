/*
 * $Id: test1.cpp 67 2010-12-06 13:50:35Z vintage $
 */
/*
 * ≤‚ ‘π‹µ¿
 */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char *argv[])
{
	int fd[2] = {0};

	int code = pipe(fd);
	if(code != 0)
	{
		printf("pipe error\n");
		return 0;
	}

	code = write(fd[1], "hello", 5);
	printf("write return code = [%d]\n", code);

	char buf[128] = {0};
	code = read(fd[0], buf, 5);
	printf("read return code = [%d]\n", code);

	//code = read(fd[0], buf, 5);
	//printf("read return code = [%d]\n", code);

	code = write(fd[1], "world", 5);
	printf("write return code = [%d]\n", code);

	// select
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd[0], &rfds);
	code = select(fd[0] + 1, &rfds, NULL, NULL, &tv);
	printf("select return code = [%d]\n", code);

	code = read(fd[0], buf, 5);
	printf("read pepe [%s]\n", buf);

	return 0;
}
