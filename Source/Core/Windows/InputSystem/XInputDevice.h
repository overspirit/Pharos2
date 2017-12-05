#pragma once

namespace Pharos
{
	namespace Core
	{
		class CXInputDevice : public CGamepadDevice
		{
		public:
			CXInputDevice(void);
			virtual ~CXInputDevice(void);

		private:
			XINPUT_STATE	m_Data;

		public:
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL);

			virtual bool UpdateDevice();
		};
	}
}
