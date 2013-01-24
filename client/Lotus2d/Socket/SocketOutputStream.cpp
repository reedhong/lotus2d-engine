/********************************************************************
	created:	2012/12/10
	filename: 	SocketOutputStream.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/


#include "SocketOutputStream.h"

namespace Lotus2d {
	SocketOutputStream::SocketOutputStream( Socket* sock, unsigned int BufferLen, unsigned int MaxBufferLen ) 
	{
		m_pSocket = sock ;
		m_bufferLen = BufferLen ;
		m_maxBufferLen = MaxBufferLen ;
		ASSERT( m_pSocket != NULL );
		ASSERT( m_bufferLen > 0 );

		m_head = 0 ;
		m_tail = 0 ;
		
		m_buffer = new char[ m_bufferLen ];
		ASSERT( m_buffer ) ;

		memset( m_buffer, 0, m_bufferLen ) ;
	}

	SocketOutputStream::~SocketOutputStream( ) 
	{
		SAFE_DELETE_ARRAY(m_buffer);
	}

	unsigned int SocketOutputStream::length( )const
	{
		if( m_head<m_tail )
			return m_tail-m_head;
		
		else if( m_head>m_tail ) 
			return m_bufferLen-m_head+m_tail ;
		
		return 0 ;
	}

	size_t SocketOutputStream::write(const void* buf, size_t len)
	{

			//					//
			//     T  H			//    H   T			LEN=10
			// 0123456789		// 0123456789
			// abcd...efg		// ...abcd...
			//					//
				
		unsigned int nFree = ( (m_head<=m_tail)?(m_bufferLen-m_tail+m_head-1):(m_head-m_tail-1) ) ;

		if( len>=nFree ){
			if( !resize( len-nFree+1 ) )
				return 0 ;
		}
			
		if( m_head<=m_tail ) {	
			if( m_head==0 ) {
				nFree = m_bufferLen - m_tail - 1;
				memcpy(m_buffer+ m_tail, buf, len ) ;
			} else {
				nFree = m_bufferLen-m_tail ;
				if( len<=nFree ){
					memcpy( m_buffer+m_tail, buf, len ) ;
				}else {
					memcpy(m_buffer+m_tail, buf, nFree ) ;
					memcpy( m_buffer, (char *)(buf)+nFree, len-nFree ) ;
				}
			}
		} else {	
			memcpy( &m_buffer[m_tail], buf, len ) ;
		}
		
		m_tail = (m_tail+len)%m_bufferLen ;
			
		return len;
	}

	void SocketOutputStream::initsize( )
	{
		m_head = 0 ;
		m_tail = 0 ;

		SAFE_DELETE_ARRAY( m_buffer ) ;
			
		m_buffer = new char[DEFAULTSOCKETOUTPUTBUFFERSIZE] ;
		ASSERT(m_buffer) ;
			
		m_bufferLen = DEFAULTSOCKETOUTPUTBUFFERSIZE ;
			
		memset( m_buffer, 0, m_bufferLen ) ;
	}

	unsigned int SocketOutputStream::flush( ) 
	{

		ASSERT( m_pSocket != NULL );

		unsigned int nFlushed = 0;
		unsigned int nSent = 0;
		unsigned int nLeft;

		if( m_bufferLen>m_maxBufferLen )
		{//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
			initsize( ) ;
			
			return SOCKET_ERROR-1 ;
		}

	#if LOTUS2D_PLATFORM == LOTUS2D_PLATFORM_WIN32
		unsigned int flag = MSG_DONTROUTE ;
	#else
		unsigned int flag = MSG_NOSIGNAL ;
	#endif
		
		{
			if ( m_head < m_tail ) 
			{
				nLeft = m_tail - m_head;
		
				while ( nLeft > 0 ) 
				{
					nSent = m_pSocket->send( &m_buffer[m_head] , nLeft , flag );
					if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nSent==SOCKET_ERROR) return SOCKET_ERROR-2 ;
					if (nSent==0) return 0;
					
					nFlushed += nSent;
					nLeft -= nSent;
					m_head += nSent;
				}

				ASSERT( nLeft == 0 );
		
			} 
			else if( m_head > m_tail ) 
			{
				nLeft = m_bufferLen - m_head;
		
				while ( nLeft > 0 ) 
				{
					nSent = m_pSocket->send( &m_buffer[m_head] , nLeft , flag );
					if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nSent==SOCKET_ERROR) return SOCKET_ERROR-3 ;
					if (nSent==0) return 0;
					
					nFlushed += nSent;
					nLeft -= nSent;
					m_head += nSent;
				}
				
				ASSERT( m_head == m_bufferLen );
				
				m_head = 0;
				
				nLeft = m_tail;
		
				while ( nLeft > 0 ) 
				{
					nSent = m_pSocket->send( &m_buffer[m_head] , nLeft , flag );
					if (nSent==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nSent==SOCKET_ERROR) return SOCKET_ERROR-4 ;
					if (nSent==0) return 0;
					
					nFlushed += nSent;
					nLeft -= nSent;
					m_head += nSent;
				}
		
				ASSERT( nLeft == 0 );
			}
		
			if ( m_head != m_tail ) 
			{
				ASSERT( m_head == m_tail );
			}
			
		}
		
		m_head = m_tail = 0 ;
		return nFlushed;
	}

	bool SocketOutputStream::resize( int size )
	{
			
		ASSERT( size != 0 );

		int orgSize = size;

		size = max( size, (int)(m_bufferLen>>1) ) ;
		unsigned int newBufferLen = m_bufferLen+size ;
		unsigned int len = length( ) ;
		
		if( size<0 ) 
		{
			if( newBufferLen<0 || newBufferLen<len )
				return false ;
		} 
		
		char * newBuffer = new char[ newBufferLen ] ;
		ASSERT( newBuffer ) ;
		if( newBuffer==NULL )
			return false ;
			
		if( m_head<m_tail ) {
			memcpy( newBuffer, &m_buffer[m_head], m_tail-m_head ) ;
		} else if( m_head>m_tail ) {
			memcpy( newBuffer, &m_buffer[m_head] , m_bufferLen-m_head );
			memcpy( &newBuffer[m_bufferLen-m_head], m_buffer, m_tail );
		}
			
		delete [] m_buffer;
			
		m_buffer = newBuffer;
		m_bufferLen = newBufferLen;
		m_head = 0;
		m_tail = len;	

		return true ;
	}

	void SocketOutputStream::cleanup( )
	{
		m_head = 0 ;
		m_tail = 0 ;
	}
}