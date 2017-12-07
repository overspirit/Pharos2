#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11Texture::D3D11Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pTexture = nullptr;
	m_pView = nullptr;

	m_pState = nullptr;
}

D3D11Texture::~D3D11Texture()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pView);
}

bool D3D11Texture::LoadFromFile(const char8* szPath)
{
	if (szPath == nullptr || *szPath == '\0') return false;

	Image image;
	if (!image.Open(szPath)) return false;

	if (image.GetBPP() != 32)
	{
		if (!image.ConvertTo32Bits()) return false;
	}

	return LoadFromImage(image);
}

bool D3D11Texture::LoadFromImage(const Image& image)
{
	uint32 width = image.GetWidth();
	uint32 height = image.GetHeight();

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//Image使用的是BGRA格式
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = image.GetDataPointer();
	data.SysMemPitch = width * 4;
	data.SysMemSlicePitch = 0;

	HRESULT hr = m_pDevice->CreateTexture2D(&desc, &data, &m_pTexture);
	if(FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = -1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pView);
	if(FAILED(hr)) return false;

	m_width = desc.Width;
	m_height = desc.Height;
	m_fmt = D3D11FMT2PF(desc.Format);
	m_eleSize = GetPFSize(m_fmt);

	return true;
}

bool D3D11Texture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = PF2D3D11FMT(fmt);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &m_pTexture);
	if(FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = -1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pView);
	if(FAILED(hr)) return false;

	m_width = width;
	m_height = height;
	m_fmt = fmt;
	m_eleSize = GetPFSize(m_fmt);

	return true;
}

bool D3D11Texture::CreateDepthTexture(int32 width, int32 height)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &m_pTexture);
	if(FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = -1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pView);
	if(FAILED(hr)) return false;

	m_width = desc.Width;
	m_height = desc.Height;
	m_fmt = D3D11FMT2PF(desc.Format);
	m_eleSize = GetPFSize(m_fmt);

	return true;
}

bool D3D11Texture::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = PF2D3D11FMT(fmt);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &m_pTexture);
	if(FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = desc.Format;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = -1;
	viewDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pView);
	if(FAILED(hr)) return false;

	m_pTexture->GetDesc(&desc);

	m_width = desc.Width;
	m_height = desc.Height;
	m_fmt = D3D11FMT2PF(desc.Format);
	m_eleSize = GetPFSize(m_fmt);

	return true;
}


bool D3D11Texture::CopyFromData(const void* pImageData, uint32 nDataSize)
{
	if(m_pTexture == nullptr || pImageData == nullptr) return false;
	if(nDataSize / m_eleSize != m_width * m_height) return false;

	m_pContext->UpdateSubresource(m_pTexture, 0, nullptr, pImageData, m_eleSize * m_width, 0);

	return true;
}

bool D3D11Texture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	if(m_pTexture == nullptr || pData == nullptr) return false;

	int32 rows = rt.bottom - rt.top;
	int32 pitchs = rt.right - rt.left;
	if(nDataSize / m_eleSize != pitchs * rows) return false;

	D3D11_BOX destBox;
	destBox.left = rt.left;
	destBox.top = rt.top;
	destBox.front = 0;
	destBox.right = rt.right;
	destBox.bottom = rt.bottom;
	destBox.back = 1;
	m_pContext->UpdateSubresource(m_pTexture, 0, &destBox, pData, m_eleSize * pitchs, 0);

	return true;
}

bool D3D11Texture::CopyFromTexture(RenderTexture* srcTex)
{
	return false;
}

bool D3D11Texture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	D3D11Texture* d3d11Tex = static_cast<D3D11Texture*>(srcTex);
	if (d3d11Tex == nullptr) return false;

	ID3D11Texture2D* d3d11TexRes = d3d11Tex->GetTexture();

	uint32 destX = destRect.left;
	uint32 destY = destRect.top;
	m_pContext->CopySubresourceRegion(m_pTexture, 0, destX, destY, 0, d3d11TexRes, 0, NULL);

	return true;
}

void D3D11Texture::SetSampleState(RenderSamplerState* state)
{
	m_pState = static_cast<D3D11SamplerState*>(state);
}

void D3D11Texture::ApplyToDevice(uint32 slot)
{
	m_pContext->VSSetShaderResources(slot, 1, &m_pView);
	m_pContext->PSSetShaderResources(slot, 1, &m_pView);
	m_pContext->GSSetShaderResources(slot, 1, &m_pView);
	m_pContext->HSSetShaderResources(slot, 1, &m_pView);
	m_pContext->DSSetShaderResources(slot, 1, &m_pView);
	m_pContext->CSSetShaderResources(slot, 1, &m_pView);

	if (m_pState != nullptr)
	{
		m_pState->ApplyToDevice(slot);
	}
}