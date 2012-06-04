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
#include "TLSLibtux.h"

//----------------------------------------------------------------
// MyValue
//----------------------------------------------------------------
TLSLibtux::MyValue::MyValue() :
	m_nFERRNO(0), m_nTPERRNO(0), m_nBalanceFactor(0)
{

}

TLSLibtux::MyValue::~MyValue()
{

}

//----------------------------------------------------------------
// TLSLibtux
//----------------------------------------------------------------
TLSLibtux::TLSLibtux()
{

}

TLSLibtux::~TLSLibtux()
{

}

TLSLibtux* TLSLibtux::GetInstance()
{
	static TLSLibtux *singleton = new TLSLibtux();
	return singleton;
}

int* TLSLibtux::GetTperrno()
{
	try
	{
		MyValue* p = dynamic_cast < MyValue* >(GetValue());
		if(NULL == p)
		{
			p = new MyValue;
			SetValue(p);
		}

		return &(p->m_nTPERRNO);
	}
	catch(...)
	{
	}

	assert(0);
}

LWPR::UINT32 TLSLibtux::GetNextBalanceFactor()
{
	try
	{
		MyValue* p = dynamic_cast < MyValue* >(GetValue());
		if(NULL == p)
		{
			p = new MyValue;
			SetValue(p);
		}

		return p->m_nBalanceFactor++;
	}
	catch(...)
	{
	}

	assert(0);
}

sigjmp_buf& TLSLibtux::GetJmpBuf()
{
	try
	{
		MyValue* p = dynamic_cast < MyValue* >(GetValue());
		if(NULL == p)
		{
			p = new MyValue;
			SetValue(p);
		}

		return p->m_jmp_buf;
	}
	catch(...)
	{
	}

	assert(0);
}

TPRETURN_INFO_T& TLSLibtux::GetTpreturnInfo()
{
	try
	{
		MyValue* p = dynamic_cast < MyValue* >(GetValue());
		if(NULL == p)
		{
			p = new MyValue;
			SetValue(p);
		}

		return p->m_tpreturn_info;
	}
	catch(...)
	{
	}

	assert(0);
}

int* TLSLibtux::GetFerrno()
{
	try
	{
		MyValue* p = dynamic_cast < MyValue* >(GetValue());
		if(NULL == p)
		{
			p = new MyValue;
			SetValue(p);
		}

		return &(p->m_nFERRNO);
	}
	catch(...)
	{
	}

	assert(0);
}

void TLSLibtux::PutEnv(char* str)
{

}

const char* TLSLibtux::GetEnv(char* str)
{

	return NULL;
}
