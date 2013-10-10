/*
 * $Id: genconfig.cpp 126 2011-03-17 13:05:38Z vintage $
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <lwpr.h>
#include "ConfigTemplate.h"


using namespace std;

typedef struct config_item_t
{
	std::string type;
	std::string name;
	std::string value;
	bool isPublic;
	std::string range;
} CONFIG_ITEM_T;

std::vector< CONFIG_ITEM_T > g_ConfigItems;

typedef std::vector< std::string > EnumItem;

typedef std::map< std::string, EnumItem > EnumTable;

EnumTable g_EnumTable;

void ParseFile(const char* file)
{
	static char bufLine[1024];
	static char bufTmp[1024];
	static char bufItem[5][256];
	assert(file != NULL);


	FILE* fp = fopen(file, "r");
	if(fp == NULL)
	{
		cerr << "fopen error" << endl;
		return;
	}

	while(!feof(fp))
	{
		memset(bufLine, 0, sizeof(bufLine));
		memset(bufTmp, 0, sizeof(bufTmp));
		memset(bufItem, 0, sizeof(bufItem));
		if(fgets(bufLine, sizeof(bufLine) - 1, fp) != NULL)
		{
			// 去掉空格，换行等
			LWPR::StringUtil::TrimAll(bufLine);

			// 去掉注释
			int code = sscanf(bufLine, "%[^#\n]", bufTmp);
			if(code != 1)
			{
				continue;
			}

			// 获取有效配置
			CONFIG_ITEM_T item;
			code = sscanf(bufTmp, "%s%s%s%s%s", bufItem[0], bufItem[1], bufItem[2], bufItem[3], bufItem[4]);
			if(4 == code || 5 == code)
			{
				item.type = bufItem[0];
				item.name = bufItem[1];
				item.value = bufItem[2];
				item.isPublic = strcmp(bufItem[3], "true") == 0;
				item.range = bufItem[4];
				g_ConfigItems.push_back(item);
			}
		}
	}

	fclose(fp);
}

void ParseEnum()
{
	for(size_t i = 0; i < g_ConfigItems.size(); i++)
	{
		if(g_ConfigItems[i].type == "enum")
		{
			LWPR::StringUtil::SplitString(g_ConfigItems[i].value, ',', g_EnumTable[g_ConfigItems[i].name]);
		}
	}
}

std::string FixParam(const CONFIG_ITEM_T& item)
{
	std::vector< std::string > sections;
	std::string strGenCode;

	LWPR::StringUtil::SplitString(item.range, ',', sections);

	if(sections.size() != 2)
	{
		fprintf(stderr, "%s range error\n", item.name.c_str());
		exit(0);
	}

	int leftValue = atoi(sections[0].substr(1).c_str());
	int rightValue = 0;
	std::string tmp = sections[1].substr(0, sections[1].length() - 1);
	rightValue = tmp == "INT_MAX" ? INT_MAX : atoi(tmp.c_str());
	bool leftClosed = sections[0].at(0) == '[';
	bool rightClosed = sections[1].at(sections[1].length() - 1) == ']';

	std::string strName = "m_" + LWPR::ConfigProperty::GetPropertyLabel(item.name.c_str());

	if(leftClosed)
	{
		LWPR::Buffer buf(1024);

		sprintf(buf.Inout(),
		        "\t%s = %s < %d ? %d : %s;\n",
		        strName.c_str(),
		        strName.c_str(),
		        leftValue,
		        leftValue,
		        strName.c_str());

		strGenCode += buf.Inout();
	}
	else
	{
		LWPR::Buffer buf(1024);

		sprintf(buf.Inout(),
		        "\t%s = %s <= %d ? %d : %s;\n",
		        strName.c_str(),
		        strName.c_str(),
		        leftValue,
		        leftValue + 1,
		        strName.c_str());

		strGenCode += buf.Inout();
	}

	if(rightClosed)
	{
		LWPR::Buffer buf(1024);

		sprintf(buf.Inout(),
		        "\t%s = %s > %d ? %d : %s;\n\n",
		        strName.c_str(),
		        strName.c_str(),
		        rightValue,
		        rightValue,
		        strName.c_str());

		strGenCode += buf.Inout();
	}
	else
	{
		LWPR::Buffer buf(1024);

		sprintf(buf.Inout(),
		        "\t%s = %s > %d ? %d : %s;\n\n",
		        strName.c_str(),
		        strName.c_str(),
		        rightValue,
		        rightValue - 1,
		        strName.c_str());

		strGenCode += buf.Inout();
	}

	return strGenCode;
}

void GenHeader()
{
	LWPR::Buffer buf(1024 * 100);

	// 枚举类型声明
	std::string enumSection;
	{
		EnumTable::iterator it = g_EnumTable.begin();
		for(; it != g_EnumTable.end(); it++)
		{
			buf.Reset();
			sprintf(buf.Inout(), "\ttypedef enum %s \n\t{\n", it->first.c_str());
			enumSection += buf.Inout();

			for(size_t i = 0; i < it->second.size(); i++)
			{
				buf.Reset();
				sprintf(buf.Inout(), "\t\t%s,\n", it->second[i].c_str());
				enumSection += buf.Inout();
			}

			buf.Reset();
			sprintf(buf.Inout(), "\t} %s;\n\n", it->first.c_str());
			enumSection += buf.Inout();
		}
	}

	// 变量声明
	std::string varSection;
	{
		for(size_t i = 0; i < g_ConfigItems.size(); i++)
		{
			if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();
				sprintf(buf.Inout(), "\t%s m_%s;\n",
				        g_ConfigItems[i].type.c_str(),
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str());

				varSection += buf.Inout();
			}
		}
	}

	// 函数声明
	std::string funSection;
	{
		for(size_t i = 0; i < g_ConfigItems.size(); i++)
		{
			if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();
				sprintf(buf.Inout(), "\tinline const %s& %s() const\n\t{\n\t\treturn m_%s;\n\t}\n\n",
				        g_ConfigItems[i].type.c_str(),
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str(),
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str());
				funSection += buf.Inout();
			}

			if(g_ConfigItems[i].type == "string")
			{
				buf.Reset();
				sprintf(buf.Inout(), "\tinline void %s(const char* value)\n\t{\n\t\tassert(value != NULL);\n\t\tm_%s = value;\n\t}\n\n",
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str(),
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str());
				funSection += buf.Inout();
			}
			else if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();
				sprintf(buf.Inout(), "\tinline void %s(%s value)\n\t{\n\t\tm_%s = value;\n\t}\n\n",
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str(),
				        g_ConfigItems[i].type.c_str(),
				        LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str()).c_str());
				funSection += buf.Inout();
			}
		}
	}

	// 生成文件
	FILE* fp = fopen("AutoConfig.h", "w");
	fprintf(fp, fmtHeader, enumSection.c_str(), funSection.c_str(), varSection.c_str());
	fclose(fp);
}

void GenCpp()
{
	LWPR::Buffer buf(1024 * 100);

	// 变量定义
	std::string varSection;
	{
		for(size_t i = 0; i < g_ConfigItems.size(); i++)
		{
			if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();

				std::string strName = "m_" + LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str());

				if(g_ConfigItems[i].type == "string")
				{
					sprintf(buf.Inout(), "\t%s = \"%s\";\n", strName.c_str(), g_ConfigItems[i].value.c_str());
				}
				else
				{
					sprintf(buf.Inout(), "\t%s = %s;\n", strName.c_str(), g_ConfigItems[i].value.c_str());
				}

				varSection += buf.Inout();
			}
		}
	}

	// 变量打印
	std::string printSection = "\tconst char* tmp = \"\";\n\ttmp = tmp;\n";
	{
		for(size_t i = 0; i < g_ConfigItems.size(); i++)
		{
			if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();
				std::string enumAssign;

				std::string strName = "m_" + LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str());

				if(!g_ConfigItems[i].isPublic)
				{
					continue;
				}

				if(g_ConfigItems[i].type == "string")
				{
					sprintf(buf.Inout(), "\tlogger->info(LTRACE, \"%s=%%s\", %s.c_str());\n\n",
					        g_ConfigItems[i].name.c_str(),
					        strName.c_str());
				}
				else if(g_ConfigItems[i].type == "int")
				{
					sprintf(buf.Inout(), "\tlogger->info(LTRACE, \"%s=%%d\", %s);\n\n",
					        g_ConfigItems[i].name.c_str(),
					        strName.c_str());
				}
				else if(g_ConfigItems[i].type == "bool")
				{
					sprintf(buf.Inout(), "\tlogger->info(LTRACE, \"%s=%%s\", %s ? \"true\" : \"false\");\n\n",
					        g_ConfigItems[i].name.c_str(),
					        strName.c_str());
				}
				// 枚举类型
				else
				{
					EnumItem item = g_EnumTable[g_ConfigItems[i].type];
					for(size_t j = 0; j < item.size(); j++)
					{
						char tmpBuf[256] = {0};
						sprintf(tmpBuf, "\tif(%s == %s) tmp = \"%s\";\n", strName.c_str(), item[j].c_str(), item[j].c_str());
						enumAssign += tmpBuf;
					}

					sprintf(buf.Inout(), "\tlogger->info(LTRACE, \"%s=%%s\", tmp);\n\n", g_ConfigItems[i].name.c_str());
				}

				printSection += enumAssign;
				printSection += buf.Inout();
			}
		}
	}

	// 变量初始化
	std::string initSection = "\tstd::string tmp;\n\n";
	{
		for(size_t i = 0; i < g_ConfigItems.size(); i++)
		{
			if(g_ConfigItems[i].type != "enum")
			{
				buf.Reset();
				std::string enumAssign;

				std::string strName = "m_" + LWPR::ConfigProperty::GetPropertyLabel(g_ConfigItems[i].name.c_str());

				if(g_ConfigItems[i].type == "string")
				{
					sprintf(buf.Inout(),
					        "\t%s = GetPropertyValue(\"%s\", %s);\n"
							"\t%s = %s == \"NONE\" ? \"\" : %s;\n\n",
					        strName.c_str(),
					        g_ConfigItems[i].name.c_str(),
							strName.c_str(),
							strName.c_str(),
							strName.c_str(),
					        strName.c_str());
				}
				else if(g_ConfigItems[i].type == "int" ||
				        g_ConfigItems[i].type == "bool")
				{
					sprintf(buf.Inout(), "\t%s = GetPropertyValue(\"%s\", %s);\n\n",
					        strName.c_str(),
					        g_ConfigItems[i].name.c_str(),
					        strName.c_str());
				}
				else
				{
					sprintf(buf.Inout(), "\ttmp = GetPropertyValue(\"%s\", \"%s\");\n",
					        g_ConfigItems[i].name.c_str(),
					        g_ConfigItems[i].value.c_str());

					EnumItem item = g_EnumTable[g_ConfigItems[i].type];
					for(size_t j = 0; j < item.size(); j++)
					{
						char tmpBuf[256] = {0};
						sprintf(tmpBuf, "\tif(tmp == \"%s\") %s = %s;\n", item[j].c_str(), strName.c_str(), item[j].c_str());
						enumAssign += tmpBuf;
					}

					enumAssign += "\n";
				}

				initSection += buf.Inout();
				initSection += enumAssign;
			}
		}

		// 生成纠正代码
		for(size_t k = 0; k < g_ConfigItems.size(); k++)
		{
			if(g_ConfigItems[k].type == "int")
			{
				initSection += FixParam(g_ConfigItems[k]);
			}
		}
	}

	// 生成文件
	FILE* fp = fopen("AutoConfig.cpp", "w");
	fprintf(fp, fmtCpp, varSection.c_str(), printSection.c_str(), initSection.c_str());
	fclose(fp);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cerr << "Useage: "
		     << argv[0]
		     << " configfile"
		     << endl;
		return -1;
	}

	ParseFile(argv[1]);

	ParseEnum();

	GenHeader();

	GenCpp();

	return 0;
}
