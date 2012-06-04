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
#ifndef TUX_BUILDCLIENT_CMDLINE_H__
#define TUX_BUILDCLIENT_CMDLINE_H__

#include <lwpr.h>

class BuildclientCmdline : public LWPR::ProgramOption
{
	BuildclientCmdline();

public:
	static BuildclientCmdline* GetInstance();

	/**
	 * [-C]
	 * 是否是编译COBOL程序
	 */
	bool IsCOBOLCompilation();

	/**
	 * [-v]
	 * 是否编译过程打印更多的信息
	 */
	bool IsVerboseMode();

	/**
	 * [-w]
	 * 是否连接工作站库libwsc.so（默认连接本地库libtux.so）
	 */
	bool IsLinkWSC();

	/**
	 * [-r rmname]
	 * 获取rmname
	 */
	bool GetRMNAME(std::string& rmname);

	/**
	 * [-o outfile]
	 * 获取目标文件名（默认值：a.out）
	 */
	std::string GetOutfile();

	/**
	 * [-f firstfiles]
	 * 获取优先于TUXEDO系统库之前连接的文件名列表
	 */
	bool GetFirstFiles(std::string& firstfiles);

	/**
	 * [-l lastfiles]
	 * 获取要在TUXEDO系统库之后连接的文件名
	 */
	bool GetLastFiles(std::string& lastfiles);

private:
	bool CheckCmdline();

private:
};

#endif // end of TUX_BUILDCLIENT_CMDLINE_H__
