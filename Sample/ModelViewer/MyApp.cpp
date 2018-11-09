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
	param.hdrEnabled = false;
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
	m_camera->BuildViewMatrix(Vector3Df(0, 2.0f, -5.0f), Vector3Df(0, 1.0f, 0));
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

bool MyApp::onKeyboardEvent(const KeyEvent& event)
{
	switch (event.key)
	{
		case KEY_A:
		{
			m_camera->Move(Vector3Df(-1.0f * m_elapsed * MOVE_SPEED, 0, 0));
		}
		break;
		case KEY_D:
		{
			m_camera->Move(Vector3Df(1.0f * m_elapsed * MOVE_SPEED, 0, 0));
		}
		break;
		case KEY_W:
		{
			m_camera->Move(Vector3Df(0, 0, 1.0f * m_elapsed * MOVE_SPEED));
		}
		break;
		case KEY_S:
		{
			m_camera->Move(Vector3Df(0, 0, -1.0f * m_elapsed * MOVE_SPEED));
		}
		break;
		case KEY_Q:
		{
			m_camera->Move(Vector3Df(0, 1.0f * m_elapsed * MOVE_SPEED, 0));
		}
		break;
		case KEY_E:
		{
			m_camera->Move(Vector3Df(0, -1.0f * m_elapsed * MOVE_SPEED, 0));
		}
		break;
	}

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
		loop = (v1 == 0 ? false : true);
	}
	else if (strcmp(name, "AnimSpeed") == 0)
	{
		m_model->SetPlayAnimationSpeed(v2);
	}
}

void MyApp::Update(float32 fElapsed)
{
	m_elapsed = fElapsed;
}

void MyApp::onRender(float32 elapsed)
{

}