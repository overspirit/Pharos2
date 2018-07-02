#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

ImageStatPostProcess::ImageStatPostProcess()
{
	m_sumLumLogTech = nullptr;
	m_sumLumIterTech = nullptr;
	m_adaptedTech = nullptr;

	m_inputTex = nullptr;
}

ImageStatPostProcess::~ImageStatPostProcess()
{
	SAFE_DELETE(m_sumLumLogTech);
	SAFE_DELETE(m_sumLumIterTech);
	SAFE_DELETE(m_adaptedTech);
	SAFE_DELETE(m_sumLogTarget);
}

bool ImageStatPostProcess::Init()
{
	m_sumLumLogTech = sRenderMgr->GenerateRenderTechnique("SumLumLog");
	m_sumLumIterTech = sRenderMgr->GenerateRenderTechnique("SumLumIterative");
	m_adaptedTech = sRenderMgr->GenerateRenderTechnique("AdaptedLum");

	RenderVariable* texCoordOffsetVar = m_sumLumLogTech->GetVariable("tex_coord_offset");


	Vector2Df texCoordOffset[4];
	texCoordOffset[0].Set(1 / 64.0f, 1 / 64.0f);
	texCoordOffset[1].Set(1 / 64.0f, -1 / 64.0f);
	texCoordOffset[2].Set(-1 / 64.0f, 1 / 64.0f);
	texCoordOffset[3].Set(-1 / 64.0f, -1 / 64.0f);
	
	texCoordOffsetVar->SetValue(texCoordOffset, sizeof(texCoordOffset));


	Renderer* renderer = sRenderMgr->GetCurrentRenderer();
	m_sumLogTarget = renderer->CreateFrameBuffer(64, 64);
	m_sumLogTex = m_sumLogTarget->CreateRenderTexture(0, Render::EPF_R32_FLOAT);

	return true;
}

void ImageStatPostProcess::SetInputPin(uint32 index, RenderTexture* tex)
{
	m_inputTex = tex;

	RenderVariable* sumLogSrcTexVar = m_sumLumLogTech->GetVariable("src_tex");
	sumLogSrcTexVar->SetValue(tex);
}

void ImageStatPostProcess::SetOutputPin(uint32 index, RenderTexture* tex)
{

}

void ImageStatPostProcess::Apply()
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();
	renderer->BindFrameBuffer(m_sumLogTarget);

	m_sumLumLogTech->ApplyToDevice();
	m_sumLumLogTech->GetPass(0)->ApplyToDevice();

	sRenderMgr->DrawFullScreenQuad(m_inputTex);
}