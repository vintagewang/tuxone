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
#include <lwpr.h>
#include "NotifyConfig.h"

using std::cout;
using std::cerr;
using std::endl;

int run(int argc, char** argv)
{
	// 初始化配置文件模块
	NotifyConfig* pNotifyConfig = NotifyConfig::GetInstance();
	pNotifyConfig->ConfigInit(argc, argv);

	// 初始化日志文件模块
	LWPR::LOGGER_OPTION_T optLogger;
	optLogger.strLogFileName = "txnotify.log";
	optLogger.nLogLevel = pNotifyConfig->TUXONE_NOTIFY_LOG_LEVEL();
	optLogger.bPrintWhere = true;
	optLogger.bPrintFile = pNotifyConfig->TUXONE_NOTIFY_LOG_ENABLE() && pNotifyConfig->TUXONE_NOTIFY_LOG_PRINTFILE();
	optLogger.bPrintConsole = pNotifyConfig->TUXONE_NOTIFY_LOG_ENABLE() && pNotifyConfig->TUXONE_NOTIFY_LOG_PRINTCONSOLE();
	logger->Initialize(optLogger);

	// 打印版权信息
	cout << "TUXONE txnotify version 0.0.1 Copyright vintage.wang@gmail.com <http://tuxone.googlecode.com>" << endl;
	pNotifyConfig->PrintConfig();

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
