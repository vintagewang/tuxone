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
#include "InputStream.h"
#include <cstring>
#include <arpa/inet.h>

namespace LWPR
{
	InputStream::InputStream(Buffer_ptr buf)
		: m_vBuffer(Buffer_var::Duplicate(buf)),
		  m_nOffSet(0)
	{
		assert(buf != NULL);
	}

	void InputStream::Reset()
	{
		m_nOffSet = 0;
	}

	BOOL InputStream::ReadBOOL()
	{
		if(!(m_nOffSet + (int)sizeof(BOOL) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		BOOL nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(BOOL));
		m_nOffSet += sizeof(BOOL);
		return nValue;
	}

	INT8 InputStream::ReadINT8()
	{
		if(!(m_nOffSet + (int)sizeof(INT8) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		INT8 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(INT8));
		m_nOffSet += sizeof(INT8);
		return nValue;
	}

	UINT8 InputStream::ReadUINT8()
	{
		if(!(m_nOffSet + (int)sizeof(UINT8) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		UINT8 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(UINT8));
		m_nOffSet += sizeof(UINT8);
		return nValue;
	}

	INT16 InputStream::ReadINT16()
	{
		if(!(m_nOffSet + (int)sizeof(INT16) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		INT16 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(INT16));
		m_nOffSet += sizeof(INT16);
		nValue = ntohs(nValue);
		return nValue;
	}

	UINT16 InputStream::ReadUINT16()
	{
		if(!(m_nOffSet + (int)sizeof(UINT16) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		UINT16 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(UINT16));
		m_nOffSet += sizeof(UINT16);
		nValue = ntohs(nValue);
		return nValue;
	}

	INT32 InputStream::ReadINT32()
	{
		if(!(m_nOffSet + (int)sizeof(INT32) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		INT32 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(INT32));
		m_nOffSet += sizeof(INT32);
		nValue = ntohl(nValue);
		return nValue;
	}

	UINT32 InputStream::ReadUINT32()
	{
		if(!(m_nOffSet + (int)sizeof(UINT32) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		UINT32 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(UINT32));
		m_nOffSet += sizeof(UINT32);
		nValue = ntohl(nValue);
		return nValue;
	}

	INT64 InputStream::ReadINT64()
	{
		if(!(m_nOffSet + (int)sizeof(INT64) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		INT64 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(INT64));
		m_nOffSet += sizeof(INT64);
		nValue = Utility::ntohl64(nValue);
		return nValue;
	}


	UINT64 InputStream::ReadUINT64()
	{
		if(!(m_nOffSet + (int)sizeof(UINT64) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		UINT64 nValue = 0;
		memcpy(&nValue, m_vBuffer->Inout() + m_nOffSet, sizeof(UINT64));
		m_nOffSet += sizeof(UINT64);
		nValue = Utility::ntohl64(nValue);
		return nValue;
	}

	FP32 InputStream::ReadFloat()
	{
		if(!(m_nOffSet + (int)sizeof(float) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		float fValue = 0;
		memcpy(&fValue, m_vBuffer->Inout() + m_nOffSet, sizeof(float));
		m_nOffSet += sizeof(float);
		return fValue;
	}

	FP64 InputStream::ReadDouble()
	{
		if(!(m_nOffSet + (int)sizeof(double) <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		double dValue = 0;
		memcpy(&dValue, m_vBuffer->Inout() + m_nOffSet, sizeof(double));
		m_nOffSet += sizeof(double);
		return dValue;
	}

	std::string InputStream::ReadString()
	{
		INT32 nLen = ReadINT32();

		if(0 == nLen) return "";

		if(!(m_nOffSet + nLen <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		Buffer_var vBuf = new Buffer(nLen + 1);
		memcpy(vBuf->Inout(), m_vBuffer->Inout() + m_nOffSet, nLen);
		m_nOffSet += nLen;

		return vBuf->Inout();
	}

	Buffer_ptr InputStream::ReadAny()
	{
		INT32 nLen = ReadINT32();

		if(!(m_nOffSet + nLen <= m_vBuffer->Capacity()))
		{
			throw UnmarshalException(EXCEPTION_TRACE, "InputStream buffer length is not enough");
		}

		Buffer_var vBuf = new Buffer(nLen + 1);
		vBuf->Size(nLen);
		memcpy(vBuf->Inout(), m_vBuffer->Inout() + m_nOffSet, nLen);
		m_nOffSet += nLen;

		return vBuf.retn();
	}

	void InputStream::ReadArray(void* buf)
	{
		Buffer_var vBuf = ReadAny();
		memcpy(buf, vBuf->Inout(), vBuf->Size());
	}

	StringBuffer_ptr InputStream::ReadLine()
	{
		int nStartPos = m_nOffSet;

		if(nStartPos >= m_vBuffer->Capacity()) return NULL;

		int i;
		for(i = nStartPos; i < m_vBuffer->Capacity(); i++)
		{
			if(*(m_vBuffer->Inout() + i) == '\n') break;
		}

		m_nOffSet = i + 1;
		Buffer_var vBuf = m_vBuffer->CopyValueOf(nStartPos, i - nStartPos);

		StringBuffer_var vStrbuf = new StringBuffer(*vBuf);
		return vStrbuf.retn();
	}

};
