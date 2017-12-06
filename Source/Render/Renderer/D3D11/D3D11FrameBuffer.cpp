#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11FrameBuffer::D3D11FrameBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	
	ZeroMemory(m_pTargetSlots, sizeof(m_pTargetSlots));
	ZeroMemory(m_pTargetTexs, sizeof(m_pTargetTexs));
	m_pDepthView = nullptr;
	m_pDepthTex = nullptr;
}

D3D11FrameBuffer::~D3D11FrameBuffer(void)
{
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		SAFE_RELEASE(m_pTargetSlots[i]);
		SAFE_DELETE(m_pTargetTexs[i]);
	}

	SAFE_RELEASE(m_pDepthView);
	SAFE_DELETE(m_pDepthTex);
}

bool D3D11FrameBuffer::InitFrameBuffer(int32 width, int32 height)
{
	// Create depth stencil texture
	ID3D11Texture2D* pDepthStencil = nullptr;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT result = m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	if (FAILED(result)) return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.Flags = 0;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	result = m_pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &m_pDepthView);
	SAFE_RELEASE(pDepthStencil);
	if (FAILED(result)) return false;

	m_width = width;
	m_height = height;

	m_viewPort.left = 0;
	m_viewPort.top = 0;
	m_viewPort.right = m_width;
	m_viewPort.bottom = m_height;

	return true;
}

bool D3D11FrameBuffer::GenerateDefaultFrameBuffer(IDXGISwapChain* pSwapChain)
{
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HRESULT result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
	if (FAILED(result)) return false;

	result = m_pDevice->CreateRenderTargetView(pBackBufferTexture, 0, &m_pTargetSlots[0]);
	if (FAILED(result))
	{
		SAFE_RELEASE(pBackBufferTexture);
		return false;
	}

	D3D11_TEXTURE2D_DESC texDesc;
	pBackBufferTexture->GetDesc(&texDesc);
	if (!InitFrameBuffer(texDesc.Width, texDesc.Height))
	{
		SAFE_RELEASE(pBackBufferTexture);
		return false;
	}

	SAFE_RELEASE(pBackBufferTexture);

	return true;
}

void D3D11FrameBuffer::ClearFrameBuffer(Color4 color, float32 depth, uint32 stencil)
{
	Color4f clearColor = color;

	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		if (m_pTargetSlots[i] != nullptr)
		{
			m_pContext->ClearRenderTargetView(m_pTargetSlots[i], (float32*)&clearColor);
		}
	}

	if (m_pDepthView != nullptr)
	{
		m_pContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}
}

RenderTexture* D3D11FrameBuffer::CreateRenderTexture(uint32 slot, EPixelFormat fmt)
{
	SAFE_DELETE(m_pTargetTexs[slot]);
	SAFE_RELEASE(m_pTargetSlots[slot]);

	m_pTargetTexs[slot] = new D3D11Texture(m_pDevice, m_pContext);
	if (m_pTargetTexs[slot]->CreateTargetTexture(m_width, m_height, fmt))
	{
		ID3D11Texture2D* pTexture = m_pTargetTexs[slot]->GetTexture();

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = PF2D3D11FMT(fmt);
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		m_pDevice->CreateRenderTargetView(pTexture, &desc, &m_pTargetSlots[slot]);
	}

	return m_pTargetTexs[slot];
}

RenderTexture* D3D11FrameBuffer::CreateDepthTexture()
{
	SAFE_DELETE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthView);

	m_pDepthTex = new D3D11Texture(m_pDevice, m_pContext);
	if (m_pDepthTex->CreateDepthTexture(m_width, m_height))
	{
		ID3D11Texture2D* pTexture = m_pDepthTex->GetTexture();

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0;//D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;	//设置READ_ONLY就不能写入关联的纹理了...
		desc.Texture2D.MipSlice = 0;
		m_pDevice->CreateDepthStencilView(pTexture, &desc, &m_pDepthView);
	}

	return m_pDepthTex;
}

RenderTexture* D3D11FrameBuffer::GetRenderTexture(uint32 slot)
{
	if (slot >= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
	{
		assert(false);
	}

	return m_pTargetTexs[slot];
}

RenderTexture* D3D11FrameBuffer::GetDepthTexture()
{
	return m_pDepthTex;
}

void D3D11FrameBuffer::ApplyDevice()
{
	m_pContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_pTargetSlots, m_pDepthView);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_viewPort.GetWidth();
	viewport.Height = (float)m_viewPort.GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = (float)m_viewPort.left;
	viewport.TopLeftY = (float)m_viewPort.top;
	m_pContext->RSSetViewports(1, &viewport);
}