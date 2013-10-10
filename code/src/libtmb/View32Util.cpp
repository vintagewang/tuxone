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
#include "View32Util.h"
#include "viewobj.h"
#include <atmi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------
// struct STRUCT_ITEM_FORMAT_T
//----------------------------------------------------------------
struct_item_format_t::struct_item_format_t() :
	type(STRUCT_DATA_TYPE_CHAR),
	count(1),
	flag(0),
	size(0),
	offset(0)
{
}

struct_item_format_t::~struct_item_format_t()
{
}

void struct_item_format_t::marshal(LWPR::OutputStream& out)
{
	out.WriteINT8(type);
	out.WriteString(cname);
	out.WriteString(fbname);
	out.WriteUINT32(count);
	out.WriteUINT32(flag);
	out.WriteUINT32(size);
	out.WriteString(nullvalue);
	out.WriteUINT32(offset);
}

void struct_item_format_t::unmarshal(LWPR::InputStream& in)
{
	type = in.ReadINT8();
	cname = in.ReadString();
	fbname = in.ReadString();
	count = in.ReadUINT32();
	flag = in.ReadUINT32();
	size = in.ReadUINT32();
	nullvalue = in.ReadString();
	offset = in.ReadUINT32();
}

char struct_item_format_t::GetTypeByString(const char* str)
{
	assert(NULL != str);
	if(!strcmp(str, STRUCT_DATA_TYPE_CHAR_STR))
	{
		return STRUCT_DATA_TYPE_CHAR;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_SHORT_STR))
	{
		return STRUCT_DATA_TYPE_SHORT;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_INT_STR))
	{
		return STRUCT_DATA_TYPE_INT;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_LONG_STR))
	{
		return STRUCT_DATA_TYPE_LONG;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_FLOAT_STR))
	{
		return STRUCT_DATA_TYPE_FLOAT;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_DOUBLE_STR))
	{
		return STRUCT_DATA_TYPE_DOUBLE;
	}
	else if(!strcmp(str, STRUCT_DATA_TYPE_STRING_STR))
	{
		return STRUCT_DATA_TYPE_STRING;
	}
	else
	{
		fprintf(stderr, "ERROR: view type error\n");
		exit(-1);
	}

	assert(0);
	return STRUCT_DATA_TYPE_CHAR;
}

LWPR::UINT32 struct_item_format_t::GetSizeByType(char t, const char* sizevalue)
{
	assert(NULL != sizevalue);

	switch(t)
	{
	case STRUCT_DATA_TYPE_CHAR:
		return sizeof(char);
	case STRUCT_DATA_TYPE_SHORT:
		return sizeof(short);
	case STRUCT_DATA_TYPE_INT:
		return sizeof(int);
	case STRUCT_DATA_TYPE_LONG:
		return sizeof(long);
	case STRUCT_DATA_TYPE_FLOAT:
		return sizeof(float);
	case STRUCT_DATA_TYPE_DOUBLE:
		return sizeof(double);
	case STRUCT_DATA_TYPE_STRING:
		{
			int value = atoi(sizevalue);
			if(value <= 0)
			{
				fprintf(stderr, "ERROR: string must be set size.\n");
				exit(-1);
			}

			return value;
		}

	default:
		fprintf(stderr, "ERROR: type error.\n");
		exit(-1);
		return 0;
	}
}

std::string struct_item_format_t::GetTypeName()
{
	switch(type)
	{
	case STRUCT_DATA_TYPE_CHAR:
		return STRUCT_DATA_TYPE_CHAR_STR;
	case STRUCT_DATA_TYPE_SHORT:
		return STRUCT_DATA_TYPE_SHORT_STR;
	case STRUCT_DATA_TYPE_INT:
		return STRUCT_DATA_TYPE_INT_STR;
	case STRUCT_DATA_TYPE_LONG:
		return STRUCT_DATA_TYPE_LONG_STR;
	case STRUCT_DATA_TYPE_FLOAT:
		return STRUCT_DATA_TYPE_FLOAT_STR;
	case STRUCT_DATA_TYPE_DOUBLE:
		return STRUCT_DATA_TYPE_DOUBLE_STR;
	case STRUCT_DATA_TYPE_STRING:
		return STRUCT_DATA_TYPE_STRING_STR;
	default:
		return "";
	}
}
//----------------------------------------------------------------
// struct STRUCT_ITEM_NET_T
//----------------------------------------------------------------
struct_item_net_t::struct_item_net_t():
	type(STRUCT_DATA_TYPE_CHAR),
	count(0),
	flag(0),
	size(0),
	offset(0)
{
}

struct_item_net_t::~struct_item_net_t()
{
}

void struct_item_net_t::marshal(LWPR::OutputStream& out)
{
	out.WriteINT8(type);
	out.WriteUINT32(count);
	out.WriteUINT32(flag);
	out.WriteUINT32(size);
	out.WriteUINT32(offset);
}

void struct_item_net_t::unmarshal(LWPR::InputStream& in)
{
	type = in.ReadINT8();
	count = in.ReadUINT32();
	flag = in.ReadUINT32();
	size = in.ReadUINT32();
	offset = in.ReadUINT32();
}

//----------------------------------------------------------------
// struct STRUCT_INFO_T
//----------------------------------------------------------------
struct_info_t::struct_info_t() :
	structSize(0)
{

}

struct_info_t::~struct_info_t()
{

}

void struct_info_t::marshal(LWPR::OutputStream& out)
{
	out.WriteString(structName);
	out.WriteUINT32(structSize);
	out.WriteUINT32(items.size());

	for(size_t i = 0; i < items.size(); i++)
	{
		items[i].marshal(out);
	}
}

void struct_info_t::unmarshal(LWPR::InputStream& in)
{
	structName = in.ReadString();
	structSize = in.ReadUINT32();
	LWPR::UINT32 len = in.ReadUINT32();

	for(size_t i = 0; i < (size_t)len; i++)
	{
		STRUCT_ITEM_FORMAT_T item;
		item.unmarshal(in);
		items.push_back(item);
	}
}

//----------------------------------------------------------------
// struct file_info_t
//----------------------------------------------------------------
file_info_t::file_info_t()
{

}

file_info_t::~file_info_t()
{

}

void file_info_t::marshal(LWPR::OutputStream& out)
{
	out.WriteString(fileName);
	out.WriteUINT32(structs.size());

	for(size_t i = 0; i < structs.size(); i++)
	{
		structs[i].marshal(out);
	}
}

void file_info_t::unmarshal(LWPR::InputStream& in)
{
	fileName = in.ReadString();
	LWPR::UINT32 len = in.ReadUINT32();

	for(size_t i = 0; i < (size_t)len; i++)
	{
		STRUCT_INFO_T item;
		item.unmarshal(in);
		structs.push_back(item);
	}
}

int file_info_t::GetItemTotal()
{
	int itemTotal = 0;
	for(size_t i = 0; i < structs.size(); i++)
	{
		itemTotal += structs[i].items.size();
	}

	return itemTotal;
}

//----------------------------------------------------------------
// struct View32Paser
//----------------------------------------------------------------
View32Paser::View32Paser(const char* filename, bool map_fml32, const char* dir /*= NULL*/) :
	m_strViewFileName(filename),
	m_bMapFml32(map_fml32)
{
	assert(NULL != filename);
	if(dir != NULL)
	{
		m_strViewGenDir = dir;
	}
	else
	{
		m_strViewGenDir = LWPR::ProcessUtil::GetWorkingDir();
	}

	m_FileInfo.fileName = LWPR::FileUtil::GetBaseName(m_strViewFileName.c_str());
}

View32Paser::~View32Paser()
{

}

bool View32Paser::Parse()
{
	return ParseTextToStruct() && GenViewHeader() && GenViewBinary();
}

bool View32Paser::LoadViewStruct(const char* filename, FILE_INFO_T& info)
{
	assert(NULL != filename);

	bool bResult = true;

	try
	{
		LWPR::CFile fp(filename, "rb");
		LWPR::Buffer_var bufFile = fp.LoadFileContent();
		LWPR::InputStream in(bufFile);
		info.unmarshal(in);
	}
	catch(...)
	{
		bResult = false;
	}

	return bResult;
}

bool View32Paser::ParseTextToStruct()
{
	bool bResult = false;
	const int INT_MAX_LINE = 1024;
	STRUCT_INFO_T structInfo;

	typedef enum read_section_type_e
	{
		SPACE_SECTION,
		STRUCT_SECTION
	} READ_SECTION_TYPE_E;

	READ_SECTION_TYPE_E runSection = SPACE_SECTION;

	// 打开文件
	FILE* fp = fopen(m_strViewFileName.c_str(), "r");
	if(NULL == fp)
	{
		fprintf(stderr, "ERROR: unable to open viewfile %s\n", m_strViewFileName.c_str());
		return false;
	}

	// 函数退出后，自动关闭文件
	AUTO_CLOSE_FILE(fp);

	// 开始读文件
	LWPR::Buffer bufLine(INT_MAX_LINE);
	LWPR::Buffer bufDoing(INT_MAX_LINE);
	int structItemNum = 0;
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

		// 当前行是否以$开头
		if(*pLine == '$')
		{
			LINE_CONTENT_T line = {COMMENT_LABEL, pLine + 1};
			m_ValidLine.push_back(line);
			continue;
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
		LWPR::Buffer bufItem[7];
		for(int i = 0; i < 7; i++)
		{
			bufItem[i].Capacity(INT_MAX_LINE);
			bufItem[i].Reset();
		}

		code = sscanf(bufDoing.Inout(), "%s %s %s %s %s %s %s"
		              , bufItem[0].Inout()
		              , bufItem[1].Inout()
		              , bufItem[2].Inout()
		              , bufItem[3].Inout()
		              , bufItem[4].Inout()
		              , bufItem[5].Inout()
		              , bufItem[6].Inout());

		switch(code)
		{
		case -1:// error
			fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: -1\n", lineNUm);
			return false;

		case 1:// END
			if(!strcmp(bufItem[0].Inout(), "END")
			   && runSection == STRUCT_SECTION
			   && structItemNum != 0)
			{
				// 生成头文件相关
				LINE_CONTENT_T line = {STRUCT_END_LABEL, "};"};
				m_ValidLine.push_back(line);
				structItemNum = 0;
				runSection = SPACE_SECTION;
				bResult = true;

				// 生成二进制文件相关
				m_FileInfo.structs.push_back(structInfo);
			}
			else
			{
				fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 1\n", lineNUm);
				return false;
			}
			break;

		case 2:// VIEW aud
			if(!strcmp(bufItem[0].Inout(), "VIEW")
			   && runSection == SPACE_SECTION
			   && structItemNum == 0)
			{
				// 生成头文件相关
				std::string name = "struct ";
				name += bufItem[1].Inout();
				name += " {";
				LINE_CONTENT_T line = {STRUCT_BEGIN_LABEL, name};
				m_ValidLine.push_back(line);
				structItemNum = 0;
				runSection = STRUCT_SECTION;

				// 生成二进制文件相关
				structInfo.structName = bufItem[1].Inout();
				structInfo.items.clear();
			}
			else
			{
				fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 2\n", lineNUm);
				return false;
			}

			break;

		case 7:// 0-type  1-cname 2-fbname 3-count 4-flag 5-size 6-null
			{
				LWPR::Buffer bufTmp(INT_MAX_LINE);

				if(runSection != STRUCT_SECTION)
				{
					fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 7\n", lineNUm);
					return false;
				}

				if(!strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_CHAR_STR)
				   || !strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_SHORT_STR)
				   || !strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_INT_STR)
				   || !strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_LONG_STR)
				   || !strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_FLOAT_STR)
				   || !strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_DOUBLE_STR))
				{
					// 生成头文件相关
					LINE_CONTENT_T line;
					line.type = STRUCT_ITEM_LABEL;
					line.value = bufItem[0].Inout();
					line.value += "\t";
					line.value += bufItem[1].Inout();

					int count = atoi(bufItem[3].Inout());
					if(count > 1)
					{
						line.value += "[";
						line.value += LWPR::StringUtil::IntToStr(count);
						line.value += "]";
					}

					line.value += ";";

					m_ValidLine.push_back(line);
				}
				else if(!strcmp(bufItem[0].Inout(), STRUCT_DATA_TYPE_STRING_STR))
				{
					// 生成头文件相关
					LINE_CONTENT_T line;
					line.type = STRUCT_ITEM_LABEL;
					line.value = "char\t";
					line.value += bufItem[1].Inout();

					int count = atoi(bufItem[3].Inout());
					if(count > 1)
					{
						line.value += "[";
						line.value += LWPR::StringUtil::IntToStr(count);
						line.value += "]";
					}

					int size = atoi(bufItem[5].Inout());
					if(size > 0)
					{
						line.value += "[";
						line.value += LWPR::StringUtil::IntToStr(size);
						line.value += "]";
					}
					else
					{
						fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 7\n", lineNUm);
						return false;
					}

					line.value += ";";

					m_ValidLine.push_back(line);
				}
				else
				{
					fprintf(stderr, "Error: The line %d is unrecognized. ERRCODE: 7\n", lineNUm);
					return false;
				}

				// 生成二进制文件相关
				STRUCT_ITEM_FORMAT_T item;
				item.type = STRUCT_ITEM_FORMAT_T::GetTypeByString(bufItem[0].Inout());
				item.cname = bufItem[1].Inout();
				item.fbname = bufItem[2].Inout();
				item.count = *bufItem[3].Inout() == '-' ? 1 : atoi(bufItem[3].Inout());
				item.count = item.count < 1 ? 1 : item.count;
				item.flag = *bufItem[4].Inout() == '-' ? 0 : atoi(bufItem[4].Inout());
				item.size = STRUCT_ITEM_FORMAT_T::GetSizeByType(item.type, bufItem[5].Inout());
				item.nullvalue = bufItem[6].Inout();
				structInfo.items.push_back(item);

				structItemNum++;
			}
			break;
		default:// error
			fprintf(stderr, "Error: The line %d is unrecognized.\n", lineNUm);
			return false;
		}
	}

	return bResult;
}

bool View32Paser::GenViewHeader()
{
	std::string headerName = GetViewHeaderFileName();

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
	fprintf(fp, "#define %s\n", LWPR::FileUtil::GenHeaderDef(headerName.c_str()).c_str());

	// 写入正文
	for(size_t i = 0; i < m_ValidLine.size(); i++)
	{
		switch(m_ValidLine[i].type)
		{
		case COMMENT_LABEL:
			fprintf(fp, "%s\n", m_ValidLine[i].value.c_str());
			break;
		case STRUCT_BEGIN_LABEL:
			fprintf(fp, "\n%s\n", m_ValidLine[i].value.c_str());
			break;
		case STRUCT_ITEM_LABEL:
			fprintf(fp, "\t%s\n", m_ValidLine[i].value.c_str());
			break;
		case STRUCT_END_LABEL:
			fprintf(fp, "%s\n\n", m_ValidLine[i].value.c_str());
			break;
		default:
			break;
		}
	}

	fprintf(fp, "#endif /* end of %s */\n", LWPR::FileUtil::GenHeaderDef(headerName.c_str()).c_str());

	return true;
}

bool View32Paser::GenViewBinary()
{
	return SaveFileInfo() && GenTestProgram();
}

bool View32Paser::SaveFileInfo()
{
	// 第一步，将view信息写入到临时文件
	FILE* fp = fopen(VIEW_BIN_TMP_FILE_NAME, "wb");
	if(NULL == fp)
	{
		fprintf(stderr, "ERROR: %s can't writen.\n", VIEW_BIN_TMP_FILE_NAME);
		return false;
	}

	// 函数退出后，自动关闭文件
	AUTO_CLOSE_FILE(fp);

	// 向文件写入数据
	LWPR::OutputStream out;
	m_FileInfo.marshal(out);

	if(1 != fwrite(out.GetStream(), out.GetStreamLength(), 1, fp))
	{
		fprintf(stderr, "ERROR: %s can't writen.\n", VIEW_BIN_TMP_FILE_NAME);
		return false;
	}

	return true;
}

bool View32Paser::GenTestProgram()
{
	LWPR::Buffer bufLine(1024);
	std::string strContent;

	// 定义结构体
	for(size_t i = 0; i < m_FileInfo.structs.size(); i++)
	{
		bufLine.Reset();
		sprintf(bufLine.Inout(), "\tstruct %s _%s;\n",
		        m_FileInfo.structs[i].structName.c_str(),
		        m_FileInfo.structs[i].structName.c_str());

		strContent += bufLine.Inout();
	}

	strContent += "\n\n";

	// 计算每个结构体的大小
	for(size_t i = 0; i < m_FileInfo.structs.size(); i++)
	{
		bufLine.Reset();
		sprintf(bufLine.Inout(), "\tstruct_size[size_index++] = sizeof(_%s);\n",
		        m_FileInfo.structs[i].structName.c_str());

		strContent += bufLine.Inout();
	}

	strContent += "\n\n";

	// 计算每个结构体成员的偏移量
	for(size_t i = 0; i < m_FileInfo.structs.size(); i++)
	{
		for(size_t k = 0; k < m_FileInfo.structs[i].items.size(); k++)
		{
			bufLine.Reset();
			sprintf(bufLine.Inout(), "\tstruct_offset[offset_index++] = (long)&(_%s.%s) - (long)&_%s;\n"
			        , m_FileInfo.structs[i].structName.c_str()
			        , m_FileInfo.structs[i].items[k].cname.c_str()
			        , m_FileInfo.structs[i].structName.c_str());

			strContent += bufLine.Inout();
		}

		strContent += "\n";
	}

	// 生成代码
	LWPR::Buffer bufBody(2048 + strContent.length());
	bufBody.Reset();

	sprintf(bufBody.Inout(), g_fmtViewObj
	        , GetViewHeaderFileName().c_str()
	        , m_FileInfo.structs.size()
	        , m_FileInfo.GetItemTotal()
	        , strContent.c_str()
	        , ("\"" + GetViewBinFileName() + "\"").c_str());

	// 将代码写入文件
	{
		FILE* fp = fopen("__viewobj.c", "w");
		if(NULL == fp)
		{
			fprintf(stderr, "ERROR: __viewobj.c can't writen.\n");
			return false;
		}

		// 函数退出后，自动关闭文件
		AUTO_CLOSE_FILE(fp);

		fprintf(fp, "%s\n", bufBody.Inout());
	}

	// 调用buildclient编译
	int status = system("buildclient -o __viewobj -f __viewobj.c");
	bool result = WIFEXITED(status) && WEXITSTATUS(status) == 0;
	if(result)
	{
		status = system((LWPR::ProcessUtil::GetWorkingDir() + "/__viewobj").c_str());
		result = WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}

	// 删除临时文件
	remove("__viewobj.c");
	remove("__viewobj");
	remove(VIEW_BIN_TMP_FILE_NAME);

	return result;
}

void View32Paser::Print()
{
	printf("--valid line------------------------------\n");
	for(size_t i = 0; i < m_ValidLine.size(); i++)
	{
		printf("\t%d %s\n", m_ValidLine[i].type, m_ValidLine[i].value.c_str());
	}

	printf("--struct data------------------------------\n");
	printf("\t%s\n", m_FileInfo.fileName.c_str());
	for(size_t i = 0; i < m_FileInfo.structs.size(); i++)
	{
		printf("\t\t%s\n", m_FileInfo.structs[i].structName.c_str());

		for(size_t k = 0; k < m_FileInfo.structs[i].items.size(); k++)
		{
			printf("\t\t\t%c %s %s %d %d %d %s %d\n"
			       , m_FileInfo.structs[i].items[k].type
			       , m_FileInfo.structs[i].items[k].cname.c_str()
			       , m_FileInfo.structs[i].items[k].fbname.c_str()
			       , m_FileInfo.structs[i].items[k].count
			       , m_FileInfo.structs[i].items[k].flag
			       , m_FileInfo.structs[i].items[k].size
			       , m_FileInfo.structs[i].items[k].nullvalue.c_str()
			       , m_FileInfo.structs[i].items[k].offset);
		}
	}
}

std::string View32Paser::GetViewBinFileName()
{
#ifdef WIN32
	std::string name = LWPR::FileUtil::ChangeFileExtName(m_strViewFileName.c_str(), "VV");
#else
	std::string name = LWPR::FileUtil::ChangeFileExtName(m_strViewFileName.c_str(), "V");
#endif

	name = LWPR::FileUtil::GetBaseName(name.c_str());
	name = m_strViewGenDir + "/" + name;

	return name;
}

std::string View32Paser::GetViewHeaderFileName()
{
	std::string name = LWPR::FileUtil::ChangeFileExtName(m_strViewFileName.c_str(), "h");
	name = LWPR::FileUtil::GetBaseName(name.c_str());
	name = m_strViewGenDir + "/" + name;

	return name;
}
/**
 * 对外提供的API
 */
extern "C" int LIBTUX_EXPORT genviewbinfile(const char* file,
        int* struct_size,
        int struct_len,
        int* item_offset,
        int item_len)
{
	assert(NULL != file);
	assert(NULL != struct_size);
	assert(NULL != item_offset);
	assert(struct_len > 0);
	assert(item_len > 0);

	bool bResult = false;

	try
	{
		// 加载文件，并校验
		FILE_INFO_T viewDataLast;
		bResult = View32Paser::LoadViewStruct(VIEW_BIN_TMP_FILE_NAME, viewDataLast);
		bResult = bResult && struct_len == (int)viewDataLast.structs.size();
		bResult = bResult && item_len == (int)viewDataLast.GetItemTotal();

		if(bResult)
		{
			// 更新文件
			int item_index = 0;
			for(size_t i = 0; i < viewDataLast.structs.size(); i++)
			{
				viewDataLast.structs[i].structSize = struct_size[i];

				for(size_t k = 0; k < viewDataLast.structs[i].items.size(); k++)
				{
					viewDataLast.structs[i].items[k].offset = item_offset[item_index++];
				}
			}

			// 并生成.v文件
			FILE* fp = fopen(file, "wb");
			if(NULL == fp)
			{
				fprintf(stderr, "ERROR: %s can't writen.\n", file);
				return false;
			}

			// 函数退出后，自动关闭文件
			AUTO_CLOSE_FILE(fp);

			// 向文件写入数据
			LWPR::OutputStream out;
			viewDataLast.marshal(out);
			if(1 != fwrite(out.GetStream(), out.GetStreamLength(), 1, fp))
			{
				fprintf(stderr, "ERROR: %s can't writen.\n", file);
				return false;
			}
		}
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
		bResult = false;
	}
	catch(...)
	{
		fprintf(stderr, "UNKNOW ERROR\n");
		bResult = false;
	}

	return bResult ? 0 : -1;
}
