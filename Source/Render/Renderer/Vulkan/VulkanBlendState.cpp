#include "PreCompile.h"
#include "Pharos.h"

VulkanBlendState::VulkanBlendState()
{
}

VulkanBlendState::~VulkanBlendState(void)
{
}

bool VulkanBlendState::CreateState(const BlendStateDesc& desc)
{
    m_attaState[0].colorWriteMask = 0xf;
    m_attaState[0].blendEnable = VK_FALSE;
    m_attaState[0].alphaBlendOp = VK_BLEND_OP_ADD;
    m_attaState[0].colorBlendOp = VK_BLEND_OP_ADD;
    m_attaState[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attaState[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attaState[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attaState[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    m_blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_blendState.flags = 0;
    m_blendState.pNext = NULL;
    m_blendState.attachmentCount = 1;
    m_blendState.pAttachments = m_attaState;
    m_blendState.logicOpEnable = VK_FALSE;
    m_blendState.logicOp = VK_LOGIC_OP_NO_OP;
    m_blendState.blendConstants[0] = 1.0f;
    m_blendState.blendConstants[1] = 1.0f;
    m_blendState.blendConstants[2] = 1.0f;
    m_blendState.blendConstants[3] = 1.0f;


	return true;
}

RenderBlendState* VulkanBlendState::Clone()
{
    return NULL;
}

void VulkanBlendState::ApplyDevice()
{
}
