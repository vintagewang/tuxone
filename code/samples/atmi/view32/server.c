/*
 * $Id: server.c 155 2011-03-13 05:44:54Z vintage $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <atmi.h>
#include <userlog.h>
#include "student.h"

int tpsvrinit(int argc, char *argv[])
{
	argc = argc;
	argv = argv;

	srandom(time(NULL));

	userlog("Welcome to the student server");
	return(0);
}

void tpsvrdone()
{
	userlog("student server, bye");
}

void ADD_STUDENT(TPSVCINFO *rqst)
{
	struct student* pStudent = (struct student*)rqst->data;

	pStudent->code = random();

	printf("student code = [%ld] name = [%s] grade = [%f]\n"
	       , pStudent->code
	       , pStudent->name
	       , pStudent->grade);

	tpreturn(TPSUCCESS, 0, (void *)pStudent, sizeof(struct student), 0);
}
