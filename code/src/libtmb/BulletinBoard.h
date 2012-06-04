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
#ifndef TUX_BULLETIN_BOARD_H__
#define TUX_BULLETIN_BOARD_H__
#include <atmi.h>
#include <lwpr.h>
#include <string>
#include <vector>
#include <list>
#include <map>

typedef enum bbslot_state_e
{
	BBSLOT_IDLE = 0,		// 空闲状态
	BBSLOT_INUSE = 1,		// 使用状态
	BBSLOT_ISOLATED = 2,	// 隔离状态
} BBSLOT_STATE_E;

const LWPR::UINT32 BB_HEADER_MAGIC_CODE = 10504816 ^ 10225799;

/**
 * Header table entity
 */
typedef struct bb_header_entity_t
{
	LWPR::UINT32		nMagicCode;			// 用来唯一标识这是TUXONE使用的共享内存
	LWPR::UINT32		nSEMID;				// 信号量IPCID
	LWPR::UINT32		nSVCUpdateSeq;		// 公告板变更序号
	LWPR::UINT32		nGatePID;			// txgate进程号
	LWPR::UINT16		nUDPPort;			// UDP 端口号（本地字节序）
	LWPR::UINT32		nWSHMaxLength;		// WSH LEN
	LWPR::UINT32		nSVCMaxLength;		// SVC LEN
	LWPR::SOCKET_ADDR_T	tServerAddr;		// txgate的监听地址（网络字节序）

	/**
	 * 构造函数
	 */
	bb_header_entity_t();

	/**
	 * 重载new[]操作符
	 */
	void* operator new(size_t size, void* addr);
	void* operator new(size_t size);

} BB_HEADER_ENTITY_T;

typedef std::map < LWPR::IPCID_T, BB_HEADER_ENTITY_T > BBHeaderMap;

/**
 * WSH table entity
 */
typedef struct bb_wsh_entity_t
{
	LWPR::INT32		nIndex;				// 下标
	LWPR::INT32		nPID;				// 进程号
	LWPR::INT32		nConnectionTotal;	// SOCKET连接总数
	LWPR::UINT16	nPort;				// UDP 端口号（本地字节序）
	BBSLOT_STATE_E	nSlotState;			// 槽位的状态，数据类型：BBSLOT_STATE_E

	/**
	 * 构造函数
	 */
	bb_wsh_entity_t();

	/**
	 * 拷贝构造函数
	 */
	bb_wsh_entity_t(const bb_wsh_entity_t& rhs);

	/**
	 * 重载赋值运算符
	 */
	bb_wsh_entity_t& operator=(const bb_wsh_entity_t& rhs);

	/**
	 * 重载new[]操作符
	 */
	void* operator new[](size_t size, void* addr);
	void* operator new[](size_t size);
} BB_WSH_ENTITY_T;

typedef std::vector < BB_WSH_ENTITY_T > BBWSHList;

typedef std::vector < int > BBIndexList;

#define BBSVC_ENTITY_BELONG_TO_SELF		(-1)
/**
 * Service table entity
 */
typedef struct bb_service_entity_t
{
	LWPR::INT32				nIndex;				// 下标
	char					strSvcName[TUXONE_SERVICE_NAME_LENGTH + 1];	// Service Name
	LWPR::SOCKET_ADDR_T		tServerAddr;	// Server的地址
	LWPR::INT32				nBelongto;		// 当前service属于哪个gate
	BBSLOT_STATE_E			nSlotState;		// 槽位的状态，数据类型：BBSLOT_STATE_E

	/**
	 * 构造函数
	 */
	bb_service_entity_t();

	/**
	 * 拷贝构造函数
	 */
	bb_service_entity_t(const bb_service_entity_t& rhs);

	/**
	 * 重载赋值运算符
	 */
	bb_service_entity_t& operator=(const bb_service_entity_t& rhs);

	/**
	 * 重载new[]操作符
	 */
	void* operator new[](size_t size, void* addr);
	void* operator new[](size_t size);
} BB_SERVICE_ENTITY_T;

typedef std::vector < BB_SERVICE_ENTITY_T > BBSVCList;

/**
 * 公告板数据流（供网络传输）
 */
typedef struct bb_stream_t
{
	BB_HEADER_ENTITY_T*			pHeader;			// 公告板头部（创建于共享内存）
	BB_WSH_ENTITY_T*			tblWSH;				// WSH表（创建于共享内存）
	LWPR::INT32					tblWSHLength;		// WSH表长度

	BB_SERVICE_ENTITY_T*		tblSVC;				// SERVICE表（创建于共享内存）
	LWPR::INT32					tblSVCLength;		// SERVICE表长度

	bb_stream_t();
	~bb_stream_t();

	void Marshal(LWPR::OutputStream& out);
	void Unmarshal(LWPR::InputStream& in);
} BB_STREAM_T;

/**
 * 公告板类，供多进程环境下使用。
 */
class BulletinBoard
{
public:
	/**
	 * 构造函数
	 */
	BulletinBoard();

	/**
	 * 析构函数
	 */
	~BulletinBoard();

	/**
	 * 单例函数
	 */
	static BulletinBoard* GetInstance();

	/**
	 * 在挂载共享内存之前初始化
	 */
	void Init(int wsh_len, int svc_len);

	/**
	 * 打印公告板内容（调试用）
	 */
	void Print();

	/**
	 * 设置公告板头
	 */
	void SetHeader(const BB_HEADER_ENTITY_T& header);

	/**
	 * 获取公告板头
	 */
	void GetHeader(BB_HEADER_ENTITY_T& header);

	/**
	 * 获取变更序列号
	 */
	LWPR::UINT32 GetSVCUpdateSeq();

	/**
	 * 初始创建公告板
	 */
	void Create(int wsh_len, int svc_len);

	/**
	 * 连接到公告板
	 */
	void Connect(LWPR::IPCID_T shmid);

	/**
	 * 断开与公告板的连接
	 */
	void Disconnect();

	/**
	 * 销毁公告板
	 */
	void Destroy();

	/**
	 * 向公告板注册WSH
	 * 如果返回false，WSH表空间不足
	 * 从wsh结构体中返回下标
	 */
	bool RegisterWSH(BB_WSH_ENTITY_T& wsh);

	/**
	 * 消除WSH在公告板中的记录（由主进程调用）
	 */
	void CleanupWSHByIndex(const BBIndexList& wsh_indexs);

	/**
	 * 消除WSH在公告板中的记录（由主进程调用）
	 */
	void CleanupWSHByPID(const LWPR::PIDList& pids);

	/**
	 * WSH主动从公告板注销（由WSH进程调用）
	 */
	void UnregisterWSH();

	/**
	 * 设置WSH的Socket连接数（由WSH进程调用）
	 * 此函数使用原子操作，不需加锁
	 */
	void UpdateWSHConnectionTotal(bool increase);

	/**
	 * 从公告板获取WSH
	 */
	int GetAllWSH(BBWSHList& wshs);

	/**
	 * 从公告板获取除自己外其他WSH进程
	 */
	int GetOtherWSH(BBWSHList& wshs);

	/**
	 * 向公告板注册SERVICE
	 * 如果返回false，SERVICE表空间不足
	 * 从svc结构体中返回下标
	 */
	bool RegisterService(BB_SERVICE_ENTITY_T& svc);

	/**
	 * 从公告板注销SERVICE
	 * 注意：只需要填写BB_SERVICE_ENTITY_T结构体中的strSvcName、tServerAddr两项
	 */
	void UnregisterService(const BBSVCList& svcs);

	/**
	 * 从公告板注销一个SERVER所属的所有SERVICE
	 */
	void UnregisterService(const LWPR::SOCKET_ADDR_T& addr);

	/**
	 * 根据服务名从公告板获取SVC
	 * 返回公告板变更序号
	 */
	LWPR::UINT32 GetAllSVC(const std::vector<std::string>& names, BBSVCList& svcs);

	/**
	 * 判断公告板是否有变更(与上次调用做比较)
	 */
	bool IsSvcUpdated();

	/**
	 * 获取公告板流
	 * fetchall表示是否获取所有Service，如果为false，则只获取属于当前gate的service
	 */
	void GetBBStream(BB_STREAM_T& stream, bool fetchall = true);

	/**
	 * 同步公告板（从其他公告板获取数据）
	 * 返回值表示是否引起公告板发生变化
	 */
	bool SynBB(const BB_STREAM_T& stream, LWPR::INT32 index);

	/**
	 * 唤醒其他wsh进程
	 */
	void WakeupOtherWSH(LWPR::SOCKET_FD_T udpfd);

	/**
	 * 遍历系统，获取公告板头列表
	 */
	static void FetchGateIPCID(BBHeaderMap& headers);

private:
	/**
	 * 计算共享内存大小
	 */
	int ComputeShmSize();

private:
	LWPR::IPCID_T				m_nSHMID;
	void*						m_pSHMAddr;			// 本进程挂载的共享内存首地址
	LWPR::IPCID_T				m_nSEMID;			// 控制共享内存读写的锁

	BB_HEADER_ENTITY_T*			m_pHeader;			// 公告板头部（创建于共享内存）

	BB_WSH_ENTITY_T*			m_tblWSH;			// WSH表（创建于共享内存）
	int							m_tblWSHLength;		// WSH表长度

	BB_SERVICE_ENTITY_T*		m_tblSVC;			// SERVICE表（创建于共享内存）
	int							m_tblSVCLength;		// SERVICE表长度

	int							m_nWSHIndex;		// WSH进程对应的下标（创建于WSH进程内）
	LWPR::UINT32				m_nSVCUpdateSeq;	// 公告板变更序号（创建于WSH进程内）
};

#endif // end of TUX_BULLETIN_BOARD_H__
