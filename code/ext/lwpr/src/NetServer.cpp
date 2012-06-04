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
#include "NetServer.h"
#include "STNetServer.h"
#include "MTNetServer.h"

namespace LWPR
{
	//----------------------------------------------------------------------------------
	// class NetServerHandler
	//----------------------------------------------------------------------------------
	SOCKET_RET_TYPE_E NetServerHandler::DoReceiveNormalData(SOCKET_FD_T fd)
	{
		return SOCKET_RET_OK;
	}

	SOCKET_RET_TYPE_E NetServerHandler::DoCloseExpiredSocket(SOCKET_FD_T fd)
	{
		return SOCKET_RET_OK;
	}

	void NetServerHandler::DoClientAmountUpdate(int amount, SOCKET_FD_T fd, bool increase)
	{

	}

	void NetServerHandler::DoSelectTimeout()
	{

	}

	void NetServerHandler::DoThreadStart()
	{

	}

	void NetServerHandler::DoThreadEnd()
	{

	}

	//----------------------------------------------------------------------------------
	// struct net_server_option
	//----------------------------------------------------------------------------------
	net_server_option::net_server_option() :
		nFdListen(-1),
		bDropListenFD(false),
		nThreadPoolMin(5),
		nThreadPoolMax(300),
		nThreadPoolMaxIdle(100),
		nConnectionMax(512),
		nSelectTimeout(0),
		nSocketExpiredTime(60 * 20),
		pHandler(NULL)
	{
	}

	//----------------------------------------------------------------------------------
	// class NetServer
	//----------------------------------------------------------------------------------
	NetServer::NetServer()
	{
	}

	NetServer::~NetServer()
	{
	}

	NetServer* NetServer::CreateNetServer(NET_SERVER_TYPE_E type, const NET_SERVER_OPTION_T& opt)
	{
		NetServer* pNetServer = NULL;

		switch(type)
		{
		case NET_SERVER_ST:
			pNetServer = new STNetServer(opt);
			break;
		case NET_SERVER_MT:
			pNetServer = new MTNetServer(opt);
			break;
		default:
			assert(0);
		}

		return pNetServer;
	}
}
