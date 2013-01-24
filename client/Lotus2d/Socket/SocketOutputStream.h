/********************************************************************
	created:	2012/12/10
	filename: 	SocketOutputStream.h
	author:		reedhong
	
	purpose:	读入数据流, 裁剪自武侠
*********************************************************************/

#ifndef __Lotus2d_SocketOutputStream_H__
#define __Lotus2d_SocketOutputStream_H__

#include "Base/Stream.h"
#include "Socket.h"

//初始化的发送缓存长度
#define DEFAULTSOCKETOUTPUTBUFFERSIZE 8192
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETOUTPUTSIZE 100*1024

namespace Lotus2d {
	class SocketOutputStream :public Stream
	{
	public :
		SocketOutputStream( Socket* sock, 
							unsigned int BufferSize = DEFAULTSOCKETOUTPUTBUFFERSIZE,
							unsigned int MaxBufferSize = DISCONNECTSOCKETOUTPUTSIZE ) ;
		virtual ~SocketOutputStream( ) ;


	public :
		virtual size_t read(void* buf, size_t count) { return 0;};
		virtual size_t write(const void* buf, size_t count);
		virtual void skip(long count) {};
		virtual void seek( size_t pos ) {};
		virtual size_t tell(void) { return 0;};
		virtual bool eof(void) const { return true;};
		virtual void close(void) {};

		unsigned int						flush( ) ;

		void						initsize( ) ;
		bool						resize( int size ) ;

		int							capacity ()const { return m_bufferLen ; }
	 
		unsigned int			length( )const ;
		unsigned int			size( )const { return length( ) ; }

		const char*			getBuffer( )const { return m_buffer ; }

		char*						getTail( )const { return &(m_buffer[m_tail]) ; }
	 
		bool						empty ()const { return m_head==m_tail ; }

		void						cleanup( ) ;

		unsigned int			getHead(){return m_head;}
		unsigned int			getTail(){return m_tail;}
	private :
		
		Socket*		m_pSocket ;
		
		char*		m_buffer ;
		
		unsigned int		m_bufferLen ;
		unsigned int		m_maxBufferLen ;
		
		unsigned int		m_head ;
		unsigned int		m_tail ;
	};

}

#endif
