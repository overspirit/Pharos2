#include "PreCompile.h"
#include "Pharos.h"

Timer::Timer()
{
    m_bTimerStopped = true;
	m_nStopTime = 0;
	m_nLastElapsedTime = 0;
	m_nBaseTime = 0;
}

Timer::~Timer(void)
{
    
}

void Timer::Reset()
{
    uint64 time = GetCurrentTime();
	m_nBaseTime = time;
	m_nLastElapsedTime = time;
	m_nStopTime = 0;
	m_bTimerStopped = false;
}

void Timer::Start()
{
    uint64 time = GetCurrentTime();
	if(m_bTimerStopped) m_nBaseTime += (time - m_nStopTime);
	m_nStopTime = 0;
	m_nLastElapsedTime = time;
	m_bTimerStopped = false;
}

void Timer::Stop()
{
    if(!m_bTimerStopped)
	{
		uint64 time = GetCurrentTime();
		m_nStopTime = time;
		m_nLastElapsedTime = time;
		m_bTimerStopped = true;
	}
}

void Timer::Advance()
{
    m_nStopTime += 1000;
}

float64 Timer::GetAbsoluteTime()
{
    return (float64)GetCurrentTime() / 1e9;
}

float64 Timer::GetTime()
{
    uint64 time = (m_nStopTime != 0)? m_nStopTime : GetCurrentTime();
	return (float64)(time - m_nBaseTime) / 1e9;
}

float32 Timer::GetElapsedTime()
{
    uint64 time = (m_nStopTime != 0)? m_nStopTime : GetCurrentTime();
    
	float64 fElapsedTime =  (float64)(time - m_nLastElapsedTime) / 1e9;
	m_nLastElapsedTime = time;
    
	return (float32)fElapsedTime;
}

bool Timer::IsStopped()
{
    return m_bTimerStopped;
}

uint64 Timer::GetCurrentTime()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64)ts.tv_sec * 1e9 + ts.tv_nsec;
}


