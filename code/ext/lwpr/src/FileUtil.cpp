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
#include "FileUtil.h"
#include "StringUtil.h"
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>


namespace LWPR
{
	bool FileUtil::IsFileExist(const char* fileName)
	{
		assert(fileName != NULL);

		struct stat bufStat = {0};
		bool bRetResult = true;

		bRetResult = (stat(fileName, &bufStat) != -1);
		bRetResult = bRetResult && (bufStat.st_size >= 0);

		return bRetResult;
	}

	char * FileUtil::BuildFile(const char *file)
	{
		char    *p = NULL;
		char    buf[512 + 1] =  {0};

		if(file != NULL)
		{
			for(p = (char*)file; abs((p = strchr(p, '/')) - file) < (int)strlen(file); p++)
			{
				memset(buf, 0, sizeof(buf));
				memcpy(buf, file, p - file);
				mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			}
		}

		return (char*)file;
	}

	char * FileUtil::BuildPath(const char *path)
	{
		BuildFile(path);

		mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		return (char*)path;
	}

	std::string FileUtil::GetBaseName(const char* file)
	{
		assert(NULL != file);

		return basename((char *)file);
	}

	std::string FileUtil::ChangeFileExtName(const char* file, const char* ext)
	{
		assert(NULL != file);
		assert(NULL != ext);

		std::string strFile = file;
		std::string::size_type pos = strFile.find_last_of('.');
		if(pos != std::string::npos)
		{
			strFile = strFile.substr(0, pos);
		}

		strFile += ".";
		strFile += ext;

		return strFile;
	}

	std::string FileUtil::GenHeaderDef(const char* file)
	{
		assert(NULL != file);
		std::string strFile = file;
		std::string::size_type pos = strFile.find_last_of('/');
		if(pos != std::string::npos)
		{
			strFile = strFile.substr(pos + 1);
		}

		pos = strFile.find_last_of('\\');
		if(pos != std::string::npos)
		{
			strFile = strFile.substr(pos + 1);
		}

		pos = strFile.find_last_of('.');
		if(pos != std::string::npos)
		{
			strFile = strFile.substr(0, pos);
		}

		strFile = StringUtil::ToUpper(strFile);
		strFile = "_" + strFile + "_H_";

		return strFile;
	}
};
