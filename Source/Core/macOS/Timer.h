#pragma once

namespace Pharos
{
    namespace Core
    {
        //???!!!需要测试线程睡眠时的计时情况
        class Timer //: public ITimer
        {
        public:
            Timer(void);
            virtual ~Timer(void);
            
        private:
            bool        m_bTimerStopped;
            float64     m_fDenomRatio;
            uint64      m_nStopTime;
            uint64      m_nLastElapsedTime;
            uint64      m_nBaseTime;
            
        private:
            uint64   GetAdjustedCurrentTime();
            
        public:
            virtual void	Reset();
            virtual void	Start();
            virtual void	Stop();
            virtual void	Advance();
            virtual float64	GetAbsoluteTime();
            virtual float64	GetTime();
            virtual float32	GetElapsedTime();
            virtual bool	IsStopped();
        };
    }
}
