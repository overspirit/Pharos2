#pragma once

namespace Pharos
{
	namespace Scene
	{
		class SceneImporter
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

			struct MaterialData
			{
				string	materialName;

				string	techName;
				vector<string>	techDefines;

				map<string, string>			varList;
				map<string, SamplerData>	samplerDataList;
				map<string, string>			stateList;
			};

			struct MeshData
			{
				MeshData()
				{
					drawType = Render::EDT_TRIANGLELIST;
				}

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
				SceneNodeData()
				{
					boundRadius = 0;
				}

				string		nodeName;
				string		parentName;
				Matrix4		localTrans;
				float32		boundRadius;

				vector<SceneNodeData>		childData;

				vector<uint32>		modelIdList;
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

			static PropType GetStringPropType(const char8* str);
			static bool isStringNumeric(const char8* str);
			static Vector2Df ParseVector2(const char8* str);
			static Vector3Df ParseVector3(const char8* str);
			static Vector4Df ParseVector4(const char8* str);
			static Matrix4 ParseMatrix4(const char8* str);

		public:
			virtual bool ImportScene(OctreeScene* scene);
			virtual bool SaveTo(const char8* file);

			virtual bool LoadSceneFile(const char8* file) = 0;
			virtual bool LoadModelFile(const char8* file) = 0;
		};
	}
}