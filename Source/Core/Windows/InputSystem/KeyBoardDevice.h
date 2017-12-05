#pragma once

namespace Pharos
{
	namespace Core
	{
		//////////////////////////////////////////////////////////////////////////
		/*
			desc:键盘设备
			*/
		//////////////////////////////////////////////////////////////////////////

		class CKeyBoardDevice : public CInputDevice
		{
		public:
			CKeyBoardDevice(void);
			virtual ~CKeyBoardDevice(void);

		private:
			//键盘当前状态
			uint8* m_pNewKeys;

			//键盘上一次调用的状态
			uint8* m_pOldKeys;

		public:
			virtual bool InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid = GUID_NULL);

			virtual bool IsKeyBoardButtonUp(KEY_CODE key);
			virtual bool IsKeyBoardButtonDown(KEY_CODE key);
		};
	}
}
