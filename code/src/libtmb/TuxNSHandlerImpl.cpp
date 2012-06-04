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
#include "TuxNSHandlerImpl.h"


TuxNSHandlerImpl::TuxNSHandlerImpl()
{
	DEBUG_FUNCTION();
}

TuxNSHandlerImpl::~TuxNSHandlerImpl()
{
	DEBUG_FUNCTION();
}

LWPR::SOCKET_RET_TYPE_E TuxNSHandlerImpl::DoReceiveNormalData(LWPR::SOCKET_FD_T fd)
{
	DEBUG_FUNCTION();

	LWPR::SOCKET_RET_TYPE_E nRet = LWPR::SOCKET_RET_OK;

	try
	{
		// 读请求报文头
		LWPR::Buffer_var vBuf = new LWPR::Buffer(TUX_HEADER_MESSAGE_SIZE);
		LWPR::InputStream in(vBuf);
		nRet = LWPR::Socket::ReadSocket(fd, *vBuf, TUX_HEADER_MESSAGE_SIZE, SERVER_SOCKET_TIMEOUT);
		if(nRet == LWPR::SOCKET_RET_OK)
		{
			TUX_HEADER_MESSAGE_T header;
			header.Unmarshal(in);

			// 读请求报文体
			LWPR::InputStream inbody(vBuf);
			nRet = LWPR::Socket::ReadSocket(fd, *vBuf, header.nMsgLength - TUX_HEADER_MESSAGE_SIZE, SERVER_SOCKET_TIMEOUT);
			if(nRet != LWPR::SOCKET_RET_OK)
			{
				logger->error(LTRACE, "Read msg request body error, [%d]", (int)nRet);
				return nRet;
			}

			Message* pRepMsg = InvokeImpl(fd, header, inbody);
			if(pRepMsg != NULL)
			{
				LWPR::OutputStream outHeader;
				LWPR::OutputStream outBody;
				pRepMsg->Marshal(outBody);
				pRepMsg->MsgHeader().nMsgLength = TUX_HEADER_MESSAGE_SIZE + outBody.GetStreamLength();
				pRepMsg->MsgHeader().Marshal(outHeader);

				// 写应答报文头
				nRet = LWPR::Socket::WriteSocket(fd, outHeader.GetStream(), outHeader.GetStreamLength(), SERVER_SOCKET_TIMEOUT);
				if(LWPR::SOCKET_RET_OK == nRet)
				{
					// 写应答报文体
					nRet = LWPR::Socket::WriteSocket(fd, outBody.GetStream(), outBody.GetStreamLength(), SERVER_SOCKET_TIMEOUT);
				}

				InvokePost(*pRepMsg);

				delete pRepMsg;
			}
			else
			{
				nRet = LWPR::SOCKET_RET_FAILED;
			}
		}
		else
		{
			logger->debug(LTRACE, "Read msg request header error, maybe peer disconnected, [%d]", (int)nRet);
		}
	}
	catch(const LWPR::Exception& e)
	{
		logger->error(LTRACE, "%s", e.what());
		nRet = LWPR::SOCKET_RET_FAILED;
	}
	catch(...)
	{
		logger->error(LTRACE, "Unknow exception");
		nRet = LWPR::SOCKET_RET_FAILED;
	}

	return nRet;
}

void TuxNSHandlerImpl::InvokePost(Message& response)
{

}
