/********************************************************************
	created:	2012/12/12
	filename: 	PacketHander.h
	author:		reedhong
	
	purpose:	每个协议对应一个处理接口
*********************************************************************/

#ifndef __PacketHandler_H__
#define __PacketHandler_H__

#include "Socket/SocketInputStream.h"
using namespace Lotus2d;

typedef void (*PacketHandlerFunc )(Stream &);


class PacketHandler
{
public:
	static void handleLogin(Stream& iStream);
};


#endif