#include "PreCompile.h"
#include "Pharos.h"

#define MAX_VERT_SIZE	(2048 * 6 * sizeof(DecalColorVertex))

RenderSprite::RenderSprite(void)
{
	m_drawColorObj = NULL;
	
	m_drawTextureObj = NULL;
	m_drawFontObj = NULL;
	
	m_vertCount = 0;
}

RenderSprite::~RenderSprite(void)
{

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

	DepthStencilStateDesc depthState;
	depthState.depthEnable = true;
	depthState.depthFunc = COMPARISON_ALWAYS;

	//sprite color
	/////////////////////////////////////////////////////////////////////
    m_spriteColorShader = sRenderer->GenerateRenderProgram();
    m_spriteColorShader->SetLibraryWithPath("Shader/Sprite2D.lib");
    m_spriteColorShader->CompileVertexFunctionWithName("Sprite2DVS");
    m_spriteColorShader->CompileFragmentFunctionWithName("Sprite2DColorPS");

    m_spriteColorPipeline = sRenderer->GenerateRenderPipeline();
    m_spriteColorPipeline->SetInputLayoutDesc(desc, 3);
    m_spriteColorPipeline->SetProgramShader(m_spriteColorShader);
    m_spriteColorPipeline->SetBlendState(blendDesc);
	m_spriteColorPipeline->SetDepthStencilState(depthState);
	/////////////////////////////////////////////////////////////////////

    //sprite texture
    /////////////////////////////////////////////////////////////////////
    m_spriteTextureShader = sRenderer->GenerateRenderProgram();
    m_spriteTextureShader->SetLibraryWithPath("Shader/Sprite2D.lib");
    m_spriteTextureShader->CompileVertexFunctionWithName("Sprite2DVS");
    m_spriteTextureShader->CompileFragmentFunctionWithName("Sprite2DTexturePS");

    m_spriteTexturePipeline = sRenderer->GenerateRenderPipeline();
    m_spriteTexturePipeline->SetInputLayoutDesc(desc, 3);
    m_spriteTexturePipeline->SetProgramShader(m_spriteTextureShader);
    m_spriteTexturePipeline->SetBlendState(blendDesc);
	m_spriteTexturePipeline->SetDepthStencilState(depthState);
    /////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////
	//font 
	/////////////////////////////////////////////////////////////////////
	m_distanceFontShader = sRenderer->GenerateRenderProgram();
	m_distanceFontShader->SetLibraryWithPath("Shader/Font.lib");
	m_distanceFontShader->CompileVertexFunctionWithName("FontVS");
	m_distanceFontShader->CompileFragmentFunctionWithName("FontPS");	

	m_distanceFontPipeline = sRenderer->GenerateRenderPipeline();
	m_distanceFontPipeline->SetInputLayoutDesc(desc, 3);
	m_distanceFontPipeline->SetProgramShader(m_distanceFontShader);
	m_distanceFontPipeline->SetBlendState(blendDesc);
	m_distanceFontPipeline->SetDepthStencilState(depthState);
	/////////////////////////////////////////////////////////////////////

	return true;
}

void RenderSprite::Destroy()
{
	SAFE_DELETE(m_drawColorObj);
	SAFE_DELETE(m_drawTextureObj);
	SAFE_DELETE(m_drawFontObj);
	
	SAFE_DELETE(m_vertBuf);

	SAFE_DELETE(m_spriteColorShader);
	SAFE_DELETE(m_spriteColorPipeline);

	SAFE_DELETE(m_spriteTextureShader);
	SAFE_DELETE(m_spriteTexturePipeline);
	
	SAFE_DELETE(m_distanceFontShader);	
	SAFE_DELETE(m_distanceFontPipeline);
}

void RenderSprite::DrawColor2D(const DecalColorVertex* vertData, uint32 vertNum, RenderResourceSet* resSet)
{
	m_vertMemBuf.Insert(m_vertCount * sizeof(DecalColorVertex), vertData, vertNum * sizeof(DecalColorVertex));

	uint32 blockIndex = m_drawColorObj->AddRenderBlock(m_vertBuf, m_spriteColorPipeline);
    m_drawColorObj->SetBlockDrawInfo(blockIndex, EDT_TRIANGLELIST, vertNum, m_vertCount);
    m_drawColorObj->SetBlockResourceSet(blockIndex, resSet);

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

void RenderSprite::Draw()
{
	m_vertBuf->CopyData(m_vertMemBuf);

	sRenderMgr->DoRender(m_drawColorObj);
    sRenderMgr->DoRender(m_drawTextureObj);
	sRenderMgr->DoRender(m_drawFontObj);

	m_vertCount = 0;
}
