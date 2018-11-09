#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

#define FONT_VERT_BUFFER_SIZE		(128 * 6)

RenderFont::RenderFont(void)
{
	m_font = nullptr;
	m_fontInfoIndex = 0;
	m_fontTex = nullptr;

	m_charColor = 0xFFFFFFFF;
	m_charOutlineScale = 0.0f;
	m_charOutlineColor = 0xFF000000;
	m_charShadowColor = 0xFF000000;
	m_charShadowOffset = Size2Di(0, 0);

	m_fontTech = nullptr;
	m_fontTextureVar = nullptr;
	m_fontDistanceVar = nullptr;
	m_fontOffsetVar = nullptr;
	m_fontColorVar = nullptr;
	m_outlineScaleVar = nullptr;
	m_outlineColorVar = nullptr;

	m_fontShadowTech = nullptr;
	m_fontShadowTextureVar = nullptr;
	m_fontShadowDistanceVar = nullptr;
	m_fontShadowOffsetVar = nullptr;
	m_fontShadowColorVar = nullptr;
	m_shadowOutlineScaleVar = nullptr;
	m_shadowOutlineColorVar = nullptr;

	m_vertBuffer.resize(FONT_VERT_BUFFER_SIZE);
}

RenderFont::~RenderFont(void)
{
	SAFE_DELETE(m_fontTech);
	SAFE_DELETE(m_fontShadowTech);
}

bool RenderFont::LoadFont(const char8* fontFilePath)
{
	m_font = sResMgr->GenerateFont(fontFilePath);
	m_fontInfoIndex = sFontTexMgr->AttendFont(m_font);
	m_fontTex = sFontTexMgr->GetFontTexture(m_fontInfoIndex);
	
	m_fontTech = sRenderMgr->GenerateRenderTechnique("Font");
	m_fontTextureVar = m_fontTech->GetVariable("g_tex");
	m_fontDistanceVar = m_fontTech->GetVariable("g_font_distance");
	m_fontOffsetVar = m_fontTech->GetVariable("g_font_offset");
	m_fontColorVar = m_fontTech->GetVariable("g_char_color");
	m_outlineScaleVar = m_fontTech->GetVariable("g_outline_scale");
	m_outlineColorVar = m_fontTech->GetVariable("g_outline_color");
	
	m_fontShadowTech = sRenderMgr->GenerateRenderTechnique("Font");
	m_fontShadowTextureVar = m_fontShadowTech->GetVariable("g_tex");
	m_fontShadowDistanceVar = m_fontShadowTech->GetVariable("g_font_distance");
	m_fontShadowOffsetVar = m_fontShadowTech->GetVariable("g_font_offset");
	m_fontShadowColorVar = m_fontShadowTech->GetVariable("g_char_color");
	m_shadowOutlineScaleVar = m_fontShadowTech->GetVariable("g_outline_scale");
	m_shadowOutlineColorVar = m_fontShadowTech->GetVariable("g_outline_color");

	Color4f charColor = m_charColor;
	Color4f outlineColor = m_charOutlineColor;
	Color4f shadowColor = m_charShadowColor;
	float32 fontDisBase = sFontTexMgr->GetFontDisBase(m_fontInfoIndex);
	float32 fontDisScale = sFontTexMgr->GetFontDisScale(m_fontInfoIndex);
	Vector2Df shadowOffset = sRenderMgr->GetSizeFromWindowSize(m_charShadowOffset.width, m_charShadowOffset.height);

	m_fontTextureVar->SetValue(m_fontTex);
	m_fontDistanceVar->SetValue(Vector4Df(fontDisBase, fontDisScale, 0, 0));
	m_fontOffsetVar->SetValue(Vector4Df(0, 0, 0, 0));
	m_fontColorVar->SetValue(charColor);
	m_outlineScaleVar->SetValue(Vector4Df(m_charOutlineScale, 0, 0, 0));	
	m_outlineColorVar->SetValue(outlineColor);

	m_fontShadowTextureVar->SetValue(m_fontTex);
	m_fontShadowDistanceVar->SetValue(Vector4Df(fontDisBase, fontDisScale, 0, 0));
	m_fontShadowOffsetVar->SetValue(Vector4Df(shadowOffset.x, shadowOffset.y, 0, 0));	
	m_fontShadowColorVar->SetValue(shadowColor);
	m_shadowOutlineScaleVar->SetValue(Vector4Df(0, 0, 0, 0));
	m_shadowOutlineColorVar->SetValue(shadowColor);
	
	return true;
}

void RenderFont::RenderText(const char8* text, int32 textLen, int32 x, int32 y)
{
	FillTextVertBuffer(text, textLen, x, y);

	float32 fontBaseValue = sFontTexMgr->GetFontDisBase(m_fontInfoIndex);
	float32 fontScaleValue = sFontTexMgr->GetFontDisScale(m_fontInfoIndex);

	if (COLOR_A(m_charShadowColor) != 0x00 && m_charShadowOffset.width != 0 && m_charShadowOffset.height != 0)
	{
		m_fontShadowDistanceVar->SetValue(Vector4Df(fontBaseValue, fontScaleValue, 0, 0));

		sDesktopMgr->PushRenderPatch(&m_vertBuffer[0], (uint32)m_vertBuffer.size(), m_fontShadowTech);
	}

	m_fontDistanceVar->SetValue(Vector4Df(fontBaseValue, fontScaleValue, 0, 0));

	sDesktopMgr->PushRenderPatch(&m_vertBuffer[0], (uint32)m_vertBuffer.size(), m_fontTech);
}

void RenderFont::FillTextVertBuffer(const char8* text, int32 textLen, int32 x, int32 y)
{
	const char8* temp = text;

	int32 offsetX = x;
	int32 offsetY = y;

	uint32 disCharSize = m_font->GetDisCharSize();
	float32 charWidthScale = (float32)m_charSize.width / disCharSize;
	float32 charHeightScale = (float32)m_charSize.height / disCharSize;

	uint32 i = 0;
	while (*temp != '\0' && textLen != 0)
	{
		//utf8编码转成unicode编码
		char16 ch = Utils::utf8_to_unicode(temp, temp);

		const tagCharInfo& charInfo = m_font->GetDisCharInfo(ch);

		//计算在屏幕的位置
		int32 left = offsetX + (int32)(charInfo.off_x * charWidthScale);
		int32 top = offsetY - (int32)(charInfo.off_y * charHeightScale);
		int32 right = left + m_charSize.width;
		int32 bottom = top + m_charSize.height;

		offsetX += (int32)(charInfo.adv_x * charWidthScale);
		offsetY += (int32)(charInfo.adv_y * charHeightScale);

 		Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(left, top);
 		Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(right, bottom);
 
		Vector4Df charTexcoord = sFontTexMgr->GetFontCharTexcoord(m_fontInfoIndex, ch);

		//如果VertBuffer容量小于绘制字符的容量，要将容量扩大
		if (m_vertBuffer.size() <= i + 6)
		{
			m_vertBuffer.resize(i + 6);
		}

		//建立顶点数据
		m_vertBuffer[i + 0] = { Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexcoord.x, charTexcoord.y) };
		m_vertBuffer[i + 1] = { Vector3Df(rightBottom.x,	leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexcoord.z, charTexcoord.y) };
		m_vertBuffer[i + 2] = { Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexcoord.z, charTexcoord.w) };
		m_vertBuffer[i + 3] = { Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexcoord.z, charTexcoord.w) };
		m_vertBuffer[i + 4] = { Vector3Df(leftTop.x,		rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexcoord.x, charTexcoord.w) };
		m_vertBuffer[i + 5] = { Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexcoord.x, charTexcoord.y) };

		i += 6;
	}
}

void RenderFont::SetFontCharSize(int32 width, int32 height)
{
	m_charSize.width = width;
	m_charSize.height = height;
}

void RenderFont::SetFontCharColor(Color4 charColor)
{
	m_charColor = charColor;

	m_fontColorVar->SetValue(Color4f(m_charColor));
}

void RenderFont::SetOutlineScale(float32 charOutlineScale)
{
	m_charOutlineScale = charOutlineScale;

	m_outlineScaleVar->SetValue(Vector4Df(m_charOutlineScale, 0, 0, 0));
}

void RenderFont::SetOutlineColor(Color4 charOuterColor)
{
	m_charOutlineColor = charOuterColor;

	m_outlineColorVar->SetValue(Color4f(m_charOutlineColor));
}

void RenderFont::SetShadowColor(Color4 charShadowColor)
{
	m_charShadowColor = charShadowColor;

	m_fontShadowColorVar->SetValue(Color4f(m_charShadowColor));
	m_shadowOutlineColorVar->SetValue(Color4f(m_charShadowColor));
}

void RenderFont::SetShadowOffset(int32 offsetX, int32 offsetY)
{
	m_charShadowOffset.width = offsetX;
	m_charShadowOffset.height = offsetY;

	Vector2Df shadowOffset = sRenderMgr->GetSizeFromWindowSize(m_charShadowOffset.width, m_charShadowOffset.height);
	m_fontShadowOffsetVar->SetValue(Vector4Df(shadowOffset.x, shadowOffset.y, 0, 0));
}