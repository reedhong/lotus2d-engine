/*
 * ²Ã¼ô×ÔLotus2d
 * created by reedhong 2012.07.27
 */

#ifndef __Lotus2d2d_Profiler_H__
#define __Lotus2d2d_Profiler_H__

#if Lotus2d_PROFILING == 1
#	define Lotus2dProfile( a ) Lotus2d::Profile _Lotus2dProfileInstance( (a) )
#	define Lotus2dProfileBegin( a ) Lotus2d::Profiler::getSingleton().beginProfile( (a) )
#	define Lotus2dProfileEnd( a ) Lotus2d::Profiler::getSingleton().endProfile( (a) )
#	define Lotus2dProfileGroup( a, g ) Lotus2d::Profile _Lotus2dProfileInstance( (a), (g) )
#	define Lotus2dProfileBeginGroup( a, g ) Lotus2d::Profiler::getSingleton().beginProfile( (a), (g) )
#	define Lotus2dProfileEndGroup( a, g ) Lotus2d::Profiler::getSingleton().endProfile( (a), (g) )
#else
#  define Lotus2dProfile( a )
#  define Lotus2dProfileBegin( a )
#  define Lotus2dProfileEnd( a )
#	define Lotus2dProfileGroup( a, g ) 
#	define Lotus2dProfileBeginGroup( a, g ) 
#	define Lotus2dProfileEndGroup( a, g ) 
#endif

namespace Lotus2d{

}


#endif