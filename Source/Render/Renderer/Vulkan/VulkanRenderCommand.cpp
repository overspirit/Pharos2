#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderCommand::VulkanRenderCommand(VkDevice device, VkCommandBuffer cmdBuf, VulkanRenderTarget* renderTarget)
{
    m_device = device;
	m_cmdBuf = cmdBuf;
	m_renderTarget = renderTarget;

    m_pipelineLayout = VK_NULL_HANDLE;
}

VulkanRenderCommand::~VulkanRenderCommand()
{
}

void VulkanRenderCommand::BeginCommand()
{
    if(m_pipelineLayout == VK_NULL_HANDLE)
    {
        CreatePipelineLayout();
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
        VulkanUniformBuffer *vulkanBuffer = static_cast<VulkanUniformBuffer*>(buffer);
    }
    else
    {
        VulkanRenderBuffer *vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);
        VkBuffer vulkan_buffer = vulkanBuffer->GetBuffer();

        const VkDeviceSize offsets[1] = {0};
        vkCmdBindVertexBuffers(m_cmdBuf, 0, 1, &vulkan_buffer, offsets);
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
    VulkanRenderPipeline* vulkanPipeline = static_cast<VulkanRenderPipeline*>(pipeline);
    vulkanPipeline->SetRenderTarget(m_pipelineLayout, m_renderTarget);

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

    VkPipeline vulkan_pipeline = vulkanPipeline->GetVulkanPipeline();
    vkCmdBindPipeline(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_pipeline);
}

void VulkanRenderCommand::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
    vkCmdDraw(m_cmdBuf, count, 1, start, 0);
}

void VulkanRenderCommand::EndCommand()
{
	vkCmdEndRenderPass(m_cmdBuf);
    VkResult res = vkEndCommandBuffer(m_cmdBuf);
	assert(res == VK_SUCCESS);
}

void VulkanRenderCommand::CreatePipelineLayout()
{
    bool use_texture = false;
    const int NUM_DESCRIPTOR_SETS = 1;
    VkDescriptorSetLayoutCreateFlags descSetLayoutCreateFlags = 0;

    VkDescriptorSetLayoutBinding layout_bindings[2];
    layout_bindings[0].binding = 0;
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_bindings[0].descriptorCount = 1;
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layout_bindings[0].pImmutableSamplers = NULL;

    if (use_texture) {
        layout_bindings[1].binding = 1;
        layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layout_bindings[1].descriptorCount = 1;
        layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layout_bindings[1].pImmutableSamplers = NULL;
    }

    /* Next take layout bindings and use them to create a descriptor set layout
     */
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout.pNext = NULL;
    descriptor_layout.flags = descSetLayoutCreateFlags;
    descriptor_layout.bindingCount = use_texture ? 2 : 1;
    descriptor_layout.pBindings = layout_bindings;

    m_setLayouts.resize(NUM_DESCRIPTOR_SETS);
    VkResult res = vkCreateDescriptorSetLayout(m_device, &descriptor_layout, NULL, m_setLayouts.data());
    assert(res == VK_SUCCESS);

    /* Now use the descriptor layout to create a pipeline layout */
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = m_setLayouts.size();
    pPipelineLayoutCreateInfo.pSetLayouts = m_setLayouts.data();

    res = vkCreatePipelineLayout(m_device, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
    assert(res == VK_SUCCESS);
}
