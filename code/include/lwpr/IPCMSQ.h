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
#ifndef LWPR_IPCMSQ_H__
#define LWPR_IPCMSQ_H__
#include "LWPRType.h"
#include "Var.h"
#include "Buffer.h"
#include "LWPRType.h"
#include "Exception.h"
//#include "IPCMsgBuffer.h"

namespace LWPR
{
	typedef enum ipcmsq_ret_type_e
	{
		IPCMSQ_RET_FAILED = -1,		// ipc msq出错
		IPCMSQ_RET_OK = 0,			// ipc msq成功
		IPCMSQ_RET_MSG_FULL = 1,	// ipc msq发送队列消息满
		IPCMSQ_RET_NO_MSG = 2,		// ipc msq接收队列无消息
		IPCMSQ_RET_EINTR = 3,		// ipc msq系统调用被信号中断
	} IPCMSQ_RET_TYPE_E;

	const int MSG_RCV_SIZE_DEFAULT = 2048;

	class IPCMSQ
	{
	public:
		/**
		 * 创建一个消息队列
		 */
		static IPCID_T Create(const IPCKEY_T key,
		                      IPC_CREATE_TYPE_E type = LWPR::CREATE_NEW_ONE,
		                      int perm = IPC_DEFAULT_PERM);

		/**
		 * 创建一个消息队列(IPC_PRIVATE)
		 */
		static IPCID_T Create();

		/**
		 * 根据IPC Key返回相应的IPC ID
		 */
		static IPCID_T GetID(const IPCKEY_T key);

		/**
		 * 销毁一个消息队列
		 */
		static void Destroy(const IPCID_T id);

		/**
		 * 向队列中发送一条消息，默认为阻塞方式
		 */
		static void Send(const IPCID_T id, const Buffer& msg, bool blocked = true);

		/**
		 * 向队列中发送一条消息，默认为阻塞方式
		 */
		static IPCMSQ_RET_TYPE_E SendMsg(IPCID_T id, const char*buf, int length, bool blocked = true);

		/**
		 * 从队列中接收一条消息，并删除队列中的这条消息，默认为阻塞模式
		 */
		static LWPR::Buffer_ptr Receive(const IPCID_T id, long type = 0, bool blocked = true);

		/**
		 * 从队列中接收一条消息，并删除队列中的这条消息，默认为阻塞模式
		 */
		static IPCMSQ_RET_TYPE_E ReceiveMsg(IPCID_T id, Buffer& msg, bool blocked = true);

	};

	DEFINE_EXCEPTION(LWPR_IPC_MSQ_GET_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_SND_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_RCV_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_CTL_ERR);

	DEFINE_EXCEPTION(LWPR_IPC_MSQ_EXIST);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_NOT_EXIST);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_RCV_NOMSG);
	DEFINE_EXCEPTION(LWPR_IPC_MSQ_SND_MSQFULL);
};

#endif // end of LWPR_IPCMSQ_H__
