#include "PreCompile.h"
#include "Pharos.h"

Texture::Texture(void)
{
	m_type = "Texture";

	m_renderImage = nullptr;
	m_round = 0;

	//兜底。。没有纹理就按照1像素纹理处理
	m_imageRect.left = 0;
	m_imageRect.top = 0;
	m_imageRect.right = 1;
	m_imageRect.bottom = 1;
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
	m_round = GetAttributeFloatValue(xmlNode, "round");

	m_renderImage = sDesktopMgr->GenerateRenderImage();
	m_renderImage->SetImageColor(m_imageColor);

	if (!imageFile.empty())
	{
		m_renderImage->LoadImage(imageFile.c_str());

		m_imageRect.left = GetAttributeIntValue(xmlNode, "left");
		m_imageRect.top = GetAttributeIntValue(xmlNode, "top");
		m_imageRect.right = GetAttributeIntValue(xmlNode, "right");
		m_imageRect.bottom = GetAttributeIntValue(xmlNode, "bottom");
	}

	return true;
}

void Texture::DrawRect(const Rect2Di& rect)
{
    m_rect = rect;
    
    m_renderImage->RenderRect(m_imageRect, m_rect, m_round);
}

void Texture::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
    
	m_renderImage->RenderRect(m_imageRect, m_rect, m_round);
}
