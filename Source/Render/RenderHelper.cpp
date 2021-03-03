#include "PreCompile.h"
#include "Pharos.h"

RenderHelper::RenderHelper()
{

}

RenderHelper::~RenderHelper()
{

}

bool RenderHelper::LoadEffectFile(const char8* szPath)
{
	XmlDocument* effectFile = sResMgr->GenerateXmlDocument(szPath);
	if (effectFile == nullptr) return false;	

	XmlNode* rootNode = effectFile->GetRootNode();
	XmlNode* libraryNode = rootNode->GetFirstNode("library");
	XmlAttribute* fileAttr = libraryNode->GetAttribute("file");
	const char8* libraryName = fileAttr->GetStringValue();


	vector<UniformInfo>		uniformInfos;
	vector<VariableInfo>	variableInfos;

	XmlNode* varNode = rootNode->GetFirstNode("variable");
	while (varNode != NULL)
	{
		XmlAttribute* typeAttr = varNode->GetAttribute("type");
		const char8* varType = typeAttr->GetStringValue();

		XmlAttribute* nameAttr = varNode->GetAttribute("name");
		const char8* varName = nameAttr->GetStringValue();

		XmlAttribute* bindAttr = varNode->GetAttribute("bind");
		uint32 varBind = (uint32)bindAttr->GetIntValue();

		if (strcmp(varType, "texture") == 0)
		{
			VariableInfo varInfo;
			varInfo.name = varName;
			varInfo.type = varType;
			varInfo.slot = varBind;
			variableInfos.push_back(varInfo);
		}
		else if (strcmp(varType, "struct") == 0)
		{
			UniformInfo uniformInfo;
			uniformInfo.name = varName;			
			uniformInfo.slot = varBind;
			
			uint32 size = 0;

			for (uint32 i = 0; i < varNode->GetChildNum(); i++)
			{
				XmlNode* memberNode = varNode->GetChildNode(i);
					
				XmlAttribute* memberTypeAttr = memberNode->GetAttribute("type");
				const char8* memberType = memberTypeAttr->GetStringValue();

				XmlAttribute* memberNameAttr = memberNode->GetAttribute("name");
				const char8* memberName = memberNameAttr->GetStringValue();

				uint32 typeSize = GetTypeSize(memberType);

				XmlAttribute* memberArraySizeAttr = memberNode->GetAttribute("array_size");
				if (memberArraySizeAttr != nullptr)
				{
					int32 arraySize = memberArraySizeAttr->GetIntValue();
					typeSize = GetTypeSize(memberType) * arraySize;
				}

				MemberInfo memberInfo;
				memberInfo.name = memberName;
				memberInfo.size = typeSize;
				uniformInfo.memberList.push_back(memberInfo);

				size += typeSize;
			}

			uniformInfo.size = size;

			uniformInfos.push_back(uniformInfo);
		}

		varNode = varNode->GetNextSibling("variable");
	}


	XmlNode* techNode = rootNode->GetFirstNode("technique");
	while (techNode != NULL)
	{
		XmlAttribute* techNameAttr =techNode->GetAttribute("name");
		const char8* techName = techNameAttr->GetStringValue();

		RenderProgram* renderProgram = sRenderer->GenerateRenderProgram();
		renderProgram->SetLibraryWithPath(libraryName);

		for (uint32 i = 0; i < techNode->GetChildNum(); i++)
		{
			XmlNode* paramNode = techNode->GetChildNode(i);
			XmlAttribute* paramNameAttr = paramNode->GetAttribute("name");
			const char8* paramName = paramNameAttr->GetStringValue();

			if (strcmp(paramName, "vertex_shader") == 0)
			{
				XmlAttribute* valueAttr = paramNode->GetAttribute("value");
				const char8* shaderName = valueAttr->GetStringValue();

				renderProgram->CompileVertexFunctionWithName(shaderName);
			}
			else if (strcmp(paramName, "pixel_shader") == 0)
			{
				XmlAttribute* valueAttr = paramNode->GetAttribute("value");
				const char8* shaderName = valueAttr->GetStringValue();

				renderProgram->CompileFragmentFunctionWithName(shaderName);
			}			
		}

		TechniqueInfo techInfo;
		techInfo.techName = techName;
		techInfo.renderProgram = renderProgram;
		techInfo.uniformInfos = uniformInfos;
		techInfo.variableInfos = variableInfos;

		m_techList[techName] = techInfo;

		techNode = techNode->GetNextSibling("technique");
	}

	return true;
}

void RenderHelper::Destroy()
{
	for (auto& iter : m_techList)
	{
		SAFE_DELETE(iter.second.renderProgram);
	}

	m_techList.clear();
	
    for (auto material : m_materialList)
    {
        SAFE_DELETE(material);
    }
    
	m_materialList.clear();
    
    for (auto& iter : m_meshList)
    {
        SAFE_DELETE(iter.second);
    }
    
    m_meshList.clear();
    
    for (auto* model : m_modelList)
    {
        SAFE_DELETE(model);
    }
    
    m_modelList.clear();
}

Material* RenderHelper::GenerateMaterial(const char8* tech, const char8* name)
{
	if (m_techList.find(tech) == m_techList.end()) return nullptr;
	
	TechniqueInfo& techInfo = m_techList[tech];	

	Material* material = new Material(techInfo.techName.c_str());
	if (!material->InitWithShaderProgram(techInfo.renderProgram))
    {
        SAFE_DELETE(material);
        return NULL;
    }
    
	m_materialList.push_back(material);

	return material;
}

Mesh* RenderHelper::GenerateMesh(const char8* name)
{
    Mesh* mesh = new Mesh(name);
    
    m_meshList[name] = mesh;
    
    return mesh;
}

Model* RenderHelper::GenerateModel(const char8* name)
{
    Model* model = new Model(name);
    
    m_modelList.push_back(model);
    
    return model;
}

void RenderHelper::SetAllMaterialParam(const Matrix4& view, const Matrix4& proj, const Vector3Df& camerPos)
{
    for (auto material : m_materialList)
    {        
        material->SetViewParamValue(view, camerPos);
        material->SetProjParamValue(proj);    

        material->UpdateParamValue();
    }
}

uint32 RenderHelper::GetTypeSize(const char8* type)
{
    if (strcmp(type, "matrix4") == 0) return sizeof(Matrix4);
    else if (strcmp(type, "float4") == 0) return sizeof(Vector4Df);
    else if (strcmp(type, "float3") == 0) return sizeof(Vector3Df);
	else if (strcmp(type, "float2") == 0) return sizeof(Vector2Df);
	else if (strcmp(type, "float") == 0) return sizeof(float32);

	return 0;
}
