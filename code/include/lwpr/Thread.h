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
#ifndef THREAD_IMPL_H__
#define THREAD_IMPL_H__
#include "LWPRType.h"
#include "Thread.h"
#include "Exception.h"
#include "Var.h"
#include "Object.h"
#include <pthread.h>

namespace LWPR
{
	typedef pthread_attr_t  THREAD_ATTR_T;

	/**
	 * 默认为分离线程，不可以调用join回收
	 */
	class Thread : virtual public Object
	{
		Thread(const Thread& rhs) {}
		void operator= (const Thread& rhs) {}

	public:
		/**
		 * 构造函数
		 */
		Thread();

		/**
		 * 析构函数
		 */
		virtual ~Thread();

		/**
		 * 设置线程为非分离线程，用户可以调用Join来回收线程
		 * 注意：需要在Start之前调用
		 */
		void EnableJoinable();

		/**
		 * 设置线程为分离线程，用户不可以调用Join来回收线程
		 * 注意：需要在Start之前调用
		 */
		void DisableJoinable();

		/**
		 * 回收线程，会使调用线程阻塞
		 */
		void Join();

		/**
		 * 使该线程开始执行
		 */
		void Start();

		/**
		 * 执行线程的具体工作，供子类重写
		 */
		virtual void Run() = 0;

		/**
		 * 返回线程ID
		 */
		THREAD_ID_T GetId();

		/**
		 * 线程是否已经退出，停止运行
		 */
		bool IsExited();

		/**
		 * 是否可以继续运行
		 */
		bool IsContinue();

		/**
		 * 停止线程运行，令线程退出
		 */
		void StopRunning();

	public:

		/**
		 * 线程Sleep
		 */
		static void Sleep(double seconds);

		/**
		 * 结束当前线程
		 */
		static void ExitThread(void *status);

		/**
		 * 返回线程ID
		 */
		static THREAD_ID_T GetCurrentThreadId();

	private:
		static void* DoThreadWork(void* arg);

	private:
		THREAD_ID_T		m_threadId;			// 线程ID
		THREAD_ATTR_T	m_threadAttr;		// 线程属性对象
		bool			m_bStarted;			// 线程是否已经启动
		bool			m_bAttrDestroyed;	// 线程属性对象是否已经销毁
		volatile bool	m_bContinue;		// 线程是否可以继续运行
	};

	DEFINE_EXCEPTION(LWPR_THREAD_API_ERR);

	DECLAREVAR(Thread);
};

#endif
