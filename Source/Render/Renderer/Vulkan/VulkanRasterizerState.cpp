#include "PreCompile.h"
#include "Pharos.h"

VulkanRasterizerState::VulkanRasterizerState()
{
}

VulkanRasterizerState:: ~VulkanRasterizerState(void)
{
}

bool VulkanRasterizerState::CreateState(const RasterizerStateDesc& desc)
{   
    m_rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_rasterState.pNext = NULL;
    m_rasterState.flags = 0;
    m_rasterState.polygonMode = VK_POLYGON_MODE_FILL;
    m_rasterState.cullMode = VK_CULL_MODE_BACK_BIT;
    m_rasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    m_rasterState.depthClampEnable = VK_FALSE;
    m_rasterState.rasterizerDiscardEnable = VK_FALSE;
    m_rasterState.depthBiasEnable = VK_FALSE;
    m_rasterState.depthBiasConstantFactor = 0;
    m_rasterState.depthBiasClamp = 0;
    m_rasterState.depthBiasSlopeFactor = 0;
    m_rasterState.lineWidth = 1.0f;

	return true;
}

RenderRasterizerState* VulkanRasterizerState::Clone()
{
    return NULL;
}

void VulkanRasterizerState::ApplyDevice()
{
}
