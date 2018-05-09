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

	Image image;
	string imageFile = GetAttributeStringValue(xmlNode, "file");
	//if (!image.Open(imageFile)) return false;

	m_imageColor = GetAttributeColorValue(xmlNode, "color");

	//if (image != nullptr)
	if(!imageFile.empty())
	{
		Renderer* renderer = sRenderMgr->GetCurrentRenderer();
		m_renderImage = renderer->LoadTexture(imageFile.c_str());

		m_renderTech = sRenderMgr->GenerateRenderTechnique("Sprite2DImage");
		RenderVariable* texVar = m_renderTech->GetVariable("g_tex");
		texVar->SetValue(m_renderImage);

		m_imageRect.left = GetAttributeIntValue(xmlNode, "left");
		m_imageRect.top = GetAttributeIntValue(xmlNode, "top");
		m_imageRect.right = GetAttributeIntValue(xmlNode, "right");
		m_imageRect.bottom = GetAttributeIntValue(xmlNode, "bottom");
	}
	//else
	{
		//m_renderImage = sRenderMgr->GenerateRenderImage(m_imageColor);
	} 		

	return true;
}

void Texture::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
}

void Texture::Render(float32 fElapsed)
{
	UIObject::Render(fElapsed);

	if (m_renderImage != nullptr)
	{
		Vector4Df uv;
		uint32 texWidth = m_renderImage->GetWidth();
		uint32 texHeight = m_renderImage->GetHeight();
		uv.x = (float32)m_imageRect.left / texWidth;
		uv.y = (float32)m_imageRect.top / texHeight;
		uv.z = (float32)m_imageRect.right / texWidth;
		uv.w = (float32)m_imageRect.bottom / texHeight;

		sRenderSpirite->RenderRect(m_renderTech, uv, m_imageColor, m_rect);
// 		m_renderImage->SetImageColor(m_imageColor);
// 		m_renderImage->SetImageRect(m_imageRect);
// 		m_renderImage->DrawImage(m_rect);
	}
}