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
#include "NotifyConfig.h"

//----------------------------------------------------------------------------------
// struct QueueConfig
//----------------------------------------------------------------------------------

QueueConfig::QueueConfig():
	id(1),
	maxConsumers(0),
	maxSuppliers(0),
	maxEvents(0),
	reliability(false),
	retry(true),
	retryInterval(1),
	maxRetries(10),
	deliverMode(AppConfig::INTURN),
	deliverTimeout(30),
	requestAccelerate(true)
{
}

//----------------------------------------------------------------------------------
// class NotifyConfig
//----------------------------------------------------------------------------------
NotifyConfig::NotifyConfig()
{
	QueueConfig qc;
	m_QueueConfigTable[TUXONE_NOTIFY_DEFAULTQUEUENAME()] = qc;

	// TODO: π©≤‚ ‘ π”√
	m_QueueConfigTable["Q1"] = qc;
	m_QueueConfigTable["Q2"] = qc;
}

NotifyConfig::~NotifyConfig()
{
}

NotifyConfig* NotifyConfig::GetInstance()
{
	static NotifyConfig *singleton = new NotifyConfig();
	return singleton;
}

void NotifyConfig::DoPropConstruct()
{
	AppConfig::DoPropConstruct();
}

const NotifyConfig::QueueConfigTable& NotifyConfig::GetQueueConfigTable()
{
	return m_QueueConfigTable;
}
