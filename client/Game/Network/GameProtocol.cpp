/********************************************************************
	created:	2012/12/10
	filename: 	GameProtocol.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "GameProtocol.h"


GameProtocol* GameProtocol::s_instance = NULL;

GameProtocol::GameProtocol(const char* szServerAddr,  unsigned short serverPort)
	:m_bodyStream(m_bodyData, MAX_PACKET_LEN), m_headStream(m_headData, PACKET_HADE_LEN)
{
	m_strServerAddr = szServerAddr;
	m_nServerPort = serverPort;
	m_seq = 24;
	memset(m_bodyData,  0, MAX_PACKET_LEN);
	initPacketHandlers();

	m_uid = 0;
	s_instance = this;
}

GameProtocol::~GameProtocol()
{

}


GameProtocol* GameProtocol::Instance()
{
	ASSERT(s_instance);
	return s_instance;
}

void GameProtocol::initPacketHandlers()
{
	memset(m_handlers, 0, PACKET_MAX*sizeof(PacketHandlerFunc));
	m_handlers[PACKET_LOGIN] = PacketHandler::handleLogin;
}


void GameProtocol::networkClose()
{

}

void GameProtocol::handlePacket()
{
	int len = m_socketInputStream.length();
	if(len < PACKET_HADE_LEN)
		return ;	// 协议头没收全

	m_socketInputStream.peek(m_headData, PACKET_HADE_LEN);
	
	// 获取协议长度
	m_headStream.seek(0);
	unsigned int  packetLen = (unsigned int)m_headStream.read32();


	if( packetLen > m_socketInputStream.length()){
		return ; // 协议体没收全
	}


	// 越过协议头
	m_socketInputStream.skip(PACKET_HADE_LEN);
	int bodyLen = packetLen-PACKET_HADE_LEN;

	// 处理协议
	m_socketInputStream.read(m_bodyData, bodyLen);
	
	m_bodyStream.setSize(bodyLen);

	PacketHandlerFunc handler = m_handlers[0];
	if(handler ){
		handler(m_bodyStream);
	}else{
		ASSERT(0);
	}
}


void GameProtocol::sendHead(int bodyLen, PacketID_t  id)
{

}

void GameProtocol::sendBody(int bodyLen)
{
	m_socketOutputStream.write(m_bodyData, bodyLen);
}

