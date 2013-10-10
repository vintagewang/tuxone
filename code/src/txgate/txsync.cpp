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
/*
 * 功能： 单进程程序，监视其他gate公告板变动情况，
 *       负责获取其他gate公告板数据，并同步本gate公告板，令当前域的所有gate公告板数据是相同的
 *       起灾备作用
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwpr.h>
#include "GateManager.h"
#include "ShareCookie.h"
#include "BulletinBoard.h"
#include "GateConfig.h"

using std::cout;
using std::cerr;
using std::endl;

static void DoSignalHandle(int sig)
{
	logger->fatal(LTRACE, "txsync catched signal %d", sig);
	GateManager::GetInstance()->Stop();
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

	strName += ("ULOG.txsync." + LWPR::StringUtil::IntToStr(LWPR::ProcessUtil::GetPID()) + ".log");

	return strName;
}

static int run(int argc, char** argv)
{
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

	// 开始同步工作
	GateManager::GetInstance()->Run();

	return 0;
}

extern int sync_main(int argc, char** argv)
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
