#include "PreCompile.h"
#include "Pharos.h"

MetalFrameBuffer::MetalFrameBuffer()
{
}

MetalFrameBuffer::~MetalFrameBuffer(void)
{
}

bool MetalFrameBuffer::InitFrameBuffer(int32 width, int32 height)
{
	return true;
}

void MetalFrameBuffer::ClearFrameBuffer(Color4 color, float32 depth, uint32 stencil)
{
}

RenderTexture* MetalFrameBuffer::CreateRenderTexture(uint32 slot, EPixelFormat fmt)
{
    return NULL;
}

RenderTexture* MetalFrameBuffer::CreateDepthTexture()
{
    return NULL;
}

RenderTexture* MetalFrameBuffer::GetRenderTexture(uint32 slot)
{
    return NULL;
}

RenderTexture* MetalFrameBuffer::GetDepthTexture()
{
    return NULL;
}

void MetalFrameBuffer::AttachTexture(uint32 slot, RenderTexture* tex)
{
}

void MetalFrameBuffer::ApplyDevice()
{
}
