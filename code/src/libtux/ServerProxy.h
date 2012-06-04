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
#ifndef TX_SERVER_PROXY_H__
#define TX_SERVER_PROXY_H__
#include <lwpr.h>
#include <vector>
#include "Message.h"

class ServerProxy;
typedef std::vector< ServerProxy* >	ServerProxyVector;

class ServerProxy : public LWPR::Object
{
public:

	ServerProxy(const char* addr);
	~ServerProxy();

public:
	bool IsServerOK();

	void Destroy();

	int GetCallTimes();

	bool ConnectServer();

	void DisconnectServer(bool immediately = false);

	TUX_INVOKE_RESULT_E CallServer(Message& msg_in, Message& msg_out);

private:
	std::string					m_strAddrServer;
	volatile bool				m_bDestroied;

	LWPR::Mutex					m_Mutex;
	LWPR::SOCKET_FD_T			m_nFDServer;

	LWPR::AtomicInteger			m_nTimesTimeout;
	LWPR::AtomicInteger			m_nTimesCall;

	static LWPR::AtomicInteger	g_nFDCount;
};



DECLAREVAR(ServerProxy);

class ServerProxyVectorHolder
{
public:
	ServerProxyVectorHolder(ServerProxyVector& servers);

	~ServerProxyVectorHolder();

	void operator = (const ServerProxyVector& servers);

private:
	void ReleaseServers();

private:
	ServerProxyVector& m_ServerProxyVector;
};

#endif // end of TX_SERVER_PROXY_H__
