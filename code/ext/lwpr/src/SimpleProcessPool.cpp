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
#include "SimpleProcessPool.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

namespace LWPR
{
	SimpleProcessPool::SimpleProcessPool()
		: m_nProcessNum(0)
	{
	}

	SimpleProcessPool::~SimpleProcessPool()
	{
		std::list <int > ::iterator it = m_ProcessList.begin();

		for(; it != m_ProcessList.end(); it++)
		{
			kill(SIGKILL, *it);
		}
	}

	void SimpleProcessPool::SetProcessNum(int process_num)
	{
		m_nProcessNum = process_num;
	}

	void SimpleProcessPool::Run(int argc, char** argv)
	{
		// 创建进程
		for(int i = 0; i < m_nProcessNum; i++)
		{
			pid_t pid = fork();
			// child
			if(pid == 0)
			{
				exit(DoChildWork(argc, argv));
			}
			// father
			else if(pid > 0)
			{
				m_ProcessList.push_back(pid);
			}

		}

		// 如果有进程退出， 则启动
		while(true)
		{
			pid_t pid = wait(NULL);
			if(pid > 0)
			{
				std::list <int > ::iterator it = m_ProcessList.begin();

				for(; it != m_ProcessList.end(); it++)
				{
					if(*it == pid)
					{
						m_ProcessList.erase(it);
						break;
					}
				}

				pid = fork();
				// child
				if(pid == 0)
				{
					exit(DoChildWork(argc, argv));
				}
				// father
				else if(pid > 0)
				{
					m_ProcessList.push_back(pid);
				}
			}
		}
	}
};
