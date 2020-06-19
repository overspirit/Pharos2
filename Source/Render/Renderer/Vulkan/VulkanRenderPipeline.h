#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderPipeline : public RenderPipeline
		{
		public:
			VulkanRenderPipeline(VkDevice device);
			virtual ~VulkanRenderPipeline();

		private:
			VkDevice	m_device;

			VkPipelineShaderStageCreateInfo*		m_shaderStages;
			
			vector<VkVertexInputBindingDescription>			m_vertexBindings;
			vector<VkVertexInputAttributeDescription>    	m_vertexAttributes;
			
			VkPipelineVertexInputStateCreateInfo			m_vertInputState;
			VkPipelineRasterizationStateCreateInfo			m_rasterState;
			VkPipelineColorBlendAttachmentState				m_attachmentState[8];
			VkPipelineColorBlendStateCreateInfo				m_blendState;	
			VkPipelineDepthStencilStateCreateInfo			m_depthStencilState;

			VkPipeline		m_pipeline;

		private:
			VkFormat GetVulkanFormat(VertElementType elementType, uint32 elementNum);
			uint32 GetVertElementSize(VertElementType elementType, uint32 elementNum);

		public:
			VkPipeline GetVulkanPipeline(VkPrimitiveTopology prim, VkPipelineLayout pipelineLayout, VkRenderPass renderPass);

		public:
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);

			virtual bool SetProgramShader(RenderProgram* program);			

			virtual void SetBlendState(RenderBlendState* state);
			virtual void SetRasterizerState(RenderRasterizerState* state);
			virtual void SetDepthStencilState(RenderDepthStencilState* state);
		};
	}
}
