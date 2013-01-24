/********************************************************************
	created:	2012/09/19
	filename: 	ObjectBase.inl
	author:		reedhong
	
	purpose:	
*********************************************************************/

#include "Base/Memory.h"

namespace Lotus2d {

	inline void* ObjectBase::operator new( size_t aSize )
	{
		return  lotus2d_malloc( aSize );
	}

	inline void ObjectBase::operator delete( void* aPtr )
	{
		lotus2d_free( aPtr );
	}

	inline void* ObjectBase::operator new [] (size_t aSize )
	{
		return lotus2d_malloc( aSize );
	}

	inline void ObjectBase::operator delete [] ( void* aPtr )
	{
		lotus2d_free( aPtr );
	}

}