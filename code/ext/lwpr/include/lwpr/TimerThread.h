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
#ifndef LWPR_TIMER_THREAD_H__
#define LWPR_TIMER_THREAD_H__
#include "LWPRType.h"
#include "Thread.h"
#include "Resource.h"
#include "ThreadPool.h"
#include "Mutex.h"
#include <list>
#include <time.h>

namespace LWPR
{
	class TimerThread :  public Thread, public Resource
	{
		typedef struct timer_request
		{
			WorkRequest* pRequest;
			time_t nTimestamp;

			bool operator < (timer_request& right);
		} TIMER_REQUEST_T;

		typedef std::list< TIMER_REQUEST_T > TimerRequestList;

	public:
		TimerThread();
		virtual ~TimerThread();

		void AddTimerRequest(WorkRequest* preq, int seconds);

	protected:

		virtual void DoTimerRequest(WorkRequest* preq);

	private:

		void Run();

	private:
		Mutex				m_Mutex;
		TimerRequestList	m_TimerRequestList;
	};
};

#endif // end of LWPR_TIMER_THREAD_H__
