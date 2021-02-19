#include "PreCompile.h"
#include "Pharos.h"

OctreeScene::OctreeScene(void)
{
	m_sceneHeight = 0;

	m_pTreeRoot = nullptr;

	m_camera = nullptr;

	m_gridNode = nullptr;
	m_showGrid = false;
}

OctreeScene::~OctreeScene(void)
{
	SAFE_DELETE(m_pTreeRoot);

	SAFE_DELETE(m_camera);

	SAFE_DELETE(m_gridNode);
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
//	const Vector3Df& cameraPos = m_camera->GetPosition();
	const Matrix4& viewMat = m_camera->GetViewMatrix();
	const Matrix4& projMat = m_camera->GetProjMatrix();

	//sRenderMgr->SetGlobalRenderEyePostion(cameraPos);
	//sRenderMgr->SetGlobalRenderViewMatrix(viewMat);
	//sRenderMgr->SetGlobalRenderProjMatrix(projMat);

	//更新场景中所有材质,应该放到材质管理器里更新...
	for (int i = 0; i < sMaterialMgr->GetMaterialNum(); i++)
	{
		Material* material = sMaterialMgr->GetMaterial(i);

		material->SetViewParamValue(viewMat);
		material->SetProjParamValue(projMat);

		Vector3Df lightDir = Vector3Df(1.0f, -1.0f, 1.0f);
		lightDir.Normalize();
		material->SetLightDirectionParamValue(lightDir);
		material->SetEnvironmentColorParamValue(0xFF4F4F4F);
		material->SetLightColorParamValue(0xFFFFFFFF);		

		material->UpdateParamValue();
	}

	const Frustum& frustum = m_camera->GetViewFrustum();
	m_pTreeRoot->FrustumCulling(frustum);

	m_pTreeRoot->Update(fElapsed);

	for (auto node : m_nodeList)
	{
		if (node != nullptr)
		{
			node->Update(fElapsed);
		}
	}

	if (m_showGrid)
	{
		m_gridNode->Update(fElapsed);
	}

	//Sprite3D::Inst()->DrawGridPlane(m_mapSize.width, m_mapSize.height, 0xFF00FFFF);
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

SceneNode* OctreeScene::GetSceneNode(uint32 index)
{
    return index < (uint32)m_nodeList.size() ? m_nodeList[index] : nullptr;
}

SceneNode* OctreeScene::GetSceneNode(const char8* name)
{
    for (vector<SceneNode*>::iterator iter = m_nodeList.begin(); iter != m_nodeList.end(); iter++)
    {
        SceneNode* childNode = *iter;
        if (strcmp(childNode->GetNodeName(), name) == 0)
        {
            return childNode;
        }
    }
    
    return nullptr;
}

