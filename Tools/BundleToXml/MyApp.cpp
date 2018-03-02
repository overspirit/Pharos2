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

	sRenderMgr->LoadEffectFile("D:/HiSceneV5/HiSceneShaderColored.fxml");
	sRenderMgr->LoadEffectFile("D:/HiSceneV5/HiSceneShaderInterior.fxml");
	sRenderMgr->LoadEffectFile("D:/HiSceneV5/HiSceneShaderStar.fxml");
	sRenderMgr->LoadEffectFile("D:/HiSceneV5/HiSceneShaderReflective.fxml");
	sRenderMgr->LoadEffectFile("D:/HiSceneV5/HiSceneShaderTextured.fxml");

	m_scene = sSceneMgr->CreateScene();
	//scene->SetSceneSize(Size2Di(128, 128), 128);
	sSceneMgr->SetCurrScene(m_scene);

	//BundleSceneImporter* sceneImporter = new BundleSceneImporter();
	//sceneImporter->LoadSceneFile("D:/HiSceneV5/HiScene-v5-4094-GS8/res/common/gs8.gpz");
	//sceneImporter->ImportScene(m_scene);
	//sceneImporter->SaveTo("D:/gs8.sceneml");

	SceneImporter* sceneImporter = sSceneMgr->CreateSceneImporter("D:/gs8.sceneml");
	sceneImporter->ImportScene(m_scene);

	SAFE_DELETE(sceneImporter);

	m_carNode = sSceneMgr->GetSceneNode("GS8");

	m_camera = m_scene->GetSceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(75.0f, 75.0f, -75.0f), Vector3Df(0, 0, 0));
	m_camera->BuildProjMatrix((float32)PI / 4, wndSize.width, wndSize.height, 1.0f, 1000.0f);

	vector<string> hideList =
	{
		"Car_CHUANGQIGS8_360Scene", "Car_Surrounding", "TrunkSpace", "Car_Features", "koubei",
		"xinxi", "Static_Obj", "Dynamic_Obj", "Configuration_Points", "Space_Configuration", "Passengers"
	};

	for (string hideName : hideList)
	{
		SceneNode* sceneNode = sSceneMgr->GetSceneNode(hideName.c_str());
		if (sceneNode != nullptr) sceneNode->SetHidden(true);
	}

// 	m_renderer = sRenderMgr->GetCurrentRenderer();
// 
// 	Vertex vertData[] =
// 	{
// 		{ Vector3Df(-0.5f,  0.5f, 0), Vector2Df(0, 0) },
// 		{ Vector3Df(0.5f, 0.5f, 0), Vector2Df(1.0f, 0) },
// 		{ Vector3Df(0.5f, -0.5f, 0), Vector2Df(1.0f, 1.0f) },
// 
// 		{ Vector3Df(0.5f, -0.5f, 0), Vector2Df(1.0f, 1.0f) },
// 		{ Vector3Df(-0.5f, -0.5f, 0), Vector2Df(0, 1.0f) },
// 		{ Vector3Df(-0.5f, 0.5f, 0), Vector2Df(0, 0) },
// 	};
// 
// 	MemoryBuffer vertDataBuf;
// 	vertDataBuf.CopyFrom(vertData, sizeof(vertData));
// 	m_copyLayout = m_renderer->GenerateRenderLayout(sizeof(vertData), &vertDataBuf);
// 
// 	VertLayoutDesc desc[] =
// 	{
// 		{ VET_FLOAT32, 3, "POSITION", 0, 0 },
// 		{ VET_FLOAT32, 2, "TEXCOORD", 0, 12 },
// 	};
// 	m_copyLayout->SetInputLayoutDesc(desc, 2);
// 
// 	m_copyTech = sRenderMgr->GenerateRenderTechnique("Copy");
// 	RenderPass* copyPass = m_copyTech->GetPass(0);
// 	m_copyShader = copyPass->GetShaderProgram();

	return true;
}

void MyApp::Destroy()
{
	SAFE_DELETE(m_scene);

	//SAFE_DELETE(m_renderer);
	//SAFE_DELETE(m_copyLayout);
	//SAFE_DELETE(m_copyTech);
}

Vector3Df MyApp::GetScreenVector(const Vector2Df& vtMove)
{
	const Size2Di& wndSize = sKernel->GetWindowSize();
	int32 nHalfWidth = (int32)(wndSize.width / 2);
	int32 nHalfHeight = (int32)(wndSize.height / 2);

	//��ӳ������,���Ƚ���Ļ�ϵĵ�ӳ��Ϊ0-1֮�����ֵ��Ȼ�󹴹ɶ���������һ����	
	Vector3Df vTo = { 0, 0, 1.0f };
	float32 x = vtMove.x / nHalfWidth;
	float32 y = -vtMove.y / nHalfHeight;
	float32 x2 = x * x;
	float32 y2 = y * y;
	if (x2 + y2 <= 1.0f) //��Ҫ����Ƿ��ܿ����������������ݿ��ܴ��ڴ��ڴ�С
	{
		vTo.x = x;
		vTo.y = y;
		vTo.z = sqrtf(1.0f - x2 - y2);
		vTo.Normalize();
	}
	return vTo;
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

		m_mousePoint = Vector2Df(event.x, event.y);
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
		Vector2Df mousePos;
		mousePos.Set(event.x, event.y);

		Vector2Df mouseMove = mousePos - m_mousePoint;
		Vector3Df screenVt = GetScreenVector(mouseMove);

		screenVt.y = 0;
		screenVt.x = -screenVt.x;
		screenVt.Normalize();

		Matrix4 rota;
		rota.BuildRotateFromTo(Vector3Df(0, 0, 1.0f), screenVt);

		m_nodeRota *= rota;
		
		m_carNode->SetLocalTransform(m_nodeRota);

		m_mousePoint = mousePos;

		//m_camera->Slither(Vector2Df((float32)event.ox, (float32)event.oy));
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