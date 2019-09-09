#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

HDRPostProcess::HDRPostProcess()
{
	m_imageStatPostProcess = nullptr;
	m_lensEffectsPostProcess = nullptr;
	m_toneMappingPostProcess = nullptr;
}

HDRPostProcess::~HDRPostProcess()
{
	SAFE_DELETE(m_imageStatPostProcess);
	SAFE_DELETE(m_lensEffectsPostProcess);
	SAFE_DELETE(m_toneMappingPostProcess);
}

bool HDRPostProcess::Init()
{
	m_imageStatPostProcess = new ImageStatPostProcess();
	//m_imageStatPostProcess->Init();
	RenderTexture* imageStatOutputTex = m_imageStatPostProcess->GetOutputPin(0);

	m_lensEffectsPostProcess = new LensEffectsPostProcess();
	//m_lensEffectsPostProcess->Init();
	RenderTexture* lensEffectOutputTex = m_lensEffectsPostProcess->GetOutputPin(0);

	m_toneMappingPostProcess = new ToneMappingPostProcess();
	//m_toneMappingPostProcess->Init();
	m_toneMappingPostProcess->SetInputPin(1, imageStatOutputTex);
	m_toneMappingPostProcess->SetInputPin(2, lensEffectOutputTex);

	return false;
}

void HDRPostProcess::SetInputPin(uint32 index, RenderTexture* tex)
{
	m_imageStatPostProcess->SetInputPin(0, tex);
	m_lensEffectsPostProcess->SetInputPin(0, tex);
	m_toneMappingPostProcess->SetInputPin(0, tex);
}

RenderTexture* HDRPostProcess::GetOutputPin(uint32 index)
{
	if(m_toneMappingPostProcess != nullptr)
		return m_toneMappingPostProcess->GetOutputPin(index);

	return nullptr;
}

void HDRPostProcess::Apply()
{
	if (m_imageStatPostProcess != nullptr) m_imageStatPostProcess->Apply();
	//if (m_lensEffectsPostProcess != nullptr) m_lensEffectsPostProcess->Apply();
	if (m_toneMappingPostProcess != nullptr) m_toneMappingPostProcess->Apply();
}