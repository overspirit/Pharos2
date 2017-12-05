#pragma once

namespace Pharos
{
	namespace Core
	{
		enum MouseType
		{
			SOFEWARE_MOUSE,
			HARDWARE_MOUSE,
		};

		class CInputManager : public Utils::Singleton<CInputManager>
		{
		public:
			CInputManager(void);
			virtual ~CInputManager(void);

		protected:
			HWND			m_hWnd;

			IDirectInput8*				m_pDirectInput;		//Input接口
			CKeyBoardDevice*			m_pKeyboardDevice;	//键盘硬件
			CSofeMouseDevice*			m_pSofeMouse;		//兼容鼠标
			CMouseDevice*				m_pMouseDevice;		//鼠标硬件
			CGamepadDevice*			m_pCurrGamepad;

			MouseType					m_emMouseType;
			vector<DWORD>				m_vtXInputIDList;
			vector<CGamepadDevice*>		m_vtGamepadList;
			
		private:
			static BOOL EnumInputDevice(const DIDEVICEINSTANCEA* pDeviceInstance, void* pUserContext);

			HRESULT SetupForIsXInputDevice();

		public:
			//初始化系统
			virtual bool InitInputDevice(HWND hWnd, HINSTANCE hInst);

			//重新连接设备
			virtual bool ResetInputDevce();

			//关闭硬件
			virtual void Shutdown();

			//更新系统
			virtual bool UpdateInputDevice();
		};
	}
}
