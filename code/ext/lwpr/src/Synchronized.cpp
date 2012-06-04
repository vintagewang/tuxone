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
#include "Synchronized.h"

namespace LWPR
{
	Synchronized::Synchronized(const IPCID_T id)
		: m_synType(SYN_IPCSEM),
		  m_id(id),
		  m_pMutex(NULL),
		  m_pSemph(NULL)
	{
		IPCSEM::Lock(m_id, true, false);
	}

	Synchronized::Synchronized(Mutex &lock)
		: m_synType(SYN_MUTEX),
		  m_id(IPC_INVALID_ID),
		  m_pMutex(&lock),
		  m_pSemph(NULL)
	{
		lock.Lock();
	}

	Synchronized::Synchronized(Semph &lock)
		: m_synType(SYN_SEMPH),
		  m_id(IPC_INVALID_ID),
		  m_pMutex(NULL),
		  m_pSemph(&lock)
	{
		lock.Lock();
	}

	Synchronized::~Synchronized()
	{
		switch(m_synType)
		{
		case SYN_IPCSEM:
			IPCSEM::Unlock(m_id, false);
			break;
		case SYN_MUTEX:
			m_pMutex->Unlock();
			break;
		case SYN_SEMPH:
			m_pSemph->Unlock();
			break;
		default:
			assert(0);
		}
	}

	//----------------------------------------------------------------------------------
	// class SynchronizedRead
	//----------------------------------------------------------------------------------
	SynchronizedRead::SynchronizedRead(RWMutex &lock)
		: m_pRWMutex(&lock)
	{
		m_pRWMutex->RDLock();
	}

	SynchronizedRead::~SynchronizedRead()
	{
		m_pRWMutex->Unlock();
	}

	//----------------------------------------------------------------------------------
	// class SynchronizedWrite
	//----------------------------------------------------------------------------------
	SynchronizedWrite::SynchronizedWrite(RWMutex &lock)
		: m_pRWMutex(&lock)
	{
		m_pRWMutex->WRLock();
	}

	SynchronizedWrite::~SynchronizedWrite()
	{
		m_pRWMutex->Unlock();
	}
};
