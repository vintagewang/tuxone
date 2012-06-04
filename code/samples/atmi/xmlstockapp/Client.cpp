/*
	Copyright (c) 2005 BEA Systems, Inc.
	All rights reserved

	THIS IS UNPUBLISHED PROPRIETARY
	SOURCE CODE OF BEA Systems, Inc.
	The copyright notice above does not
	evidence any actual or intended
	publication of such source code.
*/
#include <stdio.h>
#include "atmi.h"               /* TUXEDO  Header File */
#include "userlog.h"               /* TUXEDO  Header File */
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/TranscodingException.hpp>


#include <xercesc/dom/DOMException.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include "DOMTreeErrorReporter.hpp"
#include <string.h>
#include <stdlib.h>

#define XMLDOCSIZE      1048576 /* 1 MB XML doc size */

extern void parseXMLBuffer (char * rcvbuf);

#if defined(__STDC__) || defined(__cplusplus)
int main(int argc, char *argv[])
#else
int main(argc, argv)
int argc;
char *argv[];
#endif
{
        char *sendbuf, *rcvbuf;
        long sendlen, rcvlen;
        long flags;
        int  ret;
        char type[8], subtype[16];
        FILE *xml_fd;
        char xml_file[256];
        char *xml_buffer = NULL;
        int  nsize;

        if(argc != 2) {
                fprintf(stderr, "Usage: %s <XML doc>\n", argv[0]);
                return(1);
        }

        /* Read the XML document in first */
        (void)strcpy(xml_file, argv[1]);

        if ((xml_fd = fopen(xml_file, "r")) != NULL) {
                xml_buffer = (char *)malloc(sizeof(char) * XMLDOCSIZE);

                nsize = fread(xml_buffer, sizeof(char), XMLDOCSIZE, xml_fd);
        }

        /* Attach to System/T as a Client Process */
        if (tpinit((TPINIT *) NULL) == -1) {
                (void) fprintf(stderr, "Tpinit failed\n");
                return(1);
        } 

        sendlen = nsize + 1;
        if((sendbuf = (char *) tpalloc((char *)"XML", NULL, sendlen)) == NULL) {
                fprintf(stderr,"Error allocating send buffer, tperrno=%ld\n",tperrno);
                tpterm();
                return(1);
        }

        if((rcvbuf = (char *) tpalloc((char *)"XML", NULL, sendlen+1)) == NULL) {
                fprintf(stderr,"Error allocating receive buffer, tperrno=%ld\n",tperrno);
                tpfree(sendbuf);
                tpterm();
                return(1);
        }

        strncpy(sendbuf, xml_buffer, nsize);

        flags = TPNOTIME;
        ret = tpcall((char *)"STOCKQUOTE", (char *)sendbuf, sendlen, (char **)&rcvbuf,  
                       &rcvlen, (long)flags);

        if(ret == -1) {
                fprintf(stderr, "Can't send request to service ORDERSVC\n");
                fprintf(stderr, "Tperrno = %d\n", tperrno);
                tpfree(sendbuf);
                tpfree(rcvbuf);
                tpterm();
                return(1);
        }

	fprintf(stdout, "%s\n",rcvbuf);
	
        /* Free Buffers & Detach from System/T */
        tpfree(sendbuf);
        tpfree(rcvbuf);
        tpterm();         
	return(0);
}
