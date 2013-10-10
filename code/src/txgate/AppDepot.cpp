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
#include "AppDepot.h"

AppDepot::AppDepot()
{
}

AppDepot::~AppDepot()
{
}

AppDepot* AppDepot::GetInstance()
{
	static AppDepot *singleton = new AppDepot();
	return singleton;
}

void AppDepot::RegisterApp(LWPR::SOCKET_FD_T fd, const APP_ITEM_T& item)
{
	LWPR::Synchronized syn(m_Mutex);
	m_AppMap[fd] = item;
}

void AppDepot::UnRegisterApp(LWPR::SOCKET_FD_T fd)
{
	LWPR::Synchronized syn(m_Mutex);

	AppMap::iterator it = m_AppMap.find(fd);
	if(it != m_AppMap.end())
	{
		m_AppMap.erase(it);
	}
}

void AppDepot::UnRegisterApp(LWPR::SOCKETFDList fds)
{
	LWPR::Synchronized syn(m_Mutex);

	for(size_t i = 0; i < fds.size(); i++)
	{
		AppMap::iterator it = m_AppMap.find(fds[i]);
		if(it != m_AppMap.end())
		{
			m_AppMap.erase(it);
		}
	}
}

void AppDepot::GetAllApp(AppMap& apps)
{
	LWPR::Synchronized syn(m_Mutex);
	apps = m_AppMap;
}

void AppDepot::UpdateApp(LWPR::SOCKET_FD_T fd)
{
	LWPR::Synchronized syn(m_Mutex);

	AppMap::iterator it = m_AppMap.find(fd);
	if(it != m_AppMap.end())
	{
		it->second.nLastUpdateTime = time(NULL);
	}
}

bool AppDepot::UnRegisterApp(LWPR::SOCKET_FD_T fd, LWPR::SOCKET_ADDR_T& addr)
{
	LWPR::Synchronized syn(m_Mutex);

	AppMap::iterator it = m_AppMap.find(fd);
	if(it != m_AppMap.end())
	{
		addr = it->second.tSocketAddr;
		m_AppMap.erase(it);
		return true;
	}

	return false;
}
