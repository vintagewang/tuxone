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
#include "GateCheckThread.h"
#include "AppConfig.h"
#include "Message.h"
#include "ServiceRepository.h"
#include "LocalServiceStub.h"
#include <time.h>


GateCheckThread::GateCheckThread() :
	m_nFDPIPERead(-1),
	m_nFDPIPEWrite(-1),
	m_nGateIndex(0),
	m_nFDGate(-1),
	m_nBBUpdateSeqRegister(0),
	m_nBBUpdateSeqSubscibe(0),
	m_nHeartInterval(AppConfig::GetInstance()->TUXONE_APP_HEART_INTERVAL()),
	m_bGateNotified(false),
	m_bTermed(false),
	m_nAppType(TUX_APP_TYPE_UNKNOW),
	m_pNetServer(NULL)
{
	LWPR::StringUtil::SplitString(AppConfig::GetInstance()->TUXONE_APP_GATE_ADDR().c_str(), ';', m_GateAddrs);
}

GateCheckThread::~GateCheckThread()
{

}

GateCheckThread* GateCheckThread::GetInstance()
{
	static GateCheckThread *singleton = new GateCheckThread();
	return singleton;
}

bool GateCheckThread::Init()
{
	DEBUG_FUNCTION();

	// 创建管道
	int fdpipe[2] = {0};
	int nCode = pipe(fdpipe);
	if(-1 == nCode)
	{
		return false;
	}
	m_nFDPIPERead = fdpipe[0];
	m_nFDPIPEWrite = fdpipe[1];

	// 尝试连接Gate
	if(!ConnectGate())
	{
		logger->warn(LTRACE, "Connect gate Failed.");
	}

	return true;
}

void GateCheckThread::Run()
{
	while(IsContinue())
	{
		try
		{
			{
				LWPR::Synchronized syn(m_MutexGate);

				// 发送心跳（注册、发布Service，订阅Service）
				if(!(SendHeartBeat() || SendHeartBeat()))
				{
					// 与Gate心跳异常情况下，恢复为App自身配置的心跳参数
					m_nHeartInterval = AppConfig::GetInstance()->TUXONE_APP_HEART_INTERVAL();
				}

				// 等待通知（其他线程通知、Gate下发通知、超时自动唤醒）
				WaitForNotify();
			}

			logger->debug(LTRACE, "send heart beat to gate");

			LWPR::Synchronized syn(m_MutexResource);
		}
		catch(const LWPR::Exception& e)
		{
			logger->error(LTRACE, "%s", e.what());
		}
		catch(...)
		{
			logger->error(LTRACE, "Unknow exception");
		}
	}
}

bool GateCheckThread::ConnectGate()
{
	DEBUG_FUNCTION();

	for(size_t i = 0; i < m_GateAddrs.size() && m_nFDGate == -1 && !m_bTermed; i++)
	{
		std::string strGateAddr = m_GateAddrs[m_nGateIndex++ % m_GateAddrs.size()];
		m_nFDGate = LWPR::Socket::ConnectRemoteHost(strGateAddr.c_str());
		logger->info(LTRACE, "Connect Gate [%s] %s.", strGateAddr.c_str(), m_nFDGate != -1 ? "OK" : "FAILED");
	}

	return m_nFDGate != -1;
}

void GateCheckThread::DisconnectGate()
{
	DEBUG_FUNCTION();

	if(m_nFDGate != -1)
	{
		LWPR::Socket::CloseSocketWithShutdown(m_nFDGate);
		ServiceRepository::GetInstance()->MakeSvcRegister();
		logger->info(LTRACE, "disconnect from gate positive");
	}
}

void GateCheckThread::SetNetServer(LWPR::NetServer* pNetServer)
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_MutexOther);
	m_pNetServer = pNetServer;
	m_nAppType |= TUX_APP_TYPE_SERVER;
}

void GateCheckThread::GetServerListeningAddr(LWPR::SOCKET_ADDR_T& addr)
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_MutexOther);
	assert(NULL != m_pNetServer);
	std::string strAddr = m_pNetServer->GetListenAddr();
	LWPR::Socket::AddrTransform(strAddr.c_str(), addr);
}

void GateCheckThread::MarkClient()
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_MutexOther);
	m_nAppType |= TUX_APP_TYPE_CLIENT;
}

LWPR::UINT32 GateCheckThread::GetAppType()
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_MutexOther);
	return m_nAppType;
}

bool GateCheckThread::Wakeup()
{
	DEBUG_FUNCTION();
	char buf[1] = {0};
	LWPR::SOCKET_RET_TYPE_E ret = LWPR::Socket::WriteSocket(m_nFDPIPEWrite, buf, 1, AppConfig::GetInstance()->TUXONE_APP_SOCKET_RWTIMEOUT());
	return ret == LWPR::SOCKET_RET_OK;
}

bool GateCheckThread::HasGateNotice()
{
	return m_bGateNotified;
}

bool GateCheckThread::SendHeartBeat()
{
	DEBUG_FUNCTION();

	bool bResult = true;

	bResult = bResult && RegisterToGate();

	if((GetAppType() & TUX_APP_TYPE_CLIENT) == TUX_APP_TYPE_CLIENT
	   && m_nBBUpdateSeqRegister != m_nBBUpdateSeqSubscibe)
	{
		bResult = bResult && SubscibeFromGate();
	}

	return bResult;
}

bool GateCheckThread::RegisterToGate()
{
	DEBUG_FUNCTION();

	bool bResult = ConnectGate();
	if(bResult)
	{
		TUX_APP_REGISTER_REQ_T req;
		TUX_APP_REGISTER_REP_T rep;

		// 构造请求报文
		req.nAppType = GetAppType();
		if((req.nAppType & TUX_APP_TYPE_SERVER) == TUX_APP_TYPE_SERVER)
		{
			ServiceRepository::GetInstance()->GetSvcListWill(req.vtServices);
			GetServerListeningAddr(req.tServerAddr);
		}

		// 发起请求
		TUX_INVOKE_RESULT_E nResult = Invoke(m_nFDGate, req, rep, AppConfig::GetInstance()->TUXONE_APP_SOCKET_RWTIMEOUT());
		if(nResult == TUX_INVOKE_OK)
		{
			if(TUX_MSG_RET_CODE_OK == rep.MsgHeader().nMsgRetCode)
			{
				bResult = true;
				m_nBBUpdateSeqRegister = rep.nBBUpdateSeq;
				m_nHeartInterval = rep.nAppHeartInterval;
				ServiceRepository::GetInstance()->RegisterSvcDone(rep.vtServices);

				if((req.nAppType & TUX_APP_TYPE_SERVER) == TUX_APP_TYPE_SERVER)
				{
					logger->debug(LTRACE, "Register to gate OK, This is server [%s]", LWPR::Socket::AddrTransform(req.tServerAddr).c_str());
				}
				else
				{
					logger->debug(LTRACE, "Register to gate OK, This is not server");
				}
			}
			else
			{
				bResult = false;
				DisconnectGate();
				logger->error(LTRACE, "Register to gate, Invoke OK, But return error code [%d]", rep.MsgHeader().nMsgRetCode);
			}
		}
		else
		{
			bResult = false;
			DisconnectGate();
			logger->warn(LTRACE, "Register to gate, Invoke Failed [%d]", (int)nResult);
		}
	}

	return bResult;
}

bool GateCheckThread::SubscibeFromGate()
{
	DEBUG_FUNCTION();

	bool bResult = ConnectGate();
	if(bResult)
	{
		TUX_APP_SUBSCIBE_REQ_T req;
		TUX_APP_SUBSCIBE_REP_T rep;

		// 构造请求报文
		LocalServiceStub::GetInstance()->GetAllServiceName(req.vtServices);

		// 发起请求
		TUX_INVOKE_RESULT_E nResult = Invoke(m_nFDGate, req, rep, AppConfig::GetInstance()->TUXONE_APP_SOCKET_RWTIMEOUT());
		if(nResult == TUX_INVOKE_OK)
		{
			if(TUX_MSG_RET_CODE_OK == rep.MsgHeader().nMsgRetCode)
			{
				bResult = true;
				m_bGateNotified = false;
				m_nBBUpdateSeqSubscibe = rep.nBBUpdateSeq;
				LocalServiceStub::GetInstance()->UpdateServiceInfo(rep.vtServiceInfo);
				logger->debug(LTRACE, "Subscibe from gate OK.");
			}
			else
			{
				bResult = false;
				DisconnectGate();
				logger->error(LTRACE, "Subscibe from gate, Invoke OK, But return error code [%d]", rep.MsgHeader().nMsgRetCode);
			}
		}
		else
		{
			bResult = false;
			DisconnectGate();
			logger->warn(LTRACE, "Subscibe from gate, Invoke Failed [%d]", (int)nResult);
		}
	}
	return bResult;
}

void GateCheckThread::WaitForNotify()
{
	DEBUG_FUNCTION();
	time_t tBegin = time(NULL);

	bool bContinue = true;

	do
	{
		// 描述符可读集合
		fd_set setfds;
		FD_ZERO(&setfds);
		FD_SET(m_nFDPIPERead, &setfds);

		// 取最大描述符
		LWPR::SOCKET_FD_T maxfd = m_nFDPIPERead;

		// 是否监听Gate链路
		if(m_nFDGate != -1)
		{
			FD_SET(m_nFDGate, &setfds);
			maxfd = m_nFDGate > maxfd ? m_nFDGate : maxfd;
		}

		// select超时设置
		struct timeval tv = {0};
		tv.tv_sec = m_nHeartInterval - (time(NULL) - tBegin);
		tv.tv_usec = 0;

		// 如果时间到，则退出
		if(tv.tv_sec <= 0)
		{
			return;
		}

		int nCode = select(maxfd + 1, &setfds, NULL, NULL, &tv);
		// 有描述符可读
		if(nCode > 0)
		{
			for(int i = 0; i <= maxfd; i++)
			{
				if(FD_ISSET(i, &setfds))
				{
					// 被其他线程唤醒
					if(i == m_nFDPIPERead)
					{
						bContinue = false;
						LWPR::Socket::ClearSocket(m_nFDPIPERead);
						logger->debug(LTRACE, "awake by other thread");
					}
					// 收到Gate下发的通知
					else if(i == m_nFDGate)
					{
						logger->debug(LTRACE, "gate socket become readable");

						LWPR::Buffer buf(TUX_HEADER_MESSAGE_SIZE);
						LWPR::SOCKET_RET_TYPE_E nRet = LWPR::Socket::ReadSocket(m_nFDGate, buf, TUX_HEADER_MESSAGE_SIZE, 0);
						if(LWPR::SOCKET_RET_OK == nRet)
						{
							m_bGateNotified = true;
							logger->info(LTRACE, "receive gate notice");
						}
						else
						{
							bContinue = false;
							DisconnectGate();
						}
					}
				}
			}
		}
		// 发生超时
		else if(nCode == 0)
		{
			logger->debug(LTRACE, "select timeout");
			bContinue = false;
		}
		// 发生错误
		else if(nCode == -1)
		{
			logger->error(LTRACE, "select error");
			bContinue = false;
		}
	}
	while(bContinue);
}

bool GateCheckThread::DownloadService()
{
	DEBUG_FUNCTION();

	LWPR::Synchronized syn(m_MutexResource);

	bool bResult = Wakeup();
	if(bResult)
	{
		LWPR::Synchronized syn2(m_MutexGate);

		bResult = SubscibeFromGate();
	}

	return bResult;
}

void GateCheckThread::Term()
{
	LWPR::Synchronized syn(m_MutexResource);

	bool bResult = Wakeup();
	if(bResult)
	{
		LWPR::Synchronized syn2(m_MutexGate);

		m_bTermed = true;

		DisconnectGate();
	}
}

void GateCheckThread::Go()
{
	LWPR::Synchronized syn(m_MutexResource);

	bool bResult = Wakeup();
	if(bResult)
	{
		LWPR::Synchronized syn2(m_MutexGate);

		m_bTermed = false;
	}
}
