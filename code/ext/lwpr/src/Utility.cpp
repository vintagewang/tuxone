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
#include "Utility.h"
#include "Buffer.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <cerrno>
#include <time.h>
#include <cmath>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <limits.h>

namespace LWPR
{
	class LWPRInit
	{
	public:
		LWPRInit()
		{
			srand(time(NULL));
			Utility::RegisterSignal(SIGALRM, Utility::DefaultSignalHandler);
		}

		~LWPRInit()
		{
		}
	};

	static LWPRInit __g_LWPRInit;


	PID_T Utility::_nPID = getpid();

	TIME_PRECISION_T Utility::_nBootTime = GetTimePrecision();

	PID_T Utility::GetPid()
	{
		return _nPID;
	}

	std::string Utility::ExceptionInfo(const char *file,
	                                   int line,
	                                   const char *fun,
	                                   int errnum,
	                                   const char *excptname,
	                                   const char *dsc)
	{
		char buf[512] = {0};
#if defined(WIN32)
		const char *f = strrchr(file, '\\');
#else
		const char *f = strrchr(file, '/');
#endif

		sprintf(buf, "%s:%s:#%d [%s(%d)] [%s] %s",
		        ((f == NULL) ? file : f + 1), fun, line, ErrnoName(errnum), errnum, excptname, dsc);
		std::string ret(buf);
		return ret;
	}

	const char* Utility::ErrnoName(int num)
	{
		switch(num)
		{
		case EPERM:
			return "EPERM";
		case ENOENT:
			return "ENOENT";
		case ESRCH:
			return "ESRCH";
		case EINTR:
			return "EINTR";
		case EIO:
			return "EIO";
		case ENXIO:
			return "ENXIO";
		case E2BIG:
			return "E2BIG";
		case ENOEXEC:
			return "ENOEXEC";
		case EBADF:
			return "EBADF";
		case ECHILD:
			return "ECHILD";
		case EAGAIN:
			return "EAGAIN";
		case ENOMEM:
			return "ENOMEM";
		case EACCES:
			return "EACCES";
		case EFAULT:
			return "EFAULT";
		case ENOTBLK:
			return "ENOTBLK";
		case EBUSY:
			return "EBUSY";
		case EEXIST:
			return "EEXIST";
		case EXDEV:
			return "EXDEV";
		case ENODEV:
			return "ENODEV";
		case ENOTDIR:
			return "ENOTDIR";
		case EISDIR:
			return "EISDIR";
		case EINVAL:
			return "EINVAL";
		case ENFILE:
			return "ENFILE";
		case EMFILE:
			return "EMFILE";
		case ENOTTY:
			return "ENOTTY";
		case ETXTBSY:
			return "ETXTBSY";
		case EFBIG:
			return "EFBIG";
		case ENOSPC:
			return "ENOSPC";
		case ESPIPE:
			return "ESPIPE";
		case EROFS:
			return "EROFS";
		case EMLINK:
			return "EMLINK";
		case EPIPE:
			return "EPIPE";
		case EDOM:
			return "EDOM";
		case ERANGE:
			return "ERANGE";
		case EDEADLK:
			return "EDEADLK";
		case ENAMETOOLONG:
			return "ENAMETOOLONG";
		case ENOLCK:
			return "ENOLCK";
		case ENOSYS:
			return "ENOSYS";
		case ENOTEMPTY:
			return "ENOTEMPTY";
		case ELOOP:
			return "ELOOP";
		case ENOMSG:
			return "ENOMSG";
		case EIDRM:
			return "EIDRM";
		case ECHRNG:
			return "ECHRNG";
		case EL2NSYNC:
			return "EL2NSYNC";
		case EL3HLT:
			return "EL3HLT";
		case EL3RST:
			return "EL3RST";
		case ELNRNG:
			return "ELNRNG";
		case EUNATCH:
			return "EUNATCH";
		case ENOCSI:
			return "ENOCSI";
		case EL2HLT:
			return "EL2HLT";
		case EBADE:
			return "EBADE";
		case EBADR:
			return "EBADR";
		case EXFULL:
			return "EXFULL";
		case ENOANO:
			return "ENOANO";
		case EBADRQC:
			return "EBADRQC";
		case EBADSLT:
			return "EBADSLT";
		case EBFONT:
			return "EBFONT";
		case ENOSTR:
			return "ENOSTR";
		case ENODATA:
			return "ENODATA";
		case ETIME:
			return "ETIME";
		case ENOSR:
			return "ENOSR";
		case ENONET:
			return "ENONET";
		case ENOPKG:
			return "ENOPKG";
		case EREMOTE:
			return "EREMOTE";
		case ENOLINK:
			return "ENOLINK";
		case EADV:
			return "EADV";
		case ESRMNT:
			return "ESRMNT";
		case ECOMM:
			return "ECOMM";
		case EPROTO:
			return "EPROTO";
		case EMULTIHOP:
			return "EMULTIHOP";
		case EDOTDOT:
			return "EDOTDOT";
		case EBADMSG:
			return "EBADMSG";
		case EOVERFLOW:
			return "EOVERFLOW";
		case ENOTUNIQ:
			return "ENOTUNIQ";
		case EBADFD:
			return "EBADFD";
		case EREMCHG:
			return "EREMCHG";
		case ELIBACC:
			return "ELIBACC";
		case ELIBBAD:
			return "ELIBBAD";
		case ELIBSCN:
			return "ELIBSCN";
		case ELIBMAX:
			return "ELIBMAX";
		case ELIBEXEC:
			return "ELIBEXEC";
		case EILSEQ:
			return "EILSEQ";
		case ERESTART:
			return "ERESTART";
		case ESTRPIPE:
			return "ESTRPIPE";
		case EUSERS:
			return "EUSERS";
		case ENOTSOCK:
			return "ENOTSOCK";
		case EDESTADDRREQ:
			return "EDESTADDRREQ";
		case EMSGSIZE:
			return "EMSGSIZE";
		case EPROTOTYPE:
			return "EPROTOTYPE";
		case ENOPROTOOPT:
			return "ENOPROTOOPT";
		case EPROTONOSUPPORT:
			return "EPROTONOSUPPORT";
		case ESOCKTNOSUPPORT:
			return "ESOCKTNOSUPPORT";
		case EOPNOTSUPP:
			return "EOPNOTSUPP";
		case EPFNOSUPPORT:
			return "EPFNOSUPPORT";
		case EAFNOSUPPORT:
			return "EAFNOSUPPORT";
		case EADDRINUSE:
			return "EADDRINUSE";
		case EADDRNOTAVAIL:
			return "EADDRNOTAVAIL";
		case ENETDOWN:
			return "ENETDOWN";
		case ENETUNREACH:
			return "ENETUNREACH";
		case ENETRESET:
			return "ENETRESET";
		case ECONNABORTED:
			return "ECONNABORTED";
		case ECONNRESET:
			return "ECONNRESET";
		case ENOBUFS:
			return "ENOBUFS";
		case EISCONN:
			return "EISCONN";
		case ENOTCONN:
			return "ENOTCONN";
		case ESHUTDOWN:
			return "ESHUTDOWN";
		case ETOOMANYREFS:
			return "ETOOMANYREFS";
		case ETIMEDOUT:
			return "ETIMEDOUT";
		case ECONNREFUSED:
			return "ECONNREFUSED";
		case EHOSTDOWN:
			return "EHOSTDOWN";
		case EHOSTUNREACH:
			return "EHOSTUNREACH";
		case EALREADY:
			return "EALREADY";
		case EINPROGRESS:
			return "EINPROGRESS";
		case ESTALE:
			return "ESTALE";
		case EUCLEAN:
			return "EUCLEAN";
		case ENOTNAM:
			return "ENOTNAM";
		case ENAVAIL:
			return "ENAVAIL";
		case EISNAM:
			return "EISNAM";
		case EREMOTEIO:
			return "EREMOTEIO";
		case EDQUOT:
			return "EDQUOT";
		case ENOMEDIUM:
			return "ENOMEDIUM";
		case EMEDIUMTYPE:
			return "EMEDIUMTYPE";
		case ECANCELED:
			return "ECANCELED";
		case ENOKEY:
			return "ENOKEY";
		case EKEYEXPIRED:
			return "EKEYEXPIRED";
		case EKEYREVOKED:
			return "EKEYREVOKED";
		case EKEYREJECTED:
			return "EKEYREJECTED";
		case EOWNERDEAD:
			return "EOWNERDEAD";
		case ENOTRECOVERABLE:
			return "ENOTRECOVERABLE";
		default:
			return "UNKNOW";
		}
	}

	std::string Utility::PrintCfgNameValue(const char* name, int value, int type)
	{
		char sBuf[512] = {0};

		// =
		if(type == 0)
		{
			sprintf(sBuf, "\t%s=%d\n", name, value);
		}
		// \t
		else
		{
			sprintf(sBuf, "%-20s%d\n", name, value);
		}

		return sBuf;
	}

	std::string Utility::PrintCfgNameValue(const char* name, const std::string& value, int type)
	{
		char sBuf[512] = {0};

		// =
		if(type == 0)
		{
			sprintf(sBuf, "\t%s=\"%s\"\n", name, value.c_str());
		}
		// \t
		else
		{
			sprintf(sBuf, "%-20s\"%s\"\n", name, value.c_str());
		}

		return sBuf;
	}

	long Utility::RoundUp(long num, long size)
	{
		assert(size != 0);

		ldiv_t t = ldiv(num, size);

		if(t.rem != 0)
		{
			num = num + size - t.rem;
		}

		return num;
	}

	std::string Utility::DateNow()
	{
		time_t nowsec = time(NULL);
		struct tm* nowTime = localtime(&nowsec);

		char nowBuf[64] = {0};

		if(NULL != nowTime)
		{
			sprintf(nowBuf, "%d%02d%02d",
			        nowTime->tm_year + 1900,
			        nowTime->tm_mon + 1,
			        nowTime->tm_mday);
		}

		return nowBuf;
	}

	std::string Utility::GetRandomString()
	{
		time_t nowsec = (time_t)time(NULL);
		int n = nowsec * rand() % INT_MAX;
		char buf[64] = {0};
		sprintf(buf, "%x", n);
		return buf;
	}

	void Utility::RegisterSignal(int signum, PF_SIGHANDLER_T handler)
	{
		struct sigaction sigact;

		memset(&sigact, 0, sizeof(sigact));

		sigact.sa_handler = handler;

		sigaction(signum, &sigact, (struct sigaction*)NULL);
	}

	void Utility::DefaultSignalHandler(int signum)
	{
		printf("DefaultSignalHandler %d\n", signum);
		sleep(10);
	}

	std::string Utility::GetTimeID()
	{
		std::string strResult;
		char bufTemp[2][32] = {{0}, {0}};
		struct timeval tv = {0};
		struct timezone tz = {0};

		gettimeofday(&tv, &tz);

		// 10位定长
		sprintf(bufTemp[0], "%d", (int)tv.tv_sec);
		// 6位定长
		sprintf(bufTemp[1], "%06d", (int)tv.tv_usec);

		strResult = bufTemp[0];
		strResult += bufTemp[1];

		return strResult;
	}

	std::string Utility::DateTimeNow()
	{
		time_t nowsec = time(NULL);
		struct tm* nowTime = localtime(&nowsec);

		char nowBuf[64] = {0};

		if(NULL != nowTime)
		{
			sprintf(nowBuf, "%d/%02d/%02d %02d:%02d:%02d",
			        nowTime->tm_year + 1900,
			        nowTime->tm_mon + 1,
			        nowTime->tm_mday,
			        nowTime->tm_hour,
			        nowTime->tm_min,
			        nowTime->tm_sec);
		}

		return nowBuf;
	}

	LWPR::TIME_PRECISION_T Utility::GetTimePrecision()
	{
		TIME_PRECISION_T tp = {0};
		struct timeval tv = {0};
		struct timezone tz = {0};

		gettimeofday(&tv, &tz);

		tp.nSeconds = (LWPR::UINT32)tv.tv_sec;
		tp.nUSeconds = (LWPR::UINT32)tv.tv_usec;

		return tp;
	}

	LWPR::TIME_PRECISION_T Utility::GetBootTime()
	{
		return _nBootTime;
	}

	UINT64 Utility::htonl64(UINT64 host)
	{
#ifndef LWPR_BIG_ENDIAN
		return SWAP_BYTE_ORDER64(host);
#endif
		return host;
	}

	UINT64 Utility::ntohl64(UINT64 net)
	{
#ifndef LWPR_BIG_ENDIAN
		return SWAP_BYTE_ORDER64(net);
#endif
		return net;
	}

	bool Utility::HasLicenseEffect(int days)
	{
		static const char* mt[] =
		{
			"Jan",		// 一月  	January
			"Feb",		// 二月  	February
			"Mar",		// 三月  	March
			"Apr",		// 四月  	April
			"May",		// 五月  	May
			"Jun",		// 六月  	June
			"Jul",		// 七月  	July
			"Aug",		// 八月  	Aguest
			"Sep",		// 九月  	September
			"Oct",		// 十月  	October
			"Nov",		// 十一月	November
			"Dec",		// 十二月	December
		};

		char date[3][16];
		int nMonIndex = -1;

		memset(date, 0, sizeof(date));
		sscanf(__DATE__, "%s%s%s", date[0], date[1], date[2]);

		for(int i = 0; i < (int)(sizeof(mt) / sizeof(char *)); i++)
		{
			if(!strcmp(date[0], mt[i]))
			{
				nMonIndex = i;
				break;
			}
		}

		if(-1 == nMonIndex) return false;

		time_t currentTime = time(NULL);
		struct tm currentTm = {0};
		memcpy(&currentTm, localtime(&currentTime), sizeof(currentTm));
		currentTm.tm_year = atoi(date[2]) - 1900;
		currentTm.tm_mon = nMonIndex;
		currentTm.tm_mday = atoi(date[1]);
		time_t endTime = mktime(&currentTm);
		endTime += days * 24 * 60 * 60;

		return endTime >= currentTime;
	}

	LWPR::UINT16 Utility::IntegerToLocalByte16(LWPR::UINT16 value, bool bigendian)
	{
		bool bSwap = false;
#ifdef LWPR_BIG_ENDIAN
		bSwap = !bigendian;
#else
		bSwap = bigendian;
#endif
		return bSwap ? SWAP_BYTE_ORDER16(value) : value;
	}

	LWPR::UINT32 Utility::IntegerToLocalByte32(LWPR::UINT32 value, bool bigendian)
	{
		bool bSwap = false;
#ifdef LWPR_BIG_ENDIAN
		bSwap = !bigendian;
#else
		bSwap = bigendian;
#endif
		return bSwap ? SWAP_BYTE_ORDER32(value) : value;
	}

	LWPR::UINT64 Utility::IntegerToLocalByte64(LWPR::UINT64 value, bool bigendian)
	{
		bool bSwap = false;
#ifdef LWPR_BIG_ENDIAN
		bSwap = !bigendian;
#else
		bSwap = bigendian;
#endif
		return bSwap ? SWAP_BYTE_ORDER64(value) : value;
	}

	std::string Utility::ByteToHumanFmt(long size)
	{
		const char* fmt[] = {"B", "KB", "MB", "GB", "TB"};
		char buf[128] = {0};

		for(size_t i = 1; i < sizeof(fmt) / sizeof(char*); i++)
		{
			ldiv_t tr = ldiv(size, 1024);
			if(tr.quot >= 0 && tr.quot < 1024)
			{
				sprintf(buf, "%ld.%03ld %s", tr.quot, tr.rem, fmt[i]);
				break;
			}

			size = tr.quot;
		}

		return buf;
	}

	bool Utility::PutEnv(char* str)
	{
		if(NULL == str)
		{
			return false;
		}

		Buffer bufName;
		Buffer bufValue;

		bufName.Reset();
		bufValue.Reset();

		// 分割成name/value
		int code = sscanf(str, "%[^ =]%*[ =]%[^$]", bufName.Inout(), bufValue.Inout());
		if(code == 2)
		{
			return PutEnv(bufName.Inout(), bufValue.Inout()) == 0;
		}

		return false;
	}

	bool Utility::PutEnv(char* name, char* value)
	{
		return setenv(name, value, 1) == 0;
	}

	const char* Utility::GetEnv(char* str)
	{
		if(NULL == str)
		{
			return NULL;
		}

		return getenv(str);
	}

	//-----------------------------------------------------------------------------------
	// AlarmTimer，建议在进程中使用，线程中使用会导致进程异常退出。
	//-----------------------------------------------------------------------------------
	AlarmTimer::AlarmTimer(int timeout)
		: m_nTimeout(timeout)
	{
		if(m_nTimeout > 0)
		{
			m_nLastRemainTime = alarm(timeout);
		}
	}

	AlarmTimer::~AlarmTimer()
	{
		if(m_nTimeout > 0)
		{
			alarm(m_nLastRemainTime);
		}
	}

	//----------------------------------------------------------------------------------
	// class AutomaticCount
	//----------------------------------------------------------------------------------
	AutomaticCount::AutomaticCount(AtomicInteger& atom)
		: m_AtomicInteger(atom)
	{
		m_AtomicInteger++;
	}

	AutomaticCount::~AutomaticCount()
	{
		m_AtomicInteger--;
	}

	LWPR::UINT64 SWAP_BYTE_ORDER64(LWPR::UINT64 value)
	{
		LWPR::UINT64 tmp = 0xff;

		return ((((LWPR::UINT64)(value) & (tmp << (8 * 7))) >> 56) |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 6))) >> 40) |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 5))) >> 24) |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 4))) >> 8)  |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 3))) << 8)  |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 2))) << 24) |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 1))) << 40) |
		        (((LWPR::UINT64)(value) & (tmp << (8 * 0))) << 56));
	}
};
