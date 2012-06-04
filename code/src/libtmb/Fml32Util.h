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
#ifndef TX_FML32_UTIL_H__
#define TX_FML32_UTIL_H__

#include <string>
#include <vector>
#include <lwpr.h>
#include <fml32.h>

#define FML32_DATA_TYPE_SHORT			'a'
#define FML32_DATA_TYPE_LONG			'b'
#define FML32_DATA_TYPE_CHAR			'c'
#define FML32_DATA_TYPE_FLOAT			'd'
#define FML32_DATA_TYPE_DOUBLE			'e'
#define FML32_DATA_TYPE_STRING			'f'
#define FML32_DATA_TYPE_CARRAY			"g"

#define FML32_DATA_TYPE_SHORT_STR		"short"
#define FML32_DATA_TYPE_LONG_STR		"long"
#define FML32_DATA_TYPE_CHAR_STR		"char"
#define FML32_DATA_TYPE_FLOAT_STR		"float"
#define FML32_DATA_TYPE_DOUBLE_STR		"double"
#define FML32_DATA_TYPE_STRING_STR		"string"
#define FML32_DATA_TYPE_CARRAY_STR		"carray"

#define FML32_DATA_TYPE_SHORT_CODE		(FML32_BASE_FACTOR * FLD_SHORT)
#define FML32_DATA_TYPE_LONG_CODE		(FML32_BASE_FACTOR * FLD_LONG)
#define FML32_DATA_TYPE_CHAR_CODE		(FML32_BASE_FACTOR * FLD_CHAR)
#define FML32_DATA_TYPE_FLOAT_CODE		(FML32_BASE_FACTOR * FLD_FLOAT)
#define FML32_DATA_TYPE_DOUBLE_CODE		(FML32_BASE_FACTOR * FLD_DOUBLE)
#define FML32_DATA_TYPE_STRING_CODE		(FML32_BASE_FACTOR * FLD_STRING)
#define FML32_DATA_TYPE_CARRAY_CODE		(FML32_BASE_FACTOR * FLD_CARRAY)

#define FML32_DATA_TYPE_MAX_OFFSET		(FML32_BASE_FACTOR - 1)


typedef struct fml32_item_format_t
{
	std::string name;
	std::string number;
	std::string type;
	std::string flag;
	std::string comment;

	LWPR::UINT32 GetFieldID();
} FML32_ITEM_FORMAT_T;

typedef std::vector< FML32_ITEM_FORMAT_T > Fml32ItemVector;

class Fml32Paser
{
public:
	Fml32Paser(const char* filename, const char* dir = NULL);
	~Fml32Paser();

	/**
	 * 解析文件
	 */
	bool Parse();

	/**
	 * 生成头文件
	 */
	bool GenHeader();

	/**
	 * 为调试使用
	 */
	void Print();

	/**
	 * 从环境变量获取fml文件
	 */
	static bool GetFMLFilesFromEnv(std::vector< std::string >& files);

private:
	/**
	 * 头文件名称
	 */
	std::string GetFMLHeaderFileName();

private:
	std::string m_strFmlFileName;
	std::string m_strFmlGenDir;
	LWPR::UINT32 m_nBaseNum;
	Fml32ItemVector m_Fml32ItemVector;
};

#endif // end of TX_FML32_UTIL_H__
