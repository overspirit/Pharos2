#include "PreCompile.h"
#include "Pharos.h"

VulkanDepthStencilState::VulkanDepthStencilState()
{
}

VulkanDepthStencilState::~VulkanDepthStencilState(void)
{
}

bool VulkanDepthStencilState::CreateState(const DepthStencilStateDesc& desc)
{    
    m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_depthStencilState.pNext = NULL;
    m_depthStencilState.flags = 0;
    m_depthStencilState.depthTestEnable = VK_TRUE;
    m_depthStencilState.depthWriteEnable = VK_TRUE;
    m_depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    m_depthStencilState.depthBoundsTestEnable = VK_FALSE;
    m_depthStencilState.stencilTestEnable = VK_FALSE;
    m_depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
    m_depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
    m_depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
    m_depthStencilState.back.compareMask = 0;
    m_depthStencilState.back.reference = 0;
    m_depthStencilState.back.depthFailOp = VK_STENCIL_OP_KEEP;
    m_depthStencilState.back.writeMask = 0;
    m_depthStencilState.minDepthBounds = 0;
    m_depthStencilState.maxDepthBounds = 0;
    m_depthStencilState.stencilTestEnable = VK_FALSE;
    m_depthStencilState.front = m_depthStencilState.back;

	return true;
}

RenderDepthStencilState* VulkanDepthStencilState::Clone()
{
    return NULL;
}

void VulkanDepthStencilState::ApplyDevice()
{
}
