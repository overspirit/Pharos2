#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11Renderer::D3D11Renderer(void)
{
	m_driverType = D3D_DRIVER_TYPE_HARDWARE;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	m_device = nullptr;
	m_context = nullptr;

	m_dxgiAdapter = nullptr;
	m_dxgiFactory = nullptr;
	m_swapChain = nullptr;


	m_defFrameBuf = nullptr;
	m_adapterMemSize = 0;

	m_bindLayout = nullptr;
	m_bindShader = nullptr;
}

D3D11Renderer::~D3D11Renderer(void)
{
}

bool D3D11Renderer::Init()
{
	unsigned int creationFlags = 0;

#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_context);
	if (FAILED(result)) return false;

	IDXGIDevice* dxgiDevice = nullptr;
	result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(result)) return false;

	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	if (FAILED(result)) return false;

	result = m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
	if (FAILED(result)) return false;

	// 释放COM接口
	SAFE_RELEASE(dxgiDevice);

	DXGI_ADAPTER_DESC	adapterDesc;
	m_dxgiAdapter->GetDesc(&adapterDesc);
	char name[512] = { 0 };
	Utils::unicode_to_utf8(adapterDesc.Description, -1, name, 512);
	m_adapterName = name;	
	m_adapterMemSize = (uint32)(adapterDesc.DedicatedVideoMemory  / 1024 / 1024);

	return true;
}

void D3D11Renderer::Destroy()
{
	SAFE_DELETE(m_defFrameBuf);

	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_context);

	SAFE_RELEASE(m_dxgiAdapter);
	SAFE_RELEASE(m_dxgiFactory);
	SAFE_RELEASE(m_swapChain);
}

bool D3D11Renderer::Create(const DeviceConfig& cfg)
{
	ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = cfg.width;
	m_swapChainDesc.BufferDesc.Height = cfg.height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = (HWND)sKernel->GetWindowHandle();
	m_swapChainDesc.Windowed = true;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;

	// 现在，创建交换链
	HRESULT result = m_dxgiFactory->CreateSwapChain(m_device, &m_swapChainDesc, &m_swapChain);
	if (FAILED(result)) return false;

	m_defFrameBuf = new D3D11FrameBuffer(m_device, m_context);
	m_defFrameBuf->GenerateDefaultFrameBuffer(m_swapChain);
	m_defFrameBuf->ApplyDevice();

	return true;
}

void D3D11Renderer::BindFrameBuffer(RenderFrameBuffer* frameBuf)
{
	D3D11FrameBuffer* d3d11framebuf = static_cast<D3D11FrameBuffer*>(frameBuf);
	if (d3d11framebuf != nullptr)
	{
		d3d11framebuf->ApplyDevice();
	}
	else
	{
		m_defFrameBuf->ApplyDevice();
	}
}

void D3D11Renderer::BindLayout(RenderLayout* layout)
{
	D3D11RenderLayout* d3d11layout = static_cast<D3D11RenderLayout*>(layout);
	if (d3d11layout != nullptr)
	{
		m_bindLayout = d3d11layout;

		d3d11layout->ApplyDevice();
	}
}

void D3D11Renderer::BindShaderData(uint32 slot, RenderShaderData* data)
{
	D3D11ConstantBuffer* cb = static_cast<D3D11ConstantBuffer*>(data);
	if (cb != nullptr)
	{
		cb->ApplyDevice(slot);
	}
}

void D3D11Renderer::BindTexture(uint32 slot, RenderTexture* tex)
{
	D3D11Texture* d3d11tex = static_cast<D3D11Texture*>(tex);
	if (d3d11tex != nullptr)
	{
		d3d11tex->ApplyDevice(slot);
	}
}

void D3D11Renderer::BindProgram(RenderProgram* program)
{
	D3D11ShaderProgram* shader = static_cast<D3D11ShaderProgram*>(program);
	if (shader != nullptr)
	{
		m_bindShader = shader;

		shader->ApplyDevice();
	}
}

void D3D11Renderer::BindBlendState(RenderBlendState* state)
{
	D3D11BlendState* blendState = static_cast<D3D11BlendState*>(state);
	if (blendState != nullptr)
	{
		blendState->ApplyDevice();
	}
}

void D3D11Renderer::BindRasterizerState(RenderRasterizerState* state)
{
	D3D11RasterizerState* rasterizerState = static_cast<D3D11RasterizerState*>(state);
	if (rasterizerState != nullptr)
	{
		rasterizerState->ApplyDevice();
	}
}

void D3D11Renderer::BindDepthStencilState(RenderDepthStencilState* state)
{
	D3D11DepthStencilState* depthState = static_cast<D3D11DepthStencilState*>(state);
	if (depthState != nullptr)
	{
		depthState->ApplyDevice();
	}
}

void D3D11Renderer::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
	if (m_bindLayout == nullptr || m_bindShader == nullptr)
	{
		assert(false);
		return;
	}

	CHECK_ENUM(0, EDT_POINTLIST);
	CHECK_ENUM(1, EDT_LINELIST);
	CHECK_ENUM(2, EDT_LINESTRIP);
	CHECK_ENUM(3, EDT_TRIANGLELIST);
	CHECK_ENUM(4, EDT_TRIANGLESTRIP);

	const static D3D_PRIMITIVE_TOPOLOGY prim[] =
	{
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};

	m_context->IASetPrimitiveTopology(prim[type]);

	if (!m_bindLayout->IsInputLayoutValid())
	{
		ID3D10Blob* blob = m_bindShader->GetVertexBlob();
		if (!m_bindLayout->CreateD3D11InputLayout(blob))
		{
			assert(false);
			return;
		}
	}

	if (m_bindLayout->IsUseIndexBuffer())
	{
		m_context->DrawIndexed(count, start, 0);
	}
	else
	{
		m_context->Draw(count, start);
	}	
}

bool D3D11Renderer::Present()
{
	HRESULT hr = m_swapChain->Present(0, 0);
	if (FAILED(hr)) return false;

	return true;
}

RenderProgram* D3D11Renderer::GenerateRenderProgram()
{
	return new D3D11ShaderProgram(m_device, m_context);
}

RenderLayout* D3D11Renderer::GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf)
{
	D3D11RenderLayout* layout = new D3D11RenderLayout(m_device, m_context);
	if (!layout->CreateVertexBuffer(vertSize, buf))
	{
		SAFE_DELETE(layout);
		return nullptr;
	}
	return layout;
}

RenderTexture* D3D11Renderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
	D3D11Texture* texture = new D3D11Texture(m_device, m_context);
	if (!texture->Create(width, height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* D3D11Renderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
	D3D11Texture* texture = new D3D11Texture(m_device, m_context);
	if (!texture->CreateTargetTexture(width, height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* D3D11Renderer::CreateDepthTexture(int32 width, int32 height)
{
	D3D11Texture* texture = new D3D11Texture(m_device, m_context);
	if (!texture->CreateDepthTexture(width, height))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* D3D11Renderer::LoadTexture(const char8* szPath)
{
	D3D11Texture* texture = new D3D11Texture(m_device, m_context);
	if (!texture->LoadFromFile(szPath))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* D3D11Renderer::LoadTexture(const Image* image)
{
	D3D11Texture* texture = new D3D11Texture(m_device, m_context);
	if (!texture->LoadFromImage(image))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderShaderData* D3D11Renderer::CreateShaderData()
{
	return new D3D11ConstantBuffer(m_device, m_context);
}

RenderFrameBuffer* D3D11Renderer::CreateFrameBuffer(int32 width, int32 height)
{
	D3D11FrameBuffer* frameBuf = new D3D11FrameBuffer(m_device, m_context);
	if (!frameBuf->InitFrameBuffer(width, height))
	{
		SAFE_DELETE(frameBuf);
		return nullptr;
	}

	return frameBuf;
}

RenderSamplerState* D3D11Renderer::CreateSampleState(const SamplerStateDesc& desc)
{
	D3D11SamplerState* state = new D3D11SamplerState(m_device, m_context);
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderBlendState* D3D11Renderer::CreateBlendState(const BlendStateDesc& desc)
{
	D3D11BlendState* state = new D3D11BlendState(m_device, m_context);
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderRasterizerState* D3D11Renderer::CreateRasterizerState(const RasterizerStateDesc& desc)
{
	D3D11RasterizerState* state = new D3D11RasterizerState(m_device, m_context);
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderDepthStencilState* D3D11Renderer::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
	D3D11DepthStencilState* state = new D3D11DepthStencilState(m_device, m_context);
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}