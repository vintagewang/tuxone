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
#ifndef LWPR_SEMPH_H__
#define LWPR_SEMPH_H__
#include "LWPRType.h"
#include "Exception.h"
#include <semaphore.h>

namespace LWPR
{
	typedef sem_t SEM_T;

	class Semph
	{
	public:

		/**
		 * 构造函数
		 */
		Semph(int value = 1);

		/**
		 * 析构函数
		 */
		~Semph();

		/**
		 * 加锁
		 */
		void Lock();

		/**
		 * 释放锁
		 */
		void Unlock();

	private:
		SEM_T m_sem;
	};

	DEFINE_EXCEPTION(LWPR_THREAD_SEM_INIT_ERR);
	DEFINE_EXCEPTION(LWPR_THREAD_SEM_POST_ERR);
	DEFINE_EXCEPTION(LWPR_THREAD_SEM_WAIT_ERR);
};

#endif // end of LWPR_SEMPH_H__
