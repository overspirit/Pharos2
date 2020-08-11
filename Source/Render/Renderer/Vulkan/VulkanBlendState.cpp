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
    const static VkBlendFactor BLEND_FUNC_MAP[] = {
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_SRC_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        VK_BLEND_FACTOR_SRC_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_FACTOR_DST_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        VK_BLEND_FACTOR_DST_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA
    };

    const static VkBlendOp BLEND_OP_MAP[] = {
		VK_BLEND_OP_ADD,
        VK_BLEND_OP_SUBTRACT,
        VK_BLEND_OP_REVERSE_SUBTRACT,
        VK_BLEND_OP_MIN,
        VK_BLEND_OP_MAX
		};

    m_attaState[0].colorWriteMask = 0xf;
    m_attaState[0].blendEnable = desc.blendEnable ? VK_TRUE : VK_FALSE;
    m_attaState[0].alphaBlendOp = BLEND_OP_MAP[desc.blendOpAlpha];//VK_BLEND_OP_ADD;
    m_attaState[0].colorBlendOp = BLEND_OP_MAP[desc.blendOp];//K_BLEND_OP_ADD;
    m_attaState[0].srcColorBlendFactor = BLEND_FUNC_MAP[desc.srcBlend];//VK_BLEND_FACTOR_SRC_ALPHA;
    m_attaState[0].dstColorBlendFactor = BLEND_FUNC_MAP[desc.destBlend];//VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    m_attaState[0].srcAlphaBlendFactor = BLEND_FUNC_MAP[desc.srcBlendAlpha];//VK_BLEND_FACTOR_SRC_ALPHA;
    m_attaState[0].dstAlphaBlendFactor = BLEND_FUNC_MAP[desc.destBlendAlpha];//VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

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
