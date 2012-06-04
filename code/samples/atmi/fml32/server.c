/*
 * $Id: server.c 175 2011-03-19 04:31:31Z vintage $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <atmi.h>
#include <userlog.h>
#include <fml32.h>
#include "friend.h"

int tpsvrinit(int argc, char *argv[])
{
	argc = argc;
	argv = argv;

	userlog("Welcome to the friend server");
	return(0);
}

void tpsvrdone()
{
	userlog("friend server, bye");
}

void ADD_FRIEND(TPSVCINFO *rqst)
{
	char NAME[64] = {0};
	char SEX = 'M';
	long AGE = 0;
	char ADDR[256] = {0};
	short CHILDREN = 0;
	float WEIGHT = 0.0;
	double HEIGHT = 0.0;
	char PHOTO[128] = {0};
	long ADDTIME = 0;
	int ret = 0;
	FLDLEN32 maxlen = 0;

	/* FRIEND_NAME */
	maxlen = sizeof(NAME) - 1;
	ret = Fget32(rqst->data, FRIEND_NAME, 0, NAME, &maxlen);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_NAME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_SEX */
	ret = Fget32(rqst->data, FRIEND_SEX, 0, &SEX, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_SEX\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_AGE */
	ret = Fget32(rqst->data, FRIEND_AGE, 0, (char *)&AGE, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_AGE\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_ADDR */
	maxlen = sizeof(ADDR) - 1;
	ret = Fget32(rqst->data, FRIEND_ADDR, 0, (char *)ADDR, &maxlen);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_ADDR\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_CHILDREN */
	ret = Fget32(rqst->data, FRIEND_CHILDREN, 0, (char *)&CHILDREN, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_CHILDREN\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_WEIGHT */
	ret = Fget32(rqst->data, FRIEND_WEIGHT, 0, (char *)&WEIGHT, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_WEIGHT\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_HEIGHT */
	ret = Fget32(rqst->data, FRIEND_HEIGHT, 0, (char *)&HEIGHT, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_HEIGHT\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_PHOTO */
	maxlen = sizeof(PHOTO);
	ret = Fget32(rqst->data, FRIEND_PHOTO, 0, PHOTO, &maxlen);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_PHOTO\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_ADDTIME */
	ADDTIME = time(NULL);
	ret = Fchg32(rqst->data, FRIEND_ADDTIME, 0, (char *)&ADDTIME, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't change field FRIEND_ADDTIME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	printf("FRIEND_NAME = [%s] FRIEND_SEX = [%c] FRIEND_AGE = [%ld]\n", NAME, SEX, AGE);
	printf("\tFRIEND_ADDR = [%s]\n", ADDR);
	printf("\tFRIEND_CHILDREN = [%d] FRIEND_WEIGHT = [%f] FRIEND_HEIGHT = [%f]\n\n", CHILDREN, WEIGHT, HEIGHT);

	tpreturn(TPSUCCESS, 0, (void *)rqst->data, 0, 0);

END:
	tpreturn(TPFAIL, 0, (void *)NULL, 0, 0);
}
