#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderCommand::VulkanRenderCommand(VkDevice device, VkCommandBuffer cmdBuf, VulkanRenderTarget* renderTarget)
{
    m_device = device;
    m_cmdBuf = cmdBuf;
    m_renderTarget = renderTarget;

    // m_pipelineLayout = VK_NULL_HANDLE;

    // m_descPool = VK_NULL_HANDLE;

    // m_viewport.width = (float)m_renderTarget->GetWidth();
    // m_viewport.height = (float)m_renderTarget->GetHeight();
    // m_viewport.minDepth = (float)0.0f;
    // m_viewport.maxDepth = (float)1.0f;
    // m_viewport.x = 0;
    // m_viewport.y = 0;

    // m_scissor.extent.width =  (float)m_renderTarget->GetWidth();
    // m_scissor.extent.height = (float)m_renderTarget->GetHeight();
    // m_scissor.offset.x = 0;
    // m_scissor.offset.y = 0;
}

VulkanRenderCommand::~VulkanRenderCommand()
{
}

void VulkanRenderCommand::BeginCommand()
{
    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;

    VkResult res = vkBeginCommandBuffer(m_cmdBuf, &cmd_buf_info);
    assert(res == VK_SUCCESS);
    
    VkRenderPassBeginInfo beginPassInfo = m_renderTarget->GetRenderPassBeginInfo();

    vkCmdBeginRenderPass(m_cmdBuf, &beginPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderCommand::SetDebugLabel(const char8* label)
{

}

void VulkanRenderCommand::SetVertexBuffer(RenderBuffer *buffer)
{
    if (buffer == nullptr) return;

    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(buffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = buffer->GetBufferType();

    if (buffType != VERTEX_BUFFER) return;
    
    const VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(m_cmdBuf, 0, 1, &bufferInfo.buffer, offsets);
}

void VulkanRenderCommand::SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType)
{
    if (indexBuffer == nullptr) return;

    VulkanRenderBuffer* vulkanBuffer = static_cast<VulkanRenderBuffer*>(indexBuffer);

    VkDescriptorBufferInfo& bufferInfo = vulkanBuffer->GetVulkanBufferInfo();
    BufferType buffType = indexBuffer->GetBufferType();

    if (buffType != INDICES_BUFFER) return;

    static VkIndexType type[2];
	type[IET_UINT32] = VK_INDEX_TYPE_UINT32;
    type[IET_UINT16] = VK_INDEX_TYPE_UINT16;
    vkCmdBindIndexBuffer(m_cmdBuf, bufferInfo.buffer, 0, type[indexType]);    
}

void VulkanRenderCommand::SetPipeline(RenderResourceSet* resSet, RenderPipeline* pipeline)
{
    VulkanRenderPipeline* vulkanPipeline = static_cast<VulkanRenderPipeline*>(pipeline);
        
    VkRenderPass renderPass = m_renderTarget->GetCurrRenderPass();

    VkDescriptorSetLayout descLayout = NULL;
    VkDescriptorSet descSet = NULL;

    if (resSet != NULL)
    {
        VulkanDesciptorSet* vulkanDescSet = static_cast<VulkanDesciptorSet*>(resSet);
        descLayout = vulkanDescSet->GetVulkanDescriptorSetLayout();
        descSet = vulkanDescSet->GetVulkanDescriptorSet();
    }

    VkPipeline pl = vulkanPipeline->GetVulkanPipeline(descLayout, renderPass);
    VkPipelineLayout pipelineLayout = vulkanPipeline->GetVulkanPipelineLayout();

    if (pl == VK_NULL_HANDLE) return;

    vkCmdBindPipeline(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pl);    
    
    if (descSet != NULL)
    {
        vkCmdBindDescriptorSets(m_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descSet, 0, NULL);
    }
}

void VulkanRenderCommand::SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth)
{
    VkViewport viewport;
    viewport.width = (float)viewRect.GetWidth();
    viewport.height = (float)viewRect.GetHeight();
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    viewport.x = (float)viewRect.left;
    viewport.y = (float)viewRect.top;
    vkCmdSetViewport(m_cmdBuf, 0, 1, &viewport);
}

void VulkanRenderCommand::SetScissorRect(const Rect2Di& scissorRect)
{
    VkRect2D scissor;
    scissor.extent.width = scissorRect.GetWidth();
    scissor.extent.height = scissorRect.GetHeight();
    scissor.offset.x = scissorRect.left;
    scissor.offset.y = scissorRect.top;
    vkCmdSetScissor(m_cmdBuf, 0, 1, &scissor);
}

void VulkanRenderCommand::DrawPrimitives(uint32 start, uint32 count)
{
    vkCmdDraw(m_cmdBuf, count, 1, start, 0);
}

void VulkanRenderCommand::DrawIndexedPrimitives(uint32 start, uint32 count)
{    
    vkCmdDrawIndexed(m_cmdBuf, count, 1, start, 0, 0);
}

void VulkanRenderCommand::EndCommand()
{
    vkCmdEndRenderPass(m_cmdBuf);
    VkResult res = vkEndCommandBuffer(m_cmdBuf);
    assert(res == VK_SUCCESS);
}