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
#include "Viewc32Cmdline.h"


Viewc32Cmdline::Viewc32Cmdline()
{
}

Viewc32Cmdline* Viewc32Cmdline::GetInstance()
{
	static Viewc32Cmdline *singleton = new Viewc32Cmdline();
	return singleton;
}

bool Viewc32Cmdline::CheckCmdline()
{
	bool bRetResult = true;

	return bRetResult;
}

bool Viewc32Cmdline::IsNotMapFMLBuffer()
{
	return IsOptionValid('n');
}

bool Viewc32Cmdline::GetViewObjectFileDir(std::string& viewdir)
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('d', args))
	{
		viewdir = args[0].c_str();
		return true;
	}

	return false;
}

void Viewc32Cmdline::GetViewFiles(std::vector< std::string >& files)
{
	files = m_OtherArguments;
}
