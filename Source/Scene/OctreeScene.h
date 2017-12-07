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

			GridNode*				m_gridNode;

		public:
			TreeNode* GetTreeRootNode(){ return m_pTreeRoot; }

			//virtual void onLoadSuccess();

		public:
			virtual void Init();

			virtual void SetSceneSize(const Size2Di& mapSize, uint32 sceneHeight);

			virtual SceneCamera* GetSceneCamera(){ return m_camera; }

			virtual void AddSceneNode(SceneNode* pNode);
			virtual void RemoveSceneNode(SceneNode* pNode);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}
}

