#include "PreCompile.h"
#include "Pharos.h"

//--------------------------------------------------------------------------------------
Timer::Timer()
{
	m_bTimerStopped = true;
	m_llQPFTicksPerSec = 0;

	m_llStopTime = 0;
	m_llLastElapsedTime = 0;
	m_llBaseTime = 0;

	// Use QueryPerformanceFrequency to get the frequency of the counter
	LARGE_INTEGER qwTicksPerSec = { 0 };
	QueryPerformanceFrequency( &qwTicksPerSec );
	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}

//--------------------------------------------------------------------------------------
Timer::~Timer()
{
}


//--------------------------------------------------------------------------------------
void Timer::Reset()
{
	LARGE_INTEGER qwTime = {0};
	QueryPerformanceCounter(&qwTime);

	m_llBaseTime = qwTime.QuadPart;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_llStopTime = 0;
	m_bTimerStopped = false;
}

//--------------------------------------------------------------------------------------
void Timer::Start()
{
	// Get the current time
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter( &qwTime );

	if( m_bTimerStopped )
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
	m_llStopTime = 0;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_bTimerStopped = false;
}


//--------------------------------------------------------------------------------------
void Timer::Stop()
{
	if( !m_bTimerStopped )
	{
		LARGE_INTEGER qwTime = { 0 };
		QueryPerformanceCounter( &qwTime );
		m_llStopTime = qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
		m_bTimerStopped = true;
	}
}

//--------------------------------------------------------------------------------------
float64 Timer::GetTime()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	double fAppTime = ( double )( qwTime.QuadPart - m_llBaseTime ) / ( double )m_llQPFTicksPerSec;

	return fAppTime;
}

//--------------------------------------------------------------------------------------
float32 Timer::GetElapsedTime()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	double fElapsedTime =  (double)(qwTime.QuadPart - m_llLastElapsedTime) / (double)m_llQPFTicksPerSec;
	m_llLastElapsedTime = qwTime.QuadPart;

	return (float32)fElapsedTime;
}


//--------------------------------------------------------------------------------------
// If stopped, returns time when stopped otherwise returns current time
//--------------------------------------------------------------------------------------
LARGE_INTEGER Timer::GetAdjustedCurrentTime()
{
	LARGE_INTEGER qwTime;
	if( m_llStopTime != 0 )
		qwTime.QuadPart = m_llStopTime;
	else
		QueryPerformanceCounter( &qwTime );
	return qwTime;
}

//--------------------------------------------------------------------------------------
bool Timer::IsStopped()
{
	return m_bTimerStopped;
}
