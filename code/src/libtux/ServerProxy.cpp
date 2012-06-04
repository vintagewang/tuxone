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
#include "ServerProxy.h"
#include "AppConfig.h"

LWPR::AtomicInteger ServerProxy::g_nFDCount = 0;

ServerProxy::ServerProxy(const char* addr):
	m_strAddrServer(addr),
	m_bDestroied(false),
	m_nFDServer(-1),
	m_nTimesTimeout(0),
	m_nTimesCall(0)
{
	assert(NULL != addr);
}

ServerProxy::~ServerProxy()
{
	DisconnectServer(true);
}

bool ServerProxy::IsServerOK()
{
	DEBUG_FUNCTION();
	return !(m_nTimesTimeout >= 3) && !m_bDestroied;
}

int ServerProxy::GetCallTimes()
{
	DEBUG_FUNCTION();
	return m_nTimesCall;
}

TUX_INVOKE_RESULT_E ServerProxy::CallServer(Message& msg_in, Message& msg_out)
{
	DEBUG_FUNCTION();

	TUX_INVOKE_RESULT_E nResult = TUX_INVOKE_CONNECT_FAILED;

	LWPR::Synchronized syn(m_Mutex);

	if(!m_bDestroied)
	{
		if(ConnectServer())
		{
			nResult = Invoke(m_nFDServer, msg_in, msg_out, AppConfig::GetInstance()->TUXONE_APP_SOCKET_RWTIMEOUT());
			DisconnectServer(nResult != TUX_INVOKE_OK || msg_out.MsgHeader().DisconnectFromServer());
		}
	}

	return nResult;
}

bool ServerProxy::ConnectServer()
{
	DEBUG_FUNCTION();

	if(-1 == m_nFDServer)
	{
		m_nFDServer = LWPR::Socket::ConnectRemoteHost(m_strAddrServer.c_str());

		if(m_nFDServer != -1)
		{
			g_nFDCount++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

void ServerProxy::DisconnectServer(bool immediately /*= false*/)
{
	DEBUG_FUNCTION();

	if(m_nFDServer != -1)
	{
		if(immediately)
		{
			LWPR::Socket::CloseSocketWithShutdown(m_nFDServer);
			m_nFDServer = -1;
			g_nFDCount--;
			logger->debug(LTRACE, "disconnect from server positive");
		}
		else if(g_nFDCount > AppConfig::GetInstance()->TUXONE_APP_CLIENT_CACHEDCONNECTIONS())
		{
			LWPR::Socket::CloseSocketWithShutdown(m_nFDServer);
			m_nFDServer = -1;
			g_nFDCount--;
			logger->debug(LTRACE, "disconnect from server, because cached connection over limit");
		}
	}
}

void ServerProxy::Destroy()
{
	m_bDestroied = true;
	DecRef();
}

//----------------------------------------------------------------
// class ServerProxyVectorHolder
//----------------------------------------------------------------
ServerProxyVectorHolder::ServerProxyVectorHolder(ServerProxyVector& servers) :
	m_ServerProxyVector(servers)
{

}

ServerProxyVectorHolder::~ServerProxyVectorHolder()
{
	ReleaseServers();
}

void ServerProxyVectorHolder::operator=(const ServerProxyVector& servers)
{
	// 释放之前持有的引用
	ReleaseServers();

	// 持有新的引用
	m_ServerProxyVector = servers;
}

void ServerProxyVectorHolder::ReleaseServers()
{
	for(size_t i = 0; i < m_ServerProxyVector.size(); i++)
	{
		ServerProxy_ptr p = m_ServerProxyVector[i];
		p->DecRef();
	}

	m_ServerProxyVector.clear();
}
