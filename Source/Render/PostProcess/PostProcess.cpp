#include "PreCompile.h"
#include "Pharos.h"

PostProcess::PostProcess()
{
	m_renderer = nullptr;

	m_tech = nullptr;
	m_block = nullptr;

	m_frameBuf = nullptr;
}

PostProcess::~PostProcess()
{
	SAFE_DELETE(m_tech);
	SAFE_DELETE(m_block);
	SAFE_DELETE(m_frameBuf);
}

bool PostProcess::InitWithTech(const char8* techName)
{
	m_renderer = sRenderMgr->GetCurrentRenderer();

	m_tech = sRenderMgr->GenerateRenderTechnique(techName);
	if (m_tech == nullptr) return false;

	RenderLayout* layout = sRenderMgr->GetQuadLayout();

	m_block = sRenderMgr->GenerateRenderBlock();
	uint32 patchIndex = m_block->AddRenderBlockPatch(layout, m_tech);
	m_block->SetBlockPatchDrawType(patchIndex, Render::EDT_TRIANGLELIST);
	m_block->SetBlockPatchDrawRange(patchIndex, 0, layout->GetVertNum());

	return true;
}

void PostProcess::SetInputPin(uint32 index, RenderTexture* tex)
{
	if (index >= m_inputPins.size())
	{
		m_inputPins.resize(index + 1);
	}

	m_inputPins[index] = tex;
}

RenderTexture* PostProcess::GetInputPin(uint32 index)
{
	if (index >= m_inputPins.size())
		return nullptr;

	return m_inputPins[index];
}

void PostProcess::SetOutputPin(uint32 index, RenderTexture* tex)
{
	if (tex == nullptr) return;

	if (index >= m_outputPins.size())
	{
		m_outputPins.resize(index + 1);
	}

	m_outputPins[index] = tex;

	if (m_frameBuf == nullptr)
	{
		uint32 texWidth = tex->GetWidth();
		uint32 texHeight = tex->GetHeight();
		m_frameBuf = m_renderer->CreateFrameBuffer(texWidth, texHeight);
	}

	m_frameBuf->AttachTexture(index, tex);
}

RenderTexture* PostProcess::GetOutputPin(uint32 index)
{
	if (index >= m_outputPins.size()) return nullptr;

	return m_outputPins[index];
}

void PostProcess::Apply()
{
	m_renderer->BindFrameBuffer(m_frameBuf);

	for (uint32 i = 0; i < m_inputPins.size(); i++)
	{
		m_renderer->BindTexture(i, m_inputPins[i]);
	}

	m_block->ApplyToDevice();
}
