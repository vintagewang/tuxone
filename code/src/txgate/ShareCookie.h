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
#ifndef TX_SHARE_COOKIE_H__
#define TX_SHARE_COOKIE_H__
#include <lwpr.h>

class ShareCookie
{
public:

	ShareCookie();
	~ShareCookie();
	static ShareCookie* GetInstance();

	LWPR::SOCKET_FD_T FdTCPListening();
	void FdTCPListening(LWPR::SOCKET_FD_T fd);

	LWPR::SOCKET_FD_T FdUDPListening();
	void FdUDPListening(LWPR::SOCKET_FD_T fd);

	LWPR::UINT16 UDPPort();
	void UDPPort(LWPR::UINT16 port);

private:
	LWPR::SOCKET_FD_T	m_nFdTCPListening;
	LWPR::SOCKET_FD_T	m_nFdUDPListening;
	LWPR::UINT16		m_nUDPPort;

};

#endif // end of TX_SHARE_COOKIE_H__
