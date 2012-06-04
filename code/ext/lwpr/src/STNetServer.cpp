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
#include "STNetServer.h"

namespace LWPR
{
	STNetServer::STNetServer(const NET_SERVER_OPTION_T& opt)
		: m_bStoped(false),
		  m_fdListen(-1),
		  m_NetServerOption(opt),
		  m_nTotalClient(0)
	{
		assert(m_NetServerOption.nThreadPoolMin > 0);
		assert(m_NetServerOption.nThreadPoolMax > 0);
		assert(m_NetServerOption.nThreadPoolMaxIdle > 0);
		assert(m_NetServerOption.nConnectionMax >= 0);
		assert(m_NetServerOption.nSelectTimeout >= 0);
		assert(m_NetServerOption.nSocketExpiredTime >= 0);
		assert(m_NetServerOption.pHandler != NULL);

		m_NetServerOption.nConnectionMax = (m_NetServerOption.nConnectionMax == 0 || m_NetServerOption.nConnectionMax >= FD_SETSIZE)
		                                   ? (FD_SETSIZE - NET_SERVER_RESERVED_FDS) : m_NetServerOption.nConnectionMax;

		opt.pHandler->SetNetServer(this);
	}

	STNetServer::~STNetServer()
	{
	}

	void STNetServer::ActivateService()
	{
		// 创建监听端口
		if(m_NetServerOption.nFdListen != -1)
		{
			m_fdListen = m_NetServerOption.nFdListen;
		}
		else
		{
			m_fdListen = Socket::CreateServerTCPSocket(m_NetServerOption.strAddr.c_str());
			if(m_fdListen == -1)
			{
				throw LWPR_EVENT_NET_CREATE_SERVER_SOCKET_ERROR(EXCEPTION_TRACE, "CreateServerTCPSocket error");
			}
		}

		// 获取监听地址
		std::string strHost;
		int nPort = 0;
		if(!Socket::GetSocketAddress(m_fdListen, strHost, nPort))
		{
			throw LWPR_EVENT_NET_GETSOCKNAME_ERROR(EXCEPTION_TRACE, "GetSocketAddress error");
		}

		char bufTmp[64] = {0};
		if(strHost == "0.0.0.0")
		{
			std::string strTmp;
			if(!LWPR::Socket::GetLocalHostIP(strTmp))
			{
				throw LWPR_EVENT_NET_GETLOCALIP_ERROR(EXCEPTION_TRACE, "GetLocalHostIP error");
			}

			sprintf(bufTmp, "%s:%d", strTmp.c_str(), nPort);
		}
		else
		{
			sprintf(bufTmp, "%s:%d", strHost.c_str(), nPort);
		}

		m_strListenAddr = bufTmp;
	}

	void STNetServer::Shutdown()
	{
		m_bStoped = true;
	}

	void STNetServer::Go()
	{
		SOCKET_SET_T setActiveFd;		// 当前所有活的SOCKET
		SOCKET_FD_T fdMax = m_fdListen;	// 最大SOCKET

		// 初始化
		FD_ZERO(&setActiveFd);
		FD_SET(m_fdListen, &setActiveFd);

		// 进入服务
		while(!m_bStoped)
		{
			SOCKET_SET_T setReadableFd;
			memcpy(&setReadableFd, &setActiveFd, sizeof(setActiveFd));

			// Socket数量达到最大值，停止接收新连接（一般为多进程情况下，交由其余负载较轻的进程执行）
			if(m_nTotalClient >= m_NetServerOption.nConnectionMax
				&& m_NetServerOption.bDropListenFD)
			{
				FD_CLR(m_fdListen, &setReadableFd);
			}

			struct timeval tv = {0};
			tv.tv_sec = m_NetServerOption.nSelectTimeout;
			tv.tv_usec = 0;
			struct timeval *ptv = &tv;
			ptv = (0 == tv.tv_sec) ? NULL : ptv;

			int nCode = select(fdMax + 1, &setReadableFd, NULL, NULL, ptv);
			// 有socket可读
			if(nCode > 0)
			{
				for(int i = 0; i <= fdMax; i++)
				{
					if(FD_ISSET(i, &setReadableFd))
					{
						// 更新socket最后活跃时间
						m_mapSocketTime[i] = time(NULL);

						// 处理新来连接
						if(i == m_fdListen)
						{
							while(LWPR::Socket::IsSocketReadable(i, 0) == LWPR::SOCKET_RET_OK)
							{
								SOCKET_FD_T fd = LWPR::Socket::AcceptSocket(i);
								if(fd != -1)
								{
									// 如果新来的连接超过设置的最大数，则CloseSocket
									if(m_nTotalClient >= m_NetServerOption.nConnectionMax)
									{
										Socket::CloseSocket(fd);
									}
#ifndef WIN32
									// 在Unix平台，防止fd描述符超出select可处理范围外
									else if(fd >= FD_SETSIZE)
									{
										Socket::CloseSocket(fd);
									}
#endif
									else
									{
										FD_SET(fd, &setActiveFd);
										m_mapSocketTime[fd] = time(NULL);
										fdMax = (fd > fdMax) ? fd : fdMax;
										try
										{
											m_NetServerOption.pHandler->DoClientAmountUpdate(++m_nTotalClient, fd, true);
										}
										catch(...)
										{
										}
									}
								}
								else
								{
									break;
								}
							}
						}
						// 处理可读socket
						else
						{
							FD_CLR(i, &setActiveFd);

							SOCKET_RET_TYPE_E nRet = SOCKET_RET_OK;
							try
							{
								nRet = m_NetServerOption.pHandler->DoReceiveNormalData(i);
							}
							catch(...)
							{
								nRet = SOCKET_RET_FAILED;
							}

							switch(nRet)
							{
							case SOCKET_RET_FAILED:
							case SOCKET_RET_TIMEOUT:
								DestroySocket(i);
								break;
							case SOCKET_RET_OK:
								FD_SET(i, &setActiveFd);
								break;
							case SOCKET_RET_FREE:
								break;
							default:
								assert(0);
							}
						}
					}
				} // end of for
			}
			// select 超时
			else if(nCode == 0)
			{
				// 检查不活跃的socket
				for(int i = 0; i <= fdMax; i++)
				{
					if(FD_ISSET(i, &setActiveFd))
					{
						if(i != m_fdListen)
						{
							if((time(NULL) - m_mapSocketTime[i]) >= m_NetServerOption.nSocketExpiredTime)
							{
								try
								{
									m_NetServerOption.pHandler->DoCloseExpiredSocket(i);
								}
								catch(...)
								{
								}

								DestroySocket(i);
								FD_CLR(i, &setActiveFd);
							}
						}
					}
				}

				// 调用用户的业务函数
				try
				{
					m_NetServerOption.pHandler->DoSelectTimeout();
				}
				catch(...)
				{
				}
			}
		}

		// 退出服务
		for(int i = 0; i <= fdMax; i++)
		{
			if(FD_ISSET(i, &setActiveFd))
			{
				DestroySocket(i);
			}
		}
	}

	void STNetServer::EnableSocket(SOCKET_FD_T fd)
	{
		assert(fd >= 0);
	}

	bool STNetServer::IsServiceBusy()
	{
		return true;
	}

	std::string STNetServer::GetListenAddr()
	{
		return m_strListenAddr;
	}

	void STNetServer::DestroySocket(SOCKET_FD_T fd)
	{
		Socket::CloseSocket(fd);
		m_NetServerOption.pHandler->DoClientAmountUpdate(--m_nTotalClient, fd, false);
	}

	void STNetServer::WriteSocketPositive(SOCKET_FD_T fd, char *buf, int length)
	{
	}

	void STNetServer::WriteSocketPositive(const SOCKETFDList& fds, char *buf, int length)
	{

	}

	LWPR::INT32 STNetServer::ClientTotal()
	{
		return m_nTotalClient;
	}
}
