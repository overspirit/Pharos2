#include "PreCompile.h"
#include "Pharos.h"

MetalViewRenderTarget::MetalViewRenderTarget(id<MTLDevice> device, MTKView* view)
: MetalRenderTarget(device, view.bounds.size.width, view.bounds.size.height)
{
	m_view = view;	
}

MetalViewRenderTarget::~MetalViewRenderTarget(void)
{
}

MTLRenderPassDescriptor* MetalViewRenderTarget::GetMetalPassDescriptor()
{
	m_passDesc = m_view.currentRenderPassDescriptor;
    
	return m_passDesc;	
}

void MetalViewRenderTarget::SetClear(Color4 color, float32 depth, uint32 stencil)
{
	Math::CColor4f clearColor(color);
	MTLClearColor metalClearColor = MTLClearColorMake(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	
	m_view.clearColor = metalClearColor;
	m_view.clearDepth = depth;
	m_view.clearStencil = stencil;
}

void MetalViewRenderTarget::SetColorAttach(uint32 slot, RenderTexture* tex)
{
    
}

RenderTexture* MetalViewRenderTarget::GetColorAttachTexture(uint32 slot)
{
    return NULL;
}

void MetalViewRenderTarget::SetDepthStencilAttach(RenderTexture* tex)
{
    
}

RenderTexture* MetalViewRenderTarget::GetDepthStencilAttachTexture()
{
    return NULL;
}

EPixelFormat MetalViewRenderTarget::GetColorAttachFormat(uint32 slot)
{
	MTLPixelFormat metalPixelFormat = m_passDesc.colorAttachments[slot].texture.pixelFormat;
	return MetalFormat2PixelFormat(metalPixelFormat);
}

EPixelFormat MetalViewRenderTarget::GetDepthAttachFormat()
{
	MTLPixelFormat metalPixelFormat = m_passDesc.depthAttachment.texture.pixelFormat;
	return MetalFormat2PixelFormat(metalPixelFormat);
}
