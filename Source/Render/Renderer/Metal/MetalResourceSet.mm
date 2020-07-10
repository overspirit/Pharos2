#include "PreCompile.h"
#include "Pharos.h"

MetalResourceSet::MetalResourceSet(id<MTLDevice> device)
{
	m_device = device;
}

MetalResourceSet::~MetalResourceSet()
{
}

void MetalResourceSet::SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
	m_vertUniformBufList.resize(slot + 1);
	
	m_vertUniformBufList[slot] = static_cast<MetalRenderBuffer*>(buffer);
}

void MetalResourceSet::SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
	m_fragUniformBufList.resize(slot + 1);
	
	m_fragUniformBufList[slot] = static_cast<MetalRenderBuffer*>(buffer);
}

void MetalResourceSet::SetVertexTexture(uint32 slot, RenderTexture* tex)
{
	m_vertTexList.resize(slot + 1);
	
	m_vertTexList[slot] = static_cast<MetalRenderTexture*>(tex);
}

void MetalResourceSet::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
	m_fragTexList.resize(slot + 1);
	
	m_fragTexList[slot] = static_cast<MetalRenderTexture*>(tex);
}

bool MetalResourceSet::UpdateSet()
{
	return true;
}

void MetalResourceSet::ApplyResourceSet(id <MTLRenderCommandEncoder> renderEncoder)
{
	for (uint32 i = 0; i < m_vertUniformBufList.size(); i++)
	{
		m_vertUniformBufList[i]->ApplyVertexUniformBuffer(i, renderEncoder);
	}
	
	for (uint32 i = 0; i < m_fragUniformBufList.size(); i++)
	{
		m_fragUniformBufList[i]->ApplyVertexUniformBuffer(i, renderEncoder);
	}
	
	for (uint32 i = 0; i < m_vertTexList.size(); i++)
	{
		m_vertTexList[i]->ApplyVertexTexture(i, renderEncoder);
	}
	
	for (uint32 i = 0; i < m_fragTexList.size(); i++)
	{
		m_fragTexList[i]->ApplyFragmentTexture(i, renderEncoder);
	}
}
