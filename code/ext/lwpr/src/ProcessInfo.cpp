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
#include "ProcessInfo.h"
#include <sys/types.h>
#include <dirent.h>

namespace LWPR
{
	bool ProcessInfo::GetProcessInfoByPID(PStatusMap& status, PID_T pid)
	{
		char strProcDir[128] = {0};
		sprintf(strProcDir, "/proc/%d/status", pid);

		FILE* fp = fopen(strProcDir, "r");
		if(fp == NULL)
		{
			return false;
		}

		char item[2][512];
		char buf[1024];
		while(!feof(fp))
		{
			memset(item, 0, sizeof(item));
			memset(buf, 0, sizeof(buf));
			if(fgets(buf, sizeof(buf) - 1, fp))
			{
				int count = sscanf(buf, "%s %s", item[0], item[1]);
				if(2 == count)
				{
					status[item[0]] = item[1];
				}
			}
		}

		fclose(fp);

		return true;
	}

	bool ProcessInfo::EnumCurrentUserProcess(PIDList& pids)
	{
		const char* UNIX_PROC_DIR = "/proc";
		DIR* fdDir;
		struct dirent* dirData;

		if((fdDir = opendir(UNIX_PROC_DIR)) != NULL)
		{
			while((dirData = readdir(fdDir)) != NULL)
			{
				int pid = atoi(dirData->d_name);
				if(pid > 0)
				{
					PStatusMap status;
					if(GetProcessInfoByPID(status, pid))
					{
						if(getuid() == (unsigned int)atoi(status["Uid:"].c_str()))
						{
							pids.push_back(pid);
						}
					}
				}
			}

			closedir(fdDir);
		}
		else
		{
			return false;
		}

		return true;
	}

	int ProcessInfo::GetProcessFDCount(PID_T pid)
	{
		int count = 0;
		char strProcDir[128] = {0};
		sprintf(strProcDir, "/proc/%d/fd", pid);

		DIR* fdDir;
		struct dirent* dirData;
		if((fdDir = opendir(strProcDir)) != NULL)
		{
			while((dirData = readdir(fdDir)) != NULL) count++;

			closedir(fdDir);
		}
		else
		{
			return -1;
		}

		count = count > 0 ? count - 2 : count;

		return count;
	}
};
