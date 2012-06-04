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
#ifndef LWPR_PROCESS_UTIL_H__
#define LWPR_PROCESS_UTIL_H__
#include "LWPRType.h"
#include "Buffer.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>

namespace LWPR
{
	class ProcessUtil
	{
	public:
		static PID_T MakeProcess(const char* cmd);
		static PID_T MakeProcess(const std::vector<std::string>& cmd);
		static PID_T GetPID();
		static std::string GetPName();
		static bool IsProcessExist(PID_T pid);
		static std::string GetWorkingDir();

		static bool GetProcessCmdline(std::vector<std::string>& cmd);

		static bool GetProcessCmdline(PID_T pid, std::vector<std::string>& cmd);

	private:
	};
};

#endif // end of LWPR_PROCESS_UTIL_H__
