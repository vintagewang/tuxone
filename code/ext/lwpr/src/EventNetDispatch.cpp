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
#include "EventNetDispatch.h"
#include "Synchronized.h"
#include "Utility.h"
#include <assert.h>

namespace LWPR
{
	//----------------------------------------------------------------------------------
	// class EventNetHandler
	//----------------------------------------------------------------------------------
	SOCKET_RET_TYPE_E EventNetHandler::DoReceiveNormalData(SOCKET_FD_T fd)
	{
		assert(fd >= 0);

		return SOCKET_RET_OK;
	}

	SOCKET_RET_TYPE_E EventNetHandler::DoReceiveConnection(SOCKET_FD_T fd)
	{
		assert(fd >= 0);

		return SOCKET_RET_OK;
	}

	LWPR::SOCKET_RET_TYPE_E EventNetHandler::DoCloseExpiredSocket(SOCKET_FD_T fd)
	{
		assert(fd >= 0);

		return SOCKET_RET_OK;
	}

	//----------------------------------------------------------------------------------
	// struct event_net_option
	//----------------------------------------------------------------------------------
	event_net_option::event_net_option()
		: strIP("0.0.0.0"),
		  nMinPort(0),
		  nMaxPort(0),
		  nThreadPoolMin(5),
		  nThreadPoolMax(300),
		  nThreadPoolMaxIdle(100),
		  nHousekeepInterval(60 * 10),
		  nSocketExpiredTime(60 * 20),
		  bAllowDoHousekeep(false),
		  bAllowDoCloseHandler(false),
		  bAllowDoRcvConnHandler(false),
		  pHandler(NULL)
	{
	}

	//----------------------------------------------------------------------------------
	// class EventNetDispatch
	//----------------------------------------------------------------------------------
	EventNetDispatch::EventNetDispatch(EVENT_NET_OPTION_T& opt)
		: m_EventNetOption(opt),
		  m_nIdleThread(0),
		  m_nTotalThread(0)
	{
		if(m_EventNetOption.nMaxPort == 0)
		{
			m_EventNetOption.nMaxPort = m_EventNetOption.nMinPort;
		}

		assert(m_EventNetOption.nMinPort >= 0);
		assert(m_EventNetOption.nMaxPort >= 0);
		assert(m_EventNetOption.nThreadPoolMin > 0);
		assert(m_EventNetOption.nThreadPoolMax > 0);
		assert(m_EventNetOption.nThreadPoolMaxIdle > 0);
		assert(m_EventNetOption.nHousekeepInterval > 0);
		assert(m_EventNetOption.nSocketExpiredTime > 0);
		assert(m_EventNetOption.pHandler != NULL);

		FD_ZERO(&m_setActiveFd);
		m_listReadableSocket.reserve(FD_SETSIZE + 1);

		opt.pHandler->SetEventNetDispatch(this);
	}

	EventNetDispatch::~EventNetDispatch()
	{
		if(m_EventNetOption.pHandler != NULL)
		{
			delete m_EventNetOption.pHandler;
		}
	}

	void EventNetDispatch::Activate()
	{
		// 创建监听端口
		m_fdListen = Socket::CreateServerTCPSocket(m_EventNetOption.nMinPort, m_EventNetOption.nMaxPort, m_EventNetOption.strIP.c_str());
		if(m_fdListen == -1)
		{
			throw LWPR_EVENT_NET_CREATE_SERVER_SOCKET_ERROR(EXCEPTION_TRACE, "CreateServerTCPSocket error");
		}

		// 连接自身，（当Server端socket监听端口建立后，client端的connect动作就会顺利完成三路握手，并返回）
		std::string strSelfIP = (m_EventNetOption.strIP == "0.0.0.0" ? "127.0.0.1" : m_EventNetOption.strIP);
		m_fdConnSelfClient = Socket::ConnectRemoteHost(strSelfIP.c_str(), m_EventNetOption.nMinPort);
		if(m_fdConnSelfClient == -1)
		{
			throw LWPR_EVENT_NET_CONNECT_SELEF_ERROR(EXCEPTION_TRACE, "ConnectRemoteHost error");
		}

		// 连接自身
		m_fdConnSelfServer = Socket::AcceptSocket(m_fdListen);
		if(m_fdConnSelfClient == -1)
		{
			throw LWPR_EVENT_NET_CONNECT_SELEF_ERROR(EXCEPTION_TRACE, "AcceptSocket error");
		}

		// 校验自身Socket
		// 通过m_fdConnSelfClient写入本进程号，并通过m_fdConnSelfServer读出，如果未能成功读出，
		// 或者读出的进程号与写入的不同，则认为m_fdConnSelfClient与m_fdConnSelfServer不配套
		PID_T nPid = Utility::GetPid();
		SOCKET_RET_TYPE_E nRet = Socket::WriteSocket(m_fdConnSelfClient, (const char*)&nPid, sizeof(nPid));
		if(nRet != SOCKET_RET_OK)
		{
			throw LWPR_EVENT_NET_CONNECT_SELEF_ERROR(EXCEPTION_TRACE, "WriteSocket error");
		}

		Buffer buf;
		nRet = Socket::ReadSocket(m_fdConnSelfServer, buf, sizeof(nPid), 0);
		if(nRet != SOCKET_RET_OK)
		{
			throw LWPR_EVENT_NET_CONNECT_SELEF_ERROR(EXCEPTION_TRACE, "ReadSocket error");
		}

		PID_T* pPid = (PID_T*)buf.Inout();
		if(*pPid != nPid)
		{
			throw LWPR_EVENT_NET_CONNECT_SELEF_ERROR(EXCEPTION_TRACE, "m_fdConnSelfServer accept other process request");
		}

		// 取最大socket
		m_fdMax = (m_fdConnSelfClient > m_fdListen) ? m_fdConnSelfClient : m_fdListen;

		// 设置m_fdActive
		FD_SET(m_fdListen, &m_setActiveFd);
		FD_SET(m_fdConnSelfServer, &m_setActiveFd);

		// 创建线程对象，并启动线程
		for(int i = 0; i < m_EventNetOption.nThreadPoolMin; i++)
		{
			EventNetWorkThread* pThread = new EventNetWorkThread(*this);
			pThread->Start();
			m_listThread.push_back(pThread);
		}

		// 启动管理线程
		this->Start();
	}

	void EventNetDispatch::Halt()
	{
		SOCKET_SELF_MSG_T msg;
		msg.nMsgType = SOCKET_SELF_MSG_EXIT_THREAD;
		msg.nSocketFd = 0;

		{
			Synchronized syn(m_MutexClient);
			Socket::WriteSocket(m_fdConnSelfClient, (const char*)&msg, sizeof(msg));
		}

		Socket::CloseSocket(m_fdListen);
		this->StopRunning();
	}

	void EventNetDispatch::Run()
	{
		while(IsContinue())
		{
			// 以后会替换成wait
			Thread::Sleep(3);

			// 清理死去的线程
			ClearDeadThread();

			// 动态调整线程池
			if(IsServiceBusy())
			{
				if(m_nTotalThread < m_EventNetOption.nThreadPoolMax)
				{
					EventNetWorkThread* pThread = new EventNetWorkThread(*this);
					pThread->Start();
					m_listThread.push_back(pThread);
				}
			}
			else
			{
				if(m_nIdleThread > m_EventNetOption.nThreadPoolMaxIdle)
				{
					EventNetWorkThread* pThread = m_listThread.front();
					if(pThread != NULL)
					{
						pThread->StopRunning();
					}
				}
			}
		}

		// 令线程池中线程退出
		std::list< EventNetWorkThread* >::iterator it = m_listThread.begin();
		for(; it != m_listThread.end(); it++)
		{
			(*it)->StopRunning();
		}

		// 清理线程资源
		for(int i = 0; i < 5 && m_listThread.size() > 0; i++)
		{
			ClearDeadThread();
			Thread::Sleep(1);
		}

		Socket::CloseSocket(m_fdConnSelfServer);
		Socket::CloseSocket(m_fdConnSelfClient);
	}

	void EventNetDispatch::MakeSocketDispatching(SOCKET_FD_T fd)
	{
		assert(fd >= 0);

		SOCKET_SELF_MSG_T msg;
		msg.nMsgType = SOCKET_SELF_MSG_ADD_FD;
		msg.nSocketFd = fd;

		Synchronized syn(m_MutexClient);
		LWPR::SOCKET_RET_TYPE_E nRet = Socket::WriteSocket(m_fdConnSelfClient, (const char*)&msg, sizeof(msg));
		if(nRet != LWPR::SOCKET_RET_OK)
		{
			// 信号发生时，会执行到这里
			LWPR::Socket::CloseSocket(fd);
		}
	}

	bool EventNetDispatch::IsServiceBusy()
	{
		// 认为线程池的负载正常
		bool bResult = (m_nIdleThread >= 3);
		return !bResult;
	}

	void EventNetDispatch::ClearDeadThread()
	{
		std::list< EventNetWorkThread* >::iterator it = m_listThread.begin();
		for(; it != m_listThread.end();)
		{
			if(!(*it)->IsContinue() && (*it)->IsExited())
			{
				delete(*it);
				m_listThread.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	//----------------------------------------------------------------------------------
	// class EventNetThreadImpl
	//----------------------------------------------------------------------------------
	EventNetWorkThread::EventNetWorkThread(EventNetDispatch& dispatch)
		: m_EventNetDispatch(dispatch)
	{
	}

	EventNetWorkThread::~EventNetWorkThread()
	{
	}

	void EventNetWorkThread::Run()
	{
		AutomaticCount autoTotalThread(m_EventNetDispatch.m_nTotalThread);

		while(IsContinue())
		{
			try
			{
				// 可读socket
				SOCKET_FD_T nReadySocket = -1;

				{
					// 空闲线程计数
					AutomaticCount autoIdleThread(m_EventNetDispatch.m_nIdleThread);

					// 加锁，获取可读的Socket（进入临界区）
					Synchronized syn(m_EventNetDispatch.m_MutexEvent);

					// 找一个可读socket，开始工作
					if(m_EventNetDispatch.m_listReadableSocket.size() > 0)
					{
						nReadySocket = m_EventNetDispatch.m_listReadableSocket.back();
						m_EventNetDispatch.m_listReadableSocket.pop_back();
					}

					// 如果没有可读socket，则select
					while(-1 == nReadySocket)
					{
						// 判断当前线程是否可以继续执行
						if(!m_EventNetDispatch.IsContinue())
						{
							return;
						}

						fd_set setReadableFd;
						memcpy(&setReadableFd, &m_EventNetDispatch.m_setActiveFd, sizeof(m_EventNetDispatch.m_setActiveFd));
						struct timeval tv = {0};
						tv.tv_sec = m_EventNetDispatch.m_EventNetOption.nHousekeepInterval;
						tv.tv_usec = 0;
						struct timeval *ptv = &tv;

						if(!m_EventNetDispatch.m_EventNetOption.bAllowDoHousekeep)
						{
							ptv = NULL;
						}

						int nCode = select(m_EventNetDispatch.m_fdMax + 1, &setReadableFd, NULL, NULL, ptv);
						// 有socket可读
						if(nCode > 0)
						{
							for(int i = 0; i <= m_EventNetDispatch.m_fdMax; i++)
							{
								if(FD_ISSET(i, &setReadableFd))
								{
									// 更新socket最后活跃时间
									m_EventNetDispatch.m_mapActiveFd[i] = time(NULL);

									// 处理新来连接
									if(i == m_EventNetDispatch.m_fdListen)
									{
										SOCKET_FD_T fd = LWPR::Socket::AcceptSocket(i);
										if(fd != -1)
										{
											// 如果新来的连接超过select支持的最大数，则CloseSocket
											if(fd >= FD_SETSIZE)
											{
												Socket::CloseSocket(fd);
											}
											else
											{
												FD_SET(fd, &m_EventNetDispatch.m_setActiveFd);
												m_EventNetDispatch.m_mapActiveFd[fd] = time(NULL);
												m_EventNetDispatch.m_fdMax = (fd > m_EventNetDispatch.m_fdMax) ? fd : m_EventNetDispatch.m_fdMax;
												if(m_EventNetDispatch.m_EventNetOption.bAllowDoRcvConnHandler)
												{
													try
													{
														m_EventNetDispatch.m_EventNetOption.pHandler->DoReceiveConnection(fd);
													}
													catch(...)
													{
													}
												}
											}
										}
									}
									// 处理自身连接，用来内部控制信息的传递
									else if(i == m_EventNetDispatch.m_fdConnSelfServer)
									{
										LWPR::Buffer buf;
										SOCKET_RET_TYPE_E res = Socket::ReadSocket(i, buf, sizeof(SOCKET_SELF_MSG_T));
										if(res == SOCKET_RET_OK)
										{
											SOCKET_SELF_MSG_T* pMsg = (SOCKET_SELF_MSG_T*)buf.Inout();

											switch(pMsg->nMsgType)
											{
											case SOCKET_SELF_MSG_ADD_FD:
												FD_SET(pMsg->nSocketFd, &m_EventNetDispatch.m_setActiveFd);
												m_EventNetDispatch.m_mapActiveFd[pMsg->nSocketFd] = time(NULL);
												break;
											case SOCKET_SELF_MSG_EXIT_THREAD:
												break;
											default:
												assert(0);
											}
										}
									}
									// 处理第一个可读socket
									else if(-1 == nReadySocket)
									{
										nReadySocket = i;
										FD_CLR(i, &m_EventNetDispatch.m_setActiveFd);
									}
									// 处理其余可读socket
									else
									{
										m_EventNetDispatch.m_listReadableSocket.push_back(i);
										FD_CLR(i, &m_EventNetDispatch.m_setActiveFd);
									}
								}
							}
						}
						// select 超时
						else if(nCode == 0)
						{
							// 检查不活跃的socket
							if(m_EventNetDispatch.m_EventNetOption.bAllowDoHousekeep)
							{
								for(int i = 0; i <= m_EventNetDispatch.m_fdMax; i++)
								{
									if(FD_ISSET(i, &m_EventNetDispatch.m_setActiveFd))
									{
										if(i == m_EventNetDispatch.m_fdListen)
										{
										}
										else if(i == m_EventNetDispatch.m_fdConnSelfServer)
										{
										}
										else
										{
											if((time(NULL) - m_EventNetDispatch.m_mapActiveFd[i]) >= m_EventNetDispatch.m_EventNetOption.nSocketExpiredTime)
											{
												if(m_EventNetDispatch.m_EventNetOption.bAllowDoCloseHandler)
												{
													try
													{
														m_EventNetDispatch.m_EventNetOption.pHandler->DoCloseExpiredSocket(i);
													}
													catch(...)
													{
													}
												}

												Socket::CloseSocket(i);
												FD_CLR(i, &m_EventNetDispatch.m_setActiveFd);
											}
										}
									}
								}
							}
						}
					} // end of while(-1 == nReadableSocket)
				} // end of Lock

				// 准备处理找到的连接
				SOCKET_RET_TYPE_E nRet = SOCKET_RET_OK;
				try
				{
					nRet = m_EventNetDispatch.m_EventNetOption.pHandler->DoReceiveNormalData(nReadySocket);
				}
				catch(...)
				{
					nRet = SOCKET_RET_FAILED;
				}

				switch(nRet)
				{
				case SOCKET_RET_FAILED:
				case SOCKET_RET_TIMEOUT:
					Socket::CloseSocket(nReadySocket);
					break;
				case SOCKET_RET_OK:
					m_EventNetDispatch.MakeSocketDispatching(nReadySocket);
					break;
				case SOCKET_RET_FREE:
					break;
				default:
					assert(0);
				}
			}
			catch(const LWPR::Exception& e)
			{
				fprintf(stderr, "%s\n", e.what());
			}
		}
	}
};
