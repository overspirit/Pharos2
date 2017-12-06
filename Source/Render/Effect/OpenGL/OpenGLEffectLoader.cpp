#include "PreCompile.h"
#include "RenderGlobal.h"

OpenGLEffectLoader::OpenGLEffectLoader(void)
{
	m_funcList["include"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadPublic;
	m_funcList["attribute"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadAttribute;
	m_funcList["varying"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadVarying;
	m_funcList["uniform"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadUniform;
	m_funcList["block"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadBlock;
	m_funcList["const"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadConst;
	m_funcList["target"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadTarget;
	m_funcList["shader"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadShader;
	m_funcList["technique"] = (LOAD_FUNC)&OpenGLEffectLoader::LoadTechnique;
}

OpenGLEffectLoader::~OpenGLEffectLoader(void)
{
    
}

bool OpenGLEffectLoader::Load(const char8* szPath)
{
	IXmlDocumentPtr doc = g_pKernel->QueryXmlDocResource(szPath);
	if(doc == NULL) return false;

	m_filePath = doc->GetResFilePath();

	IXmlNodePtr pRoot = doc->GetRootNode();
	for(uint32 i = 0; i < pRoot->GetChildNum(); i++)
	{
		IXmlNodePtr pChild = pRoot->GetChildNode(i);
		string name = pChild->GetName();
		map<string, LOAD_FUNC>::iterator iter = m_funcList.find(name);
		if(iter != m_funcList.end())
		{
			LOAD_FUNC func = iter->second;
			if(!(this->*func)(pChild))
			{
				return false;
			}
		}
	}  
    
	//////////////////////////////////////////////////////////////////////////
	stringstream attrStream;
	uint32 attrSize = m_attrList.size();
	vector<SemaPair> attrInfos(attrSize);
	for(uint32 i = 0; i < attrSize; i++)
	{
		Member& member = m_attrList[i];
		attrStream << "in ";
		attrStream << member.type << " ";
		attrStream << member.name << ";\n";

		attrInfos[i].name = member.name;
		attrInfos[i].sema = member.bind;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	stringstream inVaryStream;
	for(uint32 i = 0; i < m_varyingList.size(); i++)
	{
		Member& member = m_varyingList[i];
		inVaryStream << "in ";
		inVaryStream << member.type << " ";
		inVaryStream << member.name << ";\n";
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	stringstream outVaryStream;
	for(uint32 i = 0; i < m_varyingList.size(); i++)
	{
		Member& member = m_varyingList[i];
		outVaryStream << "out ";
		outVaryStream << member.type << " ";
		outVaryStream << member.name << ";\n";
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////    
	stringstream uniformStream;
	uint32 uniformSize = m_uniformList.size();
	vector<UniformInfo> uniformInfos(uniformSize);
	for(uint32 i = 0; i < uniformSize; i++)
	{
		Member& member = m_uniformList[i];
		uniformStream << "uniform ";
		uniformStream << member.type << " ";
		uniformStream << member.name << ";\n";

		uniformInfos[i].name = m_uniformList[i].name;
		uniformInfos[i].bind = strtol(m_uniformList[i].bind.c_str(), NULL, 10);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////    
	stringstream targetStream;
	for(uint32 i = 0; i < m_targetList.size(); i++)
	{
		Member& member = m_targetList[i];
        if(!member.bind.empty()) targetStream << "layout(location=" << member.bind << ") ";
        targetStream << "out ";
		targetStream << member.type << " ";
		targetStream << member.name << ";\n";
	}
	//////////////////////////////////////////////////////////////////////////
    
	//////////////////////////////////////////////////////////////////////////
	stringstream constStream;
	for(uint32 i = 0; i < m_constList.size(); i++)
	{
		Member& member = m_constList[i];
		constStream << "const ";
		constStream << member.type << " ";
		constStream << member.name << " = ";
		constStream << member.value << ";\n";
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	stringstream blockStream;
	uint32 blockSize = m_blockList.size();
	vector<BlockInfo> blockList(blockSize);
	for(uint32 i = 0; i < m_blockList.size(); i++)
	{
		Member& block = m_blockList[i];
        if (!block.type.empty())
        {
            blockStream << "layout(" << block.type << ") ";
        }
        blockStream << "uniform " << block.name << "\n";
		blockStream << "{\n";

		BlockInfo& blockInfo = blockList[i];
		blockInfo.name = block.name;
		blockInfo.bind = strtol(block.bind.c_str(), NULL, 10);
		blockInfo.varList.resize(block.children.size());

		for(uint32 m = 0; m < block.children.size(); m++)
		{
			Member& member = block.children[m];
			blockStream << "\t";
			blockStream << member.type << " ";
			blockStream << member.name << ";\n";

			blockInfo.varList[m] = member.name;
		}

		blockStream << "};\n\n";
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	string vertexPublic = "#version 300 es\n\n";
	vertexPublic += attrStream.str() + "\n";
	vertexPublic += outVaryStream.str() + "\n";
	vertexPublic += uniformStream.str() + "\n";
	vertexPublic += constStream.str() + "\n";
	vertexPublic += blockStream.str() + "\n";
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////    
	string fragmentPublic = "#version 300 es\n\n";
	fragmentPublic += inVaryStream.str() + "\n";
	fragmentPublic += targetStream.str() + "\n";
	fragmentPublic += uniformStream.str() + "\n";
	fragmentPublic += constStream.str() + "\n";
	fragmentPublic += blockStream.str() + "\n";
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////    
	uint32 techNum = m_techniqueList.size();
	m_techInfoList.resize(techNum);
	for (uint32 i = 0; i < techNum; i++)
	{
		Member& techMember = m_techniqueList[i];

		uint32 passNum = techMember.children.size();
		vector<PassInfo> passList(passNum);

		for (uint32 j = 0; j < passNum; j++)
		{
			Member& passMember = techMember.children[j];
			PassInfo& passInfo = passList[j];

			for (uint32 k = 0; k < passMember.children.size(); k++)
			{
				Member& param = passMember.children[k];

				FillBlendState(passInfo.blendDesc, param.name.c_str(), param.value.c_str());
				FillDepthStencilState(passInfo.depthDesc, param.name.c_str(), param.value.c_str());
				FillRasterizerState(passInfo.rasterDesc, param.name.c_str(), param.value.c_str());

				if (param.name == "vertex_shader")
				{
					passInfo.vertShader += "#version 300 es\n\n";
					passInfo.vertShader += attrStream.str() + "\n";
					passInfo.vertShader += outVaryStream.str() + "\n";
					passInfo.vertShader += uniformStream.str() + "\n";
					passInfo.vertShader += constStream.str() + "\n";
					passInfo.vertShader += blockStream.str() + "\n";
					passInfo.vertShader += FindShaderText(param.value.c_str());
				}
				else if (param.name == "pixel_shader")
				{
					passInfo.pixelShader += "#version 300 es\n\n";
					passInfo.pixelShader += inVaryStream.str() + "\n";
					passInfo.pixelShader += targetStream.str() + "\n";
					passInfo.pixelShader += uniformStream.str() + "\n";
					passInfo.pixelShader += constStream.str() + "\n";
					passInfo.pixelShader += blockStream.str() + "\n";
					passInfo.pixelShader += FindShaderText(param.value.c_str());
				}
			}
		}

		TechniqueInfo& techInfo = m_techInfoList[i];
		techInfo.techName = m_techniqueList[i].name;
		techInfo.attrInfos = attrInfos;
		techInfo.uniformInfos = uniformInfos;
		techInfo.blockInfos = blockList;
		techInfo.passInfos = passList;
	}
	//////////////////////////////////////////////////////////////////////////
    
	return true;
}

const char8* OpenGLEffectLoader::FindShaderText(const char8* name)
{
	for (auto& member : m_shaderTextList)
	{
		if (member.name == name)
		{
			return member.value.c_str();
		}
	}
	return "";
}

bool OpenGLEffectLoader::LoadPublic(IXmlNodePtr pNode)
{
	IXmlAttributePtr pAttr = pNode->GetAttribute("file");
	if (pAttr != nullptr)
	{
		const char8* szPublic = pAttr->GetStringValue();
		if (!this->Load(szPublic))
		{
			//if (!this->Load(SAME_PATH(m_filePath, szPublic))) return false;
		}
	}

	return true;
}

bool OpenGLEffectLoader::LoadAttribute(IXmlNodePtr pNode)
{
	m_attrList.resize(m_attrList.size() + 1);
	Member& attr = m_attrList[m_attrList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if(pAttr != NULL) attr.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) attr.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("semantic");
    if(pAttr != NULL) attr.bind = pAttr->GetStringValue();
    
	return true;
}

bool OpenGLEffectLoader::LoadVarying(IXmlNodePtr pNode)
{
	m_varyingList.resize(m_varyingList.size() + 1);
	Member& varying = m_varyingList[m_varyingList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if(pAttr != NULL) varying.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) varying.name = pAttr->GetStringValue();

	return true;
}

bool OpenGLEffectLoader::LoadUniform(IXmlNodePtr pNode)
{
	m_uniformList.resize(m_uniformList.size() + 1);
	Member& uniform = m_uniformList[m_uniformList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if(pAttr != NULL) uniform.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) uniform.name = pAttr->GetStringValue();

    pAttr = pNode->GetAttribute("bind");
    if(pAttr != NULL) uniform.bind = pAttr->GetStringValue();
    
	return true;
}

bool OpenGLEffectLoader::LoadBlock(IXmlNodePtr pNode)
{
	m_blockList.resize(m_blockList.size() + 1);
	Member& block = m_blockList[m_blockList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) block.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("layout");
	if (pAttr != NULL) block.type = pAttr->GetStringValue();
    
	pAttr = pNode->GetAttribute("bind");
	if(pAttr != NULL) block.bind = pAttr->GetStringValue();

    pAttr = pNode->GetAttribute("size");
    if(pAttr != NULL) block.size = pAttr->GetStringValue();
    
	block.children.resize(pNode->GetChildNum());

	for(uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		IXmlNodePtr pChild = pNode->GetChildNode(i);
		Member& member = block.children[i];

		pAttr = pChild->GetAttribute("type");
		if(pAttr != NULL) member.type = pAttr->GetStringValue();

		pAttr = pChild->GetAttribute("name");
		if(pAttr != NULL) member.name = pAttr->GetStringValue();
	}

	return true;
}

bool OpenGLEffectLoader::LoadConst(IXmlNodePtr pNode)
{
	m_constList.resize(m_constList.size() + 1);
	Member& constVar = m_constList[m_constList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if(pAttr != NULL) constVar.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) constVar.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("value");
	if(pAttr != NULL) constVar.value = pAttr->GetStringValue();

	return true;
}

bool OpenGLEffectLoader::LoadTarget(IXmlNodePtr pNode)
{
	m_targetList.resize(m_targetList.size() + 1);
	Member& target = m_targetList[m_targetList.size() - 1];

	IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if(pAttr != NULL) target.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != NULL) target.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("bind");
	if(pAttr != NULL) target.bind = pAttr->GetStringValue();

	return true;
}

bool OpenGLEffectLoader::LoadShader(IXmlNodePtr pNode)
{
	m_shaderTextList.resize(m_shaderTextList.size() + 1);
	Member& shaderText = m_shaderTextList[m_shaderTextList.size() - 1];
    
    IXmlAttributePtr pAttr = pNode->GetAttribute("type");
	if (pAttr != NULL) shaderText.type = pAttr->GetStringValue();

    pAttr = pNode->GetAttribute("name");
	if (pAttr != NULL) shaderText.name = pAttr->GetStringValue();

	for (uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		IXmlNodePtr pChild = pNode->GetChildNode(i);
		if (pChild != nullptr)
		{
			shaderText.value += pChild->GetInnerText();
		}
	}
    
	return true;
}

bool OpenGLEffectLoader::LoadTechnique(IXmlNodePtr pNode)
{
	m_techniqueList.resize(m_techniqueList.size() + 1);
    Member& tech = m_techniqueList[m_techniqueList.size() - 1];
    
    IXmlAttributePtr pAttr = pNode->GetAttribute("name");
    if (pAttr != nullptr) tech.name = pAttr->GetStringValue();
    
    tech.children.resize(pNode->GetChildNum());
    for (uint32 i = 0; i < pNode->GetChildNum(); i++)
    {
        Member& pass = tech.children[i];
        IXmlNodePtr pPassInfo = pNode->GetChildNode(i);
        
        pAttr = pPassInfo->GetAttribute("name");
        if (pAttr != nullptr) pass.name = pAttr->GetStringValue();
        
        pass.children.resize(pPassInfo->GetChildNum());
        for (uint32 j = 0; j < pPassInfo->GetChildNum(); j++)
        {
            Member& param = pass.children[j];
            IXmlNodePtr pParamInfo = pPassInfo->GetChildNode(j);
            
            pAttr = pParamInfo->GetAttribute("name");
            if (pAttr != nullptr) param.name = pAttr->GetStringValue();
            
            pAttr = pParamInfo->GetAttribute("value");
            if (pAttr != nullptr) param.value = pAttr->GetStringValue();
        }
    }
    
    return true;
}
