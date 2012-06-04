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
#ifndef TUX_VIEWC32_CMDLINE_H__
#define TUX_VIEWC32_CMDLINE_H__

#include <lwpr.h>
#include <vector>

class Viewc32Cmdline : public LWPR::ProgramOption
{
	Viewc32Cmdline();

public:
	static Viewc32Cmdline* GetInstance();

	/**
	 * [-n]
	 * Used when compiling a view description file for a C structure that does not map to an FML buffer.
	 * It informs the view compiler not to look for FML information.
	 */
	bool IsNotMapFMLBuffer();

	/**
	 * [-d viewdir]
	 * Used to specify that the view object file is to be created in a directory other than the current directory.
	 */
	bool GetViewObjectFileDir(std::string& viewdir);

	/**
	 * [viewfile . . . ]
	 */
	void GetViewFiles(std::vector< std::string >& files);

private:
	bool CheckCmdline();

private:
};

#endif // end of TUX_VIEWC32_CMDLINE_H__
