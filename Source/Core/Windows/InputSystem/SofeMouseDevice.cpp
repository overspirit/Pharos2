#include "PreCompile.h"
#include "Pharos.h"

CSofeMouseDevice::CSofeMouseDevice(void)
{
}

CSofeMouseDevice::~CSofeMouseDevice(void)
{
}

bool CSofeMouseDevice::InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid)
{
	//建立硬件接口
	if(FAILED(pDirectInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr))) return false;

	//设定数据格式
	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2))) return false;

	//设定硬件协作等级，默认为前台不独占，鼠标可以更改为独占模式
	DWORD dwFlag = DISCL_FOREGROUND;
#ifdef _DEBUG
	dwFlag = DISCL_BACKGROUND;
#endif
	if(FAILED(m_pDevice->SetCooperativeLevel(hWnd, dwFlag | DISCL_NONEXCLUSIVE))) return false;	

	//连接设备
	if(FAILED(m_pDevice->Acquire())) return false;

	//初始化数据
	ZeroMemory(&m_stMouseState, sizeof(DIMOUSESTATE2));
	m_pNewData = &m_stMouseState;

	ZeroMemory(&m_stOldMouseState, sizeof(DIMOUSESTATE2));
	m_pOldData = &m_stOldMouseState;

	m_dwDataSize = sizeof(DIMOUSESTATE2);

	m_hWnd = hWnd;

	return true;
}

bool CSofeMouseDevice::UpdateDevice()
{
	if(!CInputDevice::UpdateDevice()) return false;

	m_oldPos = m_currPos;

	GetCursorPos((POINT*)&m_currPos);			// 获取鼠标指针位置（屏幕坐标）
	ScreenToClient(m_hWnd, (POINT*)&m_currPos);	// 将鼠标指针位置转换为窗口坐标

	m_moveSize.width = m_currPos.x - m_oldPos.x;
	m_moveSize.height = m_currPos.y - m_oldPos.y;

	//保存鼠标的滚轮数据
	m_nOldWheel = m_nNewWheel;
	m_nNewWheel = m_stMouseState.lZ;

	return true;
}