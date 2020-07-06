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

				string							meshName;

				MemoryBuffer					vertexData;
				vector<VertLayoutDesc>			vertDesc;
				MemoryBuffer					indexData;
				DrawType						drawType;
				uint32							vertCount;
				uint32							faceCount;								
			};

			struct SkeletonData
			{
				string							skelName;
				vector<BoneInfo>				boneInfoList;
			};

			struct TreeData
			{
				string type;
				string id;
				vector<TreeData>	children;
			};

			struct ModelData
			{
				struct MeshInfo
				{
					string id;
					vector<string> materialList;
				};

				vector<MeshInfo>	meshList;
				vector<string>	skeletonList;
				vector<string>	animList;			
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

				vector<string>		modelIdList;
			};

		protected:
			string				m_resFile;

			map<string, MaterialData>	m_materialDataList;
			map<string, MeshData>		m_meshDataList;
			map<string, SkeletonData>	m_skeletonDataList;
			map<string, SkelAnimation>	m_animDataList;
			map<string, ModelData>		m_modelDataList;
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
		};
	}
}