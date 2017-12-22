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
			bool ReadMaterial(XmlNode* node);
			bool ReadMesh(XmlNode* node, ModelData& modelData);
			bool ReadBoneInfo(XmlNode* node, ModelData& modelData);
			bool ReadAnimation(XmlNode* node, ModelData& modelData);
			bool ReadModel(XmlNode* node);
			bool ReadSceneNodeData(XmlNode* node, SceneNodeData& nodeData);
			bool ReadSceneNode(XmlNode* node);

			bool CompareVertexDesc(const vector<VertLayoutDesc>& desc1, const vector<VertLayoutDesc>& desc2);	


		public:
			virtual bool LoadSceneFile(const char8* file);
			virtual bool LoadModelFile(const char8* file);
		};
	}

	typedef shared_ptr<Scene::XmlSceneImporter>	XmlSceneImporterPtr;
}