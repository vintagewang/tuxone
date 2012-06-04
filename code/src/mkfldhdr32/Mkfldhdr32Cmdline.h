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
#ifndef TUX_MKFLDHDR32CMDLINE_H__
#define TUX_MKFLDHDR32CMDLINE_H__

#include <lwpr.h>
#include <vector>

class Mkfldhdr32Cmdline : public LWPR::ProgramOption
{
	Mkfldhdr32Cmdline();

public:
	static Mkfldhdr32Cmdline* GetInstance();

	/**
	 * [-d outdir]
	 * Specifies that the output header files are to be created in a directory other than the present working directory.
	 */
	bool GetOutDir(std::string& dir);

	/**
	 * [ field_table... ]
	 */
	void GetFMLFiles(std::vector< std::string >& files);

private:
	bool CheckCmdline();

private:
};

#endif // end of TUX_MKFLDHDR32CMDLINE_H__
