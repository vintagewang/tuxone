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
#include "ThreadPool.h"
#include "Synchronized.h"

namespace LWPR
{

	//----------------------------------------------------------------------------------
	// class thread_pool_option
	//----------------------------------------------------------------------------------
	thread_pool_option::thread_pool_option()
		: nMinThread(10),
		  nMaxThread(300),
		  nMaxIdlThread(150)
	{
	}

	//----------------------------------------------------------------------------------
	// class ThreadPool
	//----------------------------------------------------------------------------------
	ThreadPool::ThreadPool(THREAD_POOL_OPTION_T& opt)
		: m_Option(opt),
		  m_pThreadManage(NULL),
		  m_nIdleThread(0),
		  m_nTotalThread(0)
	{
		// 创建工作线程
		for(int i = 0; i < m_Option.nMinThread; i++)
		{
			ThreadWorker* pWorker = new ThreadWorker(*this);
			m_ThreadWorkerList.push_back(pWorker);
			pWorker->Start();
		}

		// 创建管理线程
		m_pThreadManage = new ThreadManage(*this);
		m_pThreadManage->Start();
	}

	ThreadPool::~ThreadPool()
	{
	}

	void ThreadPool::AddRequest(WorkRequest* preq)
	{
		if(preq != NULL)
		{
			Synchronized syn(m_Mutex);
			m_WorkRequestList.push_back(preq);
		}

		Notify();
	}

	void ThreadPool::Destroy()
	{
		m_pThreadManage->StopRunning();
	}

	void ThreadPool::ClearDeadThread()
	{
		ThreadWorkerList::iterator it = m_ThreadWorkerList.begin();
		for(; it != m_ThreadWorkerList.end();)
		{
			if(!(*it)->IsContinue())
			{
				ThreadWorker* p = *it;
				p->DecRef();
				it = m_ThreadWorkerList.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	bool ThreadPool::IsNeededToCreateThread()
	{
		return (m_nIdleThread < 3 && m_nTotalThread < m_Option.nMaxThread);
	}

	bool ThreadPool::IsNeededToDestroyThread()
	{
		return (m_nIdleThread > m_Option.nMaxIdlThread && m_nTotalThread > m_Option.nMaxIdlThread);
	}

	//----------------------------------------------------------------------------------
	// class ThreadWorker
	//----------------------------------------------------------------------------------
	ThreadPool::ThreadWorker::ThreadWorker(ThreadPool& pool)
		: m_ThreadPool(pool)
	{

	}

	ThreadPool::ThreadWorker::~ThreadWorker()
	{

	}

	void ThreadPool::ThreadWorker::Run()
	{
		// 所有线程数量
		AutomaticCount autoTotalThread(m_ThreadPool.m_nTotalThread);

		while(IsContinue())
		{
			WorkRequest* pReq = NULL;

			{
				// 空闲线程计数
				AutomaticCount autoIdleThread(m_ThreadPool.m_nIdleThread);
				m_ThreadPool.Wait();

				// 取请求
				Synchronized syn(m_ThreadPool.m_Mutex);
				ThreadPool::WorkRequestList::iterator it = m_ThreadPool.m_WorkRequestList.begin();
				if(it != m_ThreadPool.m_WorkRequestList.end())
				{
					pReq = *it;
					m_ThreadPool.m_WorkRequestList.pop_front();
				}
			}

			// 执行请求
			if(pReq != NULL)
			{
				pReq->DoWork();
			}

			// 调整线程池
			if(m_ThreadPool.IsNeededToCreateThread() || m_ThreadPool.IsNeededToDestroyThread())
			{
				m_ThreadPool.m_pThreadManage->Notify();
			}
		}
	}

	//----------------------------------------------------------------------------------
	// class ThreadManage
	//----------------------------------------------------------------------------------
	ThreadPool::ThreadManage::ThreadManage(ThreadPool& pool)
		: m_ThreadPool(pool)
	{
	}

	ThreadPool::ThreadManage::~ThreadManage()
	{
	}

	void ThreadPool::ThreadManage::Run()
	{
		while(IsContinue())
		{
			Wait(10);

			// 清理死去的线程
			m_ThreadPool.ClearDeadThread();

			// 创建线程
			if(m_ThreadPool.IsNeededToCreateThread())
			{
				ThreadPool::ThreadWorker* pWorker = new ThreadPool::ThreadWorker(m_ThreadPool);
				m_ThreadPool.m_ThreadWorkerList.push_back(pWorker);
				pWorker->Start();
			}

			// 销毁线程
			if(m_ThreadPool.IsNeededToDestroyThread())
			{
				ThreadWorkerList::iterator it = m_ThreadPool.m_ThreadWorkerList.begin();
				if(it != m_ThreadPool.m_ThreadWorkerList.end())
				{
					(*it)->StopRunning();
				}
			}
		}

		// 令线程池中线程退出
		ThreadWorkerList::iterator it = m_ThreadPool.m_ThreadWorkerList.begin();
		for(; it != m_ThreadPool.m_ThreadWorkerList.end(); it++)
		{
			(*it)->StopRunning();
		}

		// 清理线程资源
		for(int i = 0; i < 5 && m_ThreadPool.m_ThreadWorkerList.size() > 0; i++)
		{
			m_ThreadPool.ClearDeadThread();
			Thread::Sleep(1);
		}
	}
};
