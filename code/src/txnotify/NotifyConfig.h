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
#ifndef TX_NOTIFY_CONFIG_H__
#define TX_NOTIFY_CONFIG_H__

#include "AppConfig.h"
#include <map>
#include <string>

typedef struct QueueConfig
{
	int id;
	int maxConsumers;
	int maxSuppliers;
	int maxEvents;
	bool reliability;
	bool retry;
	int retryInterval;
	int maxRetries;
	AppConfig::DELIVER_MODE_E deliverMode;
	int deliverTimeout;
	bool requestAccelerate;

	QueueConfig();
} QueueConfig;

class NotifyConfig :  public AppConfig
{
public:
	typedef std::map<std::string, QueueConfig> QueueConfigTable;

public:

	NotifyConfig();
	~NotifyConfig();
	static NotifyConfig* GetInstance();

	/**
	 * 获取所有队列配置
	 */
	const QueueConfigTable& GetQueueConfigTable();

private:
	virtual void DoPropConstruct();

private:
	QueueConfigTable m_QueueConfigTable;
};

#endif // end of TX_NOTIFY_CONFIG_H__
