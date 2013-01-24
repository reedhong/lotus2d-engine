/********************************************************************
	created:	2012/12/10
	filename: 	NetworkManager.h
	author:		reedhong
	
	purpose:	
*********************************************************************/
#include "Thread/Thread.h"
#include "Socket/ClientSocket.h"
using namespace Lotus2d;

class NetworkManager 
{
protected:
	NetworkManager();
public:
	~NetworkManager();
	static NetworkManager* Instance();
	int connectLoginServer(const char* szServerAddr,  unsigned short serverPort);
	void step();
	void init();
protected:
	void waitPacket();
protected:
	//¿É¶Á¾ä±ú
	fd_set					m_readFD;
	//¿ÉÐ´¾ä±ú
	fd_set					m_writeFD;
	//Òì³£¾ä±ú
	fd_set					m_exceptFD;
	
	bool					m_isStartConnectThread;
	vector<ClientSocket * > m_clientSockets;
};
