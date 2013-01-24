/********************************************************************
	created:	2012/08/21 
	filename: 	Vector2.h
	author:		reedhong 
	
	purpose:	
*********************************************************************/

#ifndef __Lotus_Vector2_H__
#define __Lotus_Vector2_H__


#include "Math.h"

namespace Lotus2d {
	class Vector2f 
	{
	public:
		float x, y;
	public:
		inline Vector2f() { x=y=0.0f;}
		~Vector2f() {}
		// ¿½±´¹¹Ôìº¯Êý
		Vector2f(const Vector2f& a): x(a.x), y(a.y) {}

		Vector2f(float rx, float ry): x(rx), y(ry){}

		Vector2f& operator = (const Vector2f& a)
		{
			x = a.x; y = a.y; 
			return *this;
		}

		inline float operator [] ( const size_t i ) const
		{

			return *(&x+i);
		}

		inline float& operator [] ( const size_t i )
		{

			return *(&x+i);
		}

		bool operator ==(const Vector2f& a) const
		{
			return (x == a.x) && (y == a.y);
		}

		bool operator !=(const Vector2f&a) const
		{
			return (x != a.x) || ( y != a.y ) ;
		}

		void zero()
		{
			x = y =  0.0f;
		}

		Vector2f normalize()
		{
			float magSq = x*x + y*y;
			Vector2f ret = *this;
			if( magSq >Math::EPSINON ){
				float oneOverMag = 1.0f/sqrt(magSq);
				ret.x *= oneOverMag;
				ret.y *= oneOverMag;
			}

			return ret;
		}

		Vector2f operator - () const 
		{
			return Vector2f(-x, -y);
		}

		Vector2f operator +(const Vector2f &a)
		{
			return Vector2f(x+a.x, y+a.y);
		}

		Vector2f operator -(const Vector2f &a)
		{
			return Vector2f(x-a.x, y-a.y);
		}

		Vector2f operator *(float m)
		{
			return Vector2f(x*m, y*m);
		}

		static const Vector2f ZERO;
	}; // end Vector2
}

#endif
