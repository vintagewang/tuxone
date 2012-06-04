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
#include <lwpr.h>
#include <map>
#include <iostream>
#include <string>
#include "View32Util.h"

using std::cout;
using std::cerr;
using std::endl;

bool PrintViewObject(const char* file)
{
	assert(NULL != file);

	FILE_INFO_T info;
	bool bResult = View32Paser::LoadViewStruct(file, info);
	if(bResult)
	{
		printf("# VIEW OBJFILE = %s\n", info.fileName.c_str());

		for(size_t i = 0; i < info.structs.size(); i++)
		{
			printf("\nVIEW   %s\n\n", info.structs[i].structName.c_str());

			printf("# TYPE   CNAME        FBNAME     COUNT   FLAG  SIZE  NULL VALUE\n");
			printf("# -----  --------     ---------  -----   ----  ----  ----------\n\n");

			for(size_t k = 0; k < info.structs[i].items.size(); k++)
			{
				printf("%-9s%-13s%-11s%-8d%-6d%-6d%s\n"
				       , info.structs[i].items[k].GetTypeName().c_str()
				       , info.structs[i].items[k].cname.c_str()
				       , info.structs[i].items[k].fbname.c_str()
				       , info.structs[i].items[k].count
				       , info.structs[i].items[k].flag
				       , info.structs[i].items[k].size
				       , info.structs[i].items[k].nullvalue.c_str());

				printf("#                                                    DATA OFFSET = %d\n", info.structs[i].items[k].offset);
			}

			printf("\nEND\n");
		}
	}

	return bResult;
}

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("usage: %s viewobjfile [viewobjfile...]\n", argv[0]);
		return -1;
	}

	for(int i = 1; i < argc; i++)
	{
		PrintViewObject(argv[i]);
	}

	return 0;
}
