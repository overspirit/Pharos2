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
				SampleStateDesc	stateDesc;
			};

			struct TypeValue
			{
				Engine::PropType type;
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
				IMemBufferPtr					vertexData;
				vector<VertLayoutDesc>			vertDesc;

				IMemBufferPtr					indexData;

				MaterialData					materialData;

				DrawType						drawType;
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
				ModelData	modelData;
				vector<SceneNodeData>		childData;
			};

		protected:
			string				m_resFile;

			map<string, MaterialData>	m_materialDataList;
			ModelData					m_modelData;
			vector<SceneNodeData>		m_nodeDataList;
			
		private:
			IMaterialPtr CreateMaterial(const MaterialData& materialData);
			IMeshPtr CreateMesh(const MeshData& meshData);
			IModelPtr CreateModel(const ModelData& modelData);
			ISceneNodePtr CreateNode(const SceneNodeData& data);

		public:
			virtual bool ImportScene(ISceneGraphPtr scene);

			virtual bool LoadSceneFile(const char8* file) = 0;
			virtual bool LoadModelFile(const char8* file) = 0;
		};
	}

	typedef shared_ptr<Scene::SceneImporter>	SceneImporterPtr;
}