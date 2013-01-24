/********************************************************************
	created:	2012/12/10
	filename: 	SocketInputStream.h
	author:		reedhong
	
	purpose:	读入数据流, 裁剪自武侠
*********************************************************************/

#ifndef __Lotus2d_SocketInputStream_H__
#define __Lotus2d_SocketInputStream_H__


#include "Socket.h"

//初始化的接收缓存长度
#define DEFAULTSOCKETINPUTBUFFERSIZE 64*1024
//最大可以允许的缓存长度，如果超过此数值，则断开连接
#define DISCONNECTSOCKETINPUTSIZE 96*1024


#include "Base/Stream.h"
#include "Socket.h"

namespace Lotus2d {
	class SocketInputStream:public Stream
	{
	public :
		SocketInputStream( Socket* sock, 
						   unsigned int BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
						   unsigned int MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;
		virtual ~SocketInputStream( ) ;
	public :
		virtual size_t read(void* buf, size_t count);
		virtual size_t write(const void* buf, size_t count){ return 0;};
		virtual void skip(long count);
		virtual void seek( size_t pos ) {};
		virtual size_t tell(void) { return 0;};
		virtual bool eof(void) const;
		virtual void close(void) {};

		/*
		 * 只是取数据，不移动指针
		 */
		size_t peek(void* buf, size_t count) ;


		bool find( char* buf ) ;
	
		
		unsigned int	fill( ) ;

		void	initsize( ) ;
		bool	resize(int size ) ;
		
		unsigned int	capacity( )const { return m_bufferLen; }
		
		unsigned int	length( )const ;
		unsigned int	size( )const { return length(); }

		bool	empty( )const { return m_head==m_tail; }

		void	cleanUp( ) ;

		unsigned int		getHead(){return m_head;}
		unsigned int		getTail(){return m_tail;}
		unsigned int		getBuffLen(){return m_bufferLen;}
		const char*	getBuff(){return m_buffer;}
	private :
		Socket*		m_pSocket ;
		
		char*		m_buffer ;	// 循环buf
		
		unsigned int		m_bufferLen ;
		unsigned int		m_maxBufferLen ;
		
		unsigned int		m_head ;
		unsigned int		m_tail ;



	};

}




#endif
