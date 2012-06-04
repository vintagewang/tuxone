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
#include "ShareCookie.h"


ShareCookie::ShareCookie() :
	m_nFdTCPListening(-1),
	m_nFdUDPListening(-1),
	m_nUDPPort(-1)
{
}

ShareCookie::~ShareCookie()
{
}

ShareCookie* ShareCookie::GetInstance()
{
	static ShareCookie *singleton = new ShareCookie();
	return singleton;
}

LWPR::SOCKET_FD_T ShareCookie::FdTCPListening()
{
	assert(-1 != m_nFdTCPListening);
	return m_nFdTCPListening;
}

void ShareCookie::FdTCPListening(LWPR::SOCKET_FD_T fd)
{
	m_nFdTCPListening = fd;
}

LWPR::SOCKET_FD_T ShareCookie::FdUDPListening()
{
	assert(-1 != m_nFdUDPListening);
	return m_nFdUDPListening;
}

void ShareCookie::FdUDPListening(LWPR::SOCKET_FD_T fd)
{
	m_nFdUDPListening = fd;
}

LWPR::UINT16 ShareCookie::UDPPort()
{
	return m_nUDPPort;
}

void ShareCookie::UDPPort(LWPR::UINT16 port)
{
	m_nUDPPort = port;
}
