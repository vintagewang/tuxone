/*
 * $Id: client.c 174 2011-03-19 04:29:53Z vintage $
 */
#include <stdio.h>
#include <string.h>
#include <atmi.h>
#include <fml32.h>
#include "friend.h"

int main(int argc, char *argv[])
{
	char *sendrcvbuf;
	long sendrcvlen;
	int ret = 0, i = 0;
	const char* NAME = "Jack Bauer";
	char SEX = 'M';
	long AGE = 40;
	const char *ADDR = "Room 501, Unit 6, Building 20 North Donghuashi Residential Chongwen District BeiJing City";
	short CHILDREN = 1;
	float WEIGHT = 73.58;
	double HEIGHT = 175.68;
	char PHOTO[128] = {0};
	long ADDTIME = 0;

	/* Attach to System/T as a Client Process */
	if(tpinit((TPINIT *) NULL) == -1) {
		(void) fprintf(stderr, "Tpinit failed\n");
		return -1;
	}

	/* Allocate FML32 buffers for the request and the reply */
	if((sendrcvbuf = (char *) tpalloc("FML32", NULL, 0)) == NULL) {
		(void) fprintf(stderr, "Error allocating buffer\n");
		(void) fprintf(stderr, "tperrno = %d\n", tperrno);
		tpterm();
		return -1;
	}

	/* Set request value */
	if(argc >= 2) {
		NAME = argv[1];
	}

	memset(PHOTO, 'P', sizeof(PHOTO));

	/* FRIEND_NAME */
	ret = Fadd32(sendrcvbuf, FRIEND_NAME, (char *)NAME, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_NAME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_SEX */
	ret = Fadd32(sendrcvbuf, FRIEND_SEX, (char *)&SEX, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_SEX\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_AGE */
	ret = Fadd32(sendrcvbuf, FRIEND_AGE, (char *)&AGE, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_AGE\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_ADDR */
	ret = Fadd32(sendrcvbuf, FRIEND_ADDR, (char *)ADDR, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_ADDR\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_CHILDREN */
	ret = Fadd32(sendrcvbuf, FRIEND_CHILDREN, (char *)&CHILDREN, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_CHILDREN\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_WEIGHT */
	ret = Fadd32(sendrcvbuf, FRIEND_WEIGHT, (char *)&WEIGHT, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_WEIGHT\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_HEIGHT */
	ret = Fadd32(sendrcvbuf, FRIEND_HEIGHT, (char *)&HEIGHT, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_HEIGHT\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_PHOTO */
	ret = Fadd32(sendrcvbuf, FRIEND_PHOTO, PHOTO, sizeof(PHOTO));
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_PHOTO\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* FRIEND_ADDTIME */
	ret = Fadd32(sendrcvbuf, FRIEND_ADDTIME, (char *)&ADDTIME, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't add field FRIEND_ADDTIME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	/* Request the service ADD_FRIEND, waiting for a reply */
	ret = tpcall("ADD_FRIEND", (char *)sendrcvbuf, 0, (char **)&sendrcvbuf, &sendrcvlen, (long)0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't send request to service ADD_FRIEND\n");
		(void) fprintf(stderr, "tperrno = %d\n", tperrno);
		goto END;
	}

	/* FRIEND_ADDTIME */
	ret = Fget32(sendrcvbuf, FRIEND_ADDTIME, 0, (char *)&ADDTIME, 0);
	if(ret == -1) {
		(void) fprintf(stderr, "Can't get field FRIEND_ADDTIME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		goto END;
	}

	printf("Add friend OK, add time = [%ld]\n", ADDTIME);

END:
	/* Free Buffers & Detach from System/T */
	tpfree(sendrcvbuf);
	tpterm();
	return ret;
}
