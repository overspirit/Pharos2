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
			bool ReadMaterialChunk(XmlNode* node);
			bool ReadMeshChunk(XmlNode* node);
			bool ReadSkeletonChunk(XmlNode* node);
			bool ReadAnimationChunk(XmlNode* node);
			bool ReadModel(XmlNode* node, ModelData& modelData);
			bool ReadModelChunk(XmlNode* node);
			bool ReadSceneNodeChunk(XmlNode* node);
			bool ReadSceneNode(XmlNode* node, SceneNodeData& nodeData);

			bool CompareVertexDesc(const vector<VertLayoutDesc>& desc1, const vector<VertLayoutDesc>& desc2);

		public:
			virtual bool LoadSceneFile(const char8* file);
		};
	}

	typedef shared_ptr<Scene::XmlSceneImporter>	XmlSceneImporterPtr;
}