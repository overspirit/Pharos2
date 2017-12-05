#include "PreCompile.h"
#include "CoreGlobal.h"

CInputManager::CInputManager()
{
	m_hWnd = nullptr;

	m_pDirectInput = nullptr;
	m_pKeyboardDevice = nullptr;
	m_pSofeMouse = nullptr;
	m_pMouseDevice = nullptr;
	m_pCurrGamepad = nullptr;

	m_emMouseType = SOFEWARE_MOUSE;
}

CInputManager::~CInputManager()
{
	SAFE_DELETE(m_pKeyboardDevice);
	SAFE_DELETE(m_pMouseDevice);
	SAFE_DELETE(m_pSofeMouse);

	SAFE_RELEASE(m_pDirectInput);
}

bool CInputManager::InitInputDevice(HWND hWnd, HINSTANCE hInst)
{
	if(!hWnd || !IsWindow(hWnd)) return false;

	m_hWnd = hWnd;

	SetupForIsXInputDevice();

	//建立输入接口
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, nullptr))) return false;

	//枚举全部已经连接的设备,有问题...
	//m_pDirectInput->EnumDevices(DI8DEVCLASS_ALL, (LPDIENUMDEVICESCALLBACKA)&CInputManager::EnumInputDevice, this, DIEDFL_ATTACHEDONLY);

	CKeyBoardDevice* pKeyboardDevice = new CKeyBoardDevice();
	if (!pKeyboardDevice->InitDevice(m_pDirectInput, m_hWnd)) return false;
	m_pKeyboardDevice = pKeyboardDevice;

	CMouseDevice* pMouseDevice = new CSofeMouseDevice();
	if (!pMouseDevice->InitDevice(m_pDirectInput, m_hWnd)) return false;
	m_pMouseDevice = pMouseDevice;

	CSofeMouseDevice* pSofeMouse = new CSofeMouseDevice();
	if (!pSofeMouse->InitDevice(m_pDirectInput, m_hWnd)) return false;
	m_pSofeMouse = pSofeMouse;

	if (m_vtGamepadList.size() > 0) m_pCurrGamepad = m_vtGamepadList[0];

	return true;
}

bool CInputManager::ResetInputDevce()
{
	if(m_pKeyboardDevice != nullptr) m_pKeyboardDevice->Reset();	//键盘硬件
	if(m_pSofeMouse != nullptr) m_pSofeMouse->Reset();		//兼容鼠标
	if(m_pMouseDevice != nullptr) m_pMouseDevice->Reset();		//鼠标硬件

	for(uint32 i = 0; i < m_vtGamepadList.size(); i++)
	{
		m_vtGamepadList[i]->Reset();
	}

	return true;
}

void CInputManager::Shutdown()
{
	if(m_pKeyboardDevice != nullptr) m_pKeyboardDevice->Shutdown();	//键盘硬件
	if(m_pSofeMouse != nullptr) m_pSofeMouse->Shutdown();	
	//兼容鼠标
	if(m_pMouseDevice != nullptr) m_pMouseDevice->Shutdown();		//鼠标硬件

	for(uint32 i = 0; i < m_vtGamepadList.size(); i++)
	{ 
		m_vtGamepadList[i]->Shutdown();
	}
}

bool CInputManager::UpdateInputDevice()
{
	if(!m_pKeyboardDevice->UpdateDevice()) return false;

	CMouseDevice* pMouseDevice = nullptr;
	if(m_emMouseType == SOFEWARE_MOUSE) pMouseDevice = m_pMouseDevice;
	else if(m_emMouseType == HARDWARE_MOUSE) pMouseDevice = m_pSofeMouse;

	if(!pMouseDevice->UpdateDevice()) return false;

	bool ctrl = false;
	if(m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_LCONTROL) 
		|| m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_RCONTROL))
		ctrl = true;

	bool shift = false;
	if(m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_LSHIFT) 
		|| m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_RSHIFT))
		shift = true;

	bool alt = false;
	if(m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_LMENU)
		|| m_pKeyboardDevice->IsKeyBoardButtonDown(KEY_RMENU))
		alt = true;

	KeyEvent keyEvent;
	keyEvent.shift = shift;
	keyEvent.alt = alt;
	keyEvent.ctrl = ctrl;

	for(int32 i = KEY_NULL; i < KEY_NUM; i++)
	{
		if(i == KEY_LCONTROL || i == KEY_RCONTROL) continue;
		if(i == KEY_LSHIFT || i == KEY_RSHIFT) continue;
		if(i == KEY_LMENU || i == KEY_RMENU) continue;

		if(m_pKeyboardDevice->IsKeyBoardButtonDown((KEY_CODE)i))
		{
			keyEvent.key = (KEY_CODE)i;
			keyEvent.state = STATE_DOWN;
			sPlatform->onKeyboardEvent(keyEvent);
		}
		else if(m_pKeyboardDevice->IsKeyBoardButtonUp((KEY_CODE)i))
		{
			keyEvent.key = (KEY_CODE)i;
			keyEvent.state = STATE_UP;
			sPlatform->onKeyboardEvent(keyEvent);
		}
	}

	const Point2Di& pos = pMouseDevice->GetPosition();
	const Size2Di& offset = pMouseDevice->GetMoveDistance();
	int32 wheel = pMouseDevice->GetWheelPosition();

	MouseEvent mouseEvent;
	mouseEvent.button = MOUSE_NUM;
	mouseEvent.state = STATE_KEEP;
	mouseEvent.x = pos.x;
	mouseEvent.y = pos.y;
	mouseEvent.ox = offset.width;
	mouseEvent.oy = offset.height;
	mouseEvent.wheel = wheel;
	mouseEvent.shift = shift;
	mouseEvent.ctrl = ctrl;
	mouseEvent.alt = alt;

	if(pMouseDevice->IsPositionChanged())
	{
		sPlatform->onMouseEvent(mouseEvent);
	}

	for(int32 i = MOUSE_LEFT; i < MOUSE_NUM; i++)
	{		
		if(pMouseDevice->IsMouseButtonDown((MOUSE_CODE)i))
		{
			mouseEvent.button = (MOUSE_CODE)i;
			mouseEvent.state = STATE_DOWN;
			sPlatform->onMouseEvent(mouseEvent);
		}
		else if(pMouseDevice->IsMouseButtonUp((MOUSE_CODE)i))
		{
			mouseEvent.button = (MOUSE_CODE)i;			
			mouseEvent.state = STATE_UP;
			sPlatform->onMouseEvent(mouseEvent);
		}
	}

	return true;
}

BOOL CInputManager::EnumInputDevice(const DIDEVICEINSTANCEA* pDeviceInstance, void* pUserContext)
{
	CInputManager* pManager = (CInputManager*)pUserContext;

	//如果检测到某设备，就建立一个该设备的实例
	switch(pDeviceInstance->dwDevType & 0x000000FF)
	{
	case DI8DEVTYPE_KEYBOARD:
		{
			if(pManager->m_pKeyboardDevice != nullptr) return DIENUM_CONTINUE;

			CKeyBoardDevice* pKeyboardDevice = new CKeyBoardDevice();
			if(!pKeyboardDevice->InitDevice(pManager->m_pDirectInput, pManager->m_hWnd))
			{
				return DIENUM_CONTINUE;
			}
			pManager->m_pKeyboardDevice = pKeyboardDevice;
		}
		break;
	case DI8DEVTYPE_MOUSE:
		{
			if(pManager->m_pMouseDevice != nullptr && pManager->m_pSofeMouse != nullptr) return DIENUM_CONTINUE;

			CMouseDevice* pMouseDevice = new CSofeMouseDevice();
			if(!pMouseDevice->InitDevice(pManager->m_pDirectInput, pManager->m_hWnd))
			{
				return DIENUM_CONTINUE;
			}
			pManager->m_pMouseDevice = pMouseDevice;

			CSofeMouseDevice* pSofeMouse = new CSofeMouseDevice();
			if(!pSofeMouse->InitDevice(pManager->m_pDirectInput, pManager->m_hWnd))
			{
				return DIENUM_CONTINUE;
			}
			pManager->m_pSofeMouse = pSofeMouse;			
		}
		break;
	case DI8DEVTYPE_GAMEPAD:
		{
			CGamepadDevice* pGamepadDevice = nullptr;

			BOOL bIsXInput = FALSE;
			for(UINT i = 0; i < pManager->m_vtXInputIDList.size(); i++)
			{
				if(pDeviceInstance->guidProduct.Data1 == pManager->m_vtXInputIDList[i])
				{
					bIsXInput = TRUE;
				}
			}

			if(bIsXInput)
			{
				pGamepadDevice = new CXInputDevice();
			}
			else
			{
				pGamepadDevice = new CGamepadDevice();
			}

			if(!pGamepadDevice->InitDevice(pManager->m_pDirectInput, pManager->m_hWnd, pDeviceInstance->guidInstance))
			{
				return DIENUM_CONTINUE;
			}
			pManager->m_vtGamepadList.push_back(pGamepadDevice);
		}
		break;
	}

	return DIENUM_CONTINUE;
}

HRESULT CInputManager::SetupForIsXInputDevice()
{
	IWbemServices* pIWbemServices = nullptr;
	IEnumWbemClassObject* pEnumDevices = nullptr;
	IWbemLocator* pIWbemLocator = nullptr;
	IWbemClassObject* pDevices[20] = {0};
	BSTR bstrDeviceID = nullptr;
	BSTR bstrClassName = nullptr;
	BSTR bstrNamespace = nullptr;
	DWORD uReturned = 0;
	bool bCleanupCOM = false;
	UINT iDevice = 0;
	VARIANT var;
	HRESULT hr;

	// CoInit if needed
	hr = CoInitialize( nullptr );
	bCleanupCOM = SUCCEEDED( hr );

	// Create WMI
	hr = CoCreateInstance( __uuidof( WbemLocator ),
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof( IWbemLocator ),
		( LPVOID* )&pIWbemLocator );
	if( FAILED( hr ) || pIWbemLocator == nullptr )
		goto LCleanup;

	// Create BSTRs for WMI
	bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == nullptr ) goto LCleanup;
	bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == nullptr )  goto LCleanup;
	bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == nullptr ) goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer( bstrNamespace, nullptr, nullptr, 0L,
		0L, nullptr, nullptr, &pIWbemServices );
	if( FAILED( hr ) || pIWbemServices == nullptr )
		goto LCleanup;

	// Switch security level to IMPERSONATE
	CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0 );

	// Get list of Win32_PNPEntity devices
	hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, nullptr, &pEnumDevices );
	if( FAILED( hr ) || pEnumDevices == nullptr )
		goto LCleanup;

	// Loop over all devices
	for(; ; )
	{
		// Get 20 at a time
		hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
		if( FAILED( hr ) )
			goto LCleanup;
		if( uReturned == 0 )
			break;

		for( iDevice = 0; iDevice < uReturned; iDevice++ )
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, nullptr, nullptr );
			if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != nullptr )
			{
				// Check if the device ID contains "IG_".  If it does, then it抯 an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if( wcsstr( var.bstrVal, L"IG_" ) )
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
					if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
						dwVid = 0;
					WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
					if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
						dwPid = 0;

					DWORD dwVidPid = MAKELONG( dwVid, dwPid );

					m_vtXInputIDList.push_back(dwVidPid);
				}
			}
			SAFE_RELEASE( pDevices[iDevice] );
		}
	}

LCleanup:
	if( bstrNamespace )
		SysFreeString( bstrNamespace );
	if( bstrDeviceID )
		SysFreeString( bstrDeviceID );
	if( bstrClassName )
		SysFreeString( bstrClassName );
	for( iDevice = 0; iDevice < 20; iDevice++ )
		SAFE_RELEASE( pDevices[iDevice] );
	SAFE_RELEASE( pEnumDevices );
	SAFE_RELEASE( pIWbemLocator );
	SAFE_RELEASE( pIWbemServices );

	return hr;
}