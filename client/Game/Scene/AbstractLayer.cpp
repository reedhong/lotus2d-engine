/********************************************************************
	created:	2012/10/22
	filename: 	AbstractLayer.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "AbstractLayer.h"

AbstractLayer::AbstractLayer()
{

}

AbstractLayer::~AbstractLayer()
{

}

void AbstractLayer::draw()
{
	if(m_visible){
		drawImp();
	}
}