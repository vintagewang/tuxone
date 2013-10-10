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
#ifndef LWPR_NET_SERVER_H__
#define LWPR_NET_SERVER_H__
#include "LWPRType.h"
#include "Socket.h"
#include "Exception.h"
#include <string>

namespace LWPR
{
	class NetServer;

	const LWPR::INT32 NET_SERVER_RESERVED_FDS = 32;

	/**
	 * NetServer类型
	 */
	typedef enum net_server_type_e
	{
		NET_SERVER_ST,	// 单线方式
		NET_SERVER_MT	// 多线方式
	} NET_SERVER_TYPE_E;

	/**
	 * 供用户程序继承，负责具体的数据处理
	 */
	class NetServerHandler
	{
	public:
		NetServerHandler() {};
		virtual ~NetServerHandler() {};

		inline void SetNetServer(NetServer* server)
		{
			assert(server != NULL);
			m_pNetServer = server;
		}

		inline NetServer* GetNetServer()
		{
			assert(m_pNetServer != NULL);
			return m_pNetServer;
		}

		/**
		 * 收到客户端正常数据
		 */
		virtual SOCKET_RET_TYPE_E DoReceiveNormalData(SOCKET_FD_T fd);

		/**
		 * 关闭超时客户端之前
		 * 注意：EventNetDispatch在加锁状态下执行，所以DoCloseTimeoutSocket执行时间应尽量短
		 *		默认不执行，需要设置bExecuteCloseHandler选项才执行
		 */
		virtual SOCKET_RET_TYPE_E DoCloseExpiredSocket(SOCKET_FD_T fd);

		/**
		 * select系统调用超时情况下执行
		 */
		virtual void DoSelectTimeout();

		/**
		 * 客户端数目有变化情况执行
		 */
		virtual void DoClientAmountUpdate(int amount, SOCKET_FD_T fd, bool increase);

		/**
		 * 线程启动
		 */
		virtual void DoThreadStart();

		/**
		 * 线程结束
		 */
		virtual void DoThreadEnd();

	private:
		NetServer* m_pNetServer;
	};

	/**
	 * NetServer对象的行为控制
	 */
	typedef struct net_server_option
	{
		std::string strAddr;			// 监听地址，默认值为空，格式-IP:PORT
		SOCKET_FD_T nFdListen;			// 监听SOCKET描述符（默认为-1，如果外部创建，则>=0）
		bool bDropListenFD;				// 当连接数满了后，则不去accept 监听描述符，默认为false
		int nThreadPoolMin;				// 线程池中最少线程数
		int nThreadPoolMax;				// 线程池中最多线程数
		int nThreadPoolMaxIdle;			// 线程池中最多空闲线程数
		int nConnectionMax;				// 最大网络连接数，默认值：0，无限制
		int nSelectTimeout;				// SLECT超时时间（默认为0，如果为0，表示永远不超时, 如果select永远阻塞，则不能准确检测socket过期）
		int nSocketExpiredTime;			// SOCKET过期时间，超过此时间后没有数据，则强制关闭SOCKET（如果为0，表示永远不过期）
		NetServerHandler* pHandler;		// 处理具体的网络事件 （用户必须设置）

		/**
		 * 构造函数
		 */
		net_server_option();
	} NET_SERVER_OPTION_T;

	class NetServer
	{
	protected:

		NetServer();

	public:

		virtual ~NetServer();

		static NetServer* CreateNetServer(NET_SERVER_TYPE_E type, const NET_SERVER_OPTION_T& opt);

		virtual void ActivateService() = 0;

		virtual void Shutdown() = 0;

		virtual void Go() = 0;

		virtual void EnableSocket(SOCKET_FD_T fd) = 0;

		virtual void WriteSocketPositive(SOCKET_FD_T fd, char *buf, int length) = 0;

		virtual void WriteSocketPositive(const SOCKETFDList& fds, char *buf, int length) = 0;

		virtual bool IsServiceBusy() = 0;

		virtual std::string GetListenAddr() = 0;

		virtual void DestroySocket(SOCKET_FD_T fd) = 0;

		virtual LWPR::INT32 ClientTotal() = 0;

	private:
	};

	DEFINE_EXCEPTION(LWPR_EVENT_NET_CREATE_SERVER_SOCKET_ERROR);
	DEFINE_EXCEPTION(LWPR_EVENT_NET_CONNECT_SELEF_ERROR);
	DEFINE_EXCEPTION(LWPR_EVENT_NET_GETSOCKNAME_ERROR);
	DEFINE_EXCEPTION(LWPR_EVENT_NET_GETLOCALIP_ERROR);
	DEFINE_EXCEPTION(LWPR_EVENT_NET_CREATE_PIPE_ERROR);
}
#endif // end of LWPR_NET_SERVER_H__
