#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

FontString::FontString(void)
{
	m_type = "FontString";
}

FontString::~FontString(void)
{

}

bool FontString::LoadFromXml(XmlNode* xmlNode)
{
	//m_pParent = owner;

// 	m_strName = varMgr.GetStringValue("name");
// 	const char8* inherit = varMgr.GetStringValue("inherit");
// 
// 	m_showText = varMgr.GetStringValue("text");
// 	m_texAlign = GetTextAlignType(varMgr.GetStringValue("text_align"));
// 	//m_fontColor = varMgr.GetColorValue("font_color");
// 
// 	const char8* fontFace = varMgr.GetStringValue("face_file");
// 	int32 fontWidth = varMgr.GetIntValue("font_width");
// 	int32 fontHeight = varMgr.GetIntValue("font_height");
// 	int32 fontWeight = varMgr.GetIntValue("font_weight");
// 	int32 fontPitch = varMgr.GetIntValue("font_pitch");
// 	
// 	//m_fontHandle = Sprite2D::Inst()->CreateFontHandle(fontFace, fontWidth, fontHeight, fontWeight);
// 	
// 	varMgr.CallFuncWithName("Region", this, (UI_LOAD_FUNC)&FontString::LoadRegionData, nullptr);

	return false;
}

void FontString::Update(float32 fElapsed)
{
	UIObject::Update(fElapsed);
}

void FontString::Render(float32 fElapsed)
{
	//Sprite2D::Inst()->DrawText(m_fontHandle, m_showText.c_str(), -1, m_rect.left, m_rect.top, m_fontBrush);
}