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
#include "GateManager.h"
#include "GateConfig.h"
#include "BulletinBoard.h"
#include <lwpr.h>

GateManager::GateManager() :
	m_bStoped(false)
{
	std::vector<std::string> addrs;
	LWPR::StringUtil::SplitString(GateConfig::GetInstance()->TUXONE_GATE_OTHER_ADDR().c_str(), ';', addrs);

	BB_HEADER_ENTITY_T header;
	BulletinBoard::GetInstance()->GetHeader(header);
	std::string gateAddr = LWPR::Socket::AddrTransform(header.tServerAddr);

	for(size_t i = 0; i < addrs.size(); i++)
	{
		if (!LWPR::Socket::IsAddrEqual(gateAddr.c_str(), addrs[i].c_str()))
		{
			GateProxy* pGate = new GateProxy(addrs[i].c_str(), i);
			m_Gates.push_back(pGate);
			logger->info(LTRACE, "Crete gate proxy [%s] OK", addrs[i].c_str());
		}
	}
}

GateManager::~GateManager()
{
	for(size_t i = 0; i < m_Gates.size(); i++)
	{
		delete m_Gates[i];
	}
}

GateManager* GateManager::GetInstance()
{
	static GateManager *singleton = new GateManager();
	return singleton;
}

void GateManager::Run()
{
	while(!m_bStoped)
	{
		// 描述符可读集合
		fd_set setfds;
		FD_ZERO(&setfds);

		// 定义最大描述符
		LWPR::SOCKET_FD_T maxfd = 0;

		// 准备描述符
		for(size_t i = 0; i < m_Gates.size(); i++)
		{
			if(m_Gates[i]->Connect())
			{
				FD_SET(m_Gates[i]->m_nFDGate, &setfds);
				maxfd = m_Gates[i]->m_nFDGate > maxfd ? m_Gates[i]->m_nFDGate : maxfd;
			}
		}

		// select超时设置
		struct timeval tv = {0};
		tv.tv_sec = GateConfig::GetInstance()->TUXONE_GATE_SYNC_INTERVAL();
		tv.tv_usec = 0;

		int nCode = select(maxfd + 1, &setfds, NULL, NULL, &tv);
		// 有描述符可读
		if(nCode > 0)
		{
			for(int i = 0; i <= maxfd; i++)
			{
				if(FD_ISSET(i, &setfds))
				{
					for(size_t k = 0; k < m_Gates.size(); k++)
					{
						if(i == m_Gates[k]->m_nFDGate)
						{
							logger->debug(LTRACE, "receive other gate notice");
							m_Gates[k]->SendHeartBeat();
							m_Gates[k]->SyncService();
							break;
						}
					}
				}
			}
		}
		// select超时
		else if(nCode == 0)
		{
			logger->debug(LTRACE, "txsync select timeout");
			for(size_t i = 0; i < m_Gates.size(); i++)
			{
				m_Gates[i]->SendHeartBeat();
				m_Gates[i]->SyncService();
			}
		}
		// select出错
		else if(nCode == -1)
		{
			logger->error(LTRACE, "txsync select error");
		}
	}
}

void GateManager::Stop()
{
	m_bStoped = true;
}
