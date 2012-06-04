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
#include "ServiceRepository.h"


ServiceRepository::ServiceRepository()
{
}

ServiceRepository::~ServiceRepository()
{
}

ServiceRepository* ServiceRepository::GetInstance()
{
	static ServiceRepository *singleton = new ServiceRepository();
	return singleton;
}

void ServiceRepository::Register(const ServerServiceList& svcs)
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_Mutex);
	for(ServerServiceList::const_iterator it = svcs.begin(); it != svcs.end(); it++)
	{
		if(m_SVCWill.find(it->strSVCName) == m_SVCWill.end()
		   && m_SVCAlready.find(it->strSVCName) == m_SVCAlready.end())
		{
			m_SVCWill[it->strSVCName] = *it;
		}
	}
}

void ServiceRepository::Unregister(const char* svcname)
{
	DEBUG_FUNCTION();

	assert(NULL != svcname);

	LWPR::Synchronized syn(m_Mutex);

	ServiceMap::iterator it = m_SVCWill.find(svcname);
	if(it != m_SVCWill.end())
	{
		m_SVCWill.erase(it);
	}

	it = m_SVCAlready.find(svcname);
	if(it != m_SVCAlready.end())
	{
		m_SVCAlready.erase(it);
	}

	// TODO: 将来考虑增加从公告板注销
}

SVCFUNC_FP ServiceRepository::GetSvcFunction(const char* name)
{
	DEBUG_FUNCTION();

	assert(NULL != name);

	try
	{
		LWPR::Synchronized syn(m_Mutex);
		ServiceMap::iterator it;
		if((it = m_SVCAlready.find(name)) != m_SVCAlready.end()
		   || (it = m_SVCWill.find(name)) != m_SVCWill.end())
		{
			return it->second.fpSVCFunc;
		}
	}
	catch(...)
	{
	}

	return (SVCFUNC_FP)NULL;
}

void ServiceRepository::GetSvcListWill(ServiceNameVector& svcs)
{
	DEBUG_FUNCTION();

	svcs.clear();

	LWPR::Synchronized syn(m_Mutex);
	for(ServiceMap::iterator it = m_SVCWill.begin(); it != m_SVCWill.end(); it++)
	{
		svcs.push_back(it->first);
	}
}

void ServiceRepository::RegisterSvcDone(const ServiceNameVector& svcs)
{
	DEBUG_FUNCTION();

	if(svcs.size() > 0)
	{
		LWPR::Synchronized syn(m_Mutex);
		for(size_t i = 0; i < svcs.size(); i++)
		{
			ServiceMap::iterator it = m_SVCWill.find(svcs[i]);
			if(it != m_SVCWill.end())
			{
				m_SVCAlready[it->first] = it->second;
				m_SVCWill.erase(it);
			}
		}
	}
}

void ServiceRepository::MakeSvcRegister()
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_Mutex);
	if(m_SVCAlready.size() > 0)
	{
		m_SVCWill.insert(m_SVCAlready.begin(), m_SVCAlready.end());
		m_SVCAlready.clear();
	}
}
