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
#include "TimerThread.h"
#include "Synchronized.h"
#include <sys/time.h>

namespace LWPR
{
	//----------------------------------------------------------------------------------
	// class timer_request
	//----------------------------------------------------------------------------------
	bool TimerThread::timer_request::operator<(timer_request& right)
	{
		return this->nTimestamp < right.nTimestamp;
	}

	//----------------------------------------------------------------------------------
	// class TimerThread
	//----------------------------------------------------------------------------------
	TimerThread::TimerThread()
	{
		Start();
	}

	TimerThread::~TimerThread()
	{
	}

	void TimerThread::AddTimerRequest(WorkRequest* preq, int seconds)
	{
		if(preq != NULL)
		{
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);

			TIMER_REQUEST_T timer = {0};
			timer.pRequest = preq;
			timer.nTimestamp = tv.tv_sec + seconds;

			{
				Synchronized syn(m_Mutex);
				m_TimerRequestList.push_back(timer);
			}

			Notify();
		}
	}

	void TimerThread::DoTimerRequest(WorkRequest* preq)
	{
		if(preq != NULL)
		{
			preq->DoWork();
		}
	}

	void TimerThread::Run()
	{
		while(IsContinue())
		{
			TIMER_REQUEST_T tTimerReq = {0};
			bool bHasReq = false;
			bool bReqExpired = false;
			int nDiffTime = 0;

			// 取一个Timer请求
			{
				Synchronized syn(m_Mutex);
				m_TimerRequestList.sort();

				TimerRequestList::iterator it = m_TimerRequestList.begin();
				if(it != m_TimerRequestList.end())
				{
					tTimerReq = *it;
					bHasReq = true;

					struct timeval tv = {0};
					gettimeofday(&tv, NULL);

					if(tv.tv_sec >= tTimerReq.nTimestamp)
					{
						m_TimerRequestList.pop_front();
						bReqExpired = true;
					}
					else
					{
						nDiffTime = tTimerReq.nTimestamp - tv.tv_sec;
					}
				}
			}

			// 对请求执行定时
			if(bHasReq)
			{
				if(bReqExpired)
				{
					DoTimerRequest(tTimerReq.pRequest);
				}
				else
				{
					Wait(nDiffTime);
				}
			}
			else
			{
				Wait();
			}
		}
	}
};
