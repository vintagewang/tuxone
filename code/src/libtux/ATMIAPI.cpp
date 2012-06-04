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
#include <stdio.h>
#include <string.h>
#include <lwpr.h>
#include <atmi.h>
#include <userlog.h>
#include "TLSLibtux.h"
#include "AppContext.h"
#include "ATMIBufferManager.h"

// 全局对象， tuxone运行时对象
static AppContext* g_pAppContext = NULL;


/**
 * 本文件中函数为对用户提供的C样式函数，不允许有异常抛出
 */

extern "C" int* LIBTUX_EXPORT tx_get_tperrno_addr(void)
{
	return TLSLibtux::GetInstance()->GetTperrno();
}

extern "C" char* LIBTUX_EXPORT tpalloc(char *type, char *subtype, long size)
{
	try
	{
		return ATMIBufferManager::GetInstance()->AllocateBuffer(type, subtype, size);
	}
	catch(...)
	{
	}

	tperrno = TPEOS;
	return NULL;
}

extern "C" char* LIBTUX_EXPORT tprealloc(char *ptr, long size)
{
	try
	{
		return ATMIBufferManager::GetInstance()->ReallocateBuffer(ptr, size);
	}
	catch(...)
	{
	}

	tperrno = TPEOS;
	return NULL;
}

extern "C" long LIBTUX_EXPORT tptypes(char *ptr, char *type, char *subtype)
{
	try
	{
		return ATMIBufferManager::GetInstance()->GetBufferTypes(ptr, type, subtype);
	}
	catch(...)
	{
	}

	tperrno = TPEOS;
	return -1;
}

extern "C" void LIBTUX_EXPORT tpfree(char *ptr)
{
	try
	{
		ATMIBufferManager::GetInstance()->FreeBuffer(ptr);
	}
	catch(...)
	{
	}
}

extern "C" int LIBTUX_EXPORT tpinit(TPINIT *tpinfo)
{
	try
	{
		if(!g_pAppContext)
		{
			g_pAppContext = new AppContext();

			return g_pAppContext->tpinit(tpinfo);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpterm()
{
	try
	{
		if(g_pAppContext)
		{
			int ret = g_pAppContext->tpterm();
			delete g_pAppContext;
			g_pAppContext = NULL;
			return ret;
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpcall(char *svc, char *idata, long ilen, char **odata, long *olen, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			return g_pAppContext->tpcall(svc, idata, ilen, odata, olen, flags);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpacall(char *svc, char *data, long len, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpgetrply(int *cd, char **data, long *len, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" void LIBTUX_EXPORT tpcancel(int cd)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
	}
}

extern "C" int LIBTUX_EXPORT tpconnect(char *name, char *data, long len, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpdiscon(int cd)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpsend(int cd, char *data, long len, long flags, long *revent)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tprecv(int cd, char **data, long *len, long flags, long *revent)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpenqueue(char *qspace, char *qname, TPQCTL *ctl, char *data, long len, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpdequeue(char *qspace, char *qname, TPQCTL *ctl, char **data, long *len, long flags)
{
	try
	{
		if(g_pAppContext)
		{
			// TODO
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpsvrinit(int argc, char **argv)
{
	userlog("Default tpsvrinit() function used");
	return 0;
}

extern "C" void LIBTUX_EXPORT tpsvrdone()
{
	userlog("Default tpsvrdone() function used");
}

extern "C" void LIBTUX_EXPORT tpreturn(int rval, int rcode, char *data, long len, long flags)
{
	// return info
	TPRETURN_INFO_T& info = TLSLibtux::GetInstance()->GetTpreturnInfo();
	info.rval = rval;
	info.rcode = rcode;
	info.data = data;
	info.len = len;
	info.flags = flags;

	// 本地函数跳转
	siglongjmp(TLSLibtux::GetInstance()->GetJmpBuf(), 1);
}

extern "C" void LIBTUX_EXPORT tpforward(char *svc, char *data, long len, long flags)
{
	try
	{
		// TODO:
	}
	catch(...)
	{
	}
}

extern "C" int LIBTUX_EXPORT tpsvrthrinit(int argc, char **argv)
{
	userlog("Default tpsvrthrinit() function used");
	return 0;
}

extern "C" void LIBTUX_EXPORT tpsvrthrdone(void)
{
	userlog("Default tpsvrthrdone() function used");
}

#if 0
extern "C" int LIBTUX_EXPORT userlog(char* fmt, ...)
{
	char buf[512] = {0};
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf) - 2, fmt, args);
	va_end(args);

	std::string ULOG = "ULOG.";
	ULOG += LWPR::Utility::DateNow().substr(2);

	FILE *fp = fopen(ULOG.c_str(), "a");
	if(NULL == fp)
	{
		return -1;
	}

	strcat(buf, "\n");
	fprintf(fp, "%s, %s", LWPR::Utility::DateTimeNow().c_str(), buf);

	fclose(fp);

	return 0;
}
#else
extern "C" int LIBTUX_EXPORT userlog(char* fmt, ...)
{
	char buf[512] = {0};
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, args);
	va_end(args);

	logger->info(LTRACE, "%s", buf);

	return 0;
}
#endif


extern "C" char* LIBTUX_EXPORT tpstrerror(int err)
{
	switch(err)
	{
	case TPEABORT:
		return "TPEABORT - transaction cannot commit";
	case TPEBADDESC:
		return "TPEBADDESC - bad communication descriptor";
	case TPEBLOCK:
		return "TPEBLOCK - blocking condition found";
	case TPEINVAL:
		return "TPEINVAL - invalid arguments given";
	case TPELIMIT:
		return "TPELIMIT - a system limit has been reached";
	case TPENOENT:
		return "TPENOENT - no entry found";
	case TPEOS:
		return "TPEOS - operating system error";
	case TPEPERM:
		return "TPEPERM - bad permissions";
	case TPEPROTO:
		return "TPEPROTO - protocol error";
	case TPESVCERR:
		return "TPESVCERR - server error while handling request";
	case TPESVCFAIL:
		return "TPESVCFAIL - application level service failure";
	case TPESYSTEM:
		return "TPESYSTEM - internal system error";
	case TPETIME:
		return "TPETIME - timeout occured";
	case TPETRAN:
		return "TPETRAN - error starting transaction";
	case TPGOTSIG:
		return "TPGOTSIG - signal received and TPSIGRSTRT not specified";
	case TPERMERR:
		return "TPERMERR - resource manager error";
	case TPEITYPE:
		return "TPEITYPE - type and/or subtype do not match service's";
	case TPEOTYPE:
		return "TPEOTYPE - type and/or subtype do not match buffer's or unknown";
	case TPERELEASE:
		return "TPERELEASE - invalid release";
	case TPEHAZARD:
		return "TPEHAZARD - hazard exists that transaction heuristically completed";
	case TPEHEURISTIC:
		return "TPEHEURISTIC - transaction heuristically completed";
	case TPEEVENT:
		return "TPEEVENT - event occurred";
	case TPEMATCH:
		return "TPEMATCH - service name cannot be advertised due to matching conflict";
	case TPEDIAGNOSTIC:
		return "TPEDIAGNOSTIC - function failed - check diagnostic value";
	case TPEMIB:
		return "TPEMIB - Management Information Base access error";
	default:
		return "";
	}
}

extern "C" int LIBTUX_EXPORT tperrordetail(long flags)
{
	// TODO:
	return -1;
}

extern "C" char* LIBTUX_EXPORT tpstrerrordetail(int err, long flags)
{
	// TODO:
	return "";
}

extern "C" int LIBTUX_EXPORT _tmstartserver(int argc, char **argv, TMSVRARGS_T* tmsvrargs)
{
	int nRet = 0;

	// 初始化Tuxedo系统
	TPINIT tpinfo;
	memset(&tpinfo, 0, sizeof(tpinfo));

	// 试图创建AppContext对象
	if(!g_pAppContext)
	{
		g_pAppContext = new AppContext();
	}

	// 初始化Tuxedo系统
	nRet = g_pAppContext->tpinit(&tpinfo);
	if(nRet != 0)
	{
		logger->error(LTRACE, "tpinit error");
		return -1;
	}

	// 作为Server，注册服务
	nRet = g_pAppContext->tpregister(tmsvrargs->tmdsptchtbl, tmsvrargs->tmdsptchtbllen, 0);
	if(nRet != 0)
	{
		logger->error(LTRACE, "Register service error");
		return -1;
	}

	// 初始化用户应用
	nRet = tpsvrinit(argc, argv);
	if(nRet != 0)
	{
		userlog("tpsvrinit failed");
		return -1;
	}

	// 运行Server
	g_pAppContext->tprun(0);

	// 结束Tuxedo系统
	g_pAppContext->tpterm();

	// 结束用户应用
	tpsvrdone();

	// 销毁Tuxedo系统
	delete g_pAppContext;

	return 0;
}

extern "C" int LIBTUX_EXPORT tpregister(const TMSVCFUNC* svcs, long len, long flags)
{
	try
	{
		if(!g_pAppContext)
		{
			return g_pAppContext->tpregister(svcs, len, 0);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tprun(long flags)
{
	try
	{
		if(!g_pAppContext)
		{
			return g_pAppContext->tprun(0);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpadvertise(char *svcname, void (*func)(TPSVCINFO *))
{
	try
	{
		if(!g_pAppContext)
		{
			return g_pAppContext->tpadvertise(svcname, func);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern "C" int LIBTUX_EXPORT tpunadvertise(char *svcname)
{
	try
	{
		if(!g_pAppContext)
		{
			return g_pAppContext->tpunadvertise(svcname);
		}
	}
	catch(...)
	{
		tperrno = TPESYSTEM;
		return -1;
	}

	tperrno = TPEPROTO;
	return -1;
}

extern int   LIBTUX_EXPORT tuxputenv(char *string)
{

	try
	{

	}
	catch(...)
	{
		return -1;
	}

	return 0;
}

extern char* LIBTUX_EXPORT tuxgetenv(char *name)
{
	try
	{

	}
	catch(...)
	{
	}

	return NULL;
}
