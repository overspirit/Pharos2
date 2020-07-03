#include "PreCompile.h"
#include "Pharos.h"

MaterialMgr::MaterialMgr()
{

}

MaterialMgr::~MaterialMgr()
{

}

bool MaterialMgr::LoadEffectFile(const char8* szPath)
{
	XmlDocument* effectFile = sResMgr->GenerateXmlDocument(szPath);
	effectFile->Load();

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

Material* MaterialMgr::GenerateMaterial(const char8* tech)
{
	Material* material = new Material();

	return material;
}

uint32 MaterialMgr::GetTypeSize(const char8* type)
{
    if (strcmp(type, "matrix4") == 0) return sizeof(Matrix4);
    else if (strcmp(type, "float4") == 0) return sizeof(Vector4Df);
    else if (strcmp(type, "float3") == 0) return sizeof(Vector3Df);
	else if (strcmp(type, "float") == 0) return sizeof(Vector2Df);
	else if (strcmp(type, "float") == 0) return sizeof(float32);
}
