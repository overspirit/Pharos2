#pragma once

namespace Pharos
{
	namespace Scene
	{
		class SceneImporter //: public ISceneImporter
		{
		public:
			SceneImporter(void);
			virtual ~SceneImporter(void);
			
		protected:
			struct SamplerData
			{
				string samplerName;
				string texPath;
				SamplerStateDesc	stateDesc;
			};

			struct TypeValue
			{
				Core::PropType type;
				string strValue;
				float32	fValue;
				Vector2Df vt2Value;
				Vector3Df vt3Value;
				Vector4Df vt4Value;
				Matrix4 matValue;
			};

			struct MaterialData
			{
				string	materialName;

				string	techName;
				vector<string>	techDefines;

				map<string, TypeValue>			varList;
				map<string, SamplerData>	samplerDataList;
				map<string, string>			stateList;
			};

			struct MeshData
			{
				MemoryBuffer					vertexData;
				vector<VertLayoutDesc>			vertDesc;
				MemoryBuffer					indexData;
				DrawType						drawType;

				string							materialName;				
			};

			struct ModelData
			{
				vector<MeshData>				meshDataList;
				vector<BoneInfo>				boneInfoList;
				vector<SkelAnimation>			skelAnimList;
			};

			struct SceneNodeData
			{
				string		nodeName;
				string		parentName;
				Matrix4		localTrans;
				float32		boundRadius;

				vector<SceneNodeData>		childData;

				uint32		modelId;				
			};

		protected:
			string				m_resFile;

			map<string, MaterialData>	m_materialDataList;
			map<uint32, ModelData>		m_modelDataList;
			vector<SceneNodeData>		m_nodeDataList;
			
		private:
			Material* CreateMaterial(const MaterialData& materialData);
			Mesh* CreateMesh(const MeshData& meshData);
			Model* CreateModel(const ModelData& modelData);
			SceneNode* CreateNode(const SceneNodeData& data);

			void SaveSceneNodeData(SceneNodeData& nodeData, XmlNode* parentNode, XmlNode* modelRootNode);
			void SaveMaterialData(MaterialData& materialData, XmlNode* materialRootNode);
			uint32 SaveModelData(ModelData& modelData, XmlNode* modelRootNode);

		public:
			virtual bool ImportScene(OctreeScene* scene);
			virtual bool SaveTo(const char8* file);

			virtual bool LoadSceneFile(const char8* file) = 0;
			virtual bool LoadModelFile(const char8* file) = 0;
		};
	}
}