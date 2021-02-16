#include "PreCompile.h"
#include "Pharos.h"

#define FONT_VERT_BUFFER_SIZE		(128 * 6)

RenderFont::RenderFont(void)
{
	m_font = nullptr;
	m_fontInfoIndex = 0;
	m_fontTex = nullptr;

	m_charColor = 0xFFFFFFFF;
	m_charOutlineScale = 0.0f;
    m_charWeight = -0.5f;
	m_charOutlineColor = 0xFF000000;
	m_charShadowColor = 0xFF000000;
	m_charShadowOffset = Size2Di(0, 0);

	m_vertBuffer.resize(FONT_VERT_BUFFER_SIZE);
}

RenderFont::~RenderFont(void)
{
	SAFE_DELETE(m_resSet);
	
	SAFE_DELETE(m_fontBuffer);
	
//	SAFE_DELETE(m_fontTech);
//	SAFE_DELETE(m_fontShadowTech);
}

bool RenderFont::LoadFont(const char8* fontFilePath)
{
	m_font = sResMgr->GenerateFont(fontFilePath);
	m_fontInfoIndex = sFontTexMgr->AttendFont(m_font);
	m_fontTex = sFontTexMgr->GetFontTexture(m_fontInfoIndex);

	m_fontBuffer = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);	
	m_fontBuffer->Allocate(sizeof(m_drawInfo));

	m_resSet = sRenderer->GenerateRenderResuourceSet();
	
	return true;
}

void RenderFont::DrawText(const char8* text, int32 textLen, int32 x, int32 y)
{
	uint32 vertNum = FillTextVertBuffer(text, textLen, x, y);
	
	float32 fontDisBase = sFontTexMgr->GetFontDisBase(m_fontInfoIndex);
	float32 fontDisScale = sFontTexMgr->GetFontDisScale(m_fontInfoIndex) + m_charWeight;
	Vector2Df shadowOffset = sRenderMgr->GetSizeFromWindowSize(m_charShadowOffset.width, m_charShadowOffset.height);

	Color4f charColor = m_charColor;
	Color4f outlineColor = m_charOutlineColor;

	m_drawInfo.fontDistance = Vector2Df(fontDisBase, fontDisScale);
	m_drawInfo.fontOffset = Vector2Df(0, 0);
	m_drawInfo.charColor = charColor;
	m_drawInfo.outlineColor = outlineColor;
	m_drawInfo.outlineScale = m_charOutlineScale;

	m_fontBuffer->CopyData(&m_drawInfo, sizeof(m_drawInfo));

	m_resSet->SetVertexUniformBuffer(1, m_fontBuffer);
	m_resSet->SetFragmentUniformBuffer(2, m_fontBuffer);
	m_resSet->SetFragmentTexture(0, m_fontTex);

	// if (COLOR_A(m_charShadowColor) != 0x00 && m_charShadowOffset.width != 0 && m_charShadowOffset.height != 0)
	// {
	// 	m_fontShadowDistanceVar->SetValue(Vector4Df(fontBaseValue, fontScaleValue, 0, 0));

	// 	sDesktopMgr->PushRenderPatch(&m_vertBuffer[0], vertNum, m_fontShadowTech);
	// }

	// m_fontDistanceVar->SetValue(Vector4Df(fontBaseValue, fontScaleValue, 0, 0));

	sRenderSpirite->DrawFont2D(&m_vertBuffer[0], vertNum, m_resSet);
}

uint32 RenderFont::FillTextVertBuffer(const char8* text, int32 textLen, int32 x, int32 y)
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

	return i;
}

void RenderFont::SetFontCharSize(int32 width, int32 height)
{
	m_charSize.width = width;
	m_charSize.height = height;
}

void RenderFont::SetFontCharColor(Color4 charColor)
{
	m_charColor = charColor;
}

void RenderFont::SetOutlineScale(float32 charOutlineScale)
{
	m_charOutlineScale = charOutlineScale;
}

void RenderFont::SetOutlineColor(Color4 charOuterColor)
{
	m_charOutlineColor = charOuterColor;
}

void RenderFont::SetShadowColor(Color4 charShadowColor)
{
	m_charShadowColor = charShadowColor;
}

void RenderFont::SetShadowOffset(int32 offsetX, int32 offsetY)
{
	m_charShadowOffset.width = offsetX;
	m_charShadowOffset.height = offsetY;
}
