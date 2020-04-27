#include "PreCompile.h"
#include "Pharos.h"

Pharos::Render::Renderer* MakeMetalRenderer() { return new Pharos::Render::MetalRenderer(); }
Pharos::Render::RenderEffectLoader* MakeMetalEffectLoader() { return new Pharos::Render::MetalEffectLoader(); }
Pharos::Render::RenderTechnique* MakeMetalRenderTechnique() { return new Pharos::Render::MetalRenderTechnique(); }

MetalRenderer::MetalRenderer(void)
{
}

MetalRenderer::~MetalRenderer(void)
{
}

bool MetalRenderer::Init()
{
	return true;
}

void MetalRenderer::Destroy()
{
}

bool MetalRenderer::Create(const DeviceConfig& cfg)
{
	m_device = MTLCreateSystemDefaultDevice();

	m_view = (__bridge id)sKernel->GetWindowHandle();
	m_view.device = m_device;
	m_view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
	m_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
	m_view.sampleCount = 1;
	m_view.clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 1.0);
	m_view.preferredFramesPerSecond = 60;

	m_commandQueue = [m_device newCommandQueue];

	//m_defFrameBuf = new MetalFrameBuffer();
	//m_defFrameBuf->ApplyDevice();

	return true;
}

//
//RenderProgram* MetalRenderer::GenerateRenderProgram()
//{
//    return new MetalShaderProgram();
//}
//
RenderBuffer* MetalRenderer::GenerateRenderBuffer(uint32 bufSize, MemoryBuffer* buf)
{
	MetalRenderBuffer* metalBuf = new MetalRenderBuffer(m_device);
	if (!metalBuf->CreateBuffer(bufSize, buf))
	{
		SAFE_DELETE(metalBuf);
		return nullptr;
	}

	return metalBuf;
}

RenderTexture* MetalRenderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
	MetalTexture* texture = new MetalTexture(m_device);
	if (!texture->Create(width, height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}
//
//RenderTexture* MetalRenderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
//{
//    MetalTexture* texture = new MetalTexture();
//    if (!texture->CreateTargetTexture(width, height, fmt))
//    {
//        SAFE_DELETE(texture);
//        return nullptr;
//    }
//    
//    return texture;
//}
//
//RenderTexture* MetalRenderer::CreateDepthTexture(int32 width, int32 height)
//{
//    MetalTexture* texture = new MetalTexture();
//    if (!texture->CreateDepthTexture(width, height))
//    {
//        SAFE_DELETE(texture);
//        return nullptr;
//    }
//    
//    return texture;
//}

RenderTexture* MetalRenderer::LoadTexture(const char8* szPath)
{
	MetalTexture* texture = new MetalTexture(m_device);
	if (!texture->LoadFromFile(szPath))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* MetalRenderer::LoadTexture(const Image* image)
{
	MetalTexture* texture = new MetalTexture(m_device);
	if (!texture->LoadFromImage(image))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderProgram* MetalRenderer::GenerateRenderProgram(const char8* libPath)
{
	return nullptr;
}

RenderTarget* MetalRenderer::CreateRenderTarget(int32 width, int32 height)
{
	//    MetalFrameBuffer* frameBuf = new MetalFrameBuffer();
	//    if (!frameBuf->InitFrameBuffer(width, height))
	//    {
	//        SAFE_DELETE(frameBuf);
	//        return nullptr;
	//    }
	//
	//    return frameBuf;

	return nullptr;
}

RenderSamplerState* MetalRenderer::CreateSampleState(const SamplerStateDesc& desc)
{
	MetalSamplerState* state = new MetalSamplerState();
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderBlendState* MetalRenderer::CreateBlendState(const BlendStateDesc& desc)
{
	MetalBlendState* state = new MetalBlendState();
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderRasterizerState* MetalRenderer::CreateRasterizerState(const RasterizerStateDesc& desc)
{
	MetalRasterizerState* state = new MetalRasterizerState();
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderDepthStencilState* MetalRenderer::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
	MetalDepthStencilState* state = new MetalDepthStencilState();
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderCommand* MetalRenderer::GenerateRenderCommand()
{
	MetalRenderCommand* command = new MetalRenderCommand(m_commandQueue, m_view);
	if (!command->Init())
	{
		SAFE_DELETE(command);
		return nullptr;
	}

	return command;
}

RenderPipeline* MetalRenderer::GenerateRenderPipeline()
{
	return new MetalRenderPipeline(m_device);
}
