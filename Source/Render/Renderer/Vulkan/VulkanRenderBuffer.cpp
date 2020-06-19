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
    static VkBufferUsageFlagBits flags[BUFFER_TYPE_NUM];
    flags[UNIFORM_BUFFFER] = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	flags[VERTEX_BUFFER] = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	flags[INDICES_BUFFER] = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = flags[m_type];
    buf_info.size = bufSize;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;

    VkBuffer buffer;
    VkResult res = vkCreateBuffer(m_device, &buf_info, NULL, &buffer);
    assert(res == VK_SUCCESS);

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(m_device, buffer, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask);
    alloc_info.allocationSize = mem_reqs.size;

    res = vkAllocateMemory(m_device, &alloc_info, NULL, &m_memory);
    assert(res == VK_SUCCESS);

    if (buf != nullptr)
    {
        uint8_t *pData = NULL;
        res = vkMapMemory(m_device, m_memory, 0, mem_reqs.size, 0, (void **)&pData);
        assert(res == VK_SUCCESS);

        void *vertexData = buf->GetPointer();
        memcpy(pData, vertexData, bufSize);

        vkUnmapMemory(m_device, m_memory);
    }

    res = vkBindBufferMemory(m_device, buffer, m_memory, 0);
    assert(res == VK_SUCCESS);  

    m_size = bufSize;
    m_reqSize = mem_reqs.size;

    m_bufferInfo.buffer = buffer;
    m_bufferInfo.offset = 0;
    m_bufferInfo.range = bufSize;

	return true;
}

void VulkanRenderBuffer::CopyData(const MemoryBuffer& data, uint32 offset)
{
}

void VulkanRenderBuffer::CopyData(const void* data, uint32 len, uint32 offset)
{
    if (data != nullptr)
    {
        uint8_t *pData = NULL;
        VkResult res = vkMapMemory(m_device, m_memory, 0, m_reqSize, 0, (void **)&pData);
        assert(res == VK_SUCCESS);

        memcpy(pData, data, len);

        vkUnmapMemory(m_device, m_memory);
    }
}
