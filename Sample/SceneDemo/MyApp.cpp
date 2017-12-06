#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
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
"	return float4(1.0, 0, 1.0, 1.0);\n"
"}\n";

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
	param.hdrEnable = true;
	sRenderMgr->StartUp(param);

	sRenderMgr->RegisterRenderCallback(this);

	m_renderer = sRenderMgr->GetCurrentRenderer();

	Vertex vertData[] =
	{
		{ Vector3Df(-0.5f,  0.5f, 0), Vector2Df(0, 0) },
		{ Vector3Df(0.5f, 0.5f, 0), Vector2Df(1.0f, 0) },
		{ Vector3Df(0.5f, -0.5f, 0), Vector2Df(1.0f, 1.0f) },

		{ Vector3Df(0.5f, -0.5f, 0), Vector2Df(1.0f, 1.0f) },
		{ Vector3Df(-0.5f, -0.5f, 0), Vector2Df(0, 1.0f) },
		{ Vector3Df(-0.5f, 0.5f, 0), Vector2Df(0, 0) },
	};

	MemoryBuffer vertDataBuf;
	vertDataBuf.CopyFrom(vertData, sizeof(vertData));
	m_copyLayout = m_renderer->GenerateRenderLayout(sizeof(vertData), &vertDataBuf);

	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, "POSITION", 0, 0 },
		{ VET_FLOAT32, 2, "TEXCOORD", 0, 12 },
	};
	m_copyLayout->SetInputLayoutDesc(desc, 2);

	m_copyShader = m_renderer->GenerateRenderProgram();
	m_copyShader->CompileVertexShader(g_copyShaderVS, "CopyVS");
	m_copyShader->CompilePixelShader(g_copyShaderPS, "CopyPS");

	return true;
}

void MyApp::Destroy()
{
	//SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_copyLayout);
	SAFE_DELETE(m_copyShader);
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
	//m_defFrameBuf->ClearFrameBuffer(0xFFFFFF00);

	//m_renderer->Present();
}

void MyApp::onRender(float32 elapsed)
{
	m_renderer->BindLayout(m_copyLayout);
	m_renderer->BindProgram(m_copyShader);

	m_renderer->DrawImmediate(EDT_TRIANGLELIST, 0, 6);
}