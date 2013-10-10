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
#ifndef LWPR_MUTEX_H__
#define LWPR_MUTEX_H__
#include "LWPRType.h"
#include "Exception.h"

namespace LWPR
{

	typedef pthread_mutex_t  PTHREAD_MUTEX_T;

	class Mutex
	{
	public:

		/**
		 * 构造函数
		 */
		Mutex();

		/**
		 * 析构函数
		 */
		~Mutex();

		/**
		 * 加锁
		 */
		void Lock();

		/**
		 * 非阻塞加锁
		 * 如果锁定成功返回true，否则返回false
		 */
		bool TryLock();

		/**
		 * 释放锁
		 */
		void Unlock();

		/**
		 * 获取锁，为条件变量准备
		 */
		PTHREAD_MUTEX_T* GetMutexRef();

	private:

		PTHREAD_MUTEX_T m_mutex;
	};
};

#endif // end of LWPR_MUTEX_H__
