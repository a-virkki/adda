/* File: prec_time.h
 * $Author$
 * $Date::                            $
 * Descr: definitions of inline functions for precise timing
 *
 * Copyright (C) 2006-2008 University of Amsterdam
 * This code is covered by the GNU General Public License.
 */
#ifndef __prec_time_h
#define __prec_time_h

/* Precise timing gives an accuracy of order micro_sec. It gives extensive information on timing of
 * FFT initialization, D-matrix initialization, and Matrix Vector multiplication. It is optimized
 * to consume as little time as possible by itself. It is used mostly for locating and optimizing
 * the bottlenecks of the code execution. It is not ANSI C, therefore is system dependent, though
 * is expected to work for most.
 */

//#define PRECISE_TIMING // uncomment to perform precise timing

#ifdef PRECISE_TIMING

#include "os.h"       // for OS definitions
#include "function.h" // for INLINE and function attributes

#ifdef WINDOWS
#	define SYSTEM_TIME LARGE_INTEGER
#elif defined(POSIX)
#	include <sys/time.h> // for timeval and gettimeofday
#	include <stdio.h>    // needed for definition of NULL
#	define SYSTEM_TIME struct timeval
#else
#	error *** Unknown operation system. Precise timing is not supported. ***
#endif

void InitTime(SYSTEM_TIME *t);
void SetTimerFreq(void);
double TimerToSec(const SYSTEM_TIME *t) ATT_PURE;
double DiffSec(const SYSTEM_TIME *t1,const SYSTEM_TIME *t2) ATT_PURE;

//============================================================

INLINE void elapsed(const SYSTEM_TIME *t1,const SYSTEM_TIME *t2,SYSTEM_TIME *res)
// compute time difference
{
#ifdef WINDOWS
	res->QuadPart=t2->QuadPart-t1->QuadPart;
#elif defined(POSIX)
	res->tv_sec=t2->tv_sec-t1->tv_sec;
	res->tv_usec=t2->tv_usec-t1->tv_usec;
#endif
}

//============================================================

INLINE void ElapsedInc(const SYSTEM_TIME *t1,const SYSTEM_TIME *t2,SYSTEM_TIME *res)
// compute time difference, increment result by this value
{
#ifdef WINDOWS
	res->QuadPart+=(t2->QuadPart-t1->QuadPart);
#elif defined(POSIX)
	res->tv_sec+=(t2->tv_sec-t1->tv_sec);
	res->tv_usec+=(t2->tv_usec-t1->tv_usec);
#endif
}

//============================================================

INLINE void GetTime(SYSTEM_TIME *t)
// get current time
{
#ifdef WINDOWS
	QueryPerformanceCounter(t);
#elif defined(POSIX)
	// gettimeofday is described only in POSIX 1003.1-2001, but it should work for many other systems
	gettimeofday(t,NULL);
#endif
}

#endif // PRECISE_TIMING

#endif // __prec_time_h
