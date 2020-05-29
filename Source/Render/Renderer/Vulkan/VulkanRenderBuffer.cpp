#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderBuffer::VulkanRenderBuffer(BufferType type, VkDevice device)
{
    m_type = type;
	m_device = device;
}

VulkanRenderBuffer::~VulkanRenderBuffer()
{
}



bool VulkanRenderBuffer::Allocate(uint32 bufSize, MemoryBuffer* buf)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = bufSize;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult res = vkCreateBuffer(m_device, &buf_info, NULL, &m_buffer);
    assert(res == VK_SUCCESS);


    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(m_device, m_buffer, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask);
    alloc_info.allocationSize = mem_reqs.size;

    res = vkAllocateMemory(m_device, &alloc_info, NULL, &m_memory);
    assert(res == VK_SUCCESS);


    uint8_t* pData = NULL;
    res = vkMapMemory(m_device, m_memory, 0, mem_reqs.size, 0, (void**)&pData);
    assert(res == VK_SUCCESS);

    void* vertexData = buf->GetPointer();
    memcpy(pData, vertexData, bufSize);

    vkUnmapMemory(m_device, m_memory);

    res = vkBindBufferMemory(m_device, m_buffer, m_memory, 0);
    assert(res == VK_SUCCESS);

	return true;
}

void VulkanRenderBuffer::CopyData(const MemoryBuffer& data, uint32 offset)
{
}

void VulkanRenderBuffer::CopyData(const void* data, uint32 len, uint32 offset)
{
}

void VulkanRenderBuffer::ApplyDevice(uint32 slot)
{

}
