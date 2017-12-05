#pragma once

namespace Pharos
{
	namespace Core
	{
		class CInputSystem;

		//////////////////////////////////////////////////////////////////////////
		/*
			desc：输入设备基类
			*/
		//////////////////////////////////////////////////////////////////////////

		class CInputDevice
		{
		protected:
			CInputDevice(void);
			virtual ~CInputDevice(void);

		protected:
			IDirectInputDevice8*	m_pDevice;//输入设备硬件接口
			HWND					m_hWnd;//窗口句柄

			uint32		m_dwDataSize;
			void*		m_pNewData;
			void*		m_pOldData;

		public:
			//初始化系统
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL) = 0;

			//重新连接设备
			virtual bool Reset();

			//关闭硬件
			virtual void Shutdown();

			//更新系统
			virtual bool UpdateDevice();
		};
	}
}
