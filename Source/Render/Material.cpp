#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

Material::Material()
{
	m_renderTech = nullptr;

	m_worldVar = nullptr;
	m_viewVar = nullptr;
	m_projVar = nullptr;
	m_eyePosVar = nullptr;
}

Material::~Material()
{
	SAFE_DELETE(m_renderTech);

	for (RenderTexture* tex : m_texList)
	{
		SAFE_DELETE(tex);
	}
}

void Material::SetRenderTechnique(const char8* techName)
{
 	m_renderTech = sRenderMgr->GenerateRenderTechnique(techName);

	m_techName = techName;

	if (m_renderTech != nullptr)
	{
		m_worldVar = m_renderTech->GetVariable("g_world");
		m_viewVar = m_renderTech->GetVariable("g_view");
		m_projVar = m_renderTech->GetVariable("g_proj");
		m_eyePosVar = m_renderTech->GetVariable("g_eye_pos");

		if (m_worldVar == nullptr ||
			m_viewVar == nullptr ||
			m_projVar == nullptr)
		{
			assert(false);
		}

		m_worldVar->SetDataSize(64);
		m_viewVar->SetDataSize(64);
		m_projVar->SetDataSize(64);
		if(m_eyePosVar != nullptr) m_eyePosVar->SetDataSize(16);
	}
}

void Material::SetParameterValue(const char8* valueName, float32 vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue, 0, 0, 1.0f));

	m_variableList[valueName] = var;
}

void Material::SetParameterValue(const char8* valueName, const Vector2Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, 0, 1.0f));

	m_variableList[valueName] = var;
}

void Material::SetParameterValue(const char8* valueName, const Vector3Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, vlaue.z, 1.0f));

	m_variableList[valueName] = var;
}

void Material::SetParameterValue(const char8* valueName, const Vector4Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(vlaue);

	m_variableList[valueName] = var;
}

void Material::SetParameterValue(const char8* valueName, const Matrix4& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(vlaue);

	m_variableList[valueName] = var;
}

void Material::SetParameterValue(const char8* valueName, RenderTexture* texture)
{
	if (m_renderTech == nullptr) return;
	if (texture == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(texture);

	m_variableList[valueName] = var;

	m_texList.push_back(texture);
}

void Material::Apply()
{
	for (auto& iter : m_variableList)
	{
		string valueName = iter.first;
		RenderVariable* var = iter.second;
	}
}