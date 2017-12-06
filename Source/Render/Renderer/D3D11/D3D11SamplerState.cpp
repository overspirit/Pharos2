#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11SamplerState::D3D11SamplerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pState = nullptr;
}

D3D11SamplerState::~D3D11SamplerState(void)
{
	SAFE_RELEASE(m_pState);
}

bool D3D11SamplerState::CreateState(const SamplerStateDesc& desc)
{
	static D3D11_FILTER filters[] =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_ANISOTROPIC,
	};

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = filters[desc.filter];
	samplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)(desc.addressU + D3D11_TEXTURE_ADDRESS_WRAP);
	samplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)(desc.addressV + D3D11_TEXTURE_ADDRESS_WRAP);
	samplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)(desc.addressW + D3D11_TEXTURE_ADDRESS_WRAP);
	samplerDesc.MipLODBias = desc.mipLODBias;
	samplerDesc.MaxAnisotropy = desc.maxAnisotropy;
	samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)(desc.comparisonFunc + D3D11_COMPARISON_NEVER);
	samplerDesc.BorderColor[0] = desc.borderColor.r;
	samplerDesc.BorderColor[1] = desc.borderColor.g;
	samplerDesc.BorderColor[2] = desc.borderColor.b;
	samplerDesc.BorderColor[3] = desc.borderColor.a;
	samplerDesc.MinLOD = desc.minLOD;
	samplerDesc.MaxLOD = desc.maxLOD;

	HRESULT hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_pState);
	if(FAILED(hr)) return false;

	m_desc = desc;

	return true;
}

RenderSamplerState* D3D11SamplerState::Clone()
{
	D3D11SamplerState* state = new D3D11SamplerState(m_pDevice, m_pContext);
	state->m_desc = m_desc;
	state->m_pState = m_pState;
	m_pState->AddRef();
	return state;
}

void D3D11SamplerState::ApplyToDevice(uint32 slot)
{
	m_pContext->VSSetSamplers(slot, 1, &m_pState);
	m_pContext->PSSetSamplers(slot, 1, &m_pState);
	m_pContext->GSSetSamplers(slot, 1, &m_pState);
	m_pContext->HSSetSamplers(slot, 1, &m_pState);
	m_pContext->DSSetSamplers(slot, 1, &m_pState);
	m_pContext->CSSetSamplers(slot, 1, &m_pState);
}