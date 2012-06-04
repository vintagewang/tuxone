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
#ifndef TUX_APP_NSHANDLER_IMPL_H__
#define TUX_APP_NSHANDLER_IMPL_H__
#include <lwpr.h>
#include <atmi.h>
#include "Message.h"
#include "TuxNSHandlerImpl.h"

class AppNSHandlerImpl : public TuxNSHandlerImpl
{
public:

	AppNSHandlerImpl();
	~AppNSHandlerImpl();

	/**
	 * 关闭超时客户端之前
	 */
	virtual LWPR::SOCKET_RET_TYPE_E DoCloseExpiredSocket(LWPR::SOCKET_FD_T fd);

	/**
	 * 客户端数目有变化情况执行
	 */
	virtual void DoClientAmountUpdate(int amount, LWPR::SOCKET_FD_T fd, bool increase);

	virtual void DoThreadStart();

	virtual void DoThreadEnd();

	/**
	 * 各种请求处理
	 */
	virtual Message* InvokeImpl(LWPR::SOCKET_FD_T fd,
	                            const TUX_HEADER_MESSAGE_T& req_header,
	                            LWPR::InputStream& req_body);

	virtual void InvokePost(Message& response);

private:
	void CallService(LWPR::SOCKET_FD_T fd, TUX_APP_TPCALL_REQ_T& request, TUX_APP_TPCALL_REP_T& response);

	/**
	 * 注意：此函数中要调用函数本地跳转方法，不能有C++的对象
	 * 返回值表示应答代码
	 */
	void ExecuteServiceMethod(TPSVCINFO* rqst, SVCFUNC_FP fp, TUX_APP_TPCALL_REQ_T& request, TUX_APP_TPCALL_REP_T& response);

private:
	char **m_argv;
	int m_argc;
};

#endif // end of TUX_APP_NSHANDLER_IMPL_H__
