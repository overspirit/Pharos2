#pragma once

namespace Pharos
{
	namespace Scene
	{
		class XmlSceneImporter : public SceneImporter
		{
		public:
			XmlSceneImporter(void);
			virtual ~XmlSceneImporter(void);
						
		private:
			bool ReadMaterial(XmlNode* rootNode);
			bool ReadMesh(XmlNode* rootNode);
			bool ReadBoneInfo(XmlNode* rootNode);
			bool ReadAnimation(XmlNode* rootNode);
			
			bool CompareVertexDesc(const vector<VertLayoutDesc>& desc1, const vector<VertLayoutDesc>& desc2);	

		public:
			virtual bool LoadSceneFile(const char8* file);
			virtual bool LoadModelFile(const char8* file);
		};
	}

	typedef shared_ptr<Scene::XmlSceneImporter>	XmlSceneImporterPtr;
}