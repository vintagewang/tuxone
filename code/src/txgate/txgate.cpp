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
#include <signal.h>
#include <lwpr.h>
#include "GateConfig.h"
#include "BulletinBoard.h"
#include "ShareCookie.h"

using std::cout;
using std::cerr;
using std::endl;

extern int wsh_main(int argc, char** argv);
extern int sync_main(int argc, char** argv);

static volatile bool g_bTerminated = false;
static volatile LWPR::PID_T g_nSyncPID = -1;

static void DoSignalHandle(int sig)
{
	logger->fatal(LTRACE, "txgate catched signal %d", sig);
	g_bTerminated = true;
}

static void CreateWSH(int argc, char** argv)
{
	DEBUG_FUNCTION();

	LWPR::PID_T pid = fork();
	// 子进程
	if(pid == 0)
	{
		exit(wsh_main(argc, argv));
	}
	// 父进程
	else if(pid > 0)
	{
		logger->info(LTRACE, "fork a wsh process [%d]", pid);
	}
	// 出错
	else if(pid == -1)
	{
		// TODO:
		logger->fatal(LTRACE, "fork error");
	}
}

static void CreateSYNC(int argc, char** argv)
{
	DEBUG_FUNCTION();

	LWPR::PID_T pid = fork();
	// 子进程
	if(pid == 0)
	{
		exit(sync_main(argc, argv));
	}
	// 父进程
	else if(pid > 0)
	{
		g_nSyncPID = pid;
		logger->info(LTRACE, "fork a sync process [%d]", pid);
	}
	// 出错
	else if(pid == -1)
	{
		g_nSyncPID = pid;
		logger->fatal(LTRACE, "fork error");
	}
}

static void DoHousekeeping(int argc, char** argv)
{
	DEBUG_FUNCTION();

	try
	{
		// 回收子进程
		LWPR::PIDList pids;
		while(1)
		{
			LWPR::PID_T pid = waitpid(-1, NULL, WNOHANG);
			if(pid > 0)
			{
				if(pid == g_nSyncPID)
				{
					CreateSYNC(argc, argv);
				}
				else
				{
					pids.push_back(pid);
				}
			}
			else
			{
				break;
			}
		}

		// 清除公告板记录
		BulletinBoard::GetInstance()->CleanupWSHByPID(pids);

		// 获取所有WSH进程信息进行校验
		BBWSHList wshs;
		int nWSHCount = BulletinBoard::GetInstance()->GetAllWSH(wshs);
		BBIndexList exitedwsh;
		for(int i = 0; i < nWSHCount; i++)
		{
			if(!LWPR::ProcessUtil::IsProcessExist(wshs[i].nPID))
			{
				exitedwsh.push_back(wshs[i].nIndex);
			}
		}

		// 清除公告板记录
		BulletinBoard::GetInstance()->CleanupWSHByIndex(exitedwsh);

		// 计算当前客户端连接数
		nWSHCount = BulletinBoard::GetInstance()->GetAllWSH(wshs);
		int nConnectionTotal = 0;
		for(int k = 0; k < nWSHCount; k++)
		{
			nConnectionTotal += wshs[k].nConnectionTotal;
		}

		// 计算当前系统支持的并发量
		int nWSHAccessMax = GateConfig::GetInstance()->TUXONE_GATE_WSH_ACCESSMAX() >= FD_SETSIZE ?
		                    GateConfig::GetInstance()->TUXONE_GATE_WSH_ACCESSMAX() - LWPR::NET_SERVER_RESERVED_FDS :
		                    GateConfig::GetInstance()->TUXONE_GATE_WSH_ACCESSMAX();
		int nConcurrentSupported = nWSHAccessMax * nWSHCount;

		// 低于低水位，需要创建WSH进程
		int nReservedConnect = nConcurrentSupported - nConnectionTotal;
		logger->debug(LTRACE, "reserved connection num %d", nReservedConnect);
		if((nReservedConnect <= GateConfig::GetInstance()->TUXONE_GATE_CONCURRENT_LOWWATER())
		   && nWSHCount < GateConfig::GetInstance()->TUXONE_GATE_WSH_MAX())
		{
			CreateWSH(argc, argv);
		}
		else if((nReservedConnect >= GateConfig::GetInstance()->TUXONE_GATE_CONCURRENT_HIGHWATER())
		        && nWSHCount >= GateConfig::GetInstance()->TUXONE_GATE_WSH_MIN())
		{
			for(int k = 0; k < nWSHCount; k++)
			{
				if(wshs[k].nConnectionTotal == 0)
				{
					// TODO: 杀死WSH进程
				}
			}
		}
	}
	catch(LWPR::Exception& e)
	{
		logger->error(LTRACE, "%s", e.what());
	}
	catch(std::exception& e)
	{
		logger->error(LTRACE, "%s", e.what());
	}
	catch(...)
	{
		logger->error(LTRACE, "Unknow exception");
	}
}

static void DoDaemon(int argc, char** argv)
{
	DEBUG_FUNCTION();

	// 创建WSH进程
	for(int i = 0; !g_bTerminated && i < GateConfig::GetInstance()->TUXONE_GATE_WSH_MIN(); i++)
	{
		CreateWSH(argc, argv);
	}

	// 创建SYNC进程
	CreateSYNC(argc, argv);

	while(!g_bTerminated)
	{
		fd_set fds;
		struct timeval tv = {0};
		FD_ZERO(&fds);

		FD_SET(ShareCookie::GetInstance()->FdUDPListening(), &fds);
		tv.tv_sec = GateConfig::GetInstance()->TUXONE_GATE_HOUSEKEEPING_INTERVAL();
		int ret = select(ShareCookie::GetInstance()->FdUDPListening() + 1, &fds, NULL, NULL, &tv);
		// 收到UDP消息
		if(ret > 0)
		{
			logger->debug(LTRACE, "select OK, receive UDP data");
			// 根据消息的类型具体工作
			DoHousekeeping(argc, argv);
		}
		// 超时
		else if(ret == 0)
		{
			logger->debug(LTRACE, "select timeout");
			DoHousekeeping(argc, argv);
		}
		// 发生错误（可能是信号中断）
		else if(ret == -1)
		{
			logger->error(LTRACE, "select error");
		}
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

	strName += "ULOG." + (LWPR::ProcessUtil::GetPName() + "." + LWPR::StringUtil::IntToStr(LWPR::ProcessUtil::GetPID()) + ".log");

	return strName;
}

#ifndef TUXONE_TRIAL_TIME
#define TUXONE_TRIAL_TIME 365
#endif

static int run(int argc, char** argv)
{
	// 校验License
	if(!LWPR::Utility::HasLicenseEffect(TUXONE_TRIAL_TIME))
	{
		fprintf(stderr, "Your License is expired, Please contact vintage.wang@gmail.com!\n");
		return -1;
	}

	// 信号处理
	LWPR::Utility::RegisterSignal(SIGINT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGTERM, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGHUP, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGQUIT, DoSignalHandle);
	LWPR::Utility::RegisterSignal(SIGCHLD, SIG_IGN);
	LWPR::Utility::RegisterSignal(SIGPIPE, SIG_IGN);

	// 初始化配置文件模块
	GateConfig::GetInstance()->ConfigInit(argc, argv);

	// 初始化日志文件模块
	LWPR::LOGGER_OPTION_T optLogger;
	optLogger.strLogFileName = GetLogFileName();
	optLogger.nLogLevel = GateConfig::GetInstance()->TUXONE_GATE_LOG_LEVEL();
	optLogger.bPrintWhere = GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTWHERE();
	optLogger.bPrintFile = GateConfig::GetInstance()->TUXONE_GATE_LOG_ENABLE() && GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTFILE();
	optLogger.bPrintConsole = GateConfig::GetInstance()->TUXONE_GATE_LOG_ENABLE() && GateConfig::GetInstance()->TUXONE_GATE_LOG_PRINTCONSOLE();
	logger->Initialize(optLogger);

	// 创建TCP监听端口号
	LWPR::SOCKET_FD_T nFdTCPListening = LWPR::Socket::CreateServerTCPSocket(GateConfig::GetInstance()->TUXONE_GATE_LISTENER_ADDR().c_str());
	if(-1 == nFdTCPListening)
	{
		logger->fatal(LTRACE, "create TCP socket error");
		// TODO: 抛出异常
		return -1;
	}

	// 创建UDP监听端口号
	LWPR::UINT16 nPort = 0;
	LWPR::SOCKET_FD_T nFdUDPListening = LWPR::Socket::CreateServerUDPSocket(nPort);
	if(-1 == nFdUDPListening)
	{
		logger->fatal(LTRACE, "create UDP socket error");
		return -1;
	}

	// 保存描述符至ShareCookie，供子进程使用
	ShareCookie::GetInstance()->FdTCPListening(nFdTCPListening);
	ShareCookie::GetInstance()->FdUDPListening(nFdUDPListening);
	ShareCookie::GetInstance()->UDPPort(nPort);

	// 创建公告板
	BulletinBoard::GetInstance()->Create(GateConfig::GetInstance()->TUXONE_GATE_WSH_MAX(),
	                                     GateConfig::GetInstance()->TUXONE_GATE_BBL_BERTHMAX());

	// 初始化公告板
	BB_HEADER_ENTITY_T header;
	header.nMagicCode = BB_HEADER_MAGIC_CODE;
	header.nSVCUpdateSeq = 0;
	header.nGatePID = LWPR::Utility::GetPid();
	header.nUDPPort = ShareCookie::GetInstance()->UDPPort();
	LWPR::Socket::AddrTransform(GateConfig::GetInstance()->TUXONE_GATE_LISTENER_ADDR().c_str(), header.tServerAddr);
	BulletinBoard::GetInstance()->SetHeader(header);

	// 打印版权信息
	printf("TUXONE Gate Version 2.0 Copyright vintage.wang@gmail.com <http://tuxone.googlecode.com>\n");
	GateConfig::GetInstance()->PrintConfig();

	// 进入主事件循环
	DoDaemon(argc, argv);

	// 程序退出，执行资源清理动作
	BBWSHList wshs;
	int nWSHCount = BulletinBoard::GetInstance()->GetAllWSH(wshs);
	for(int i = 0; i < nWSHCount; i++)
	{
		// TODO: 杀死所有WSH进程，要先通过UDP发送通知，然后等待指定时间如果进程还没有退出，则强制杀死
		if(wshs[i].nPID > 0)
		{
			kill(wshs[i].nPID, SIGKILL);
			logger->debug(LTRACE, "Send signal [SIGKILL] to wsh Process %d", wshs[i].nPID);
		}
	}

	// 杀死同步进程
	if(-1 != g_nSyncPID)
	{
		kill(g_nSyncPID, SIGKILL);
		logger->debug(LTRACE, "Send signal [SIGKILL] to sync Process %d", g_nSyncPID);
	}

	// 回收子进程
	while(waitpid(-1, NULL, WNOHANG) > 0);

	BulletinBoard::GetInstance()->Destroy();

	return 0;
}

int main(int argc, char** argv)
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
