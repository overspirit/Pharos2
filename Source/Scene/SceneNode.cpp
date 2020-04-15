#include "PreCompile.h"
#include "Pharos.h"

SceneNode::SceneNode()
{
	m_hidden = false;

	m_boundingRadius = 0;

	m_pOwner = nullptr;

	m_boundingRadius = 0;

	m_parent = nullptr;
}

SceneNode::~SceneNode()
{
	for (Model* model : m_modelList)
	{
		SAFE_DELETE(model);
	}
}

void SceneNode::InitNode(const char8* name, SceneNode* parent)
{
	m_name = name;

	m_parent = parent;
}

void SceneNode::SetLocalTransform(const Matrix4& mat)
{
	m_localTransform = mat;
}

void SceneNode::SetLocalPosition(const Vector3Df& pos)
{
	m_localTransform.SetTranslation(pos);
}

const Matrix4& SceneNode::GetLocalTransform()
{
	return m_localTransform;
}

const Matrix4& SceneNode::GetGlobalTransform()
{
	return m_globalTransform;
}

const Vector3Df& SceneNode::GetLocalPosition()
{
	m_localPos = m_localTransform.GetTranslation();

	return m_localPos;
}

const Vector3Df& SceneNode::GetGlobalPosition()
{
	m_globalPos = m_globalTransform.GetTranslation();

	return m_globalPos;
}

void SceneNode::SetBoundingRadius(float32 fRadius)
{
	m_boundingRadius = fRadius;
}

void SceneNode::AddChildNode(SceneNode* node)
{
	m_childList.push_back(node);
	node->m_parent = this;
}

void SceneNode::RemoveChildNode(SceneNode* node)
{
	for (auto iter = m_childList.begin(); iter != m_childList.end(); iter++)
	{
		if (*iter == node)
		{
			m_childList.erase(iter);
			break;
		}
	}
}

void SceneNode::PlayAnim(const char8* animName)
{

}

void SceneNode::Update(float32 fElapsed)
{
	if (m_parent != nullptr)
	{
		m_globalTransform = m_localTransform * m_parent->GetGlobalTransform();
	}
	else
	{
		m_globalTransform = m_localTransform;
	}

	if (m_hidden) return;

	for (auto model : m_modelList)
	{
		model->UpdateAnimation(fElapsed);

		model->TransformWorld(m_globalTransform);

		model->Draw();
	}


	for (uint32 i = 0; i < m_childList.size(); i++)
	{
		m_childList[i]->Update(fElapsed);
	}
}
