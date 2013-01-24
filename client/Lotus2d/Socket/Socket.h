/********************************************************************
	created:	2012/09/21
	filename: 	Socket.h
	author:		reedhong
	
	purpose:	socket(tcp socket)网络操作
*********************************************************************/

#ifndef __Lotus2d_Socket_H__
#define __Lotus2d_Socket_H__

#include "Base/Prerequisites.h"
#include "Base/Config.h"

#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
#include <WinSock.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#endif

#include "Base/Prerequisites.h"


#if LOTUS2D_PLATFORM != LOTUS2D_PLATFORM_WIN32
	#define	SOCKET int
	#define	INVALID_SOCKET   -1
	#define	SOCKET_ERROR	 -1
#endif
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
static const UINT szSOCKADDR_IN = sizeof(SOCKADDR_IN);
#define IP_SIZE			24
static const INT SOCKET_ERROR_WOULDBLOCK = -100;
#define SELECTTIMEOUT -100

namespace Lotus2d {
	class Socket 
	{
	public :		
		// constructor
		Socket () ;
		Socket (const char* host, unsigned short port) ;
		// destructor
		virtual ~Socket () ;
	public :
		bool create() ;
		
		// close connection
		void close () ;
	
		/*
		 * conntimeo: 连接超时时间
		 */
		bool connect(const char *host, unsigned short port, int conntimeo = DEFAULT_CONNECT_TIMEOUT, int retry = DEFAULT_CONNECT_RETRY);

		// close previous connection and connect to another socket
		bool reconnect () ;
		
		// send data to peer
		int send (const char* buf, size_t len, int flags = 0) ;
		
		// receive data from peer
		int receive (char* buf, size_t len, int flags = 0) ;
		
		int available ()const ;

		void checkError() const;

	//////////////////////////////////////////////////
	// methods
	//////////////////////////////////////////////////
	private:
		int _connect(int sockfd, const sockaddr_in &remotehost, unsigned short port, int conntimeo);
	public :
	 
		// get/set socket's linger status
		unsigned int getLinger ()const ;
		bool setLinger (unsigned int lingertime) ;

		bool isReuseAddr ()const ;
		bool setReuseAddr (bool on = TRUE) ;

		// get is Error
		int getSockError()const ;
	 
		// set socket's nonblocking status
		bool setNonBlocking (bool on = TRUE) ;
	 
		// get/set receive buffer size
		size_t getReceiveBufferSize ()const ;
		bool setReceiveBufferSize (size_t size) ;
	 
		// get/set send buffer size
		size_t getSendBufferSize ()const ;
		bool setSendBufferSize (size_t size) ;
	 


		// check if socket is valid
		bool isValid ()const ;

		// get socket descriptor
		SOCKET getSOCKET ()const ;

		bool isSockError()const ;
	public:
		static bool InitSocketSystem();

	public :

		SOCKET m_socketID;

		// socket address structure
		SOCKADDR_IN m_sockAddr;
		
		// peer host 支持域名访问
		string m_host; 
		
		// peer port
		unsigned m_port;

		int mConnectTimeout;
		int mRetry;
		

	};
}

#endif
