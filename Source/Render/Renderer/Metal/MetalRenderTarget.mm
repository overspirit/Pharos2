#include "PreCompile.h"
#include "Pharos.h"

MetalRenderTarget::MetalRenderTarget(id<MTLDevice> device, int32 width, int32 height)
{
	m_device = device;
	
	ZeroMemory(m_colorAttach, sizeof(m_colorAttach));
	m_depthStencilAttach = nullptr;
    
    m_passDesc = [MTLRenderPassDescriptor renderPassDescriptor];
    
    m_width = width;
    m_height = height;
}

MetalRenderTarget::~MetalRenderTarget(void)
{
}

void MetalRenderTarget::SetClear(Color4 color, float32 depth, uint32 stencil)
{
	Math::CColor4f clearColor(color);
	MTLClearColor metalClearColor = MTLClearColorMake(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	
	for(int i = 0; i < 8; i++)
	{
		MTLRenderPassColorAttachmentDescriptor* colorAttach = m_passDesc.colorAttachments[i];
		if(colorAttach != nil)
		{
			colorAttach.clearColor = metalClearColor;
		}
	}
	
	if(m_passDesc.depthAttachment != nil)
	{
		m_passDesc.depthAttachment.clearDepth = depth;
	}
	
	if(m_passDesc.stencilAttachment != nil)
	{
		m_passDesc.stencilAttachment.clearStencil = stencil;
	}
}

void MetalRenderTarget::SetColorAttach(uint32 slot, RenderTexture* tex)
{
    MetalRenderTexture* texture = static_cast<MetalRenderTexture*>(tex);
    
    m_colorAttach[slot] = texture;
    
    m_passDesc.colorAttachments[slot].texture = texture->GetMetalTexture();
    m_passDesc.colorAttachments[slot].loadAction = MTLLoadActionClear;
    m_passDesc.colorAttachments[slot].storeAction = MTLStoreActionStore;
}

RenderTexture* MetalRenderTarget::GetColorAttachTexture(uint32 slot)
{
    return m_colorAttach[slot];
}

void MetalRenderTarget::SetDepthStencilAttach(RenderTexture* tex)
{
    MetalRenderTexture* texture = static_cast<MetalRenderTexture*>(tex);
    
    m_depthStencilAttach = texture;
    
    m_passDesc.depthAttachment.texture = texture->GetMetalTexture();
    m_passDesc.depthAttachment.loadAction = MTLLoadActionClear;
    m_passDesc.depthAttachment.clearDepth = 1.0;
    m_passDesc.depthAttachment.storeAction = MTLStoreActionStore;
    
    m_passDesc.stencilAttachment.texture = texture->GetMetalTexture();
    m_passDesc.stencilAttachment.loadAction = MTLLoadActionClear;
    m_passDesc.stencilAttachment.clearStencil = 0;
    m_passDesc.stencilAttachment.storeAction = MTLStoreActionStore;
}

RenderTexture* MetalRenderTarget::GetDepthStencilAttachTexture()
{
    return m_depthStencilAttach;
}

EPixelFormat MetalRenderTarget::GetColorAttachFormat(uint32 slot)
{
	if(m_colorAttach[slot] != nullptr)
	{
		return m_colorAttach[slot]->GetFormat();
	}

	return EPF_END;
}

EPixelFormat MetalRenderTarget::GetDepthAttachFormat()
{
	if(m_depthStencilAttach != nullptr)
	{
		return m_depthStencilAttach->GetFormat();
	}

	return EPF_END;
}
