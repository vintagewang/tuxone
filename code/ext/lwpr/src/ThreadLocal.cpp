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
#include "ThreadLocal.h"

namespace LWPR
{
	ThreadLocal::ThreadLocal()
		: m_Key(0)
	{
		int retcode = 0;

		retcode = pthread_key_create(&m_Key, KeyDestructor);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_key_create error");
	}

	ThreadLocal::~ThreadLocal()
	{
		pthread_key_delete(m_Key);
	}

	TLSValue* ThreadLocal::GetValue()
	{
		void* p = pthread_getspecific(m_Key);
		return static_cast < TLSValue* > (p);
	}

	void ThreadLocal::SetValue(TLSValue* value)
	{
		int retcode = 0;

		retcode = pthread_setspecific(m_Key, (void *)value);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_setspecific error");
	}

	void ThreadLocal::KeyDestructor(void* value)
	{
		TLSValue* p = static_cast < TLSValue* > (value);
		delete p;
	}
};
