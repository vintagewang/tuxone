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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <lwpr.h>
#include "BulletinBoard.h"
#include "Message.h"

using std::cout;
using std::cerr;
using std::endl;

static void DoSignalHandle(int sig)
{
	printf("\nCatched signal %d, exited.\n", sig);
	exit(0);
}

#if 0
static void PrintBB(BB_STREAM_T& bb)
{
	// 打印header
	printf("nMagicCode [%d]\n", bb.pHeader->nMagicCode);
	printf("nSEMID [%d]\n", bb.pHeader->nSEMID);
	printf("nSVCUpdateSeq [%d]\n", bb.pHeader->nSVCUpdateSeq);
	printf("nGatePID [%d]\n", bb.pHeader->nGatePID);
	printf("nUDPPort [%d]\n", bb.pHeader->nUDPPort);
	printf("tServerAddr [%s]\n", LWPR::Socket::AddrTransform(bb.pHeader->tServerAddr).c_str());

	// 打印wsh表
	printf("-------------------------------------------------------------------\n");
	for(int i = 0; i < bb.tblWSHLength; i++)
	{
		if(bb.tblWSH[i].nSlotState != BBSLOT_IDLE)
		{
			printf("WSH TABLE [%d] nIndex [%d]\n", i, bb.tblWSH[i].nIndex);
			printf("WSH TABLE [%d] nPID [%d]\n", i, bb.tblWSH[i].nPID);
			printf("WSH TABLE [%d] nConnectionTotal [%d]\n", i, bb.tblWSH[i].nConnectionTotal);
			printf("WSH TABLE [%d] nPort [%d]\n", i, bb.tblWSH[i].nPort);
			printf("WSH TABLE [%d] nSlotState [%d]\n\n", i, bb.tblWSH[i].nSlotState);
		}
	}

	// 打印service表
	printf("-------------------------------------------------------------------\n");
	for(int i = 0; i < bb.tblSVCLength; i++)
	{
		if(bb.tblSVC[i].nSlotState != BBSLOT_IDLE)
		{
			printf("SERVICE TABLE [%d] nIndex [%d]\n", i, bb.tblSVC[i].nIndex);
			printf("SERVICE TABLE [%d] strSvcName [%s]\n", i, bb.tblSVC[i].strSvcName);
			std::string addr;
			LWPR::Socket::AddrTransform(bb.tblSVC[i].tServerAddr, addr);
			printf("SERVICE TABLE [%d] tServerAddr [%s]\n", i, addr.c_str());
			printf("SERVICE TABLE [%d] nBelongto [%d]\n", i, bb.tblSVC[i].nBelongto);
			printf("SERVICE TABLE [%d] nSlotState [%d]\n\n", i, bb.tblSVC[i].nSlotState);
		}
	}
}

#endif
static void bbs()
{
	BB_STREAM_T stream;
	BulletinBoard::GetInstance()->GetBBStream(stream, true);

	// Service Table
	if(stream.tblSVCLength)
	{
		printf("\n%-16s %-22s\n", "Service Name", "Server Address");
		printf("---------------- ----------------------\n");
		for(int i = 0; i < stream.tblSVCLength; i++)
		{
			printf("%-16s %-22s\n", stream.tblSVC[i].strSvcName, LWPR::Socket::AddrTransform(stream.tblSVC[i].tServerAddr).c_str());
		}
	}
	else
	{
		printf("No Server Registered To The Bulletin Board\n");
	}
}

static void help()
{
	printf("bbs - Print Bulletin Board Detail.\n");
}

static int run(int argc, char** argv)
{
	// 信号处理
	LWPR::Utility::RegisterSignal(SIGINT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGTERM, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGHUP, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGQUIT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGCHLD, SIG_IGN);
	LWPR::Utility::RegisterSignal(SIGPIPE, SIG_IGN);

	// 打印版权信息
	cout << "TUXONE tmadmin version 0.0.1 Copyright vintage.wang@gmail.com <http://tuxone.googlecode.com>" << endl;

	// 遍历系统中所有可访问的Gate
	BBHeaderMap headers;
	BulletinBoard::FetchGateIPCID(headers);

	// 选择其中一个连接
	LWPR::IPCID_T ipcid = -1;
	switch(headers.size())
	{
	case 0:
		cout << "No Tuxone Gate In current system." << endl;
		return 0;
	case 1:
		ipcid = headers.begin()->first;
		break;
		// >0
	default:
		{
			printf("There are %d gates in current system. Choose one please.\n\n", headers.size());

			LWPR::IPCIDList ids;
			int i = 1;
			for(BBHeaderMap::iterator it = headers.begin(); it != headers.end(); it++, i++)
			{
				printf("\t%d - %s\n", i, LWPR::Socket::AddrTransform(it->second.tServerAddr).c_str());
				ids.push_back(it->first);
			}

			int chooseNum = -1;
			while(chooseNum == -1)
			{
				printf("\nEnter a number:");
				fflush(stdout);

				char bufInput[128] = {0};
				const char* pInput = fgets(bufInput, sizeof(bufInput) - 1, stdin);
				if(NULL != pInput)
				{
					int num = atoi(bufInput);
					if(num >= 1 && num <= (int)headers.size())
					{
						chooseNum = num;
					}
				}
			}

			ipcid = ids[chooseNum - 1];
		}
	}

	// 连接公告板
	BulletinBoard::GetInstance()->Connect(ipcid);
	BB_HEADER_ENTITY_T connectedHeader;
	BulletinBoard::GetInstance()->GetHeader(connectedHeader);

	printf("Atatched To The Gate <%s> Successfully.\n", LWPR::Socket::AddrTransform(connectedHeader.tServerAddr).c_str());

	// 接收用户的输入
	std::string cmdLast;
	while(true)
	{
		printf("\n> ");
		fflush(stdout);

		char bufInput[512] = {0};
		const char* pInput = fgets(bufInput, sizeof(bufInput) - 1, stdin);
REPEAT:
		if(pInput != NULL)
		{
			LWPR::StringUtil::TrimAll(bufInput);
			if(!strcmp(pInput, "bbs"))
			{
				cmdLast = pInput;
				bbs();
			}
			else if(!strcmp(pInput, "help")
			        || !strcmp(pInput, "h"))
			{
				cmdLast = pInput;
				help();
			}
			else if(!strcmp(pInput, "exit")
			        || !strcmp(pInput, "q"))
			{
				return 0;
			}
			else if(strlen(pInput) == 0)
			{
				if(cmdLast.length() != 0)
				{
					pInput = cmdLast.c_str();
					goto REPEAT;
				}
			}
			else
			{
				printf("ERROR:No such command.\n");
			}
		}
	}

	// 断开与公告板的连接
	BulletinBoard::GetInstance()->Disconnect();

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(LWPR::Exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(std::exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
