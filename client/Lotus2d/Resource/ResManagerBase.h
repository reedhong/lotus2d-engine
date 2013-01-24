/********************************************************************
	created:	2012/12/03
	filename: 	ResManagerBase.h
	author:		reedhong
	
	purpose:	定义游戏资源管理的接口，主要是方便二进制资源的访问，主要实现两个接口
						
*********************************************************************/

#ifndef __Lotus2d_ResManagerBase_H__
#define __Lotus2d_ResManagerBase_H__

#include "Base/Prerequisites.h"
#include "Resource.h"
#include "Base/Stream.h"

namespace Lotus2d {
	class ResourceManagerBase
	{
	protected:
		ResourceManagerBase();
		virtual ~ResourceManagerBase();
	public:
		static ResourceManagerBase* getInstance();
		static void setInstance(ResourceManagerBase* rmb);
	protected:
		static ResourceManagerBase* s_instance;
	public:
		// 根据资源的key，加载资源
		virtual Resource* loadResouce(unsigned int key, eResType type) = 0;
		// 根据资源的路径，加载资源
		virtual Resource* loadResouce(const string& path, eResType type) = 0;
		virtual void releaseResouce(Resource*  resource) {}

	};
}

#endif
