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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <lwpr.h>
#include <lwpr/ProcessUtil.h>
#include "GateConfig.h"
#include "ShareCookie.h"
#include "BulletinBoard.h"
#include "GateNSHandlerImpl.h"
#include "WaitNotifyThread.h"
#include "HousekeepingThread.h"


using std::cout;
using std::cerr;
using std::endl;

static volatile bool g_bTerminated = false;
static LWPR::NetServer* g_pNetServer = NULL;

static void DoSignalHandle(int sig)
{
	logger->fatal(LTRACE, "txwsh catched signal %d", sig);
	g_bTerminated = true;
	if(g_pNetServer != NULL)
	{
		g_pNetServer->Shutdown();
	}
}

static std::string GetLogFileName()
{
	std::string strName;

	const char* pEnv = getenv("TUXLOGDIR");
	if(pEnv && strlen(pEnv) > 0)
	{
		strName = pEnv;
		strName += "/";
		LWPR::FileUtil::BuildPath(strName.c_str());
	}

	strName += ("ULOG.txwsh." + LWPR::StringUtil::IntToStr(LWPR::ProcessUtil::GetPID()) + ".log");

	return strName;
}

static int run(int argc, char** argv)
{
	// 清除从父进程继承的无用描述符
	LWPR::SOCKET_FD_T fdUdp = ShareCookie::GetInstance()->FdUDPListening();
	LWPR::Socket::CloseSocket(fdUdp);

	// 信号处理
	LWPR::Utility::RegisterSignal(SIGINT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGTERM, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGHUP, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGQUIT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGCHLD, SIG_IGN);
	LWPR::Utility::RegisterSignal(SIGPIPE, SIG_IGN);

	// 初始化日志文件模块
	LWPR::LOGGER_OPTION_T optLogger;
	optLogger.strLogFileName = GetLogFileName();
	optLogger.nLogLevel = GateConfig::GetInstance()->TUXONE_GATE_LOG_LEVEL();
	optLogger.bPrintWhere = GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTWHERE();
	optLogger.bPrintFile = GateConfig::GetInstance()->TUXONE_GATE_LOG_ENABLE() && GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTFILE();
	optLogger.bPrintConsole = GateConfig::GetInstance()->TUXONE_GATE_LOG_ENABLE() && GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTCONSOLE();
	logger->Initialize(optLogger);

	// 创建UDP监听端口号
	LWPR::UINT16 nPort = 0;
	LWPR::SOCKET_FD_T nFdUDPListening = LWPR::Socket::CreateServerUDPSocket(nPort);
	if(-1 == nFdUDPListening)
	{
		logger->fatal(LTRACE, "create UDP socket error");
		// TODO: 抛出异常
		return -1;
	}

	// 向公告板注册
	BB_WSH_ENTITY_T wsh;
	wsh.nPID = LWPR::ProcessUtil::GetPID();
	wsh.nPort = nPort;
	if(!BulletinBoard::GetInstance()->RegisterWSH(wsh))
	{
		logger->error(LTRACE, "BB WSH table space is not enough");
		BulletinBoard::GetInstance()->Disconnect();
		return -1;
	}

	// 创建NetServer对象
	LWPR::NET_SERVER_OPTION_T opt;
	opt.nFdListen = ShareCookie::GetInstance()->FdTCPListening();
	opt.bDropListenFD = true;
	opt.nThreadPoolMin = GateConfig::GetInstance()->TUXONE_GATE_WSH_THREADPOOL_THREADMIN();
	opt.nThreadPoolMax = GateConfig::GetInstance()->TUXONE_GATE_WSH_THREADPOOL_THREADMAX();
	opt.nThreadPoolMaxIdle = GateConfig::GetInstance()->TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE();
	opt.nConnectionMax = GateConfig::GetInstance()->TUXONE_GATE_WSH_ACCESSMAX();
	opt.nSelectTimeout = 0;				// 默认为0，表示网络模型不负责主动关闭过期Socket
	opt.nSocketExpiredTime = 120;		// 如果opt.nSelectTimeout == 0，此选项不起作用
	opt.pHandler = new GateNSHandlerImpl();
	g_pNetServer = LWPR::NetServer::CreateNetServer(LWPR::NET_SERVER_MT, opt);

	// 启动服务
	HousekeepingThread::GetInstance()->SetNetServer(g_pNetServer);
	HousekeepingThread::GetInstance()->SetUDPFd(nFdUDPListening);
	WaitNotifyThread::GetInstance()->SetUDPFd(nFdUDPListening);

	HousekeepingThread::GetInstance()->Start();
	WaitNotifyThread::GetInstance()->Start();
	g_pNetServer->ActivateService();
	g_pNetServer->Go();
	HousekeepingThread::GetInstance()->StopRunning();
	WaitNotifyThread::GetInstance()->StopRunning();

	return 0;
}

extern int wsh_main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(LWPR::Exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(std::exception& e)
	{
		cerr << e.what() << endl;
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
