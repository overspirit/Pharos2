#pragma once

namespace Pharos
{
	namespace Scene
	{
		class TreeNode;

		class SceneNode
		{
		public:
			SceneNode();
			virtual ~SceneNode();

		protected:
			TreeNode*	m_pOwner;

			string		m_name;

			bool		m_hidden;

			Matrix4		m_localTransform;
			Matrix4		m_globalTransform;
			Vector3Df	m_localPos;
			Vector3Df	m_globalPos;

			float32		m_boundingRadius;

			SceneNode*				m_parent;
			vector<SceneNode*>		m_childList;

			vector<Model*>		m_modelList;
			RenderObject*		m_renderObj;

		public:
			virtual void SetTreeOwner(TreeNode* pOwner) { m_pOwner = pOwner; }
			virtual TreeNode* GetTreeOwner() { return m_pOwner; }

		public:
			virtual void InitNode(const char8* name, SceneNode* parent);

			virtual const char8* GetNodeName() { return m_name.c_str(); }
			
			virtual uint32 GetModelNum() { return (uint32)m_modelList.size(); }
			virtual Model* GetModel(uint32 index) { return index < (uint32)m_modelList.size() ? m_modelList[index] : nullptr; }
			virtual void AddModel(Model* model) { m_modelList.push_back(model); }

			virtual void SetLocalTransform(const Matrix4& mat);
			virtual void SetLocalPosition(const Vector3Df& pos);
			
			virtual const Matrix4& GetLocalTransform();
			virtual const Matrix4& GetGlobalTransform();
			virtual const Vector3Df& GetLocalPosition();
			virtual const Vector3Df& GetGlobalPosition();

			virtual void SetBoundingRadius(float32 fRadius);
			virtual float32 GetBoundingRadius(){ return m_boundingRadius; }

			virtual void AddChildNode(SceneNode* node);
			virtual void RemoveChildNode(SceneNode* node);

			virtual void SetHidden(bool hidden) { m_hidden = hidden; }
			virtual bool GetHidden() { return m_hidden; }

			virtual void PlayAnim(const char8* animName);

			virtual void Update(float32 fElapsed);
		};
	}
}