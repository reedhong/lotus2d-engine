/********************************************************************
	created:	2012/12/10
	filename: 	ClientSocket.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_ClientSocket_H__
#define __Lotus2d_ClientSocket_H__

#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

namespace Lotus2d {
	class ClientSocket
	{
	public:
		ClientSocket();
		~ClientSocket();
		int  connectSever();
		bool processExcept( );
		bool processInput();
		bool processOutput();
		//bool processConnect();

		virtual void networkClose();
		virtual void handlePacket() = 0;
	public:
		///服务器信息
		string								m_strServerAddr;
		unsigned short				m_nServerPort;
		Socket							m_socket;
		///输入缓冲区
		SocketInputStream		m_socketInputStream;
		///输出缓冲区
		SocketOutputStream	m_socketOutputStream;

		bool									m_beConnected;

	};

}



#endif