#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderCommand::VulkanRenderCommand(VkDevice device, VkCommandBuffer cmdBuf, VulkanRenderTarget* renderTarget)
{
    m_device = device;
	m_cmdBuf = cmdBuf;
	m_renderTarget = renderTarget;

    m_pipelineLayout = VK_NULL_HANDLE;

    m_descPool = VK_NULL_HANDLE;
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
        descriptor_pool.maxSets = 1;
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

    if (buffer->GetBufferType() == UNIFORM_BUFFFER)
    {
        VulkanUniformBuffer* vulkanBuffer = static_cast<VulkanUniformBuffer*>(buffer);

        vulkanBuffer->Apply(m_descPool, slot);

        VkDescriptorSet descSet = vulkanBuffer->GetDescriptorSet();
        VkDescriptorSetLayout descSetLayout = vulkanBuffer->GetDescriptorSetLayout();

        m_setLayouts.push_back(descSetLayout);
        m_setList.push_back(descSet);
    }
    else
    {
        VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

        vulkanBuffer->Apply(m_cmdBuf, slot);
    }
}

void VulkanRenderCommand::SetFragmentBuffer(uint32 slot, RenderBuffer *buffer)
{
}

void VulkanRenderCommand::SetFragmentTexture(uint32 slot, RenderTexture *tex)
{
}

#define NUM_VIEWPORTS 1
#define NUM_SCISSORS NUM_VIEWPORTS

void VulkanRenderCommand::SetPipeline(RenderPipeline *pipeline)
{
    VkViewport viewport;
    viewport.width = (float)m_renderTarget->GetWidth();
    viewport.height = (float)m_renderTarget->GetHeight();
    viewport.minDepth = (float)0.0f;
    viewport.maxDepth = (float)1.0f;
    viewport.x = 0;
    viewport.y = 0;
    vkCmdSetViewport(m_cmdBuf, 0, NUM_VIEWPORTS, &viewport);

    VkRect2D scissor;
    scissor.extent.width =  (float)m_renderTarget->GetWidth();
    scissor.extent.height = (float)m_renderTarget->GetHeight();
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vkCmdSetScissor(m_cmdBuf, 0, NUM_SCISSORS, &scissor);

    m_currentPipeline = static_cast<VulkanRenderPipeline*>(pipeline);

}

void VulkanRenderCommand::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
    if (m_pipelineLayout == VK_NULL_HANDLE)
    {
        /* Now use the descriptor layout to create a pipeline layout */
        VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
        pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pPipelineLayoutCreateInfo.pNext = NULL;
        pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
        pPipelineLayoutCreateInfo.setLayoutCount = m_setLayouts.size();
        pPipelineLayoutCreateInfo.pSetLayouts = m_setLayouts.data();

        VkResult res = vkCreatePipelineLayout(m_device, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
        assert(res == VK_SUCCESS);
    }

    //建立pipeline
    m_currentPipeline->SetRenderTarget(m_pipelineLayout, m_renderTarget);

    VkPipeline vulkan_pipeline = m_currentPipeline->GetVulkanPipeline();
    vkCmdBindPipeline(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline);

    uint32 descSetSize = m_setList.size();
    if (descSetSize > 0)
    {
        vkCmdBindDescriptorSets(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, descSetSize, m_setList.data(), 0, NULL);
    }

    vkCmdDraw(m_cmdBuf, count, 1, start, 0);

    m_setLayouts.clear();
    m_setList.clear();
}

void VulkanRenderCommand::EndCommand()
{
	vkCmdEndRenderPass(m_cmdBuf);
    VkResult res = vkEndCommandBuffer(m_cmdBuf);
	assert(res == VK_SUCCESS);
}