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
#ifndef TX_APP_CONFIG_H__
#define TX_APP_CONFIG_H__

#include "AutoConfig.h"
#include <map>
#include <string>

class AppConfig :  public AutoConfig
{
public:

	AppConfig();
	~AppConfig();
	static AppConfig* GetInstance();

private:
	virtual void DoPropConstruct();
	virtual std::string GetPreConfigFile();
};

#endif // end of TX_APP_CONFIG_H__
