#include "PreCompile.h"
#include "Pharos.h"

MetalRenderTarget::MetalRenderTarget()
{
}

MetalRenderTarget::~MetalRenderTarget(void)
{
}

bool MetalRenderTarget::Init(int32 width, int32 height)
{
	return true;
}

void MetalRenderTarget::Clear(Color4 color, float32 depth, uint32 stencil)
{
}

RenderTexture* MetalRenderTarget::CreateRenderTexture(uint32 slot, EPixelFormat fmt)
{
	return NULL;
}

RenderTexture* MetalRenderTarget::CreateDepthTexture()
{
	return NULL;
}

RenderTexture* MetalRenderTarget::GetRenderTexture(uint32 slot)
{
	return NULL;
}

RenderTexture* MetalRenderTarget::GetDepthTexture()
{
	return NULL;
}

void MetalRenderTarget::AttachTexture(uint32 slot, RenderTexture* tex)
{
}

void MetalRenderTarget::ApplyDevice()
{
}
