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
#include "View32Util.h"
#include <lwpr.h>
#include <map>
#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;

int run(int argc, char *argv[])
{
	if(!Viewc32Cmdline::GetInstance()->ParseCmdlineOption(argc, argv, "nd:"))
	{
PRINT_HELP:
		printf("Usage: %s [-n] [-d viewdir] viewfile [viewfile...]\n", argv[0]);
		return -1;
	}

	// 取要编译的.v文件
	std::vector<std::string> viewFiles;
	Viewc32Cmdline::GetInstance()->GetViewFiles(viewFiles);
	if(viewFiles.empty())
	{
		goto PRINT_HELP;
	}

	// 生成所在目录
	std::string viewdir;
	bool hasViewdir = Viewc32Cmdline::GetInstance()->GetViewObjectFileDir(viewdir);

	// 开始编译
	bool parseResult = true;
	for(size_t i = 0; i < viewFiles.size() && parseResult; i++)
	{
		View32Paser parser(viewFiles[i].c_str(),
		                   !Viewc32Cmdline::GetInstance()->IsNotMapFMLBuffer(),
		                   hasViewdir ? viewdir.c_str() : NULL);

		printf("# ... processing %s ...\n\n", viewFiles[i].c_str());
		parseResult = parser.Parse();
	}

	return parseResult ? 0 : -1;
}

int main(int argc, char *argv[])
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
