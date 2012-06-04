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
#ifndef TUX_ATMIBUFFER_MANAGER_H__
#define TUX_ATMIBUFFER_MANAGER_H__

#include <map>
#include <string>
#include <lwpr.h>
#include "Message.h"

typedef struct atmi_buffer_item_t
{
	int nCurrentSize;				// 当前Buffer大小
	int nActualSize;				// 当前内存块实际大小
	TYPED_BUFFER_E eBufferType;	// 当前内存块类型
	std::string strSubType;			// 当前内存块子类型
	bool bIsInUse;					// 当前内存块是否正在使用
} ATMI_BUFFER_ITEM_T;

class ATMIBufferManager
{
public:
	/**
	 * 构造函数
	 */
	ATMIBufferManager();

	/**
	 * 析构函数
	 */
	~ATMIBufferManager();

	/**
	 * 单例函数
	 */
	static ATMIBufferManager* GetInstance();

	/**
	 * 分配内存块
	 */
	char* AllocateBuffer(char *type, char *subtype, long size);

	/**
	 * 重新分配内存块
	 */
	char* ReallocateBuffer(char *ptr, long size);

	/**
	 * 获取内存块类型
	 */
	long GetBufferTypes(char *ptr, char *type, char *subtype);

	bool GetBufferTypes(char *ptr, TYPED_BUFFER_E& type);

	bool GetBufferTypes(char *ptr, TYPED_BUFFER_E& type, std::string& subtype);

	/**
	 * 释放内存块
	 */
	void FreeBuffer(const char *ptr);

private:
	typedef std::map< char*, ATMI_BUFFER_ITEM_T > ATMIBufferMap;

	LWPR::Mutex		m_Mutex;
	ATMIBufferMap	m_BufferList;
	int				m_nBufferTotalSize;
};

#endif // end of TUX_ATMIBUFFER_MANAGER_H__
