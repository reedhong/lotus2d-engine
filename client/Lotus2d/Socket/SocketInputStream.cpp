/********************************************************************
	created:	2012/12/10
	filename: 	SocketInputStream.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "SocketInputStream.h"

namespace Lotus2d{

	SocketInputStream::SocketInputStream( Socket* sock, unsigned int BufferLen, unsigned int MaxBufferLen ) 
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

	SocketInputStream::~SocketInputStream( ) 
	{
		SAFE_DELETE_ARRAY(m_buffer);
	}

	unsigned int SocketInputStream::length( )const
	{
		if( m_head<m_tail )
			return m_tail-m_head;
		
		else if( m_head>m_tail ) 
			return m_bufferLen-m_head+m_tail ;
		return 0 ;
	}

	//返回0表示没有读到数据
	size_t SocketInputStream::read(void* buf, size_t count ) 
	{
		ASSERT( buf != NULL );	
		if ( count == 0 )
			return 0 ;
			
		if ( count > length() )
			return 0 ;
		
		if ( m_head < m_tail ) {
			memcpy( buf, m_buffer+m_head, count ) ;
		} else {
			unsigned int rightLen = m_bufferLen-m_head ;
			if( count<=rightLen ) {
				memcpy( buf, m_buffer+m_head, count ) ;
			} else {
				memcpy( buf, m_buffer+m_head, rightLen ) ;
				memcpy( (void *)((char *)(buf)+rightLen), m_buffer, count-rightLen ) ;
			}
		}
		m_head = (m_head+count)%m_bufferLen ;
		return count ;
	}

	size_t SocketInputStream::peek(void* buf, size_t count)
	{
		ASSERT( buf != NULL );	
		if ( count == 0 )
			return 0 ;

		if ( count > length() )
			return 0 ;

		if ( m_head < m_tail ) {
			memcpy( buf, m_buffer+m_head, count ) ;
		} else {
			unsigned int rightLen = m_bufferLen-m_head ;
			if( count<=rightLen ) {
				memcpy( buf, m_buffer+m_head, count ) ;
			} else {
				memcpy( buf, m_buffer+m_head, rightLen ) ;
				memcpy( (void *)((char *)(buf)+rightLen), m_buffer, count-rightLen ) ;
			}
		}
		return count ;
	}


	void SocketInputStream::skip(long count)
	{			
		if( count == 0 )
			return  ;

		if( count>(long)length( ) )
			return  ;

		m_head = (m_head+(int)count)%m_bufferLen ;
	}

	bool SocketInputStream::eof(void) const
	{
		return false;
	}

	bool SocketInputStream::find( char* buf )
	{
#if 0
		ASSERT( buf!=NULL ) ;	
		bool IsFind = false;
		while ( length() > PACK_COMPART_SIZE ){
			if( m_Head<m_Tail ) {
				memcpy( buf , &m_Buffer[m_Head] , PACK_COMPART_SIZE );
			} else {
				unsigned int rightLen = m_BufferLen-m_Head ;
				if( PACK_COMPART_SIZE<=rightLen ) {
					memcpy( &buf[0], &m_Buffer[m_Head], PACK_COMPART_SIZE ) ;
				} else {
					memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
					memcpy( &buf[rightLen], &m_Buffer[0], PACK_COMPART_SIZE-rightLen ) ;
				}
			}
			if (strstr(buf, PACK_COMPART) == NULL){
				m_Head++;
				if (m_Head >= m_BufferLen){
					m_Head -= m_BufferLen;
				}
				continue;
			}
			else{
				IsFind = true;
				break;
			}
		}

		if (!IsFind){
			return false;
		}

		m_Head += (unsigned int)PACK_COMPART_SIZE;
		if (m_Head >= m_BufferLen){
			m_Head -= m_BufferLen;
		}
#endif
		return true ;

	}


	void SocketInputStream::initsize( )
	{
		m_head = 0 ;
		m_tail = 0 ;

		SAFE_DELETE_ARRAY( m_buffer ) ;
			
		m_buffer = new char[DEFAULTSOCKETINPUTBUFFERSIZE] ;
		ASSERT(m_buffer) ;
			
		m_bufferLen = DEFAULTSOCKETINPUTBUFFERSIZE ;
			
		memset( m_buffer, 0, m_bufferLen ) ;
	}
		
	unsigned int SocketInputStream::fill( ) 
	{
		unsigned int nFilled = 0 ;
		unsigned int nReceived = 0 ;
		unsigned int nFree = 0 ;

		if ( m_head <= m_tail ) {
			if ( m_head == 0 ) {
				nReceived = 0 ;
				nFree = m_bufferLen-m_tail-1 ;
				if( nFree != 0 ){
					nReceived = m_pSocket->receive( &m_buffer[m_tail] , nFree );
					if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
					if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-1 ;
					if (nReceived==0) return SOCKET_ERROR-2 ;

					m_tail += nReceived;
					nFilled += nReceived;
				}

				if( nReceived == nFree ) {
					unsigned int available = m_pSocket->available();
					if ( available > 0 ) {
						if( (m_bufferLen+available+1)>m_maxBufferLen ){
							initsize( ) ;
							return SOCKET_ERROR-3 ;
						}
						if( !resize( available+1 ) )
							return 0 ;

						nReceived = m_pSocket->receive( &m_buffer[m_tail] , available );
						if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
						if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-4 ;
						if (nReceived==0) return SOCKET_ERROR-5;

						m_tail += nReceived;
						nFilled += nReceived;
					}
				}
			} else {
				//
				//    H   T		LEN=10
				// 0123456789
				// ...abcd...
				//

				nFree = m_bufferLen-m_tail ;
				nReceived = m_pSocket->receive( &m_buffer[m_tail], nFree );
				if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
				if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-6 ;
				if( nReceived==0 ) return SOCKET_ERROR-7 ;

				m_tail = (m_tail+nReceived)%m_bufferLen ;
				nFilled += nReceived ;

				if( nReceived==nFree ) {
	//				ASSERT( m_Tail == 0 );

					nReceived = 0 ;
					nFree = m_head-1 ;
					if( nFree!=0 ){
						nReceived = m_pSocket->receive( &m_buffer[0] , nFree );
						if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
						if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR -8;
						if( nReceived==0 ) return SOCKET_ERROR-9 ;

						m_tail += nReceived;
						nFilled += nReceived;
					}

					if( nReceived==nFree ) {
						unsigned int available = m_pSocket->available();
						if ( available > 0 ) {
							if( (m_bufferLen+available+1)>m_maxBufferLen ){
								initsize( ) ;
								return SOCKET_ERROR-10 ;
							}
							if( !resize( available+1 ) )
								return 0 ;

							nReceived = m_pSocket->receive( &m_buffer[m_tail] , available );
							if (nReceived==SOCKET_ERROR_WOULDBLOCK) return 0 ; 
							if (nReceived==SOCKET_ERROR) return SOCKET_ERROR-11 ;
							if (nReceived==0) return SOCKET_ERROR-12;

							m_tail += nReceived;
							nFilled += nReceived;
						}
					}
				}
			}
		} else {	
			//
			//     T  H		LEN=10
			// 0123456789
			// abcd...efg
			//

			nReceived = 0 ;
			nFree = m_head-m_tail-1 ;
			if( nFree!=0 ){
				nReceived = m_pSocket->receive( &m_buffer[m_tail], nFree ) ;
				if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
				if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-13 ;
				if( nReceived==0 ) return SOCKET_ERROR-14 ;

				m_tail += nReceived ;
				nFilled += nReceived ;
			}
			if( nReceived==nFree ) {
				unsigned int available = m_pSocket->available( ) ;
				if ( available>0 ) {
					if( (m_bufferLen+available+1)>m_maxBufferLen ){
						initsize( ) ;
						return SOCKET_ERROR-15 ;
					}
					if( !resize( available+1 ) )
						return 0 ;

					nReceived = m_pSocket->receive( &m_buffer[m_tail], available ) ;
					if( nReceived==SOCKET_ERROR_WOULDBLOCK ) return 0 ; 
					if( nReceived==SOCKET_ERROR ) return SOCKET_ERROR-16 ;
					if( nReceived==0 ) return SOCKET_ERROR-17 ;

					m_tail += nReceived ;
					nFilled += nReceived ;
				}
			}
		}

		return nFilled ;
	}

	bool SocketInputStream::resize(int size )
	{		
		size = max(size, (int)(m_bufferLen>>1));
		unsigned int newBufferLen = m_bufferLen + size;
		unsigned int len = length();
		
		if ( size < 0 ) {
			if ( newBufferLen < 0 || newBufferLen < len )
				return false ;		
		} 
		
		char * newBuffer = new char[ newBufferLen ];
	//	ASSERT( newBuffer ) ;
			
		if ( m_head < m_tail ) {
			memcpy( newBuffer , &m_buffer[m_head] , m_tail - m_head );
		} else if ( m_head > m_tail ) {
			memcpy( newBuffer , &m_buffer[m_head] , m_bufferLen - m_head );
			memcpy( &newBuffer[ m_bufferLen - m_head ] , m_buffer , m_tail );
		}
			
		delete [] m_buffer ;
			
		m_buffer = newBuffer ;
		m_bufferLen = newBufferLen ;
		m_head = 0 ;
		m_tail = len ;

		return true ;
	}

	void SocketInputStream::cleanUp( )
	{
		m_head = 0 ;
		m_tail = 0 ;
	}

}
