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
#ifndef TUX_TLSLIBTUX_H__
#define TUX_TLSLIBTUX_H__
#include <lwpr.h>
#include <setjmp.h>
#include <map>

typedef std::map< std::string, std::string > EnvTable;

typedef struct tpreturn_info
{
	int rval;
	long rcode;
	char *data;
	long len;
	long flags;
} TPRETURN_INFO_T;

class TLSLibtux : public LWPR::ThreadLocal
{
public:

	TLSLibtux();
	~TLSLibtux();
	static TLSLibtux* GetInstance();

	int* GetTperrno();

	int* GetFerrno();

	LWPR::UINT32 GetNextBalanceFactor();

	sigjmp_buf& GetJmpBuf();

	TPRETURN_INFO_T& GetTpreturnInfo();

	void PutEnv(char* str);

	const char* GetEnv(char* str);

private:
	class MyValue : public LWPR::TLSValue
	{
	public:
		MyValue();
		~MyValue();

	public:
		int				m_nFERRNO;			// 错误码
		int				m_nTPERRNO;			// 错误码
		LWPR::UINT32	m_nBalanceFactor;	// 为负载均衡使用
		sigjmp_buf		m_jmp_buf;
		TPRETURN_INFO_T m_tpreturn_info;
		EnvTable		m_EnvTable;			// 每个线程对应的环境变量
	};
};

#endif // end of TUX_TLSLIBTUX_H__
