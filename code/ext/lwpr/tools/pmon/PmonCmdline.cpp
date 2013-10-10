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
#include "PmonCmdline.h"
#include <stdlib.h>

PmonCmdline::PmonCmdline()
{
}

PmonCmdline* PmonCmdline::GetInstance()
{
	static PmonCmdline *singleton = new PmonCmdline();
	return singleton;
}

bool PmonCmdline::CheckCmdline()
{
	bool bRetResult = true;

	return bRetResult;
}

void PmonCmdline::GetPIDS(LWPR::PIDList& pids)
{
	for(size_t i = 0; i < m_OtherArguments.size(); i++)
	{
		pids.push_back(atoi(m_OtherArguments[i].c_str()));
	}
}

int PmonCmdline::GetStatisticsInterval()
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('i', args))
	{
		return atoi(args[0].c_str());
	}

	return 3;
}

std::string PmonCmdline::GetLogDir()
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('d', args))
	{
		return args[0].c_str();
	}

	return LWPR::ProcessUtil::GetWorkingDir();
}
