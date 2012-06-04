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
#include "BulletinBoard.h"
#include <lwpr.h>
#include <assert.h>
#include <string.h>

//----------------------------------------------------------------
// bb_header_entity_t
//----------------------------------------------------------------
bb_header_entity_t::bb_header_entity_t()
{

}

void* bb_header_entity_t::operator new(size_t size, void* addr)
{
	assert(addr != NULL);
	return addr;
}

void* bb_header_entity_t::operator new(size_t size)
{
	return ::operator new(size);
}
//----------------------------------------------------------------
// bb_wsh_entity_t
//----------------------------------------------------------------
void* bb_wsh_entity_t::operator new[](size_t size, void* addr)
{
	assert(addr != NULL);
	return addr;
}

void* bb_wsh_entity_t::operator new[](size_t size)
{
	return ::operator new[](size);
}

bb_wsh_entity_t::bb_wsh_entity_t()
{
}

bb_wsh_entity_t::bb_wsh_entity_t(const bb_wsh_entity_t& rhs)
{
	*this = rhs;
}

bb_wsh_entity_t& bb_wsh_entity_t::operator=(const bb_wsh_entity_t& rhs)
{
	if(this !=  &rhs)
	{
		nIndex = rhs.nIndex;
		nPID = rhs.nPID;
		nConnectionTotal = rhs.nConnectionTotal;
		nPort = rhs.nPort;
		nSlotState = rhs.nSlotState;
	}

	return *this;
}

//----------------------------------------------------------------
// bb_service_entity_t
//----------------------------------------------------------------
void* bb_service_entity_t::operator new[](size_t size, void* addr)
{
	assert(addr != NULL);
	return addr;
}

void* bb_service_entity_t::operator new[](size_t size)
{
	return ::operator new[](size);
}

bb_service_entity_t::bb_service_entity_t()
{
}

bb_service_entity_t::bb_service_entity_t(const bb_service_entity_t& rhs)
{
	*this = rhs;
}

bb_service_entity_t& bb_service_entity_t::operator=(const bb_service_entity_t& rhs)
{
	if(this !=  &rhs)
	{
		nIndex = rhs.nIndex;
		memcpy(strSvcName, rhs.strSvcName, sizeof(rhs.strSvcName));
		memcpy(&tServerAddr, &rhs.tServerAddr, sizeof(rhs.tServerAddr));
		nBelongto = rhs.nBelongto;
		nSlotState = rhs.nSlotState;
	}

	return *this;
}

//----------------------------------------------------------------
// bb_stream_t
//----------------------------------------------------------------
bb_stream_t::bb_stream_t():
	pHeader(NULL),
	tblWSH(NULL),
	tblWSHLength(0),
	tblSVC(NULL),
	tblSVCLength(0)
{
}

bb_stream_t::~bb_stream_t()
{
	delete pHeader;
	delete[] tblWSH;
	delete[] tblSVC;
}

void bb_stream_t::Marshal(LWPR::OutputStream& out)
{
	// pHeader
	out.WriteUINT32(pHeader->nMagicCode);
	out.WriteUINT32(pHeader->nSEMID);
	out.WriteUINT32(pHeader->nSVCUpdateSeq);
	out.WriteUINT32(pHeader->nGatePID);
	out.WriteUINT16(pHeader->nUDPPort);
	out.WriteUINT32(pHeader->nWSHMaxLength);
	out.WriteUINT32(pHeader->nSVCMaxLength);
	pHeader->tServerAddr.Marshal(out);

	// tblWSH
	out.WriteINT32(tblWSHLength);
	for(LWPR::INT32 i = 0; i < tblWSHLength; i++)
	{
		out.WriteINT32(tblWSH[i].nIndex);
		out.WriteINT32(tblWSH[i].nPID);
		out.WriteINT32(tblWSH[i].nConnectionTotal);
		out.WriteUINT16(tblWSH[i].nPort);
		out.WriteINT32((LWPR::INT32)tblWSH[i].nSlotState);
	}

	// tblSVC
	out.WriteINT32(tblSVCLength);
	for(LWPR::INT32 k = 0; k < tblSVCLength; k++)
	{
		out.WriteINT32(tblSVC[k].nIndex);
		out.WriteArray(tblSVC[k].strSvcName, sizeof(tblSVC[k].strSvcName));
		tblSVC[k].tServerAddr.Marshal(out);
		out.WriteINT32((LWPR::INT32)tblSVC[k].nBelongto);
		out.WriteINT32((LWPR::INT32)tblSVC[k].nSlotState);
	}
}

void bb_stream_t::Unmarshal(LWPR::InputStream& in)
{
	// pHeader
	pHeader = new bb_header_entity_t();
	pHeader->nMagicCode = in.ReadUINT32();
	pHeader->nSEMID = in.ReadUINT32();
	pHeader->nSVCUpdateSeq = in.ReadUINT32();
	pHeader->nGatePID = in.ReadUINT32();
	pHeader->nUDPPort = in.ReadUINT16();
	pHeader->nWSHMaxLength = in.ReadUINT32();
	pHeader->nSVCMaxLength = in.ReadUINT32();
	pHeader->tServerAddr.Unmarshal(in);

	// tblWSH
	tblWSHLength = in.ReadINT32();
	if(tblWSHLength > 0)
	{
		tblWSH = new BB_WSH_ENTITY_T[tblWSHLength];
		for(LWPR::INT32 i = 0; i < tblWSHLength; i++)
		{
			tblWSH[i].nIndex = in.ReadINT32();
			tblWSH[i].nPID = in.ReadINT32();
			tblWSH[i].nConnectionTotal = in.ReadINT32();
			tblWSH[i].nPort = in.ReadUINT16();
			tblWSH[i].nSlotState = (BBSLOT_STATE_E)in.ReadINT32();
		}
	}

	// tblSVC
	tblSVCLength = in.ReadINT32();
	if(tblSVCLength > 0)
	{
		tblSVC = new BB_SERVICE_ENTITY_T[tblSVCLength];
		for(LWPR::INT32 k = 0; k < tblSVCLength; k++)
		{
			tblSVC[k].nIndex = in.ReadINT32();
			in.ReadArray(tblSVC[k].strSvcName);
			tblSVC[k].tServerAddr.Unmarshal(in);
			tblSVC[k].nBelongto = in.ReadINT32();
			tblSVC[k].nSlotState = (BBSLOT_STATE_E)in.ReadINT32();
		}
	}
}

BulletinBoard::BulletinBoard():
	m_nSHMID(-1),
	m_pSHMAddr((void *) - 1),
	m_nSEMID(-1),
	m_pHeader(0),
	m_tblWSH(0),
	m_tblWSHLength(0),
	m_tblSVC(0),
	m_tblSVCLength(0),
	m_nWSHIndex(-1)
{

}

BulletinBoard::~BulletinBoard()
{

}

BulletinBoard* BulletinBoard::GetInstance()
{
	static BulletinBoard *singleton = new BulletinBoard();
	return singleton;
}

void BulletinBoard::Print()
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	// 打印header
	printf("nMagicCode [%d]\n", m_pHeader->nMagicCode);
	printf("nSEMID [%d]\n", m_pHeader->nSEMID);
	printf("nSVCUpdateSeq [%d]\n", m_pHeader->nSVCUpdateSeq);
	printf("nGatePID [%d]\n", m_pHeader->nGatePID);
	printf("nUDPPort [%d]\n", m_pHeader->nUDPPort);
	printf("nWSHMaxLength [%d]\n", m_pHeader->nWSHMaxLength);
	printf("nSVCMaxLength [%d]\n", m_pHeader->nSVCMaxLength);
	printf("tServerAddr [%s]\n", LWPR::Socket::AddrTransform(m_pHeader->tServerAddr).c_str());

	// 打印wsh表
	printf("-------------------------------------------------------------------\n");
	for(int i = 0; i < m_tblWSHLength; i++)
	{
		if(m_tblWSH[i].nSlotState != BBSLOT_IDLE)
		{
			printf("WSH TABLE [%d] nIndex [%d]\n", i, m_tblWSH[i].nIndex);
			printf("WSH TABLE [%d] nPID [%d]\n", i, m_tblWSH[i].nPID);
			printf("WSH TABLE [%d] nConnectionTotal [%d]\n", i, m_tblWSH[i].nConnectionTotal);
			printf("WSH TABLE [%d] nPort [%d]\n", i, m_tblWSH[i].nPort);
			printf("WSH TABLE [%d] nSlotState [%d]\n\n", i, m_tblWSH[i].nSlotState);
		}
	}

	// 打印service表
	printf("-------------------------------------------------------------------\n");
	for(int i = 0; i < m_tblSVCLength; i++)
	{
		if(m_tblSVC[i].nSlotState != BBSLOT_IDLE)
		{
			printf("SERVICE TABLE [%d] nIndex [%d]\n", i, m_tblSVC[i].nIndex);
			printf("SERVICE TABLE [%d] strSvcName [%s]\n", i, m_tblSVC[i].strSvcName);
			std::string addr;
			LWPR::Socket::AddrTransform(m_tblSVC[i].tServerAddr, addr);
			printf("SERVICE TABLE [%d] tServerAddr [%s]\n", i, addr.c_str());
			printf("SERVICE TABLE [%d] nBelongto [%d]\n", i, m_tblSVC[i].nBelongto);
			printf("SERVICE TABLE [%d] nSlotState [%d]\n\n", i, m_tblSVC[i].nSlotState);
		}
	}
}

void BulletinBoard::Create(int wsh_len, int svc_len)
{
	// 初始化变量
	m_tblWSHLength = wsh_len;
	m_tblSVCLength = svc_len;

	// 创建共享内存对象
	m_nSHMID = LWPR::IPCSHM::Create(IPC_PRIVATE, ComputeShmSize());

	// 创建信号灯对象
	m_nSEMID = LWPR::IPCSEM::Create(IPC_PRIVATE);

	// 挂载共享内存到当前进程
	m_pSHMAddr = LWPR::IPCSHM::Mount(m_nSHMID);

	// 在共享内存中建立Header
	char *pSHMAddr = (char*)m_pSHMAddr;
	m_pHeader = new(pSHMAddr) BB_HEADER_ENTITY_T();
	pSHMAddr = pSHMAddr + sizeof(BB_HEADER_ENTITY_T);

	// 在共享内存中建立WSH
	m_tblWSH = new(pSHMAddr) BB_WSH_ENTITY_T[m_tblWSHLength];
	pSHMAddr = pSHMAddr + sizeof(BB_WSH_ENTITY_T) * m_tblWSHLength;

	// 在共享内存中建立SVC
	m_tblSVC = new(pSHMAddr) BB_SERVICE_ENTITY_T[m_tblSVCLength];

	// 设置当前WSH进程内的公告板变更号
	m_nSVCUpdateSeq = m_pHeader->nSVCUpdateSeq;

	// 初始化公告板头
	m_pHeader->nWSHMaxLength = m_tblWSHLength;
	m_pHeader->nSVCMaxLength = m_tblSVCLength;
}

void BulletinBoard::Connect(LWPR::IPCID_T shmid)
{
	// 注意：	父进程已经挂载了共享内存，子进程就不需要重新挂载了。
	//			如果子进程在父进程挂载情况下又挂载一次，则父子进程无法通信
	// 创建共享内存对象
	m_nSHMID = shmid;

	// 挂载共享内存到当前进程
	m_pSHMAddr = LWPR::IPCSHM::Mount(m_nSHMID);

	// 在共享内存中建立Header
	char *pSHMAddr = (char*)m_pSHMAddr;
	m_pHeader = new(pSHMAddr) BB_HEADER_ENTITY_T();
	pSHMAddr = pSHMAddr + sizeof(BB_HEADER_ENTITY_T);

	// 初始化变量
	m_nSEMID = m_pHeader->nSEMID;
	m_tblWSHLength = m_pHeader->nWSHMaxLength;
	m_tblSVCLength = m_pHeader->nSVCMaxLength;

	// 在共享内存中建立WSH
	m_tblWSH = new(pSHMAddr) BB_WSH_ENTITY_T[m_tblWSHLength];
	pSHMAddr = pSHMAddr + sizeof(BB_WSH_ENTITY_T) * m_tblWSHLength;

	// 在共享内存中建立SVC
	m_tblSVC = new(pSHMAddr) BB_SERVICE_ENTITY_T[m_tblSVCLength];

	// 设置本地公告板变更号为0
	m_nSVCUpdateSeq = 0;
}

void BulletinBoard::Disconnect()
{
	if(m_pSHMAddr != (void *) - 1)
	{
		LWPR::IPCSHM::Unmount(m_pSHMAddr);
	}
}

void BulletinBoard::Destroy()
{
	Disconnect();

	LWPR::IPCSEM::Destroy(m_nSEMID);
	LWPR::IPCSHM::Destroy(m_nSHMID);
}

bool BulletinBoard::RegisterWSH(BB_WSH_ENTITY_T& wsh)
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	bool bResult = false;

	for(int i = 0; i < m_tblWSHLength; i++)
	{
		if(m_tblWSH[i].nSlotState == BBSLOT_IDLE)
		{
			wsh.nIndex = i;
			wsh.nConnectionTotal = 0;
			wsh.nSlotState = BBSLOT_INUSE;
			m_tblWSH[i] = wsh;
			m_nWSHIndex = i;
			bResult = true;
			break;
		}
	}

	return bResult;
}

void BulletinBoard::CleanupWSHByIndex(const BBIndexList& wsh_indexs)
{
	if(wsh_indexs.size() > 0)
	{
		// 信号灯加锁
		LWPR::Synchronized syn(m_nSEMID);

		for(size_t i = 0; i < wsh_indexs.size(); i++)
		{
			m_tblWSH[wsh_indexs[i]].nSlotState = BBSLOT_IDLE;
		}
	}
}

void BulletinBoard::CleanupWSHByPID(const LWPR::PIDList& pids)
{
	if(pids.size() > 0)
	{
		// 信号灯加锁
		LWPR::Synchronized syn(m_nSEMID);

		for(size_t i = 0; i < pids.size(); i++)
		{
			for(int k = 0; k < m_tblWSHLength; k++)
			{
				if(m_tblWSH[k].nPID == pids[i])
				{
					m_tblWSH[k].nSlotState = BBSLOT_IDLE;
				}
			}
		}
	}
}

void BulletinBoard::UnregisterWSH()
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	if(m_nWSHIndex >= 0 && m_nWSHIndex < m_tblWSHLength)
	{
		m_tblWSH[m_nWSHIndex].nSlotState = BBSLOT_IDLE;
	}
}

void BulletinBoard::UpdateWSHConnectionTotal(bool increase)
{
	if(m_nWSHIndex >= 0 && m_nWSHIndex < m_tblWSHLength)
	{
		if(increase)
		{
			LWPR::AtomicInteger::IncrementAndGet(&m_tblWSH[m_nWSHIndex].nConnectionTotal);
		}
		else
		{
			LWPR::AtomicInteger::DecrementAndGet(&m_tblWSH[m_nWSHIndex].nConnectionTotal);
		}
	}
}

int BulletinBoard::GetAllWSH(BBWSHList& wshs)
{
	wshs.clear();

	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	int nCount = 0;

	for(int i = 0; i < m_tblWSHLength; i++)
	{
		if(m_tblWSH[i].nSlotState == BBSLOT_INUSE)
		{
			wshs.push_back(m_tblWSH[i]);
			nCount++;
		}
	}

	return nCount;
}

int BulletinBoard::GetOtherWSH(BBWSHList& wshs)
{
	wshs.clear();

	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	int nCount = 0;

	for(int i = 0; i < m_tblWSHLength; i++)
	{
		if(m_tblWSH[i].nSlotState == BBSLOT_INUSE && i != m_nWSHIndex)
		{
			wshs.push_back(m_tblWSH[i]);
			nCount++;
		}
	}

	return nCount;
}

bool BulletinBoard::RegisterService(BB_SERVICE_ENTITY_T& svc)
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	bool bResult = false;

	int nFirstIdleIndex = -1;
	for(int i = 0; i < m_tblSVCLength; i++)
	{
		// 先找一个空的坑位
		if(m_tblSVC[i].nSlotState == BBSLOT_IDLE && -1 == nFirstIdleIndex)
		{
			nFirstIdleIndex = i;
		}
		// 再找是否已经注册过
		else if(m_tblSVC[i].nSlotState != BBSLOT_IDLE
		        && m_tblSVC[i].nBelongto == BBSVC_ENTITY_BELONG_TO_SELF
		        && !memcmp(&m_tblSVC[i].tServerAddr, &svc.tServerAddr, sizeof(LWPR::SOCKET_ADDR_T))
		        && !strncmp(m_tblSVC[i].strSvcName, svc.strSvcName, TUXONE_SERVICE_NAME_LENGTH))
		{
			svc.nIndex = i;

			// 如果是其它状态，如隔离状态
			if(m_tblSVC[i].nSlotState != BBSLOT_INUSE)
			{
				m_pHeader->nSVCUpdateSeq++;
				m_tblSVC[i].nSlotState = BBSLOT_INUSE;
			}

			bResult = true;
			break;
		}
	}

	if(!bResult && -1 != nFirstIdleIndex)
	{
		svc.nIndex = nFirstIdleIndex;
		svc.nSlotState = BBSLOT_INUSE;
		svc.nBelongto = BBSVC_ENTITY_BELONG_TO_SELF;
		m_tblSVC[nFirstIdleIndex] = svc;
		bResult = true;
		m_pHeader->nSVCUpdateSeq++;
	}

	return bResult;
}

void BulletinBoard::UnregisterService(const BBSVCList& svcs)
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	bool bUpdated = false;

	for(size_t i = 0; i < svcs.size(); i++)
	{
		for(int k = 0; k < m_tblSVCLength; k++)
		{
			if(!memcmp(&svcs[i].tServerAddr, &m_tblSVC[k].tServerAddr, sizeof(LWPR::SOCKET_ADDR_T))
			   && !strncmp(svcs[i].strSvcName, m_tblSVC[k].strSvcName, TUXONE_SERVICE_NAME_LENGTH)
			   && BBSVC_ENTITY_BELONG_TO_SELF == m_tblSVC[i].nBelongto)
			{
				m_tblSVC[k].nSlotState = BBSLOT_IDLE;
				bUpdated = true;
			}
		}
	}

	if(bUpdated)
	{
		m_pHeader->nSVCUpdateSeq++;
	}
}

void BulletinBoard::UnregisterService(const LWPR::SOCKET_ADDR_T& addr)
{
	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	bool bUpdated = false;

	for(int i = 0; i < m_tblSVCLength; i++)
	{
		if(!memcmp(&addr, &m_tblSVC[i].tServerAddr, sizeof(LWPR::SOCKET_ADDR_T))
		   && BBSVC_ENTITY_BELONG_TO_SELF == m_tblSVC[i].nBelongto)
		{
			m_tblSVC[i].nSlotState = BBSLOT_IDLE;
			bUpdated = true;
		}
	}

	if(bUpdated)
	{
		m_pHeader->nSVCUpdateSeq++;
	}
}

LWPR::UINT32 BulletinBoard::GetAllSVC(const std::vector<std::string>& names, BBSVCList& svcs)
{
	svcs.clear();

	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	for(size_t i = 0; i < names.size(); i++)
	{
		for(int k = 0; k < m_tblSVCLength; k++)
		{
			if(m_tblSVC[k].nSlotState == BBSLOT_INUSE
			   && !strncmp(m_tblSVC[k].strSvcName, names[i].c_str(), TUXONE_SERVICE_NAME_LENGTH))
			{
				svcs.push_back(m_tblSVC[k]);
			}
		}
	}

	return m_pHeader->nSVCUpdateSeq;
}

int BulletinBoard::ComputeShmSize()
{
	int size = 0;

	size += sizeof(BB_HEADER_ENTITY_T);

	size += sizeof(BB_WSH_ENTITY_T) * m_tblWSHLength;

	size += sizeof(BB_SERVICE_ENTITY_T) * m_tblSVCLength;

	size *= 2;

	return size;
}

LWPR::UINT32 BulletinBoard::GetSVCUpdateSeq()
{
	return m_pHeader->nSVCUpdateSeq;
}

bool BulletinBoard::IsSvcUpdated()
{
	if(m_pHeader->nSVCUpdateSeq != m_nSVCUpdateSeq)
	{
		m_nSVCUpdateSeq = m_pHeader->nSVCUpdateSeq;
		return true;
	}

	return false;
}

void BulletinBoard::GetBBStream(BB_STREAM_T& stream, bool fetchall /*= true*/)
{
	// 清空参数
	memset(&stream, 0, sizeof(stream));

	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	// pHeader
	stream.pHeader = new BB_HEADER_ENTITY_T();
	memcpy(stream.pHeader, m_pHeader, sizeof(BB_HEADER_ENTITY_T));

	// tblWSHLength
	for(int m = 0; m < m_tblWSHLength; m++)
	{
		if(m_tblWSH[m].nSlotState != BBSLOT_IDLE)
		{
			stream.tblWSHLength++;
		}
	}

	// tblWSH
	if(stream.tblWSHLength > 0)
	{
		stream.tblWSH = new BB_WSH_ENTITY_T[stream.tblWSHLength];

		for(int k = 0, i = 0; k < m_tblWSHLength; k++)
		{
			if(m_tblWSH[k].nSlotState != BBSLOT_IDLE)
			{
				stream.tblWSH[i++] = m_tblWSH[k];
			}
		}
	}

	// tblSVCLength
	for(int n = 0; n < m_tblSVCLength; n++)
	{
		if(m_tblSVC[n].nSlotState != BBSLOT_IDLE
		   && (fetchall || BBSVC_ENTITY_BELONG_TO_SELF == m_tblSVC[n].nBelongto))
		{
			stream.tblSVCLength++;
		}
	}

	// tblSVC
	if(stream.tblSVCLength > 0)
	{
		stream.tblSVC = new BB_SERVICE_ENTITY_T[stream.tblSVCLength];

		for(int k = 0, i = 0; k < m_tblSVCLength; k++)
		{
			if(m_tblSVC[k].nSlotState != BBSLOT_IDLE
			   && (fetchall || BBSVC_ENTITY_BELONG_TO_SELF == m_tblSVC[k].nBelongto))
			{
				stream.tblSVC[i++] = m_tblSVC[k];
			}
		}
	}
}

void BulletinBoard::SetHeader(const BB_HEADER_ENTITY_T& header)
{
	memcpy(m_pHeader, &header, sizeof(BB_HEADER_ENTITY_T));
	m_pHeader->nSEMID = m_nSEMID;
	m_pHeader->nWSHMaxLength = m_tblWSHLength;
	m_pHeader->nSVCMaxLength = m_tblSVCLength;
}

void BulletinBoard::GetHeader(BB_HEADER_ENTITY_T& header)
{
	memcpy(&header, m_pHeader, sizeof(BB_HEADER_ENTITY_T));
}

bool BulletinBoard::SynBB(const BB_STREAM_T& stream, LWPR::INT32 index)
{
	bool bUpdated = false;

	// 信号灯加锁
	LWPR::Synchronized syn(m_nSEMID);

	// 查找当前gate已经注册了的svc
	BBSVCList svcs_same;
	for(int i = 0; i < m_tblSVCLength; i++)
	{
		if(BBSLOT_IDLE != m_tblSVC[i].nSlotState
		   && index == m_tblSVC[i].nBelongto)
		{
			bool bFound = false;
			for(int k = 0; k < stream.tblSVCLength && !bFound; k++)
			{
				if(!memcmp(&stream.tblSVC[k].tServerAddr, &m_tblSVC[i].tServerAddr, sizeof(m_tblSVC[i].tServerAddr))
				   && !strncmp(stream.tblSVC[k].strSvcName, m_tblSVC[i].strSvcName, TUXONE_SERVICE_NAME_LENGTH))
				{
					bFound = true;
				}
			}

			if(bFound)
			{
				svcs_same.push_back(m_tblSVC[i]);
			}
			else
			{
				m_tblSVC[i].nSlotState = BBSLOT_IDLE;
				bUpdated = true;
			}
		}
	}

	// 查找当前gate与同步gate的差
	BBSVCList svcs_diff;
	for(int k = 0; k < stream.tblSVCLength; k++)
	{
		bool bFound = false;
		for(size_t i = 0; i < svcs_same.size() && !bFound; i++)
		{
			if(!memcmp(&stream.tblSVC[k].tServerAddr, &svcs_same[i].tServerAddr, sizeof(svcs_same[i].tServerAddr))
			   && !strncmp(stream.tblSVC[k].strSvcName, svcs_same[i].strSvcName, TUXONE_SERVICE_NAME_LENGTH))
			{
				bFound = true;
			}
		}

		if(!bFound)
		{
			svcs_diff.push_back(stream.tblSVC[k]);
		}
	}

	// 更新空位
	if(!svcs_diff.empty())
	{
		size_t diff_index = 0;
		for(int i = 0; i < m_tblSVCLength; i++)
		{
			if(BBSLOT_IDLE == m_tblSVC[i].nSlotState)
			{
				m_tblSVC[i] = svcs_diff[diff_index++];
				m_tblSVC[i].nSlotState = BBSLOT_INUSE;
				m_tblSVC[i].nBelongto = index;
				m_tblSVC[i].nIndex = i;
				bUpdated = true;
			}

			if(diff_index == svcs_diff.size())
			{
				break;
			}
		}
	}

	if(bUpdated)
	{
		m_pHeader->nSVCUpdateSeq++;
	}

	return bUpdated;
}

void BulletinBoard::WakeupOtherWSH(LWPR::SOCKET_FD_T udpfd)
{
	BBWSHList wshs;
	GetOtherWSH(wshs);

	for(size_t i = 0; i < wshs.size(); i++)
	{
		LWPR::Socket::WriteUDPSocket(udpfd, wshs[i].nPort, "N", 1);
		logger->debug(LTRACE, "notify other wsh, pid [%d], udp port [%d]", wshs[i].nPID, wshs[i].nPort);
	}
}

void BulletinBoard::Init(int wsh_len, int svc_len)
{
	m_tblWSHLength = wsh_len;
	m_tblSVCLength = svc_len;
}

void BulletinBoard::FetchGateIPCID(BBHeaderMap& headers)
{
	LWPR::IPCIDList ids;
	LWPR::IPCSHM::FetchSHMID(ids, BB_HEADER_MAGIC_CODE);
	for (size_t i = 0; i < ids.size(); i++)
	{
		try
		{
			BB_HEADER_ENTITY_T* pHeader = (BB_HEADER_ENTITY_T*)LWPR::IPCSHM::Mount(ids[i]);

			if(pHeader->nMagicCode == BB_HEADER_MAGIC_CODE)
			{
				headers[ids[i]] = *pHeader;
			}

			LWPR::IPCSHM::Unmount(pHeader);
		}
		catch(...)
		{
		}
	}
}
