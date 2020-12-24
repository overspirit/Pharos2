#include "PreCompile.h"
#include "Pharos.h"

MetalResourceSet::MetalResourceSet(id<MTLDevice> device)
{
	m_device = device;
}

MetalResourceSet::~MetalResourceSet()
{
}

bool MetalResourceSet::SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
	m_vertUniformBufList.resize(slot + 1);
	
	m_vertUniformBufList[slot] = static_cast<MetalRenderBuffer*>(buffer);
    
    return true;
}

bool MetalResourceSet::SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
	m_fragUniformBufList.resize(slot + 1);
	
	m_fragUniformBufList[slot] = static_cast<MetalRenderBuffer*>(buffer);
    
    return true;
}

bool MetalResourceSet::SetVertexTexture(uint32 slot, RenderTexture* tex)
{
	m_vertTexList.resize(slot + 1);
	
	m_vertTexList[slot] = static_cast<MetalRenderTexture*>(tex);
    
    return true;
}

bool MetalResourceSet::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
	m_fragTexList.resize(slot + 1);
	
	m_fragTexList[slot] = static_cast<MetalRenderTexture*>(tex);
    
    return true;
}

void MetalResourceSet::ApplyResourceSet(id <MTLRenderCommandEncoder> renderEncoder)
{
	for (uint32 i = 0; i < m_vertUniformBufList.size(); i++)
	{
        if (m_vertUniformBufList[i] != NULL)
        {
            m_vertUniformBufList[i]->ApplyVertexUniformBuffer(i, renderEncoder);
        }
	}
	
	for (uint32 i = 0; i < m_fragUniformBufList.size(); i++)
	{
        if (m_fragUniformBufList[i] != NULL)
        {
            m_fragUniformBufList[i]->ApplyFragmentUniformBuffer(i, renderEncoder);
        }
	}
	
	for (uint32 i = 0; i < m_vertTexList.size(); i++)
	{
        if (m_vertTexList[i] != NULL)
        {
            m_vertTexList[i]->ApplyVertexTexture(i, renderEncoder);
        }
	}
	
	for (uint32 i = 0; i < m_fragTexList.size(); i++)
	{
        if (m_fragTexList[i] != NULL)
        {
            m_fragTexList[i]->ApplyFragmentTexture(i, renderEncoder);
        }
	}
}
