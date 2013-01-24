/********************************************************************
	created:	2012/12/10
	filename: 	GameProtocol.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __GameProtocol_H__
#define __GameProtocol_H__

#include "Socket/ClientSocket.h"
#include "Base/MemoryStream.h"
using namespace Lotus2d;

#include "PacketHandler.h"
#include  "PacketIDDefined.h"
#include  "Constants/GameConfig.h"


class GameProtocol:  public ClientSocket
{
protected:
	static GameProtocol* s_instance;
public:
	GameProtocol(const char* szServerAddr,  unsigned short serverPort);
	~GameProtocol();

	static GameProtocol* Instance();

	virtual void networkClose();
	virtual void handlePacket();

	/*************各种该协议发送, 实现放在SendPacket.cpp中*******************/
	void sendLogin(unsigned int  uid,  const char* pwd);

	/*************end 各种该协议发送*******************/
	void initPacketHandlers();
	void sendHead(int bodyLen, PacketID_t  id);
	void sendBody(int bodyLen);

public:
	unsigned int m_uid;
	short m_seq;
	PacketHandlerFunc			m_handlers[PACKET_MAX];

	unsigned char					m_bodyData[MAX_PACKET_LEN];
	MemoryStream				m_bodyStream;

	unsigned char					m_headData[PACKET_HADE_LEN];
	MemoryStream				m_headStream;
	
};


#endif
