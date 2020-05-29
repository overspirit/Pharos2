#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderPipeline::VulkanRenderPipeline(VkDevice device)
{
	m_device = device;

	m_pipeline = VK_NULL_HANDLE;
}

VulkanRenderPipeline::~VulkanRenderPipeline()
{
}

bool VulkanRenderPipeline::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{   
    m_vertexBinding.binding = 0;
    m_vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    m_vertexBinding.stride = 16;

    m_vertexAttribute[0].binding = 0;
    m_vertexAttribute[0].location = 0;
    m_vertexAttribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    m_vertexAttribute[0].offset = 0;

    m_vertexAttribute[1].binding = 0;
    m_vertexAttribute[1].location = 1;
    m_vertexAttribute[1].format = VK_FORMAT_B8G8R8A8_UNORM;//use_texture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
    m_vertexAttribute[1].offset = 12;

	memset(&m_vertInputState, 0, sizeof(m_vertInputState));
	m_vertInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertInputState.pNext = NULL;
	m_vertInputState.flags = 0;
	m_vertInputState.vertexBindingDescriptionCount = 1;
	m_vertInputState.pVertexBindingDescriptions = &m_vertexBinding;
	m_vertInputState.vertexAttributeDescriptionCount = 2;
	m_vertInputState.pVertexAttributeDescriptions = m_vertexAttribute;

	return true;
}

bool VulkanRenderPipeline::SetProgramShader(RenderProgram* program)
{
	VulkanShaderProgram* vulkanProgram =  static_cast<VulkanShaderProgram*>(program);

	m_shaderStages = vulkanProgram->GetShaderStage();

	return true;
}

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT
#define NUM_VIEWPORTS 1
#define NUM_SCISSORS NUM_VIEWPORTS

void VulkanRenderPipeline::SetRenderTarget(VkPipelineLayout pipelineLayout, VulkanRenderTarget* target)
{
	if (m_pipeline != VK_NULL_HANDLE) return;

	VkRenderPass renderPass = target->GetCurrRenderPass();


	VulkanRasterizerState rasterState;
	RasterizerStateDesc rasterdesc;
	rasterState.CreateState(rasterdesc);
	VkPipelineRasterizationStateCreateInfo rs = rasterState.GetStateCreateInfo();


	VulkanBlendState blendState;
	BlendStateDesc	blendDesc;
	blendState.CreateState(blendDesc);
	VkPipelineColorBlendStateCreateInfo cb = blendState.GetStateCreateInfo();	


    VulkanDepthStencilState depthStencilState;
	DepthStencilStateDesc	depthStencilDesc;
	depthStencilState.CreateState(depthStencilDesc);
	VkPipelineDepthStencilStateCreateInfo ds = depthStencilState.GetStateCreateInfo();



    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
	memset(dynamicStateEnables, 0, sizeof(dynamicStateEnables));

    VkPipelineDynamicStateCreateInfo dynamicState = {};    
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;


	VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	
    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = NUM_VIEWPORTS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = NUM_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
    vp.pScissors = NULL;
    vp.pViewports = NULL;



    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = NUM_SAMPLES;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;

    VkGraphicsPipelineCreateInfo pipeline;
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.pNext = NULL;
    pipeline.layout = pipelineLayout;
    pipeline.basePipelineHandle = VK_NULL_HANDLE;
    pipeline.basePipelineIndex = 0;
    pipeline.flags = 0;
    pipeline.pVertexInputState = &m_vertInputState;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &rs;
    pipeline.pColorBlendState = &cb;
    pipeline.pTessellationState = NULL;
    pipeline.pMultisampleState = &ms;
    pipeline.pDynamicState = &dynamicState;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &ds;
    pipeline.pStages = m_shaderStages;
    pipeline.stageCount = 2;
    pipeline.renderPass = renderPass;
    pipeline.subpass = 0;

    VkResult res = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline, NULL, &m_pipeline);
    assert(res == VK_SUCCESS);
	
}

void VulkanRenderPipeline::SetBlendState(RenderBlendState* state)
{

}

void VulkanRenderPipeline::SetRasterizerState(RenderRasterizerState* state)
{

}

void VulkanRenderPipeline::SetDepthStencilState(RenderDepthStencilState* state)
{

}
