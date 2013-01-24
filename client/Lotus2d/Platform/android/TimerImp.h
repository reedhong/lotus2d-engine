/*
 * ²Ã¼õ×ÔOgre
 * created by reedhong 2012.07.18
 */

#ifndef __AndroidTimer_H__
#define __AndroidTimer_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	/** Timer class */
	class _OgreExport Timer : public TimerAlloc
	{
	private:
		struct timeval start;
		clock_t zeroClock;
	public:
		Timer();
		~Timer();

		/** Method for setting a specific option of the Timer. These options are usually
            specific for a certain implementation of the Timer class, and may (and probably
            will) not exist across different implementations.  reset() must be called after
			all setOption() calls.
            @param
                strKey The name of the option to set
            @param
                pValue A pointer to the value - the size should be calculated by the timer
                based on the key
            @return
                On success, true is returned.
            @par
                On failure, false is returned.
        */
        bool setOption( const String& strKey, const void* pValue ) { return false; }

		/** Resets timer */
		void reset();

		/** Returns milliseconds since initialisation or last reset */
		unsigned long getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		unsigned long getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMicrosecondsCPU();

		static String nowString(const char* format="%Y-%m-%d %H:%M:%S");
		static void sleep(unsigned long milliSeconds);
	};
}
#endif
