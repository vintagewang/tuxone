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
#include <stdio.h>
#include <string.h>
#include <atmi.h>
#include <fml32.h>
#include "friend.h"

int main(int argc, char** argv)
{
	const char* NAME = "Jack Bauer";
	char *sendrcvbuf;
	int ret;

	/* Allocate FML32 buffers for the request and the reply */
	if((sendrcvbuf = (char *) tpalloc("FML32", NULL, 0)) == NULL)
	{
		(void) fprintf(stderr, "Error allocating buffer\n");
		(void) fprintf(stderr, "tperrno = %d\n", tperrno);
		return -1;
	}

	/* FRIEND_NAME */
	ret = Fadd32(sendrcvbuf, FRIEND_NAME, (char *)NAME, 0);
	if(ret == -1)
	{
		(void) fprintf(stderr, "Can't add field FRIEND_NAME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		return -1;
	}

	/* FRIEND_NAME */
	char buf[128] = {0};
	FLDLEN32 maxlen = sizeof(buf) - 1;
	ret = Fget32(sendrcvbuf, FRIEND_NAME, 0, buf, &maxlen);
	if(ret == -1)
	{
		(void) fprintf(stderr, "Can't get field FRIEND_NAME\n");
		(void) fprintf(stderr, "Ferror32 = %d\n", Ferror32);
		return -1;
	}

	/* ¥Ú”° */
	printf("buf = [%s]\n", buf);

	return 0;


}
