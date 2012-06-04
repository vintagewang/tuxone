#include <atmi.h>

int main(int argc, char** argv)
{
	char *sendbuf = NULL;

	sendbuf = (char *) tpalloc("CARRAY", NULL, 1024);
	if(sendbuf == NULL) {
		(void) fprintf(stderr, "Error allocating send buffer\n");
		return -1;
	}

	tpfree(sendbuf);
}
