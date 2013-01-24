/********************************************************************
	created:	2012/12/10
	filename: 	ClientSocket.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ClientSocket.h"

namespace Lotus2d {

	ClientSocket::ClientSocket():m_socketInputStream(&m_socket),
		m_socketOutputStream(&m_socket)
	{
		m_beConnected = false;
	}

	ClientSocket::~ClientSocket()
	{

	}


	bool ClientSocket::processExcept( )
	{
		networkClose();
		return true;
	}

	bool ClientSocket::processInput()
	{
		UINT ret = m_socketInputStream.fill( ) ;
		if( (INT)ret <= SOCKET_ERROR )
		{
			networkClose();
			return false;
		}
		handlePacket();
		return true;
	}

	void ClientSocket::networkClose()
	{
		m_socket.close();
	}

	bool ClientSocket::processOutput()
	{
		UINT ret = m_socketOutputStream.flush( ) ;
		if( (INT)ret <= SOCKET_ERROR ){
			networkClose();
			return false;
		}
		return true;
	}


	int ClientSocket::connectSever()
	{
		//关闭SOCKET
		int iRet;
		m_socket.close();
		m_beConnected = false;
		//创建新的SOCKET
		bool bCreated = m_socket.create();
		if(bCreated){
			bool bConnect = m_socket.connect( m_strServerAddr.c_str(), m_nServerPort);
			if(bConnect){
				iRet = 1;
				m_beConnected = true;
			}else{
				m_socket.close();
				iRet = -2;
			}
		}else{
			iRet = -1;
		}
		//processConnect();
		return iRet;
	}


}  // end namespace 
