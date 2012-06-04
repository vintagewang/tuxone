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
#ifndef TX_TUX_NSHANDLER_IMPL_H__
#define TX_TUX_NSHANDLER_IMPL_H__
#include <lwpr.h>
#include "Message.h"

class TuxNSHandlerImpl : public LWPR::NetServerHandler
{
public:
	TuxNSHandlerImpl();
	virtual ~TuxNSHandlerImpl();

	/**
	 * 收到客户端正常数据
	 */
	virtual LWPR::SOCKET_RET_TYPE_E DoReceiveNormalData(LWPR::SOCKET_FD_T fd);

	/**
	 * 各种请求处理
	 */
	virtual Message* InvokeImpl(LWPR::SOCKET_FD_T fd,
	                            const TUX_HEADER_MESSAGE_T& req_header,
	                            LWPR::InputStream& req_body) = 0;

	/**
	 * InvokeImpl执行成功后执行
	 */
	virtual void InvokePost(Message& response);

protected:
	static const int SERVER_SOCKET_TIMEOUT = 60;
};

#endif // end of TX_TUX_NSHANDLER_IMPL_H__
