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
#include "WaitNotifyThread.h"
#include "GateConfig.h"
#include "HousekeepingThread.h"

WaitNotifyThread::WaitNotifyThread()
	: m_nUDPFd(-1)
{
	DEBUG_FUNCTION();
}

WaitNotifyThread::~WaitNotifyThread()
{
	DEBUG_FUNCTION();
}

WaitNotifyThread* WaitNotifyThread::GetInstance()
{
	static WaitNotifyThread *singleton = new WaitNotifyThread();
	return singleton;
}

void WaitNotifyThread::SetUDPFd(LWPR::SOCKET_FD_T fd)
{
	DEBUG_FUNCTION();
	m_nUDPFd = fd;
}

void WaitNotifyThread::Run()
{
	DEBUG_FUNCTION();
	assert(-1 != m_nUDPFd);

	while(IsContinue())
	{
		try
		{
			fd_set fds;
			struct timeval tv = {0};
			FD_ZERO(&fds);

			FD_SET(m_nUDPFd, &fds);
			tv.tv_sec = GateConfig::GetInstance()->TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL();
			int ret = select(m_nUDPFd + 1, &fds, NULL, NULL, &tv);
			// 收到UDP消息
			if(ret > 0)
			{
				logger->debug(LTRACE, "select OK, receive UDP data");
				// 根据消息的类型具体工作
				HousekeepingThread::GetInstance()->Wakeup(false);
				LWPR::Socket::ClearUDPSocket(m_nUDPFd);
			}
			// 超时
			else if(ret == 0)
			{
				logger->debug(LTRACE, "select timeout");
				HousekeepingThread::GetInstance()->Wakeup(false);
			}
			// 发生错误（可能是信号中断）
			else if(ret == -1)
			{
				logger->error(LTRACE, "select error");
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

