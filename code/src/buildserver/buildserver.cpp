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
#include "BuildserverCmdline.h"
#include "MainStub.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <lwpr.h>

bool GenMainFile(std::string& fileName)
{
	std::string newFileName = LWPR::Utility::GetRandomString();
	newFileName = newFileName.substr(0, 4);
	newFileName = "BS-" + newFileName;
	newFileName += ".c";
	fileName = newFileName;

	char* p = getenv("TUXDIR");
	if(p == NULL)
	{
		return false;
	}

	std::vector< std::string > services;
	if(!BuildserverCmdline::GetInstance()->GetServiceNames(services))
	{
		return false;
	}

	std::string strFun;
	std::string strItem;
	for(size_t i = 0; i < services.size(); i++)
	{
		{
			char buf[256] = {0};
			sprintf(buf, "extern void %s(TPSVCINFO *);\n", services[i].c_str());
			strFun += buf;
		}

		{
			char buf[256] = {0};
			sprintf(buf, "	{ (char*)\"%s\", (SVCFUNC_FP) %s},\n",
			        services[i].c_str(),
			        services[i].c_str());

			strItem += buf;
		}
	}

	LWPR::Buffer_var vNewBuf = new LWPR::Buffer(strlen(g_fmtMainStub) + strFun.length() + strItem.length() + 1024);
	LWPR::CFile newFile(newFileName.c_str(), "w");
	sprintf(vNewBuf->Inout(), g_fmtMainStub, strFun.c_str(), strItem.c_str());
	LWPR::StringBuffer vStrBuf(*vNewBuf);
	newFile.FWRITE(vStrBuf);

	return true;
}

int main(int argc, char *argv[])
{
	if(!BuildserverCmdline::GetInstance()->ParseCmdlineOption(argc, argv, "Cs:vo:f:l:r:gE:tk"))
	{
		printf("Usage: %s [-s services] [-v] [-o outfile] [-f firstfiles] [-l lastfiles]\n", argv[0]);
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
	strCMPCMD += BuildserverCmdline::GetInstance()->GetOutfile();
	strCMPCMD += " ";

	strCMPCMD += "-L${TUXDIR}/lib ";

	std::string strFirstFiles;

	if(BuildserverCmdline::GetInstance()->GetFirstFiles(strFirstFiles))
	{
		strCMPCMD += strFirstFiles;
		strCMPCMD += " ";
	}

	std::string mainStubFile;
	try
	{
		if(GenMainFile(mainStubFile))
		{
			strCMPCMD += mainStubFile;
			strCMPCMD += " ";
		}
	}
	catch(...)
	{
		return -1;
	}

	strCMPCMD += "-ltux -llwpr ";

	std::string strLastFiles;
	if(BuildserverCmdline::GetInstance()->GetLastFiles(strLastFiles))
	{
		strCMPCMD += strLastFiles;
		strCMPCMD += " ";
	}

	if(BuildserverCmdline::GetInstance()->IsVerboseMode())
	{
		printf("%s\n", strCMPCMD.c_str());
	}

	int code = system(strCMPCMD.c_str());

	if(!BuildserverCmdline::GetInstance()->IsKeepMainStubFile())
	{
		remove(mainStubFile.c_str());
	}

	return code;
}
