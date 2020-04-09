#include "PreCompile.h"
#include "Pharos.h"

FontString::FontString(void)
{
	m_type = "FontString";

	m_renderFont = nullptr;
}

FontString::~FontString(void)
{
	SAFE_DELETE(m_renderFont);
}

bool FontString::LoadFromXml(XmlNode* xmlNode)
{
	if (!UIObject::LoadFromXml(xmlNode)) return false;
	
	const char8* fontFile = GetAttributeStringValue(xmlNode, "font_file");
	int32 fontWidth = GetAttributeIntValue(xmlNode, "font_width");
	int32 fontHeight = GetAttributeIntValue(xmlNode, "font_height");
	m_showText = GetAttributeStringValue(xmlNode, "text");

	m_renderFont = sDesktopMgr->GenerateRenderFont(fontFile);
	m_renderFont->SetFontCharSize(fontWidth, fontHeight);

	return true;
}

void FontString::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
}

void FontString::Render(float32 fElapsed)
{
	m_renderFont->RenderText(m_showText.c_str(), -1, m_rect.left, m_rect.top);
}
