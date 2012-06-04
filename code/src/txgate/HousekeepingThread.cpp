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
#include "HousekeepingThread.h"
#include "GateConfig.h"
#include "BulletinBoard.h"
#include "AppDepot.h"
#include "Message.h"


HousekeepingThread::HousekeepingThread()
	: m_pNetServer(NULL),
	  m_nLastCheckTime(0),
	  m_nUDPFd(-1),
	  m_bWakeupSelf(false)
{
	DEBUG_FUNCTION();
}

HousekeepingThread::~HousekeepingThread()
{
	DEBUG_FUNCTION();
}

HousekeepingThread* HousekeepingThread::GetInstance()
{
	static HousekeepingThread *singleton = new HousekeepingThread();
	return singleton;
}

void HousekeepingThread::SetNetServer(LWPR::NetServer* pNetServer)
{
	DEBUG_FUNCTION();
	assert(NULL != pNetServer);
	m_pNetServer = pNetServer;
}

void HousekeepingThread::Run()
{
	DEBUG_FUNCTION();
	m_nLastCheckTime = time(NULL);

	while(IsContinue())
	{
		try
		{
			Wait(GateConfig::GetInstance()->TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL());

			// 如果距离上次检查Server时间大于TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL，则执行检查
			if((time(NULL) - m_nLastCheckTime) >= GateConfig::GetInstance()->TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL())
			{
				m_nLastCheckTime = time(NULL);
				logger->debug(LTRACE, "Check app...");

				// 获取本WSH进程所辖的所有App
				AppMap apps;
				AppDepot::GetInstance()->GetAllApp(apps);

				// 执行App有效性校验
				for(AppMap::iterator it = apps.begin(); it != apps.end(); it++)
				{
					if((m_nLastCheckTime - it->second.nLastUpdateTime) > GateConfig::GetInstance()->TUXONE_GATE_WSH_SERVERVALIDTIME())
					{
						// 从应用集合注销
						AppDepot::GetInstance()->UnRegisterApp(it->first);

						// 从公告板注销
						if((it->second.nAppType & TUX_APP_TYPE_SERVER) == TUX_APP_TYPE_SERVER)
						{
							BulletinBoard::GetInstance()->UnregisterService(it->second.tSocketAddr);
						}

						// 断开网络连接
						m_pNetServer->DestroySocket(it->first);
						logger->info(LTRACE, "Close expired socket [%d] [%d] [%s]",
						             it->first,
						             it->second.nAppType,
						             LWPR::Socket::AddrTransform(it->second.tSocketAddr).c_str());
					}
				}
			}

			if(BulletinBoard::GetInstance()->IsSvcUpdated())
			{
				logger->debug(LTRACE, "bulletin board updated");

				// 通知当前WSH进程所辖Client
				BroadcastNotice();

				// 如果是当前WSH进程引起的公告板变更，则通知其他WSH进程
				NotifyOtherWSH();
			}
		}
		catch(LWPR::Exception& e)
		{
			logger->error(LTRACE, "%s", e.what());
		}
		catch(std::exception& e)
		{
			logger->error(LTRACE, "%s", e.what());
		}
		catch(...)
		{
			logger->error(LTRACE, "Unknow exception");
		}
	}
}

void HousekeepingThread::BroadcastNotice()
{
	LWPR::SOCKETFDList fds;

	// 获取本WSH进程所辖的所有App
	AppMap apps;
	AppDepot::GetInstance()->GetAllApp(apps);

	// 收集所有客户端
	for(AppMap::iterator it = apps.begin(); it != apps.end(); it++)
	{
		if((it->second.nAppType & TUX_APP_TYPE_CLIENT) == TUX_APP_TYPE_CLIENT)
		{
			fds.push_back(it->first);
		}
	}

	logger->debug(LTRACE, "broadcast notice to %d clients", fds.size());

	if(!fds.empty())
	{
		// 准备通知报文
		TUX_HEADER_MESSAGE_T header;
		LWPR::OutputStream outHeader;
		header.nMsgType = TUX_MSG_TYPE_GATE_NOTICE;
		header.nMsgLength = TUX_HEADER_MESSAGE_SIZE;
		header.Marshal(outHeader);

		// 下发主动通知
		m_pNetServer->WriteSocketPositive(fds, outHeader.GetStream(), outHeader.GetStreamLength());
	}
}

void HousekeepingThread::SetUDPFd(LWPR::SOCKET_FD_T fd)
{
	DEBUG_FUNCTION();
	m_nUDPFd = fd;
}

void HousekeepingThread::Wakeup(bool self)
{
	{
		LWPR::Synchronized syn(m_Mutex);
		if(!m_bWakeupSelf && self)
		{
			m_bWakeupSelf = self;
		}
	}

	Notify();
}

bool HousekeepingThread::IsWakeupBySelf()
{
	LWPR::Synchronized syn(m_Mutex);
	bool who = m_bWakeupSelf;
	m_bWakeupSelf = false;
	return who;
}

void HousekeepingThread::NotifyOtherWSH()
{
	if(IsWakeupBySelf())
	{
		BulletinBoard::GetInstance()->WakeupOtherWSH(m_nUDPFd);
	}
}
