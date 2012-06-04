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
#ifndef TUX_HOUSEKEEPING_THREAD_H__
#define TUX_HOUSEKEEPING_THREAD_H__
#include <lwpr.h>
#include <time.h>

/**
 * 1、检测Server是否过期，销毁过期的Server
 * 2、向没有过期的Server发送Service变更通知
 * 3、向其他WSH进程发送Service变更通知
 */
class HousekeepingThread : public LWPR::Thread, public LWPR::Resource
{
public:

	HousekeepingThread();
	~HousekeepingThread();
	static HousekeepingThread* GetInstance();

	void SetNetServer(LWPR::NetServer* pNetServer);
	void SetUDPFd(LWPR::SOCKET_FD_T fd);

	/**
	 * 唤醒线程
	 * self 表示是否是当前进程唤醒
	 */
	void Wakeup(bool self);

	/**
	 * 是否是被自身唤醒
	 */
	bool IsWakeupBySelf();

	virtual void Run();

private:
	/**
	 * 向所有客户端发送广播
	 */
	void BroadcastNotice();

	/**
	 * 如果是当前WSH进程导致公告板变更，则通知其他WSH进程
	 */
	void NotifyOtherWSH();

private:
	LWPR::NetServer*	m_pNetServer;
	time_t				m_nLastCheckTime;
	LWPR::SOCKET_FD_T	m_nUDPFd;

	LWPR::Mutex			m_Mutex;
	bool				m_bWakeupSelf;
};

#endif // end of TUX_HOUSEKEEPING_THREAD_H__
