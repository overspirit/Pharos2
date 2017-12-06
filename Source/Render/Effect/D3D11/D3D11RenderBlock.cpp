#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11RenderBlock::D3D11RenderBlock()
{

}

D3D11RenderBlock::~D3D11RenderBlock()
{

}

bool D3D11RenderBlock::Init(RenderLayout* layout, RenderTechnique* tech)
{
	m_renderer = static_cast<D3D11Renderer*>(sRenderMgr->GetCurrentRenderer());

	D3D11RenderTechnique* d3d11Tech = static_cast<D3D11RenderTechnique*>(tech);
	D3D11RenderLayout* d3d11Layout = static_cast<D3D11RenderLayout*>(layout);
	if (d3d11Layout == nullptr || d3d11Tech == nullptr) return false;

	m_layout = d3d11Layout;
	m_tech = d3d11Tech;

	return true;
}

void D3D11RenderBlock::ApplyToDevice()
{
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