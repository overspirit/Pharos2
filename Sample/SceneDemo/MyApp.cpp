#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"
#include "Global.h"

const static char8 g_copyShaderVS[] =
"void CopyVS(float4 pos : POSITION, float2 tex0 : TEXCOORD, out float2 oTex : TEXCOORD, out float4 oPos : SV_Position)\n"
"{\n"
"	oPos = pos;\n"
"	oTex = tex0;\n"
"}\n";

const static char8 g_copyShaderPS[] =
"float4 CopyPS(float2 tex : TEXCOORD) : SV_Target\n"
"{\n"
"	return float4(1.0, 1.0, 0, 1.0);\n"
"}\n";

MyApp::MyApp()
{
	m_bLeftDown = false;
	m_bRightDown = false;

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
	param.hdrEnable = true;
	sRenderMgr->StartUp(param);

	sRenderMgr->RegisterRenderCallback(this);

	m_scene = sSceneMgr->CreateScene();
	//scene->SetSceneSize(Size2Di(128, 128), 128);
	sSceneMgr->SetCurrScene(m_scene);

	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter("hero3/hero3.sceneml");
	sceneImporter->ImportScene(m_scene);

	SAFE_DELETE(sceneImporter);

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(5.0f, 5.0f, -5.0f), Vector3Df(0, 0, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);

	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_scene);
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
	if (event.button == MOUSE_LEFT)
	{
		m_bLeftDown = (event.state == STATE_DOWN) ? true : false;
	}
	else if (event.button == MOUSE_RIGHT)
	{
		m_bRightDown = (event.state == STATE_DOWN) ? true : false;
	}
	else
	{
		m_camera->Stretch(event.wheel / 120.0f);
	}

	if (m_bLeftDown)
	{
		m_camera->Slither(Vector2Df((float32)event.ox, (float32)event.oy));
	}
	else if (m_bRightDown)
	{
		m_camera->Round(Vector2Df((float32)event.ox, (float32)event.oy));
	}

	return false;
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
	//m_defFrameBuf->ClearFrameBuffer(0xFFFFFF00);

	//m_renderer->Present();
}

void MyApp::onRender(float32 elapsed)
{
	//m_renderer->BindLayout(m_copyLayout);
	//m_renderer->BindProgram(m_copyShader);

	//m_renderer->DrawImmediate(EDT_TRIANGLELIST, 0, 6);
}