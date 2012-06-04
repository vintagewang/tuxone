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
#include "ProgramOption.h"
#include <unistd.h>

namespace LWPR
{
	ProgramOption::ProgramOption()
	{
	}

	ProgramOption::~ProgramOption()
	{
	}

	bool ProgramOption::ParseCmdlineOption(int argc, char **argv, const char *optstring)
	{
		bool bRetResult = true;

		// 初始化变量
		m_InvalidOptions.clear();
		m_Options.clear();
		m_OtherArguments.clear();
		m_strFirstArgument = argv[0];

		char ch;
		while((ch = getopt(argc, argv, optstring)) != -1)
		{
			switch(ch)
			{
			case '?':
				m_InvalidOptions.push_back(optopt);
				bRetResult = false;
				break;
			default:
				m_Options[ch];
				if(optarg != NULL)
				{
					m_Options[ch].push_back(optarg);
				}
			}
		}

		argc -= optind;
		argv += optind;

		bRetResult = argc < 0 ? false : bRetResult;

		if(bRetResult)
		{
			for(int i = 0; i < argc; i++)
			{
				m_OtherArguments.push_back(argv[i]);
			}

			bRetResult = CheckCmdline();
		}

		return bRetResult;
	}

	bool ProgramOption::CheckCmdline()
	{
		return true;
	}

	bool ProgramOption::IsOptionValid(char opt)
	{
		if(m_Options.find(opt) != m_Options.end())
		{
			return true;
		}

		return false;
	}

	bool ProgramOption::GetArgumentsByOption(char opt, std::vector< std::string >& args)
	{
		if(IsOptionValid(opt))
		{
			args = m_Options[opt];
		}

		return (args.size() > 0);
	}

	const std::string& ProgramOption::GetFirstArgument() const
	{
		return m_strFirstArgument;
	}
};
