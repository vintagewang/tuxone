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
#include "BuildclientCmdline.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwpr.h>

int main(int argc, char *argv[])
{
	if(!BuildclientCmdline::GetInstance()->ParseCmdlineOption(argc, argv, "Cvwr:o:f:l:"))
	{
		printf("usage: %s [-v] [-o outfile] [-f firstfiles] [-l lastfiles]\n", argv[0]);
		return -1;
	}

	char *penv = NULL;
	std::string strCC;
	std::string strCFLAGS;
	std::string strCMPCMD;

	penv = getenv("CC");
	if(penv == NULL || strlen(penv) == 0)
	{
		strCC = "cc";
	}
	else
	{
		strCC = penv;
	}

	penv = getenv("CFLAGS");
	if(penv != NULL && strlen(penv) > 0)
	{
		strCFLAGS = penv;
	}

	// ×¼±¸±àÒëÃüÁî
	strCMPCMD = strCC;
	strCMPCMD += " ";

	strCMPCMD += strCFLAGS;
	strCMPCMD += " ";

	strCMPCMD += "-I$TUXDIR/include ";

	strCMPCMD += "-o ";
	strCMPCMD += BuildclientCmdline::GetInstance()->GetOutfile();
	strCMPCMD += " ";

	strCMPCMD += "-L${TUXDIR}/lib ";

	std::string strFirstFiles;
	if(BuildclientCmdline::GetInstance()->GetFirstFiles(strFirstFiles))
	{
		strCMPCMD += strFirstFiles;
		strCMPCMD += " ";
	}

	strCMPCMD += "-ltux -llwpr ";

	std::string strLastFiles;
	if(BuildclientCmdline::GetInstance()->GetLastFiles(strLastFiles))
	{
		strCMPCMD += strLastFiles;
		strCMPCMD += " ";
	}

	if(BuildclientCmdline::GetInstance()->IsVerboseMode())
	{
		printf("%s\n", strCMPCMD.c_str());
	}

	return system(strCMPCMD.c_str());
}
