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
#ifndef TUX_GATE_CHECK_THREAD_H__
#define TUX_GATE_CHECK_THREAD_H__
#include <lwpr.h>

class GateCheckThread : public LWPR::Thread,
	public LWPR::Resource
{
public:
	GateCheckThread();
	~GateCheckThread();
	static GateCheckThread* GetInstance();

	/**
	 * 初始化
	 */
	bool Init();

	/**
	 * 设置NetServer，同时标记App为服务端
	 */
	void SetNetServer(LWPR::NetServer* pNetServer);

	/**
	 * 标记App为客户端身份
	 */
	void MarkClient();

	/**
	 * 唤醒线程（线程阻塞在Select函数调用中）
	 */
	bool Wakeup();

	/**
	 * 是否有Gate下达的通知
	 */
	bool HasGateNotice();

	/**
	 * 从Gate更新Service
	 */
	bool DownloadService();

	void Term();

	void Go();

private:
	/**
	 * 获取App类型
	 */
	LWPR::UINT32 GetAppType();

	/**
	 * 获取Server的监听地址
	 */
	void GetServerListeningAddr(LWPR::SOCKET_ADDR_T& addr);

	/**
	 * 连接Gate
	 */
	bool ConnectGate();

	/**
	 * 同Gate断开
	 */
	void DisconnectGate();

	/**
	 * 向Gate发送心跳
	 */
	bool SendHeartBeat();

	/**
	 * 等待通知（其他线程通知、Gate下发通知、超时自动唤醒）
	 */
	void WaitForNotify();

	/**
	 * 线程函数
	 */
	virtual void Run();

	/**
	 * 注册
	 */
	bool RegisterToGate();

	/**
	 * 订阅
	 */
	bool SubscibeFromGate();

private:
	LWPR::SOCKET_FD_T	m_nFDPIPERead;
	LWPR::SOCKET_FD_T	m_nFDPIPEWrite;

	typedef std::vector<std::string> GateAddrs;
	GateAddrs			m_GateAddrs;
	LWPR::UINT32		m_nGateIndex;
	//
	// 与 Gate 通信相关
	//
	LWPR::Mutex			m_MutexGate;
	LWPR::SOCKET_FD_T	m_nFDGate;
	LWPR::UINT32		m_nBBUpdateSeqRegister;
	LWPR::UINT32		m_nBBUpdateSeqSubscibe;

	// 只有当前线程访问，不需要加锁
	LWPR::UINT32		m_nHeartInterval;

	volatile bool		m_bGateNotified;

	volatile bool		m_bTermed;

	LWPR::Mutex			m_MutexOther;
	LWPR::UINT32		m_nAppType;
	LWPR::NetServer*	m_pNetServer;

	// 其他线程用来同当前线程竞争资源
	LWPR::Mutex			m_MutexResource;
};

#endif // end of TUX_GATE_CHECK_THREAD_H__
