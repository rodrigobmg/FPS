#pragma once
#include <windows.h>
#include <string>
#include <xinput.h>
#include <assert.h>
#include "../DesignPattern.h"
#define DXUT_GAMEPAD_TRIGGER_THRESHOLD      30
#undef min // use __min instead
#undef max // use __max instead

class Time
{
public:
	inline double GetAbsolute()	{
		LARGE_INTEGER qwTime = { 0 };
		QueryPerformanceCounter( &qwTime );
		double fTime = qwTime.QuadPart / ( double )m_llQPFTicksPerSec;
		return fTime;
	}

	SINGLETON(Time)

protected:
	Time() {
		// Use QueryPerformanceFrequency to get the frequency of the counter
		LARGE_INTEGER qwTicksPerSec = { 0 };
		QueryPerformanceFrequency( &qwTicksPerSec );
		m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	//--------------------------------------------------------------------------------------
	// Limit the current thread to one processor (the current one). This ensures that timing code 
	// runs on only one processor, and will not suffer any ill effects from power management.
	//--------------------------------------------------------------------------------------

	inline void LimitThreadAffinityToCurrentProc()	{
		HANDLE hCurrentProcess = GetCurrentProcess();

		// Get the processor affinity mask for this process
		DWORD_PTR dwProcessAffinityMask = 0;
		DWORD_PTR dwSystemAffinityMask = 0;

		if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 &&
			dwProcessAffinityMask )
		{
			// Find the lowest processor that our process is allows to run against
			DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ( ( ~dwProcessAffinityMask ) + 1 ) );

			// Set this as the processor that our thread must always run against
			// This must be a subset of the process affinity mask
			HANDLE hCurrentThread = GetCurrentThread();
			if( INVALID_HANDLE_VALUE != hCurrentThread )
			{
				SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
				CloseHandle( hCurrentThread );
			}
		}

		CloseHandle( hCurrentProcess );
	}

	LONGLONG m_llQPFTicksPerSec;
};