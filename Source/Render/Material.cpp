#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::SetRenderTechnique(const char8* techName)
{
	m_renderTech = sRenderMgr->GenerateRenderTechnique(techName);
	if (m_renderTech != nullptr)
	{
		m_worldVar = m_renderTech->GetVariable("g_world");
		m_viewVar = m_renderTech->GetVariable("g_view");
		m_projVar = m_renderTech->GetVariable("g_proj");
		m_eyePosVar = m_renderTech->GetVariable("g_eye_pos");
	}
}

void Material::SetParameterValue(const char8* valueName, float32 vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue, 0, 0, 1.0f));
}

void Material::SetParameterValue(const char8* valueName, const Vector2Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, 0, 1.0f));
}

void Material::SetParameterValue(const char8* valueName, const Vector3Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(Vector4Df(vlaue.x, vlaue.y, vlaue.z, 1.0f));
}

void Material::SetParameterValue(const char8* valueName, const Vector4Df& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(vlaue);
}

void Material::SetParameterValue(const char8* valueName, const Matrix4& vlaue)
{
	if (m_renderTech == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(vlaue);
}

void Material::SetParameterValue(const char8* valueName, RenderTexture* texture)
{
	if (m_renderTech == nullptr) return;
	if (texture == nullptr) return;

	RenderVariable* var = m_renderTech->GetVariable(valueName);
	if (var == nullptr) return;

	var->SetValue(texture);
}

