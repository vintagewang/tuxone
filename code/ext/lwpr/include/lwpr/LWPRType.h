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
#ifndef LWPR_LWPR_TYPE_H__
#define LWPR_LWPR_TYPE_H__
#include <cassert>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>


namespace LWPR
{
	typedef char                         BOOL;      // bool
	typedef unsigned    char            UINT8;      // Unsigned  8 bit quantity
	typedef signed      char             INT8;      // Signed    8 bit quantity
	typedef unsigned    short           UINT16;     // Unsigned 16 bit quantity
	typedef signed      short            INT16;     // Signed   16 bit quantity
	typedef unsigned    int             UINT32;     // Unsigned 32 bit quantity
	typedef signed      int              INT32;     // Signed   32 bit quantity
	typedef unsigned    long long       UINT64;     // Unsigned 64 bit quantity
	typedef signed      long long        INT64;     // Signed   64 bit quantity
	typedef float                        FP32;      // Single precision floating point
	typedef double                       FP64;      // Double precision floating point

#define LWPR_TRUE			1
#define LWPR_FALSE			0
};

namespace LWPR
{
	typedef key_t		IPCKEY_T;
	typedef int			IPCID_T;
	typedef pid_t		PID_T;
	typedef msqid_ds	MSQ_INFO_T;

	const int IPC_INVALID_ID = -1;

	typedef enum ipc_create_type
	{
		CREATE_NEW_ONE,         // 创建一个新IPC，如果已经存在，则返回成功
		CREATE_NEW_ONE_EXCL,    // 创建一个新IPC，如果已经存在，则返回失败
		GET_EXIST_ONE,          // 取得已经存在的IPC，如果不存在，则返回失败
	} IPC_CREATE_TYPE_E;

	static const int IPC_DEFAULT_PERM = 0666;

	typedef enum permission_e
	{
		RW,
		READ,
		WRITE,
	} PERMISSION_E;

	typedef void (*SIGHANDLER_PFT)(int);

	typedef pthread_t		THREAD_ID_T;
	typedef pthread_key_t	THREAD_KEY_T;
	typedef pid_t			PID_T;
	typedef int				SOCKET_FD_T;
	typedef fd_set			SOCKET_SET_T;
	typedef std::vector <PID_T> PIDList;
	typedef std::vector <IPCID_T> IPCIDList;
	typedef std::vector <SOCKET_FD_T> SOCKETFDList;
};

#endif // end of LWPR_LWPR_TYPE_H__
