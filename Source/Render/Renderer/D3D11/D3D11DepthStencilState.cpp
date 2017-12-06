#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11DepthStencilState::D3D11DepthStencilState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	
	m_pState = nullptr;
}

D3D11DepthStencilState::~D3D11DepthStencilState(void)
{
	SAFE_RELEASE(m_pState);
}

bool D3D11DepthStencilState::CreateState(const DepthStencilStateDesc& desc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = desc.depthEnable;
	depthStencilDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)desc.depthWriteMask;
	depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)(desc.depthFunc + D3D11_COMPARISON_NEVER);
	depthStencilDesc.StencilEnable = desc.stencilEnable;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = desc.stencilWriteMask;
	depthStencilDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)(desc.frontStencilFailOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)(desc.frontStencilDepthFailOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)(desc.frontStencilPassOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)(desc.frontStencilFunc + D3D11_COMPARISON_NEVER);
	depthStencilDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)(desc.backStencilFailOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)(desc.backStencilDepthFailOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)(desc.backStencilPassOp + D3D11_STENCIL_OP_KEEP);
	depthStencilDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)(desc.backStencilFunc + D3D11_COMPARISON_NEVER);

	HRESULT hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pState);
	if(FAILED(hr)) return false;

	m_desc = desc;

	return true;
}

RenderDepthStencilState* D3D11DepthStencilState::Clone()
{
	D3D11DepthStencilState* state = new D3D11DepthStencilState(m_pDevice, m_pContext);
	state->m_desc = m_desc;
	state->m_pState = m_pState;
	m_pState->AddRef();
	return state;
}

void D3D11DepthStencilState::ApplyDevice()
{
	m_pContext->OMSetDepthStencilState(m_pState, m_desc.stencilRefValue);
}