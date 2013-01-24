/********************************************************************
	created:	2012/09/20
	filename: 	Socket.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Socket.h"

namespace Lotus2d {

	Socket::Socket () 
	{ 
		m_socketID = INVALID_SOCKET ;
		memset( &m_sockAddr, 0, sizeof(SOCKADDR_IN) ) ;
		m_port = 0 ;
	}

	Socket::Socket ( const char* host , unsigned short port ) 
	{ 
		m_host = host;
		m_port = port ;

		create() ;	
	}

	Socket::~Socket () 
	{ 
		close() ;
	}

	bool Socket::create( )
	{
		m_socketID = ::socket(AF_INET , SOCK_STREAM , 0);
		if(m_socketID == INVALID_SOCKET)
		{
			// 获取错误情况
#ifdef LOTUS2D_DEBUG
			checkError();
#endif
			return FALSE;
		}

		memset( &m_sockAddr , 0 , sizeof(m_sockAddr) );
		m_sockAddr.sin_family = AF_INET;
		return TRUE;
	}

	// close previous connection and connect to another server socket
	bool Socket::reconnect ()
	{
		// delete old socket impl object
		this->close();
		return this->connect(m_host.c_str(), m_port, mConnectTimeout, mRetry);	
	}

	bool Socket::connect(const char *host, unsigned short port, int conntimeo, int retry)
	{
		m_host = host;
		m_port = port;
		mConnectTimeout = conntimeo;
		mRetry = retry;
		//! init remote sockaddr_in.
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_port = htons(port);

		struct hostent* hostInfo = gethostbyname(m_host.c_str());
		if (host == NULL)
			return false;

		unsigned int ipHost = * (unsigned int  *) hostInfo -> h_addr_list [0];
		m_sockAddr.sin_addr.s_addr =ipHost;
	
		LOGD("Socket::connect(%s, %d, %d, %d)", host,  port, conntimeo, retry);
		//! 连接retry次.
		while (retry--)
		{
			// 成功
			int iRet = _connect(m_socketID, m_sockAddr, port, conntimeo);
			if(iRet == 0){
				return true;
			}
			if( iRet == SELECTTIMEOUT)
				continue;
			this->close();
		}
		LOGD("Socket::connect(%s, %d, %d, %d), errono", host,  port, conntimeo, retry, errno);
		checkError();
		return false;
	}

	int Socket::_connect(int sockfd, const sockaddr_in &remotehost, unsigned short port, int conntimeo)
	{
		int ret;
		int error;
		struct timeval tval;
		fd_set rset, wset;

		if (sockfd == -1)
			goto _err;

		if(!setNonBlocking(true)){
			goto _err;
		}

		if ( (ret=::connect(sockfd, (struct sockaddr*)&remotehost, sizeof(remotehost))) < 0 ){
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			INT iErr = WSAGetLastError();
			//A blocking operation is currently executing.
			if(iErr != WSAEINPROGRESS && iErr != WSAEWOULDBLOCK){
				goto _err;
			}
#else 
			if (errno != EINPROGRESS)
			{
				goto _err;
			}
#endif
		}
		if (ret == 0) //connect on immediately
			goto _done;
		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);
		wset = rset;
		if (conntimeo > 0)
			tval.tv_sec = (long)conntimeo;
		else
			tval.tv_sec = 0;
		tval.tv_usec = 0;
		if((ret = ::select(sockfd + 1, &rset, &wset, NULL, conntimeo ? &tval : NULL)) == 0) // time out
		{
			return SELECTTIMEOUT;
		}
		else if(ret == -1)
		{
			goto _err;
		}


		if(FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
			int len = sizeof(error);
			if(::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, &len) < 0){
				LOGE("fcntl last-1");
				goto _err;
			}
			if(error){
				errno = error;
				goto _err;
			}
		} else
			goto _err;
_done:
		// 保持非阻塞
		//if(setNonBlocking(false)){
		//	goto _err;
		//}
		return 0;
_err:
		return -1;
	}

	void Socket::close () 
	{ 
		if( isValid() && !isSockError() ) {
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
			::closesocket(m_socketID);
#else
			::close(m_socketID);
#endif
		}

		m_socketID = INVALID_SOCKET ;
		memset( &m_sockAddr, 0, sizeof(SOCKADDR_IN) ) ;
		m_host = "";
		m_port = 0 ;
	}
		

	int Socket::send (const char* buf, size_t len, int flags) 
	{ 
		return ::send(m_socketID, buf, len, flags);
	}

	int Socket::receive (char* buf, size_t len, int flags) 
	{ 
		return ::recv( m_socketID , buf , len , flags );
	}

	int Socket::available ()const
	{ 
		
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		u_long argp = 0;
		ioctlsocket(m_socketID,FIONREAD,&argp);
		return (int)argp;
#else 
		int arg = 0;
		ioctl(m_socketID,FIONREAD,&arg);
		return arg;
#endif
	}

	unsigned int Socket::getLinger ()const 
	{ 
		struct linger ling;
		int len = (int)sizeof(ling);
		
		::getsockopt( m_socketID , SOL_SOCKET , SO_LINGER , (char*)&ling , &len );
		
		return ling.l_linger;
	}

	bool Socket::setLinger (unsigned int lingertime) 
	{

		struct linger ling;
		
		ling.l_onoff = lingertime > 0 ? 1 : 0;
		ling.l_linger = lingertime;
		
		int iRet = ::setsockopt( m_socketID , SOL_SOCKET , SO_LINGER , (const char*)&ling , sizeof(ling) );

		return (iRet >= 0) ;
	}

	int Socket::getSockError()const 
	{ 
		return isSockError(); 
	}


	bool Socket::setNonBlocking (bool on ) 
	{ 
		int iRet;
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		unsigned long  argp = ( on == TRUE ) ? 1 : 0;
		 iRet = ::ioctlsocket(m_socketID,FIONBIO,&argp);
#else 
		int flags = ::fcntl(m_socketID , F_GETFL , 0 );
		if ( on )
			// make nonblocking fd
			flags |= O_NONBLOCK;
		else
			// make blocking fd
			flags &= ~O_NONBLOCK;

		iRet =::fcntl(m_socketID , F_SETFL , flags );

#endif
		return (iRet >=0) ;
	}

	size_t Socket::getReceiveBufferSize ()const 
	{ 
		size_t ReceiveBufferSize=0;
		int size = (int)sizeof(ReceiveBufferSize);

		::getsockopt( m_socketID , SOL_SOCKET , SO_RCVBUF , (char *)&ReceiveBufferSize, &size );
		
		return ReceiveBufferSize;
	}

	bool Socket::setReceiveBufferSize (size_t size)
	{ 
		int iRet = ::setsockopt( m_socketID , SOL_SOCKET , SO_RCVBUF , (const char *)&size, sizeof(size_t) );
		return (iRet !=SOCKET_ERROR) ;
	}

	size_t Socket::getSendBufferSize ()const 
	{ 
		size_t SendBufferSize=0;
		int size = (int)sizeof(SendBufferSize);

		::getsockopt( m_socketID , SOL_SOCKET , SO_SNDBUF , (char *)&SendBufferSize, &size );
		
		return SendBufferSize;
	}

	bool Socket::setSendBufferSize (UINT size)
	{
		int iRet = ::setsockopt( m_socketID , SOL_SOCKET , SO_SNDBUF , (const char*)&size, sizeof(size_t) ) ;
		return (iRet !=SOCKET_ERROR) ;
	}


	bool Socket::isValid ()const 
	{
		return m_socketID!=INVALID_SOCKET; 
	}

	SOCKET Socket::getSOCKET ()const 
	{ 

		return m_socketID; 

	}

	bool Socket::isSockError()const
	{
		int error;
		int len = (int)sizeof(error);
		
		int iRet = ::getsockopt( m_socketID , SOL_SOCKET , SO_ERROR , (char *)&error, &len );

		if( iRet == 0 ) {
			return FALSE;
		}else{ 			  
			return TRUE;
		}
	}

	
	bool Socket::InitSocketSystem()
	{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		WSADATA wsadata;	  
		unsigned short winsock_version=0x0101;
		if(WSAStartup(winsock_version,&wsadata))
		{ 
			return FALSE;
		}
#endif
		return TRUE;
	}

	void Socket::checkError() const
	{
#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		INT iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			break ;
		case WSAENETDOWN : 
			break ;
		case WSAEAFNOSUPPORT : 
			break ;
		case WSAEINPROGRESS : 
			break ;
		case WSAEMFILE : 
			break ;
		case WSAENOBUFS : 
			break ;
		case WSAEPROTONOSUPPORT : 
			break ;
		case WSAEPROTOTYPE : 
			break ;
		case WSAESOCKTNOSUPPORT : 
			break ;
		default : break;
		};//end of switch
#else 
		switch ( errno ) 
		{
		case EPROTONOSUPPORT :
		case EMFILE : 
		case ENFILE : 
		case EACCES : 
		case ENOBUFS : 
		default : 
			{
				break;
			}
		}//end of switch
#endif
	}

} // end Lotus2d