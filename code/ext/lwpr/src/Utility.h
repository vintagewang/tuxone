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
#ifndef LWPR_UTILITY_H__
#define LWPR_UTILITY_H__
#include "LWPRType.h"
#include "AtomicInteger.h"
#include <string>

namespace LWPR
{
#define SWAP_BYTE_ORDER16(A) \
	((((LWPR::UINT16)(A) & 0xff00) >> 8) | \
	(((LWPR::UINT16)(A) & 0x00ff) << 8))

#define SWAP_BYTE_ORDER32(A) \
	((((LWPR::UINT32)(A) & 0xff000000) >> 24) | \
	(((LWPR::UINT32)(A) & 0x00ff0000) >> 8 ) | \
	(((LWPR::UINT32)(A) & 0x0000ff00) << 8 ) | \
	(((LWPR::UINT32)(A) & 0x000000ff) << 24))

	LWPR::UINT64 SWAP_BYTE_ORDER64(LWPR::UINT64 value);

	typedef struct time_precision_t
	{
		LWPR::UINT32 nSeconds;	// 秒
		LWPR::UINT32 nUSeconds;	// 微秒
	} TIME_PRECISION_T;

	class Utility
	{
		static PID_T _nPID;
		static TIME_PRECISION_T _nBootTime;

	public:
		static const char* ErrnoName(int num);
		static std::string ExceptionInfo(const char *file,
		                                 int line,
		                                 const char *fun,
		                                 int errnum,
		                                 const char *excptname,
		                                 const char *dsc);
		static PID_T GetPid();

		/**
		 * 获取当前时刻的精确时间
		 */
		static TIME_PRECISION_T GetTimePrecision();

		/**
		 * 获取当前进程的启动时间
		 */
		static TIME_PRECISION_T GetBootTime();

		/**
		 * 打印名值对，0-带=， 1-不带=
		 */
		static std::string PrintCfgNameValue(const char* name, int value, int type = 0);

		/**
		 * 打印名值对，0-带=， 1-不带=
		 */
		static std::string PrintCfgNameValue(const char* name, const std::string& value, int type = 0);

		/**
		 * 令num等于Size的倍数，且结果>=num
		 * 此函数一般用于字节对齐，或者是内存页对齐等方面
		 */
		static long RoundUp(long num, long size);

		/**
		 * 返回当前系统日期 YYYYMMDD
		 */
		static std::string DateNow();

		/**
		 * 返回当前系统日期 YYYY/MM/DD HH:MM:SS
		 */
		static std::string DateTimeNow();

		/**
		 * 获取一个唯一字符串，已时间为基准
		 */
		static std::string GetRandomString();

		/**
		 * 获取以当前时间为唯一字符串的ID
		 * 返回值： 16位定长字符串
		 */
		static std::string GetTimeID();

		/**
		 * 注册信号处理
		 */
		typedef void (*PF_SIGHANDLER_T)(int);
		static void RegisterSignal(int signum, PF_SIGHANDLER_T handler);

		/**
		 * 信号处理函数
		 */
		static void DefaultSignalHandler(int signum);

		/**
		 * host long 64 to network
		 */
		static UINT64 htonl64(UINT64 host);

		/**
		* network to host long 64
		 */
		static UINT64 ntohl64(UINT64 net);

		/**
		 * License是否有效
		 */
		static bool HasLicenseEffect(int days);

		static LWPR::UINT16 IntegerToLocalByte16(LWPR::UINT16 value, bool bigendian);

		static LWPR::UINT32 IntegerToLocalByte32(LWPR::UINT32 value, bool bigendian);

		static LWPR::UINT64 IntegerToLocalByte64(LWPR::UINT64 value, bool bigendian);

		/**
		 * 将字节数转化成易读的格式
		 */
		static std::string ByteToHumanFmt(long size);

		/**
		 * 设置环境变量
		 */
		bool PutEnv(char* str);

		/**
		 * 设置环境变量
		 */
		bool PutEnv(char* name, char* value);

		/**
		 * 返回环境变量
		 */
		const char* GetEnv(char* str);
	};

	class AlarmTimer
	{
	public:
		/**
		 * 构造函数
		 */
		AlarmTimer(int timeout);

		/**
		 * 析构函数
		 */
		~AlarmTimer();

	private:
		int m_nLastRemainTime;
		int m_nTimeout;
	};

	class AutomaticCount
	{
	public:
		/**
		 * 构造函数
		 */
		AutomaticCount(AtomicInteger& atom);

		/**
		 * 析构函数
		 */
		~AutomaticCount();

	private:
		AtomicInteger& m_AtomicInteger;
	};
};

#endif // end of LWPR_UTILITY_H__
