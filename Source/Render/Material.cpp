#include "PreCompile.h"
#include "Pharos.h"

Material::Material()
{
	m_renderPipeline = nullptr;
}

Material::~Material()
{	
	for (RenderTexture* tex : m_texList)
	{
		SAFE_DELETE(tex);
	}
}

bool Material::SetRenderTechnique(const char8* techName)
{
	//m_renderPipeline = sRenderMgr->GetRenderPipeline(techName);
	//if (m_renderPipeline == nullptr) return false;

	m_techName = techName;

	return true;
}

//#error("Material")
bool Material::SetTexture(const char8* valueName, RenderTexture* texture)
{
	m_texList.push_back(texture);
	return true;
}
