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
#include "IPCSEM.h"
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

namespace LWPR
{
	union semun
	{
		int             val;
		struct semid_ds *buf;
		ushort          *array;
	};
	typedef union semun     SEMUN_U;
	typedef struct sembuf   SEMBUF_T;

	//#define TUXONE_SEMVMX SEMVMX
#define TUXONE_SEMVMX 1

	LWPR::IPCID_T IPCSEM::Create(const IPCKEY_T key,
	                             bool iswrlock /*= false*/,
	                             IPC_CREATE_TYPE_E type /*= LWPR::CREATE_NEW_ONE*/,
	                             int perm /*= IPC_DEFAULT_PERM*/)
	{
		int retcode = 0;
		int flg = perm;

		switch(type)
		{
		case CREATE_NEW_ONE:
			flg |= IPC_CREAT;
			break;
		case CREATE_NEW_ONE_EXCL:
			flg |= IPC_CREAT;
			flg |= IPC_EXCL;
			break;
		case GET_EXIST_ONE:
			break;
		default:
			throw LWPR_BAD_PARM(EXCEPTION_TRACE, "sem create type error");
		}

		retcode = ::semget(key, 1, flg);
		if(retcode == -1)
		{
			throw LWPR_IPC_SEM_GET_ERR(EXCEPTION_TRACE, "semget error");
		}

		SEMUN_U semarg;
		semarg.val = iswrlock ? TUXONE_SEMVMX : 1;

		if(::semctl(retcode, 0, SETVAL, semarg) == -1)
			throw LWPR_IPC_SEM_RESET_ERR(EXCEPTION_TRACE, "semctl error");

		return retcode;
	}


	LWPR::IPCID_T IPCSEM::GetID(const IPCKEY_T key)
	{
		int retcode = ::semget(key, 0, 0);

		if(retcode == IPC_INVALID_ID)
		{
			throw LWPR_IPC_SEM_NOT_EXIST(EXCEPTION_TRACE, "sem not exist");
		}

		return retcode;
	}

	void IPCSEM::Destroy(const IPCID_T id)
	{
		::semctl(id, 0, IPC_RMID, 0);
	}

	void IPCSEM::Lock(const IPCID_T id, bool blocked /*= true*/, bool iswrlock /*= false*/)
	{
		int retcode = 0;
		SEMBUF_T opt;

		memset(&opt, 0, sizeof(opt));

		opt.sem_num = 0;
		opt.sem_flg = SEM_UNDO;
		opt.sem_op = iswrlock ? TUXONE_SEMVMX : 1;
		opt.sem_op *= -1;

		if(!blocked)
			opt.sem_flg |= IPC_NOWAIT;

SEMOP:
		retcode = ::semop(id, &opt, 1);
		if(retcode == -1)
		{
			if(EINTR == errno)
				goto SEMOP;

			if(EAGAIN == errno)
				throw LWPR_IPC_SEM_LOCKED_BEFORE(EXCEPTION_TRACE, "semop the sem is locked before");

			throw LWPR_IPC_SEM_LOCK_ERR(EXCEPTION_TRACE, "semop error");
		}
	}

	void IPCSEM::Unlock(const IPCID_T id, bool iswrlock /*= false*/)
	{
		int retcode = 0;
		SEMBUF_T opt;

		assert(id >= 0);

		memset(&opt, 0, sizeof(opt));

		opt.sem_num = 0;
		opt.sem_op = iswrlock ? TUXONE_SEMVMX : 1;

SEMOP:
		retcode = ::semop(id, &opt, 1);
		if(retcode == -1)
		{
			if(EINTR == errno)
				goto SEMOP;

			throw LWPR_IPC_SEM_UNLOCK_ERR(EXCEPTION_TRACE, "semop error");
		}
	}

	bool IPCSEM::IsLocked(const IPCID_T id)
	{
		int semval = 0;

		semval = ::semctl(id, 0, GETVAL);

		if(-1 == semval)
			throw LWPR_IPC_SEM_CTL_ERR(EXCEPTION_TRACE, "semctl error");

		return (semval == 0 ? true : false);
	}
};
