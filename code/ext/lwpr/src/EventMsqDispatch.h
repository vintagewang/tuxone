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
#ifndef LWPR_EVENT_MSQ_DISPATCH_H__
#define LWPR_EVENT_MSQ_DISPATCH_H__
#include "LWPRType.h"
#include "Buffer.h"
#include "Thread.h"
#include "IPCMSQ.h"
#include "Synchronized.h"
#include "LWPRType.h"
#include <vector>

namespace LWPR
{
	/**
	 * 供用户程序继承，负责具体的数据处理
	 */
	class EventMsqHandler
	{
	public:
		/**
		 * 构造函数
		 */
		EventMsqHandler() {};

		/**
		 * 析构函数
		 */
		virtual ~EventMsqHandler() {};

		/**
		 * 收到客户端正常数据
		 */
		virtual void DoReceiveMsqData(Buffer& msg) = 0;
	};

	/**
	 * 选项设置，供用户设置来改变EventMsqDispatch的行为
	 */
	typedef struct event_msq_option
	{
		IPCID_T				nMsqId;			// 消息队列ID，默认值：-1（用户必须设置）
		int					nThreadCount;	// 线程池中线程数量，默认值：10
		EventMsqHandler*	pHandler;		// 处理具体的消息队列事件 （用户必须设置）

		/**
		 * 默认构造函数
		 */
		event_msq_option();
	} EVENT_MSQ_OPTION_T;

	/**
	 * 内部用来中断阻塞线程的消息
	 */
	typedef struct event_msq_magic_msg
	{
		long		nMsgType;
		long		nMsgData;

		/**
		 * 默认构造函数
		 */
		event_msq_magic_msg()
			: nMsgType(10225799), nMsgData(10225799)
		{
		}
	} EVENT_MSQ_MAGIC_MSG_T;

	class EventMsqThreadImpl;

	class EventMsqDispatch
	{
		friend class EventMsqThreadImpl;

	public:

		/**
		 * 构造函数
		 */
		EventMsqDispatch(const EVENT_MSQ_OPTION_T& opt);

		/**
		 * 析构函数
		 */
		~EventMsqDispatch();

		/**
		 * 创建监听端口、启动线程
		 */
		void Start();

		/**
		 * 停止线程、关闭socket
		 */
		void Halt();

		/**
		 * 回收线程、关闭SOCKET
		 */
		void Reap();

	private:
		Mutex m_Mutex;

		EVENT_MSQ_OPTION_T m_EventMsqOption;
		volatile bool m_bContinue;
		std::vector< EventMsqThreadImpl* > m_Threads;
	};

	class EventMsqThreadImpl : public Thread
	{
	public:
		/**
		 * 构造函数
		 */
		EventMsqThreadImpl(EventMsqDispatch& dispatch);
		/**
		 * 析构函数
		 */
		~EventMsqThreadImpl();

		/**
		 * 线程Run函数
		 */
		void Run();

	private:
		EventMsqDispatch& m_EventMsqDispatch;
	};
};

#endif // end of LWPR_EVENT_MSQ_DISPATCH_H__
