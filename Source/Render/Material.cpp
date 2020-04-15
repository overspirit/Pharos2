#include "PreCompile.h"
#include "Pharos.h"

Material::Material()
{
	m_renderTech = nullptr;
}

Material::~Material()
{
	SAFE_DELETE(m_renderTech);
	
	for (RenderTexture* tex : m_texList)
	{
		SAFE_DELETE(tex);
	}
}

bool Material::SetRenderTechnique(const char8* techName)
{
 	m_renderTech = sRenderMgr->GenerateRenderTechnique(techName);
	if (m_renderTech == nullptr) return false;

	m_techName = techName;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, float32 vlaue)
{
	if (m_renderTech == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(Vector4Df(vlaue, 0, 0, 1.0f));

	m_variableList[valueName] = var;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, const Vector2Df& vlaue)
{
	if (m_renderTech == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, 0, 1.0f));

	m_variableList[valueName] = var;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, const Vector3Df& vlaue)
{
	if (m_renderTech == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, vlaue.z, 1.0f));

	m_variableList[valueName] = var;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, const Vector4Df& vlaue)
{
	if (m_renderTech == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(vlaue);

	m_variableList[valueName] = var;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, const Matrix4& vlaue)
{
	if (m_renderTech == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(vlaue);

	m_variableList[valueName] = var;

	return true;
}

bool Material::SetParameterValue(const char8* valueName, RenderTexture* texture)
{
	if (m_renderTech == nullptr) return false;
	if (texture == nullptr) return false;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return false;

	var->SetValue(texture);

	m_variableList[valueName] = var;

	m_texList.push_back(texture);

	return true;
}
