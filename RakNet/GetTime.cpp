/// \file
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.


#if defined(_WIN32) && !defined(_XBOX) && !defined(X360)
#include "WindowsIncludes.h"
// To call timeGetTime
// on Code::Blocks, this needs to be libwinmm.a instead
#pragma comment(lib, "Winmm.lib")
#endif

#include "GetTime.h"
#if defined(_XBOX) || defined(X360)
                            
#endif
#if defined(_WIN32)
DWORD mProcMask;
DWORD mSysMask;
HANDLE mThread;
static LARGE_INTEGER yo;
#elif defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
                                                                                                                                                                                                  
#else
#include <sys/time.h>
#include <unistd.h>
static timeval tp;
RakNet::TimeUS initialTime;
#endif

static bool initialized=false;

#if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
#include "SimpleMutex.h"
RakNet::TimeUS lastNormalizedReturnedValue=0;
RakNet::TimeUS lastNormalizedInputValue=0;
/// This constraints timer forward jumps to 1 second, and does not let it jump backwards
/// See http://support.microsoft.com/kb/274323 where the timer can sometimes jump forward by hours or days
/// This also has the effect where debugging a sending system won't treat the time spent halted past 1 second as elapsed network time
RakNet::TimeUS NormalizeTime(RakNet::TimeUS timeIn)
{
	RakNet::TimeUS diff, lastNormalizedReturnedValueCopy;
	static RakNet::SimpleMutex mutex;
	
	mutex.Lock();
	if (timeIn>lastNormalizedInputValue)
	{
		diff = timeIn-lastNormalizedInputValue;
		if (diff > GET_TIME_SPIKE_LIMIT)
			lastNormalizedReturnedValue+=GET_TIME_SPIKE_LIMIT;
		else
			lastNormalizedReturnedValue+=diff;
	}

	lastNormalizedInputValue=timeIn;
	lastNormalizedReturnedValueCopy=lastNormalizedReturnedValue;
	mutex.Unlock();

	return lastNormalizedReturnedValueCopy;
}
#endif // #if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
RakNet::Time RakNet::GetTime( void )
{
	return (RakNet::Time)(GetTimeUS()/1000);
}
RakNet::TimeMS RakNet::GetTimeMS( void )
{
	return (RakNet::TimeMS)(GetTimeUS()/1000);
}
#if defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
#elif defined(_XBOX) || defined(X360)
                                                                                                                                                                                                                                                                                                                                                                                                                                                           
#elif defined(_WIN32) && !defined(_XBOX) && !defined(X360)
RakNet::TimeUS GetTimeUS_Windows( void )
{
	if ( initialized == false)
	{
		initialized = true;

		// Save the current process
#if !defined(_WIN32_WCE)
		HANDLE mProc = GetCurrentProcess();

		// Get the current Affinity
#if _MSC_VER >= 1400 && defined (_M_X64)
		GetProcessAffinityMask(mProc, (PDWORD_PTR)&mProcMask, (PDWORD_PTR)&mSysMask);
#else
		GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);
#endif
		mThread = GetCurrentThread();

#endif // _WIN32_WCE
		QueryPerformanceFrequency( &yo );
	}	

	RakNet::TimeUS curTime;
	LARGE_INTEGER PerfVal;
	QueryPerformanceCounter( &PerfVal );
	__int64 quotient, remainder;
	quotient=((PerfVal.QuadPart) / yo.QuadPart);
	remainder=((PerfVal.QuadPart) % yo.QuadPart);
	curTime = (RakNet::TimeUS) quotient*(RakNet::TimeUS)1000000 + (remainder*(RakNet::TimeUS)1000000 / yo.QuadPart);

#if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
	return NormalizeTime(curTime);
#else
	return curTime;
#endif // #if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
}
#elif (defined(__GNUC__)  || defined(__GCCXML__))
RakNet::TimeUS GetTimeUS_Linux( void )
{
	if ( initialized == false)
	{
		gettimeofday( &tp, 0 );
		initialized=true;
		// I do this because otherwise RakNet::Time in milliseconds won't work as it will underflow when dividing by 1000 to do the conversion
		initialTime = ( tp.tv_sec ) * (RakNet::TimeUS) 1000000 + ( tp.tv_usec );
	}

	// GCC
	RakNet::TimeUS curTime;
	gettimeofday( &tp, 0 );

	curTime = ( tp.tv_sec ) * (RakNet::TimeUS) 1000000 + ( tp.tv_usec );

#if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
	return NormalizeTime(curTime - initialTime);
#else
	return curTime - initialTime;
#endif // #if defined(GET_TIME_SPIKE_LIMIT) && GET_TIME_SPIKE_LIMIT>0
}
#endif

RakNet::TimeUS RakNet::GetTimeUS( void )
{
#if defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
                        
#elif defined(_XBOX) || defined(X360)
                        
#elif defined(_WIN32)
	return GetTimeUS_Windows();
#else
	return GetTimeUS_Linux();
#endif
}
