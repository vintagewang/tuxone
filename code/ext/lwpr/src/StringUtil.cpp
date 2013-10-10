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
#include "StringUtil.h"
#include <cstring>
#include <algorithm>

namespace LWPR
{
	char* StringUtil::TrimAll(char *str)
	{
		int i, j, k;

		if(str == NULL) return str;

		for(i = 0;
		    str[i] != 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n');
		    i++);

		for(j = strlen(str) - 1;
		    j >= 0 && (str[j] == ' ' || str[j] == '\t' || str[j] == '\r' || str[j] == '\n');
		    j--);

		for(k = 0; k <= j - i; k++) str[k] = str[k + i];

		str[k] = 0;

		return str;
	}

	char* StringUtil::TrimAll(char *buf, int num)
	{
		int i, j, k;

		if(buf == NULL) return buf;

		for(i = 0;
		    i < num && (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\r' || buf[i] == '\n');
		    i++);

		for(j = num - 1;
		    j >= 0 && (buf[j] == ' ' || buf[j] == '\t' || buf[j] == '\r' || buf[j] == '\n');
		    j--);

		for(k = 0; k <= j - i; k++) buf[k] = buf[k + i];

		buf[k] = 0;

		return buf;
	}

	char* StringUtil::TrimQuotationChar(char *str)
	{
		if(str == NULL) return NULL;

		int nLen = strlen(str);

		if(nLen > 0)
		{
			if(*str == '\'' || *str == '\"')
			{
				*str = ' ';
			}

			if(*(str + nLen - 1) == '\'' || *(str + nLen - 1) == '\"')
			{
				*(str + nLen - 1) = ' ';
			}
		}

		return TrimAll(str);
	}

	bool StringUtil::IsBlankChar(char ch)
	{
		const char* blank = " \t\r\n";
		int num = strlen(blank);

		for(int i = 0; i < num; i++)
		{
			if(ch == *(blank + i))
			{
				return true;
			}
		}

		return false;
	}

	int StringUtil::Find(const char* buf, int num, char ch)
	{
		if(buf == NULL) return -1;

		for(int i = 0; i < num; i++)
		{
			if(*(buf + i) == ch)
			{
				return i;
			}
		}

		return -1;
	}

	void StringUtil::VForm(Buffer& buf, const char* format, va_list args)
	{
		while(true)
		{
			int size = buf.Capacity() - buf.Size();
			size--;

			int n = vsnprintf(buf.Inout() + buf.Size(), size, format, args);

			// OK
			if((n > -1) && (n < size))
			{
				buf.Size(buf.Size() + n);
				return;
			}

			// Retry
			buf.Capacity(buf.Capacity() + size);
		}
	}

	std::string& StringUtil::ToUpper(std::string& str)
	{
		std::string::iterator it = str.begin();
		for(; it != str.end(); it++)
		{
			*it = toupper(*it);
		}

		return str;
	}

	std::string& StringUtil::ToLower(std::string& str)
	{
		std::string::iterator it = str.begin();
		for(; it != str.end(); it++)
		{
			*it = tolower(*it);
		}

		return str;
	}

	void StringUtil::SplitString(const std::string& src, char delimiter, std::vector< std::string >& vs)
	{
		std::string strFmt = "%[^ D]%*[ D]%[^$]";
		std::replace(strFmt.begin(), strFmt.end(), 'D', delimiter);

		std::string strLine = src;
		LWPR::Buffer bufItem1(src.length() + 1);
		LWPR::Buffer bufItem2(src.length() + 1);

		for(bool bContinue = true; bContinue;)
		{
			bufItem1.Reset();
			bufItem2.Reset();

			int ret = sscanf(strLine.c_str(), strFmt.c_str(), bufItem1.Inout(), bufItem2.Inout());
			switch(ret)
			{
			case 2:
				if(strlen(bufItem1.Inout()) > 0)
				{
					vs.push_back(bufItem1.Inout());
				}

				strLine = bufItem2.Inout();
				break;
			case 1:
				if(strlen(bufItem1.Inout()) > 0)
				{
					vs.push_back(bufItem1.Inout());
				}
			default:
				bContinue = false;
				break;
			}
		}
	}

	std::string StringUtil::IntToStr(int num)
	{
		char buf[32] = {0};
		sprintf(buf, "%d", num);
		return buf;
	}
};
