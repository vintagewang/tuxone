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
#ifndef TUX_WAIT_NOTIFY_THREAD_H__
#define TUX_WAIT_NOTIFY_THREAD_H__
#include <lwpr.h>

/**
 * 此线程负责接收UDP通知
 */
class WaitNotifyThread : public LWPR::Thread
{
public:

	WaitNotifyThread();
	~WaitNotifyThread();
	static WaitNotifyThread* GetInstance();

	void SetUDPFd(LWPR::SOCKET_FD_T fd);

	virtual void Run();

private:
	LWPR::SOCKET_FD_T	m_nUDPFd;

};

#endif // end of TUX_WAIT_NOTIFY_THREAD_H__
