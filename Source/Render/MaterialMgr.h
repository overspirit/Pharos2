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

		private:
			uint32 GetTypeSize(const char8* type);
			
		public:
			virtual bool LoadEffectFile(const char8* szPath);
			virtual Material* GenerateMaterial(const char8* tech);
		};
	}
}
