#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

OctreeScene::OctreeScene(void)
{
	m_sceneHeight = 0;

	m_pTreeRoot = nullptr;

	m_camera = nullptr;

	m_gridNode = nullptr;
}

OctreeScene::~OctreeScene(void)
{
	SAFE_DELETE(m_pTreeRoot);
}

void OctreeScene::Init()
{
	m_mapSize.width = 100;
	m_mapSize.height = 100;
	m_sceneHeight = 100;

	Vector3Df center = Vector3Df(m_mapSize.width / 2.0f, m_sceneHeight / 2.0f, m_mapSize.height / 2.0f);

	const Size2Di& wndSize = sKernel->GetWindowSize();
	m_camera = new SceneCamera();
	m_camera->BuildViewMatrix(Vector3Df(0, (float32)m_sceneHeight, 0), center);
	m_camera->BuildProjMatrix((float32)PI / 4.0f, wndSize.width, wndSize.height, 1.0f, 1000.0f);

	m_pTreeRoot = new TreeNode();
	m_pTreeRoot->Init(this, nullptr, -1);
	m_pTreeRoot->SetCenter(center);
	m_pTreeRoot->SetRadius(sqrt(center.x * center.x + center.y * center.y + center.z * center.z));
	m_pTreeRoot->ComputeBasePos();

	if (m_gridNode == nullptr)
	{
		m_gridNode = new GridNode();
		if (!m_gridNode->BuildGridData())
		{
			m_gridNode = nullptr;
		}
	}
}

void OctreeScene::SetSceneSize(const Size2Di& mapSize, uint32 sceneHeight)
{
	m_mapSize = mapSize;
	m_sceneHeight = sceneHeight;

	Vector3Df center = Vector3Df(m_mapSize.width / 2.0f, m_sceneHeight / 2.0f, m_mapSize.height / 2.0f);

	m_pTreeRoot->SetCenter(center);
	m_pTreeRoot->SetRadius(sqrt(center.x * center.x + center.y * center.y + center.z * center.z));
	m_pTreeRoot->ComputeBasePos();
}

//void OctreeScene::onLoadSuccess()
//{
// 	SceneInfo* pSceneInfo = (SceneInfo*)data;
// 
// 	Vector3Df center;
// 	center.x = pSceneInfo->GetMapWidth() / 2.0f;
// 	center.y = pSceneInfo->GetMapLength() / 2.0f;
// 	center.z = pSceneInfo->GetSceneHeight() / 2.0f;
// 
// 	m_pTreeRoot->SetCenter(center);
// 	m_pTreeRoot->SetRadius(sqrt(center.x * center.x + center.y * center.y + center.z * center.z));
// 	m_pTreeRoot->ComputeBasePos();
// 
// 	//执行视锥检测
// 	//////////////////////////////////////////////////////////////////////////
// 	//...
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	for(int32 i = 0; i < pSceneInfo->GetChildNodeNum(); i++)
// 	{
// 		NodeInfo* pNodeInfo = pSceneInfo->GetChildNode(i);
// 		SceneNode* pNode = DEBUG_NEW SceneNode();
// 		pNode->Init(pNodeInfo);
// 		this->AddSceneNode(pNode);
// 	}
//}

void OctreeScene::Update(float32 fElapsed)
{
	const Vector3Df& cameraPos = m_camera->GetPosition();
	Vector4Df cameraPosValue = Vector4Df(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	const Matrix4& viewMat = m_camera->GetViewMatrix();
	const Matrix4& projMat = m_camera->GetProjMatrix();

	sRenderMgr->SetGlobalRenderValue("CAMERA_WORLD_POSITION", cameraPosValue);
	//sRenderMgr->SetGlobalRenderValue("WORLD_MATRIX", );
	sRenderMgr->SetGlobalRenderValue("VIEW_MATRIX", viewMat);
	sRenderMgr->SetGlobalRenderValue("PROJ_MATRIX", projMat);

	const Frustum& frustum = m_camera->GetViewFrustum();
	m_pTreeRoot->FrustumCulling(frustum);

	m_pTreeRoot->Update(fElapsed);

	for (auto node : m_nodeList)
	{
		if (node != nullptr)
			node->Update(fElapsed);
	}

	m_gridNode->Update(fElapsed);

	//Sprite3D::Inst()->DrawGridPlane(m_mapSize.width, m_mapSize.height, 0xFF00FFFF);
}

void OctreeScene::Render(float32 fElapsed)
{

}

void OctreeScene::AddSceneNode(SceneNode* pNode)
{
	if (pNode == nullptr) return;

	m_nodeList.push_back(pNode);
	m_pTreeRoot->InsertItem(pNode);
}

void OctreeScene::RemoveSceneNode(SceneNode* pNode)
{
	if (pNode == nullptr) return;

	for (vector<SceneNode*>::iterator iter = m_nodeList.begin(); iter != m_nodeList.end(); iter++)
	{
		SceneNode* childNode = *iter;
		if (childNode == pNode)
		{
			m_nodeList.erase(iter);
			break;
		}
	}

	TreeNode* pOwner = pNode->GetTreeOwner();
	pOwner->RemoveOwnItem(pNode);
}