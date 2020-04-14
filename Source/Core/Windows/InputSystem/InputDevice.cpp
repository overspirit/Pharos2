#include "PreCompile.h"
#include "Pharos.h"

CInputDevice::CInputDevice(void)
{
	m_pDevice = nullptr;
	m_hWnd = nullptr;

	m_dwDataSize = 0;
	m_pNewData = nullptr;
	m_pOldData = nullptr;
}

CInputDevice::~CInputDevice(void)
{
	//关闭设备
	if(m_pDevice)
	{
		m_pDevice->Unacquire();

		SAFE_RELEASE(m_pDevice);
	}
}

void CInputDevice::Shutdown()
{
	//Release();
}

bool CInputDevice::Reset()
{
	if(FAILED(m_pDevice->Acquire()))
	{
		ZeroMemory(m_pOldData, m_dwDataSize);
		ZeroMemory(m_pNewData, m_dwDataSize);
		return false;
	}

	return true;
}

bool CInputDevice::UpdateDevice()
{
	//首先保存上一次的设备数据
	if(m_pNewData && m_pOldData)
	{
		memcpy(m_pOldData, m_pNewData, m_dwDataSize);
		
		//轮询设备
		if(FAILED(m_pDevice->Poll()))
		{
			//如果失去设备则再次尝试连接设备
			if(FAILED(m_pDevice->Acquire()) || FAILED(m_pDevice->Poll()))
			{
				ZeroMemory(m_pOldData, m_dwDataSize);
				ZeroMemory(m_pNewData, m_dwDataSize);
				return false;
			}
		}

		//得到设备数据
		if(FAILED(m_pDevice->GetDeviceState(m_dwDataSize, m_pNewData)))
		{
			//同上操作
			if(FAILED(m_pDevice->Acquire()) || FAILED(m_pDevice->GetDeviceState(m_dwDataSize, m_pNewData)))
			{
				ZeroMemory(m_pOldData, m_dwDataSize);
				ZeroMemory(m_pNewData, m_dwDataSize);
				return false;
			}
		}

		return true;
	}

	return false;
}