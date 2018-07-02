#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"
#include "DesktopGlobal.h"
#include "Global.h"

#define MOVE_SPEED 10.0f

MyApp::MyApp()
{

}

MyApp::~MyApp()
{
}

IMPL_CREATE_APPLICATION(MyApp)

bool MyApp::Init()
{
	Size2Di wndSize = sKernel->GetWindowSize();

	Render::RenderParam param;
	param.width = wndSize.width;
	param.height = wndSize.height;
	param.sampleType = Render::EMT_None;
	param.sync = false;
	param.fullScreen = false;
	//param.gammaEnabled = false;
	sRenderMgr->StartUp(param);

	sRenderMgr->RegisterRenderCallback(this);

	m_renderer = sRenderMgr->GetCurrentRenderer();
	m_copyTex = m_renderer->LoadTexture("Model/Earth/AtmosphericScattering.bmp");

	m_copyTech = sRenderMgr->GenerateRenderTechnique("Copy");
	m_copyShader = m_copyTech->GetPass(0)->GetShaderProgram();

	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_copyTex);

	SAFE_DELETE(m_copyTech);
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
	return false;
}

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	return false;
}

void MyApp::onControlCreate(const char8* name, int32 v1, float32 v2)
{

}

void MyApp::onControlValueChange(const char8* name, int32 v1, float32 v2)
{
}

void MyApp::Update(float32 fElapsed)
{

}

void MyApp::onRender(float32 elapsed)
{
	m_renderer->BindProgram(m_copyShader);

	sRenderMgr->DrawFullScreenQuad(m_copyTex);
}