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
#ifndef TUX_GATE_NSHANDLER_IMPL_H__
#define TUX_GATE_NSHANDLER_IMPL_H__
#include <lwpr.h>
#include "TuxNSHandlerImpl.h"

class GateNSHandlerImpl : public TuxNSHandlerImpl
{
public:
	GateNSHandlerImpl();
	~GateNSHandlerImpl();

	/**
	 * 关闭超时客户端之前
	 */
	virtual LWPR::SOCKET_RET_TYPE_E DoCloseExpiredSocket(LWPR::SOCKET_FD_T fd);

	/**
	 * 客户端数目有变化情况执行
	 */
	virtual void DoClientAmountUpdate(int amount, LWPR::SOCKET_FD_T fd, bool increase);

	/**
	 * 各种请求处理
	 */
	virtual Message* InvokeImpl(LWPR::SOCKET_FD_T fd, const TUX_HEADER_MESSAGE_T& req_header, LWPR::InputStream& req_body);

private:
	void FetchBB(LWPR::SOCKET_FD_T fd, TUX_FETCH_BB_REQ_T& request, TUX_FETCH_BB_REP_T& response);

	void RegisterApp(LWPR::SOCKET_FD_T fd, TUX_APP_REGISTER_REQ_T& request, TUX_APP_REGISTER_REP_T& response);

	void SubscibeService(LWPR::SOCKET_FD_T fd, TUX_APP_SUBSCIBE_REQ_T& request, TUX_APP_SUBSCIBE_REP_T& response);

private:
};

#endif // end of TUX_GATE_NSHANDLER_IMPL_H__
