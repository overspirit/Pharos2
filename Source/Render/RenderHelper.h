#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderHelper : public Utils::Singleton<RenderHelper>
		{
		public:
            RenderHelper();
			virtual ~RenderHelper();

		private:
			struct MemberInfo
			{
				string	name;
				uint32	size;
			};
			
			struct UniformInfo
			{
				string				name;
				vector<MemberInfo>	memberList;
				uint32				size;
				uint32				slot;
			};
			
			struct VariableInfo
			{
				string	type;
				string	name;
				uint32	slot;
			};

			struct TechniqueInfo
			{
				string			techName;
				RenderProgram*	renderProgram;

				vector<UniformInfo>		uniformInfos;
				vector<VariableInfo>	variableInfos;
			};

		private:
			map<string, TechniqueInfo>		m_techList;
            map<string, Mesh*>              m_meshList;
            
			vector<Material*>	m_materialList;            
            vector<Model*>    m_modelList;
            
		private:
			uint32 GetTypeSize(const char8* type);
			
		public:
			virtual bool LoadEffectFile(const char8* szPath);

			virtual void Destroy();

			virtual Material* GenerateMaterial(const char8* tech, const char8* name);
            virtual Mesh* GenerateMesh(const char8* name);
            virtual Model* GenerateModel(const char8* name);
            
            virtual void SetAllMaterialParam(const Matrix4& view, const Matrix4& proj, const Vector3Df& camerPos);
		};
	}
}
