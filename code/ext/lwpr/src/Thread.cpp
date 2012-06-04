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
#include "Thread.h"
#include <cassert>
#include <cstring>
#include <time.h>

namespace LWPR
{
	Thread::Thread()
		: m_threadId(0),
		  m_bStarted(false),
		  m_bAttrDestroyed(false),
		  m_bContinue(true)
	{
		int retcode = 0;

		retcode = pthread_attr_init(&m_threadAttr);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_attr_init error");
	}

	Thread::~Thread()
	{
		if(!m_bAttrDestroyed)
		{
			pthread_attr_destroy(&m_threadAttr);
		}
	}

	void Thread::Start()
	{
		int retcode = 0;

		if(!m_bStarted)
		{
			retcode = pthread_create(&m_threadId, &m_threadAttr, DoThreadWork, (void *)this);
			if(retcode != 0)
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_create error");

			m_bStarted = true;
		}
	}

	THREAD_ID_T Thread::GetId()
	{
		if(!m_bStarted)
		{
			return 0;
		}

		return this->m_threadId;
	}

	void Thread::Sleep(double seconds)
	{
		int retcode = 0;

		struct timespec tv;
		tv.tv_sec = (time_t) seconds;
		tv.tv_nsec = (long)((seconds - tv.tv_sec) * 1e+9);

		retcode = ::nanosleep(&tv, &tv);
	}

	void* Thread::DoThreadWork(void* arg)
	{
		Thread* thread = static_cast<Thread*>(arg);

		try
		{
			// 线程执行前，令线程的引用计数加一
			// 线程执行完，令引用计数减一
			class PairDo
			{
				Thread* m_p;
			public:
				PairDo(Thread* p) : m_p(p)
				{
					m_p->IncRef();
				}
				~PairDo()
				{
					try
					{
						m_p->StopRunning();
						m_p->DecRef();
					}
					catch(...)
					{
					}
				}
			} _PairDo(thread);

			thread->Run();
		}
		catch(const LWPR::Exception& e)
		{
			// TODO Log
		}
		catch(...)
		{
			// TODO Log
		}

		return NULL;
	}

	void Thread::ExitThread(void *status)
	{
		pthread_exit(status);
	}

	LWPR::THREAD_ID_T Thread::GetCurrentThreadId()
	{
		return pthread_self();
	}

	void Thread::EnableJoinable()
	{
		if(!m_bStarted)
		{
			int retcode = pthread_attr_setdetachstate(&m_threadAttr, PTHREAD_CREATE_JOINABLE);
			if(retcode != 0)
			{
				m_bAttrDestroyed = true;
				pthread_attr_destroy(&m_threadAttr);
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_attr_setdetachstate error");
			}
		}
	}

	void Thread::DisableJoinable()
	{
		if(!m_bStarted)
		{
			int retcode = pthread_attr_setdetachstate(&m_threadAttr, PTHREAD_CREATE_DETACHED);
			if(retcode != 0)
			{
				m_bAttrDestroyed = true;
				pthread_attr_destroy(&m_threadAttr);
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_attr_setdetachstate error");
			}
		}
	}

	void Thread::Join()
	{
		if(m_bStarted)
		{
			int retcode = pthread_join(m_threadId, NULL);
			retcode = retcode; // 阻止某些编译器的警告
		}
	}

	bool Thread::IsExited()
	{
		if(m_bStarted)
		{
			int retcode = pthread_kill(m_threadId, 0);
			return (retcode == ESRCH);
		}

		return false;
	}

	bool Thread::IsContinue()
	{
		return m_bContinue;
	}

	void Thread::StopRunning()
	{
		m_bContinue = false;
	}
};
