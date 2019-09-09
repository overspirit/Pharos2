#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"
#include "DesktopGlobal.h"
#include "Global.h"

#define MOVE_SPEED 10.0f

MyApp::MyApp()
{
	m_bLeftDown = false;
	m_bRightDown = false;

	m_elapsed = 0;
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
	
	//Scene
	//////////////////////////////////////////////////////////////////////////
	m_scene = sSceneMgr->CreateScene();
	sSceneMgr->SetCurrScene(m_scene);

	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter("Model/Earth/earth.sceneml");
	sceneImporter->ImportScene(m_scene);
	SAFE_DELETE(sceneImporter);

	SceneNode* atmosphereNode = sSceneMgr->GetSceneNode("Atmosphere");
	Mesh* atmosphereMesh = atmosphereNode->GetModel(0)->GetMesh(0);
	atmosphereMesh->SetAttachMaterial(nullptr);

	SceneNode* planetNode = sSceneMgr->GetSceneNode("Planet");
	Mesh* plantMesh = planetNode->GetModel(0)->GetMesh(0);
	plantMesh->SetAttachMaterial(nullptr);

	//m_scene->GetSceneNodeNum()
	//SceneNode* sceneNode = m_scene->GetSceneNode(0);
	//m_model = sceneNode->GetModel(0);

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(0, 0, -3.5f), Vector3Df(0, 0, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);
	//////////////////////////////////////////////////////////////////////////

	//UI
	//////////////////////////////////////////////////////////////////////////
	sDesktopMgr->SetDesktopSize(wndSize.width, wndSize.height);
	//sDesktopMgr->RegisterControlViewer(this);
	sDesktopMgr->LoadUILayoutFile("Interface/AtmosphericScattering/AtmosphericScattering.xml");

	//////////////////////////////////////////////////////////////////////////
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

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	return true;
}

void MyApp::Update(float32 fElapsed)
{
	m_elapsed = fElapsed;
}

void MyApp::Render(float32 fElapsed)
{

}

void MyApp::onRender(float32 elapsed)
{

}