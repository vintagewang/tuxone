/*
	Copyright (c) 2005 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/

/* #ident	"@(#) samples/atmi/simpapp/simpserv.c	$Revision: 51 $" */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <atmi.h>	/* TUXEDO Header File */
#include <userlog.h>	/* TUXEDO Header File */
#include "xmlWrapper.h"
#include <string.h>

/* tpsvrinit is executed when a server is booted, before it begins
   processing requests.  It is not necessary to have this function.
   Also available is tpsvrdone (not used in this example), which is
   called at server shutdown time.
*/

#if defined(__STDC__) || defined(__cplusplus)
int tpsvrinit(int argc, char *argv[])
#else
int tpsvrinit(argc, argv)
int argc;
char **argv;
#endif
{
	/* Some compilers warn if argc and argv aren't used. */
	argc = argc;
	argv = argv;

	/* userlog writes to the central TUXEDO message log */
	userlog("Welcome to the simple server");
	return(0);
}

/* This function performs the actual service requested by the client.
   Its argument is a structure containing among other things a pointer
   to the data buffer, and the length of the data buffer.
*/

#ifdef __cplusplus
extern "C"
#endif
void
#if defined(__STDC__) || defined(__cplusplus)
STOCKQUOTE(TPSVCINFO *rqst)
#else
STOCKQUOTE(rqst)
TPSVCINFO *rqst;
#endif
{
	int i;
	struct wrapper* wrap;
	char* xmlbuf;
	FILE *fp;
	char *temp;

	wrap = getHandle();
	
	parseXMLBuffer(wrap,&rqst->data);
	rqst->len = strlen(rqst->data);
        xmlbuf = (char *) tpalloc("XML", NULL, (rqst->len)+1);
	memset(xmlbuf, NULL, (rqst->len)+1);
        strncpy(xmlbuf, rqst->data, rqst->len);
        /* Return the transformed buffer to the requestor. */
        tpreturn(TPSUCCESS, 123, xmlbuf, (rqst->len)+1, 0);
}
