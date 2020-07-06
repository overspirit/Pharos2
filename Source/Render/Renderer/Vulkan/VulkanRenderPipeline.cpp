#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderPipeline::VulkanRenderPipeline(VkDevice device)
{
	m_device = device;

	m_pipeline = VK_NULL_HANDLE;

	m_rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_rasterState.pNext = NULL;
    m_rasterState.flags = 0;
    m_rasterState.polygonMode = VK_POLYGON_MODE_FILL;
    m_rasterState.cullMode = VK_CULL_MODE_BACK_BIT;//VK_CULL_MODE_FRONT_BIT;
    m_rasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    m_rasterState.depthClampEnable = VK_FALSE;
    m_rasterState.rasterizerDiscardEnable = VK_FALSE;
    m_rasterState.depthBiasEnable = VK_FALSE;
    m_rasterState.depthBiasConstantFactor = 0;
    m_rasterState.depthBiasClamp = 0;
    m_rasterState.depthBiasSlopeFactor = 0;
    m_rasterState.lineWidth = 1.0f;

	m_attachmentState[0].colorWriteMask = 0xf;
    m_attachmentState[0].blendEnable = VK_FALSE;
    m_attachmentState[0].alphaBlendOp = VK_BLEND_OP_ADD;
    m_attachmentState[0].colorBlendOp = VK_BLEND_OP_ADD;
    m_attachmentState[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attachmentState[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attachmentState[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_attachmentState[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    m_blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_blendState.flags = 0;
    m_blendState.pNext = NULL;
    m_blendState.attachmentCount = 1;
    m_blendState.pAttachments = m_attachmentState;
    m_blendState.logicOpEnable = VK_FALSE;
    m_blendState.logicOp = VK_LOGIC_OP_NO_OP;
    m_blendState.blendConstants[0] = 1.0f;
    m_blendState.blendConstants[1] = 1.0f;
    m_blendState.blendConstants[2] = 1.0f;
    m_blendState.blendConstants[3] = 1.0f;

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

	m_drawType = EDT_TRIANGLELIST;
}

VulkanRenderPipeline::~VulkanRenderPipeline()
{
}

bool VulkanRenderPipeline::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{
    m_vertexAttributes.resize(descNum);

    map<uint32, uint32> bindStride;
    
    for (uint32 i = 0; i < descNum; i++)
    {
        m_vertexAttributes[i].binding = desc[i].layoutIndex;
        m_vertexAttributes[i].location = desc[i].elementLocation;
        m_vertexAttributes[i].format = GetVulkanFormat(desc[i].elementType, desc[i].elementNum);
        m_vertexAttributes[i].offset = desc[i].elementOffset;

        bindStride[desc[i].layoutIndex] += GetVertElementSize(desc[i].elementType, desc[i].elementNum);
    }

	m_vertexBindings.clear();
	
    for (auto& iter : bindStride)
    {    
        VkVertexInputBindingDescription vertexBinding;
        vertexBinding.binding = iter.first;
        vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        vertexBinding.stride = iter.second;

        m_vertexBindings.push_back(vertexBinding);
    }

	m_vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertInputState.pNext = NULL;
	m_vertInputState.flags = 0;
	m_vertInputState.vertexBindingDescriptionCount = m_vertexBindings.size();
	m_vertInputState.pVertexBindingDescriptions = m_vertexBindings.data();
	m_vertInputState.vertexAttributeDescriptionCount = m_vertexAttributes.size();
	m_vertInputState.pVertexAttributeDescriptions = m_vertexAttributes.data();

	return true;
}

bool VulkanRenderPipeline::SetProgramShader(RenderProgram* program)
{
	VulkanShaderProgram* vulkanProgram =  static_cast<VulkanShaderProgram*>(program);

	m_shaderStages = vulkanProgram->GetShaderStage();

	return true;
}

void VulkanRenderPipeline::SetBlendState(RenderBlendState* state)
{
	//BlendStateDesc	blendDesc;
	//blendState.CreateState(blendDesc);

	VulkanBlendState* blendState = static_cast<VulkanBlendState*>(state);

	m_blendState = blendState->GetStateCreateInfo();
	VkPipelineColorBlendAttachmentState* attachState = blendState->GetAttachmentState();

	for(uint32 i = 0; i < m_blendState.attachmentCount; i++)
	{
		m_attachmentState[i] = attachState[i];
	}

	m_blendState.pAttachments = m_attachmentState;	
}

void VulkanRenderPipeline::SetRasterizerState(RenderRasterizerState* state)
{
	//RasterizerStateDesc rasterdesc;
	//rasterState.CreateState(rasterdesc);

	VulkanRasterizerState* rasterState = static_cast<VulkanRasterizerState*>(state);
	
	m_rasterState = rasterState->GetStateCreateInfo();
}

void VulkanRenderPipeline::SetDepthStencilState(RenderDepthStencilState* state)
{
	//DepthStencilStateDesc	depthStencilDesc;
	//depthStencilState.CreateState(depthStencilDesc);

    VulkanDepthStencilState* depthStencilState = static_cast<VulkanDepthStencilState*>(state);

	m_depthStencilState = depthStencilState->GetStateCreateInfo();
}


VkPipeline VulkanRenderPipeline::GetVulkanPipeline(VkDescriptorSetLayout descSetLayout, VkRenderPass renderPass)
{
	if (descSetLayout == VK_NULL_HANDLE || renderPass == VK_NULL_HANDLE)
	{
		return VK_NULL_HANDLE;
	}
	
	if (m_pipeline != VK_NULL_HANDLE) //todo: 信息发生变化要重新建立 
	{
		return m_pipeline;	
	}

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = 1;
    pPipelineLayoutCreateInfo.pSetLayouts = &descSetLayout;

    VkResult res = vkCreatePipelineLayout(m_device, &pPipelineLayoutCreateInfo, NULL, &m_pipelineLayout);
    assert(res == VK_SUCCESS);


    VkDynamicState dynamicStateEnables[2];
    dynamicStateEnables[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStateEnables[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 2;


    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = 1;
    vp.scissorCount = 1;
    vp.pScissors = NULL;
    vp.pViewports = NULL;


    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;



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

    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = prim[m_drawType];


    VkGraphicsPipelineCreateInfo pipeline;
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.pNext = NULL;
    pipeline.layout = m_pipelineLayout;
    pipeline.basePipelineHandle = VK_NULL_HANDLE;
    pipeline.basePipelineIndex = 0;
    pipeline.flags = 0;
    pipeline.pVertexInputState = &m_vertInputState;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &m_rasterState;
    pipeline.pColorBlendState = &m_blendState;
    pipeline.pTessellationState = NULL;
    pipeline.pMultisampleState = &ms;
    pipeline.pDynamicState = &dynamicState;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &m_depthStencilState;
    pipeline.pStages = m_shaderStages;
    pipeline.stageCount = 2;
    pipeline.renderPass = renderPass;
    pipeline.subpass = 0;

    res = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline, NULL, &m_pipeline);
    assert(res == VK_SUCCESS);

	return m_pipeline;
}


VkFormat VulkanRenderPipeline::GetVulkanFormat(VertElementType elementType, uint32 elementNum)
{
	switch (elementType)
	{
	    case Pharos::Render::VET_FLOAT32:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R32_SFLOAT;
				case 2: return VK_FORMAT_R32G32_SFLOAT;
				case 3: return VK_FORMAT_R32G32B32_SFLOAT;
				case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
		}
		break;
	    case Pharos::Render::VET_FLOAT16:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R16_SFLOAT;
				case 2: return VK_FORMAT_R16G16_SFLOAT;
				case 3: return VK_FORMAT_R16G16B16_SFLOAT;
				case 4: return VK_FORMAT_R16G16B16A16_SFLOAT;
			}
		}
		break;
	    case Pharos::Render::VET_SINT32:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R32_SINT;
				case 2: return VK_FORMAT_R32G32_SINT;
				case 3: return VK_FORMAT_R32G32B32_SINT;
				case 4: return VK_FORMAT_R32G32B32A32_SINT;
			}
		}
		break;
	    case Pharos::Render::VET_SINT16:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R16_SINT;
				case 2: return VK_FORMAT_R16G16_SINT;
				case 3: return VK_FORMAT_R16G16B16_SINT;
				case 4: return VK_FORMAT_R16G16B16A16_SINT;
			}
		}
		break;
	    case Pharos::Render::VET_SINT8:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R8_SINT;
				case 2: return VK_FORMAT_R8G8_SINT;
				case 3: return VK_FORMAT_R8G8B8_SINT;
				case 4: return VK_FORMAT_R8G8B8A8_SINT;
			}
		}
		break;
	    case Pharos::Render::VET_UINT32:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R32_UINT;
				case 2: return VK_FORMAT_R32G32_UINT;
				case 3: return VK_FORMAT_R32G32B32_UINT;
				case 4: return VK_FORMAT_R32G32B32A32_UINT;
			}
		}
		break;
	    case Pharos::Render::VET_UINT16:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R16_UINT;
				case 2: return VK_FORMAT_R16G16_UINT;
				case 3: return VK_FORMAT_R16G16B16_UINT;
				case 4: return VK_FORMAT_R16G16B16A16_UINT;
			}
		}
		break;
	    case Pharos::Render::VET_UINT8:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R8_UINT;
				case 2: return VK_FORMAT_R8G8_UINT;
				case 3: return VK_FORMAT_R8G8B8_UINT;
				case 4: return VK_FORMAT_R8G8B8A8_UINT;
			}
		}
		break;
	    case Pharos::Render::VET_SNORM16:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R16_SNORM;
				case 2: return VK_FORMAT_R16G16_SNORM;
				case 3: return VK_FORMAT_R16G16B16_SNORM;
				case 4: return VK_FORMAT_R16G16B16A16_SNORM;
			}
		}
		break;
	    case Pharos::Render::VET_SNORM8:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R8_SNORM;
				case 2: return VK_FORMAT_R8G8_SNORM;
				case 3: return VK_FORMAT_R8G8B8_SNORM;
				case 4: return VK_FORMAT_R8G8B8A8_SNORM;
			}
		}
		break;
	    case Pharos::Render::VET_UNORM16:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R16_UNORM;
				case 2: return VK_FORMAT_R16G16_UNORM;
				case 3: return VK_FORMAT_R16G16B16_UNORM;
				case 4: return VK_FORMAT_R16G16B16A16_UNORM;
			}
		}
		break;
	    case Pharos::Render::VET_UNORM8:
		{
			switch (elementNum)
			{
				case 1: return VK_FORMAT_R8_UNORM;
				case 2: return VK_FORMAT_R8G8_UNORM;
				case 3: return VK_FORMAT_R8G8B8_UNORM;
				case 4: return VK_FORMAT_R8G8B8A8_UNORM;
			}
		}
		break;
	}
	
	return VK_FORMAT_MAX_ENUM;
}

uint32 VulkanRenderPipeline::GetVertElementSize(VertElementType elementType, uint32 elementNum)
{
	uint32 elementBitNum = 0;
	
	switch (elementType)
	{
		case Pharos::Render::VET_FLOAT32:
		case Pharos::Render::VET_UINT32:
		case Pharos::Render::VET_SINT32:
			elementBitNum = 4;
		    break;
		case Pharos::Render::VET_FLOAT16:
		case Pharos::Render::VET_SINT16:
		case Pharos::Render::VET_UINT16:
		case Pharos::Render::VET_SNORM16:
		case Pharos::Render::VET_UNORM16:
			elementBitNum = 2;
			break;
		case Pharos::Render::VET_UINT8:
		case Pharos::Render::VET_SINT8:
		case Pharos::Render::VET_SNORM8:
		case Pharos::Render::VET_UNORM8:
			elementBitNum = 1;
			break;
	}
	
	return elementBitNum * elementNum;
}