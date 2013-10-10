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
#include "Fml32Util.h"
#include <lwpr.h>
#include <stdlib.h>
#include <string.h>

LWPR::UINT32 fml32_item_format_t::GetFieldID()
{
	if(type == FML32_DATA_TYPE_SHORT_STR)
	{
		return FML32_DATA_TYPE_SHORT_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_LONG_STR)
	{
		return FML32_DATA_TYPE_LONG_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_CHAR_STR)
	{
		return FML32_DATA_TYPE_CHAR_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_FLOAT_STR)
	{
		return FML32_DATA_TYPE_FLOAT_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_DOUBLE_STR)
	{
		return FML32_DATA_TYPE_DOUBLE_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_STRING_STR)
	{
		return FML32_DATA_TYPE_STRING_CODE + atoi(number.c_str());
	}
	else if(type == FML32_DATA_TYPE_CARRAY_STR)
	{
		return FML32_DATA_TYPE_CARRAY_CODE + atoi(number.c_str());
	}

	return 0;
}

Fml32Paser::Fml32Paser(const char* filename, const char* dir /*= NULL*/) :
	m_nBaseNum(0)
{
	assert(NULL != filename);

	if(dir != NULL)
	{
		m_strFmlGenDir = dir;
	}
	else
	{
		m_strFmlGenDir = LWPR::ProcessUtil::GetWorkingDir();
	}

	m_strFmlFileName = filename;
}

Fml32Paser::~Fml32Paser()
{

}

bool Fml32Paser::Parse()
{
	bool bResult = false;
	const int INT_MAX_LINE = 1024;

	// 打开文件
	FILE* fp = fopen(m_strFmlFileName.c_str(), "r");
	if(NULL == fp)
	{
		fprintf(stderr, "ERROR: unable to open fml %s\n", m_strFmlFileName.c_str());
		return false;
	}

	// 函数退出后，自动关闭文件
	AUTO_CLOSE_FILE(fp);

	// 开始读文件
	LWPR::Buffer bufLine(INT_MAX_LINE);
	LWPR::Buffer bufDoing(INT_MAX_LINE);
	for(int lineNUm = 1; !feof(fp); lineNUm++)
	{
		bufLine.Reset();

		// 读一行文本
		char* pLine = fgets(bufLine.Inout(), INT_MAX_LINE, fp);
		pLine = LWPR::StringUtil::TrimAll(pLine);

		// 文本行是否有效
		if(pLine == NULL)
		{
			break;
		}

		// 去掉注释
		int code = sscanf(pLine, "%[^#]", bufDoing.Inout());
		if(code != 1)
		{
			continue;
		}

		// 去除注释后是否有效
		if(strlen(pLine) == 0)
		{
			continue;
		}

		// 开始正文解析
		LWPR::Buffer bufItem[5];
		for(int i = 0; i < 5; i++)
		{
			bufItem[i].Capacity(INT_MAX_LINE);
			bufItem[i].Reset();
		}

		code = sscanf(bufDoing.Inout(), "%s %s %s %s %s"
		              , bufItem[0].Inout()
		              , bufItem[1].Inout()
		              , bufItem[2].Inout()
		              , bufItem[3].Inout()
		              , bufItem[4].Inout());

		switch(code)
		{
		case -1:// error
			fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: -1\n", lineNUm);
			return false;

		case 2:// *base 100
			if(!strcmp(bufItem[0].Inout(), "*base"))
			{
				m_nBaseNum = atoi(bufItem[1].Inout());
			}
			else
			{
				fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 2\n", lineNUm);
				return false;
			}

			break;

		case 5:// 0-name  1-number 2-type 3-flag 4-comment
			if((!strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_SHORT_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_LONG_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_CHAR_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_FLOAT_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_DOUBLE_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_STRING_STR)
			    || !strcmp(bufItem[2].Inout(), FML32_DATA_TYPE_CARRAY_STR))
			   && (atoi(bufItem[1].Inout()) + m_nBaseNum) <= FML32_DATA_TYPE_MAX_OFFSET)
			{
				FML32_ITEM_FORMAT_T item;
				item.name = bufItem[0].Inout();
				item.number = bufItem[1].Inout();
				item.type = bufItem[2].Inout();
				item.flag = bufItem[3].Inout();
				item.comment = bufItem[4].Inout();
				m_Fml32ItemVector.push_back(item);
				bResult = true;
			}
			else
			{
				fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 5\n", lineNUm);
				return false;
			}
			break;
		default:// error
			fprintf(stderr, "Error: The line %d is unrecognized.\n", lineNUm);
			return false;
		}
	}

	return bResult;
}

void Fml32Paser::Print()
{
	printf("*base = %d\n", m_nBaseNum);

	for(size_t i = 0; i < m_Fml32ItemVector.size(); i++)
	{
		printf("name = [%s] number = [%s]  type = [%s] flag = [%s] comment = [%s]\n"
		       , m_Fml32ItemVector[i].name.c_str()
		       , m_Fml32ItemVector[i].number.c_str()
		       , m_Fml32ItemVector[i].type.c_str()
		       , m_Fml32ItemVector[i].flag.c_str()
		       , m_Fml32ItemVector[i].comment.c_str());
	}
}

bool Fml32Paser::GenHeader()
{
	std::string headerName = GetFMLHeaderFileName();

	// 创建头文件
	FILE* fp = fopen(headerName.c_str(), "w");
	if(NULL == fp)
	{
		fprintf(stderr, "ERROR: %s is can't writen.\n", headerName.c_str());
		return false;
	}

	// 函数退出后，自动关闭文件
	AUTO_CLOSE_FILE(fp);

	// 写入可充入宏定义
	fprintf(fp, "#ifndef %s\n", LWPR::FileUtil::GenHeaderDef(headerName.c_str()).c_str());
	fprintf(fp, "#define %s\n\n", LWPR::FileUtil::GenHeaderDef(headerName.c_str()).c_str());

	// 写入正文
	for(size_t i = 0; i < m_Fml32ItemVector.size(); i++)
	{
		fprintf(fp, "#define %-32s ((FLDID32)%12u)  /* number: %-10d type: %s */\n"
		        , m_Fml32ItemVector[i].name.c_str()
		        , m_Fml32ItemVector[i].GetFieldID() + m_nBaseNum
		        , atoi(m_Fml32ItemVector[i].number.c_str()) + m_nBaseNum
		        , m_Fml32ItemVector[i].type.c_str());
	}

	fprintf(fp, "\n#endif /* end of %s */\n", LWPR::FileUtil::GenHeaderDef(headerName.c_str()).c_str());

	return true;
}

std::string Fml32Paser::GetFMLHeaderFileName()
{
	std::string name = LWPR::FileUtil::ChangeFileExtName(m_strFmlFileName.c_str(), "h");
	name = LWPR::FileUtil::GetBaseName(name.c_str());
	name = m_strFmlGenDir + "/" + name;
	return name;
}

bool Fml32Paser::GetFMLFilesFromEnv(std::vector< std::string >& files)
{
	std::vector< std::string > tbls; // .fml文件 FIELDTBLS32
	std::vector< std::string > dirs; // .fml文件 FLDTBLDIR32

	const char* pEnv = getenv("FIELDTBLS32");
	if (pEnv != NULL)
	{
		LWPR::StringUtil::SplitString(pEnv, ',', tbls);
	}

	pEnv = getenv("FLDTBLDIR32");
	if (pEnv != NULL)
	{
		LWPR::StringUtil::SplitString(pEnv, ':', dirs);
	}

	for (size_t i = 0; i < dirs.size(); i++)
	{
		for (size_t k = 0; k < tbls.size(); k++)
		{
			std::string strFile = dirs[i] + "/" + tbls[k];
			if(LWPR::FileUtil::IsFileExist(strFile.c_str()))
			{
				files.push_back(strFile);
			}
		}
	}

	return !files.empty();
}
