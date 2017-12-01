#pragma once

namespace Pharos
{
	namespace Core
	{
		class Timer //: public ITimer
		{
		public:
			Timer();
			virtual ~Timer();

		protected:
			LARGE_INTEGER   GetAdjustedCurrentTime();

			bool m_bTimerStopped;
			LONGLONG m_llQPFTicksPerSec;

			LONGLONG m_llStopTime;
			LONGLONG m_llLastElapsedTime;
			LONGLONG m_llBaseTime;

		public:
			virtual void	Reset();
			virtual void	Start();
			virtual void	Stop();
			virtual float64	GetTime();
			virtual float32	GetElapsedTime();
			virtual bool	IsStopped();
		};
	}
}