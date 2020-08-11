#include "PreCompile.h"
#include "Pharos.h"

VulkanDesciptorSet::VulkanDesciptorSet(VkDevice device, VkDescriptorPool descPool)
{
    m_device = device;
    m_descPool = descPool;
    
    m_descSetLayout = VK_NULL_HANDLE;
    m_descSet = VK_NULL_HANDLE;

    m_slotList.resize(16);
    memset(m_slotList.data(), 0xFF, m_slotList.size());
}

VulkanDesciptorSet::~VulkanDesciptorSet()
{
    if (m_descSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(m_device, m_descSetLayout, NULL);
        m_descSetLayout = VK_NULL_HANDLE;
    }

    if (m_descSet != VK_NULL_HANDLE)
    {
        vkFreeDescriptorSets(m_device, m_descPool, 1, &m_descSet);        
        m_descSet = VK_NULL_HANDLE;
    }
}

bool VulkanDesciptorSet::SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
    if (slot >= 16)
    {
        return false;
    }

    if (m_descSetLayout != VK_NULL_HANDLE && m_descSet != VK_NULL_HANDLE)
    {
        return false;
    }

    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = buffer->GetBufferType();

    if (buffType != UNIFORM_BUFFFER) return false;
    
    uint8 index = m_slotList[slot];
    if (index == 0xFF)
    {
        uint32 bindingSize = m_layoutBindings.size();
        m_slotList[slot] = bindingSize;
        index = bindingSize;

        m_layoutBindings.resize(index + 1);
        m_writeDescSets.resize(index + 1);
    } 

    VkDescriptorSetLayoutBinding& layoutBindings = m_layoutBindings[index];
    layoutBindings.binding = slot;
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings.descriptorCount = 1;
    layoutBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings.pImmutableSamplers = NULL; 

    VkWriteDescriptorSet& writeDescSet = m_writeDescSets[index];
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.pNext = NULL;
    writeDescSet.dstSet = NULL; //建立时要填充此参数
    writeDescSet.dstBinding = slot;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescSet.pImageInfo = NULL;
    writeDescSet.pBufferInfo = &bufferInfo;
    writeDescSet.pTexelBufferView = NULL;

    return true;
}

bool VulkanDesciptorSet::SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer)
{
    if (slot >= 16)
    {
        return false;
    }

    if (m_descSetLayout != VK_NULL_HANDLE && m_descSet != VK_NULL_HANDLE)
    {
        return false;
    }

    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = buffer->GetBufferType();

    if (buffType != UNIFORM_BUFFFER) return false;
    
    uint8 index = m_slotList[slot];
    if (index == 0xFF)
    {
        uint32 bindingSize = m_layoutBindings.size();
        m_slotList[slot] = bindingSize;
        index = bindingSize;

        m_layoutBindings.resize(index + 1);
        m_writeDescSets.resize(index + 1);
    } 

    VkDescriptorSetLayoutBinding& layoutBindings = m_layoutBindings[index];
    layoutBindings.binding = slot;
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings.descriptorCount = 1;
    layoutBindings.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBindings.pImmutableSamplers = NULL; 

    VkWriteDescriptorSet& writeDescSet = m_writeDescSets[index];
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.pNext = NULL;
    writeDescSet.dstSet = NULL; //建立时要填充此参数
    writeDescSet.dstBinding = slot;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescSet.pImageInfo = NULL;
    writeDescSet.pBufferInfo = &bufferInfo;
    writeDescSet.pTexelBufferView = NULL;

    return true;
}

bool VulkanDesciptorSet::SetVertexTexture(uint32 slot, RenderTexture* tex)
{
    if (slot >= 16)
    {
        return false;
    }

    if (m_descSetLayout != VK_NULL_HANDLE && m_descSet != VK_NULL_HANDLE)
    {
        return false;
    }

    return true;
}

bool VulkanDesciptorSet::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
    if (slot >= 16)
    {
        return false;
    }
    
    VulkanRenderTexture* vulkanTexture = static_cast<VulkanRenderTexture*>(tex);
    
    vulkanTexture->FlushVulkanImage();

    if (m_descSetLayout != VK_NULL_HANDLE && m_descSet != VK_NULL_HANDLE)
    {
        return false;
    }
    
    VkDescriptorImageInfo& imageInfo = vulkanTexture->GetVulkanImageInfo();    

    uint8 index = m_slotList[slot];
    if (index == 0xFF)
    {
        uint32 bindingSize = m_layoutBindings.size();
        m_slotList[slot] = bindingSize;
        index = bindingSize;

        m_layoutBindings.resize(index + 1);
        m_writeDescSets.resize(index + 1);
    } 

    VkDescriptorSetLayoutBinding& layoutBindings = m_layoutBindings[index];
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBindings.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBindings.binding = slot;
    layoutBindings.descriptorCount = 1;
    layoutBindings.pImmutableSamplers = NULL;    

    VkWriteDescriptorSet& writeDescSet = m_writeDescSets[index];
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.pNext = NULL;
    writeDescSet.dstSet = NULL; //要填充此参数
    writeDescSet.dstBinding = slot;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorCount = 1;
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;    
    writeDescSet.pImageInfo = &imageInfo;
    writeDescSet.pBufferInfo = NULL;
    writeDescSet.pTexelBufferView = NULL;    

    return true;
}

void VulkanDesciptorSet::UpdateSet()
{
    if (m_descSetLayout == VK_NULL_HANDLE)
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
}