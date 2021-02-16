#pragma once

namespace Pharos
{
	namespace Scene
	{
		class SceneMgr : public Utils::Singleton<SceneMgr>
		{
		public:
			SceneMgr();
			virtual ~SceneMgr();

		private:
			OctreeScene*	m_currScene;

			map<string, SceneNode*>		m_sceneNodeList;

		public:
			virtual bool Init();
			virtual void Destroy();

			virtual OctreeScene* CreateScene();
			virtual SceneNode* CreateSceneNode(const char8* name, SceneNode* parent);
			virtual SceneCamera* CreateCamera();

			virtual SceneNode* GetSceneNode(const char8* name);

			virtual void SetCurrScene(OctreeScene* scene);
			virtual OctreeScene* GetCurrScene(){ return m_currScene; }

			virtual SceneImporter* CreateSceneImporter(const char8* sceneFile);

			virtual void Update(float32 fElapsed);
		};
	}
}
