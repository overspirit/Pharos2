#include "PreCompile.h"
#include "Pharos.h"

#define MAX_VERT_SIZE	(2048 * 6 * sizeof(DecalColorVertex))

RenderSprite::RenderSprite(void)
{
//	m_renderTech = nullptr;
}

RenderSprite::~RenderSprite(void)
{
//	SAFE_DELETE(m_renderTech);
}

bool RenderSprite::Init()
{
	m_drawColorObj = sRenderMgr->GenerateRenderObject();
	m_drawTextureObj = sRenderMgr->GenerateRenderObject();
	m_drawFontObj = sRenderMgr->GenerateRenderObject();;

	m_vertBuf = sRenderer->GenerateRenderBuffer(VERTEX_BUFFER);
	m_vertBuf->Allocate(MAX_VERT_SIZE);
	
	m_vertMemBuf.Alloc(MAX_VERT_SIZE);

	VertLayoutDesc desc[] =
	{
		{ VET_FLOAT32, 3, VAL_POSITION, 0, 0 },
		{ VET_UNORM8, 4, VAL_COLOR, 12, 0 },
		{ VET_FLOAT32, 2, VAL_TEXCOORD0, 16, 0 },
	};


	BlendStateDesc blendDesc;
	blendDesc.blendEnable = true;
	blendDesc.alphaToCoverageEnable = false;
	blendDesc.srcBlend = BLEND_SRC_ALPHA;
	blendDesc.destBlend = BLEND_INV_SRC_ALPHA;
	blendDesc.blendOp = BLEND_OP_ADD;
	blendDesc.srcBlendAlpha = BLEND_SRC_ALPHA;
	blendDesc.destBlendAlpha = BLEND_INV_SRC_ALPHA;
	blendDesc.blendOpAlpha = BLEND_OP_ADD;
	m_blendState = sRenderer->CreateBlendState(blendDesc);

	//sprite texture
	/////////////////////////////////////////////////////////////////////
	m_spriteTextureShader = sRenderer->GenerateRenderProgram();
	m_spriteTextureShader->SetLibraryWithPath("Shader/Sprite2D.lib");
	m_spriteTextureShader->CompileVertexFunctionWithName("Sprite2DVS");
	m_spriteTextureShader->CompileFragmentFunctionWithName("Sprite2DTexturePS");

	m_spriteTexturePipeline = sRenderer->GenerateRenderPipeline();
	m_spriteTexturePipeline->SetInputLayoutDesc(desc, 3);
	m_spriteTexturePipeline->SetProgramShader(m_spriteTextureShader);
	m_spriteTexturePipeline->SetBlendState(m_blendState);
	/////////////////////////////////////////////////////////////////////

	//sprite color
	/////////////////////////////////////////////////////////////////////
	m_spriteColorShader = sRenderer->GenerateRenderProgram();
	m_spriteColorShader->SetLibraryWithPath("Shader/Sprite2D.lib");
	m_spriteColorShader->CompileVertexFunctionWithName("Sprite2DVS");
	m_spriteColorShader->CompileFragmentFunctionWithName("Sprite2DColorPS");	

	m_spriteColorPipeline = sRenderer->GenerateRenderPipeline();
	m_spriteColorPipeline->SetInputLayoutDesc(desc, 3);
	m_spriteColorPipeline->SetProgramShader(m_spriteColorShader);
	m_spriteColorPipeline->SetBlendState(m_blendState);
	/////////////////////////////////////////////////////////////////////

	//font 
	/////////////////////////////////////////////////////////////////////
	m_distanceFontShader = sRenderer->GenerateRenderProgram();
	m_distanceFontShader->SetLibraryWithPath("Shader/Font.lib");
	m_distanceFontShader->CompileVertexFunctionWithName("FontVS");
	m_distanceFontShader->CompileFragmentFunctionWithName("FontPS");	

	m_distanceFontPipeline = sRenderer->GenerateRenderPipeline();
	m_distanceFontPipeline->SetInputLayoutDesc(desc, 3);
	m_distanceFontPipeline->SetProgramShader(m_distanceFontShader);
	m_distanceFontPipeline->SetBlendState(m_blendState);
	/////////////////////////////////////////////////////////////////////	

	return true;
}

void RenderSprite::Destroy()
{

}

void RenderSprite::DrawRect(Color4 color, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	//建立顶点数据
	DecalColorVertex vt[] =
	{
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	leftTop.y,		0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(rightBottom.x,	rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		rightBottom.y,	0),		color,		Vector2Df(0, 0) },
		{ Vector3Df(leftTop.x,		leftTop.y,		0),		color,		Vector2Df(0, 0) },
	};

	this->DrawColor2D(vt, 6);
}

void RenderSprite::DrawWireFrame(Color4 color, const Rect2Di& drawRect)
{
	//计算在屏幕的位置
	Vector2Df leftTop = sRenderMgr->GetPosFromWindowPos(drawRect.left, drawRect.top);
	Vector2Df rightBottom = sRenderMgr->GetPosFromWindowPos(drawRect.right, drawRect.bottom);

	//建立顶点数据
	DecalColorVertex vt[] =
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

	this->DrawColor2D(vt, 6);
}

void RenderSprite::DrawColor2D(const DecalColorVertex* vertData, uint32 vertNum)
{
	m_vertMemBuf.Insert(m_vertCount * sizeof(DecalColorVertex), vertData, vertNum * sizeof(DecalColorVertex));

	uint32 blockIndex = m_drawColorObj->AddRenderBlock(m_vertBuf, m_spriteColorPipeline);
	m_drawColorObj->SetBlockDrawInfo(blockIndex, EDT_TRIANGLELIST, vertNum, m_vertCount);

	m_vertCount += vertNum;
}

void RenderSprite::DrawTexture2D(const DecalColorVertex* vertData, uint32 vertNum, RenderResourceSet* resSet)
{
	m_vertMemBuf.Insert(m_vertCount * sizeof(DecalColorVertex), vertData, vertNum * sizeof(DecalColorVertex));

	uint32 blockIndex = m_drawTextureObj->AddRenderBlock(m_vertBuf, m_spriteTexturePipeline);
	m_drawTextureObj->SetBlockDrawInfo(blockIndex, EDT_TRIANGLELIST, vertNum, m_vertCount);
	m_drawTextureObj->SetBlockResourceSet(blockIndex, resSet);

	m_vertCount += vertNum;
}

void RenderSprite::DrawFont2D(const DecalColorVertex* vertData, uint32 vertNum, RenderResourceSet* resSet)
{
	m_vertMemBuf.Insert(m_vertCount * sizeof(DecalColorVertex), vertData, vertNum * sizeof(DecalColorVertex));

	uint32 blockIndex = m_drawFontObj->AddRenderBlock(m_vertBuf, m_distanceFontPipeline);
	m_drawFontObj->SetBlockDrawInfo(blockIndex, EDT_TRIANGLELIST, vertNum, m_vertCount);
	m_drawFontObj->SetBlockResourceSet(blockIndex, resSet);

	m_vertCount += vertNum;
}

void RenderSprite::Render()
{
	m_vertBuf->CopyData(m_vertMemBuf, m_vertCount * sizeof(DecalColorVertex));

	sRenderMgr->DoRender(m_drawColorObj);
	sRenderMgr->DoRender(m_drawTextureObj);
	sRenderMgr->DoRender(m_drawFontObj);

	m_vertCount = 0;
}