#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

ImageStatPostProcess::ImageStatPostProcess()
{
	m_sumLumLogTech = nullptr;
	m_sumLogTarget = nullptr;
	m_sumLogTex = nullptr;

	m_sumLumIterTech16 = nullptr;
	m_sumIterTarget16 = nullptr;
	m_sumIterTex16 = nullptr;

	m_sumLumIterTech4 = nullptr;
	m_sumIterTarget4 = nullptr;
	m_sumIterTex4 = nullptr;

	m_adaptedTech = nullptr;
	m_adaptedLumTarget = nullptr;
	m_adaptedLumTex = nullptr;
}

ImageStatPostProcess::~ImageStatPostProcess()
{
	SAFE_DELETE(m_sumLumLogTech);
	SAFE_DELETE(m_sumLogTarget);

	SAFE_DELETE(m_sumLumIterTech16);
	SAFE_DELETE(m_sumIterTarget16);

	SAFE_DELETE(m_sumLumIterTech4);
	SAFE_DELETE(m_sumIterTarget4);

	SAFE_DELETE(m_adaptedTech);
	SAFE_DELETE(m_adaptedLumTarget);
}

bool ImageStatPostProcess::Init()
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	m_sumLumLogTech = sRenderMgr->GenerateRenderTechnique("SumLumLog");
	m_sumLumIterTech16 = sRenderMgr->GenerateRenderTechnique("SumLumIterative");
	m_sumLumIterTech4 = sRenderMgr->GenerateRenderTechnique("SumLumIterative");
	m_adaptedTech = sRenderMgr->GenerateRenderTechnique("AdaptedLum");
	
	Vector2Df texCoordOffset[4];
	
	//////////////////////////////////////////////////////////////////////////
	texCoordOffset[0].Set(1 / 64.0f, 1 / 64.0f);
	texCoordOffset[1].Set(1 / 64.0f, -1 / 64.0f);
	texCoordOffset[2].Set(-1 / 64.0f, 1 / 64.0f);
	texCoordOffset[3].Set(-1 / 64.0f, -1 / 64.0f);
	RenderVariable* texCoordOffsetVar = m_sumLumLogTech->GetVariable("tex_coord_offset");
	texCoordOffsetVar->SetValue(texCoordOffset, sizeof(texCoordOffset));
		
	m_sumLogTarget = renderer->CreateFrameBuffer(64, 64);
	m_sumLogTex = m_sumLogTarget->CreateRenderTexture(0, Render::EPF_R32_FLOAT);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	texCoordOffset[0].Set(1 / 16.0f, 1 / 16.0f);
	texCoordOffset[1].Set(1 / 16.0f, -1 / 16.0f);
	texCoordOffset[2].Set(-1 / 16.0f, 1 / 16.0f);
	texCoordOffset[3].Set(-1 / 16.0f, -1 / 16.0f);
	texCoordOffsetVar = m_sumLumIterTech16->GetVariable("tex_coord_offset");
	texCoordOffsetVar->SetValue(texCoordOffset, sizeof(texCoordOffset));

	m_sumIterTarget16 = renderer->CreateFrameBuffer(16, 16);
	m_sumIterTex16 = m_sumIterTarget16->CreateRenderTexture(0, Render::EPF_R32_FLOAT);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	texCoordOffset[0].Set(1 / 4.0f, 1 / 4.0f);
	texCoordOffset[1].Set(1 / 4.0f, -1 / 4.0f);
	texCoordOffset[2].Set(-1 / 4.0f, 1 / 4.0f);
	texCoordOffset[3].Set(-1 / 4.0f, -1 / 4.0f);
	texCoordOffsetVar = m_sumLumIterTech4->GetVariable("tex_coord_offset");
	texCoordOffsetVar->SetValue(texCoordOffset, sizeof(texCoordOffset));

	m_sumIterTarget4 = renderer->CreateFrameBuffer(4, 4);
	m_sumIterTex4 = m_sumIterTarget4->CreateRenderTexture(0, Render::EPF_R32_FLOAT);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	m_adaptedLumTarget = renderer->CreateFrameBuffer(1, 1);
	m_adaptedLumTex = m_adaptedLumTarget->CreateRenderTexture(0, Render::EPF_R32_FLOAT);
	//////////////////////////////////////////////////////////////////////////

	m_outputPins.push_back(m_adaptedLumTex);

	return true;
}

void ImageStatPostProcess::Apply()
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();
	if (m_inputPins.size() <= 0) return;

	//////////////////////////////////////////////////////////////////////////
	renderer->BindFrameBuffer(m_sumLogTarget);

	m_sumLumLogTech->ApplyToDevice();
	m_sumLumLogTech->GetPass(0)->ApplyToDevice();

	sRenderMgr->DrawFullScreenQuad(m_inputPins[0]);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	renderer->BindFrameBuffer(m_sumIterTarget16);

	m_sumLumIterTech16->ApplyToDevice();
	m_sumLumIterTech16->GetPass(0)->ApplyToDevice();
	
	sRenderMgr->DrawFullScreenQuad(m_sumLogTex);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	renderer->BindFrameBuffer(m_sumIterTarget4);

	m_sumLumIterTech4->ApplyToDevice();
	m_sumLumIterTech4->GetPass(0)->ApplyToDevice();

	sRenderMgr->DrawFullScreenQuad(m_sumIterTex16);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	renderer->BindFrameBuffer(m_adaptedLumTarget);
	
	m_adaptedTech->ApplyToDevice();
	m_adaptedTech->GetPass(0)->ApplyToDevice();

	sRenderMgr->DrawFullScreenQuad(m_sumIterTex4);
	//////////////////////////////////////////////////////////////////////////
}