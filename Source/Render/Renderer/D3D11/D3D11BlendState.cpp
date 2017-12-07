#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11BlendState::D3D11BlendState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pState = nullptr;
}

D3D11BlendState::~D3D11BlendState(void)
{
	SAFE_RELEASE(m_pState);
}

bool D3D11BlendState::CreateState(const BlendStateDesc& desc)
{
	const static D3D11_BLEND mapList[] =
	{
		D3D11_BLEND_ZERO,
		D3D11_BLEND_ONE,
		D3D11_BLEND_SRC_COLOR,
		D3D11_BLEND_INV_SRC_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_DEST_COLOR,
		D3D11_BLEND_INV_DEST_COLOR,
		D3D11_BLEND_DEST_ALPHA,
		D3D11_BLEND_INV_DEST_ALPHA,
	};

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = desc.alphaToCoverageEnable;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = desc.blendEnable;
	blendDesc.RenderTarget[0].SrcBlend = mapList[desc.srcBlend];
	blendDesc.RenderTarget[0].DestBlend = mapList[desc.destBlend];
	blendDesc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)(desc.blendOp + D3D11_BLEND_OP_ADD);
	blendDesc.RenderTarget[0].SrcBlendAlpha = mapList[desc.srcBlendAlpha];
	blendDesc.RenderTarget[0].DestBlendAlpha = mapList[desc.destBlendAlpha];
	blendDesc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)(desc.blendOpAlpha + D3D11_BLEND_OP_ADD);
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = m_pDevice->CreateBlendState(&blendDesc, &m_pState);
	if(FAILED(hr)) return false;

	m_desc = desc;

	return true;
}

RenderBlendState* D3D11BlendState::Clone()
{
	D3D11BlendState* state = new D3D11BlendState(m_pDevice, m_pContext);
	state->m_desc = m_desc;
	state->m_pState = m_pState;
	m_pState->AddRef();
	return state;
}

void D3D11BlendState::ApplyDevice()
{
	float blendFactor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	m_pContext->OMSetBlendState(m_pState, blendFactor, 0xFFFFFFFF);
}