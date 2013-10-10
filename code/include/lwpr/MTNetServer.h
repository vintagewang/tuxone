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
#ifndef LWPR_MTNET_SERVER_H__
#define LWPR_MTNET_SERVER_H__

#include "LWPRType.h"
#include "Exception.h"
#include "Socket.h"
#include "Thread.h"
#include "Mutex.h"
#include "Resource.h"
#include "NetServer.h"
#include "AtomicInteger.h"
#include <time.h>
#include <map>
#include <vector>
#include <list>

namespace LWPR
{
	/**
	 * MTNetServer模块内部传输传递的控制消息类型
	 */
	typedef enum socket_self_msg_type
	{
		SOCKET_SELF_MSG_ADD_FD,
		SOCKET_SELF_MSG_REMOVE_FD,
		SOCKET_SELF_MSG_EXIT_THREAD,
		SOCKET_SELF_MSG_WRITE_DATA_TO_CLIENT,
	} SOCKET_SELF_MSG_TYPE_E;

	/**
	 * MTNetServer模块内部传输传递的控制消息结构
	 */
	typedef struct socket_self_msg
	{
		SOCKET_SELF_MSG_TYPE_E nMsgType;
		SOCKET_FD_T nSocketFd;
		LWPR::INT32 nLength;
	} SOCKET_SELF_MSG_T;

	/**
	 * TODO: 下一步考虑将主动向Socket写数据通过同步锁的形式告知MTNetServer对象，而非通过管道告知。
	 */
	class MTNetServer : public NetServer,
		public Thread,
		public Resource
	{
		class WorkThread : public Thread
		{
		public:
			WorkThread(MTNetServer& server);
			virtual void Run();

		private:
			MTNetServer& m_MTNetServer;
		};

		typedef std::vector< SOCKET_FD_T > SocketVector;
		typedef std::map< SOCKET_FD_T, LWPR::Buffer_ptr> SocketList;
		typedef std::map< SOCKET_FD_T, time_t > SocketMap;
		typedef std::list< WorkThread* > WorkThreadList;

		friend class WorkThread;

	public:

		MTNetServer(const NET_SERVER_OPTION_T& opt);
		~MTNetServer();

		virtual void ActivateService();

		virtual void Shutdown();

		virtual void Go();

		virtual void EnableSocket(SOCKET_FD_T fd);

		virtual void WriteSocketPositive(SOCKET_FD_T fd, char *buf, int length);

		virtual void WriteSocketPositive(const SOCKETFDList& fds, char *buf, int length);

		virtual bool IsServiceBusy();

		virtual std::string GetListenAddr();

		virtual void DestroySocket(SOCKET_FD_T fd);

		virtual LWPR::INT32 ClientTotal();

	private:
		virtual void Run();

		inline void ClearDeadThread();

		inline bool IsIdleThreadTooLittle();

		inline bool IsIdleThreadTooMuch();

		inline bool IsNeededToCreateThread();

		inline bool IsNeededToDestroyThread();

		inline void DestroyOneThread();

		void DestroySocketInternal(SOCKET_FD_T fd);

	private:
		SOCKET_FD_T			m_fdListen;			// 监听套接口
		SOCKET_FD_T			m_fdSelfServer;		// 连接自身的SOCKET，SERVER角色
		SOCKET_FD_T			m_fdSelfClient;		// 连接自身的SOCKET，CLIENT角色
		SOCKET_FD_T			m_fdMax;			// 最大SOCKET
		SOCKET_SET_T		m_setActiveFd;		// 当前所有活的SOCKET

		SocketVector		m_ReadableSockets;	// 当前可读的SOCKET，即这些SOCKET有可读数据
		SocketList			m_WriteableSockets;	// 当前可写的SOCKET，即需要向这些SOCKET
		SocketMap			m_mapSocketTime;	// 当前所有活的SOCKET的最后活跃时间
		Mutex				m_MutexEvent;		// for WorkerThread
		Mutex				m_MutexClient;		// for m_fdConnSelfClient
		Mutex				m_MutexWrite;		// for m_WriteableSockets
		NET_SERVER_OPTION_T	m_NetServerOption;	// 选项配置
		std::string			m_strListenAddr;	// 监听地址
		WorkThreadList		m_WorkThreadList;	// 工作线程列表
		AtomicInteger		m_nIdleThread;		// 空闲线程数量
		AtomicInteger		m_nTotalThread;		// 所有线程数量
		AtomicInteger		m_nTotalClient;		// 客户端总数
	};

}
#endif // end of LWPR_MTNET_SERVER_H__
