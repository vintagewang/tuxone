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
#include "LocalServiceStub.h"
#include "Message.h"


//----------------------------------------------------------------
// SERVICE_ITEM_T
//----------------------------------------------------------------
service_item_t::service_item_t()
{
}

//----------------------------------------------------------------
// LocalServiceStub
//----------------------------------------------------------------
LocalServiceStub::LocalServiceStub()
{
}

LocalServiceStub::~LocalServiceStub()
{
}

LocalServiceStub* LocalServiceStub::GetInstance()
{
	static LocalServiceStub *singleton = new LocalServiceStub();
	return singleton;
}

bool LocalServiceStub::PickServers(const char* svc, ServerProxyVectorHolder& holder)
{
	assert(NULL != svc);

	ServerProxyVector servers;

	{
		LWPR::Synchronized syn(m_Mutex);
		SERVICE_ITEM_T& item = m_ServiceItemMap[svc];
		for(size_t i = 0; i < item.vtServerProxy.size(); i++)
		{
			ServerProxy_ptr pServerProxy = item.vtServerProxy[i];
			if(pServerProxy->IsServerOK())
			{
				servers.push_back(ServerProxy_var::Duplicate(pServerProxy));
			}
		}
	}

	holder = servers;

	return servers.size() > 0;
}

void LocalServiceStub::GetAllServiceName(std::vector<std::string>& svcs)
{
	LWPR::Synchronized syn(m_Mutex);

	for(ServiceItemMap::iterator it = m_ServiceItemMap.begin();
	    it != m_ServiceItemMap.end(); it++)
	{
		svcs.push_back(it->first);
	}
}

ServerProxy* LocalServiceStub::CreateServerProxy(const char* addr)
{
	assert(NULL != addr);

	if(m_ServerProxyMap.find(addr) == m_ServerProxyMap.end())
	{
		m_ServerProxyMap[addr] = new ServerProxy(addr);
	}

	return ServerProxy_var::Duplicate(m_ServerProxyMap[addr]);
}

void LocalServiceStub::UpdateServiceInfo(std::vector<SERVICE_INFO_T>& svcs)
{
	ServiceItemMap itemTmp;

	LWPR::Synchronized syn(m_Mutex);

	// 创建新的ServerProxy引用
	for(size_t i = 0; i < svcs.size(); i++)
	{
		ServerProxy_ptr pProxy = CreateServerProxy(LWPR::Socket::AddrTransform(svcs[i].tServerAddr).c_str());
		itemTmp[svcs[i].strSvcName].vtServerProxy.push_back(pProxy);
	}

	// 释放之前持有的ServerProxy引用
	for(ServiceItemMap::iterator it = m_ServiceItemMap.begin();
	    it != m_ServiceItemMap.end(); it++)
	{
		for(size_t k = 0; k < it->second.vtServerProxy.size(); k++)
		{
			it->second.vtServerProxy[k]->DecRef();
		}
	}

	// 对比消除没有用的ServerProxy
	for(ServerProxyMap::iterator kt = m_ServerProxyMap.begin();
	    kt != m_ServerProxyMap.end();)
	{
		bool bFound = false;

		for(ServiceItemMap::iterator it = itemTmp.begin();
		    it != itemTmp.end() && !bFound; it++)
		{
			for(size_t i = 0; i < it->second.vtServerProxy.size() && !bFound; i++)
			{
				if(kt->second == it->second.vtServerProxy[i])
				{
					bFound = true;
				}
			}
		}

		if(!bFound)
		{
			kt->second->Destroy();
			m_ServerProxyMap.erase(kt++);
		}
		else
		{
			kt++;
		}
	}

	// 保存新订阅的Service
	m_ServiceItemMap = itemTmp;
}

void LocalServiceStub::DisconnectAll()
{
	LWPR::Synchronized syn(m_Mutex);

	for(ServiceItemMap::iterator it = m_ServiceItemMap.begin();
	    it != m_ServiceItemMap.end(); it++)
	{
		SERVICE_ITEM_T& item = it->second;
		for(size_t i = 0; i < item.vtServerProxy.size(); i++)
		{
			ServerProxy_ptr pServerProxy = item.vtServerProxy[i];
			pServerProxy->DisconnectServer(true);
		}
	}
}
