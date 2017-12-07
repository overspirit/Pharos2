#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

TreeNode::TreeNode(void)
{
	m_pScene = nullptr;

	m_pParent = nullptr;
	m_nIndex = -1;
	ZeroMemory(m_children, sizeof(m_children));
	m_dwComputeFlag = 0;

	m_fRadius = 0;

	m_nDepth = 0;

	m_bCulling = false;
}

TreeNode::~TreeNode(void)
{
	for(int32 i = 0; i < 8; i++)
	{
		SAFE_DELETE(m_children[i]);
	}
}

void TreeNode::Init(OctreeScene* pScene, TreeNode* pParent, int32 nIndex)
{
	m_pScene = pScene;

	m_pParent = pParent;
	m_nIndex = nIndex;

	if(pParent != nullptr)
	{
		m_nDepth = pParent->GetDepth() + 1;
	}
}

void TreeNode::FrustumCulling(const Frustum& frustum)
{
	//本节点被剪裁掉就不用更新子节点了
	//这个还有待验证，是否在视锥体外就不更新了
	//如果本节点不在视锥体内，则本节点被剪裁掉。
	m_bCulling = !frustum.SphereInFrustum(m_vtCenter, m_fRadius);

	//遍历更新子节点
	for(int32 i = 0; i < 8; i++)
	{
		if(m_children[i]) m_children[i]->FrustumCulling(frustum);
	}
}

void TreeNode::Update(float32 fElapsed)
{
	for(auto child : m_children)
	{
		if (child != nullptr)
			child->Update(fElapsed);
	}

	////执行其他剪裁方式等
	//if (true)
	//{
	//	for (auto node : m_nodeList)
	//	{
	//		if (node != nullptr)
	//			node->DoUpdate(fElapsed);
	//	}
	//}

	////视椎剪裁
	//if (!m_bCulling)
	//{
	//	for (auto node : m_nodeList)
	//	{
	//		if (node != nullptr)
	//			node->DoRender(fElapsed);
	//	}
	//}

	//Sprite3D::Inst()->DrawBoundingSphere(m_vtCenter, m_fRadius, 0xFFFF0000);
}

//计算给定的球是否在这个TreeNode中
bool TreeNode::IsInSphere(const Vector3Df& vtCenter, float32 fRadius)
{
	float32 x = m_vtCenter.x - vtCenter.x;
	float32 y = m_vtCenter.y - vtCenter.y;
	float32 z = m_vtCenter.z - vtCenter.z;
	float32 len2 = x * x + y * y + z * z;
	float32 r = m_fRadius - fRadius;
	if(r > 0 && len2 < r * r) return true;
	return false;
}

//将一个SceneNode插入到这个TreeNode中,TreeNode会首先检测本身是否合适
//管理这个SceneNode,不合适就寻找是否有合适的子节点来管理这个SceneNode,
//没有的话会建立一个合适的TreeNode
void TreeNode::InsertItem(SceneNode* pNode)
{
	const Vector3Df& vtItemCenter = pNode->GetGlobalPosition();
	float32 fItemRadius = pNode->GetBoundingRadius();

	//物体半径大于三分之一节点半径就直接这个节点放到树节点中
	if(fItemRadius > m_fRadius / 3.0f || m_nDepth >= 16) 
	{
		m_nodeList.push_back(pNode);
		pNode->SetTreeOwner(this);
		return;
	}

	//遍历，寻找距离给出的物体最近的子节点索引
	int32 nMinIndex = -1;
	float32 fMinLength = FLT_MAX;
	for(int32 i = 0; i < 8; i++)
	{
		//计算该点到计算点的距离，
		float32 len = 0;

		//获得计算点，如果有子节点就使用子节点的中心点
		//如果没有子节点就使用基本点
		if(m_children[i] != nullptr)
		{
			const Vector3Df& vtCenter = m_children[i]->GetCenter();
			len = (vtItemCenter - vtCenter).GetLengthSQ();
		}
		else
		{
			len = (vtItemCenter - m_basePos[i]).GetLengthSQ();
		}

		if(len < fMinLength) 
		{
			//如果小于存储的最小值就存储索引
			fMinLength = len;
			nMinIndex = i;
		}
	}

	//如果没有最近的子节点就要建立
	if(!m_children[nMinIndex])
	{
		m_children[nMinIndex] = new TreeNode();
		m_children[nMinIndex]->Init(m_pScene, this, nMinIndex);
		m_children[nMinIndex]->SetCenter(m_basePos[nMinIndex]);	//该子节点的中心点就是基本点
		m_dwComputeFlag |= 1 << nMinIndex;	//为优化计算置标志位
	}	

	//重新计算这个子节点的大小，并将这个SceneNode插入到这个子节点中
	m_children[nMinIndex]->Recompute(vtItemCenter, fItemRadius);
	m_children[nMinIndex]->InsertItem(pNode);
}

//重新计算
void TreeNode::Recompute(const Vector3Df& vtCenter, float32 fRadius)
{
	Vector3Df dis = m_vtCenter - vtCenter;
	float32 len = dis.GetLength() + fRadius;
	if(len > m_fRadius)
	{
		m_fRadius = len;

		//如果已经有8个子节点，就不必计算基本点了
		if(m_dwComputeFlag == 0xFF) return;

		this->ComputeBasePos();
	}
}

//计算基本点
void TreeNode::ComputeBasePos()
{
	float32 r = m_fRadius/2.0f;
	float32 offset = Math::g_sin[45] * (m_fRadius / 2.0f);

	for(int32 i = 0; i < 8; i++)
	{		
		if(m_children[i] != nullptr) continue;

		Vector3Df& vt = m_basePos[i];
		vt.x = m_vtCenter.x + ((i & 1) * 2 - 1) * offset;
		vt.y = m_vtCenter.y + (((i & 2) >> 1) * 2 - 1) * offset;
		vt.z = m_vtCenter.z + (((i & 4) >> 2) * 2 - 1) * offset;
	}
}

//刷新管理的SceneNode，如果SceneNode的位置或大小发生变化，就会调用这个函数
void TreeNode::RefreshOwnItem(SceneNode* pNode)
{
	if(pNode->GetTreeOwner() != this) return;

	const Vector3Df& vtItemCenter = pNode->GetGlobalPosition();
	float32 fItemRadius = pNode->GetBoundingRadius();	

	//如果变化之后仍在本节点之内，就直接返回
	if(this->IsInSphere(vtItemCenter, fItemRadius)) return;

	//将该节点从本节点中移除
	m_nodeList.remove(pNode);

	//加入到根节点中
	TreeNode* pTreeRoot = m_pScene->GetTreeRootNode();
	pTreeRoot->InsertItem(pNode);

	//获得该节点新的从属节点
	TreeNode* pNewOwner = pNode->GetTreeOwner();

	//从本节点向上遍历到根节点或新插入的节点，如果本节点已经无效，则删除本节点
	TreeNode* pTreeNode = this;
	while(true)
	{
		TreeNode* pParent = pTreeNode->m_pParent;
		if(pParent == nullptr || pTreeNode == pNewOwner) break;

		if(pTreeNode->IsVaild()) break;
	
		int32 childIndex = pTreeNode->m_nIndex;
		pParent->RemoveChild(childIndex);

		pTreeNode = pParent;
	}
}

void TreeNode::RemoveOwnItem(SceneNode* pNode)
{	
	if(pNode->GetTreeOwner() != this) return;

	m_nodeList.remove(pNode);

	//从该节点的从属节点向上遍历，如果这个节点已经无效，则删除该节点
	TreeNode* pTreeNode = this;
	while(true)
	{
		TreeNode* pParent = pTreeNode->m_pParent;
		if(pParent == nullptr) break;

		if(pTreeNode->IsVaild()) break;

		int32 childIndex = pTreeNode->m_nIndex;
		pParent->RemoveChild(childIndex);

		pTreeNode = pParent;
	}
}

bool TreeNode::IsVaild()
{
	//m_pParent无效说明是根节点，根节点一直有效
	if(m_pParent == nullptr) return true;

	//没有子节点且没有管理的场景节点，就无效
	if(m_nodeList.size() <= 0 && m_dwComputeFlag == 0)
	{
		return false;		
	}

	return true;
}

//删除子节点，
void TreeNode::RemoveChild(int32 nIndex)
{
	SAFE_DELETE(m_children[nIndex]);
	m_dwComputeFlag &= ~(1 << nIndex);
}