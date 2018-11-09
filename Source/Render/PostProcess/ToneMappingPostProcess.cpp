#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

ToneMappingPostProcess::ToneMappingPostProcess()
{
	m_toneMappingTech = nullptr;
	m_toneMappingTarget = nullptr;
	m_toneMappingTex = nullptr;
}

ToneMappingPostProcess::~ToneMappingPostProcess()
{
	SAFE_DELETE(m_toneMappingTech);
	SAFE_DELETE(m_toneMappingTarget);
}

bool ToneMappingPostProcess::Init()
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();

	m_toneMappingTech = sRenderMgr->GenerateRenderTechnique("ToneMapping");
	
	m_toneMappingTarget = renderer->CreateFrameBuffer(1280, 720);
	m_toneMappingTex = m_toneMappingTarget->CreateRenderTexture(0, Render::EPF_RGBA8_UNORM);
	
	m_outputPins.push_back(m_toneMappingTex);
	
	return true;
}

void ToneMappingPostProcess::Apply()
{
	Renderer* renderer = sRenderMgr->GetCurrentRenderer();
	if (m_inputPins.size() <= 0) return;

 	renderer->BindFrameBuffer(m_toneMappingTarget);

	if (m_inputPins[1] != nullptr) renderer->BindTexture(1, m_inputPins[1]);

 	m_toneMappingTech->ApplyToDevice();
 	m_toneMappingTech->GetPass(0)->ApplyToDevice();
 
 	sRenderMgr->DrawFullScreenQuad(m_inputPins[0]);
}