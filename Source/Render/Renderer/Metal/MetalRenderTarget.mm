#include "PreCompile.h"
#include "Pharos.h"

MetalRenderTarget::MetalRenderTarget(id<MTLDevice> device)
{
	m_device = device;
	
	ZeroMemory(m_colorAttach, sizeof(m_colorAttach));
	m_depthAttach = nullptr;
	m_stencilAttach = nullptr;
}

MetalRenderTarget::~MetalRenderTarget(void)
{
}

bool MetalRenderTarget::InitRenderPass(int32 width, int32 height)
{
	m_passDesc = [MTLRenderPassDescriptor renderPassDescriptor];
	//m_passDesc.colorAttachments[0].clearColor = MTLClearColorMake(1.0, 1.0, 1.0, 1.0);
	
	m_width = width;
	m_height = height;
	
	return true;
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

RenderTexture* MetalRenderTarget::GenerateColorAttach(uint32 slot, EPixelFormat fmt)
{
	MetalRenderTexture* texture = new MetalRenderTexture(m_device);
	if(!texture->MakeColorAttach(m_width, m_height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}
	
	m_colorAttach[slot] = texture;
	
	m_passDesc.colorAttachments[0].texture = texture->GetMetalTexture();
	m_passDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
	m_passDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
	
	return texture;
}

void MetalRenderTarget::SetColorAttach(uint32 slot, RenderTexture* tex)
{
	
}

RenderTexture* MetalRenderTarget::GenerateDepthAttach(EPixelFormat fmt)
{
	MetalRenderTexture* texture = new MetalRenderTexture(m_device);
	if(!texture->MakeDepthAttach(m_width, m_height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}
	
	m_depthAttach = texture;
	
	m_passDesc.depthAttachment.texture = texture->GetMetalTexture();
	m_passDesc.depthAttachment.loadAction = MTLLoadActionClear;
	m_passDesc.depthAttachment.clearDepth = 1.0;
	m_passDesc.depthAttachment.storeAction = MTLStoreActionStore;
	
	return nullptr;
}

void MetalRenderTarget::SetDepthAttach(RenderTexture* tex)
{
	
}

RenderTexture* MetalRenderTarget::GenerateStencilAttach(EPixelFormat fmt)
{
	//	m_passDesc.stencilAttachment.texture = texture;
	//	m_passDesc.stencilAttachment.loadAction = MTLLoadActionClear;
	//	m_passDesc.stencilAttachment.clearStencil = 0;
	//	m_passDesc.stencilAttachment.storeAction = MTLStoreActionStore;
	
	return nullptr;
}

void MetalRenderTarget::SetStencilAttach(RenderTexture* tex)
{
	
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
	if(m_depthAttach != nullptr)
	{
		return m_depthAttach->GetFormat();
	}
	
	return EPF_END;
}

EPixelFormat MetalRenderTarget::GetStencilAttachFormat()
{
	if(m_stencilAttach != nullptr)
	{
		return m_stencilAttach->GetFormat();
	}
	
	return EPF_END;
}
