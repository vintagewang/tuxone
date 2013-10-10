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
#include "AppNSHandlerImpl.h"
#include "AppConfig.h"
#include "ServiceRepository.h"
#include "TLSLibtux.h"
#include "ATMIBufferManager.h"
#include "View32Manager.h"
#include "FML32API.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

AppNSHandlerImpl::AppNSHandlerImpl()
	: m_argv(NULL), m_argc(0)
{
	DEBUG_FUNCTION();
	std::vector<std::string>& args = AppConfig::GetInstance()->GetOtherArgs();
	m_argc = args.size();
	m_argv = new char*[m_argc];
	for(size_t i = 0; i < (size_t)m_argc; i++)
	{
		m_argv[i] = (char *)args[i].c_str();
	}
}

AppNSHandlerImpl::~AppNSHandlerImpl()
{
	DEBUG_FUNCTION();
	delete[] m_argv;
}

LWPR::SOCKET_RET_TYPE_E AppNSHandlerImpl::DoCloseExpiredSocket(LWPR::SOCKET_FD_T fd)
{
	DEBUG_FUNCTION();

	return LWPR::SOCKET_RET_OK;
}

void AppNSHandlerImpl::DoClientAmountUpdate(int amount, LWPR::SOCKET_FD_T fd, bool increase)
{
	DEBUG_FUNCTION();
}

void AppNSHandlerImpl::DoThreadStart()
{
	DEBUG_FUNCTION();

	int code = tpsvrthrinit(m_argc, m_argv);
	if(code < 0)
	{
		logger->error(LTRACE, "tpsvrthrinit error %d", code);
	}
}

void AppNSHandlerImpl::DoThreadEnd()
{
	DEBUG_FUNCTION();

	tpsvrthrdone();
}

Message* AppNSHandlerImpl::InvokeImpl(LWPR::SOCKET_FD_T fd, const TUX_HEADER_MESSAGE_T& req_header, LWPR::InputStream& req_body)
{
	DEBUG_FUNCTION();

	Message* pMsgRep = NULL;

	try
	{
		switch(req_header.nMsgType)
		{
		case TUX_MSG_TYPE_PING_PANG_REQ:
			{
				// 解析请求报文
				TUX_APP_PINGPANG_REQ_T msgReq;
				msgReq.Unmarshal(req_body);
				msgReq.MsgHeader(req_header);

				// 创建应答报文
				TUX_APP_PINGPANG_REP_T* pMsgRepCurrent = new TUX_APP_PINGPANG_REP_T();
				pMsgRep = pMsgRepCurrent;

				// 业务逻辑实现
				pMsgRepCurrent->MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_OK;
			}
			break;
		case TUX_MSG_TYPE_TPCALL_REQ:
			{
				// 解析请求报文
				TUX_APP_TPCALL_REQ_T msgReq;
				msgReq.Unmarshal(req_body);
				msgReq.MsgHeader(req_header);

				// 创建应答报文
				TUX_APP_TPCALL_REP_T* pMsgRepCurrent = new TUX_APP_TPCALL_REP_T();
				pMsgRep = pMsgRepCurrent;

				// 业务逻辑实现
				CallService(fd, msgReq, *pMsgRepCurrent);
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

	if(pMsgRep)
	{
		pMsgRep->MsgHeader().DisconnectFromServer(GetNetServer()->ClientTotal() > AppConfig::GetInstance()->TUXONE_APP_SERVER_MAXLONGCONNECTIONS());
	}

	return pMsgRep;
}


void AppNSHandlerImpl::InvokePost(Message& response)
{
	try
	{
		switch(response.MsgHeader().nMsgType)
		{
		case TUX_MSG_TYPE_PING_PANG_REP:
			break;
		case TUX_MSG_TYPE_TPCALL_REP:
			{
				TUX_APP_TPCALL_REP_T& rep = dynamic_cast <TUX_APP_TPCALL_REP_T&>(response);
				ATMIBufferManager::GetInstance()->FreeBuffer(rep.pRepBuf);
			}
			break;
		default:
			break;
		}
	}
	catch(...)
	{
	}
}

void AppNSHandlerImpl::CallService(LWPR::SOCKET_FD_T fd, TUX_APP_TPCALL_REQ_T& request, TUX_APP_TPCALL_REP_T& response)
{
	DEBUG_FUNCTION();

	// 查找Service对应的执行函数
	SVCFUNC_FP fp = ServiceRepository::GetInstance()->GetSvcFunction(request.strSvcName.c_str());
	if(fp == NULL)
	{
		logger->warn(LTRACE, "Service [%s] not found", request.strSvcName.c_str());
		response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_NOT_FOUND;
		return;
	}

	// 构造Service函数入参
	TPSVCINFO rqst;
	memset(&rqst, 0, sizeof(rqst));
	strncpy(rqst.name, request.strSvcName.c_str(), TUXONE_SERVICE_NAME_LENGTH);
	rqst.flags = request.nFlags;
	if(request.bHasReq)
	{
		if(TYPED_BUFFER_VIEW32 == request.nReqBufType)
		{
			LWPR::Buffer_var vBufResult = new LWPR::Buffer(1024);
			bool bRetResult = View32Manager::GetInstance()->AdjustStructData(request.strReqBufTypeSub.c_str(),
			                  request.vReqBuf->Inout(),
			                  request.tStructInfo,
			                  request.MsgHeader().IsBigEndian(),
			                  *vBufResult);

			if(!bRetResult)
			{
				logger->error(LTRACE, "AdjustStructData error [%s]", request.strReqBufTypeSub.c_str());
				response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;
				return;
			}

			request.vReqBuf = vBufResult;
		}
		else if(TYPED_BUFFER_FML32 == request.nReqBufType)
		{
			FMLHeaderToLocal(request.vReqBuf->Inout());
		}

		rqst.data = request.vReqBuf->Inout();
		rqst.len = request.vReqBuf->Size();
	}

	// 执行Service业务方法
	ExecuteServiceMethod(&rqst, fp, request, response);
}

void AppNSHandlerImpl::ExecuteServiceMethod(TPSVCINFO* rqst, SVCFUNC_FP fp, TUX_APP_TPCALL_REQ_T& request, TUX_APP_TPCALL_REP_T& response)
{
	DEBUG_FUNCTION();

	assert(rqst != NULL);
	assert(fp != NULL);

	response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;

	// 保留函数返回点
	int code = sigsetjmp(TLSLibtux::GetInstance()->GetJmpBuf(), 1);

	// 返回值为0表示从sigsetjmp返回，其他值表示从siglongjmp返回
	if(code == 0)
	{
		logger->debug(LTRACE, "Invoke user service function");
		// 执行用户service函数
		fp(rqst);
	}
	else
	{
		TPRETURN_INFO_T& info = TLSLibtux::GetInstance()->GetTpreturnInfo();
		response.nFlags = info.flags;
		response.pRepBuf = info.data;
		response.nRepLen = info.len;
		response.nRetCode = info.rcode;
		response.bHasRep = (response.pRepBuf != NULL);

		switch(info.rval)
		{
		case TPSUCCESS:
			{
				response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_OK;

				// 获取应答数据类型
				if(!ATMIBufferManager::GetInstance()->GetBufferTypes((char *)response.pRepBuf, response.nRepBufType, response.strRepBufTypeSub))
				{
					response.nRepBufType = TYPED_BUFFER_CARRAY;
				}

				if(response.bHasRep)
				{
					// 如果应答buf与请求buf是同一个buf
					if(request.vReqBuf->Inout() == response.pRepBuf)
					{
						// 仅仅为了持有一份引用
						response.vRepBuf = request.vReqBuf;

						// 应答Buffer的数据保持同请求数据一致
						response.nRepBufType = request.nReqBufType;
						response.strRepBufTypeSub = request.strReqBufTypeSub;
					}

					// 如果是VIEW32类型，需要附加结构体信息
					if(TYPED_BUFFER_VIEW32 == response.nRepBufType)
					{
						if(!View32Manager::GetInstance()->GetStructInfo(response.strRepBufTypeSub.c_str(), response.tStructInfo))
						{
							response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;
							return;
						}

						response.nRepLen = response.tStructInfo.structSize;
					}
					// 如果是FML32类型，需要调整应答数据长度及字节序
					else if(TYPED_BUFFER_FML32 == response.nRepBufType)
					{
						response.nRepLen = FMLValidSize((FBFR32 *)response.pRepBuf);
						FMLHeaderToNet(response.pRepBuf);
					}
					// 如果是STRING类型，需要调整应答数据长度
					else if(TYPED_BUFFER_STRING == response.nRepBufType)
					{
						response.nRepLen = strlen(response.pRepBuf);
					}
				}

				// 处理应答数据
				switch(response.nRepBufType)
				{
				case TYPED_BUFFER_CARRAY:
					break;
				case TYPED_BUFFER_STRING:
					break;
				case TYPED_BUFFER_MBSTRING:
					break;
				case TYPED_BUFFER_VIEW32:
					break;
				case TYPED_BUFFER_FML32:
					break;
				case TYPED_BUFFER_XML:
					break;
				default:
					response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;
					break;
				}
			}
			break;
		case TPFAIL:
			{
				response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_FAIL;
			}
			break;
		case TPEXIT:
			{
				response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;
				exit(0);
			}
			break;
		default:
			response.MsgHeader().nMsgRetCode = TUX_MSG_RET_CODE_SVC_ERR;
			break;
		}
	}
}
