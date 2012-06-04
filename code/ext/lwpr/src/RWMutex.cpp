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
#include "RWMutex.h"
#include "Thread.h"

namespace LWPR
{
	RWMutex::RWMutex()
	{
		int retcode = 0;

		retcode = pthread_rwlock_init(&m_rwlock, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_init error");
	}

	RWMutex::~RWMutex()
	{
		pthread_rwlock_destroy(&m_rwlock);
	}

	void RWMutex::RDLock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_rdlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_rdlock error");
	}

	void RWMutex::WRLock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_wrlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_wrlock error");

	}

	void RWMutex::Unlock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_unlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_unlock error");
	}
};
