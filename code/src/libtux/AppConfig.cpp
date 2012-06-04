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

//----------------------------------------------------------------------------------
// class AppConfig
//----------------------------------------------------------------------------------
AppConfig::AppConfig()
{

}

AppConfig::~AppConfig()
{
}

AppConfig* AppConfig::GetInstance()
{
	static AppConfig *singleton = new AppConfig();
	return singleton;
}

void AppConfig::DoPropConstruct()
{
	AutoConfig::DoPropConstruct();

	if (TUXONE_APP_BUFFER_CACHEDSIZE() == 0)
	{
		TUXONE_APP_BUFFER_CACHEDSIZE(INT_MAX);
	}
}

std::string AppConfig::GetPreConfigFile()
{
	return "TUXCONFIG";
}
