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
#ifndef LWPR_CFILE_H__
#define LWPR_CFILE_H__
#include "LWPRType.h"
#include "Exception.h"
#include "Buffer.h"
#include "StringBuffer.h"
#include <cstdio>

namespace LWPR
{
	class CFile
	{
	public:

		/**
		 * 构造函数
		 */
		CFile(const char* fname, const char* mode);

		/**
		 * 析构函数
		 */
		virtual ~CFile();

		/**
		 * 获取文件的大小
		 */
		int FSIZE();

		/**
		 * 将文件内容全部加载到内存
		 */
		LWPR::Buffer_ptr LoadFileContent();

		/**
		 * 写数据到文件
		 */
		void FWRITE(const LWPR::Buffer& buf);

		/**
		 * 写文本到文件
		 */
		void FWRITE(const LWPR::StringBuffer& strbuf);

	private:
		FILE *m_fp;
	};

	class AutoCloseFile
	{
	public:
		AutoCloseFile(FILE* fp);
		~AutoCloseFile();
	private:
		FILE* m_fp;
	};

#define AUTO_CLOSE_FILE(fp)	LWPR::AutoCloseFile __AutoCloseFile(fp)

	DEFINE_EXCEPTION(FileNotFoundException);
	DEFINE_EXCEPTION(IOException);
};

#endif // end of LWPR_CFILE_H__
