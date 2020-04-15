#include "PreCompile.h"
#include "Pharos.h"

Timer::Timer()
{
    m_bTimerStopped = true;
	m_nStopTime = 0;
	m_nLastElapsedTime = 0;
	m_nBaseTime = 0;
    
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);
    m_fDenomRatio = (float64)timebase.numer / (float64)timebase.denom;
}

Timer::~Timer(void)
{
    
}

void Timer::Reset()
{
    uint64 time = mach_absolute_time();
	m_nBaseTime = time;
	m_nLastElapsedTime = time;
	m_nStopTime = 0;
	m_bTimerStopped = false;
}

void Timer::Start()
{
    uint64 time = mach_absolute_time();
	if(m_bTimerStopped) m_nBaseTime += (time - m_nStopTime);
	m_nStopTime = 0;
	m_nLastElapsedTime = time;
	m_bTimerStopped = false;
}

void Timer::Stop()
{
    if(!m_bTimerStopped)
	{
		uint64 time = mach_absolute_time();
		m_nStopTime = time;
		m_nLastElapsedTime = time;
		m_bTimerStopped = true;
	}
}

void Timer::Advance()
{
    m_nStopTime += (m_fDenomRatio * 1e8);
}

float64 Timer::GetAbsoluteTime()
{
    return mach_absolute_time() * m_fDenomRatio / 1e9;
}

float64 Timer::GetTime()
{
    uint64 time = GetAdjustedCurrentTime();
	return (time - m_nBaseTime ) * m_fDenomRatio / 1e9;
}

float32 Timer::GetElapsedTime()
{
    uint64 time = GetAdjustedCurrentTime();
    
	float64 fElapsedTime =  (float64)(time - m_nLastElapsedTime) * m_fDenomRatio / 1e9;
	m_nLastElapsedTime = time;
    
	return (float32)fElapsedTime;
}

bool Timer::IsStopped()
{
    return m_bTimerStopped;
}

uint64 Timer::GetAdjustedCurrentTime()
{
    uint64 time = 0;
	if( m_nStopTime != 0 )
		time = m_nStopTime;
	else
		time = mach_absolute_time();
	return time;
}


