#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderTechnique
		{
		public:
			virtual ~RenderTechnique(){}

		protected:
			struct VarBlock
			{
				uint32						slot;
				RenderShaderData*			shaderData;
				vector<RenderVariable*>		varList;
			};

		protected:
			string		m_techName;

			map<string, RenderVariable*>		m_varMap;	//包含所有变量的Map，便于查找

			vector<RenderVariable*>				m_varList;	//单独的变量，现在只有只有一种Texture
			vector<VarBlock>					m_blockList;//对应D3D11中的ConstantBuf,OpenGL中的UniformBuf

			vector<RenderPass*>					m_passList;

		public:
			virtual bool Create(TechniqueInfo* techInfo) = 0;

			virtual RenderTechnique* Clone() = 0;

			virtual const char8* GetTechName() { return m_techName.c_str(); }

			RenderVariable* GetVariable(const char8* name)
			{
				auto iter = m_varMap.find(name);
				return iter != m_varMap.end() ? iter->second : nullptr;
			}

			uint32 GetPassNum()
			{
				return (uint32)m_passList.size();
			}

			RenderPass* GetPass(uint32 index)
			{
				if (index >= m_passList.size()) return nullptr;
				return m_passList[index];
			}
		};
	}
}