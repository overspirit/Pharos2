#include "PreCompile.h"
#include "Pharos.h"

VulkanDesciptorSet::VulkanDesciptorSet(VkDevice device, VkDescriptorPool descPool)
{
    m_device = device;
    m_descPool = descPool;
    
    m_descSetLayout = VK_NULL_HANDLE;
    m_descSet = VK_NULL_HANDLE;
}

VulkanDesciptorSet::~VulkanDesciptorSet()
{
}

void VulkanDesciptorSet::SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = buffer->GetBufferType();

    if (buffType != UNIFORM_BUFFFER) return;
    
    VkDescriptorSetLayoutBinding layoutBindings = {};
    layoutBindings.binding = slot;
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings.descriptorCount = 1;
    layoutBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings.pImmutableSamplers = NULL;

    if (slot >= m_layoutBindings.size())
    {
        m_layoutBindings.resize(slot + 1);
    }
    
    m_layoutBindings[slot] = layoutBindings;


    VkWriteDescriptorSet writeDescSet = {};
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.pNext = NULL;
    writeDescSet.dstSet = NULL; //建立时要填充此参数
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescSet.pBufferInfo = &bufferInfo;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.dstBinding = slot;

    if (slot >= m_writeDescSets.size())
    {
        m_writeDescSets.resize(slot + 1);
    }

    m_writeDescSets[slot] = writeDescSet;
}

void VulkanDesciptorSet::SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer)
{

}

void VulkanDesciptorSet::SetVertexTexture(uint32 slot, RenderTexture* tex)
{

}

void VulkanDesciptorSet::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
    VulkanRenderTexture* vulkanTexture = static_cast<VulkanRenderTexture*>(tex);

    VkDescriptorSetLayoutBinding layoutBindings;
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBindings.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBindings.binding = slot;
    layoutBindings.descriptorCount = 1;
    layoutBindings.pImmutableSamplers = NULL;

    if (slot >= m_layoutBindings.size())
    {
        m_layoutBindings.resize(slot + 1);
    }
    
    m_layoutBindings[slot] = layoutBindings;


    VkWriteDescriptorSet writeDescSet = {};
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.dstSet = NULL; //要填充此参数
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescSet.dstBinding = slot;
    writeDescSet.pImageInfo = &vulkanTexture->GetVulkanImageInfo();
    writeDescSet.descriptorCount = 1;

    if (slot >= m_writeDescSets.size())
    {
        m_writeDescSets.resize(slot + 1);
    }

    m_writeDescSets[slot] = writeDescSet;
}

bool VulkanDesciptorSet::ApplyResourceSet()
{
    if (m_descSetLayout == VK_NULL_HANDLE) //todo: 或者binding发生变化
    {
        VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
        descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_layout.pNext = NULL;
        descriptor_layout.flags = 0;
        descriptor_layout.bindingCount = m_layoutBindings.size();
        descriptor_layout.pBindings = m_layoutBindings.data();

        VkResult res = vkCreateDescriptorSetLayout(m_device, &descriptor_layout, NULL, &m_descSetLayout);
        assert(res == VK_SUCCESS); 
    }

    if (m_descSet == VK_NULL_HANDLE)
    {
        VkDescriptorSetAllocateInfo alloc_info[1];
        alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info[0].pNext = NULL;
        alloc_info[0].descriptorPool = m_descPool;
        alloc_info[0].descriptorSetCount = 1;
        alloc_info[0].pSetLayouts = &m_descSetLayout;

        VkResult res = vkAllocateDescriptorSets(m_device, alloc_info, &m_descSet);
        assert(res == VK_SUCCESS);

        for (auto &writeDescSet : m_writeDescSets)
        {
            writeDescSet.dstSet = m_descSet;
        }
        
        vkUpdateDescriptorSets(m_device, m_writeDescSets.size(), m_writeDescSets.data(), 0, NULL);
    }

    return true;
}