/*
 * $Id: server.c 84 2011-02-13 14:17:06Z vintage $
 */
#include <stdio.h>
#include <atmi.h>
#include <userlog.h>
#include <unistd.h>

int tpsvrinit(int argc, char *argv[])
{
	argc = argc;
	argv = argv;

	userlog("Welcome to the echo server");
	return(0);
}

void tpsvrdone()
{
	userlog("echo server, bye");
}

void ECHO(TPSVCINFO *rqst)
{
	char *retbuf = NULL;

	printf("Request: \"%s\"\n", rqst->data);

	if((retbuf = (char *) tpalloc("STRING", NULL, 256)) == NULL) {
		(void) fprintf(stderr, "Error allocating return buffer\n");
		tpreturn(TPFAIL, 0, NULL, 0L, 0);
	}

	sprintf(retbuf, "server pid [%d]", getpid());

	tpreturn(TPSUCCESS, 0, retbuf, 0L, 0);
}
