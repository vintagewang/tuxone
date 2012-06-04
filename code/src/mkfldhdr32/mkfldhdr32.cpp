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
#include "Fml32Util.h"
#include <lwpr.h>
#include <map>
#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;

int run(int argc, char *argv[])
{
	if(!Mkfldhdr32Cmdline::GetInstance()->ParseCmdlineOption(argc, argv, "d:"))
	{
PRINT_HELP:
		printf("usage: %s [-d target-directory] [field_table ...]\n", argv[0]);
		return -1;
	}

	// 取要编译的.v文件
	std::vector<std::string> fmlFiles;
	Mkfldhdr32Cmdline::GetInstance()->GetFMLFiles(fmlFiles);
	if(fmlFiles.empty() && !Fml32Paser::GetFMLFilesFromEnv(fmlFiles))
	{
		goto PRINT_HELP;
	}

	// 生成所在目录
	std::string outDir;
	bool hasOutDir = Mkfldhdr32Cmdline::GetInstance()->GetOutDir(outDir);

	// 开始编译
	bool parseResult = true;
	for(size_t i = 0; i < fmlFiles.size() && parseResult; i++)
	{
		Fml32Paser parser(fmlFiles[i].c_str(),
		                  hasOutDir ? outDir.c_str() : NULL);

		parseResult = parser.Parse();
		parseResult = parseResult && parser.GenHeader();
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
