#include "PreCompile.h"
#include "CoreGlobal.h"

CXInputDevice::CXInputDevice(void)
{
}

CXInputDevice::~CXInputDevice(void)
{
}

bool CXInputDevice::InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid)
{
	return false;
}

bool CXInputDevice::UpdateDevice()
{
	//XInputGetState(0, &m_Data);

	return false;
}