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
#ifndef TUX_APP_DEPOT_H__
#define TUX_APP_DEPOT_H__
#include <lwpr.h>
#include <map>
#include "Message.h"

typedef struct app_item_t
{
	LWPR::UINT32		nAppType;
	time_t				nLastUpdateTime;
	LWPR::SOCKET_ADDR_T	tSocketAddr;

	app_item_t(): nAppType(TUX_APP_TYPE_UNKNOW), nLastUpdateTime(time(NULL))
	{
		memset(&tSocketAddr, 0, sizeof(tSocketAddr));
	}

} APP_ITEM_T;

typedef std::map< LWPR::SOCKET_FD_T, APP_ITEM_T > AppMap;

/**
 * 存储App信息，包括网络连接，App类型，App上次心跳时间
 */
class AppDepot
{
public:

	AppDepot();
	~AppDepot();
	static AppDepot* GetInstance();

	void RegisterApp(LWPR::SOCKET_FD_T fd, const APP_ITEM_T& item);

	void UnRegisterApp(LWPR::SOCKET_FD_T fd);

	void UnRegisterApp(LWPR::SOCKETFDList fds);

	bool UnRegisterApp(LWPR::SOCKET_FD_T fd, LWPR::SOCKET_ADDR_T& addr);

	void GetAllApp(AppMap& apps);

	void UpdateApp(LWPR::SOCKET_FD_T fd);

private:
	LWPR::Mutex	m_Mutex;
	AppMap		m_AppMap;
};

#endif // end of TUX_APP_DEPOT_H__
