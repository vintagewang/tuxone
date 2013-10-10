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
#ifndef LWPR_IPCSEM_H__
#define LWPR_IPCSEM_H__
#include "LWPRType.h"
#include "Exception.h"

namespace LWPR
{
	/**
	 * 本文件中提到的信号量都是为读写控制所用
	 * 分两种类型：正常锁与读写锁
	 */
	class IPCSEM
	{
	public:
		/**
		 * 创建一个信号量，单值信号量
		 * iswrlock标志是否是读写锁，如果是读写锁，则设置信号量的值为SHRT_MAX
		 * 读锁时减1，写锁时减－SHRT_MAX
		 */
		static IPCID_T Create(const IPCKEY_T key,
		                      bool iswrlock = false,
		                      IPC_CREATE_TYPE_E type = LWPR::CREATE_NEW_ONE,
		                      int perm = IPC_DEFAULT_PERM);

		/**
		 * 根据IPC Key返回相应的IPC ID
		 */
		static IPCID_T GetID(const IPCKEY_T key);

		/**
		 * 销毁一个信号量对象
		 */
		static void Destroy(const IPCID_T id);

		/**
		 * 加锁信号量
		 */
		static void Lock(const IPCID_T id, bool blocked = true, bool iswrlock = false);

		/**
		 * 解锁信号量
		 */
		static void Unlock(const IPCID_T id, bool iswrlock = false);

		/**
		 * 判断信号量是否处于加锁状态
		 */
		static bool IsLocked(const IPCID_T id);
	};

	DEFINE_EXCEPTION(LWPR_IPC_SEM_GET_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SEM_RESET_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SEM_LOCK_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SEM_UNLOCK_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SEM_CTL_ERR);

	DEFINE_EXCEPTION(LWPR_IPC_SEM_LOCKED_BEFORE);
	DEFINE_EXCEPTION(LWPR_IPC_SEM_NOT_EXIST);
};

#endif // end of LWPR_IPCSEM_H__
