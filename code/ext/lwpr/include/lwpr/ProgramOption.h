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
#ifndef LWPR_PROGRAM_OPTION_H__
#define LWPR_PROGRAM_OPTION_H__
#include "LWPRType.h"
#include <string>
#include <map>
#include <vector>

namespace LWPR
{
	class ProgramOption
	{
	public:
		/**
		 * 构造函数
		 */
		ProgramOption();

		/**
		 * 析构函数
		 */
		virtual ~ProgramOption();

		/**
		 * 解析命令行选项
		 */
		bool ParseCmdlineOption(int argc, char **argv, const char *optstring);

		/**
		 * 选项是否有效
		 */
		bool IsOptionValid(char opt);

		/**
		 * 获取选项后对应的参数列表
		 */
		bool GetArgumentsByOption(char opt, std::vector< std::string >& args);

		/**
		 * 获取程序名，即第一个参数
		 */
		const std::string& GetFirstArgument() const;

	protected:
		/**
		 * 在命令行选项解析完之后，校验命令行是否满足应用需求
		 */
		virtual bool CheckCmdline();

	protected:
		// 被正确识别的选项
		std::map< char, std::vector<std::string> > m_Options;
		// 除选项外，其他有用的参数
		std::vector<std::string> m_OtherArguments;
		// 被识别的无效选项
		std::vector<char> m_InvalidOptions;
		// 程序第一个参数，即程序名
		std::string m_strFirstArgument;
	};
};

#endif // end of LWPR_PROGRAM_OPTION_H__
