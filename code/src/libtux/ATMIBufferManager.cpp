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
#include "ATMIBufferManager.h"
#include "AppConfig.h"
#include "View32Manager.h"
#include <fml32.h>
#include <atmi.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


ATMIBufferManager::ATMIBufferManager() :
	m_nBufferTotalSize(0)
{
}

ATMIBufferManager::~ATMIBufferManager()
{
}

ATMIBufferManager* ATMIBufferManager::GetInstance()
{
	static ATMIBufferManager *singleton = new ATMIBufferManager();
	return singleton;
}

char* ATMIBufferManager::AllocateBuffer(char *type, char *subtype, long size)
{
	// 参数检查
	if(type == NULL || size < 0)
	{
		tperrno = TPEINVAL;
		return NULL;
	}

	TYPED_BUFFER_E eBufferType = TYPED_BUFFER_CARRAY;

	if(!strcmp(type, "CARRAY"))
	{
		if(size <= 0)
		{
			tperrno = TPEINVAL;
			return NULL;
		}

		eBufferType = TYPED_BUFFER_CARRAY;
	}
	else if(!strcmp(type, "XML"))
	{
		if(size <= 0)
		{
			tperrno = TPEINVAL;
			return NULL;
		}

		eBufferType = TYPED_BUFFER_XML;
	}
	else if(!strcmp(type, "STRING"))
	{
		if(size < 512)
		{
			size = 512;
		}

		eBufferType = TYPED_BUFFER_STRING;
	}
	else if(!strcmp(type, "VIEW32"))
	{
		if(size < 1024)
		{
			size = 1024;
		}

		if(subtype == NULL || strlen(subtype) == 0)
		{
			tperrno = TPEINVAL;
			return NULL;
		}

		int dfltsize = 0;
		if(!View32Manager::GetInstance()->IsStructExist(subtype, dfltsize))
		{
			tperrno = TPENOENT;
			return NULL;
		}

		// If subtype is non-NULL in tmtype_sw for a particular buffer type, then subtype must be specified when tpalloc() is called.
		// The allocated buffer will be at least as large as the larger of size and dfltsize,
		// where dfltsize is the default buffer size specified in tmtype_sw for the particular buffer type.
		// For buffer type STRING the minimum is 512 bytes; for buffer types FML and VIEW the minimum is 1024 bytes.
		size = size > dfltsize ? size : dfltsize;

		eBufferType = TYPED_BUFFER_VIEW32;
	}
	else if(!strcmp(type, "FML32"))
	{
		if(size < 1024)
		{
			size = 1024;
		}

		eBufferType = TYPED_BUFFER_FML32;
	}
	else if(!strcmp(type, "MBSTRING"))
	{
		if(size < 1024)
		{
			size = 1024;
		}

		eBufferType = TYPED_BUFFER_MBSTRING;
	}
	else
	{
		tperrno = TPENOENT;
		return NULL;
	}

	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.begin();
	for(; it != m_BufferList.end(); it++)
	{
		if(!it->second.bIsInUse
		   && size <= it->second.nActualSize)
		{
			it->second.bIsInUse = true;
			it->second.nCurrentSize = size;
			it->second.eBufferType = eBufferType;

			if(eBufferType == TYPED_BUFFER_VIEW32)
			{
				it->second.strSubType = subtype;
			}
			else if(eBufferType == TYPED_BUFFER_FML32)
			{
				Finit32(it->first, size);
			}

			return it->first;
		}
	}

	// 创建新的Buffer
	char* pBuffer = (char *)malloc(size);
	if(pBuffer == NULL)
	{
		tperrno = TPEOS;
		return NULL;
	}

	ATMI_BUFFER_ITEM_T item;
	item.nCurrentSize = size;
	item.nActualSize = size;
	item.eBufferType = eBufferType;
	item.bIsInUse = true;
	if(eBufferType == TYPED_BUFFER_VIEW32)
	{
		item.strSubType = subtype;
	}
	else if(eBufferType == TYPED_BUFFER_FML32)
	{
		Finit32(pBuffer, size);
	}

	m_BufferList[pBuffer] = item;
	m_nBufferTotalSize += item.nActualSize;
	return pBuffer;
}

char* ATMIBufferManager::ReallocateBuffer(char *ptr, long size)
{
	// 参数检查
	if(ptr == NULL
	   || size <= 0)
	{
		tperrno = TPEINVAL;
		return NULL;
	}

	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.find(ptr);
	if(it != m_BufferList.end())
	{
		if(it->second.bIsInUse)
		{
			// 调整Buffer大小
			if(it->second.nActualSize < size)
			{
				// 调整原有内存块大小
				char* pBuffer = (char *)realloc(it->first, size);
				if(pBuffer == NULL)
				{
					tperrno = TPEOS;
					return NULL;
				}

				// 分配成功情况下，修改Buffer总大小
				m_nBufferTotalSize -= it->second.nActualSize;

				// 新内存块只是从原有内存块后面扩大情况
				if(pBuffer == it->first)
				{
					it->second.nCurrentSize = size;
					it->second.nActualSize = size;
				}
				// 完全重新分配了一块新的内存
				else
				{
					m_BufferList.erase(it);

					ATMI_BUFFER_ITEM_T item = it->second;
					item.nActualSize = size;
					item.nCurrentSize = size;

					m_BufferList[pBuffer] = item;
				}

				// 记录Buffer总大小
				m_nBufferTotalSize += size;

				return pBuffer;
			}
			else
			{
				return ptr;
			}
		}
	}

	// 没有找到之前的Buffer
	tperrno = TPEOS;
	return NULL;
}

long ATMIBufferManager::GetBufferTypes(char *ptr, char *type, char *subtype)
{
	// 参数检查
	if(ptr == NULL
	   || type == NULL
	   || subtype == NULL)
	{
		tperrno = TPEINVAL;
		return -1;
	}

	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.find(ptr);
	if(it != m_BufferList.end())
	{
		if(it->second.bIsInUse)
		{
			switch(it->second.eBufferType)
			{
			case TYPED_BUFFER_CARRAY:
				strcpy(type, "CARRAY");
				break;
			case TYPED_BUFFER_STRING:
				strcpy(type, "STRING");
				break;
			case TYPED_BUFFER_MBSTRING:
				strcpy(type, "MBSTRING");
				break;
			case TYPED_BUFFER_VIEW32:
				strcpy(type, "VIEW32");
				strcpy(subtype, it->second.strSubType.c_str());
				break;
			case TYPED_BUFFER_FML32:
				strcpy(type, "FML32");
				break;
			case TYPED_BUFFER_XML:
				strcpy(type, "XML");
				break;
			default:
				assert(0);
			}

			return it->second.nCurrentSize;
		}
	}

	tperrno = TPEINVAL;
	return -1;
}

bool ATMIBufferManager::GetBufferTypes(char *ptr, TYPED_BUFFER_E& type)
{
	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.find(ptr);
	if(it != m_BufferList.end())
	{
		if(it->second.bIsInUse)
		{
			type = it->second.eBufferType;
			return true;
		}
	}

	return false;
}

bool ATMIBufferManager::GetBufferTypes(char *ptr, TYPED_BUFFER_E& type, std::string& subtype)
{
	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.find(ptr);
	if(it != m_BufferList.end())
	{
		if(it->second.bIsInUse)
		{
			type = it->second.eBufferType;
			subtype = it->second.strSubType;
			return true;
		}
	}

	return false;
}

void ATMIBufferManager::FreeBuffer(const char *ptr)
{
	// 参数检查
	if(ptr == NULL)
	{
		return;
	}

	// 同步控制
	LWPR::Synchronized syn(m_Mutex);

	// 从已有Buffer列表中查找
	ATMIBufferMap::iterator it = m_BufferList.find((char *)ptr);
	if(it != m_BufferList.end())
	{
		if(it->second.bIsInUse)
		{
			if(m_nBufferTotalSize > AppConfig::GetInstance()->TUXONE_APP_BUFFER_CACHEDSIZE())
			{
				m_nBufferTotalSize -= it->second.nActualSize;
				free(it->first);
				m_BufferList.erase(it);
			}
			else
			{
				it->second.bIsInUse = false;
			}
		}
	}
}
