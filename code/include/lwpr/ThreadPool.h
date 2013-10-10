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
#ifndef LWPR_THREAD_POOL_H__
#define LWPR_THREAD_POOL_H__
#include "LWPRType.h"
#include "Thread.h"
#include "Resource.h"
#include "Mutex.h"
#include "Synchronized.h"
#include <list>

namespace LWPR
{
	/**
	 * ThreadPool选项控制
	 */
	typedef struct thread_pool_option
	{
		int nMinThread;
		int nMaxThread;
		int nMaxIdlThread;

		thread_pool_option();
	} THREAD_POOL_OPTION_T;

	/**
	 * 工作请求类，供用户继承
	 */
	class WorkRequest
	{
	public:
		WorkRequest() {}
		virtual ~WorkRequest() {}
		virtual void DoWork() = 0;
	};

	class ThreadPool : public Resource
	{
		// 工作线程
		class ThreadWorker : public Thread
		{
		public:
			ThreadWorker(ThreadPool& pool);
			~ThreadWorker();
			void Run();
		private:
			ThreadPool& m_ThreadPool;
		};

		// 管理线程
		class ThreadManage : public Thread, public Resource
		{
		public:
			ThreadManage(ThreadPool& pool);
			~ThreadManage();
			void Run();
		private:
			ThreadPool& m_ThreadPool;
		};

		friend class ThreadWorker;
		friend class ThreadManage;

		typedef std::list< WorkRequest* > WorkRequestList;
		typedef std::list< ThreadWorker* > ThreadWorkerList;

	public:
		ThreadPool(THREAD_POOL_OPTION_T& opt);
		virtual ~ThreadPool();

		void AddRequest(WorkRequest* preq);

		void Destroy();

	private:
		inline void ClearDeadThread();

		inline bool IsNeededToCreateThread();

		inline bool IsNeededToDestroyThread();

	private:
		THREAD_POOL_OPTION_T	m_Option;			// 选项控制
		ThreadManage*			m_pThreadManage;	// 管理线程

		Mutex					m_Mutex;			// for m_WorkRequestList
		WorkRequestList			m_WorkRequestList;	// 请求链表

		ThreadWorkerList		m_ThreadWorkerList;	// 线程链表
		AtomicInteger			m_nIdleThread;		// 空闲线程数量
		AtomicInteger			m_nTotalThread;		// 所有线程数量
	};
};

#endif // end of LWPR_THREAD_POOL_H__
