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
#ifndef LWPR_RWMUTEX_H__
#define LWPR_RWMUTEX_H__
#include "LWPRType.h"
#include "Exception.h"

namespace LWPR
{

	typedef pthread_rwlock_t PTHREAD_RWLOCK_T;

	class RWMutex
	{
	public:

		/**
		 * 构造函数
		 */
		RWMutex();

		/**
		 * 析构函数
		 */
		~RWMutex();

		/**
		 * 读锁 Read Lock
		 */
		void RDLock();

		/**
		 * 写锁 Write Lock
		 */
		void WRLock();

		/**
		 * 释放锁
		 */
		void Unlock();

	private:

		PTHREAD_RWLOCK_T m_rwlock;
	};
};

#endif // end of LWPR_RWMUTEX_H__
