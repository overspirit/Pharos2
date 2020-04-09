#include "PreCompile.h"
#include "Pharos.h"

#define HORI_CHAR_NUM 64	//横向有多少个字符
#define VERT_CHAR_NUM 64	//纵向有多少个字符

#define DEF_CHAR_MIN_VALUE -0.5f
#define DEF_CHAR_MAX_VALUE 0.1375f

FontTextureManager::FontTextureManager(void)
{

}

FontTextureManager::~FontTextureManager(void)
{

}

uint32 FontTextureManager::AttendFont(Font* font)
{
	uint32 fontInfoSize = (uint32)m_fontInfoList.size();
	m_fontInfoList.resize(fontInfoSize + 1);
		
	FontInfo& fontInfo = *m_fontInfoList.rbegin();
	fontInfo.font = font;

	ResumeFontInfo(fontInfo);

	return fontInfoSize;
}

void FontTextureManager::DestroyAll()
{
	for (uint32 i = 0; i < m_fontInfoList.size(); i++)
	{
		FontInfo& fontInfo = m_fontInfoList[i];
		//SAFE_DELETE(fontInfo.font);
		SAFE_DELETE(fontInfo.fontTex);
	}

	m_fontInfoList.clear();
}

void FontTextureManager::ResumeFontInfo(FontInfo& fontInfo)
{
	Font* font = fontInfo.font;
	uint32 disCharSize = font->GetDisCharSize();

	fontInfo.currMinValue = Math::minimum(DEF_CHAR_MIN_VALUE, font->GetDisMinValue());
	fontInfo.currMaxValue = Math::maximum(DEF_CHAR_MAX_VALUE, font->GetDisMaxValue());

	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	if (fontInfo.fontTex == nullptr)
	{		
		fontInfo.fontTex = renderer->CreateTexture(disCharSize * HORI_CHAR_NUM, disCharSize * VERT_CHAR_NUM, Render::EPF_A8_UNORM);
	}
	else
	{
		uint32 texWidth = fontInfo.fontTex->GetWidth();
		uint32 texHeight = fontInfo.fontTex->GetHeight();

		if (texWidth < disCharSize * HORI_CHAR_NUM || texHeight < disCharSize * VERT_CHAR_NUM)
		{
			SAFE_DELETE(fontInfo.fontTex);

			fontInfo.fontTex = renderer->CreateTexture(disCharSize * HORI_CHAR_NUM, disCharSize * VERT_CHAR_NUM, Render::EPF_A8_UNORM);
		}
	}

	fontInfo.currCharPosX = 0;
	fontInfo.currCharPosY = 0;

	fontInfo.charIndexList.resize(65535);
	memset(&fontInfo.charIndexList[0], 0xFF, sizeof(uint16) * fontInfo.charIndexList.size());

	fontInfo.charPosList.clear();
}

Vector4Df FontTextureManager::GetFontCharTexcoord(uint32 fontIndex, char16 ch)
{
	FontInfo& fontInfo = m_fontInfoList[fontIndex];

	Font* font = fontInfo.font;
	float32 fontMinValue = font->GetDisMinValue();
	float32 fontMaxValue = font->GetDisMaxValue();

	//如果纹理满了 或者 minValue 和 maxValue 不在原有的区间
	if ((fontInfo.currCharPosX == 0 && fontInfo.currCharPosY == 0) 
		|| (fontMinValue < fontInfo.currMinValue || fontMaxValue > fontInfo.currMaxValue))
	{
		ResumeFontInfo(fontInfo);
	}

	uint16 charIndex = fontInfo.charIndexList[ch];
	if (charIndex == 0xFFFF)
	{
		charIndex = InsertChar(fontInfo, ch);
	}

	CharPos& charPos = fontInfo.charPosList[charIndex];
	uint32 disCharSize = fontInfo.font->GetDisCharSize();
	uint32 texWidth = fontInfo.fontTex->GetWidth();
	uint32 texHeight = fontInfo.fontTex->GetHeight();

	Vector4Df charTexcoord;
	charTexcoord.x = (float32)charPos.x * disCharSize / texWidth;
	charTexcoord.y = (float32)charPos.y * disCharSize / texHeight;
	charTexcoord.z = (float32)(charPos.x + 1) * disCharSize / texWidth;
	charTexcoord.w = (float32)(charPos.y + 1) * disCharSize / texHeight;

	return charTexcoord;
}

float32 FontTextureManager::GetFontDisBase(uint32 fontIndex)
{
	FontInfo& fontInfo = m_fontInfoList[fontIndex];
	uint32 fontDisCharSize = fontInfo.font->GetDisCharSize();
	return fontInfo.currMinValue * fontDisCharSize + 1.0f;
}

float32 FontTextureManager::GetFontDisScale(uint32 fontIndex)
{
	FontInfo& fontInfo = m_fontInfoList[fontIndex];
	uint32 fontDisCharSize = fontInfo.font->GetDisCharSize();
	return (fontInfo.currMaxValue - fontInfo.currMinValue) * fontDisCharSize;
}

RenderTexture* FontTextureManager::GetFontTexture(uint32 fontIndex)
{
	return m_fontInfoList[fontIndex].fontTex;
}

//向字体纹理中添加一个字符信息
uint32 FontTextureManager::InsertChar(FontInfo& fontInfo, char16 ch)
{	
	//扩大FontInfo中的charInfoList，并存储该字符的索引
	vector<CharPos>& charPosList = fontInfo.charPosList;
	uint32 charPosListSize = (uint32)charPosList.size();
	fontInfo.charIndexList[ch] = charPosListSize;
	charPosList.resize(charPosListSize + 1);

	CharPos& charPos = *charPosList.rbegin();
	charPos.x = fontInfo.currCharPosX;
	charPos.y = fontInfo.currCharPosY;

	//////////////////////////////////////////////////////////////////////////
	float32* charDisData = fontInfo.font->GetDisCharData(ch);
	uint32 fontCharSize = fontInfo.font->GetDisCharSize();
	uint32 charDataSize = fontCharSize * fontCharSize;

	vector<uint8> charData;
	charData.resize(charDataSize);

	for (uint32 i = 0; i < charDataSize; i++)
	{
		charData[i] = (uint8)((charDisData[i] - fontInfo.currMinValue) / (fontInfo.currMaxValue - fontInfo.currMinValue) * 255.0f);
	}
	
	Rect2Di copyRect;
	copyRect.left = charPos.x * fontCharSize;
	copyRect.top = charPos.y * fontCharSize;
	copyRect.right = copyRect.left + fontCharSize;
	copyRect.bottom = copyRect.top + fontCharSize;
	fontInfo.fontTex->CopyRectFromData(&charData[0], charDataSize, copyRect);
	//////////////////////////////////////////////////////////////////////////

	//计算下一个字符要使用的位置
	//////////////////////////////////////////////////////////////////////////
	uint32 currCharSize = (uint32)fontInfo.charPosList.size();
	fontInfo.currCharPosX = currCharSize % HORI_CHAR_NUM;
	fontInfo.currCharPosY = currCharSize / HORI_CHAR_NUM % VERT_CHAR_NUM;
	//////////////////////////////////////////////////////////////////////////

	return charPosListSize;
}
