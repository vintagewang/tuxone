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
#ifndef LWPR_EVENT_NET_DISPATCH_H__
#define LWPR_EVENT_NET_DISPATCH_H__
#include "LWPRType.h"
#include "Exception.h"
#include "Socket.h"
#include "Thread.h"
#include "Mutex.h"
#include <time.h>
#include <map>
#include <vector>
#include <list>

// for select
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace LWPR
{
	class EventNetDispatch;

	/**
	 * 供用户程序继承，负责具体的数据处理
	 */
	class EventNetHandler
	{
	public:
		/**
		 * 构造函数
		 */
		EventNetHandler() {};

		/**
		 * 析构函数
		 */
		virtual ~EventNetHandler() {};

		inline void SetEventNetDispatch(EventNetDispatch* dispatch)
		{
			assert(dispatch != NULL);
			m_pEventNetDispatch = dispatch;
		}

		inline EventNetDispatch* GetEventNetDispatch()
		{
			assert(m_pEventNetDispatch != NULL);
			return m_pEventNetDispatch;
		}

		/**
		 * 收到客户端正常数据
		 */
		virtual SOCKET_RET_TYPE_E DoReceiveNormalData(SOCKET_FD_T fd);

		/**
		 * 收到客户端新的连接
		 * 注意：EventNetDispatch在加锁状态下执行，所以DoReceiveConnection执行时间应尽量短。
		 *		默认不执行，需要设置bExecuteRcvConnHandler选项才执行
		 */
		virtual SOCKET_RET_TYPE_E DoReceiveConnection(SOCKET_FD_T fd);

		/**
		 * 关闭超时客户端之前
		 * 注意：EventNetDispatch在加锁状态下执行，所以DoCloseTimeoutSocket执行时间应尽量短
		 *		默认不执行，需要设置bExecuteCloseHandler选项才执行
		 */
		virtual SOCKET_RET_TYPE_E DoCloseExpiredSocket(SOCKET_FD_T fd);

	protected:
		EventNetDispatch* m_pEventNetDispatch;

	};

	/**
	 * 选项设置，供用户设置来改变EventNetDispatch的行为
	 */
	typedef struct event_net_option
	{
		std::string strIP;				// 监听IP，默认值："0.0.0.0"（建议设置）
		int nMinPort;					// 监听PORT，默认值：0（建议设置）
		int nMaxPort;					// 监听PORT，默认值：0
		int nThreadPoolMin;				// 线程池中最少线程数
		int nThreadPoolMax;				// 线程池中最多线程数
		int nThreadPoolMaxIdle;			// 线程池中最多空闲线程数
		int nHousekeepInterval;			// 检测SOCKET是否过期的间隔时间
		int nSocketExpiredTime;			// Socket过期时间，超过此时间后没有数据，则强制关闭SOCKET
		bool bAllowDoHousekeep;			// 是否允许执行-检测SOCKET是否过期
		bool bAllowDoCloseHandler;		// 是否允许执行-DoCloseExpiredSocket
		bool bAllowDoRcvConnHandler;	// 是否允许执行-DoReceiveConnection
		EventNetHandler* pHandler;		// 处理具体的网络事件 （用户必须设置）

		/**
		 * 默认构造函数
		 */
		event_net_option();
	} EVENT_NET_OPTION_T;

	/**
	 * EventNetDispatch模块内部传输传递的控制消息类型
	 */
	typedef enum socket_self_msg_type
	{
		SOCKET_SELF_MSG_ADD_FD,
		SOCKET_SELF_MSG_EXIT_THREAD,
	} SOCKET_SELF_MSG_TYPE_E;

	/**
	 * EventNetDispatch模块内部传输传递的控制消息结构
	 */
	typedef struct socket_self_msg
	{
		SOCKET_SELF_MSG_TYPE_E nMsgType;
		SOCKET_FD_T nSocketFd;
	} SOCKET_SELF_MSG_T;

	class EventNetWorkThread;

	// 当服务器处于空闲状态时：
	// 检查每个SOCKET的活跃程度，当SOCKET长时间没有数据，则删除它。
	class EventNetDispatch : public Thread
	{
		friend class EventNetWorkThread;

		typedef std::vector<SOCKET_FD_T> SocketList;

	public:
		/**
		 * 构造函数
		 */
		EventNetDispatch(EVENT_NET_OPTION_T& opt);

		/**
		 * 析构函数
		 */
		~EventNetDispatch();

		/**
		 * 创建监听端口、启动线程
		 */
		void Activate();

		/**
		 * 停止线程、关闭socket
		 */
		void Halt();

		/**
		 * 令SOCKET是可分配的，即将SOCKET加入到SELECT集合中
		 */
		void MakeSocketDispatching(SOCKET_FD_T fd);

		/**
		 * 线程池是否处于忙状态
		 */
		bool IsServiceBusy();

	private:
		/**
		 * 管理线程运行函数
		 */
		virtual void Run();

		/**
		 * 清理死去的线程
		 */
		void ClearDeadThread();

	private:

		SOCKET_FD_T m_fdListen;			// 监听套接口
		SOCKET_FD_T m_fdConnSelfServer;	// 连接自身的SOCKET，Server角色
		SOCKET_FD_T m_fdMax;			// 最大socket
		fd_set m_setActiveFd;			// 当前所有活的socket
		SocketList m_listReadableSocket;// 当前可读的socket，即这些socket有可读数据
		std::map< SOCKET_FD_T, time_t > m_mapActiveFd; // 当前所有活的socket的最后活跃时间
		Mutex m_MutexEvent;

		SOCKET_FD_T m_fdConnSelfClient;	// 连接自身的SOCKET，Client角色
		Mutex m_MutexClient;

		EVENT_NET_OPTION_T m_EventNetOption;
		std::list< EventNetWorkThread* > m_listThread;
		AtomicInteger m_nIdleThread;
		AtomicInteger m_nTotalThread;
	};

	// 具体的线程处理类
	class EventNetWorkThread : public Thread
	{
	public:
		/**
		 * 构造函数
		 */
		EventNetWorkThread(EventNetDispatch& dispatch);
		/**
		 * 析构函数
		 */
		~EventNetWorkThread();

		/**
		 * 线程Run函数
		 */
		void Run();

	private:
		EventNetDispatch& m_EventNetDispatch;
	};

	DEFINE_EXCEPTION(LWPR_EVENT_NET_CREATE_SERVER_SOCKET_ERROR);
	DEFINE_EXCEPTION(LWPR_EVENT_NET_CONNECT_SELEF_ERROR);
};

#endif // end of LWPR_EVENT_NET_DISPATCH_H__
