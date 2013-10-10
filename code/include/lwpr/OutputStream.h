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
#ifndef LWPR_OUTPUT_STREAM_H__
#define LWPR_OUTPUT_STREAM_H__
#include "LWPRType.h"
#include "Object.h"
#include "Buffer.h"
#include <string>

namespace LWPR
{
	static const int OUTPUT_STREAM_DEFAULT_SIZE = 128;

	/**
	 * 为网络传输服务
	 */
	class OutputStream : public Object
	{
		OutputStream(const OutputStream& rhs);
		OutputStream& operator=(const OutputStream& rhs);

	public:
		/**
		 * 构造函数
		 */
		OutputStream(int capacity = OUTPUT_STREAM_DEFAULT_SIZE);
		/**
		 * 重置写偏移量
		 */
		void Reset();
		/**
		 * 写一个BOOL值到Buffer
		 */
		void WriteBOOL(BOOL value);
		/**
		 * 写一个整数到buffer，并转化为网络字节序
		 */
		void WriteINT8(INT8 value);
		void WriteUINT8(UINT8 value);
		void WriteINT16(INT16 value);
		void WriteUINT16(UINT16 value);
		void WriteINT32(INT32 value);
		void WriteUINT32(UINT32 value);
		void WriteINT64(INT64 value);
		void WriteUINT64(UINT64 value);
		/**
		 * 写一个float型的浮点数到buffer
		 */
		void WriteFloat(FP32 value);
		/**
		 * 写一个double型的浮点数到buffer
		 */
		void WriteDouble(FP64 value);
		/**
		 * 写一个字符串到buffer
		 */
		void WriteString(const char* value);
		void WriteString(const std::string& value);
		/**
		 * 写一个buffer到buffer
		 */
		void WriteAny(const char* buf, int size);
		void WriteArray(const void* buf, int size);
		/**
		 * 获得生成的Buffer
		 */
		Buffer_ptr GetBuffer();

		char *GetStream();
		int GetStreamLength();

	private:
		Buffer_var	m_vBuffer;
		int			m_nOffSet;
	};

	DECLAREVAR(OutputStream);
};

#endif // end of LWPR_OUTPUT_STREAM_H__
