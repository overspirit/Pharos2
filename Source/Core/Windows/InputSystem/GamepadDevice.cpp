#include "PreCompile.h"
#include "Pharos.h"

CGamepadDevice::CGamepadDevice(void)
{
	ZeroMemory(&m_oldState, sizeof(DIJOYSTATE2));
	ZeroMemory(&m_newState, sizeof(DIJOYSTATE2));
}

CGamepadDevice::~CGamepadDevice(void)
{
}

bool CGamepadDevice::InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid)
{
	if(FAILED(pDirectInput->CreateDevice(guid, &m_pDevice, nullptr))) return false;

	//设定数据格式
	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick2))) return false;

	//设定硬件协作等级
	DWORD dwFlag = DISCL_FOREGROUND;
#ifdef _DEBUG
	dwFlag = DISCL_BACKGROUND;
#endif
	if(FAILED(m_pDevice->SetCooperativeLevel(hWnd, dwFlag | DISCL_EXCLUSIVE))) return false;

	//连接设备
	if(FAILED(m_pDevice->Acquire())) return false;

	m_pOldData = &m_oldState;
	m_pNewData = &m_newState;
	m_dwDataSize = sizeof(DIJOYSTATE2);

	return true;
}

bool CGamepadDevice::UpdateDevice()
{
	return CInputDevice::UpdateDevice();
}

void CGamepadDevice::SetRestrictSize(int32 nWidth, int32 nHeight)
{

}

void CGamepadDevice::SetInputVibration(uint16 wLeftMotorSpeed, uint16 wRightMotorSpeed)
{

}

void CGamepadDevice::SetDeadZone(int32 nWidth, int32 nHeight)
{

}

bool CGamepadDevice::GamepadButtonUp(uint8 key)
{
	return false;
}

bool CGamepadDevice::GamepadButtonDown(uint8 key)
{
	return false;
}

const Point2Di& CGamepadDevice::GetPosition()
{
	return m_currPos;
}

const Point2Di& CGamepadDevice::GetPosition2()
{
	return m_currPos2;
}

int32 CGamepadDevice::GetZPosition()
{
	return 0;
}

int32 CGamepadDevice::GetZPosition2()
{
	return 0;
}