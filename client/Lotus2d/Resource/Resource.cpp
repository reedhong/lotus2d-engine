/********************************************************************
	created:	2012/12/02
	filename: 	Resource.cpp
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Resource.h"
#include "ResManagerBase.h"

namespace Lotus2d {
	Resource::Resource()
	{
		m_refCount = 1;

	}

	Resource::~Resource()
	{
		
	}

	void Resource::release()
	{
		m_refCount--;
		// 通过检测的方式释放资源
		if(m_refCount == 0){
			ResourceManagerBase::getInstance()->releaseResouce(this);
			delete this;
		}
	}

	void Resource::addRef()
	{
		m_refCount++;
	}
}
