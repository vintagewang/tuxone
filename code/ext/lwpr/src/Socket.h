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
#ifndef LWPR_SOCKET_H__
#define LWPR_SOCKET_H__
#include "LWPRType.h"
#include "Buffer.h"
#include "OutputStream.h"
#include "InputStream.h"
#include <string>

namespace LWPR
{
	typedef enum socket_ret_type_e
	{
		SOCKET_RET_FAILED = -1,		// Socket出错
		SOCKET_RET_OK = 0,			// Socket状态：正常、可读、可写
		SOCKET_RET_TIMEOUT = 1,		// Socket读写超时
		SOCKET_RET_FREE = 2,		// Socket不受EventDispatch控制
		SOCKET_RET_NOT_WRABLE = 3,	// Socket读队列无数据，或者写队列满了.一般用于select
	} SOCKET_RET_TYPE_E;

	/**
	 * 此结构体用于网络传输（存储为网络字节序）
	 */
	typedef struct socket_addr_t
	{
		LWPR::UINT32 ip;
		LWPR::UINT16 port;

		void Marshal(LWPR::OutputStream& out);
		void Unmarshal(LWPR::InputStream& in);
	} SOCKET_ADDR_T;

	class Socket
	{
	public:
		/**
		 * 解析地址串
		 * 地址串格式1 0
		 * 地址串格式2 192.168.0.1:26000
		 * 地址串格式3 0.0.0.0:26000
		 * 地址串格式4 192.168.0.1:26000~36000
		 * 地址串格式5 0.0.0.0:26000~36000
		 */
		static bool SplitAddr(const char* addr, std::string& host, int& min_port, int& max_port);

		/**
		 * 地址转化函数
		 */
		static bool AddrTransform(const char* addr_src, SOCKET_ADDR_T& addr_dest);
		static void AddrTransform(const SOCKET_ADDR_T& addr_src, std::string& addr_dest);
		static std::string AddrTransform(const SOCKET_ADDR_T& addr_src);
		static bool IsAddrEqual(const char* addr1, const char* addr2);

		/**
		 * 创建Server Socket，并监听端口
		 * @return 返回监听Socket，如果出错，返回-1
		 */
		static SOCKET_FD_T CreateServerTCPSocket(const char* host, int port);

		/**
		 * 创建Server Socket，并监听端口
		 * @return 返回监听Socket，如果出错，返回-1
		 */
		static SOCKET_FD_T CreateServerTCPSocket(const char* addr);

		/**
		 * 创建UDP Socket
		 * @return 返回Socket描述符，如果出错，返回-1
		 */
		static SOCKET_FD_T CreateServerUDPSocket(LWPR::UINT16& port);

		/**
		 * 创建UDP Socket
		 * @return 返回Socket描述符，如果出错，返回-1
		 */
		static SOCKET_FD_T CreateClientUDPSocket();

		/**
		 * 从连接队列获取连接
		 * @return 返回连接Socket，如果出错，返回-1
		 */
		static SOCKET_FD_T AcceptSocket(SOCKET_FD_T fd);

		/**
		 * 连接远端服务器
		 * @return 返回连接Socket，如果出错，返回-1
		 */
		static SOCKET_FD_T ConnectRemoteHost(const char* host, int port);

		/**
		 * 连接远端服务器
		 * @return 返回连接Socket，如果出错，返回-1
		 */
		static SOCKET_FD_T ConnectRemoteHost(const char* addr);

		/**
		 * 关闭socket
		 * 注意：执行完关闭操作后，会令fd=-1
		 */
		static void CloseSocket(SOCKET_FD_T& fd);

		static void CloseSocketWithShutdown(SOCKET_FD_T& fd);

		/**
		 * 测试Socket是否可读
		 *		timeout = 0 不阻塞
		 *		timeout = INT_MAX 一直阻塞
		 *		timeout > 0 阻塞指定时间
		 */
		static SOCKET_RET_TYPE_E IsSocketReadable(SOCKET_FD_T fd, int timeout);

		/**
		 * 测试Socket是否可读，如果Socket不可读，则一直阻塞
		 */
		static SOCKET_RET_TYPE_E IsSocketReadable(SOCKET_FD_T fd);

		/**
		 * 测试Socket是否可写
		 *		timeout = 0 不阻塞
		 *		timeout = INT_MAX 一直阻塞
		 *		timeout > 0 阻塞指定时间
		 */
		static SOCKET_RET_TYPE_E IsSocketWritable(SOCKET_FD_T fd, int timeout);

		/**
		 * 测试Socket是否可写，如果Socket不可写，则一直阻塞
		 */
		static SOCKET_RET_TYPE_E IsSocketWritable(SOCKET_FD_T fd);

		/**
		 * 读Socket
		 *		timeout = 0 不阻塞
		 *		timeout = INT_MAX 一直阻塞
		 *		timeout > 0 阻塞指定时间
		 */
		static SOCKET_RET_TYPE_E ReadSocket(SOCKET_FD_T fd, LWPR::Buffer& buf, int length, int timeout);

		/**
		 * 读Socket，阻塞模式
		 */
		static SOCKET_RET_TYPE_E ReadSocket(SOCKET_FD_T fd, LWPR::Buffer& buf, int length);

		/**
		 * 读Socket
		 *		timeout = 0 不阻塞
		 *		timeout = INT_MAX 一直阻塞
		 *		timeout > 0 阻塞指定时间
		 */
		static SOCKET_RET_TYPE_E ReadSocket(SOCKET_FD_T fd, char* buf, int length, int timeout);

		/**
		 * 读Socket，阻塞模式
		 */
		static SOCKET_RET_TYPE_E ReadSocket(SOCKET_FD_T fd, char* buf, int length);

		/**
		 * 写Socket，如果长度为0，则返回成功
		 *		timeout = 0 不阻塞
		 *		timeout = INT_MAX 一直阻塞
		 *		timeout > 0 阻塞指定时间
		 */
		static SOCKET_RET_TYPE_E WriteSocket(SOCKET_FD_T fd, const char* buf, int length, int timeout);

		/**
		 * 写Socket，阻塞模式
		 */
		static SOCKET_RET_TYPE_E WriteSocket(SOCKET_FD_T fd, const char* buf, int length);

		/**
		 * 向UDP Socket写数据
		 */
		static SOCKET_RET_TYPE_E WriteUDPSocket(SOCKET_FD_T fd, const char* addr, const char* buf, int length);
		static SOCKET_RET_TYPE_E WriteUDPSocket(SOCKET_FD_T fd, int port, const char* buf, int length);

		/**
		 * 清除UDP Socket队列中数据
		 */
		static void ClearUDPSocket(SOCKET_FD_T fd);

		/**
		 * 清除TCP Socket队列中数据
		 */
		static void ClearSocket(SOCKET_FD_T fd);

	public:
		/**
		 * 获取已经监听的socket，IP地址和端口号
		 */
		static bool GetSocketAddress(int fd, std::string& ip, int& port);

		/**
		 * 获取本地主机的IP地址
		 */
		static bool GetLocalHostIP(std::string& ip);

	private:
	};
};

#endif // end of LWPR_SOCKET_H__
