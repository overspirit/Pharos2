#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

#define FONT_CHAR_SIZE 32		//字体纹理中每个字符的大小
#define TEX_FONT_NUM 64		//字体纹理中每行的字符数
#define VERTEX_DATA_BUFFER_SIZE (64 * 6 * sizeof(Vertex))
#define VERTEX_LAYOUT_BUFFER_SIZE (1000 * 6 * sizeof(Vertex))

//目前需要改进的地方:
//1,font中获得base,scal,dis_size等数据我都认为所有font是一样的
//2,字体有锯齿，汉字过于贴近边缘造成描边没有位置，可能需要改进dis_char算法
//3,没有扩大layout中的顶点数据功能，如果超过layout中的顶点数据范围会奔溃

RenderSprite::RenderSprite(void)
{
	m_layoutStart = 0;

	m_renderColorTech = nullptr;

	m_renderBlock = nullptr;
}

RenderSprite::~RenderSprite(void)
{

}

bool RenderSprite::Init(Renderer* renderer)
{
	m_renderer = renderer;
	
	m_layout = renderer->GenerateRenderLayout(VERTEX_LAYOUT_BUFFER_SIZE);

	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, "POSITION", 0, 0 },
		{ VET_UNORM8, 4, "COLOR", 0, 12 },
		{ VET_FLOAT32, 2, "TEXCOORD", 0, 16 },
	};
	
	m_layout->SetInputLayoutDesc(desc, 3);
	
	//AddSumitInfo();

	memset(m_submitBlockIndex, 0xFF, sizeof(m_submitBlockIndex));

	m_imageVertDataBuf.ChangeSize(VERTEX_DATA_BUFFER_SIZE);

	m_renderColorTech = sRenderMgr->GenerateRenderTechnique("Sprite2DColor");

	m_renderBlock = sRenderMgr->GenerateRenderBlock();

	return true;
}

void RenderSprite::Destroy()
{
	SAFE_DELETE(m_renderBlock);

	SAFE_DELETE(m_layout);
	SAFE_DELETE(m_renderColorTech);
}

void RenderSprite::AddSumitInfo()
{
	uint32 infoSize = (uint32)m_submitInfoList.size();
	m_submitInfoList.resize(infoSize + 1);

	SubmitInfo& submitInfo = m_submitInfoList[infoSize];

	uint32 texSize = FONT_CHAR_SIZE * TEX_FONT_NUM;
	RenderTexture* fontTex = m_renderer->CreateTexture(texSize, texSize, EPF_A8_UNORM);

	//!!!注意。由于都使用一个Effect文件，所以不管使用还是不使用，都要给所有变量赋值....
	RenderTechnique* fontTech = sRenderMgr->GenerateRenderTechnique("Font");
	RenderVariable* fontTechTexVar = fontTech->GetVariable("g_tex");
	RenderVariable* fontTechBaseVar = fontTech->GetVariable("g_base");
	RenderVariable* fontTechScaleVar = fontTech->GetVariable("g_scale");
	RenderVariable* fontTechOutlineScaleVar = fontTech->GetVariable("g_outline_scale");
	RenderVariable* fontTechColorVar = fontTech->GetVariable("g_font_color");
	RenderVariable* fontTechOutlineColorVar = fontTech->GetVariable("g_outline_color");
	RenderVariable* fontTechShadowOffsetVar = fontTech->GetVariable("g_shadow_offset");

	fontTechTexVar->SetValue(fontTex);
	fontTechOutlineScaleVar->SetValue(0.0f);
	fontTechOutlineColorVar->SetValue(Color4f(0, 0, 0, 1.0f));
	fontTechShadowOffsetVar->SetValue(Vector2Df(0, 0));

	RenderTechnique* fontShadowTech = sRenderMgr->GenerateRenderTechnique("FontShadow");
	RenderVariable* fontShadowTechTexVar = fontShadowTech->GetVariable("g_tex");
	RenderVariable* fontShadowTechBaseVar = fontShadowTech->GetVariable("g_base");
	RenderVariable* fontShadowTechScaleVar = fontShadowTech->GetVariable("g_scale");
	RenderVariable* fontShadowTechOutlineScaleVar = fontShadowTech->GetVariable("g_outline_scale");
	RenderVariable* fontShadowTechColorVar = fontShadowTech->GetVariable("g_font_color");
	RenderVariable* fontShadowTechOutlineColorVar = fontShadowTech->GetVariable("g_outline_color");
	RenderVariable* fontShadowTechShadowOffsetVar = fontShadowTech->GetVariable("g_shadow_offset");

	fontShadowTechTexVar->SetValue(fontTex);
	fontShadowTechOutlineScaleVar->SetValue(0.0f);
	fontShadowTechOutlineColorVar->SetValue(Color4f(0, 0, 0, 1.0f));

	RenderTechnique* fontOutlineTech = sRenderMgr->GenerateRenderTechnique("FontOutline");
	RenderVariable* fontOutlineTechTexVar = fontOutlineTech->GetVariable("g_tex");
	RenderVariable* fontOutlineTechBaseVar = fontOutlineTech->GetVariable("g_base");
	RenderVariable* fontOutlineTechScaleVar = fontOutlineTech->GetVariable("g_scale");
	RenderVariable* fontOutlineTechOutlineScaleVar = fontOutlineTech->GetVariable("g_outline_scale");
	RenderVariable* fontOutlineTechColorVar = fontOutlineTech->GetVariable("g_font_color");
	RenderVariable* fontOutlineTechOutlineColorVar = fontOutlineTech->GetVariable("g_outline_color");
	RenderVariable* fontOutlineTechShadowOffsetVar = fontOutlineTech->GetVariable("g_shadow_offset");

	fontOutlineTechTexVar->SetValue(fontTex);
	fontOutlineTechShadowOffsetVar->SetValue(Vector2Df(0, 0));

	MemoryBuffer vertData(VERTEX_DATA_BUFFER_SIZE);

	submitInfo.vertData = vertData;
	submitInfo.dataVertNum = 0;

	submitInfo.fontTex = fontTex;

	submitInfo.fontTech = fontTech;
	submitInfo.fontTechBaseVar = fontTechBaseVar;
	submitInfo.fontTechScaleVar = fontTechScaleVar;
	submitInfo.fontTechColorVar = fontTechColorVar;

	submitInfo.fontShadowTech = fontShadowTech;
	submitInfo.fontShadowTechBaseVar = fontShadowTechBaseVar;
	submitInfo.fontShadowTechScaleVar = fontShadowTechScaleVar;
	submitInfo.fontShadowTechColorVar = fontShadowTechColorVar;
	submitInfo.fontShadowTechShadowOffsetVar = fontShadowTechShadowOffsetVar;

	submitInfo.fontOutlineTech = fontOutlineTech;
	submitInfo.fontOutlineTechBaseVar = fontOutlineTechBaseVar;
	submitInfo.fontOutlineTechScaleVar = fontOutlineTechScaleVar;
	submitInfo.fontOutlineTechOutlineScaleVar = fontOutlineTechOutlineScaleVar;
	submitInfo.fontOutlineTechColorVar = fontOutlineTechColorVar;
	submitInfo.fontOutlineTechOutlineColorVar = fontOutlineTechOutlineColorVar;

	//submitInfo.fontTechBlock = m_renderer->GenerateRenderBlock(m_layout, fontTech);
	//submitInfo.fontOutlineTechBlock = m_renderer->GenerateRenderBlock(m_layout, fontOutlineTech);
	//submitInfo.fontShadowTechBlock = m_renderer->GenerateRenderBlock(m_layout, fontShadowTech);

	m_currFontTexPos.x = 0;
	m_currFontTexPos.y = 0;
}

uint32 RenderSprite::GetFontInfoIndex(Font* font)
{
	uint32 index = 0xFFFFFFFF;

	auto iter = m_fontInfoIndexMap.find(font);
	if (iter == m_fontInfoIndexMap.end())
	{
		uint32 listSize = (uint32)m_fontInfoList.size();
		m_fontInfoList.resize(listSize + 1);

		index = listSize;

		m_fontInfoIndexMap[font] = index;
		m_fontInfoList[index].font = font;
	}
	else
	{
		index = iter->second;
	}

	return index;
}

//绘制函数...
void RenderSprite::RenderText(uint32 fontIndex, const char8* text, int32 textLen, const SpriteBrush& brush, int32 x, int32 y)
{
	//根据fontIndex寻找FontInfo
	FontInfo& fontInfo = m_fontInfoList[fontIndex];
	Font* font = fontInfo.font;
	float32 scale = brush.scale;

	const char8* temp = text;

	int32 offsetX = x;
	int32 offsetY = y;

	while (*temp != '\0' && textLen != 0)
	{
		//utf8编码转成unicode编码
		char16 ch = Utils::utf8_to_unicode(temp, temp);

		const tagCharInfo& charInfo = font->GetDisCharInfo(ch);

		//寻找该字符的unicode编码寻找是否存在该字符的信息信息
		uint16 charInfoIndex = fontInfo.charIndex[ch];
		if (charInfoIndex == 0xFFFF)//等于0xFFFF就是不存在该字符的信息。就向纹理中插入该字符信息
		{
			InsertChar(fontInfo, ch, charInfo.width, charInfo.height);
			charInfoIndex = fontInfo.charIndex[ch];
		}

		//获得该字符的信息
		const CharInfo& charTexInfo = fontInfo.charInfoList[charInfoIndex];
		
		//计算在屏幕的位置
		int32 left = offsetX + (int32)(charInfo.left * scale);
		int32 top = offsetY + (int32)(charInfo.top * scale);
		int32 right = left + (int32)(FONT_CHAR_SIZE * scale);
		int32 bottom = top + (int32)(FONT_CHAR_SIZE * scale);

		Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(left, top);
		Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(right, bottom);

		//建立顶点数据
		Vertex vt[] =
		{
			{ Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexInfo.left, charTexInfo.top) },
			{ Vector3Df(rightBottom.x,	leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexInfo.right, charTexInfo.top) },
			{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexInfo.right, charTexInfo.bottom) },
			{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexInfo.right, charTexInfo.bottom) },
			{ Vector3Df(leftTop.x,		rightBottom.y,	0),		0xFFFFFFFF,		Vector2Df(charTexInfo.left, charTexInfo.bottom) },
			{ Vector3Df(leftTop.x,		leftTop.y,		0),		0xFFFFFFFF,		Vector2Df(charTexInfo.left, charTexInfo.top) },
		};

		//寻找是否有使用该SubmitInfo的SubmitBlock
		uint16 infoIndex = m_submitBlockIndex[charTexInfo.index];
		if (infoIndex == 0xFFFF)	//不存在就建立一个SubmitBlock
		{
			SubmitInfo& submitInfo = m_submitInfoList[charTexInfo.index];
			float32 fontDisBase = font->GetDisBase();
			float32 fontDisScale = font->GetDisScale();

			//IRenderBlockPtr fontTechBlock;
			//IRenderBlockPtr fontShadowTechBlock;

            RenderTechnique* fontTech;
            RenderTechnique* fontShadowTech;
            
			//如果没有描边，也就是outlineScale等于0的时候，选择使用普通字体的shader
			//否则就是用带有描边的shader
			if (brush.outlineScale > 0.0f)
			{
				submitInfo.fontOutlineTechBaseVar->SetValue(fontDisBase);
				submitInfo.fontOutlineTechScaleVar->SetValue(fontDisScale);				
				submitInfo.fontOutlineTechColorVar->SetValue(Color4f(brush.color));
				submitInfo.fontOutlineTechOutlineScaleVar->SetValue(brush.outlineScale);
				submitInfo.fontOutlineTechOutlineColorVar->SetValue(Color4f(brush.outlineColor));

				//fontTechBlock = submitInfo.fontOutlineTechBlock;
                fontTech = submitInfo.fontOutlineTech;
			}
			else
			{
				submitInfo.fontTechBaseVar->SetValue(fontDisBase);
				submitInfo.fontTechScaleVar->SetValue(fontDisScale);
				submitInfo.fontTechColorVar->SetValue(Color4f(brush.color));

				//fontTechBlock = submitInfo.fontTechBlock;
                fontTech = submitInfo.fontTech;
			}
			
			//根据是否有阴影偏移，选择是否使用阴影的shader
			if (brush.shadowOffsetX != 0 || brush.shadowOffsetY != 0)
			{
				Vector2Df textPos = sRenderMgr->GetPosFromWindowPos(x, y);
				Vector2Df shadowPos = sRenderMgr->GetPosFromWindowPos(x + brush.shadowOffsetX, y + brush.shadowOffsetY);

				submitInfo.fontShadowTechBaseVar->SetValue(fontDisBase);
				submitInfo.fontShadowTechScaleVar->SetValue(fontDisScale);
				submitInfo.fontShadowTechColorVar->SetValue(Color4f(brush.shadowColor));
				submitInfo.fontShadowTechShadowOffsetVar->SetValue(shadowPos - textPos);

				//fontShadowTechBlock = submitInfo.fontShadowTechBlock;
                fontShadowTech = submitInfo.fontShadowTech;
			}

			uint32 submitBlockSize = (uint32)m_submitBlockList.size();
			m_submitBlockList.resize(submitBlockSize + 1);
			m_submitBlockList[submitBlockSize].vertData = submitInfo.vertData;
			m_submitBlockList[submitBlockSize].dataVertNum = 0;
			//m_submitBlockList[submitBlockSize].fontTechBlock = fontTechBlock;
			//m_submitBlockList[submitBlockSize].fontShadowTechBlock = fontShadowTechBlock;
            m_submitBlockList[submitBlockSize].fontTech = fontTech;
            m_submitBlockList[submitBlockSize].fontShadowTech = fontShadowTech;

			m_submitBlockIndex[charTexInfo.index] = submitBlockSize;

			infoIndex = submitBlockSize;
		}

		//获得使用的SubmitBlock
		SubmitBlock& submitBlock = m_submitBlockList[infoIndex];
		//检查一下是否超过了分配的顶点数据大小范围，超过了就扩大分配范围....
		if ((submitBlock.dataVertNum + 6) * sizeof(Vertex) >= submitBlock.vertData.GetLength())
		{
			submitBlock.vertData.ChangeSize(Math::ceil_power_2((int32)((submitBlock.dataVertNum + 6) * sizeof(Vertex))));
		}

		//向vertData中拷贝数据...
		submitBlock.vertData.Insert(submitBlock.dataVertNum * sizeof(Vertex), vt, sizeof(vt));
		submitBlock.dataVertNum += 6;

		//计算下一个字符的位置
		offsetX += (int32)(charInfo.adv_x * scale);
		offsetY += (int32)(charInfo.adv_y * scale);

		textLen--;
	}
}

void RenderSprite::RenderRect(RenderTechnique* renderTech, const Vector4Df& uv, Color4 color, const Rect2Di& destRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(destRect.left, destRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(destRect.right, destRect.bottom);

	float32 texLeft = uv.x;
	float32 texTop = uv.y;
	float32 texRight = uv.z;
	float32 texBottom = uv.w;

	//建立顶点数据
	Vertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(texLeft, texTop) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(texRight, texTop) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(texRight, texBottom) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(texLeft, texBottom) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(texLeft, texTop) },
	};

	m_imageVertDataBuf.CopyFrom(vt, sizeof(vt));

	m_layout->CopyVertexBuffer(&m_imageVertDataBuf, sizeof(vt), m_layoutStart * sizeof(Vertex));

	//递交绘制信息
	//IRenderBlockPtr fontBlock = submitInfo.fontTechBlock;
	//IRenderBlockPtr imageRenderBlock = sRenderMgr->GenerateRenderBlock(m_layout, renderTech);
	//imageRenderBlock->SetDrawType(EDT_TRIANGLELIST);
	//imageRenderBlock->SetLayoutRange(m_layoutStart, 6);

	//sRenderMgr->DoRender(imageRenderBlock);

	uint32 patchIndex = m_renderBlock->AddRenderBlockPatch(m_layout, renderTech);
	m_renderBlock->SetBlockPatchDrawType(patchIndex, EDT_TRIANGLELIST);
	m_renderBlock->SetBlockPatchDrawRange(patchIndex, m_layoutStart, 6);

	m_layoutStart += 6;
}

void RenderSprite::RenderBox(const Rect2Di& box, Color4 color)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(box.left, box.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(box.right, box.bottom);

	//建立顶点数据
	Vertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },

		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },

		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },

		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
	};

	m_imageVertDataBuf.CopyFrom(vt, sizeof(vt));

	m_layout->CopyVertexBuffer(&m_imageVertDataBuf, sizeof(vt), m_layoutStart * sizeof(Vertex));

	//递交绘制信息
	//IRenderBlockPtr fontBlock = submitInfo.fontTechBlock;
	//IRenderBlockPtr imageRenderBlock = sRenderMgr->GenerateRenderBlock(m_layout, m_renderColorTech);
	//imageRenderBlock->SetDrawType(EDT_LINELIST);
	//imageRenderBlock->SetLayoutRange(m_layoutStart, 8);

	//sRenderMgr->DoRender(imageRenderBlock);

	m_layoutStart += 8;
}

//向字体纹理中添加一个字符信息
void RenderSprite::InsertChar(FontInfo& fontInfo, char16 ch, uint16 charWidth, uint16 charHeight)
{
	//扩大FontInfo中的charInfoList，并存储该字符的索引
	vector<CharInfo>& charInfoList = fontInfo.charInfoList;
	uint32 charInfoListSize = (uint32)charInfoList.size();
	charInfoList.resize(charInfoListSize + 1);
	fontInfo.charIndex[ch] = charInfoListSize;

	CharInfo& charInfo = charInfoList[charInfoListSize];

	//m_submitInfoList中最后一个纹理信息肯定是当前使用的纹理信息
	uint32 currSubmitIndex = (uint32)(m_submitInfoList.size() - 1);

	//计算该字符在纹理的位置
	RenderTexture* fontTex = m_submitInfoList[currSubmitIndex].fontTex;
	uint32 fontTexWidth = fontTex->GetWidth();
	uint32 fontTexHeight = fontTex->GetHeight();

	charInfo.ch = ch;
	charInfo.index = currSubmitIndex;
	charInfo.left = (float32)m_currFontTexPos.x / fontTexWidth;
	charInfo.top = (float32)m_currFontTexPos.y / fontTexHeight;
	charInfo.right = (float32)(m_currFontTexPos.x + FONT_CHAR_SIZE) / fontTexWidth;
	charInfo.bottom = (float32)(m_currFontTexPos.y + FONT_CHAR_SIZE) / fontTexHeight;

	//////////////////////////////////////////////////////////////////////////
	uint8* chData = fontInfo.font->GetDisCharData(ch);
	uint32 fontCharSize = fontInfo.font->GetDisCharSize();
	uint32 chDataSize = fontCharSize * fontCharSize;
    
	//向纹理中拷贝数据，FONT_CHAR_SIZE目前是定值....
    if(chData != nullptr)
    {
        Rect2Di copyRect;
        copyRect.left = m_currFontTexPos.x;
        copyRect.top = m_currFontTexPos.y;
        copyRect.right = m_currFontTexPos.x + FONT_CHAR_SIZE;
        copyRect.bottom = m_currFontTexPos.y + FONT_CHAR_SIZE;
        fontTex->CopyRectFromData(chData, chDataSize, copyRect);
    }
	//////////////////////////////////////////////////////////////////////////

	//计算下一个字符要使用的位置
	//////////////////////////////////////////////////////////////////////////
	uint32 nextPosX = (m_currFontTexPos.x + FONT_CHAR_SIZE) % fontTexWidth;
	uint32 nextPosY = (m_currFontTexPos.y + FONT_CHAR_SIZE) % fontTexHeight;
	
	m_currFontTexPos.x = nextPosX;
	m_currFontTexPos.y = (nextPosX == 0) ? nextPosY : m_currFontTexPos.y;//如果X为0了，说明要新建一行，也就是nextPosY，否则还保持原值

	//如果x和y同时为0，说明要新建一张纹理，
	if (m_currFontTexPos.x == 0 && m_currFontTexPos.y == 0)
	{
		AddSumitInfo();
	}
	//////////////////////////////////////////////////////////////////////////
}

void RenderSprite::PostSubmitBlock()
{
	//递交绘制信息
	for (auto& submitInfo : m_submitBlockList)
	{
		m_layout->CopyVertexBuffer(&submitInfo.vertData, submitInfo.dataVertNum * sizeof(Vertex), m_layoutStart * sizeof(Vertex));
		
		//递交绘制信息
		//IRenderBlockPtr fontBlock = submitInfo.fontTechBlock;
//         IRenderBlockPtr fontBlock = sRenderMgr->GenerateRenderBlock(m_layout, submitInfo.fontTech);
// 		fontBlock->SetDrawType(EDT_TRIANGLELIST);
// 		fontBlock->SetLayoutRange(m_layoutStart, submitInfo.dataVertNum);
// 
// 		sRenderMgr->DoRender(fontBlock);

        //阴影的绘制使用相同的layout,但是shader不同
        //if (submitInfo.fontShadowTechBlock != nullptr)
        if(submitInfo.fontShadowTech != nullptr)
        {
            //IRenderBlockPtr fontShadowBlock = submitInfo.fontShadowTechBlock;
//             IRenderBlockPtr fontShadowBlock = sRenderMgr->GenerateRenderBlock(m_layout, submitInfo.fontShadowTech);
//             fontShadowBlock->SetDrawType(EDT_TRIANGLELIST);
//             fontShadowBlock->SetLayoutRange(m_layoutStart, submitInfo.dataVertNum);
//             
// 			sRenderMgr->DoRender(fontShadowBlock);
        }
        
		//使用的layout数量要加上拷贝的数量
		m_layoutStart += submitInfo.dataVertNum;

		submitInfo.dataVertNum = 0;
	}

	//submitBlock信息要清空
	memset(m_submitBlockIndex, 0xFF, sizeof(m_submitBlockIndex));

	m_submitBlockList.clear();

	sRenderMgr->DoRender(m_renderBlock);
}

void RenderSprite::Resume()
{
	m_layoutStart = 0;
}
