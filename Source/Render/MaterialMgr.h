#pragma once

namespace Pharos
{
	namespace Render
	{
		class MaterialMgr : public Utils::Singleton<MaterialMgr>
		{
		public:
			MaterialMgr();
			virtual ~MaterialMgr();

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

			vector<Material*>	m_materialList;

		private:
			uint32 GetTypeSize(const char8* type);
			
		public:
			virtual bool LoadEffectFile(const char8* szPath);

			virtual void Destroy();

			virtual Material* GenerateMaterial(const char8* tech);
			
			virtual uint32 GetMaterialNum() { return m_materialList.size(); }
			virtual Material* GetMaterial(uint32 index) { return m_materialList[index]; }
		};
	}
}
