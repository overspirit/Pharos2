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

			VkPipeline			m_pipeline;
			VkPipelineLayout	m_pipelineLayout;

			DrawType		m_drawType;

		private:
			VkFormat GetVulkanFormat(VertElementType elementType, uint32 elementNum);
			uint32 GetVertElementSize(VertElementType elementType, uint32 elementNum);	

		public:			
			VkPipelineLayout MakeVulkanPipelineLayout(VkDescriptorSetLayout descSetLayout);
			VkPipeline MakeVulkanPipeline(VkRenderPass renderPass);

		public:
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);

			virtual bool SetProgramShader(RenderProgram* program);			

			virtual void SetBlendState(const BlendStateDesc& state);
			virtual void SetRasterizerState(const RasterizerStateDesc& state);
			virtual void SetDepthStencilState(const DepthStencilStateDesc& state);

			virtual void SetDrawType(DrawType drawType) { m_drawType = drawType; }		
		};
	}
}
