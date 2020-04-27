#include "PreCompile.h"
#include "Global.h"

MyApp::MyApp()
{
	m_renderer = nullptr;
}

MyApp::~MyApp()
{
}

IMPL_CREATE_APPLICATION(MyApp)

bool MyApp::Init()
{
	Size2Di wndSize = sKernel->GetWindowSize();

	m_renderer = MakeRenderer();

	DeviceConfig cfg;
	m_renderer->Create(cfg);

	//layout
	//////////////////////////////////////////////////////////////////
	Vertex vertData[] =
	{
		{ Vector3Df(-0.5f,  0.5f, 0), 0xFFFF0000},// Vector2Df(0, 0) },
		{ Vector3Df(0.5f, 0.5f, 0), 0xFF00FF00},//, Vector2Df(1.0f, 0) },
		{ Vector3Df(0.5f, -0.5f, 0), 0xFF00FFFF},//, Vector2Df(1.0f, 1.0f) },

		{ Vector3Df(0.5f, -0.5f, 0), 0xFF00FFFF},//, 0xFFFF0000},//, Vector2Df(1.0f, 1.0f) },
		{ Vector3Df(-0.5f, -0.5f, 0), 0xFF0000FF},//, Vector2Df(0, 1.0f) },
		{ Vector3Df(-0.5f, 0.5f, 0), 0xFFFF0000},//, Vector2Df(0, 0) },
	};

	MemoryBuffer vertDataBuf;
	vertDataBuf.CopyFrom(vertData, sizeof(vertData));

	m_vertBuf = m_renderer->GenerateRenderBuffer(sizeof(vertData), &vertDataBuf);
	//////////////////////////////////////////////////////////////////

	//render pipeline
	//////////////////////////////////////////////////////////////////
	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, VAL_POSITION, 0, 0 },
		{ VET_UNORM8, 4, VAL_COLOR, 12, 0 },
	};

	m_pipeline = m_renderer->GenerateRenderPipeline();
	m_pipeline->SetInputLayoutDesc(desc, 2);
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	//texture
	m_texture = m_renderer->LoadTexture("drang.jpeg");
	//m_texture = m_renderer->LoadTexture("Prev.png");
	//////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////
	//uniform buffer
	//m_renderer->BindLayout(m_layout);
	//m_renderer->Bind
	//////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////
	//render command
	m_renderCommand = m_renderer->GenerateRenderCommand();
	//////////////////////////////////////////////////////////////////   

	//Scene
	//////////////////////////////////////////////////////////////////////////
	m_scene = sSceneMgr->CreateScene();
	sSceneMgr->SetCurrScene(m_scene);
	m_scene->SetSceneGridShow(true);

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(0, 0, -5.0f), Vector3Df(0, 0, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);
	//////////////////////////////////////////////////////////////////////////

	m_uniformBuf = m_renderer->GenerateRenderBuffer(sizeof(SceneMatrix));

	return true;
}

void MyApp::Destroy()
{

}

void MyApp::onWindowChangeSize(int32 width, int32 height)
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
	m_sceneMat.proj = m_camera->GetProjMatrix();
	m_sceneMat.view = m_camera->GetViewMatrix();

	m_uniformBuf->CopyData(&m_sceneMat, sizeof(SceneMatrix));

	m_renderCommand->SetVertexBuffer(2, m_uniformBuf);

	m_renderCommand->SetPipeline(m_pipeline);

	m_renderCommand->SetVertexBuffer(0, m_vertBuf);

	m_renderCommand->SetFragmentTexture(0, m_texture);

	m_renderCommand->DrawImmediate(EDT_TRIANGLELIST, 0, 6);
	m_renderCommand->Present();
}

void MyApp::onRender(float32 elapsed)
{

}
