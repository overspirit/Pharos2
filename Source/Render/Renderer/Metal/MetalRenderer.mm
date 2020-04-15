#include "PreCompile.h"
#include "Pharos.h"

MetalRenderer::MetalRenderer(void)
{
}

MetalRenderer::~MetalRenderer(void)
{
}

bool MetalRenderer::Init()
{
	return false;
}

void MetalRenderer::Destroy()
{
}

bool MetalRenderer::Create(const DeviceConfig& cfg)
{
	return true;
}

void MetalRenderer::BindFrameBuffer(RenderFrameBuffer* frameBuf)
{
}

void MetalRenderer::BindLayout(RenderLayout* layout)
{
}

void MetalRenderer::BindShaderData(uint32 slot, RenderShaderData* data)
{
}

void MetalRenderer::BindTexture(uint32 slot, RenderTexture* tex)
{
}

void MetalRenderer::BindProgram(RenderProgram* program)
{
}

void MetalRenderer::BindBlendState(RenderBlendState* state)
{
}

void MetalRenderer::BindRasterizerState(RenderRasterizerState* state)
{
}

void MetalRenderer::BindDepthStencilState(RenderDepthStencilState* state)
{
}

void MetalRenderer::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
}

bool MetalRenderer::Present()
{
    return true;
}

RenderProgram* MetalRenderer::GenerateRenderProgram()
{
    return NULL;
}

RenderLayout* MetalRenderer::GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf)
{
    return NULL;
}

RenderTexture* MetalRenderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
    return NULL;
}

RenderTexture* MetalRenderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
    return NULL;
}

RenderTexture* MetalRenderer::CreateDepthTexture(int32 width, int32 height)
{
    return NULL;
}

RenderTexture* MetalRenderer::LoadTexture(const char8* szPath)
{
    return NULL;
}

RenderTexture* MetalRenderer::LoadTexture(const Image* image)
{
    return NULL;
}

RenderShaderData* MetalRenderer::CreateShaderData()
{
    return NULL;
}

RenderFrameBuffer* MetalRenderer::CreateFrameBuffer(int32 width, int32 height)
{
    return NULL;
}

RenderSamplerState* MetalRenderer::CreateSampleState(const SamplerStateDesc& desc)
{
    return NULL;
}

RenderBlendState* MetalRenderer::CreateBlendState(const BlendStateDesc& desc)
{
    return NULL;
}

RenderRasterizerState* MetalRenderer::CreateRasterizerState(const RasterizerStateDesc& desc)
{
    return NULL;
}

RenderDepthStencilState* MetalRenderer::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
    return NULL;
}
