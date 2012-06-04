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
#include "Semph.h"
#include <assert.h>

namespace LWPR
{

	Semph::Semph(int value /*= 1*/)
	{
		assert(value >= 1);

		int retcode = 0;

		retcode = sem_init(&m_sem, 0, value);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_INIT_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_init error");
	}

	Semph::~Semph()
	{
		sem_destroy(&m_sem);
	}

	void Semph::Lock()
	{
		int retcode = 0;

		retcode = sem_wait(&m_sem);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_WAIT_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_wait error");
	}

	void Semph::Unlock()
	{
		int retcode = 0;

		retcode = sem_post(&m_sem);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_POST_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_post error");
	}
};
