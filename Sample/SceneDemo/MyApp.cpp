#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"
#include "DesktopGlobal.h"
#include "Global.h"

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

	//Scene
	//////////////////////////////////////////////////////////////////////////
	m_scene = sSceneMgr->CreateScene();
	sSceneMgr->SetCurrScene(m_scene);

	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter("Model/Ifrit/Ifrit.sceneml");
	sceneImporter->ImportScene(m_scene);
	SAFE_DELETE(sceneImporter);

	SceneNode* sceneNode = m_scene->GetSceneNode(0);
	m_model = sceneNode->GetModel(0);

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(5.0f, 5.0f, -5.0f), Vector3Df(0, 0, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);
	//////////////////////////////////////////////////////////////////////////

	//UI
	//////////////////////////////////////////////////////////////////////////
	sDesktopMgr->SetDesktopSize(wndSize.width, wndSize.height);
	sDesktopMgr->RegisterControlViewer(this);
	sDesktopMgr->LoadUILayoutFile("Interface/Console/Console.xml");

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

bool MyApp::onKeyboardEvent(const KeyEvent& evnet)
{
	return true;
}

void MyApp::onControlCreate(const char8* name, int32 v1, float32 v2)
{

}

void MyApp::onControlValueChange(const char8* name, int32 v1, float32 v2)
{
	static const char8* animNameList[] =
	{
		"atk1", "atk2", "atk3", "atk4", "bak1", "bak2", "bak3", "bko1", "blk",
		"coma", "dead", "duck", "ent", "fko1", "jmp1", "jmp2", "jmp3", "lei1",
		"lei2", "out", "rdy", "rsh", "rsha", "run", "runa", "sf01", "sf02",
		"sk01", "sk02", "sk03", "sk04", "std", "stop", "stpa", "wlk", "wlka",
	};

	static int32 animIndex = 0;

	static bool loop = false;

	if (strcmp(name, "NextAnim") == 0)
	{
		animIndex++;
		animIndex = animIndex % (sizeof(animNameList) / sizeof(const char8*));

		m_model->SetCurrentAnimation(animNameList[animIndex]);
	}
	else if (strcmp(name, "PrevAnim") == 0)
	{
		animIndex--;
		if (animIndex < 0) animIndex = (sizeof(animNameList) / sizeof(const char8*)) - 1;
		animIndex = animIndex % (sizeof(animNameList) / sizeof(const char8*));

		m_model->SetCurrentAnimation(animNameList[animIndex]);
	}
	else if (strcmp(name, "PlayAnim") == 0)
	{
		m_model->PlayAnimation(loop);
	}
	else if (strcmp(name, "PauseAnim") == 0)
	{
		m_model->PauseAnimation();
	}
	else if (strcmp(name, "StopAnim") == 0)
	{
		m_model->StopAnimation();
	}
	else if (strcmp(name, "LoopPlay") == 0)
	{
		loop = (bool)v1;
	}
	else if (strcmp(name, "AnimSpeed") == 0)
	{
		m_model->SetPlayAnimationSpeed(v2);
	}
}

void MyApp::Update(float32 fElapsed)
{

}

void MyApp::Render(float32 fElapsed)
{

}

void MyApp::onRender(float32 elapsed)
{

}