/********************************************************************
	created:	2012/12/10
	filename: 	NetworkManager.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "NetworkManager.h"
#include "GameProtocol.h"
#include "Game.h"


NetworkManager::NetworkManager()
{
	m_isStartConnectThread = false;
}

NetworkManager::~NetworkManager()
{
}

NetworkManager* NetworkManager::Instance()
{
	static NetworkManager 	s_instance;
	s_instance.init();
	return &s_instance;
}

void NetworkManager::init()
{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
	WSADATA wsaData;
	if(0!=WSAStartup( MAKEWORD( 2, 2 ), &wsaData)  ||
		(LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) )
	{
		WSACleanup();
		LOGD("Could not find a usable WinSock DLL!");
		return;
	}
#endif
}


int NetworkManager::connectLoginServer(const char* szServerAddr,  unsigned short serverPort)
{
	GameProtocol* cs = new GameProtocol(szServerAddr, serverPort);
	m_clientSockets.push_back((ClientSocket *)cs);
	return cs->connectSever();
}

void NetworkManager::step()
{
	waitPacket();
}

void NetworkManager::waitPacket()
{
	FD_ZERO(&m_readFD);
	FD_ZERO(&m_writeFD);
	FD_ZERO(&m_exceptFD);

	vector<ClientSocket *>::iterator iter = m_clientSockets.begin();
	int socketCount = 0;
	for(; iter != m_clientSockets.end();  ++ iter){
		ClientSocket* cs = *iter;
		if( cs->m_socket.isValid()){
			FD_SET(cs->m_socket.getSOCKET(), &m_readFD);
			FD_SET(cs->m_socket.getSOCKET(), &m_writeFD);
			FD_SET(cs->m_socket.getSOCKET(), &m_exceptFD);
			socketCount++;
		}
	}
	if(socketCount <= 0)
		return ;

	timeval	timeout ;
	timeout.tv_sec = 0 ;
	timeout.tv_usec = 0 ;
	int ret = ::select(socketCount+1, &m_readFD, &m_writeFD, &m_exceptFD, &timeout);
	if( ret == 0){
		LOGD("");
		return ;
	}
	iter = m_clientSockets.begin();
	for(; iter != m_clientSockets.end();  ++ iter){
		ClientSocket* cs = *iter;
		if (FD_ISSET(cs->m_socket.getSOCKET(), &m_readFD)){
			cs->processInput();
		}
		if( FD_ISSET(cs->m_socket.getSOCKET(), &m_writeFD ) ){
			cs->processOutput();
		}
		if( FD_ISSET(cs->m_socket.getSOCKET(), &m_exceptFD) ){
			cs->processExcept();
		}
	}
}