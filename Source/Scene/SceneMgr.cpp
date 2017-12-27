#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

SceneMgr::SceneMgr()
{
	m_currScene = nullptr;
}

SceneMgr::~SceneMgr()
{
}

bool SceneMgr::Init()
{
	return true;
}

void SceneMgr::Destroy()
{
	for (auto iter : m_sceneNodeList)
	{
		SceneNode* sceneNode = iter.second;
		SAFE_DELETE(sceneNode);
	}
}

OctreeScene* SceneMgr::CreateScene()
{
	OctreeScene* scene = new OctreeScene();
	scene->Init();
	return scene;
}

SceneImporter* SceneMgr::CreateSceneImporter(const char8* sceneFile)
{
	Utils::Path path(sceneFile);
	const char8* ext = path.GetExtension();
	ext = strlwr((char8*)ext);

	SceneImporter* importer = nullptr;
	if (strcmp(ext, ".sceneml") == 0)
	{
		importer = new XmlSceneImporter();
		if (importer->LoadSceneFile(sceneFile)) return importer;
	}
	//else if (strcmp(ext, ".gpz") == 0)
	//{
		//importer = new BundleSceneImporter();
		//if (importer->LoadSceneFile(sceneFile)) return importer;
	//}

	SAFE_DELETE(importer);
	return nullptr;
}


void SceneMgr::SetCurrScene(OctreeScene* scene)
{
	m_currScene = scene;
}

SceneNode* SceneMgr::CreateSceneNode(const char8* name, SceneNode* parent)
{
	if (m_sceneNodeList.find(name) != m_sceneNodeList.end()) return nullptr;

	SceneNode* sceneNode = new SceneNode();
	sceneNode->InitNode(name, parent);
	m_sceneNodeList[name] = sceneNode;
	return sceneNode;	
}

SceneNode* SceneMgr::GetSceneNode(const char8* name)
{
	auto iter = m_sceneNodeList.find(name);
	return (iter != m_sceneNodeList.end()) ? iter->second : nullptr;
}

SceneCamera* SceneMgr::CreateCamera()
{
	return new SceneCamera();
}

void SceneMgr::Update(float32 fElapsed)
{
	//ResMgr::Inst()->Process();

	if(m_currScene != nullptr)
	{
		m_currScene->Update(fElapsed);
	}

  	//Sprite3D::Inst()->DrawLine(Vector3Df(0, 0, 0), Vector3Df(10.0f, 0, 0), 0xFFFF0000);
  	//Sprite3D::Inst()->DrawLine(Vector3Df(0, 0, 0), Vector3Df(0, 10.0f, 0), 0xFF00FF00);
  	//Sprite3D::Inst()->DrawLine(Vector3Df(0, 0, 0), Vector3Df(0, 0, 10.0f), 0xFF0000FF);
}

void SceneMgr::Render(float32 fElapsed)
{
// 	for (auto& renderHelper : m_renderHelperList)
// 	{
// 		renderHelper->DoRender();
// 	}
// 
// 	m_renderHelperList.clear();

	if (m_currScene != nullptr)
	{
		m_currScene->Render(fElapsed);
	}
}