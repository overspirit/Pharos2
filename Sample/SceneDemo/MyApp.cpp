#include "PreCompile.h"
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
	Size2Di wndSize = sKernel->GetWindowSize();
	
	RenderParam param;
	param.width = wndSize.width;
	param.height = wndSize.height;
	param.backColor = 0xFFFFFFFF;
	param.sampleType = EMT_None;
	param.sync = false;
	param.fullScreen = false;
	param.hdrEnabled = true;
	param.gammaEnabled = true;
	sRenderMgr->StartUp(param);
	
	//Scene
	//////////////////////////////////////////////////////////////////////////
	m_scene = sSceneMgr->CreateScene();
	sSceneMgr->SetCurrScene(m_scene);
	m_scene->SetSceneGridShow(true);
	
	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(0, 20.0f, -50.0f), Vector3Df(0, 1.0f, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);
	//////////////////////////////////////////////////////////////////////////
	
	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter("Model/Car/car.sceneml");
	sceneImporter->ImportScene(m_scene);
	SAFE_DELETE(sceneImporter);
	
	SceneNode* sceneNode = m_scene->GetSceneNode(0);
	m_model = sceneNode->GetModel(0);
	
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
		if (event.state != STATE_KEEP)
		{
			m_bLeftDown = (event.state == STATE_DOWN) ? true : false;
		}
	}
	else if (event.button == MOUSE_RIGHT)
	{
		if (event.state != STATE_KEEP)
		{
			m_bRightDown = (event.state == STATE_DOWN) ? true : false;
		}
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
	// m_sceneMat.view = m_camera->GetViewMatrix();
	// m_sceneMat.proj = m_camera->GetProjMatrix();
	// m_sceneMat.color = Color4f(0.7f, 0.7, 0.3f, 1.0f);

	//m_model->SetUniformBuffer(&m_sceneMat, sizeof(m_sceneMat));
}

void MyApp::onRender(float32 elapsed)
{

}
