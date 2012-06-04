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


BuildserverCmdline::BuildserverCmdline()
{
}

BuildserverCmdline* BuildserverCmdline::GetInstance()
{
	static BuildserverCmdline *singleton = new BuildserverCmdline();
	return singleton;
}

bool BuildserverCmdline::CheckCmdline()
{
	bool bRetResult = true;

	// tuxedo库之前或者之后，至少要有一项可用
	if(!IsOptionValid('f')
	   && !IsOptionValid('l'))
	{
		bRetResult = false;
	}

	// 远程库与资源管理器互斥
	if(IsOptionValid('w')
	   && IsOptionValid('r'))
	{
		bRetResult = false;
	}

	// 不允许有除选项外的其他参数
	if(m_OtherArguments.size() > 0)
	{
		bRetResult = false;
	}

	return bRetResult;
}

bool BuildserverCmdline::IsCOBOLCompilation()
{
	if(IsOptionValid('C'))
	{
		return true;
	}

	return false;
}

bool BuildserverCmdline::IsVerboseMode()
{
	if(IsOptionValid('v'))
	{
		return true;
	}

	return false;
}

bool BuildserverCmdline::GetRMNAME(std::string& rmname)
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('r', args))
	{
		rmname = args[0].c_str();
		return true;
	}

	return false;
}

std::string BuildserverCmdline::GetOutfile()
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('o', args))
	{
		return args[0];
	}
	else
	{
		return "a.out";
	}
}

bool BuildserverCmdline::GetFirstFiles(std::string& firstfiles)
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('f', args))
	{
		for(size_t i = 0; i < args.size(); i++)
		{
			if(i > 0)
			{
				firstfiles += " ";
			}

			firstfiles += args[i];
		}

		return true;
	}

	return false;
}

bool BuildserverCmdline::GetLastFiles(std::string& lastfiles)
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('l', args))
	{
		for(size_t i = 0; i < args.size(); i++)
		{
			if(i > 0)
			{
				lastfiles += " ";
			}

			lastfiles += args[i];
		}

		return true;
	}

	return false;
}

bool BuildserverCmdline::GetServiceNames(std::vector<std::string>& services)
{
	if(GetArgumentsByOption('s', services))
	{
		return true;
	}

	return false;
}

bool BuildserverCmdline::IsKeepMainStubFile()
{
	if(IsOptionValid('k'))
	{
		return true;
	}

	return false;
}

bool BuildserverCmdline::IsMultithreading()
{
	if(IsOptionValid('t'))
	{
		return true;
	}

	return false;
}
