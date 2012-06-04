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
#include "AppConfig.h"

AppConfig::AppConfig()
{
	m_TUXONE_NOTIFY_ADDR = "0.0.0.0:26000";
	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN = 5;
	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX = 300;
	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE = 100;
	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN = 5;
	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX = 300;
	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE = 100;
	m_TUXONE_NOTIFY_LOG_ENABLE = false;
	m_TUXONE_NOTIFY_LOG_LEVEL = 0;
	m_TUXONE_NOTIFY_LOG_PRINTFILE = false;
	m_TUXONE_NOTIFY_LOG_PRINTCONSOLE = true;
	m_TUXONE_NOTIFY_DBDIR = ".";
	m_TUXONE_NOTIFY_DEFAULTQUEUENAME = "defaultQueue";

}

AppConfig::~AppConfig()
{
}

AppConfig* AppConfig::GetInstance()
{
	static AppConfig *singleton = new AppConfig();
	return singleton;
}

void AppConfig::PrintConfig()
{
	const char* tmp = "";

	printf("tuxone.notify.addr=%s\n", m_TUXONE_NOTIFY_ADDR.c_str());

	printf("tuxone.notify.receiver.threadPool.threadMin=%d\n", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN);

	printf("tuxone.notify.receiver.threadPool.threadMax=%d\n", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX);

	printf("tuxone.notify.receiver.threadPool.threadMaxIdle=%d\n", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE);

	printf("tuxone.notify.sender.threadPool.threadMin=%d\n", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN);

	printf("tuxone.notify.sender.threadPool.threadMax=%d\n", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX);

	printf("tuxone.notify.sender.threadPool.threadMaxIdle=%d\n", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE);

	printf("tuxone.notify.log.enable=%s\n", m_TUXONE_NOTIFY_LOG_ENABLE ? "true" : "false");

	printf("tuxone.notify.log.level=%d\n", m_TUXONE_NOTIFY_LOG_LEVEL);

	printf("tuxone.notify.log.printFile=%s\n", m_TUXONE_NOTIFY_LOG_PRINTFILE ? "true" : "false");

	printf("tuxone.notify.log.printConsole=%s\n", m_TUXONE_NOTIFY_LOG_PRINTCONSOLE ? "true" : "false");

	printf("tuxone.notify.dbdir=%s\n", m_TUXONE_NOTIFY_DBDIR.c_str());

	printf("tuxone.notify.defaultQueueName=%s\n", m_TUXONE_NOTIFY_DEFAULTQUEUENAME.c_str());


}

void AppConfig::DoPropConstruct()
{
	std::string tmp;

	m_TUXONE_NOTIFY_ADDR = GetPropertyValue("tuxone.notify.addr", m_TUXONE_NOTIFY_ADDR);

	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN = GetPropertyValue("tuxone.notify.receiver.threadPool.threadMin", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN);

	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX = GetPropertyValue("tuxone.notify.receiver.threadPool.threadMax", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX);

	m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE = GetPropertyValue("tuxone.notify.receiver.threadPool.threadMaxIdle", m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE);

	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN = GetPropertyValue("tuxone.notify.sender.threadPool.threadMin", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN);

	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX = GetPropertyValue("tuxone.notify.sender.threadPool.threadMax", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX);

	m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE = GetPropertyValue("tuxone.notify.sender.threadPool.threadMaxIdle", m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE);

	m_TUXONE_NOTIFY_LOG_ENABLE = GetPropertyValue("tuxone.notify.log.enable", m_TUXONE_NOTIFY_LOG_ENABLE);

	m_TUXONE_NOTIFY_LOG_LEVEL = GetPropertyValue("tuxone.notify.log.level", m_TUXONE_NOTIFY_LOG_LEVEL);

	m_TUXONE_NOTIFY_LOG_PRINTFILE = GetPropertyValue("tuxone.notify.log.printFile", m_TUXONE_NOTIFY_LOG_PRINTFILE);

	m_TUXONE_NOTIFY_LOG_PRINTCONSOLE = GetPropertyValue("tuxone.notify.log.printConsole", m_TUXONE_NOTIFY_LOG_PRINTCONSOLE);

	m_TUXONE_NOTIFY_DBDIR = GetPropertyValue("tuxone.notify.dbdir", m_TUXONE_NOTIFY_DBDIR);

	m_TUXONE_NOTIFY_DEFAULTQUEUENAME = GetPropertyValue("tuxone.notify.defaultQueueName", m_TUXONE_NOTIFY_DEFAULTQUEUENAME);


}
