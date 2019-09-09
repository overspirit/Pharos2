#pragma once

namespace Pharos
{
	namespace Scene
	{
		class OctreeScene
		{
		public:
			OctreeScene(void);
			virtual ~OctreeScene(void);
			
		private:
			string		m_strMapFile;
			Size2Di		m_mapSize;
			uint32		m_sceneHeight;

			TreeNode*				m_pTreeRoot;

			vector<SceneNode*>		m_nodeList;

			SceneCamera*			m_camera;	

			bool					m_showGrid;
			GridNode*				m_gridNode;

		public:
			TreeNode* GetTreeRootNode(){ return m_pTreeRoot; }

			//virtual void onLoadSuccess();

		public:
			virtual void Init();

			virtual void SetSceneSize(const Size2Di& mapSize, uint32 sceneHeight);
			virtual void SetSceneGridShow(bool show) { m_showGrid = show; }

			virtual SceneCamera* GetSceneCamera(){ return m_camera; }

			virtual void AddSceneNode(SceneNode* pNode);
			virtual void RemoveSceneNode(SceneNode* pNode);

			virtual uint32 GetSceneNodeNum() { return (uint32)m_nodeList.size(); }
			virtual SceneNode* GetSceneNode(uint32 index) { return index < (uint32)m_nodeList.size() ? m_nodeList[index] : nullptr; }

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}

