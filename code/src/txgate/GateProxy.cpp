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
#include "GateProxy.h"
#include "Message.h"
#include "GateConfig.h"
#include "BulletinBoard.h"
#include "ShareCookie.h"
#include <assert.h>

GateProxy::GateProxy(const char* addr, LWPR::INT32 index):
	m_strGateAddr(addr),
	m_nGateIndex(index),
	m_nFDGate(-1),
	m_nBBUpdateSeqSync(0),
	m_nBBUpdateSeqHeart(0)
{
	DEBUG_FUNCTION();
	assert(NULL != addr);
}

GateProxy::~GateProxy()
{
	DEBUG_FUNCTION();
	Disconnect();
}

bool GateProxy::SendHeartBeat()
{
	DEBUG_FUNCTION();
	bool bResult = Connect();
	if(bResult)
	{
		TUX_APP_REGISTER_REQ_T req;
		TUX_APP_REGISTER_REP_T rep;

		// 构造请求报文
		req.nAppType = TUX_APP_TYPE_CLIENT;

		// 发起请求
		TUX_INVOKE_RESULT_E nResult = Invoke(m_nFDGate, req, rep, GateConfig::GetInstance()->TUXONE_GATE_SOCKET_RWTIMEOUT());
		if(nResult == TUX_INVOKE_OK && TUX_MSG_RET_CODE_OK == rep.MsgHeader().nMsgRetCode)
		{
			m_nBBUpdateSeqHeart = rep.nBBUpdateSeq;
			bResult = true;
			logger->debug(LTRACE, "heart beat to other gate [%s] OK.", m_strGateAddr.c_str());
		}
		else
		{
			Disconnect();
			bResult = false;
			logger->warn(LTRACE, "heart beat to other gate [%s] Failed.", m_strGateAddr.c_str());
		}
	}

	return bResult;
}

bool GateProxy::SyncService()
{
	DEBUG_FUNCTION();
	bool bResult = Connect();
	if(bResult && m_nBBUpdateSeqSync != m_nBBUpdateSeqHeart)
	{
		// 获取公告板信息
		TUX_FETCH_BB_REQ_T req;
		TUX_FETCH_BB_REP_T rep;
		req.bFetchAll = false;
		TUX_INVOKE_RESULT_E code = Invoke(m_nFDGate, req, rep, GateConfig::GetInstance()->TUXONE_GATE_SOCKET_RWTIMEOUT());
		// 处理应答结果
		if(code == TUX_INVOKE_OK && rep.MsgHeader().nMsgRetCode == TUX_MSG_RET_CODE_OK)
		{
			BB_STREAM_T bb;
			LWPR::InputStream in(rep.vBufBody);
			bb.Unmarshal(in);
			bResult = true;

			logger->debug(LTRACE, "Fetch %d service from other gate", bb.tblSVCLength);
			BulletinBoard::GetInstance()->SynBB(bb, m_nGateIndex);
			if(BulletinBoard::GetInstance()->IsSvcUpdated())
			{
				logger->info(LTRACE, "Update bulltin board service table from other gate [%s] OK.", m_strGateAddr.c_str());
				BulletinBoard::GetInstance()->WakeupOtherWSH(ShareCookie::GetInstance()->FdUDPListening());
			}
		}
		else
		{
			bResult = false;
			Disconnect();
			logger->error(LTRACE, "Fetch bb return code %d.", rep.MsgHeader().nMsgRetCode);
		}
	}

	return bResult;
}

bool GateProxy::Connect()
{
	bool bResult = true;

	if(-1 == m_nFDGate)
	{
		m_nFDGate = LWPR::Socket::ConnectRemoteHost(m_strGateAddr.c_str());
		bResult = m_nFDGate != -1;

		if(bResult)
		{
			logger->info(LTRACE, "connect gate [%s] OK.", m_strGateAddr.c_str());
		}
		else
		{
			logger->warn(LTRACE, "connect gate [%s] Failed.", m_strGateAddr.c_str());
		}
	}

	return bResult;
}

void GateProxy::Disconnect()
{
	LWPR::Socket::CloseSocket(m_nFDGate);
	logger->info(LTRACE, "disconnect from gate [%s]", m_strGateAddr.c_str());

	ClearBB();
}

void GateProxy::ClearBB()
{
	BB_STREAM_T bb;
	BulletinBoard::GetInstance()->SynBB(bb, m_nGateIndex);
	logger->debug(LTRACE, "Clear Service in BB, Because gate [%s] break down", m_strGateAddr.c_str());
}
