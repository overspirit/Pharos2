#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

Texture::Texture(void)
{
	m_type = "Texture";

	m_renderImage = nullptr;
	m_renderTech = nullptr;
}

Texture::~Texture(void)
{
	SAFE_DELETE(m_renderImage);
	SAFE_DELETE(m_renderTech);
}

bool Texture::LoadFromXml(XmlNode* xmlNode)
{
	if (!UIObject::LoadFromXml(xmlNode)) return false;

	string imageFile = GetAttributeStringValue(xmlNode, "file");
	m_imageColor = GetAttributeColorValue(xmlNode, "color");

	if(!imageFile.empty())
	{
		Renderer* renderer = sRenderMgr->GetCurrentRenderer();
		m_renderImage = renderer->LoadTexture(imageFile.c_str());

		m_renderTech = sRenderMgr->GenerateRenderTechnique("Sprite2DImage");
		RenderVariable* texVar = m_renderTech->GetVariable("g_tex");
		texVar->SetValue(m_renderImage);
	}
	else
	{
		m_renderTech = sRenderMgr->GenerateRenderTechnique("Sprite2DColor");
	} 		

	m_imageRect.left = GetAttributeIntValue(xmlNode, "left");
	m_imageRect.top = GetAttributeIntValue(xmlNode, "top");
	m_imageRect.right = GetAttributeIntValue(xmlNode, "right");
	m_imageRect.bottom = GetAttributeIntValue(xmlNode, "bottom");

	return true;
}

void Texture::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
}

void Texture::Render(float32 fElapsed)
{
	UIObject::Render(fElapsed);

	Vector4Df uv;

	if (m_renderImage != nullptr)
	{		
		uint32 texWidth = m_renderImage->GetWidth();
		uint32 texHeight = m_renderImage->GetHeight();
		uv.x = (float32)m_imageRect.left / texWidth;
		uv.y = (float32)m_imageRect.top / texHeight;
		uv.z = (float32)m_imageRect.right / texWidth;
		uv.w = (float32)m_imageRect.bottom / texHeight;
	}
	
	sRenderSpirite->RenderRect(m_renderTech, uv, m_imageColor, m_rect);	
}