#include "PreCompile.h"
#include "Pharos.h"

MetalRenderer::MetalRenderer(void)
{
}

MetalRenderer::~MetalRenderer(void)
{
}

IMPL_MAKE_RENDERER(MetalRenderer)

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
	
	Color4f clearColor = cfg.backColor;

	m_view = (__bridge id)sKernel->GetWindowHandle();
	m_view.device = m_device;
	m_view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
	m_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
	m_view.sampleCount = 1;
	m_view.clearColor = MTLClearColorMake(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	m_view.preferredFramesPerSecond = 60;

	m_commandQueue = [m_device newCommandQueue];
	m_currCmdBuffer = [m_commandQueue commandBuffer];
	
	m_defaultTarget = new MetalViewRenderTarget(m_device, m_view);
	
	return true;
}

void MetalRenderer::Commit()
{
	[m_currCmdBuffer presentDrawable : m_view.currentDrawable];
	
	[m_currCmdBuffer commit];
}

//该方法实现自IMetalCommandFactory, 目前逻辑为当前的CommandBuffer 未Commit 之前都返回同一个对象。
id<MTLCommandBuffer> MetalRenderer::MakeMetalCommandBuffer()
{
	if(m_currCmdBuffer.status <= MTLCommandBufferStatusEnqueued)
	{
		//NSLog(@"command buffer status:%lu", (unsigned long)m_currCmdBuffer.status);
		
		return m_currCmdBuffer;
	}
	
	m_currCmdBuffer = [m_commandQueue commandBuffer];
	
	return m_currCmdBuffer;
}

RenderBuffer* MetalRenderer::GenerateRenderBuffer(BufferType type)
{
	return new MetalRenderBuffer(type, m_device);
}

RenderTexture* MetalRenderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
	MetalRenderTexture* texture = new MetalRenderTexture(m_device);
	if (!texture->Create(width, height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* MetalRenderer::LoadTexture(const char8* szPath)
{
	MetalRenderTexture* texture = new MetalRenderTexture(m_device);
	if (!texture->LoadFromFile(szPath))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderTexture* MetalRenderer::LoadTexture(const Image* image)
{
	MetalRenderTexture* texture = new MetalRenderTexture(m_device);
	if (!texture->LoadFromImage(image))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}

	return texture;
}

RenderProgram* MetalRenderer::GenerateRenderProgram()
{
	return new MetalShaderProgram(m_device);
}

RenderTarget* MetalRenderer::CreateRenderTarget(int32 width, int32 height)
{
	MetalRenderTarget* renderTarget = new MetalRenderTarget(m_device);
	if(!renderTarget->InitRenderPass(width, height))
	{
		SAFE_DELETE(renderTarget);
		return nullptr;
	}
	
	return renderTarget;
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
	MetalDepthStencilState* state = new MetalDepthStencilState(m_device);
	if (!state->CreateState(desc))
	{
		SAFE_DELETE(state);
		return nullptr;
	}
	return state;
}

RenderCommand* MetalRenderer::GenerateRenderCommand(RenderTarget* renderTarget)
{
	MetalRenderCommand* command = new MetalRenderCommand(this, static_cast<MetalRenderTarget*>(renderTarget));
	
	return command;
}

RenderResourceSet* MetalRenderer::GenerateRenderResuourceSet()
{
	return new MetalResourceSet(m_device);
}

RenderPipeline* MetalRenderer::GenerateRenderPipeline()
{
	return new MetalRenderPipeline(m_device);
}
