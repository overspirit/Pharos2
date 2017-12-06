#include "PreCompile.h"
#include "CoreGlobal.h"

#define KEYS_SIZE 256

CKeyBoardDevice::CKeyBoardDevice(void)
{
	m_pDevice = nullptr; 

	m_pNewKeys = nullptr;
	m_pOldKeys = nullptr;
}

CKeyBoardDevice::~CKeyBoardDevice(void)
{
	SAFE_DELETE(m_pNewKeys);
	SAFE_DELETE(m_pOldKeys);
}

bool CKeyBoardDevice::InitDevice(IDirectInput8* pDirectInput, HWND hWnd, const GUID& guid)
{
	//建立设备接口
	if(FAILED(pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr))) return false;

	//设定数据格式
	if(FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard))) return false;

	//设定硬件协作等级，键盘只能为非独占模式
	DWORD dwFlag = DISCL_FOREGROUND;
#ifdef _DEBUG
	dwFlag = DISCL_BACKGROUND;
#endif
	if(FAILED(m_pDevice->SetCooperativeLevel(hWnd, dwFlag | DISCL_NONEXCLUSIVE))) return false;

	//连接设备
	if(FAILED(m_pDevice->Acquire())) return false;

	//初始化数据
	m_pNewKeys = new uint8[KEYS_SIZE];
	ZeroMemory(m_pNewKeys, KEYS_SIZE);
	m_pNewData = m_pNewKeys;

	m_pOldKeys = new uint8[KEYS_SIZE];
	ZeroMemory(m_pOldKeys, KEYS_SIZE);
	m_pOldData = m_pOldKeys;

	m_dwDataSize = KEYS_SIZE;

	return true;
}

bool CKeyBoardDevice::IsKeyBoardButtonUp(KEY_CODE key)
{
	//如果某位置的数据与0x80为TRUE，这说明该位置的键被按下
	//同理如果上一次该位置的数据为TRUE,且这一次的数据为FALSE，
	//说明该位置的键为弹起状态
	return (((m_pOldKeys[key] & 0x80)) && (!(m_pNewKeys[key] & 0x80)));
}

bool CKeyBoardDevice::IsKeyBoardButtonDown(KEY_CODE key)
{
	//如果某位置的数据与0x80为TRUE，这说明该位置的键被按下
	return ((m_pNewKeys[key] & 0x80) && (!(m_pOldKeys[key] & 0x80)));
}