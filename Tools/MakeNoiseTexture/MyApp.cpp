#include "PreCompile.h"
#include "Global.h"

#define MOVE_SPEED 10.0f

MyApp::MyApp()
{
	m_pause = false;
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
	param.gammaEnabled = false;
	sRenderMgr->StartUp(param);

	sRenderMgr->RegisterRenderCallback(this);

	m_renderer = sRenderMgr->GetCurrentRenderer();

	m_mainTex = m_renderer->LoadTexture("Model/wall.jpg");

	m_noiseTex = m_renderer->CreateTexture(256, 256, Render::EPF_RGBA8_UNORM);

	m_noise.SetSize(256, 256);

	m_colorData = new Color4[256 * 256];

	float* noiseData = m_noise.GenerateNoiseData();

	for (uint32 i = 0; i < 256 * 256; i++)
	{
		float gray = noiseData[i];
		uint32 t = (uint32)((gray + 1.0f) / 2.0f * 255.0f);

		m_colorData[i] = 0xFF000000 | t << 16 | t << 8 | t;
	}

	m_noiseTex->CopyFromData(m_colorData, 256 * 256 * sizeof(Color4));
	
	m_dissTech = sRenderMgr->GenerateRenderTechnique("Dissolve");
	m_dissShader = m_dissTech->GetPass(0)->GetShaderProgram();

 	m_dissColor = m_dissTech->GetVariable("g_dissColor");
 	m_dissColor->SetValue(Vector4Df(1.0f, 0, 0, 1.0f));
 
	m_addColor = m_dissTech->GetVariable("g_addColor");
 	m_addColor->SetValue(Vector4Df(1.0f, 1.0f, 0, 1.0f));
 
 	m_dissRange = m_dissTech->GetVariable("g_dissRange");
 	m_dissRange->SetValue(Vector4Df(0.1f, 0.1f, 0.1f, 0.1f));

	m_blendVar = m_dissTech->GetVariable("g_dissAmount");
	m_blendVar->SetValue(Vector4Df(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_colorData);

	SAFE_DELETE(m_mainTex);

	SAFE_DELETE(m_noiseTex);

	SAFE_DELETE(m_dissTech);
}

void MyApp::onWindowChangeSize(int32 width, int32 height)
{

}

bool MyApp::onMouseEvent(const MouseEvent& event)
{
	return false;
}

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	if (event.state != STATE_UP) return false;

	switch (event.key)
	{
		case KEY_UP:
		{
			float per = m_noise.GetPersistence();
			m_noise.SetPersistence(per + 0.1f);
		}
		break;
		case KEY_DOWN:
		{
			float per = m_noise.GetPersistence();
			m_noise.SetPersistence(per - 0.1f);
		}
		break;
		case KEY_LEFT:
		{
			int oct = m_noise.GetOctaves();
			m_noise.SetOctaves(oct + 1);
		}
		break;
		case KEY_RIGHT:
		{
			int oct = m_noise.GetOctaves();
			m_noise.SetOctaves(oct - 1);
		}
		break;
		case KEY_RETURN:
		{
			m_pause = !m_pause;
			return true;
		}
		break;
	}

	float* noiseData = m_noise.GenerateNoiseData();

	for (uint32 i = 0; i < 256 * 256; i++)
	{
		float gray = noiseData[i];
		uint32 t = (uint32)((gray + 1.0f) / 2.0f * 255.0f);

		m_colorData[i] = 0xFF000000 | t << 16 | t << 8 | t;
	}

	m_noiseTex->CopyFromData(m_colorData, 256 * 256 * sizeof(Color4));
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
	static int count = 0;

	float32 a = count / 200 % 255 / 255.0f;
	
	m_blendVar->SetValue(Vector4Df(a, a, a, a));

	if(!m_pause) count++;

	m_renderer->BindTexture(1, m_noiseTex);

	m_dissTech->ApplyToDevice();

	m_renderer->BindProgram(m_dissShader);

	sRenderMgr->DrawFullScreenQuad(m_mainTex);
}