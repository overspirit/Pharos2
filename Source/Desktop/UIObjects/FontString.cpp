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
	int32 fontWeight = GetAttributeIntValue(xmlNode, "font_weight");
    Color4 fontColor = GetAttributeColorValue(xmlNode, "font_color");
	m_showText = GetAttributeStringValue(xmlNode, "text");

	m_renderFont = sDesktopMgr->GenerateRenderFont(fontFile);
    if(m_renderFont == nullptr) return false;    
	m_renderFont->SetFontCharSize(fontWidth, fontHeight);
	m_renderFont->SetFontCharColor(fontColor);
	m_renderFont->SetFontWeight(fontWeight);

	return true;
}

void FontString::SetText(float32 value)
{
    char buf[32];
    sprintf(buf, "%.2f", value);
    m_showText = buf;
}

void FontString::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
    
	m_renderFont->DrawText(m_showText.c_str(), -1, m_rect.left, m_rect.top);
}
