/*
 * $Id: client.c 208 2011-05-02 07:38:35Z vintage $
 */
#include <stdio.h>
#include <atmi.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
	char *sendbuf, *rcvbuf;
	long sendlen, rcvlen;
	int ret = 0, i = 0;

	/* Attach to System/T as a Client Process */
	if(tpinit((TPINIT *) NULL) == -1) {
		(void) fprintf(stderr, "Tpinit failed\n");
		return -1;
	}

	/* Allocate STRING buffers for the request and the reply */
	if((sendbuf = (char *) tpalloc("STRING", NULL, 256)) == NULL) {
		(void) fprintf(stderr, "Error allocating send buffer\n");
		tpterm();
		return -1;
	}

	if((rcvbuf = (char *) tpalloc("STRING", NULL, 256)) == NULL) {
		(void) fprintf(stderr, "Error allocating receive buffer\n");
		tpfree(sendbuf);
		tpterm();
		return -1;
	}

	while (1) {
		sprintf(sendbuf, "invoke serial number [%d]", i++);

		/* Request the service ECHO, waiting for a reply */
		ret = tpcall("ECHO", (char *)sendbuf, 0, (char **)&rcvbuf, &rcvlen, (long)0);
		if(ret == -1) {
			(void) fprintf(stderr, "Can't send request to service ECHO\n");
			(void) fprintf(stderr, "tperrno = %d\n", tperrno);
		}
		else {
			(void) fprintf(stdout, "send buf: \"%s\" --- receive buf: \"%s\"\n", sendbuf, rcvbuf);
		}

#ifdef WIN32
		Sleep(0);
#else
		sleep(0);
#endif
	}

	/* Free Buffers & Detach from System/T */
	tpfree(sendbuf);
	tpfree(rcvbuf);
	tpterm();
	return(0);
}
