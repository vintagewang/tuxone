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
#ifndef TX_VIEW32_UTIL_H__
#define TX_VIEW32_UTIL_H__
#include <string>
#include <vector>
#include <map>
#include <lwpr.h>

#define STRUCT_DATA_TYPE_CHAR			'a'
#define STRUCT_DATA_TYPE_SHORT			'b'
#define STRUCT_DATA_TYPE_INT			'c'
#define STRUCT_DATA_TYPE_LONG			'd'
#define STRUCT_DATA_TYPE_FLOAT			'e'
#define STRUCT_DATA_TYPE_DOUBLE			'f'
#define STRUCT_DATA_TYPE_STRING			'g'

#define STRUCT_DATA_TYPE_CHAR_STR		"char"
#define STRUCT_DATA_TYPE_SHORT_STR		"short"
#define STRUCT_DATA_TYPE_INT_STR		"int"
#define STRUCT_DATA_TYPE_LONG_STR		"long"
#define STRUCT_DATA_TYPE_FLOAT_STR		"float"
#define STRUCT_DATA_TYPE_DOUBLE_STR		"double"
#define STRUCT_DATA_TYPE_STRING_STR		"string"

typedef struct struct_item_format_t
{
	char					type;
	std::string				cname;
	std::string				fbname;
	LWPR::UINT32			count;
	LWPR::UINT32			flag;
	LWPR::UINT32			size;
	std::string				nullvalue;
	LWPR::UINT32			offset;

	struct_item_format_t();
	~struct_item_format_t();
	void marshal(LWPR::OutputStream& out);
	void unmarshal(LWPR::InputStream& in);
	static char GetTypeByString(const char* str);
	static LWPR::UINT32 GetSizeByType(char t, const char* sizevalue);
	std::string GetTypeName();
} STRUCT_ITEM_FORMAT_T;

typedef std::vector< STRUCT_ITEM_FORMAT_T > StructItemVector;

/**
 * 针对网络传输部分
 */
typedef struct struct_item_net_t
{
	char					type;
	LWPR::UINT32			count;
	LWPR::UINT32			flag;
	LWPR::UINT32			size;
	LWPR::UINT32			offset;

	struct_item_net_t();
	~struct_item_net_t();
	void marshal(LWPR::OutputStream& out);
	void unmarshal(LWPR::InputStream& in);
} STRUCT_ITEM_NET_T;

typedef std::vector< STRUCT_ITEM_NET_T > StructNetVector;
typedef std::map< std::string, StructNetVector > StructNetMap;

typedef struct struct_info_t
{
	std::string				structName;
	LWPR::UINT32			structSize;
	StructItemVector		items;

	struct_info_t();
	~struct_info_t();
	void marshal(LWPR::OutputStream& out);
	void unmarshal(LWPR::InputStream& in);
} STRUCT_INFO_T;

typedef std::vector< STRUCT_INFO_T > StructInfoVector;
typedef std::map<std::string, STRUCT_INFO_T > StructInfoMap;

typedef struct file_info_t
{
	std::string				fileName;
	StructInfoVector		structs;

	file_info_t();
	~file_info_t();
	void marshal(LWPR::OutputStream& out);
	void unmarshal(LWPR::InputStream& in);
	int GetItemTotal();
} FILE_INFO_T;

class View32Paser
{
	typedef enum line_type_e
	{
		COMMENT_LABEL,
		STRUCT_BEGIN_LABEL,
		STRUCT_ITEM_LABEL,
		STRUCT_END_LABEL
	} LINE_TYPE_E;

	typedef struct line_content_t
	{
		LINE_TYPE_E type;
		std::string value;
	} LINE_CONTENT_T;

#define VIEW_BIN_TMP_FILE_NAME		"tuxone.view.tmp"

public:
	View32Paser(const char* filename, bool map_fml32, const char* dir = NULL);
	~View32Paser();

	bool Parse();

	/**
	 * 为调试使用
	 */
	void Print();

	/**
	 * 根据二进制view文件，生成结构体信息
	 */
	static bool LoadViewStruct(const char* filename, FILE_INFO_T& info);

private:
	bool ParseTextToStruct();

	bool GenViewHeader();

	bool GenViewBinary();

	bool SaveFileInfo();

	bool GenTestProgram();

	std::string GetViewBinFileName();

	std::string GetViewHeaderFileName();

private:
	std::string m_strViewFileName;
	bool m_bMapFml32;
	std::string m_strViewGenDir;
	FILE_INFO_T m_FileInfo;
	std::vector<LINE_CONTENT_T> m_ValidLine;
};

#endif // end of TX_VIEW32_UTIL_H__
