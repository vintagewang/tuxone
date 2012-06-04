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
#ifndef LWPR_STRING_UTIL_H__
#define LWPR_STRING_UTIL_H__
#include "LWPRType.h"
#include "Buffer.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>

namespace LWPR
{
	class StringUtil
	{
	public:
		/**
		 * 去除前后空格、制表符、换行符
		 */
		static char* TrimAll(char *str);
		static char* TrimAll(char *buf, int num);

		/**
		 * 去除前后引号, 然后去除前后空格、制表符、换行符
		 */
		static char* TrimQuotationChar(char *str);

		/**
		 * 是否是空白字符（空格、制表符、换行符）
		 */
		static bool IsBlankChar(char ch);

		static std::string& ToUpper(std::string& str);

		static std::string& ToLower(std::string& str);

		/**
		 * 在一个字符缓冲区中查找字符ch首次出现的位置
		 */
		static int Find(const char* buf, int num, char ch);

		static void VForm(Buffer& buf, const char* format, va_list args);

		static void SplitString(const std::string& src, char delimiter, std::vector< std::string >& vs);

		/**
		 * 类型转化
		 */
		static std::string IntToStr(int num);
	};
};

#endif // end of LWPR_STRING_UTIL_H__
