#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11RasterizerState::D3D11RasterizerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pState = nullptr;
}

D3D11RasterizerState:: ~D3D11RasterizerState(void)
{
	SAFE_RELEASE(m_pState);
}

bool D3D11RasterizerState::CreateState(const RasterizerStateDesc& desc)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = (D3D11_FILL_MODE)(desc.fillMode + D3D11_FILL_WIREFRAME);
	rasterizerDesc.CullMode= (D3D11_CULL_MODE)(desc.cullMode + D3D11_CULL_NONE);
	rasterizerDesc.FrontCounterClockwise= desc.frontCounterClockwise;
	rasterizerDesc.DepthBias= desc.depthBias;
	rasterizerDesc.DepthBiasClamp= desc.depthBiasClamp;
	rasterizerDesc.SlopeScaledDepthBias= desc.slopeScaledDepthBias;
	rasterizerDesc.DepthClipEnable= desc.depthClipEnable;
	rasterizerDesc.ScissorEnable= desc.scissorEnable;
	rasterizerDesc.MultisampleEnable= desc.antialiasedLineEnable;
	rasterizerDesc.AntialiasedLineEnable= desc.antialiasedLineEnable;

	HRESULT hr = m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pState);
	if(FAILED(hr)) return false;
	
	m_desc = desc;

	return true;
}

RenderRasterizerState* D3D11RasterizerState::Clone()
{
	D3D11RasterizerState* state = new D3D11RasterizerState(m_pDevice, m_pContext);
	state->m_desc = m_desc;
	state->m_pState = m_pState;
	m_pState->AddRef();
	return state;
}

void D3D11RasterizerState::ApplyDevice()
{
	m_pContext->RSSetState(m_pState);
}