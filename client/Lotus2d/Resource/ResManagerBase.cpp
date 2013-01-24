/********************************************************************
	created:	2012/12/03
	filename: 	ResManagerBase.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "ResManagerBase.h"

namespace Lotus2d {
	ResourceManagerBase* ResourceManagerBase::s_instance = NULL;

	ResourceManagerBase::ResourceManagerBase()
	{

	}

	ResourceManagerBase::~ResourceManagerBase()
	{

	}

	ResourceManagerBase* ResourceManagerBase::getInstance()
	{
		return s_instance;
	}
	
	void ResourceManagerBase::setInstance(ResourceManagerBase* rmb)
	{
		s_instance = rmb;
	}

}
