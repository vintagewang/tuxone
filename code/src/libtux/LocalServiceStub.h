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
#ifndef TUX_SERVICE_STUB_H__
#define TUX_SERVICE_STUB_H__
#include <lwpr.h>
#include <map>
#include <vector>
#include "ServerProxy.h"
#include "Message.h"

typedef struct service_item_t
{
	std::vector<ServerProxy_ptr>	vtServerProxy;

	service_item_t();
} SERVICE_ITEM_T;

// svc -- servers
typedef std::map< std::string, SERVICE_ITEM_T > ServiceItemMap;

// addr -- server
typedef std::map < std::string, ServerProxy* > ServerProxyMap;

class LocalServiceStub
{
public:

	LocalServiceStub();
	~LocalServiceStub();
	static LocalServiceStub* GetInstance();

public:

	bool PickServers(const char* svc, ServerProxyVectorHolder& holder);

	void GetAllServiceName(std::vector<std::string>& svcs);

	void UpdateServiceInfo(std::vector<SERVICE_INFO_T>& svcs);

	ServerProxy* CreateServerProxy(const char* addr);

	void DisconnectAll();

private:
	LWPR::Mutex		m_Mutex;
	ServiceItemMap	m_ServiceItemMap;
	ServerProxyMap	m_ServerProxyMap;
};

#endif // end of TUX_SERVICE_STUB_H__
