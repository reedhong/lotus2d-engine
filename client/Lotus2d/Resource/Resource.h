/********************************************************************
	created:	2012/12/02
	filename: 	Resource.h
	author:		reedhong
	
	purpose:	资源, 系统中的资源继承于此，资源的释放调用release接口
						资源通过指针访问，挂接到不同的对象上去，挂接到新的对象时
						记得释放
*********************************************************************/

#ifndef __Lotus2d_Resource_H__
#define __Lotus2d_Resource_H__

#include "Base/ObjectBase.h"

namespace Lotus2d {

	enum eResType{
		eResSprite = 0,
		eResPatchSprite,
		eResTexture,
	};

	class Resource: public ObjectBase
	{
	protected:
		Resource();
		virtual ~Resource();
	public:
		void release();
		void addRef();
		void resetRef() { m_refCount = 1;}
	private:
		int	m_refCount;
	};

}

#endif