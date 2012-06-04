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
#ifndef LWPR_FILE_UTIL_H__
#define LWPR_FILE_UTIL_H__
#include <vector>
#include <string>

namespace LWPR
{
	class FileUtil
	{
	public:
		static bool IsFileExist(const char* fileName);

		/**
		 * 递归建立目录
		 * file 入参 含全路径、文件名
		 *      例:  /post_app/hd_app/center2/file/20071229/11/rptd/100711030306220071229.rpt
		 *
		 * 返回file
		 */
		static char *BuildFile(const char *file);

		/*
		 * 递归建立目录
		 * path 入参 含全路径
		 *      例:  /post_app/hd_app/center2/file/20071229/11/rptd
		 *           /post_app/hd_app/center2/file/20071229/11/rptd/
		 *
		 * 返回@path
		 */
		static char *BuildPath(const char *path);

		/**
		 * 去除文件路径，仅含文件名
		 */
		static std::string GetBaseName(const char* file);

		/**
		 * 更改文件扩展名
		 */
		static std::string ChangeFileExtName(const char* file, const char* ext);

		/**
		 * 根据文件名，生成可重入宏定义
		 */
		static std::string GenHeaderDef(const char* file);
	};
};

#endif // end of LWPR_FILE_UTIL_H__
