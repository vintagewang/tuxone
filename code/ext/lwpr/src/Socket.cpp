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
#include "Socket.h"
#include "Utility.h"
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netinet/tcp.h> // for TCP_NODELAY

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

namespace LWPR
{
	//----------------------------------------------------------------------------------
	// struct socket_addr_t
	//----------------------------------------------------------------------------------
	void socket_addr_t::Marshal(LWPR::OutputStream& out)
	{
		out.WriteUINT32(ip);
		out.WriteUINT16(port);
	}

	void socket_addr_t::Unmarshal(LWPR::InputStream& in)
	{
		ip = in.ReadUINT32();
		port = in.ReadUINT16();
	}

	//----------------------------------------------------------------------------------
	// class Socket
	//----------------------------------------------------------------------------------
	LWPR::SOCKET_FD_T Socket::CreateServerTCPSocket(const char* host, int port)
	{
		assert(host != NULL);
		assert(port >= 0);

		SOCKET_FD_T fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bool bRetResult = (fd != -1);

		int optvalue = 0;
		bRetResult = bRetResult && (-1 != setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optvalue, (int)sizeof(optvalue)));

		optvalue = 1;
		bRetResult = bRetResult && (-1 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, (int)sizeof(optvalue)));

		optvalue = 1;
		bRetResult = bRetResult && (-1 != setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optvalue, (int)sizeof(optvalue)));

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(host);
		addr.sin_port = htons(port);
		bRetResult = bRetResult && (-1 != bind(fd, (struct sockaddr*)&addr, (int)sizeof(addr)));

		bRetResult = bRetResult && (-1 != listen(fd, 100000));

		if(!bRetResult && -1 != fd)
		{
			CloseSocket(fd);
			fd = -1;
		}

		return fd;
	}

	LWPR::SOCKET_FD_T Socket::CreateServerUDPSocket(LWPR::UINT16& port)
	{
		SOCKET_FD_T fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		bool bRetResult = (fd != -1);

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		bRetResult = bRetResult && (-1 != bind(fd, (struct sockaddr*)&addr, (int)sizeof(addr)));

		if(bRetResult)
		{
			std::string ip;
			int portTmp;
			if(!GetSocketAddress(fd, ip, portTmp))
			{
				CloseSocket(fd);
				fd = -1;
			}
			else
			{
				port = portTmp;
			}
		}
		else
		{
			CloseSocket(fd);
			fd = -1;
		}

		return fd;
	}


	LWPR::SOCKET_FD_T Socket::CreateClientUDPSocket()
	{
		SOCKET_FD_T fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		return fd;
	}

	LWPR::SOCKET_FD_T Socket::CreateServerTCPSocket(const char* addr)
	{
		assert(NULL != addr);
		std::string host;
		int min_port = 0;
		int max_port = 0;

		if(SplitAddr(addr, host, min_port, max_port))
		{
			for(int i = min_port; i <= max_port; i++)
			{
				LWPR::SOCKET_FD_T nFd = CreateServerTCPSocket(host.c_str(), i);
				if(nFd != -1)
				{
					return nFd;
				}
			}
		}

		return -1;
	}

	LWPR::SOCKET_FD_T Socket::AcceptSocket(SOCKET_FD_T fd)
	{
		assert(fd > 0);
		int nRetCode = accept(fd, NULL, NULL);

		return nRetCode;
	}

	LWPR::SOCKET_FD_T Socket::ConnectRemoteHost(const char* host, int port)
	{
		assert(host != NULL);
		assert(port > 0);

		SOCKET_FD_T fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bool bRetResult = (fd != -1);

		int optvalue = 0;
		bRetResult = bRetResult && (-1 != setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optvalue, (int)sizeof(optvalue)));

		optvalue = 1;
		bRetResult = bRetResult && (-1 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, (int)sizeof(optvalue)));

		optvalue = 1;
		bRetResult = bRetResult && (-1 != setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optvalue, (int)sizeof(optvalue)));

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(host);
		addr.sin_port = htons(port);
		bRetResult = bRetResult && (-1 != connect(fd, (struct sockaddr*)&addr, (int)sizeof(addr)));

		if(!bRetResult && -1 != fd)
		{
			CloseSocket(fd);
			fd = -1;
		}

		return fd;
	}

	LWPR::SOCKET_FD_T Socket::ConnectRemoteHost(const char* addr)
	{
		assert(NULL != addr);
		std::string host;
		int min_port = 0;
		int max_port = 0;

		if(SplitAddr(addr, host, min_port, max_port))
		{
			host = (host == "0.0.0.0") ? "127.0.0.1" : host;
			return ConnectRemoteHost(host.c_str(), min_port);
		}

		return -1;
	}

	void Socket::CloseSocket(SOCKET_FD_T& fd)
	{
		if (fd >= 0)
		{
			SOCKET_FD_T oldFd = fd;
			fd = -1;
			//shutdown(oldFd, SHUT_RDWR);
			close(oldFd);
		}
	}

	LWPR::SOCKET_RET_TYPE_E Socket::ReadSocket(SOCKET_FD_T fd, LWPR::Buffer& buf, int length, int timeout)
	{
		buf.Size(length);

		return ReadSocket(fd, buf.Inout(), length, timeout);
	}

	LWPR::SOCKET_RET_TYPE_E Socket::ReadSocket(SOCKET_FD_T fd, LWPR::Buffer& buf, int length)
	{
		buf.Size(length);

		return ReadSocket(fd, buf.Inout(), length, INT_MAX);
	}

	LWPR::SOCKET_RET_TYPE_E Socket::ReadSocket(SOCKET_FD_T fd, char* buf, int length, int timeout)
	{
		int nDoneLen = 0, nRetNum = 0;
		SOCKET_RET_TYPE_E nRetResult = SOCKET_RET_FAILED;

		if(0 == length) return SOCKET_RET_OK;
		if(length < 0) return SOCKET_RET_FAILED;

		do
		{
			nRetResult = IsSocketReadable(fd, timeout);
			if(nRetResult == SOCKET_RET_OK)
			{
				nRetNum = read(fd, buf + nDoneLen, length - nDoneLen);
				if(nRetNum == -1)
				{
					return ((errno == EINTR) ? SOCKET_RET_TIMEOUT : SOCKET_RET_FAILED);
				}
				else if(nRetNum == 0)
				{
					return SOCKET_RET_FAILED;
				}

				nDoneLen += nRetNum;
			}
		}
		while(nRetResult == SOCKET_RET_OK && (nDoneLen < length));

		return nRetResult;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::ReadSocket(SOCKET_FD_T fd, char* buf, int length)
	{
		return ReadSocket(fd, buf, length, INT_MAX);
	}

	LWPR::SOCKET_RET_TYPE_E Socket::WriteSocket(SOCKET_FD_T fd, const char* buf, int length, int timeout)
	{
		int nDoneLen = 0, nRetNum = 0;
		SOCKET_RET_TYPE_E nRetResult = SOCKET_RET_FAILED;

		if(0 == length) return SOCKET_RET_OK;
		if(length < 0) return SOCKET_RET_FAILED;

		do
		{
			nRetResult = IsSocketWritable(fd, timeout);
			if(nRetResult == SOCKET_RET_OK)
			{
				nRetNum = write(fd, buf + nDoneLen, length - nDoneLen);
				if(nRetNum == -1)
				{
					return ((errno == EINTR) ? SOCKET_RET_TIMEOUT : SOCKET_RET_FAILED);
				}
				else if(nRetNum == 0)
				{
					return SOCKET_RET_FAILED;
				}

				nDoneLen += nRetNum;
			}
		}
		while(nRetResult == SOCKET_RET_OK && (nDoneLen < length));

		return nRetResult;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::WriteSocket(SOCKET_FD_T fd, const char* buf, int length)
	{
		return WriteSocket(fd, buf, length, INT_MAX);
	}

	LWPR::SOCKET_RET_TYPE_E Socket::IsSocketReadable(SOCKET_FD_T fd, int timeout)
	{
		assert(fd >= 0);
		assert(timeout >= 0);

		fd_set fds;
		struct timeval tv = {0};

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		struct timeval *ptv = (timeout == INT_MAX ? NULL : &tv);

		int retval = select(fd + 1, &fds, NULL, NULL, ptv);
		// Select出错
		if(retval == -1)
		{
			return SOCKET_RET_FAILED;
		}
		// Socket超时
		else if(retval == 0)
		{
			return timeout == 0 ? SOCKET_RET_NOT_WRABLE : SOCKET_RET_TIMEOUT;
		}

		// Socket可读
		return SOCKET_RET_OK;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::IsSocketReadable(SOCKET_FD_T fd)
	{
		return IsSocketReadable(fd, INT_MAX);
	}

	LWPR::SOCKET_RET_TYPE_E Socket::IsSocketWritable(SOCKET_FD_T fd, int timeout)
	{
		assert(fd >= 0);
		assert(timeout >= 0);

		fd_set fds;
		struct timeval tv = {0};

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		tv.tv_sec = timeout;
		tv.tv_usec = 0;

		struct timeval *ptv = (timeout == INT_MAX ? NULL : &tv);

		int retval = select(fd + 1, NULL, &fds, NULL, ptv);
		// Select出错
		if(retval == -1)
		{
			return SOCKET_RET_FAILED;
		}
		// Socket超时
		else if(retval == 0)
		{
			return timeout == 0 ? SOCKET_RET_NOT_WRABLE : SOCKET_RET_TIMEOUT;
		}

		// Socket可读
		return SOCKET_RET_OK;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::IsSocketWritable(SOCKET_FD_T fd)
	{
		return IsSocketWritable(fd, INT_MAX);
	}

	bool Socket::GetSocketAddress(int fd, std::string& ip, int& port)
	{
		assert(fd >= 0);

		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		addr.sin_port = htons(0);
		socklen_t len = sizeof(addr);

		int ret = getsockname(fd, (struct sockaddr*)&addr, &len);
		if(ret != -1)
		{
			ip = inet_ntoa(addr.sin_addr);
			port = ntohs(addr.sin_port);
		}

		return (ret != -1);
	}

	bool Socket::GetLocalHostIP(std::string& ip)
	{
		struct ifreq reqs[32];
		struct ifconf conf = {0};

		memset(reqs, 0, sizeof(reqs));
		conf.ifc_buf = (char *)reqs;
		conf.ifc_len = sizeof(reqs);

		int nETH = conf.ifc_len / sizeof(struct ifreq);
		int nSocket = socket(AF_INET, SOCK_STREAM, 0);
		bool bResult = nSocket != -1;

		bResult = bResult && (ioctl(nSocket, SIOCGIFCONF, (char*)&conf) != -1);

		if(bResult)
		{
			close(nSocket);
			bResult = false;
			for(int i = 0; i < nETH; i++)
			{
				int nAddr = ((sockaddr_in*) & (reqs[i].ifr_addr))->sin_addr.s_addr;
				if((0X100007F != nAddr) && (0 != nAddr))
				{
					ip = inet_ntoa(((sockaddr_in*) & (reqs[i].ifr_addr))->sin_addr);
					bResult = true;
					break;
				}
			}
		}

		return bResult;
	}

	bool Socket::SplitAddr(const char* addr, std::string& host, int& min_port, int& max_port)
	{
		assert(addr != NULL);

		std::string strAddr = addr;
		bool bResult = false;

		if(strAddr == "0")
		{
			host = "0.0.0.0";
			min_port = 0;
			max_port = 0;
			bResult = true;
		}
		else
		{
			char buf[4][64];

			memset(buf, 0, sizeof(buf));

			// 先分割成HOST与PORT
			int ret = sscanf(addr, "%[^ :]%*[ :]%[^$]", buf[0], buf[1]);
			if(ret == 2)
			{
				host = buf[0];
				bResult = true;

				// 再分割MINPORT与MAXPORT
				ret = sscanf(buf[1], "%[^ ~]%*[ ~]%[^$]", buf[2], buf[3]);
				switch(ret)
				{
				case 1:
					min_port = atoi(buf[2]);
					max_port = min_port;
					bResult = true;
					break;
				case 2:
					min_port = atoi(buf[2]);
					max_port = atoi(buf[3]);
					bResult = true;
					break;
				default:
					break;
				}
			}
		}

		return bResult && (max_port >= min_port) && (min_port >= 0);
	}

	bool Socket::AddrTransform(const char* addr_src, SOCKET_ADDR_T& addr_dest)
	{
		std::string host;
		int min_port, max_port;
		bool bResult = SplitAddr(addr_src, host, min_port, max_port);

		in_addr_t ip;
		uint16_t port;
		bResult = bResult && (INADDR_NONE != (ip = inet_addr(host.c_str())));

		if(bResult)
		{
			port = ntohs(min_port);
			addr_dest.ip = ip;
			addr_dest.port = port;
		}

		return bResult;
	}

	void Socket::AddrTransform(const SOCKET_ADDR_T& addr_src, std::string& addr_dest)
	{
		in_addr inaddr = {0};
		inaddr.s_addr = addr_src.ip;
		std::string ip = inet_ntoa(inaddr);
		LWPR::UINT32 port = htons(addr_src.port);

		char buf[64] = {0};
		sprintf(buf, "%s:%d", ip.c_str(), port);

		addr_dest = buf;
	}

	std::string Socket::AddrTransform(const SOCKET_ADDR_T& addr_src)
	{
		std::string str;
		AddrTransform(addr_src, str);
		return str;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::WriteUDPSocket(SOCKET_FD_T fd, const char* addr, const char* buf, int length)
	{
		assert(addr != NULL);
		assert(buf != NULL);

		int nDoneLen = 0, nRetNum = 0;
		SOCKET_RET_TYPE_E nRetResult = SOCKET_RET_FAILED;

		if(0 == length) return SOCKET_RET_OK;
		if(length < 0) return SOCKET_RET_FAILED;

		std::string host;
		int min_port = 0;
		int max_port = 0;
		if(SplitAddr(addr, host, min_port, max_port))
		{
			struct sockaddr_in addrt = {0};
			addrt.sin_family = AF_INET;
			addrt.sin_addr.s_addr = inet_addr(host.c_str());
			addrt.sin_port = htons(min_port);

			do
			{
				nRetResult = IsSocketWritable(fd, 0);
				if(nRetResult == SOCKET_RET_OK)
				{
					nRetNum = sendto(fd, buf + nDoneLen, length - nDoneLen,
					                 0, (struct sockaddr *)&addrt, sizeof(addrt));
					if(nRetNum == -1)
					{
						return ((errno == EINTR) ? SOCKET_RET_TIMEOUT : SOCKET_RET_FAILED);
					}
					else if(nRetNum == 0)
					{
						return SOCKET_RET_FAILED;
					}

					nDoneLen += nRetNum;
				}
			}
			while(nRetResult == SOCKET_RET_OK && (nDoneLen < length));
		}

		return nRetResult;
	}

	LWPR::SOCKET_RET_TYPE_E Socket::WriteUDPSocket(SOCKET_FD_T fd, int port, const char* buf, int length)
	{
		char addrbuf[64] = {0};

		sprintf(addrbuf, "127.0.0.1:%d", port);
		return WriteUDPSocket(fd, addrbuf, buf, length);
	}

	void Socket::ClearUDPSocket(SOCKET_FD_T fd)
	{
		const int BUF_LEN = 128;
		int nRetNum = 0;
		Buffer buf(BUF_LEN);

		while(IsSocketReadable(fd, 0) == SOCKET_RET_OK && nRetNum >= 0)
		{
			nRetNum = recvfrom(fd, buf.Inout(), BUF_LEN, 0, NULL, NULL);
		}
	}

	void Socket::ClearSocket(SOCKET_FD_T fd)
	{
		const int BUF_LEN = 512;
		int nRetNum = 0;
		Buffer buf(BUF_LEN);

		while(IsSocketReadable(fd, 0) == SOCKET_RET_OK && nRetNum >= 0)
		{
			nRetNum = read(fd, buf.Inout(), BUF_LEN);
		}
	}

	bool Socket::IsAddrEqual(const char* addr1, const char* addr2)
	{
		assert(NULL != addr1);
		assert(NULL != addr2);

		std::string host1, host2;
		int min1, min2;
		int max1, max2;

		bool bRet = SplitAddr(addr1, host1, min1, max1);
		bRet = bRet && SplitAddr(addr2, host2, min2, max2);
		bRet = bRet && (min1 == min2);
		bRet = bRet && (((host1 == "0.0.0.0" || host1 == "127.0.0.1") && (host2 == "0.0.0.0" || host2 == "127.0.0.1")) ? true : host1 == host2);
		return bRet;
	}

	void Socket::CloseSocketWithShutdown( SOCKET_FD_T& fd )
	{
		if (fd >= 0)
		{
			SOCKET_FD_T oldFd = fd;
			fd = -1;
			shutdown(oldFd, SHUT_RDWR);
			close(oldFd);
		}
	}
};
