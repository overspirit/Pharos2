#include "PreCompile.h"
#include "Pharos.h"

Texture::Texture(void)
{
	m_type = "Texture";

	m_renderImage = nullptr;
}

Texture::~Texture(void)
{
	SAFE_DELETE(m_renderImage);
}

bool Texture::LoadFromXml(XmlNode* xmlNode)
{
	if (!UIObject::LoadFromXml(xmlNode)) return false;

	string imageFile = GetAttributeStringValue(xmlNode, "file");
	m_imageColor = GetAttributeColorValue(xmlNode, "color");
	m_imageColor = COLOR_ARGB_TO_RGBA(m_imageColor);

	m_renderImage = sDesktopMgr->GenerateRenderImage(imageFile.c_str());

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

	if (m_renderImage != nullptr)
	{
		m_renderImage->RenderImageRect(m_imageRect, m_imageColor, m_rect);
	}
	else
	{
		sRenderSpirite->DrawRect(m_imageColor, m_rect);
	}
}
