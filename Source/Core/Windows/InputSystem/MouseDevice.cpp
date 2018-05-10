#include "PreCompile.h"
#include "CoreGlobal.h"

CMouseDevice::CMouseDevice(void)
{
	m_pDevice = nullptr;

	m_nNewWheel = 0;
	m_nOldWheel = 0;

	ZeroMemory(&m_stMouseState, sizeof(DIMOUSESTATE2));
	ZeroMemory(&m_stOldMouseState, sizeof(DIMOUSESTATE2));
}

CMouseDevice::~CMouseDevice(void)
{
}

bool CMouseDevice::InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid)
{
	//建立硬件接口
	if(FAILED(pDirectInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr))) return false;

	//设定数据格式
	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2))) return false;

	//设定硬件协作等级，默认为前台不独占，鼠标可以更改为独占模式
	if(FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) return false;

	//连接设备
	if(FAILED(m_pDevice->Acquire())) return false;

	//初始化数据
	ZeroMemory(&m_stMouseState, sizeof(DIMOUSESTATE2));
	m_pNewData = &m_stMouseState;

	ZeroMemory(&m_stOldMouseState, sizeof(DIMOUSESTATE2));
	m_pOldData = &m_stOldMouseState;

	m_dwDataSize = sizeof(DIMOUSESTATE2);

	return true;
}

bool CMouseDevice::UpdateDevice()
{
	if(!CInputDevice::UpdateDevice()) return false;

	RECT rt;
	GetClientRect(m_hWnd, &rt);
	int32 lWidth = rt.right - rt.left;
	int32 lHeight = rt.bottom - rt.top;

	m_oldPos = m_newPos;

	//鼠标返回的是移动的距离信息，我们要自己
	//计算位置信息，并限制位置的范围
	m_newPos.x += m_stMouseState.lX;
	m_newPos.y += m_stMouseState.lY;

	//保存移动的距离
	m_moveSize.width = m_stMouseState.lX;
	m_moveSize.height = m_stMouseState.lY;

	m_currPos.x = Math::clamp(m_newPos.x, 0, lWidth);
	m_currPos.y = Math::clamp(m_newPos.y, 0, lHeight);

	//保存鼠标的滚轮数据
	m_nOldWheel = m_nNewWheel;
	m_nNewWheel = m_stMouseState.lZ;

	return true;
}

bool CMouseDevice::IsPositionChanged()
{
	if(m_currPos != m_oldPos || (m_nNewWheel != 0 && m_nNewWheel != m_nOldWheel)) return true;

	return false;
}

bool CMouseDevice::IsMouseButtonUp(MOUSE_CODE key)
{
	//同键盘的原理一样，不同的是鼠标的键最多只有8个
	//如果某位置的数据与0x80为TRUE，这说明该位置的键被按下
	//同理如果上一次该位置的数据为TRUE,且这一次的数据为FALSE，
	//说明该位置的键为弹起状态
	return (((m_stOldMouseState.rgbButtons[key] & 0x80)) && (!(m_stMouseState.rgbButtons[key] & 0x80)));
}

bool CMouseDevice::IsMouseButtonDown(MOUSE_CODE key)
{
	//如果某位置的数据与0x80为TRUE，这说明该位置的键被按下
	return ((m_stMouseState.rgbButtons[key] & 0x80) && true);
}

const Point2Di& CMouseDevice::GetPosition()
{
	return m_currPos;
}

const Size2Di& CMouseDevice::GetMoveDistance()
{
	return m_moveSize;
}

int32 CMouseDevice::GetWheelPosition()
{
	return m_nNewWheel;
}