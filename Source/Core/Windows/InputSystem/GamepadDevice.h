#pragma once

namespace Pharos
{
	namespace Core
	{
		class CGamepadDevice : public CInputDevice
		{
		public:
			CGamepadDevice(void);
			virtual ~CGamepadDevice(void);

		private:
			DIJOYSTATE2		m_oldState;
			DIJOYSTATE2		m_newState;

		protected:
			Point2Di	m_currPos;
			Point2Di	m_currPos2;

		public:
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL);

			virtual bool UpdateDevice();

			virtual void SetRestrictSize(int32 nWidth, int32 nHeight);
			virtual void SetInputVibration(uint16 wLeftMotorSpeed, uint16 wRightMotorSpeed);
			virtual void SetDeadZone(int32 nWidth, int32 nHeight);

			virtual bool GamepadButtonUp(uint8 key);
			virtual bool GamepadButtonDown(uint8 key);

			virtual const Point2Di& GetPosition();
			virtual const Point2Di& GetPosition2();
			virtual int32 GetZPosition();
			virtual int32 GetZPosition2();
		};
	}
}