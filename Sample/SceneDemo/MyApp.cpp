#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "Global.h"

MyApp::MyApp()
{
}

MyApp::~MyApp()
{
}

IMPL_CREATE_APPLICATION(MyApp)

bool MyApp::Init()
{
	DeviceConfig cfg;
	cfg.wndHandle = sKernel->GetWindowHandle();
	cfg.width = sKernel->GetWindowSize().width;
	cfg.height = sKernel->GetWindowSize().height;
	cfg.sampleType = EMT_None;
	cfg.sync = false;
	cfg.fullScreen = false;
	cfg.bufferFormat = EPF_RGBA8_UNORM;
	cfg.depthFormat = EPF_D24_UNORM_S8_UINT;
	cfg.refreshRate = 0;

	m_renderer = new D3D11Renderer();
	m_renderer->Init();
	m_renderer->Create(cfg);

	m_defFrameBuf = m_renderer->GetDefaultFrameBuffer();

	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_renderer);
}

void MyApp::onViewCreate()
{

}

void MyApp::onViewDestroy()
{

}

void MyApp::onViewChangeSize(int32 width, int32 height)
{

}

bool MyApp::onMouseEvent(const MouseEvent& event)
{
	return true;
}

bool MyApp::onKeyboardEvent(const KeyEvent& evnet)
{
	return true;
}

void MyApp::onSensorAccelerometerEvent(const Vector3Df& acc, float64 timestamp)
{

}

void MyApp::onSensorMagenticFieldEvent(const Vector3Df& magnetic, float64 timestamp)
{

}

void MyApp::onSensorGyroscopeEvnet(const Vector3Df& gryo, float64 timestamp)
{

}

void MyApp::Update(float32 fElapsed)
{

}

void MyApp::Render(float32 fElapsed)
{
	m_defFrameBuf->ClearFrameBuffer(0xFFFFFF00);

	m_renderer->Present();
}