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
#ifndef TUX_PMON_CMDLINE_H__
#define TUX_PMON_CMDLINE_H__

#include <lwpr.h>

class PmonCmdline : public LWPR::ProgramOption
{
	PmonCmdline();

public:
	static PmonCmdline* GetInstance();

	/**
	 * [-d logdir]
	 */
	std::string GetLogDir();

	/**
	 * [-i statistics interval ]
	 */
	int GetStatisticsInterval();

	/**
	 * [pid . . . ]
	 */
	void GetPIDS(LWPR::PIDList& pids);

private:
	bool CheckCmdline();

};

#endif // end of TUX_VIEWC32_CMDLINE_H__
