/********************************************************************
	created:	2012/10/30
	filename: 	Math.h
	author:		reedhong
	
	purpose:	
*********************************************************************/

#ifndef __Lotus2d_Math_H__
#define __Lotus2d_Math_H__

#include <cmath>
#include <cstdlib>

namespace Lotus2d
{
	/** @def CCRANDOM_MINUS1_1
	 returns a random float between -1 and 1
	 */
	#define RANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)

	/** @def CCRANDOM_0_1
	 returns a random float between 0 and 1
	 */
	#define RANDOM_0_1() ((float)rand()/RAND_MAX)

	/** @def CC_DEGREES_TO_RADIANS
	 converts degrees to radians
	 */
	#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

	/** @def CC_RADIANS_TO_DEGREES
	 converts radians to degrees
	 */
	#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180
	
		#define SWAP(x, y, type)	\
		{	type temp = (x);		\
		x = y; y = temp;		\
		}


	class Math{
	public:
		static float  EPSINON;
		static float PI;
		static float Deg2Rad;
		static float Rad2Deg;
	public:
		Math();
		~Math();

	public:
		//////////////////////////////////////////////////////////////////////////
		// 封装一些常用的数学函数，为后续扩展或者加速做准备
		static float abs(float f)
		{
			return std::abs(f);
		}

		static float fabs(float f)
		{
			return std::fabs(f);
		}
		static float cos(float f)
		{
			return std::cos(f);
		}

		static float acos(float fValue)
		{
			if ( -1.0 < fValue )
			{
				if ( fValue < 1.0 )
					return std::acos(fValue);
				else
					return 0.0;
			}
			else
			{
				return PI;
			}
		}

		static float sin(float f)
		{
			return std::sin(f);
		}

		static float tan(float f)
		{
			return std::tan(f);
		}

		static float  atan2(float s, float c)
		{
			return std::atan2(s, c);
		}

		static float sqrt(float f)
		{
			return std::sqrt(f);
		}

		static float inv_sqrt(float f)
		{
			return 1.0f/std::sqrt(f);
		}

		static bool floatEqual( float a, float b, float tolerance = EPSINON )
		{
			if (fabs(b-a) <= tolerance)
				return true;
			else
				return false;
		}
		static inline float DegreesToRadians(float degrees) { return degrees * Deg2Rad; }
		static inline float RadiansToDegrees(float radians) { return radians * Rad2Deg; }

		static inline bool isNaN(float f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

		static float clampf(float value, float min_inclusive, float max_inclusive)
		{
			if (min_inclusive > max_inclusive) {
				SWAP(min_inclusive, max_inclusive, float);
			}
			return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
		}

	}; // end Math

	class Color4f
	{
	public:
		float	a,r, g, b;
	public:
		Color4f()
		{
			a = r=g=b = 0.0f;
		}
		Color4f(const Color4f &c)
		{
			a = c.a;
			r = c.r;
			g = c.g;
			b = c.b;
		}
		Color4f& operator =(const Color4f& c)
		{
			a = c.a;
			r = c.r;
			g = c.g;
			b = c.b;

			return (*this);
		}

		void setBGR(int bgr)
		{
			b =  ((bgr>>16) & 0xFF)/255.0f;
			g =  ((bgr>>8) & 0xFF)/255.0f;
			r =  ((bgr) & 0xFF)/255.0f;
		}
		
		int getBGR()
		{
				int iRet = (int)(r*255.0f) + (((int)(g*255.0f))<<8)+ (((int)(b*255.0f))<<16);
				return iRet;
		}

		int getARGB()
		{
			int iRet = (int)(b*255.0f) + (((int)(g*255.0f))<<8)+ (((int)(r*255.0f))<<16)+(((int)(a*255.0f))<<24);
			return iRet;
		}

		void setAlpha(unsigned char alpha)
		{
			a = alpha/255.0f;
		}

		int getAlpha()
		{
			return (int)(a*255.0f);
		}
	};

	class Color4b
	{
	public:
		unsigned char	m_a,m_r, m_g, m_b;
	public:
		Color4b()
		{
			m_a = m_r=m_g=m_b = 0;
		}
		Color4b(const Color4b &c)
		{
			m_a = c.m_a;
			m_r = c.m_r;
			m_g = c.m_g;
			m_b = c.m_b;
		}
		Color4b& operator =(const Color4b& c)
		{
			m_a = c.m_a;
			m_r = c.m_r;
			m_g = c.m_g;
			m_b = c.m_b;
		}
	};

}

#endif