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
#include "Mkfldhdr32Cmdline.h"


Mkfldhdr32Cmdline::Mkfldhdr32Cmdline()
{
}

Mkfldhdr32Cmdline* Mkfldhdr32Cmdline::GetInstance()
{
	static Mkfldhdr32Cmdline *singleton = new Mkfldhdr32Cmdline();
	return singleton;
}

bool Mkfldhdr32Cmdline::CheckCmdline()
{
	bool bRetResult = true;

	return bRetResult;
}

bool Mkfldhdr32Cmdline::GetOutDir(std::string& dir)
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('d', args))
	{
		dir = args[0].c_str();
		return true;
	}

	return false;
}

void Mkfldhdr32Cmdline::GetFMLFiles(std::vector< std::string >& files)
{
	files = m_OtherArguments;
}
