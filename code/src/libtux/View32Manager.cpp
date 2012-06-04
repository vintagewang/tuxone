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
#include "View32Manager.h"


View32Manager::View32Manager()
{
}

View32Manager::~View32Manager()
{
}

View32Manager* View32Manager::GetInstance()
{
	static View32Manager *singleton = new View32Manager();
	return singleton;
}

void View32Manager::LoadStruct()
{
	std::vector< std::string > tbls; // .fml文件 VIEWFILES32
	std::vector< std::string > dirs; // .fml文件 VIEWDIR32

	const char* pEnv = getenv("VIEWFILES32");
	if(pEnv != NULL)
	{
		LWPR::StringUtil::SplitString(pEnv, ',', tbls);
	}

	pEnv = getenv("VIEWDIR32");
	if(pEnv != NULL)
	{
		LWPR::StringUtil::SplitString(pEnv, ':', dirs);
	}

	for(size_t i = 0; i < dirs.size(); i++)
	{
		for(size_t k = 0; k < tbls.size(); k++)
		{
			std::string strFile = dirs[i] + "/" + tbls[k];
			if(LWPR::FileUtil::IsFileExist(strFile.c_str()))
			{
				FILE_INFO_T info;
				if(View32Paser::LoadViewStruct(strFile.c_str(), info))
				{
					for(size_t m = 0; m < info.structs.size(); m++)
					{
						m_Structs[info.structs[m].structName] = info.structs[m];
						logger->debug(LTRACE, "Load view32 struct [%s] [%s] OK.", strFile.c_str(), info.structs[m].structName.c_str());
					}
				}
				else
				{
					logger->warn(LTRACE, "Load view32 file [%s] error.", strFile.c_str());
				}
			}
		}
	}
}

bool View32Manager::IsStructExist(const char* name, int& size)
{
	assert(NULL != name);

	StructInfoMap::iterator it = m_Structs.find(name);
	if(m_Structs.end() != it)
	{
		size = it->second.structSize;
		return true;
	}

	return false;
}

bool View32Manager::GetStructInfo(const char* name, STRUCT_INFO_T& info)
{
	assert(NULL != name);

	StructInfoMap::iterator it = m_Structs.find(name);
	if(m_Structs.end() != it)
	{
		info = it->second;
		return true;
	}

	return false;
}

bool View32Manager::AdjustStructData(const char* name,
                                     const void* indata,
                                     const STRUCT_INFO_T& remoteinfo,
                                     const bool inbigendian,
                                     LWPR::Buffer& outdata)
{
	assert(NULL != name);

	StructInfoMap::iterator it = m_Structs.find(name);
	if(m_Structs.end() != it)
	{
		STRUCT_INFO_T& localinfo = it->second;

		if(localinfo.items.size() != remoteinfo.items.size())
		{
			return false;
		}

		// 设置生成结构体的大小
		outdata.Size(localinfo.structSize);

		for(size_t i = 0; i < localinfo.items.size(); i++)
		{
			logger->debug(LTRACE, "%s %s local offset %d, remote offset %d"
			              , localinfo.structName.c_str()
			              , localinfo.items[i].cname.c_str()
			              , localinfo.items[i].offset
			              , remoteinfo.items[i].offset);

			switch(localinfo.items[i].type)
			{
			case STRUCT_DATA_TYPE_FLOAT:
			case STRUCT_DATA_TYPE_DOUBLE:
			case STRUCT_DATA_TYPE_STRING:
			case STRUCT_DATA_TYPE_CHAR:
				memcpy(outdata.Inout() + localinfo.items[i].offset,
				       (char *)indata + remoteinfo.items[i].offset,
				       remoteinfo.items[i].size * remoteinfo.items[i].count);
				break;
			case STRUCT_DATA_TYPE_SHORT:
				{
					for(LWPR::UINT32  k = 0; k < localinfo.items[i].count; k++)
					{
						short value = 0;
						memcpy(&value,
						       (char *)indata + remoteinfo.items[i].offset + k * remoteinfo.items[i].size ,
						       remoteinfo.items[i].size);

						// 字节序转化
						value = LWPR::Utility::IntegerToLocalByte16(value, inbigendian);


						memcpy((char *)outdata.Inout() + localinfo.items[i].offset + k * localinfo.items[i].size ,
						       &value,
						       localinfo.items[i].size);
					}
				}
				break;
			case STRUCT_DATA_TYPE_INT:
				{
					for(LWPR::UINT32  k = 0; k < localinfo.items[i].count; k++)
					{
						int value = 0;
						memcpy(&value,
						       (char *)indata + remoteinfo.items[i].offset + k * remoteinfo.items[i].size ,
						       remoteinfo.items[i].size);

						// 字节序转化
						value = LWPR::Utility::IntegerToLocalByte32(value, inbigendian);

						memcpy((char *)outdata.Inout() + localinfo.items[i].offset + k * localinfo.items[i].size ,
						       &value,
						       localinfo.items[i].size);
					}
				}
				break;
			case STRUCT_DATA_TYPE_LONG:
				{
					for(LWPR::UINT32  k = 0; k < localinfo.items[i].count; k++)
					{
						long value = 0;

						if(4 == remoteinfo.items[i].size)
						{
							LWPR::UINT32 value32 = 0;
							memcpy(&value32,
							       (char *)indata + remoteinfo.items[i].offset + k * remoteinfo.items[i].size ,
							       remoteinfo.items[i].size);

							// 字节序转化
							value = (long)LWPR::Utility::IntegerToLocalByte32(value32, inbigendian);
						}
						else if(8 == remoteinfo.items[i].size)
						{
							LWPR::UINT64 value64 = 0;
							memcpy(&value64,
							       (char *)indata + remoteinfo.items[i].offset + k * remoteinfo.items[i].size ,
							       remoteinfo.items[i].size);

							// 字节序转化
							value = (long)LWPR::Utility::IntegerToLocalByte64(value64, inbigendian);
						}

						memcpy((char *)outdata.Inout() + localinfo.items[i].offset + k * localinfo.items[i].size ,
						       &value,
						       localinfo.items[i].size);
					}
				}
				break;
			default:
				return false;
			}
		}

		return true;
	}

	return false;
}
