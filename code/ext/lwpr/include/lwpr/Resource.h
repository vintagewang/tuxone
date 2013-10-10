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
#ifndef LWPR_RESOURCE_H__
#define LWPR_RESOURCE_H__
#include "LWPRType.h"
#include "Exception.h"
#include "Mutex.h"

namespace LWPR
{
	typedef pthread_cond_t PTHREAD_COND_T;

	class Resource
	{
	public:
		Resource();

		~Resource();

		void Wait();

		void Wait(int seconds);

		void Notify();

		void Notifyall();

	private:
		Mutex m_mutex;
		PTHREAD_COND_T m_cond;
		volatile int m_nWaitThreads;
		volatile int m_nResource;
	};
};

#endif // end of LWPR_RESOURCE_H__
