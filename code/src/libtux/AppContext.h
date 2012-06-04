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
#ifndef TUX_APP_CONTEXT_H__
#define TUX_APP_CONTEXT_H__
#include <atmi.h>
#include <string>

namespace LWPR
{
	class NetServer;
}

class AppContext
{
public:
	AppContext();
	~AppContext();

	int tpinit(TPINIT *tpinfo);
	int tpterm();
	int tpcall(char *svc, char *idata, long ilen, char **odata, long *olen, long flags);

	/**
	 * 此函数非线程安全
	 */
	int tpregister(const TMSVCFUNC* svcs, long len, long flags);
	int tprun(long flags);

	/**
	 * 注册注销Service
	 */
	int tpadvertise(char *svcname, void (*func)(TPSVCINFO *));
	int tpunadvertise(char *svcname);
private:
	std::string GetLogFileName();

private:
	volatile bool			m_bFirstInit;
	volatile bool			m_bInited;

	volatile bool			m_bFirstRegister;
	volatile bool			m_bFirstCall;

	LWPR::NetServer*		m_pNetServer;
private:

};

#endif // end of TUX_APP_CONTEXT_H__
