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
#include "Resource.h"
#include "Thread.h"
#include "Synchronized.h"
#include <sys/time.h>
#include <time.h>

namespace LWPR
{
	Resource::Resource()
		: m_nWaitThreads(0), m_nResource(0)
	{
		int retcode = pthread_cond_init(&m_cond, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_init error");
	}

	Resource::~Resource()
	{
		pthread_cond_destroy(&m_cond);
	}

	void Resource::Wait()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		m_nWaitThreads++;

		while(m_nResource == 0)
		{
			retcode = pthread_cond_wait(&m_cond, m_mutex.GetMutexRef());
			if(retcode != 0)
			{
				m_nWaitThreads--;
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_wait error");
			}
		}

		if(retcode == 0 && m_nResource > 0)
		{
			if(m_nWaitThreads == 1)
			{
				m_nResource = 0;
			}
			else if(m_nWaitThreads > 1)
			{
				m_nResource--;
			}
		}

		m_nWaitThreads--;
	}

	void Resource::Wait(int seconds)
	{
		assert(seconds >= 0);

		int retcode = 0;

		Synchronized syn(m_mutex);

		m_nWaitThreads++;

		while(m_nResource == 0)
		{
			struct timespec tv = {0};
			struct timeval tv2 = {0};

			gettimeofday(&tv2, NULL);
			tv.tv_sec = tv2.tv_sec + (time_t) seconds;

			retcode = pthread_cond_timedwait(&m_cond, m_mutex.GetMutexRef(), &tv);
			if(retcode == ETIMEDOUT)
			{
				break;
			}
			else if(retcode != 0)
			{
				m_nWaitThreads--;
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_timedwait error");
			}
		}

		if(retcode == 0 && m_nResource > 0)
		{
			if(m_nWaitThreads == 1)
			{
				m_nResource = 0;
			}
			else if(m_nWaitThreads > 1)
			{
				m_nResource--;
			}
		}

		m_nWaitThreads--;
	}

	void Resource::Notify()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		retcode = pthread_cond_signal(&m_cond);
		if(retcode != 0) throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_signal error");

		m_nResource += 1;
	}

	void Resource::Notifyall()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		if(m_nWaitThreads > 0)
		{
			retcode = pthread_cond_broadcast(&m_cond);
			if(retcode != 0) throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_broadcast error");
			m_nResource += m_nWaitThreads;
		}
	}
};
