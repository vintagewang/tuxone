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
#include "ProcessUtil.h"
#include "StringBuffer.h"
#include "CFile.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace LWPR
{
	PID_T ProcessUtil::MakeProcess(const char* cmd)
	{
		assert(cmd != NULL && strlen(cmd) != 0);

		PID_T pid = fork();
		// child
		if(pid == 0)
		{
			int cmdlen = strlen(cmd);
			char* cmdbuf = (char *)malloc(cmdlen + 1);
			strcpy(cmdbuf, cmd);
			char *argv[128] = {0};

			int j = 0;
			argv[j++] = cmdbuf;
			for(int i = 0; i < cmdlen + 1 && j < 128; i++)
			{
				if(*(cmdbuf + i) == ' ')
				{
					*(cmdbuf + i) = 0;
					argv[j++] = cmdbuf + i + 1;
				}
			}

			argv[j] = 0;

			execv(argv[0], argv);

			exit(-1);
		}
		// father
		else if(pid > 0)
		{
		}

		return pid;
	}

	LWPR::PID_T ProcessUtil::MakeProcess(const std::vector<std::string>& cmd)
	{
		assert(cmd.size() > 0);

		PID_T pid = fork();
		// child
		if(pid == 0)
		{
			char **argv = new char*[cmd.size() + 1];
			memset(argv, 0, sizeof(char*) * (cmd.size() + 1));

			for(size_t i = 0; i < cmd.size(); i++)
			{
				argv[i] = (char *)cmd[i].c_str();

				if((i + 1) == cmd.size())
				{
					argv[i + 1] = NULL;
				}
			}

			execv(argv[0], argv);

			exit(-1);
		}
		// father
		else if(pid > 0)
		{
		}

		return pid;
	}

	LWPR::PID_T ProcessUtil::GetPID()
	{
		return getpid();
	}

	std::string ProcessUtil::GetPName()
	{
		std::string strResult = "Unknow";
#ifdef WIN32

#elif LINUX
		char bufProc[128] = {0};
		sprintf(bufProc, "/proc/%d/stat", getpid());

		FILE* fp = fopen(bufProc, "r");
		if(fp != NULL)
		{
			int pid;
			char bufExe[512] = {0};
			if(fscanf(fp, "%d (%[^)]", &pid, bufExe) == 2)
			{
				strResult = bufExe;
			}

			fclose(fp);
		}

#elif HPUX
#elif AIX
#elif SOALRIS
#endif

		return strResult;
	}

	bool ProcessUtil::IsProcessExist(PID_T pid)
	{
		bool bResult = (-1 == kill(pid, 0) && ESRCH == errno);
		return !bResult;
	}

	std::string ProcessUtil::GetWorkingDir()
	{
		StringBuffer buf(1024);

		if(NULL != getcwd(buf.str(), 1024))
		{
			return buf.str();
		}

		return "";
	}

	bool ProcessUtil::GetProcessCmdline(std::vector<std::string>& cmd)
	{
		return GetProcessCmdline(GetPID(), cmd);
	}

	bool ProcessUtil::GetProcessCmdline(PID_T pid, std::vector<std::string>& cmd)
	{
		bool bResult = true;
#ifdef WIN32
#elif LINUX
		char bufProc[128] = {0};
		sprintf(bufProc, "/proc/%d/cmdline", pid);

		FILE* fp = fopen(bufProc, "rb");
		bResult = bResult && (NULL != fp);

		const int CMD_MAX_SIZE = 1024 * 10;
		LWPR::Buffer bufCmd(CMD_MAX_SIZE);
		bResult = bResult && (fread(bufCmd.Inout(), 1, CMD_MAX_SIZE, fp) > 0);
		if(bResult)
		{
			char *args = bufCmd.Inout();
			for(int len = 0; (len = strlen(args)) != 0;)
			{
				cmd.push_back(args);
				args = args + len + 1;
			}
		}
#elif HPUX
#elif AIX
#elif SOALRIS
#endif

		return bResult;
	}
};
