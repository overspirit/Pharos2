#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11RenderBlock::D3D11RenderBlock()
{
	m_renderer = nullptr;

	m_tech = nullptr;
	m_layout = nullptr;
}

D3D11RenderBlock::~D3D11RenderBlock()
{

}

void D3D11RenderBlock::BindLayout(RenderLayout* layout)
{
	m_layout = static_cast<D3D11RenderLayout*>(layout);
}

void D3D11RenderBlock::BindTechnique(RenderTechnique* tech)
{
	m_tech = static_cast<D3D11RenderTechnique*>(tech);
}

void D3D11RenderBlock::ApplyToDevice()
{
	m_renderer = static_cast<D3D11Renderer*>(sRenderMgr->GetCurrentRenderer());
	if (m_layout == nullptr || m_tech == nullptr)
	{
		assert(false);
		return;
	}

	if (m_countNum == 0)
	{
		m_startIndex = 0;

		if (m_layout->IsUseIndexBuffer())
		{
			m_countNum = m_layout->GetIndexNum();
		}
		else
		{
			m_countNum = m_layout->GetVertNum();
		}
	}

	m_tech->ApplyToDevice();

	m_renderer->BindLayout(m_layout);

	for (uint32 i = 0; i < m_tech->GetPassNum(); i++)
	{
		D3D11RenderPass* d3d11pass = static_cast<D3D11RenderPass*>(m_tech->GetPass(i));
		if (d3d11pass == nullptr)
			break;

		d3d11pass->ApplyToDevice();

		m_renderer->DrawImmediate(m_drawType, m_startIndex, m_countNum);
	}
}