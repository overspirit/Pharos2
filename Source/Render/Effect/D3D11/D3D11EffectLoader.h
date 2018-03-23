#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11EffectLoader : public RenderEffectLoader
		{
		public:
			D3D11EffectLoader(void);
			virtual ~D3D11EffectLoader(void);

		protected:
			typedef bool (D3D11EffectLoader::*LOAD_FUNC)(XmlNode* pNode);
			typedef bool (*ENUM_FUNC)(int32* enumValue, void* enumDefine);

		protected:
			struct Member
			{
				string type;
				string name;
				string value;
				string bind;
				string arraySize;				
				vector<Member> children;
			};

		protected:
			map<string, LOAD_FUNC>	m_funcList;
			map<string, ENUM_FUNC>	m_enumList;

			string		m_filePath;

			vector<Member>			m_constantList;
			vector<Member>			m_structList;
			vector<Member>			m_variableList;
			vector<Member>			m_macroList;
			vector<Member>			m_techniqueList;

			string					m_shaderText;
			string					m_effectText;

			vector<D3D11RenderTechInfo*>	m_techInfoList;			

		private:
			virtual bool LoadConstant(XmlNode* pNode);
			virtual bool LoadVariable(XmlNode* pNode);
			virtual bool LoadMacro(XmlNode* pNode);
			virtual bool LoadStruct(XmlNode* pNode);
			virtual bool LoadShader(XmlNode* pNode);
			virtual bool LoadInclude(XmlNode* pNode);
			virtual bool LoadState(XmlNode* pNode);
			virtual bool LoadTechnique(XmlNode* pNode);

		public:
			virtual bool Load(const char8* szPath);
			
			virtual uint32 GetTechniqueInfoNum() { return (uint32)m_techInfoList.size(); }
			virtual RenderTechInfo* GetTechniqueInfo(uint32 index) { return m_techInfoList[index]; }
		};
	}
}
