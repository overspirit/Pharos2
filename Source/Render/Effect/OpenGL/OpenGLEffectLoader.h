#pragma once

namespace Pharos
{
	namespace Render
	{
        struct SemaPair
        {
            string name;
            string sema;
        };
        
        struct UniformInfo
        {
            string name;
            uint32 bind;
        };
        
        struct BlockInfo
        {
            string name;
            uint32 bind;
            vector<string> varList;
        };
        
        struct PassInfo
        {
            string vertShader;
            string pixelShader;
            
            RasterizerStateDesc rasterDesc;
            BlendStateDesc	blendDesc;
            DepthStencilStateDesc depthDesc;
        };
        
        struct TechniqueInfo
        {
            string techName;
            
            vector<SemaPair>        attrInfos;
            vector<UniformInfo>     uniformInfos;
            vector<BlockInfo>       blockInfos;
            vector<PassInfo>        passInfos;
        };
        
		class OpenGLEffectLoader : public RenderEffectLoader
		{
		public:
			OpenGLEffectLoader(void);
			virtual ~OpenGLEffectLoader(void);

		private:
			typedef bool (OpenGLEffectLoader::*LOAD_FUNC)(IXmlNodePtr pNode);

		private:
			struct Member
			{
				string type;
				string name;
				string value;
				string bind;
                string size;
				vector<Member>  children;
			};

		private:
			map<string, LOAD_FUNC>  m_funcList;

			string		m_filePath;

			vector<Member>  m_attrList;
			vector<Member>  m_varyingList;
			vector<Member>  m_uniformList;
			vector<Member>  m_constList;
			vector<Member>  m_targetList;
			vector<Member>  m_blockList;			
			vector<Member>	m_shaderTextList;
			vector<Member>	m_techniqueList;

			vector<TechniqueInfo>	m_techInfoList;

		private:
			virtual const char8* FindShaderText(const char8* name);

			virtual bool LoadPublic(IXmlNodePtr pNode);
			virtual bool LoadAttribute(IXmlNodePtr pNode);
			virtual bool LoadVarying(IXmlNodePtr pNode);
			virtual bool LoadUniform(IXmlNodePtr pNode);
			virtual bool LoadBlock(IXmlNodePtr pNode);
			virtual bool LoadConst(IXmlNodePtr pNode);
			virtual bool LoadTarget(IXmlNodePtr pNode);
			virtual bool LoadShader(IXmlNodePtr pNode);
			virtual bool LoadTechnique(IXmlNodePtr pNode);

		public:
			virtual bool Load(const char8* szPath);

			virtual const vector<TechniqueInfo>& GetTechInfoList() { return m_techInfoList; }
		};
	}

	typedef std::shared_ptr<Render::OpenGLEffectLoader> OpenGLEffectLoaderPtr;
}
