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
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <lwpr.h>
#include "PmonCmdline.h"

using namespace std;

void PrintLog(const char* file, char* fmt, ...)
{
	char buf[512] = {0};
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, args);
	va_end(args);

	printf("%s\n", buf);

	if(!file) return;

	FILE* fp = fopen(file, "a");
	if(NULL == fp) return;

	fprintf(fp, "%s\n", buf);

	fclose(fp);
}

int main(int argc, char *argv[])
{
	if(!PmonCmdline::GetInstance()->ParseCmdlineOption(argc, argv, "d:i:"))
	{
		printf("Usage: %s [-d logdir] [-i statistics interval ] [pid...]\n", argv[0]);
		return -1;
	}

	LWPR::PIDList pids;
	unsigned int index = 0;

FETCH_PIDS:
	pids.clear();
	PmonCmdline::GetInstance()->GetPIDS(pids);
	if(pids.empty())
	{
		LWPR::ProcessInfo::EnumCurrentUserProcess(pids);
	}

	for(size_t i = 0; i < pids.size(); i++)
	{
		int nFDCount = 0;
		LWPR::ProcessInfo::PStatusMap status;
		bool bResult = LWPR::ProcessInfo::GetProcessInfoByPID(status, pids[i]);
		bResult = bResult && (nFDCount = LWPR::ProcessInfo::GetProcessFDCount(pids[i])) >= 0;
		if(bResult)
		{
			std::string strLogFile;
			std::string strPName;
			int nThreadCount = 0;
			long nVmSize = 0;
			long nVmRSS = 0;
			int nPID = 0;

			// 统计文件名
			strPName = status["Name:"];
			strLogFile = status["Name:"];
			strLogFile += ".";
			strLogFile += status["Pid:"];
			strLogFile += ".txt";
			strLogFile = PmonCmdline::GetInstance()->GetLogDir() + "/" + strLogFile;

			// 消耗虚拟内存大小
			nVmSize = atol(status["VmSize:"].c_str()) * 1024;

			// 消耗物理内存大小
			nVmRSS = atol(status["VmRSS:"].c_str()) * 1024;

			// 线程数目
			nThreadCount = atoi(status["Threads:"].c_str());

			// 进程ID
			nPID = atoi(status["Pid:"].c_str());

			// 打印
			//                                   时间 线程  物理内存     虚拟内存     FDS PID PNAME
			PrintLog(strLogFile.c_str(), "%10d  %-20s %5d  %10lld (%12s) %10lld (%12s) %5d %7d %-32s"
			         , index
			         , LWPR::Utility::DateTimeNow().c_str()
			         , nThreadCount
			         , nVmRSS
			         , LWPR::Utility::ByteToHumanFmt(nVmRSS).c_str()
			         , nVmSize
			         , LWPR::Utility::ByteToHumanFmt(nVmSize).c_str()
			         , nFDCount
			         , nPID
			         , strPName.c_str());
		}
	}

	LWPR::Thread::Sleep(PmonCmdline::GetInstance()->GetStatisticsInterval());
	index++;
	printf("\n");
	goto FETCH_PIDS;

	return 0;
}
