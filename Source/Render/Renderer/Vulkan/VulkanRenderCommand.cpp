#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderCommand::VulkanRenderCommand(VkDevice device, VkCommandBuffer cmdBuf, VulkanRenderTarget* renderTarget)
{
    m_device = device;
    m_cmdBuf = cmdBuf;
    m_renderTarget = renderTarget;

    m_pipelineLayout = VK_NULL_HANDLE;

    m_descPool = VK_NULL_HANDLE;

    m_viewport.width = (float)m_renderTarget->GetWidth();
    m_viewport.height = (float)m_renderTarget->GetHeight();
    m_viewport.minDepth = (float)0.0f;
    m_viewport.maxDepth = (float)1.0f;
    m_viewport.x = 0;
    m_viewport.y = 0;

    m_scissor.extent.width =  (float)m_renderTarget->GetWidth();
    m_scissor.extent.height = (float)m_renderTarget->GetHeight();
    m_scissor.offset.x = 0;
    m_scissor.offset.y = 0;
}

VulkanRenderCommand::~VulkanRenderCommand()
{
}

void VulkanRenderCommand::BeginCommand()
{
    if (m_descPool == VK_NULL_HANDLE)
    {
        VkDescriptorPoolSize type_count[2];
        type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        type_count[0].descriptorCount = 1;
        type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        type_count[1].descriptorCount = 1;

        VkDescriptorPoolCreateInfo descriptor_pool = {};
        descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool.pNext = NULL;
        descriptor_pool.maxSets = 1000;
        descriptor_pool.poolSizeCount = 2;
        descriptor_pool.pPoolSizes = type_count;

        VkResult res = vkCreateDescriptorPool(m_device, &descriptor_pool, NULL, &m_descPool);
        assert(res == VK_SUCCESS);
    }

    VkRenderPassBeginInfo beginPassInfo = m_renderTarget->GetRenderPassBeginInfo();

    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;

    VkResult res = vkBeginCommandBuffer(m_cmdBuf, &cmd_buf_info);
    assert(res == VK_SUCCESS);


    vkCmdBeginRenderPass(m_cmdBuf, &beginPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderCommand::SetDebugLabel(const char8* label)
{

}

void VulkanRenderCommand::SetVertexBuffer(uint32 slot, RenderBuffer *buffer)
{
    if (buffer == nullptr) return;

    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = buffer->GetBufferType();

    switch(buffType)
    {
        case UNIFORM_BUFFFER:
        {
            VkDescriptorSetLayoutBinding layoutBindings = {};
            layoutBindings.binding = slot;
            layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            layoutBindings.descriptorCount = 1;
            layoutBindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            layoutBindings.pImmutableSamplers = NULL;

            m_layoutBindings.push_back(layoutBindings);

            VkWriteDescriptorSet writeDescSet = {};
            writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescSet.pNext = NULL;
            writeDescSet.dstSet = NULL;     //建立时要填充此参数
            writeDescSet.descriptorCount = 1;
            writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescSet.pBufferInfo = &bufferInfo;
            writeDescSet.dstArrayElement = 0;
            writeDescSet.dstBinding = slot;

            m_writeDescSets.push_back(writeDescSet);
        }
        break;
        case VERTEX_BUFFER:
        {
            const VkDeviceSize offsets[1] = {0};
            vkCmdBindVertexBuffers(m_cmdBuf, slot, 1, &bufferInfo.buffer, offsets);
        }
        break;
        case INDICES_BUFFER:
        {
            //vkCmdBindIndexBuffer();
        }
    }
}

void VulkanRenderCommand::SetFragmentBuffer(uint32 slot, RenderBuffer *buffer)
{
}

void VulkanRenderCommand::SetFragmentTexture(uint32 slot, RenderTexture *tex)
{
    VulkanRenderTexture* vulkanTexture = static_cast<VulkanRenderTexture*>(tex);

    VkDescriptorSetLayoutBinding layoutBindings;
    layoutBindings.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBindings.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBindings.binding = slot;
    layoutBindings.descriptorCount = 1;
    layoutBindings.pImmutableSamplers = NULL;

    m_layoutBindings.push_back(layoutBindings);


    VkWriteDescriptorSet writeDescSet = {};
    writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescSet.dstSet = NULL;     //要填充此参数
    writeDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescSet.dstBinding = slot;
    writeDescSet.pImageInfo = &vulkanTexture->GetVulkanImageInfo();
    writeDescSet.descriptorCount = 1;

    m_writeDescSets.push_back(writeDescSet);
}

void VulkanRenderCommand::SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType)
{

}

void VulkanRenderCommand::SetPipeline(RenderPipeline *pipeline)
{
    m_currentPipeline = static_cast<VulkanRenderPipeline*>(pipeline);
}

void VulkanRenderCommand::SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth)
{
    m_viewport.width = (float)viewRect.GetWidth();
    m_viewport.height = (float)viewRect.GetHeight();
    m_viewport.minDepth = minDepth;
    m_viewport.maxDepth = maxDepth;
    m_viewport.x = (float)viewRect.left;
    m_viewport.y = (float)viewRect.top;
}

void VulkanRenderCommand::SetScissorRect(const Rect2Di& scissorRect)
{
    m_scissor.extent.width = scissorRect.GetWidth();
    m_scissor.extent.height = scissorRect.GetHeight();
    m_scissor.offset.x = scissorRect.left;
    m_scissor.offset.y = scissorRect.top;
}

void VulkanRenderCommand::DrawPrimitives(DrawType type, uint32 start, uint32 count)
{
    CHECK_ENUM(0, EDT_POINTLIST);
    CHECK_ENUM(1, EDT_LINELIST);
    CHECK_ENUM(2, EDT_LINESTRIP);
    CHECK_ENUM(3, EDT_TRIANGLELIST);
    CHECK_ENUM(4, EDT_TRIANGLESTRIP);

    const static VkPrimitiveTopology prim[] =
    {
        VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    };

    if (m_pipelineLayout == VK_NULL_HANDLE) //todo: 或者binding发生变化
    {
        VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
        descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_layout.pNext = NULL;
        descriptor_layout.flags = 0;
        descriptor_layout.bindingCount = m_layoutBindings.size();
        descriptor_layout.pBindings = m_layoutBindings.data();

        VkResult res = vkCreateDescriptorSetLayout(m_device, &descriptor_layout, NULL, &m_descSetLayout);
        assert(res == VK_SUCCESS);

        VkDescriptorSetAllocateInfo alloc_info[1];
        alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info[0].pNext = NULL;
        alloc_info[0].descriptorPool = m_descPool;
        alloc_info[0].descriptorSetCount = 1;
        alloc_info[0].pSetLayouts = &m_descSetLayout;

        res = vkAllocateDescriptorSets(m_device, alloc_info, &m_descSet);
        assert(res == VK_SUCCESS);

        /* Now use the descriptor layout to create a pipeline layout */
        VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
        pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pPipelineLayoutCreateInfo.pNext = NULL;
        pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
        pPipelineLayoutCreateInfo.setLayoutCount = 1;
        pPipelineLayoutCreateInfo.pSetLayouts = &m_descSetLayout;

        res = vkCreatePipelineLayout(m_device, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
        assert(res == VK_SUCCESS);

        for (auto& writeDescSet : m_writeDescSets)
        {
            writeDescSet.dstSet = m_descSet;
        }

        vkUpdateDescriptorSets(m_device, m_writeDescSets.size(), m_writeDescSets.data(), 0, NULL);
    }

    //建立pipeline
    VkRenderPass renderPass = m_renderTarget->GetCurrRenderPass();
    VkPipeline vulkanPipeline = m_currentPipeline->GetVulkanPipeline(prim[type], m_pipelineLayout, renderPass);

    vkCmdBindPipeline(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

    if (m_descSet != NULL)
    {
        vkCmdBindDescriptorSets(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descSet, 0, NULL);
    }

    vkCmdSetViewport(m_cmdBuf, 0, 1, &m_viewport);
    vkCmdSetScissor(m_cmdBuf, 0, 1, &m_scissor);

    vkCmdDraw(m_cmdBuf, count, 1, start, 0);

    m_layoutBindings.clear();
    m_writeDescSets.clear();
}

void VulkanRenderCommand::DrawIndexedPrimitives(DrawType type, uint32 indexCount, uint32 indexOffset)
{

}

void VulkanRenderCommand::EndCommand()
{
    vkCmdEndRenderPass(m_cmdBuf);
    VkResult res = vkEndCommandBuffer(m_cmdBuf);
    assert(res == VK_SUCCESS);
}