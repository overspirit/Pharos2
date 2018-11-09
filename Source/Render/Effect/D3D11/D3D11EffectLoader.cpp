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
	for (uint32 i = 0; i < m_techInfoList.size(); i++)
	{
		SAFE_DELETE(m_techInfoList[i]);
	}
}

bool D3D11EffectLoader::Load(const char8* szPath)
{	
	XmlDocument* doc = sResMgr->GenerateXmlDocument(szPath);
	if (doc == nullptr) return false;
	if (!doc->Load()) return false;
	
	m_filePath = doc->GetResFilePath();

	XmlNode* pRoot = doc->GetRootNode();
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

			if (!member.arraySize.empty())
			{
				ss << "[" << member.arraySize << "]";
			}

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
	
	//建立TechInfo
	//////////////////////////////////////////////////////////////////////////
 	for (uint32 i = 0; i < (uint32)m_techniqueList.size(); i++)
 	{
		Member& techMember = m_techniqueList[i];

		D3D11RenderTechInfo* techInfo = new D3D11RenderTechInfo(); 		
		techInfo->SetTechniqueName(techMember.name.c_str());
		techInfo->SetTechniqueText(m_effectText.c_str());

		//添加Variable 
		//////////////////////////////////////////////////////////////////////////
		for (uint32 j = 0; j < (uint32)m_variableList.size(); j++)
		{
			string varName = m_variableList[j].name;
			uint32 varBind = strtol(m_variableList[j].bind.c_str() + 1, NULL, 10);
			techInfo->AddVariable(varName.c_str(), varBind);
		}
		//////////////////////////////////////////////////////////////////////////

		//添加ConstantBuffer
		//////////////////////////////////////////////////////////////////////////
			for (uint32 j = 0; j < (uint32)m_constantList.size(); j++)
		{
			string name = m_constantList[j].name;
			uint32 bind = strtol(m_constantList[j].bind.c_str() + 1, NULL, 10);
			uint32 contantIndex = techInfo->AddConstantBuffer(name.c_str(), bind);

			for (uint32 k = 0; k < (uint32)m_constantList[j].children.size(); k++)
			{
				Member& paramMember = m_constantList[j].children[k];
				techInfo->AddConstantBufferVariable(contantIndex, paramMember.name.c_str());			
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//添加pass
		//////////////////////////////////////////////////////////////////////////
		for (uint32 j = 0; j < (uint32)techMember.children.size(); j++)
 		{
 			Member& passMember = techMember.children[j];

			BlendStateDesc blendDesc;
			DepthStencilStateDesc depthDesc;
			RasterizerStateDesc rasterDesc;
			string vertEnter;
			string pixelEnter;

 			for (uint32 k = 0; k < passMember.children.size(); k++)
 			{
 				Member& param = passMember.children[k];
 
 				FillBlendState(blendDesc, param.name.c_str(), param.value.c_str());
 				FillDepthStencilState(depthDesc, param.name.c_str(), param.value.c_str());
 				FillRasterizerState(rasterDesc, param.name.c_str(), param.value.c_str());
 				
 				if (param.name == "vertex_shader")
 				{
 					vertEnter = param.value;
 				}
 				else if (param.name == "pixel_shader")
 				{
 					pixelEnter = param.value;
 				}
 			}

			uint32 passIndex = techInfo->AddPass(vertEnter.c_str(), pixelEnter.c_str());
			techInfo->SetPassState(passIndex, rasterDesc);
			techInfo->SetPassState(passIndex, depthDesc);
			techInfo->SetPassState(passIndex, blendDesc);
 		}
		//////////////////////////////////////////////////////////////////////////

		m_techInfoList.push_back(techInfo);
 	}
	//////////////////////////////////////////////////////////////////////////
	
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
	
	constant.children.resize(pNode->GetChildNum());

	for(uint32 i = 0; i < pNode->GetChildNum(); i++)
	{
		Member& child = constant.children[i];
		XmlNode* pChildInfo = pNode->GetChildNode(i);

		pAttr = pChildInfo->GetAttribute("type");
		if(pAttr != nullptr) child.type = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("name");
		if (pAttr != nullptr)
		{
			child.name = pAttr->GetStringValue();

			if (child.name.find('[') != string::npos || child.name.find(']') != string::npos)
			{
				//child.name = "";
			}
		}

		pAttr = pChildInfo->GetAttribute("bind");
		if(pAttr != nullptr) child.bind = pAttr->GetStringValue();

		pAttr = pChildInfo->GetAttribute("array_size");
		if (pAttr != nullptr) child.arraySize = pAttr->GetStringValue();

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