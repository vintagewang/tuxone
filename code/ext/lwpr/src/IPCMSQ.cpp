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
#include "IPCMSQ.h"
#include <cassert>

namespace LWPR
{
	LWPR::IPCID_T IPCMSQ::Create(const IPCKEY_T key,
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
			assert(0);
		}

		retcode = ::msgget(key, flg);
		//if(retcode == -1)
		//{
		//	if(EEXIST == errno)
		//		throw LWPR_IPC_MSQ_EXIST(EXCEPTION_TRACE, "msgget the key is already exist");

		//	throw LWPR_IPC_MSQ_GET_ERR(EXCEPTION_TRACE, "msgget error");
		//}

		return retcode;
	}

	LWPR::IPCID_T IPCMSQ::Create()
	{
		return Create(IPC_PRIVATE, CREATE_NEW_ONE, IPC_DEFAULT_PERM);
	}

	LWPR::IPCID_T IPCMSQ::GetID(const IPCKEY_T key)
	{
		int retcode = ::msgget(key, 0);

		//if(retcode == INVALID_IPC_ID)
		//{
		//	throw LWPR_IPC_MSQ_NOT_EXIST(EXCEPTION_TRACE, "msq not exist");
		//}

		return retcode;
	}

	void IPCMSQ::Destroy(const IPCID_T id)
	{
		::msgctl(id, IPC_RMID, 0);
	}

	void IPCMSQ::Send(const IPCID_T id, const Buffer& msg, bool blocked /*= true*/)
	{
		int retcode = 0;
		int flg = 0;

		if(!blocked)
			flg |= IPC_NOWAIT;

		if(msg.Size() == 0)
			throw LWPR_BAD_PARM(EXCEPTION_TRACE, "msg buffer size is not set");

		retcode = ::msgsnd(id, msg.Inout(), msg.Size(), flg);
		if(retcode == -1)
		{
			if(EAGAIN == errno)
				throw LWPR_IPC_MSQ_SND_MSQFULL(EXCEPTION_TRACE, "msgsnd msq is full");

			throw LWPR_IPC_MSQ_SND_ERR(EXCEPTION_TRACE, "msgsnd error");
		}
	}

	LWPR::Buffer_ptr IPCMSQ::Receive(const IPCID_T id, long type /*= 0*/, bool blocked /*= true*/)
	{
		int retcode = 0;
		int flg = 0;

		if(!blocked)
			flg |= IPC_NOWAIT;

		Buffer_var vbufMsg = new Buffer(MSG_RCV_SIZE_DEFAULT);

		int mult = 1;
		int dftSize = vbufMsg->Capacity();

MSGRCV:
		vbufMsg->Capacity(dftSize *(mult++));
		retcode = ::msgrcv(id, vbufMsg->Inout(), vbufMsg->Capacity() - sizeof(long), type, flg);
		if(retcode == -1)
		{
			if(E2BIG == errno)
			{
				goto MSGRCV;
			}

			if(ENOMSG == errno ||
			   EAGAIN == errno)
				throw LWPR_IPC_MSQ_RCV_NOMSG(EXCEPTION_TRACE, "msgrcv no msg in queue");

			throw LWPR_IPC_MSQ_RCV_ERR(EXCEPTION_TRACE, "msgrcv error");
		}

		if(retcode == 0)
			throw LWPR_IPC_MSQ_RCV_ERR(EXCEPTION_TRACE, "msgrcv error, receive msg size is 0");

		vbufMsg->Size(retcode + sizeof(long));

		return vbufMsg.retn();
	}

	LWPR::IPCMSQ_RET_TYPE_E IPCMSQ::SendMsg(IPCID_T id, const char*buf, int length, bool blocked /*= true*/)
	{
		int nFlag = 0;

		if(buf == NULL || length <= 0)
		{
			return IPCMSQ_RET_FAILED;
		}

		if(!blocked)
		{
			nFlag |= IPC_NOWAIT;
		}

		int nRetCode = ::msgsnd(id, buf, length - sizeof(long), nFlag);
		if(nRetCode == -1)
		{
			if(EAGAIN == errno)
			{
				return IPCMSQ_RET_MSG_FULL;
			}
			else if(EINTR == errno)
			{
				return IPCMSQ_RET_EINTR;
			}
			else
			{
				return IPCMSQ_RET_FAILED;
			}
		}

		return IPCMSQ_RET_OK;
	}

	LWPR::IPCMSQ_RET_TYPE_E IPCMSQ::ReceiveMsg(IPCID_T id, Buffer& msg, bool blocked /*= true*/)
	{
		int nFlag = 0;

		if(!blocked)
			nFlag |= IPC_NOWAIT;

		int nMult = 1;
		int nDefaultSize = msg.Capacity(MSG_RCV_SIZE_DEFAULT);

MSGRCV:
		msg.Capacity(nDefaultSize *(nMult++));
		int nRetCode = ::msgrcv(id, msg.Inout(), msg.Capacity() - sizeof(long), 0, nFlag);
		if(nRetCode == -1)
		{
			if(E2BIG == errno)
			{
				goto MSGRCV;
			}
			else if(ENOMSG == errno || EAGAIN == errno)
			{
				return IPCMSQ_RET_NO_MSG;
			}
			else if(EINTR == errno)
			{
				return IPCMSQ_RET_EINTR;
			}
			else
			{
				return IPCMSQ_RET_FAILED;
			}
		}

		msg.Size(nRetCode + sizeof(long));

		return IPCMSQ_RET_OK;
	}
};
