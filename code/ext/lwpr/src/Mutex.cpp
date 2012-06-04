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
#include "Mutex.h"
#include "Thread.h"

namespace LWPR
{

	Mutex::Mutex()
	{
		int retcode = 0;

		retcode = pthread_mutex_init(&m_mutex, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_init error");
	}

	Mutex::~Mutex()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void Mutex::Lock()
	{
		int retcode = 0;

		retcode = pthread_mutex_lock(&m_mutex);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_lock error");
	}

	bool Mutex::TryLock()
	{
		int retcode = 0;

		retcode = pthread_mutex_trylock(&m_mutex);

		return (retcode == 0);
	}

	void Mutex::Unlock()
	{
		int retcode = 0;

		retcode = pthread_mutex_unlock(&m_mutex);
		//if(retcode != 0)
		//	throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_unlock error");
	}

	PTHREAD_MUTEX_T* Mutex::GetMutexRef()
	{
		return &m_mutex;
	}
};
