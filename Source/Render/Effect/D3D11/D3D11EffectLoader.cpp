#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11EffectLoader::D3D11EffectLoader()
{
	m_funcList["include"] = (LOAD_FUNC)&D3D11EffectLoader::LoadInclude;
	m_funcList["constant"] = (LOAD_FUNC)&D3D11EffectLoader::LoadConstant;
	m_funcList["variable"] = (LOAD_FUNC)&D3D11EffectLoader::LoadVariable;
	m_funcList["macro"] = (LOAD_FUNC)&D3D11EffectLoader::LoadMacro;
	m_funcList["struct"] = (LOAD_FUNC)&D3D11EffectLoader::LoadStruct;
	m_funcList["shader"] = (LOAD_FUNC)&D3D11EffectLoader::LoadShader;
	m_funcList["state"] = (LOAD_FUNC)&D3D11EffectLoader::LoadState;
	m_funcList["technique"] = (LOAD_FUNC)&D3D11EffectLoader::LoadTechnique;

	m_enumList["cull_mode"] = (ENUM_FUNC)&D3D11EffectLoader::GetEnumFromStr<ECullMode, CullModeDefine>;
}

D3D11EffectLoader::~D3D11EffectLoader()
{
}

bool D3D11EffectLoader::Load(const char8* szPath)
{	
	XmlDocument doc;
	if (!doc.Open(szPath)) return false;
	if (!doc.Load()) return false;

	m_filePath = doc.GetResFilePath();

	XmlNode* pRoot = doc.GetRootNode();
	for(uint32 i = 0; i < pRoot->GetChildNum(); i++)
	{
		XmlNode* pChild = pRoot->GetChildNode(i);
		string name = pChild->GetName();
		map<string, LOAD_FUNC>::iterator iter = m_funcList.find(name);
		if (iter != m_funcList.end())
		{
			LOAD_FUNC func = iter->second;
			if(!(this->*func)(pChild))
			{
				return false;
			}
		}
	}

	stringstream ss;

	for(uint32 i = 0; i < m_macroList.size(); i++)
	{
		ss << "#define " << m_macroList[i].name << " " << m_macroList[i].value << "\n";
	}

	ss << "\n";

	for(uint32 i = 0; i < m_constantList.size(); i++)
	{
		ss << "cbuffer " << m_constantList[i].name << " ";
		ss << ": register(" << m_constantList[i].bind << ")\n";
		ss << "{\n";
		for(uint32 m = 0; m < m_constantList[i].children.size(); m++)
		{
			Member& member = m_constantList[i].children[m];
			ss << "\t" << member.type << " " << member.name;

			if(!member.bind.empty())
			{
				ss << " : packoffset(" << member.bind << ");\n";
			}
			else
			{
				ss << ";\n";
			}
		}
		ss << "};\n\n";
	}

	for(uint32 i = 0; i < m_variableList.size(); i++)
	{
		ss << m_variableList[i].type << " " << m_variableList[i].name;
		if(!m_variableList[i].bind.empty())
		{
			ss << " : register(" << m_variableList[i].bind << ");\n";
		}
		else
		{
			ss << ";\n";
		}
	}

	ss << "\n";

	for(uint32 i = 0; i < m_structList.size(); i++)
	{
		ss << "struct " << m_structList[i].name << "\n";
		ss << "{\n";
		for(uint32 m = 0; m < m_structList[i].children.size(); m++)
		{
			Member& member = m_structList[i].children[m];
			ss << "\t" << member.type << " " << member.name;
			ss << " : " << member.bind << ";\n";
		}
		ss << "};\n\n";
	}

	m_effectText = ss.str() + m_shaderText;

	//sema list
	//////////////////////////////////////////////////////////////////////////
	uint32 structNum = (uint32)m_structList.size();
	vector<SemaStruct> structList(structNum);
	for (uint32 i = 0; i < structNum; i++)
	{
		uint32 semaNum = (uint32)m_structList[i].children.size();
		vector<SemaPair> semaList(semaNum);
		for (uint32 j = 0; j < semaNum; j++)
		{
			semaList[j].name = m_structList[i].children[j].name;
			semaList[j].sema = m_structList[i].children[j].bind;
		}

		structList[i].name = m_structList[i].name;
		structList[i].semaList = semaList;
	}
	//////////////////////////////////////////////////////////////////////////

	//constant buffer
	//////////////////////////////////////////////////////////////////////////
	uint32 cbuffNum = (uint32)m_constantList.size();
	vector<ConstantBuffInfo> varBufList(cbuffNum);
	for (uint32 i = 0; i < cbuffNum; i++)
	{
		uint32 paramNum = (uint32)m_constantList[i].children.size();
		vector<string> varInfoList(paramNum);
		for (uint32 j = 0; j < paramNum; j++)
		{
			Member& paramMember = m_constantList[i].children[j];
			varInfoList[j] = paramMember.name;
		}

		ConstantBuffInfo& varBufInfo = varBufList[i];
		varBufInfo.name = m_constantList[i].name;
		varBufInfo.bind = strtol(m_constantList[i].bind.c_str() + 1, NULL, 10);
		varBufInfo.varList = varInfoList;
	}
	//////////////////////////////////////////////////////////////////////////

	//variable buffer
	//////////////////////////////////////////////////////////////////////////
	uint32 varNum = (uint32)m_variableList.size();
	vector<VariableInfo> varList(varNum);
	for (uint32 i = 0; i < m_variableList.size(); i++)
	{
		varList[i].name = m_variableList[i].name;
		varList[i].bind = strtol(m_variableList[i].bind.c_str() + 1, NULL, 10);
	}
	//////////////////////////////////////////////////////////////////////////

	uint32 techNum = (uint32)m_techniqueList.size();
	m_techInfoList.resize(techNum);
	for (uint32 i = 0; i < techNum; i++)
	{
		Member& techMember = m_techniqueList[i];

		uint32 passNum = (uint32)techMember.children.size();
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
					passInfo.vertEnter = param.value;
				}
				else if (param.name == "pixel_shader")
				{
					passInfo.pixelEnter = param.value;
				}
			}
		}

		TechniqueInfo& techInfo = m_techInfoList[i];
		techInfo.techName = m_techniqueList[i].name;
		techInfo.semaInfos = structList;
		techInfo.varInfos = varList;
		techInfo.constantBufInfos = varBufList;
		techInfo.passInfos = passList;
	}

	for (uint32 i = 0; i < m_techInfoList.size(); i++)
	{
		D3D11RenderTechnique* tech = new D3D11RenderTechnique();
		if (tech->Create(m_effectText.c_str(), m_techInfoList[i]))
		{
			m_techList.push_back(tech);
		}
		else
		{
			assert(false);
		}
	}
	
	return true;
}

bool D3D11EffectLoader::LoadConstant(XmlNode* pNode)
{
	m_constantList.resize(m_constantList.size() + 1);
	Member& constant = m_constantList[m_constantList.size() - 1];

	XmlAttribute* pAttr = pNode->GetAttribute("name");
	if(pAttr != nullptr) constant.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("bind");
	if(pAttr != nullptr) constant.bind = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("size");
	if (pAttr != nullptr) constant.size = pAttr->GetStringValue();

	constant.children.resize(pNode->GetChildNum());

	for(uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		Member& child = constant.children[i];
		XmlNode* pChildInfo = pNode->GetChildNode(i);

		pAttr = pChildInfo->GetAttribute("type");
		if(pAttr != nullptr) child.type = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("name");
		if(pAttr != nullptr) child.name = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("bind");
		if(pAttr != nullptr) child.bind = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("size");
		if (pAttr != nullptr) child.size = pAttr->GetStringValue();

	}

	return true;
}

bool D3D11EffectLoader::LoadVariable(XmlNode* pNode)
{
	m_variableList.resize(m_variableList.size() + 1);
	Member& variable = m_variableList[m_variableList.size() - 1];

	XmlAttribute* pAttr = pNode->GetAttribute("type");
	if(pAttr != nullptr) variable.type = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("name");
	if(pAttr != nullptr) variable.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("bind");
	if(pAttr != nullptr) variable.bind = pAttr->GetStringValue();

	return true;
}

bool D3D11EffectLoader::LoadStruct(XmlNode* pNode)
{
	m_structList.resize(m_structList.size() + 1);
	Member& structInfo = m_structList[m_structList.size() - 1];

	XmlAttribute* pAttr = pNode->GetAttribute("name");
	if(pAttr != nullptr) structInfo.name = pAttr->GetStringValue();

	structInfo.children.resize(pNode->GetChildNum());

	for(uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		Member& member = structInfo.children[i];
		XmlNode* pChildInfo = pNode->GetChildNode(i);

		pAttr = pChildInfo->GetAttribute("type");
		if(pAttr != nullptr) member.type = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("name");
		if(pAttr != nullptr) member.name = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("semantic");
		if(pAttr != nullptr) member.bind = pAttr->GetStringValue();
	}

	return true;
}

bool D3D11EffectLoader::LoadMacro(XmlNode* pNode)
{
	m_macroList.resize(m_macroList.size() + 1);
	Member& macro = m_macroList[m_macroList.size() - 1];

	XmlAttribute* pAttr = pNode->GetAttribute("name");
	if(pAttr != nullptr) macro.name = pAttr->GetStringValue();

	pAttr = pNode->GetAttribute("value");
	if(pAttr != nullptr) macro.value = pAttr->GetStringValue();

	return true;
}

bool D3D11EffectLoader::LoadInclude(XmlNode* pNode)
{
	XmlAttribute* pAttr = pNode->GetAttribute("file");
	if(pAttr != nullptr)
	{
		const char8* szPublic = pAttr->GetStringValue();
		if (!this->Load(szPublic))
		{
			Utils::Path relativePath(m_filePath.c_str());
			string fullPath = string(relativePath.GetFullPath()) + szPublic;
			if (!this->Load(fullPath.c_str())) return false;
		}
	}

	return true;
}

bool D3D11EffectLoader::LoadShader(XmlNode* pNode)
{
	for (uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		XmlNode* pChild = pNode->GetChildNode(i);
		if (pChild != nullptr)
		{
			m_shaderText += pChild->GetInnerText();
		}
	}

	return true;
}

bool D3D11EffectLoader::LoadState(XmlNode* pNode)
{
	return true;
}

bool D3D11EffectLoader::LoadTechnique(XmlNode* pNode)
{
	m_techniqueList.resize(m_techniqueList.size() + 1);
	Member& tech = m_techniqueList[m_techniqueList.size() - 1];

	XmlAttribute* pAttr = pNode->GetAttribute("name");
	if (pAttr != nullptr) tech.name = pAttr->GetStringValue();

	tech.children.resize(pNode->GetChildNum());
	for (uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		Member& pass = tech.children[i];
		XmlNode* pPassInfo = pNode->GetChildNode(i);
		
		pAttr = pPassInfo->GetAttribute("name");
		if (pAttr != nullptr) pass.name = pAttr->GetStringValue();
		
		pass.children.resize(pPassInfo->GetChildNum());
		for (uint32 j = 0; j < pPassInfo->GetChildNum(); j++)
		{
			Member& param = pass.children[j];
			XmlNode* pParamInfo = pPassInfo->GetChildNode(j);

			pAttr = pParamInfo->GetAttribute("name");
			if (pAttr != nullptr) param.name = pAttr->GetStringValue();

			pAttr = pParamInfo->GetAttribute("value");
			if (pAttr != nullptr) param.value = pAttr->GetStringValue();
		}
	}

	return true;
}