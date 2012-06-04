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
#include "Message.h"

TUX_INVOKE_RESULT_E Invoke(LWPR::SOCKET_FD_T fd, Message& msg_in, Message& msg_out, LWPR::INT32 timeout)
{
	assert(fd >= 0);
	assert(timeout >= 0);

	try
	{
		LWPR::SOCKET_RET_TYPE_E nRet = LWPR::SOCKET_RET_OK;

		// 预处理请求报文
		LWPR::OutputStream outHeader;
		LWPR::OutputStream outBody;

		msg_in.Marshal(outBody);

		msg_in.MsgHeader().nMsgLength = outBody.GetStreamLength() + TUX_HEADER_MESSAGE_SIZE;
		msg_in.MsgHeader().Marshal(outHeader);

		// 写请求报文头
		nRet = LWPR::Socket::WriteSocket(fd, outHeader.GetStream(), outHeader.GetStreamLength(), timeout);
		switch(nRet)
		{
		case LWPR::SOCKET_RET_FAILED:
			return TUX_INVOKE_SEND_HEADER_FAILED;
		case LWPR::SOCKET_RET_TIMEOUT:
			return TUX_INVOKE_SEND_HEADER_TIMEOUT;
		default:
			break;
		}

		// 写请求报文体
		nRet = LWPR::Socket::WriteSocket(fd, outBody.GetStream(), outBody.GetStreamLength(), timeout);
		switch(nRet)
		{
		case LWPR::SOCKET_RET_FAILED:
			return TUX_INVOKE_SEND_BODY_FAILED;
		case LWPR::SOCKET_RET_TIMEOUT:
			return TUX_INVOKE_SEND_BODY_TIMEOUT;
		default:
			break;
		}

		// 读应答报文头
		while(true)
		{
			LWPR::Buffer_var vBufRep = new LWPR::Buffer(TUX_HEADER_MESSAGE_SIZE);
			nRet = LWPR::Socket::ReadSocket(fd, *vBufRep, TUX_HEADER_MESSAGE_SIZE, timeout);
			switch(nRet)
			{
			case LWPR::SOCKET_RET_FAILED:
				return TUX_INVOKE_RECEIVE_FAILED;
			case LWPR::SOCKET_RET_TIMEOUT:
				return TUX_INVOKE_RECEIVE_TIMEOUT;
			default:
				break;
			}

			LWPR::InputStream inHeader(vBufRep);
			msg_out.MsgHeader().Unmarshal(inHeader);
			if(TUX_MSG_TYPE_GATE_NOTICE == msg_out.MsgHeader().nMsgType)
			{
				// 如果收到的应答报文头是Gate主动下达的通知，则继续读报文头
				continue;
			}

			// 读应答报文体
			nRet = LWPR::Socket::ReadSocket(fd, *vBufRep, msg_out.MsgHeader().nMsgLength - TUX_HEADER_MESSAGE_SIZE, timeout);
			switch(nRet)
			{
			case LWPR::SOCKET_RET_FAILED:
				return TUX_INVOKE_RECEIVE_FAILED;
			case LWPR::SOCKET_RET_TIMEOUT:
				return TUX_INVOKE_RECEIVE_TIMEOUT;
			default:
				break;
			}

			LWPR::InputStream inBody(vBufRep);
			msg_out.Unmarshal(inBody);

			// 退出循环
			break;
		}
	}
	catch(const LWPR::UnmarshalException& e)
	{
		// 协议错误
		return TUX_INVOKE_RECEIVE_PROTOCOL_ERROR;
	}
	catch(...)
	{
		// 未知错误
		return TUX_INVOKE_RECEIVE_OS_ERROR;
	}

	return TUX_INVOKE_OK;
}

//----------------------------------------------------------------
// Message
//----------------------------------------------------------------
Message::Message()
{
}

Message::~Message()
{
}

void Message::Marshal(LWPR::OutputStream& out)
{

}

void Message::Unmarshal(LWPR::InputStream& in)
{

}

TUX_HEADER_MESSAGE_T& Message::MsgHeader()
{
	return m_tHeader;
}

void Message::MsgHeader(const TUX_HEADER_MESSAGE_T& header)
{
	m_tHeader = header;
}

//----------------------------------------------------------------
// TUX_HEADER_MESSAGE_T
//----------------------------------------------------------------
tux_header_message_t::tux_header_message_t() :
	nMsgLength(TUX_HEADER_MESSAGE_SIZE),
	nMsgType(0),
	nMsgFlags(0),
	nMsgRetCode(TUX_MSG_RET_CODE_OK)
{
#ifdef LWPR_BIG_ENDIAN
	nMsgFlags |= TUX_MSG_BIG_ENDIAN;
#else
	nMsgFlags &= ~TUX_MSG_BIG_ENDIAN;
#endif
}

void tux_header_message_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteINT32(nMsgLength);
	out.WriteINT32(nMsgType);
	out.WriteINT32(nMsgFlags);
	out.WriteINT32(nMsgRetCode);
}

void tux_header_message_t::Unmarshal(LWPR::InputStream& in)
{
	nMsgLength = in.ReadINT32();
	nMsgType = in.ReadINT32();
	nMsgFlags = in.ReadINT32();
	nMsgRetCode = in.ReadINT32();
}

bool tux_header_message_t::IsBigEndian()
{
	return (TUX_MSG_BIG_ENDIAN & nMsgFlags);
}

bool tux_header_message_t::DisconnectFromServer()
{
	return (TUX_MSG_DISCONNECT_SERVER & nMsgFlags);
}

void tux_header_message_t::DisconnectFromServer(bool discon)
{
	if (discon)
	{
		nMsgFlags |= TUX_MSG_DISCONNECT_SERVER;
	}
	else
	{
		nMsgFlags &= ~TUX_MSG_DISCONNECT_SERVER;
	}
}
//----------------------------------------------------------------
// TUX_FETCH_BB_REQ_T
//----------------------------------------------------------------
tux_fetch_bb_req_t::tux_fetch_bb_req_t() :
	bFetchAll(true)
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_ADMIN_GET_BB_REQ;
}

void tux_fetch_bb_req_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteBOOL(bFetchAll);
}

void tux_fetch_bb_req_t::Unmarshal(LWPR::InputStream& in)
{
	bFetchAll = in.ReadBOOL();
}

//----------------------------------------------------------------
// TUX_FETCH_BB_REP_T
//----------------------------------------------------------------
tux_fetch_bb_rep_t::tux_fetch_bb_rep_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_ADMIN_GET_BB_REP;
}

void tux_fetch_bb_rep_t::Marshal(LWPR::OutputStream& out)
{
	if(vBufBody != NULL)
	{
		out.WriteAny(vBufBody->Inout(), vBufBody->Size());
	}
}

void tux_fetch_bb_rep_t::Unmarshal(LWPR::InputStream& in)
{
	vBufBody = in.ReadAny();
}

//----------------------------------------------------------------
// TUX_APP_REGISTER_REQ_T
//----------------------------------------------------------------
tux_app_register_req_t::tux_app_register_req_t():
	nAppType(0)
{
	memset(&tServerAddr, 0, sizeof(tServerAddr));
	MsgHeader().nMsgType = TUX_MSG_TYPE_APP_REGISTER_REQ;
}

void tux_app_register_req_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteUINT32(nAppType);
	out.WriteArray(&tServerAddr, sizeof(tServerAddr));
	out.WriteUINT32((LWPR::UINT32)vtServices.size());
	for(size_t i = 0; i < vtServices.size(); i++)
	{
		out.WriteString(vtServices[i]);
	}
}

void tux_app_register_req_t::Unmarshal(LWPR::InputStream& in)
{
	nAppType = in.ReadUINT32();
	in.ReadArray(&tServerAddr);
	LWPR::UINT32 len = in.ReadUINT32();
	while(len--)
	{
		std::string value = in.ReadString();
		vtServices.push_back(value);
	}
}

//----------------------------------------------------------------
// TUX_APP_REGISTER_REP_T
//----------------------------------------------------------------

service_info_t::service_info_t()
{
	memset(&tServerAddr, 0, sizeof(LWPR::SOCKET_ADDR_T));
}

tux_app_register_rep_t::tux_app_register_rep_t() :
	nBBUpdateSeq(0), nAppHeartInterval(0)
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_APP_REGISTER_REP;
}

void tux_app_register_rep_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteUINT32(nBBUpdateSeq);
	out.WriteUINT32(nAppHeartInterval);
	out.WriteUINT32((LWPR::UINT32)vtServices.size());
	for(size_t i = 0; i < vtServices.size(); i++)
	{
		out.WriteString(vtServices[i]);
	}
}

void tux_app_register_rep_t::Unmarshal(LWPR::InputStream& in)
{
	nBBUpdateSeq = in.ReadUINT32();
	nAppHeartInterval = in.ReadUINT32();
	LWPR::UINT32 len = in.ReadUINT32();
	while(len--)
	{
		std::string value = in.ReadString();
		vtServices.push_back(value);
	}
}

//----------------------------------------------------------------
// TUX_APP_PINGPANG_REQ_T
//----------------------------------------------------------------
tux_app_pingpang_req_t::tux_app_pingpang_req_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_PING_PANG_REQ;
}

tux_app_pingpang_rep_t::tux_app_pingpang_rep_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_PING_PANG_REP;
}

//----------------------------------------------------------------
// TUX_APP_SUBSCIBE_REQ_T
//----------------------------------------------------------------
tux_app_subscibe_req_t::tux_app_subscibe_req_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_APP_SUBSCIBE_REQ;
}

void tux_app_subscibe_req_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteUINT32((LWPR::UINT32)vtServices.size());
	for(size_t i = 0; i < vtServices.size(); i++)
	{
		out.WriteString(vtServices[i]);
	}
}

void tux_app_subscibe_req_t::Unmarshal(LWPR::InputStream& in)
{
	LWPR::UINT32 len = in.ReadUINT32();
	while(len--)
	{
		std::string value = in.ReadString();
		vtServices.push_back(value);
	}
}

//----------------------------------------------------------------
// TUX_APP_SUBSCIBE_REP_T
//----------------------------------------------------------------
tux_app_subscibe_rep_t::tux_app_subscibe_rep_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_APP_SUBSCIBE_REP;
}

void tux_app_subscibe_rep_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteUINT32(nBBUpdateSeq);
	out.WriteUINT32((LWPR::UINT32)vtServiceInfo.size());
	for(size_t i = 0; i < vtServiceInfo.size(); i++)
	{
		out.WriteString(vtServiceInfo[i].strSvcName);
		vtServiceInfo[i].tServerAddr.Marshal(out);
	}
}

void tux_app_subscibe_rep_t::Unmarshal(LWPR::InputStream& in)
{
	nBBUpdateSeq = in.ReadUINT32();
	LWPR::UINT32 len = in.ReadUINT32();
	while(len--)
	{
		SERVICE_INFO_T value;
		value.strSvcName = in.ReadString();
		value.tServerAddr.Unmarshal(in);

		vtServiceInfo.push_back(value);
	}
}

//----------------------------------------------------------------
// TUX_APP_TPCALL_REQ_T
//----------------------------------------------------------------
tux_app_tpcall_req_t::tux_app_tpcall_req_t() :
	bHasReq(LWPR_FALSE),
	pReqBuf(NULL),
	nReqLen(0),
	nReqBufType(TYPED_BUFFER_CARRAY),
	nFlags(0)
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_TPCALL_REQ;
}

void tux_app_tpcall_req_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteString(strSvcName);
	out.WriteBOOL(bHasReq);
	if(bHasReq)
	{
		out.WriteArray(pReqBuf, nReqLen);
		out.WriteUINT32((LWPR::UINT32)nReqBufType);
		out.WriteString(strReqBufTypeSub);

		if(TYPED_BUFFER_VIEW32 == nReqBufType)
		{
			tStructInfo.marshal(out);
		}
	}

	out.WriteUINT32(nFlags);
}

void tux_app_tpcall_req_t::Unmarshal(LWPR::InputStream& in)
{
	strSvcName = in.ReadString();
	bHasReq = in.ReadBOOL();
	if(bHasReq)
	{
		vReqBuf = in.ReadAny();
		nReqBufType = (TYPED_BUFFER_E)in.ReadUINT32();
		strReqBufTypeSub = in.ReadString();

		if(TYPED_BUFFER_VIEW32 == nReqBufType)
		{
			tStructInfo.unmarshal(in);
		}
	}

	nFlags = in.ReadUINT32();
}

//----------------------------------------------------------------
// TUX_APP_TPCALL_REP_T
//----------------------------------------------------------------
tux_app_tpcall_rep_t::tux_app_tpcall_rep_t() :
	bHasRep(false),
	pRepBuf(NULL),
	nRepLen(0),
	nFlags(0),
	nRetCode(0)
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_TPCALL_REP;
}

void tux_app_tpcall_rep_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteBOOL(bHasRep);
	if(bHasRep)
	{
		out.WriteArray(pRepBuf, nRepLen);

		out.WriteUINT32((LWPR::UINT32)nRepBufType);
		out.WriteString(strRepBufTypeSub);

		if(TYPED_BUFFER_VIEW32 == nRepBufType)
		{
			tStructInfo.marshal(out);
		}
	}

	out.WriteUINT32(nFlags);
	out.WriteINT32(nRetCode);
}

void tux_app_tpcall_rep_t::Unmarshal(LWPR::InputStream& in)
{
	bHasRep = in.ReadBOOL();
	if(bHasRep)
	{
		vRepBuf = in.ReadAny();

		nRepBufType = (TYPED_BUFFER_E)in.ReadUINT32();
		strRepBufTypeSub = in.ReadString();

		if(TYPED_BUFFER_VIEW32 == nRepBufType)
		{
			tStructInfo.unmarshal(in);
		}
	}

	nFlags = in.ReadUINT32();
	nRetCode = in.ReadINT32();
}

//----------------------------------------------------------------
// TUX_APP_SERVER_INFO_REQ_T
//----------------------------------------------------------------
tux_app_server_info_req_t::tux_app_server_info_req_t()
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_SERVER_INFO_REQ;
}

void tux_app_server_info_req_t::Marshal(LWPR::OutputStream& out)
{

}

void tux_app_server_info_req_t::Unmarshal(LWPR::InputStream& in)
{

}

tux_app_server_info_rep_t::tux_app_server_info_rep_t() : nClientTotal(0)
{
	MsgHeader().nMsgType = TUX_MSG_TYPE_SERVER_INFO_REP;
}

void tux_app_server_info_rep_t::Marshal(LWPR::OutputStream& out)
{
	out.WriteString(strServerName);
	out.WriteINT32(nClientTotal);
}

void tux_app_server_info_rep_t::Unmarshal(LWPR::InputStream& in)
{
	strServerName = in.ReadString();
	nClientTotal = in.ReadINT32();
}
