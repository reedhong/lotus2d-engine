/********************************************************************
	created:	2012/12/12
	filename: 	PacketHandler.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "PacketHandler.h"
#include "Game.h"

void PacketHandler::handleLogin(Stream& iStream)
{
	int8 state = iStream.read8();
}
