#include "PreCompile.h"
#include "Pharos.h"

VulkanUniformBuffer::VulkanUniformBuffer(BufferType type, VkDevice device)
: VulkanRenderBuffer(type, device)
{
    m_descSetLayout = nullptr;
    m_descSet = nullptr;
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
}

void VulkanUniformBuffer::Apply(VkDescriptorPool descPool, uint32 slot)
{
    if (m_descSet == nullptr)
    {
        VkDescriptorSetLayoutBinding layout_bindings;
        layout_bindings.binding = slot;
        layout_bindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layout_bindings.descriptorCount = 1;
        layout_bindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        layout_bindings.pImmutableSamplers = NULL;

        /* Next take layout bindings and use them to create a descriptor set layout
     */
        VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
        descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_layout.pNext = NULL;
        descriptor_layout.flags = 0;
        descriptor_layout.bindingCount = 1;
        descriptor_layout.pBindings = &layout_bindings;

        VkResult res = vkCreateDescriptorSetLayout(m_device, &descriptor_layout, NULL, &m_descSetLayout);
        assert(res == VK_SUCCESS);

        VkDescriptorSetAllocateInfo alloc_info[1];
        alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info[0].pNext = NULL;
        alloc_info[0].descriptorPool = descPool;
        alloc_info[0].descriptorSetCount = 1;
        alloc_info[0].pSetLayouts = &m_descSetLayout;

        res = vkAllocateDescriptorSets(m_device, alloc_info, &m_descSet);
        assert(res == VK_SUCCESS);
    }

    VkDescriptorBufferInfo  buffer_info;
    buffer_info.buffer = m_buffer;
    buffer_info.offset = 0;
    buffer_info.range = m_size;

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = NULL;
    write.dstSet = m_descSet;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.pBufferInfo = &buffer_info;
    write.dstArrayElement = 0;
    write.dstBinding = 0;

    vkUpdateDescriptorSets(m_device, 1, &write, 0, NULL);
}
