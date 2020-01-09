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
#include "GateNSHandlerImpl.h"
#include "BulletinBoard.h"
#include "GateConfig.h"
#include "AppDepot.h"
#include "HousekeepingThread.h"


GateNSHandlerImpl::GateNSHandlerImpl()
{
	DEBUG_FUNCTION();
}

GateNSHandlerImpl::~GateNSHandlerImpl()
{
	DEBUG_FUNCTION();
}

LWPR::SOCKET_RET_TYPE_E GateNSHandlerImpl::DoCloseExpiredSocket(LWPR::SOCKET_FD_T fd)
{
	DEBUG_FUNCTION();

	LWPR::SOCKET_RET_TYPE_E ret = LWPR::SOCKET_RET_OK;
	return ret;
}

void GateNSHandlerImpl::DoClientAmountUpdate(int amount, LWPR::SOCKET_FD_T fd, bool increase)
{
	DEBUG_FUNCTION();

	BulletinBoard::GetInstance()->UpdateWSHConnectionTotal(increase);

	if(!increase)
	{
		LWPR::SOCKET_ADDR_T addr;
		if(AppDepot::GetInstance()->UnRegisterApp(fd, addr))
		{
			BulletinBoard::GetInstance()->UnregisterService(addr);
			HousekeepingThread::GetInstance()->Wakeup(true);
			logger->info(LTRACE, "App, [%s] disconnected", LWPR::Socket::AddrTransform(addr).c_str());
		}
	}
}

Message* GateNSHandlerImpl::InvokeImpl(LWPR::SOCKET_FD_T fd, const TUX_HEADER_MESSAGE_T& req_header, LWPR::InputStream& req_body)
{
	DEBUG_FUNCTION();

	Message* pMsgRep = NULL;

	try
	{
		switch(req_header.nMsgType)
		{
			// 管理工具类：获取公告板数据流
		case TUX_MSG_TYPE_ADMIN_GET_BB_REQ:
			{
				// 解析请求报文
				TUX_FETCH_BB_REQ_T msgReq;
				msgReq.Unmarshal(req_body);
				msgReq.MsgHeader(req_header);

				// 创建应答报文
				TUX_FETCH_BB_REP_T* pMsgRepCurrent = new TUX_FETCH_BB_REP_T();
				pMsgRep = pMsgRepCurrent;

				// 业务逻辑实现
				FetchBB(fd, msgReq, *pMsgRepCurrent);
			}
			break;
			// APP与Gate通信：注册APP、发布SERVICE
		case TUX_MSG_TYPE_APP_REGISTER_REQ:
			{
				// 解析请求报文
				TUX_APP_REGISTER_REQ_T msgReq;
				msgReq.Unmarshal(req_body);
				msgReq.MsgHeader(req_header);

				// 创建应答报文
				TUX_APP_REGISTER_REP_T* pMsgRepCurrent = new TUX_APP_REGISTER_REP_T();
				pMsgRep = pMsgRepCurrent;

				// 业务逻辑实现
				RegisterApp(fd, msgReq, *pMsgRepCurrent);
			}
			break;
			// APP与Gate通信：Client向Gate订阅服务
		case TUX_MSG_TYPE_APP_SUBSCIBE_REQ:
			{
				// 解析请求报文
				TUX_APP_SUBSCIBE_REQ_T msgReq;
				msgReq.Unmarshal(req_body);
				msgReq.MsgHeader(req_header);

				// 创建应答报文
				TUX_APP_SUBSCIBE_REP_T* pMsgRepCurrent = new TUX_APP_SUBSCIBE_REP_T();
				pMsgRep = pMsgRepCurrent;

				// 业务逻辑实现
				SubscibeService(fd, msgReq, *pMsgRepCurrent);
			}
			break;
		default:
			break;
		}
	}
	catch(const LWPR::Exception& e)
	{
		logger->error(LTRACE, "%s", e.what());
		delete pMsgRep;
		pMsgRep = NULL;
	}
	catch(...)
	{
		logger->error(LTRACE, "Unknow Exception");
		delete pMsgRep;
		pMsgRep = NULL;
	}

	return pMsgRep;
}

void GateNSHandlerImpl::FetchBB(LWPR::SOCKET_FD_T fd, TUX_FETCH_BB_REQ_T& request, TUX_FETCH_BB_REP_T& response)
{
	DEBUG_FUNCTION();

	// 获取公告板数据
	BB_STREAM_T bb;
	BulletinBoard::GetInstance()->GetBBStream(bb, request.bFetchAll);

	LWPR::OutputStream out;
	bb.Marshal(out);

	// 设置应答报文
	response.vBufBody = out.GetBuffer();
	response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_OK;
}

void GateNSHandlerImpl::RegisterApp(LWPR::SOCKET_FD_T fd, TUX_APP_REGISTER_REQ_T& request, TUX_APP_REGISTER_REP_T& response)
{
	DEBUG_FUNCTION();

	bool bRegisterOK = false;

	// 注册Service（TODO: 此处存在性能问题，后续优化）
	if((request.nAppType & TUX_APP_TYPE_SERVER) == TUX_APP_TYPE_SERVER)
	{
		for(size_t i = 0; i < request.vtServices.size(); i++)
		{
			BB_SERVICE_ENTITY_T svc;
			memset(&svc, 0, sizeof(svc));

			svc.tServerAddr = request.tServerAddr;
			strncpy(svc.strSvcName, request.vtServices[i].c_str(), TUXONE_SERVICE_NAME_LENGTH);

			if(BulletinBoard::GetInstance()->RegisterService(svc))
			{
				logger->info(LTRACE, "Service, [%s] [%s] register OK", svc.strSvcName, LWPR::Socket::AddrTransform(svc.tServerAddr).c_str());
				response.vtServices.push_back(svc.strSvcName);
				bRegisterOK = true;
			}
			else
			{
				logger->warn(LTRACE, "Service, [%s] [%s] register FAILED, because BB service table not enough",
				             svc.strSvcName, LWPR::Socket::AddrTransform(svc.tServerAddr).c_str());
				break;
			}
		}
	}

	// 注册到本地
	APP_ITEM_T item;
	item.nAppType = request.nAppType;
	item.nLastUpdateTime = time(NULL);
	item.tSocketAddr = request.tServerAddr;
	AppDepot::GetInstance()->RegisterApp(fd, item);

	// 通知线程
	if(bRegisterOK) HousekeepingThread::GetInstance()->Wakeup(true);

	// 设置应答报文
	response.nBBUpdateSeq = BulletinBoard::GetInstance()->GetSVCUpdateSeq();
	response.nAppHeartInterval = GateConfig::GetInstance()->TUXONE_GATE_WSH_APP_HEART_INTERVAL();
	response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_OK;
}

void GateNSHandlerImpl::SubscibeService(LWPR::SOCKET_FD_T fd, TUX_APP_SUBSCIBE_REQ_T& request, TUX_APP_SUBSCIBE_REP_T& response)
{
	DEBUG_FUNCTION();

	// 设置应答报文
	BBSVCList bbsvcs;
	response.nBBUpdateSeq = BulletinBoard::GetInstance()->GetAllSVC(request.vtServices, bbsvcs);

	// 转化报文
	for(size_t i = 0; i < bbsvcs.size(); i++)
	{
		SERVICE_INFO_T info;
		info.strSvcName = bbsvcs[i].strSvcName;
		info.tServerAddr = bbsvcs[i].tServerAddr;
		response.vtServiceInfo.push_back(info);
	}

	response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_OK;
}

// test for source owners feature
