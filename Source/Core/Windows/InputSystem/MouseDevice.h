#pragma once

namespace Pharos
{
	namespace Core
	{
		//////////////////////////////////////////////////////////////////////////
		/*
			desc:鼠标状态
			*/
		//////////////////////////////////////////////////////////////////////////

		class CMouseDevice : public CInputDevice
		{
		public:
			CMouseDevice(void);
			virtual ~CMouseDevice(void);

		protected:
			//鼠标当前状态
			DIMOUSESTATE2 m_stMouseState;

			//鼠标上次调用的状态
			DIMOUSESTATE2 m_stOldMouseState;

			//鼠标位置
			Point2Di	m_currPos;
			Point2Di	m_oldPos;
			Point2Di	m_newPos;

			//鼠标移动距离
			Size2Di		m_moveSize;

			//鼠标滚轮移动距离
			int32		m_nNewWheel;
			int32		m_nOldWheel;

		public:
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL);

			virtual bool UpdateDevice();

			virtual bool IsPositionChanged();

			virtual bool IsMouseButtonUp(MOUSE_CODE key);
			virtual bool IsMouseButtonDown(MOUSE_CODE key);

			virtual const Point2Di& GetPosition();
			virtual const Size2Di& GetMoveDistance();
			virtual int32 GetWheelPosition();
		};
	}
}