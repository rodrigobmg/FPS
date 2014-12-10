#pragma once
#include "class.Time.h"

class Timer : public Time
{
public:
    Timer();
    void Reset(); // resets the timer
    void Start(); // starts the timer
    void Stop();  // stop (or pause) the timer
    void Advance(); // advance the timer by 0.1 seconds
    double GetTime(); // get the current time
    float GetElapsedTime(); // get the time that elapsed between Get*ElapsedTime() calls
    void GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime ); // get all time values at once
    bool IsStopped(); // returns true if timer stopped

private:
    LARGE_INTEGER   GetAdjustedCurrentTime();
    bool m_bUsingQPF;
    bool m_bTimerStopped;
    LONGLONG m_llStopTime;
    LONGLONG m_llLastElapsedTime;
    LONGLONG m_llBaseTime;
};


inline Timer::Timer()	{
    m_bTimerStopped = true;
    m_llStopTime = 0;
    m_llLastElapsedTime = 0;
    m_llBaseTime = 0;
}
inline void Timer::Reset()	{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    m_llBaseTime = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_llStopTime = 0;
    m_bTimerStopped = FALSE;
}
inline void Timer::Start()	{
    // Get the current time
    LARGE_INTEGER qwTime = { 0 };
    QueryPerformanceCounter( &qwTime );

    if( m_bTimerStopped) m_llBaseTime += qwTime.QuadPart - m_llStopTime;
    m_llStopTime = 0;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = FALSE;
}
inline void Timer::Stop()	{
    if( !m_bTimerStopped ) return;
    LARGE_INTEGER qwTime = { 0 };
    QueryPerformanceCounter( &qwTime );
    m_llStopTime = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = TRUE;
}
inline void Timer::Advance()	{
    m_llStopTime += m_llQPFTicksPerSec / 10;
}
inline double Timer::GetTime()	{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    double fAppTime = ( double )( qwTime.QuadPart - m_llBaseTime ) / ( double )m_llQPFTicksPerSec;
    return fAppTime;
}
inline void Timer::GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime )
{
    assert( pfTime && pfAbsoluteTime && pfElapsedTime );
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    float fElapsedTime = ( float )( ( double )( qwTime.QuadPart - m_llLastElapsedTime ) / ( double )
                                    m_llQPFTicksPerSec );
    m_llLastElapsedTime = qwTime.QuadPart;

    // Clamp the timer to non-negative values to ensure the timer is accurate.
    // fElapsedTime can be outside this range if processor goes into a 
    // power save mode or we somehow get shuffled to another processor.  
    // However, the main thread should call SetThreadAffinityMask to ensure that 
    // we don't get shuffled to another processor.  Other worker threads should NOT call 
    // SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
    // the main thread.
    if( fElapsedTime < 0.0f )
        fElapsedTime = 0.0f;

    *pfAbsoluteTime = qwTime.QuadPart / ( double )m_llQPFTicksPerSec;
    *pfTime = ( qwTime.QuadPart - m_llBaseTime ) / ( double )m_llQPFTicksPerSec;
    *pfElapsedTime = fElapsedTime;
}
inline float Timer::GetElapsedTime()
{
    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();
    double fElapsedTime = ( float )( ( double )( qwTime.QuadPart - m_llLastElapsedTime ) / ( double ) m_llQPFTicksPerSec );
    m_llLastElapsedTime = qwTime.QuadPart;
    // See the explanation about clamping in Timer::GetTimeValues()
    if( fElapsedTime < 0.0f ) fElapsedTime = 0.0f;
    return ( float )fElapsedTime;
}

//--------------------------------------------------------------------------------------
// If stopped, returns time when stopped otherwise returns current time
//--------------------------------------------------------------------------------------
inline LARGE_INTEGER Timer::GetAdjustedCurrentTime()	{
    LARGE_INTEGER qwTime;
    if( m_llStopTime != 0 )
        qwTime.QuadPart = m_llStopTime;
    else
        QueryPerformanceCounter( &qwTime );
    return qwTime;
}

inline bool Timer::IsStopped()	{
    return m_bTimerStopped;
}


