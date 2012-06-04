/*
 * $Id: client.c 154 2011-03-13 05:36:42Z vintage $
 */
#include <stdio.h>
#include <atmi.h>
#include <string.h>
#include "student.h"

int main(int argc, char *argv[])
{
	char *sendrcvbuf;
	long sendrcvlen;
	int ret = 0, i = 0;
	struct student* pStudent = NULL;

	if(argc != 2) {
		(void) fprintf(stderr, "Usage: client name\n");
		return -1;
	}

	/* Attach to System/T as a Client Process */
	if(tpinit((TPINIT *) NULL) == -1)
	{
		(void) fprintf(stderr, "Tpinit failed\n");
		return -1;
	}

	/* Allocate STRING buffers for the request and the reply */
	if((sendrcvbuf = (char *) tpalloc("VIEW32", "student", sizeof(struct student))) == NULL)
	{
		(void) fprintf(stderr, "Error allocating buffer\n");
		tpterm();
		return -1;
	}

	/* Set request value */
	pStudent = (struct student*)sendrcvbuf;
	pStudent->code = 0;
	pStudent->grade = 99;
	strncpy(pStudent->name, argv[1], 80);

	/* Request the service ECHO, waiting for a reply */
	ret = tpcall("ADD_STUDENT", (char *)sendrcvbuf, 0, (char **)&sendrcvbuf, &sendrcvlen, (long)0);
	if(ret == -1)
	{
		(void) fprintf(stderr, "Can't send request to service ADD_STUDENT\n");
		(void) fprintf(stderr, "tperrno = %d\n", tperrno);
	}
	else
	{
		pStudent = (struct student*)sendrcvbuf;
		printf("return student code = [%ld]\n", pStudent->code);
	}

	/* Free Buffers & Detach from System/T */
	tpfree(sendrcvbuf);
	tpterm();
	return(0);
}
