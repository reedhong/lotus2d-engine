/*
 * ²Ã¼õ×ÔOgre
 * created by reedhong 2012.07.18
 */
#ifndef __Win32Timer_H__
#define __Win32Timer_H__

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>
#include <ctime>
using namespace std;

#include "Base/Prerequisites.h"

namespace Lotus2d
{
	class  Timer
    {
    private:
		clock_t mZeroClock;
        DWORD mStartTick;
		LONGLONG mLastTime;
        LARGE_INTEGER mStartTime;
        LARGE_INTEGER mFrequency;

		DWORD_PTR mTimerMask;
    public:
		/** Timer constructor.  MUST be called on same thread that calls getMilliseconds() */
		Timer();
		~Timer();

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

		static string nowString(const char* format="%Y-%m-%d %H:%M:%S");

		/*
		 * ºÁÃë
		 * 1 second = 1,000 microseconds = 1,000,000 milliseconds
		 */
		static void sleep(unsigned long milliSeconds);
    };
	/** @} */
	/** @} */
}
#endif
