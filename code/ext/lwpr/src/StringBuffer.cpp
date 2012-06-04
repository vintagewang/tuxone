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
#include "StringBuffer.h"
#include "StringUtil.h"
#include <cstring>

namespace LWPR
{
	StringBuffer::StringBuffer()
		: Buffer()
	{
	}

	StringBuffer::StringBuffer(int capacity /*= BUFFER_DEFAULT_CAPACITY*/)
		: Buffer(capacity)
	{

	}

	StringBuffer::StringBuffer(char* str)
	{
		*this = str;
	}

	StringBuffer::StringBuffer(const char* str)
	{
		*this = str;
	}

	StringBuffer::StringBuffer(const Buffer& buf)
	{
		*this = buf;
	}

	StringBuffer::StringBuffer(const StringBuffer& strbuf)
	{
		*this = strbuf;
	}


	void StringBuffer::operator=(char* str)
	{
		if(str == NULL) return;

		this->Capacity(strlen(str) + 1);
		strcpy(this->Inout(), str);
	}

	void StringBuffer::operator=(const char* str)
	{
		if(str == NULL) return;

		this->Capacity(strlen(str) + 1);
		strcpy(this->Inout(), str);
	}

	void StringBuffer::operator=(const Buffer& buf)
	{
		this->Capacity(buf.Capacity() + 1);
		char *src = buf.Inout();
		char *dst = this->Inout();
		for(int i = 0; i < buf.Capacity() && (*dst++ = *src++); i++);
		*dst = 0;
	}

	void StringBuffer::operator=(const StringBuffer& strbuf)
	{
		this->Capacity(strbuf.Capacity());
		strcpy(this->Inout(), strbuf.Inout());
	}

	char* StringBuffer::str() const
	{
		return this->Inout();
	}

	int StringBuffer::Length() const
	{
		return strlen(this->Inout());
	}

	int StringBuffer::FindFirstOf(char ch, int index /*= 0*/) const
	{
		int nLen = Length();

		for(int i = index; i < nLen; i++)
		{
			if(*(this->Inout() + i) == ch)
			{
				return i;
			}
		}

		return -1;
	}

	char* StringBuffer::Trim()
	{
		return LWPR::StringUtil::TrimAll(this->Inout());
	}

	char* StringBuffer::Append(const char* str)
	{
		if(str != NULL)
		{
			this->Capacity(Length() + strlen(str) + 1);
			strcat(this->Inout(), str);
		}

		return this->Inout();
	}
};
